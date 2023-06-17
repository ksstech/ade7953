/*
 * ade7953.c - Copyright (c) 2023 Andre M. Maree / KSS Technologies (Pty) Ltd.
 * References:
 * https://devices.esphome.io/devices/Shelly-Plus-2PM
 * https://github.com/arendst/Tasmota/blob/development/tasmota/tasmota_xnrg_energy/xnrg_07_ade7953.ino
 * https://kb.shelly.cloud/knowledge-base/shelly-plus-2pm#Usermanualscommonparts-Installationguides
 * https://github.com/mongoose-os-libs/ade7953
 *
 * Notes & references:
 * Add IRQ handler, use to check for overload and/or overheat, turn off relays
 * mode option to select WHr or KWHr, set default, variable used as divisor
 */

#include "hal_variables.h"

#if (halHAS_ADE7953 > 0)
#include "ade7953.h"
#include "hal_storage.h"
#include "FreeRTOS_Support.h"
#include "options.h"
#include "printfx.h"
#include "syslog.h"
#include "systiming.h"					// timing debugging
#include "x_errors_events.h"

#include "nvs.h"

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

//	#if (ade7953USE_I2C == 1)
//	#elif (ade7953USE_SPI == 1)
//	#endif

// ####################################### Private variables #######################################


// ####################################### Public variables ########################################

u8_t NumADE7953 = 0;
ade7953_t sADE7953[halHAS_ADE7953] = { 0 };
ade7953nvs_t ade7953nvs[halHAS_ADE7953] = { 0 };
const ade7953nvs_t ade7953Def = { 0 };

// ############################### common support routines #########################################

int ade7953CalcRegSize(u16_t Reg) {
	if (Reg == ade7953REG_VERSION || Reg == ade7953REG_EX_REF || Reg < 0x100)
		return 1;
	if (Reg < 0x200)
		return 2;
	if (Reg < 0x300)
		return 3;
	return 4;
}

int ade7953Write(ade7953_t * psADE7953, u16_t Reg, i32_t Val) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953));
	int iRV, Size = ade7953CalcRegSize(Reg);
	#if (ade7953USE_I2C == 1)
	iRV = ade7953WriteI2C(psADE7953, Reg, Size, Val);
	#elif (ade7953USE_SPI == 1)
	iRV = ade7953WriteSPI(psADE7953, Reg, Size, Val);
	#endif
	if ((iRV < erSUCCESS) && !(Reg == ade7953REG_CONFIG && (Val & ade7953REG_CONFIG_SWRST)))
		SL_ERR("Error Reg=0x%X iRV=%d", Reg, iRV);
	return iRV;
}

int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pVal) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953));
	int iRV, Size = ade7953CalcRegSize(Reg);
	u8_t Data[4] = {0};
#if (ade7953USE_I2C == 1)
	iRV = ade7953ReadI2C(psADE7953, Reg, Size, Data);
#elif (ade7953USE_SPI == 1)
	iRV = ade7953ReadSPI(psADE7953, Reg, Size, Data);
#endif
	return iRV;
}

/*
 * @brief	Callback handler after reading both IRQSTATA & IRQSTATB
 */
void IRAM_ATTR ade7953IRQ_CB(void * Arg) {
	ade7953_t * psADE7953 = (ade7953_t *) Arg;
	psADE7953->CallBack = NULL;
	ade7953ReportStatus(NULL, psADE7953);
}

/**
 * @brief
 * @param
 * @return
 **/
void IRAM_ATTR ade7953IntHandler(void * Arg) {
	u8_t eDev = (int) Arg;
	IF_myASSERT(debugPARAM, eDev < halHAS_ADE7953);
	ade7953_t * psADE7953 = &sADE7953[eDev];
	psADE7953->CallBack = ade7953IRQ_CB;
	ade7953Read(psADE7953, ade7953REG_IRQSTATA, &psADE7953->sIRQSTATA);
}

/*int	ade7953LoadNVSConfig(u8_t eChan, u8_t Idx) {
	IF_myASSERT(debugPARAM, Idx < ade7953CALIB_NUM);
	size_t	SizeBlob = ade7953CALIB_NUM * sizeof(ade7953nvs_t);
	ade7953nvs_t * psCalib = pvRtosMalloc(SizeBlob);
	int iRV = halSTORAGE_ReadBlob(halSTORAGE_STORE, halSTORAGE_KEY_ADE7953, psCalib, &SizeBlob, ESP_OK);
	if (iRV == erSUCCESS) {
		//	Add code
	} else {
		SL_ERR("Failed Ch=%d config=%d", eChan, Idx);
	}
	vRtosFree (psCalib);
	return iRV;
}*/

/*u8_t ade7953CalcInfo(epw_t * psEW) {
	xEpWorkToUri(psEW);
	psEW->idx = psEW->uri - URI_ADE7953_E_ACT_FWD_0;
	psEW->eChan = 0;
	// In case of multiple channels, adjust till in range....
	while (psEW->idx >= ADE7953_NUMURI_0) {
		psEW->idx -= ADE7953_NUMURI_0;
		psEW->eChan++;
	}
	IF_myASSERT(debugRESULT, (psEW->idx < ADE7953_NUMURI_0) && (psEW->eChan < halHAS_ADE7953));
	return psEW->idx;
}*/

// ############################## identification & initialization ##################################

int	ade7953Identify(void * pVoid) {
	ade7953_t * psADE7953 = &sADE7953[NumADE7953];
	psADE7953->pin = ade7953IRQ_PIN1;
	gpio_config_t irq_pin_cfg = {
		.pin_bit_mask = 1ULL << ade7953IRQ_PIN1,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE ,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_NEGEDGE,
	};
	ESP_ERROR_CHECK(gpio_config(&irq_pin_cfg));
	halGPIO_IRQconfig(psADE7953->pin, ade7953IntHandler, psADE7953);

#if (ade7953USE_I2C == 1)
	psADE7953->psI2C = pVoid;
#elif (ade7953USE_SPI == 1)
	psADE7953->hndl = pVoid;
#endif
	int iRV;
	u8_t Ver = 0;
	iRV = ade7953Read(psADE7953, ade7953REG_VERSION, &Ver);
	if (iRV < erSUCCESS) {
		SL_ERR("Device Failed!!!");
	} else {
		IF_SL_INFO(debugTRACK, "Silicon Version=0x%0hhX", Ver);
	}
	return iRV;
}

/**
 * @brief
 * @param
 * @return
 */
int ade7953Config(void * pVoid) {
	ade7953_t * psADE7953 = &sADE7953[NumADE7953];
	/* Check that blob with CALibration and ADJustment values exists
	 * If not existing, create with factory defaults as first record */
	size_t	SizeBlob = ade7953CALIB_NUM * sizeof(ade7953nvs_t);
	ade7953nvs_t * psCalib = pvRtosMalloc(SizeBlob);
	int iRV = halSTORAGE_ReadBlob(halSTORAGE_STORE, halSTORAGE_KEY_ADE7953, psCalib, &SizeBlob, ESP_ERR_NVS_NOT_FOUND);
	if ((iRV != erSUCCESS) || (SizeBlob != (ade7953CALIB_NUM * sizeof(ade7953nvs_t)))) {
		memset(psCalib, 0, SizeBlob = ade7953CALIB_NUM * sizeof(ade7953nvs_t));
		memcpy(psCalib, &ade7953Def, sizeof(ade7953nvs_t));
		iRV = halSTORAGE_WriteBlob(halSTORAGE_STORE, halSTORAGE_KEY_ADE7953, psCalib, SizeBlob);
		IF_myASSERT(debugRESULT, iRV == erSUCCESS);
	} else {
		memcpy(&psADE7953->sCurCalib, &psCalib[0], sizeof(ade7953nvs_t));
	}
	vRtosFree(psCalib);

	iRV = ade7953Write(psADE7953, ade7953REG_CONFIG, ade7953REG_CONFIG_SWRST);
    do {
		vTaskDelay(pdMS_TO_TICKS(10));
    } while (!ade7953Read(psADE7953, ade7953REG_IRQSTATA, &psADE7953->sIRQSTATA) || !(psADE7953->sIRQSTATA.val & ade7953REG_IRQSTATA_RESET));

    ade7953Write(psADE7953, ade7953REG_CONFIG, 0x04);	// Lock comms interface, enable high pass filter
    ade7953Write(psADE7953, ade7953REG_UNLOCK, 0xAD);	// Unlock reg 0x0120
    ade7953Write(psADE7953, ade7953REG_OPTIMUM, 0x30);	// enable optimum settings

	DevIDflag |= 1 << devID_ADE7953;
	IF_SYSTIMER_INIT(debugTIMING, stADE7953R, stMICROS, "ADE7953RD", 1500, 15000);
	IF_SYSTIMER_INIT(debugTIMING, stADE7953W, stMICROS, "ADE7953WR", 1500, 15000);

	return (ade7953ReadConfig(psADE7953) & 0x8000) ? erSUCCESS : erFAILURE;
}

int ade7953ReConfig(void * pVoid) { return ade7953Config(pVoid); }

// ###################################### general support ##########################################

u16_t ade7953ReadConfig(ade7953_t * psADE7953) {
	ade7953Read(psADE7953, ade7953REG_CONFIG, &psADE7953->sCONFIG.val);
	return psADE7953->sCONFIG.val;
}

// ############################### device reporting functions ######################################

void ade7953ReportCalib(void) { }

void ade7953ReportAdjust(void) { }

void ade7953ReportData(void) { }

void ade7953ReportStatus(report_t * psRprt, ade7953_t * psADE7953) {
	wprintfx(psRprt, "IRQA: EN=0x%06X STAT==0x%06X", psADE7953->sIRQENA, psADE7953->sIRQSTATA);
	wprintfx(psRprt, "IRQB: EN=0x%06X STAT==0x%06X", psADE7953->sIRQENB, psADE7953->sIRQSTATB);
}

void ade7953Report(void) { }
#endif	// halHAS_ADE7953
