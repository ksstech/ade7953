/*
 * ade7953_spi.c - Copyright (c) 2023 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#include "hal_variables.h"

#if (ade7953USE_SPI > 0)
#include "ade7953.h"
#include "hal_spi.h"
#include "hal_storage.h"
#include "FreeRTOS_Support.h"
#include "options.h"
#include "printfx.h"
#include "syslog.h"
#include "systiming.h"					// timing debugging
#include "x_errors_events.h"

// ######################################## Build macros ###########################################

#define	debugFLAG					0xF000

#define	debugREAD					(debugFLAG & 0x0001)
#define	debugWRITE					(debugFLAG & 0x0002)
#define	debugRMW					(debugFLAG & 0x0004)
#define	debugCURRENT				(debugFLAG & 0x0008)
#define	debugPOWER					(debugFLAG & 0x0010)

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ########################################### Macros ##############################################



// ###################################### Private variables #######################################

spi_device_interface_config_t ade7953spi = {
	.command_bits		= 0,
	.address_bits		= 0,
	.dummy_bits			= 0,
	.mode				= 3,							// only SPI mode 3 supported
	.duty_cycle_pos		= 128,							// same as 0 = 50/50% duty cycle
	.cs_ena_pretrans 	= 0,
	.cs_ena_posttrans	= 0,
	.clock_speed_hz		= 1200000,
	.input_delay_ns		= 0,
	.spics_io_num		= GPIO_NUM_5,					// D8 = CS pin
	.flags				= 0,
	.queue_size			= 16,
	.pre_cb				= 0,							// no callback handler
	.post_cb			= 0,
};


// ############################### common support routines #########################################

void ade7953WriteU16(u8_t eChan, u8_t address, u16_t val) {
	IF_myASSERT(debugPARAM, address < 0x70 && ade7953_handle[eChan]);
	xRtosSemaphoreTake(&ade7953mutex[eChan], portMAX_DELAY);
	spi_transaction_t ade7953_buf;
	memset(&ade7953_buf, 0, sizeof(ade7953_buf));
	ade7953_buf.length		= 8 * 3;
	ade7953_buf.flags 		= SPI_TRANS_USE_TXDATA;
	ade7953_buf.tx_data[0]	= address;
	ade7953_buf.tx_data[1]	= val >> 8;
	ade7953_buf.tx_data[2]	= val & 0xFF;
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953W);
	ESP_ERROR_CHECK(spi_device_transmit(ade7953_handle[eChan], &ade7953_buf));
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953W);
	xRtosSemaphoreGive(&ade7953mutex[eChan]);
	IF_P(debugWRITE, "TX: addr=%02x d0=%02x d1=%02x\r\n", ade7953_buf.tx_data[0], ade7953_buf.tx_data[1], ade7953_buf.tx_data[2]);
}

u16_t ade7953ReadU16(u8_t eChan, u8_t address) {
	IF_myASSERT(debugPARAM, address < 0x70 && ade7953_handle[eChan]);
	xRtosSemaphoreTake(&ade7953mutex[eChan], portMAX_DELAY);
	spi_transaction_t ade7953_buf;
	memset(&ade7953_buf, 0, sizeof(ade7953_buf));
	ade7953_buf.length		= 8 * 3;
	ade7953_buf.flags 		= SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
	ade7953_buf.tx_data[0]	= address | 0x80;
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953R);
	ESP_ERROR_CHECK(spi_device_transmit(ade7953_handle[eChan], &ade7953_buf));
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953R);
	xRtosSemaphoreGive(&ade7953mutex[eChan]);
	IF_P(debugREAD, "RX: addr=%02x  d0=%02x  d1=%02x  dx=%04x\r\n", ade7953_buf.tx_data[0], ade7953_buf.rx_data[1], ade7953_buf.rx_data[2], (ade7953_buf.rx_data[1] << 8) | ade7953_buf.rx_data[2]);
	return (ade7953_buf.rx_data[1] << 8) | ade7953_buf.rx_data[2];
}

void ade7953WriteRegister(u8_t eChan, u8_t Reg, u16_t Val) {
	if (INRANGE(PLconstH, Reg, MET_MODE)) {
		if (ade7953ReadU16(eChan, CALSTART) != CODE_START) {
			SL_NOT("CALSTART (x20) in wrong state, must be x5678");
		} else {
			ade7953WriteU16(eChan, Reg, Val);				// write new value & update CRC
			ade7953WriteU16(eChan, CRC_1, ade7953ReadU16(eChan, CRC_1));
			IF_PX(debugTRACK && ioB1GET(ioM90write), "Before: #%d %'-hY\r\n", Reg-PLconstH, SO_MEM(nvs_ade7953_t, calreg), &ade7953nvs[eChan].calreg);
			ade7953nvs[eChan].calreg[Reg-PLconstH] = Val;
			IF_PX(debugTRACK && ioB1GET(ioM90write), "After : #%d %'-hY\r\n", Reg-PLconstH, SO_MEM(nvs_ade7953_t, calreg), &ade7953nvs[eChan].calreg);
		}
	} else if (INRANGE(U_GAIN, Reg, Q_OFST_N)) {
		if (ade7953ReadU16(eChan, ADJSTART) != CODE_START) {
			SL_NOT("ADJSTART (x30) in wrong state, must be x5678");
		} else {
			ade7953WriteU16(eChan, Reg, Val);				// write new value & update CRC
			ade7953WriteU16(eChan, CRC_2, ade7953ReadU16(eChan, CRC_2));
			IF_PX(debugTRACK && ioB1GET(ioM90write), "Before: #%d %'-hY\r\n", Reg-U_GAIN, SO_MEM(nvs_ade7953_t, adjreg), &ade7953nvs[eChan].adjreg);
			ade7953nvs[eChan].adjreg[Reg-U_GAIN] = Val;
			IF_PX(debugTRACK && ioB1GET(ioM90write), "After : #%d %'-hY\r\n", Reg-U_GAIN, SO_MEM(nvs_ade7953_t, adjreg), &ade7953nvs[eChan].adjreg);
		}
	} else if (Reg == SOFTRESET || INRANGE(FUNC_ENAB, Reg, POWER_MODE) || Reg == CALSTART || Reg == ADJSTART) {
		ade7953WriteU16(eChan, Reg, Val);				// write new value
		if (INRANGE(FUNC_ENAB, Reg, POWER_MODE)) {
			IF_PX(debugTRACK && ioB1GET(ioM90write), "Before: #%d %'-hY\r\n", Reg-FUNC_ENAB, SO_MEM(nvs_ade7953_t, cfgreg), &ade7953nvs[eChan].cfgreg);
			ade7953nvs[eChan].cfgreg[Reg-FUNC_ENAB] = Val;
			IF_PX(debugTRACK && ioB1GET(ioM90write), "After : #%d %'-hY\r\n", Reg-FUNC_ENAB, SO_MEM(nvs_ade7953_t, cfgreg), &ade7953nvs[eChan].cfgreg);
		}
	} else {
		SL_ERR("Invalid register=0x%02X", Reg);
	}
}

u16_t ade7953ReadModifyWrite(u8_t eChan, u8_t Addr, u16_t Value, u16_t Mask) {
	IF_P(debugRMW, "  C=%d  R=%d  &=x%04X  |=x%04X", eChan, Addr, Value, Mask);
	u16_t CurValue = ade7953ReadU16(eChan, Addr);
	IF_P(debugRMW, "  V=x%04X", CurValue);
	CurValue &= ~Mask;
	IF_P(debugRMW, " -> x%04X", CurValue);
	CurValue |= Value;
	IF_P(debugRMW, " -> x%04X\r\n", CurValue);
	ade7953WriteRegister(eChan, Addr, CurValue);
	return CurValue;
}

s16_t ade7953ReadI16S(u8_t eChan, u8_t Reg) {
	u16_t RawVal = ade7953ReadU16(eChan, Reg);
	bool	Sign	= RawVal & 0x8000 ? true : false;
	s16_t	i16Val	= RawVal & 0x7FFF;
	s16_t	ConVal	= Sign == true ? -1 * i16Val : i16Val;
	return ConVal;
}

s16_t ade7953ReadI16TC(u8_t eChan, u8_t Reg) { return ~ade7953ReadU16(eChan, Reg) + 1; }

u32_t ade7953ReadU32(u8_t eChan, u8_t Reg) {
	#if	(ade7953READ_32BIT == 1)
	return (ade7953ReadU16(eChan, Reg) << 16) + ade7953ReadU16(eChan, LSB);
	#else
	return (ade7953ReadU16(eChan, Reg) << 16);
	#endif
}

i32_t ade7953ReadI32TC(u8_t eChan, u8_t Reg) { return ~ade7953ReadU32(eChan, Reg) + 1; }

// ############################## identification & initialization ##################################

int	ade7953Identify(u8_t eChan) { return erSUCCESS; }

int ade7953Config(void) { return erSUCCESS; }

int	ade7953Init(u8_t eChan) { return erSUCCESS; }

#endif	// ade7953USE_SPI
