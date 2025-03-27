// ade7953.c - Copyright (c) 2023-24 Andre M. Maree / KSS Technologies (Pty) Ltd.

#include "hal_platform.h"

#if (HAL_ADE7953 > 0)
#include "ade7953.h"
#include "hal_i2c_common.h"
#include "hal_memory.h"
#include "hal_options.h"
#include "hal_flash.h"
#include "report.h"
#include "syslog.h"
#include "systiming.h"					// timing debugging
#include "errors_events.h"
#include "string_general.h"

#include "nvs.h"
#include <string.h>

// ######################################## Build macros ###########################################

#define	debugFLAG					0xF000

#define	debugTIMING					(debugFLAG_GLOBAL & debugFLAG & 0x1000)
#define	debugTRACK					(debugFLAG_GLOBAL & debugFLAG & 0x2000)
#define	debugPARAM					(debugFLAG_GLOBAL & debugFLAG & 0x4000)
#define	debugRESULT					(debugFLAG_GLOBAL & debugFLAG & 0x8000)

// ########################################### Macros ##############################################
// ######################################### Structures ############################################
// ####################################### Private variables #######################################

//				V_Ofst		I_Ofst		V_Scale		Iscale		Pscale		Escale
//	ESPhome								26000.0		100000.0	154.0		????
//	TasMota								26000.0		100000.0	154.0
//	Mongoose	-0.068		-0.017		26137.0		105318.59	164.0		25240.0

#define	V_Ofst	-0.068
#define	I_Ofst	-0.017
#define	V_Scale	26000.0
#define	I_Scale	10000.0
#define	P_Scale	154.0
#define	E_Scale	25240.0

// ###################################### Private constants ########################################

static const u16_t aRegCFG[ade7953NUM_CHAN][6] = {
	{ regAIGAIN, regAVGAIN, regAWGAIN, regAVARGAIN, regAVAGAIN, regPHCALA },
	#if	(ade7953USE_CH2 > 0)
	{ regBIGAIN, regBVGAIN, regBWGAIN, regBVARGAIN, regBVAGAIN, regPHCALB },
	#endif
};

static const u16_t aRegDNL[3] = {regAP_NOLOAD, regVAR_NOLOAD, regVA_NOLOAD };

// ######################################### Structures ############################################

const ade7953_defaults_t sADE7953Defaults = {
	//				 IGAIN	VGAIN	WGAIN  VARGAIN	VAGAIN	PHCAL
	.cal 		= { 0, 		0, 		0, 		0, 		0, 		0},
	//				AP VAR VA
	.dnl 		= { 0, 0, 0 },
	// Factors used for conversion of I32 to F32
	.Vgain		= ade7953_GAIN_1,
	.Vofst		= V_Ofst,
	.Vscale		= V_Scale,
	.Igain[0]	= ade7953_GAIN_1,
	.Iofst[0]	= I_Ofst,
	.Iscale[0]	= I_Scale,
	.Pscale[0]	= P_Scale,
	.Escale[0]	= E_Scale,
	#if	(ade7953USE_CH2 > 0)
	.Igain[1]	= ade7953_GAIN_1,
	.Iofst[1]	= I_Ofst,
	.Iscale[1]	= I_Scale,
	.Pscale[1]	= P_Scale,
	.Escale[1]	= E_Scale,
	#endif
};

// ###################################### Private variables ########################################

static ade7953_defaults_t sADE7953CfgBuf;

// ####################################### Public variables ########################################

u8_t NumADE7953;
ade7953_t sADE7953[HAL_ADE7953];

// ############################### common support routines #########################################

/**
 * @brief	Calculate size of specified register (in bytes)
 * @return	Size of specified register (in bytes)
 */
int ade7953CalcRegSize(u16_t Reg) {
	if (Reg < 0x100 || Reg == regVERSION || Reg == regEX_REF) return 1;
	if (Reg < 0x200) return 2;
	if (Reg < 0x300) return 3;
	return 4;
}

/**
 * @brief	Calculate the integer value of a register based on the varying size
 * @return	value as unsigned integer...
*/
i32_t ade7953CalcRegValue(u16_t Reg, u8_t * pU8) {
	int Size = ade7953CalcRegSize(Reg);
	i32_t I32 = 0;
	for (int i = 0; i < Size; i++) I32 += pU8[i] << (i * 8);
	return I32;
}

i32_t ade7953CalcSign(i32_t I32, int Size) {
	u32_t sign_mask = 0;
	if (Size == 1) sign_mask = (1 << 7);
	if (Size == 2) sign_mask = (1 << 15);
	if (Size == 3) sign_mask = (1 << 23);
	if (Size == 4) sign_mask = (1 << 31);
	if ((I32 & sign_mask) != 0) {
		I32 &= ~sign_mask;
		I32 |= (1 << 31);
	}
	return I32;
}

/**
 * @brief	Write preformatted buffer contents to specified register
 * @return	number of data bytes written (if no error) or error code
*/
int ade7953Write(ade7953_t * psADE7953, u16_t Reg, void * pV) {
	IF_myASSERT(debugPARAM, halMemorySRAM(psADE7953) && halMemoryANY(pV));
	int Size = ade7953CalcRegSize(Reg);
	u8_t caBuf[6] = { (Reg >> 8) & 0xFF, Reg & 0xFF };
	memcpy(&caBuf[2], pV, Size);						// Add register content to address in buffer
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953W);
	int iRV = halI2C_Queue(psADE7953->psI2C, i2cW_B, caBuf, Size + 2, NULL, 0, (i2cq_p1_t)NULL, (i2cq_p2_t)NULL);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953W);
	if ((iRV < erSUCCESS) && !(Reg == regCONFIG && (caBuf[2] & regCONFIG_SWRST)))
		SL_ERR("Error Reg=0x%X iRV=%d", Reg, iRV);
	return iRV < erSUCCESS ? iRV : Size;
}

/**
 * @brief	Write integer value to specified register (optionally update local storage)
 * @return	number of data bytes written (if no error) or error code
*/
int ade7953WriteValue(ade7953_t * psADE7953, u16_t Reg, void * pV, i32_t I32) {
	IF_myASSERT(debugPARAM, halMemorySRAM(psADE7953));
	int Size = ade7953CalcRegSize(Reg);
	u8_t caBuf[4];
	int Len = 0;
	while (Size--) caBuf[Len++] = (I32 >> (8 * Size)) & 0xFF;	// correct BE -> LE conversion
	if (pV) memcpy(pV, caBuf, Len);								// Update local memory store
	return ade7953Write(psADE7953, Reg, caBuf);
}

/**
 * @brief	integer value from specified register
 * @return	number of data bytes read (register length) or error code
*/
int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pV) {
	IF_myASSERT(debugPARAM, halMemorySRAM(psADE7953) && halMemorySRAM(pV));
	u8_t caBuf[2] = { (Reg >> 8) & 0xFF,  Reg & 0xFF };
	int Size = ade7953CalcRegSize(Reg);

	// depending on whether a callback address is set, select I2C opcode
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953R);
	int iRV = halI2C_Queue(psADE7953->psI2C, psADE7953->cb ? i2cWRC_F : i2cWR_B, caBuf,
		sizeof(caBuf), pV, Size, (i2cq_p1_t)psADE7953->cb, (i2cq_p2_t) (void *)psADE7953);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953R);
	if (iRV < erSUCCESS) return iRV;
	if (Size > 1) xmemrev(pV, Size);
	return Size;
}

/**
 * @brief	sensor value ([un]signed integer)
 * @return	if no error, number of bytes read else erFAILURE
*/
int ade7953ReadValue(ade7953_t * psADE7953, u16_t Reg, void * pV, i32_t * pI32, bool bSign) {
	u8_t caData[4];
	if (!pV) pV = caData;
	int iRV = ade7953Read(psADE7953, Reg, pV);
	if (iRV < erSUCCESS) return erFAILURE;
	*pI32 = ade7953CalcRegValue(Reg, pV);				// Convert to value
	if (bSign) *pI32 = ade7953CalcSign(*pI32, iRV);		// fix sign if required
	return iRV;
}

int ade7953Update(ade7953_t * psADE7953, u16_t Reg, void * pV, u32_t maskAND, u32_t maskOR) {
	int Size = ade7953CalcRegSize(Reg);
	u32_t maskMAX = 0xFFFFFFFF >> ((4 - Size) * 8);
	maskAND &= maskMAX;
	maskOR &= maskMAX;
	int iRV = erFAILURE;
	if (maskAND || maskOR) {
		u8_t caBuf[2] = { (Reg >> 8) & 0xFF, Reg & 0xFF };
		iRV = halI2C_Queue(psADE7953->psI2C, i2cWRMW_BS, caBuf, sizeof(caBuf), pV, Size, (i2cq_p1_t) maskAND, (i2cq_p2_t) maskOR);
	}
	return iRV;
}

u16_t ade7953ReadConfig(ade7953_t * psADE7953) {
	int iRV = ade7953Read(psADE7953, regCONFIG, &psADE7953->config.val);
	return (iRV < erSUCCESS) ? 0 : psADE7953->config.val;
}

int ade7953SoftReset(ade7953_t * psADE7953) {
	psADE7953->psI2C->Test = 1;							// disable handling error we expect to get
	psADE7953->config.val = regCONFIG_SWRST;
	int iRV = ade7953Write(psADE7953, regCONFIG, &psADE7953->config.val);
	if (iRV < erSUCCESS) goto exit;
	psADE7953->psI2C->Test = 0;							// re-enable error handling

	u32_t Retries = 50;
	do {
		vTaskDelay(pdMS_TO_TICKS(10));
		if (!--Retries) {
			iRV = erTIMEOUT;
			break;
		}
		iRV = ade7953Read(psADE7953, regIRQSTATA, &psADE7953->is_a);	// regRSTIRQSTATA
	} while (iRV < erSUCCESS || (psADE7953->is_a.RESET == 0));
exit:
	return iRV;
}

// ######################################### IRQ support ###########################################

/*
 * @brief	Callback handler after reading both IRQSTATA & IRQSTATB
 * @param	pointer to ADE7953 device structure
 */
void IRAM_ATTR ade7953IntCB(void * Arg) {
	ade7953ReportIRQs(NULL, Arg);
}

/**
 * @brief
 * @param
 * @return
 **/
void IRAM_ATTR ade7953IntHandler(void * Arg) {
	IF_myASSERT(debugPARAM, (int) Arg < HAL_ADE7953);
	PXTL(); ade7953_t * psADE7953 = &sADE7953[(int) Arg];

	#if(ade7953USE_CH2 == 0)
	psADE7953->cb = &ade7953IRQ_CB;						// schedule Chan-A read with CB handler
	ade7953Read(psADE7953, regRSTIRQSTATA, &psADE7953->is_a);
	#else
	psADE7953->cb = NULL;								// schedule Chan-A read to update in SRAM
	ade7953Read(psADE7953, regRSTIRQSTATA, &psADE7953->is_a);
	psADE7953->cb = &ade7953IntCB;						// schedule Chan-B read with CB handler
	ade7953Read(psADE7953, regRSTIRQSTATB, &psADE7953->is_b);
	#endif

}

int ade7953ChangeIntMask(ade7953_t * psADE7953, int eCh, u32_t maskAND, u32_t maskOR) {
	u16_t Reg;
	void * pV;
	if (eCh == 0) {
		Reg = regIRQENA;
		pV = &psADE7953->ie_a;
	} else {
		Reg = regIRQENB;
		pV = &psADE7953->ie_b;
	}
	return ade7953Update(psADE7953, Reg, pV, maskAND, maskOR);
}

void ade7953IntConfig(int DevIdx) {
	gpio_config_t irq_pin_cfg = { .pin_bit_mask = (1ULL << ade7953PIN_IRQ), .mode = GPIO_MODE_INPUT,
//		.pull_up_en = GPIO_PULLUP_DISABLE, .pull_down_en = GPIO_PULLDOWN_ENABLE, .intr_type = GPIO_INTR_ANYEDGE,
//		.pull_up_en = GPIO_PULLUP_ENABLE, .pull_down_en = GPIO_PULLDOWN_DISABLE, .intr_type = GPIO_INTR_ANYEDGE,
		.pull_up_en = GPIO_PULLUP_ENABLE, .pull_down_en = GPIO_PULLDOWN_DISABLE, .intr_type = GPIO_INTR_NEGEDGE,
	};
	ESP_ERROR_CHECK(gpio_config(&irq_pin_cfg));
	halGPIO_IRQconfig(ade7953PIN_IRQ, ade7953IntHandler, (void *) DevIdx);
}

// ##################################### Configuration support #####################################

/**
 * @brief
 * @return
*/
int ade7953LoadNVSCalib(u8_t Idx) {
	IF_myASSERT(debugPARAM, Idx < ade7953_NUM_CONFIGS);
	size_t Size = ade7953_NUM_CONFIGS * sizeof(ade7953_defaults_t);
	ade7953_defaults_t * psCal = malloc(Size);
	int iRV = halFlashReadBlob(halFLASH_STORE, ade7953STORAGE_KEY, psCal, &Size);
	if ((iRV != erSUCCESS) || (Size != (ade7953_NUM_CONFIGS * sizeof(ade7953_defaults_t)))) {
		memset(psCal, 0, Size = ade7953_NUM_CONFIGS * sizeof(ade7953_defaults_t));// Clear blob memory
		memcpy(psCal, &sADE7953Defaults, sizeof(sADE7953CfgBuf));		// Reset 1st dataset to defaults
		iRV = halFlashWriteBlob(halFLASH_STORE, ade7953STORAGE_KEY, psCal, Size);
		IF_myASSERT(debugRESULT, iRV == erSUCCESS);
		SL_WARN("NVS config (%d) not found, default created/loaded", Idx);
		Idx = 0;
	}
	memcpy(&sADE7953CfgBuf, psCal+Idx, sizeof(sADE7953CfgBuf));
	free(psCal);
	return iRV;
}

/**
 * @brief
 * @return
*/
int ade7953SetOffset(ade7953_t * psADE7953) {
	// check ALL calculation since change of reciprocal in initialised variable at top
	int iRV = ade7953WriteValue(psADE7953, regVRMSOS, &psADE7953->ofst.v, sADE7953Defaults.Vofst / sADE7953Defaults.Vscale);
	if (iRV > erFAILURE)
		iRV = ade7953WriteValue(psADE7953, regAIRMSOS, &psADE7953->ofst.ia, sADE7953Defaults.Iofst[0] / sADE7953Defaults.Iscale[0]);
	#if	(ade7953USE_CH2 > 0)
	if (iRV > erFAILURE)
		iRV = ade7953WriteValue(psADE7953, regBIRMSOS, &psADE7953->ofst.ib, sADE7953Defaults.Iofst[1] / sADE7953Defaults.Iscale[1]);
	#endif
	return iRV;
}

/**
 * @brief
 * @return
*/
int ade7953SetGain(ade7953_t * psADE7953) {
	int iRV = ade7953WriteValue(psADE7953, regPGA_V, &psADE7953->pga.v, sADE7953Defaults.Vgain);
	if (iRV > erFAILURE)
		iRV = ade7953WriteValue(psADE7953, regPGA_IA, &psADE7953->pga.ia, sADE7953Defaults.Igain[0]);
	#if	(ade7953USE_CH2 > 0)
	if (iRV > erFAILURE)
		iRV = ade7953WriteValue(psADE7953, regPGA_IB, &psADE7953->pga.ib, sADE7953Defaults.Igain[1]);
	#endif
	return iRV;
}

/**
 * @brief
*/
int ade7953SetNoLoadLevel(ade7953_t * psADE7953) {
	static u8_t const cDisable = 0x07;
	int iRV = ade7953Write(psADE7953, regDISNOLOAD, (u8_t *) &cDisable);
	if (iRV >= erSUCCESS) {
		for (int i = 0; i < NO_ELEM(ade7953_defaults_t, dnl); ++i) {
			iRV = ade7953WriteValue(psADE7953, aRegDNL[i], &psADE7953->valdnl.reg[i], sADE7953CfgBuf.dnl[i]);
		}
		if (iRV >= erSUCCESS) {
			static u8_t const cEnable = 0x00;
			iRV = ade7953Write(psADE7953, regDISNOLOAD, (u8_t *) &cEnable);
		}
	}
	return iRV;
}

/**
 * @brief
 * @return
*/
int	ade7953SetCalibration(ade7953_t * psADE7953, u8_t eCh) {
	int iRV = erSUCCESS;
	for (int i = 0; i < NO_ELEM(ade7953_defaults_t, cal); ++i) {
		u16_t Reg = aRegCFG[eCh][i];
		i32_t Val = sADE7953CfgBuf.cal[i];
		if (INRANGE(regPHCALA, Reg, regPHCALB) && Val < 0)
			Val = abs(Val) | 0x200;						// Add sign magnitude
		iRV = ade7953WriteValue(psADE7953, Reg, NULL, Val);
	}
	return iRV;
}

// ############################## identification & initialization ##################################

/**
 * @brief
 * @return
*/
int	ade7953Identify(i2c_di_t * psI2C) {
	ade7953_t * psADE7953 = &sADE7953[psI2C->DevIdx];
	psI2C->Type = i2cDEV_ADE7953;
	psI2C->Speed = i2cSPEED_400;
	psI2C->TObus = 25;
	psI2C->Test = 1;									// test mode
	psADE7953->psI2C = psI2C;
	ade7953IntConfig(psI2C->DevIdx);
	int iRV = ade7953Read(psADE7953, regVERSION, &psADE7953->ver);
	if (iRV < erSUCCESS) return iRV;
	SL_DBG("Silicon Version=0x%0hhX", psADE7953->ver);
	psI2C->DevIdx = NumADE7953++;
	psI2C->IDok = 1;
	psI2C->Test = 0;
	return iRV;
}

/**
 * @brief
 * @param
 * @return
 */
int ade7953Config(i2c_di_t * psI2C) {
	if (!psI2C->IDok) return erINV_STATE;
	psI2C->CFGok = 0;

	if (!psI2C->CFGerr) {								// if 1st time here, not recall ARO an error
		IF_SYSTIMER_INIT(debugTIMING, stADE7953R, stMICROS, "ADE7953R", 1500, 15000);
		IF_SYSTIMER_INIT(debugTIMING, stADE7953W, stMICROS, "ADE7953W", 1500, 15000);
	}

	ade7953_t * psADE7953 = &sADE7953[psI2C->DevIdx];
	int iRV = ade7953SoftReset(psADE7953);
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953WriteValue(psADE7953, regCONFIG, &psADE7953->config.val, 0x0004);	// Lock comms, enable HPF
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953WriteValue(psADE7953, regUNLOCK, NULL, 0xAD);		// Unlock reg 0x0120
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953WriteValue(psADE7953, regOPTIMUM, NULL, 0x0030);	// enable optimum settings
	if (iRV < erSUCCESS) goto exit;

// ##################################### Accuracy calibration ######################################

/*
	// Ensure the selected calibration data loaded from NVS blob
	ade7953LoadNVSCalib(xOptionGet(ade7953NVS));
	iRV = ade7953SetOffsetGain(psADE7953);				// #1-MGOS
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953SetNoLoadLevel(psADE7953);				// #1-Tasmota
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953SetCalibration(psADE7953, 0);			// #2-Tasmota
	if (iRV < erSUCCESS) goto exit;

	#if	(ade7953USE_CH2 > 0)
	iRV = ade7953SetCalibration(psADE7953, 1);
	if (iRV < erSUCCESS) goto exit;
	#endif
*/
	iRV = ade7953WriteValue(psADE7953, regLCYCMODE, &psADE7953->cfglcm, 0x40);
	if (iRV < erSUCCESS) goto exit;
	// Enable
	iRV = ade7953ChangeIntMask(psADE7953, 0, 0x00FFFFFF, 0x00290000);
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953ReadConfig(psADE7953);
	if (iRV == 0x0004) {			// 0x8000 must be false, comms must be locked to I2C
		halEventUpdateDevice(devMASK_ADE7953, 1);
		psI2C->CFGok = 1;
		iRV = erSUCCESS;
	} else {
		iRV = erINV_CONFIG;
	}
exit:
	return iRV;
}

// ###################################### general support ##########################################


// ############################### device reporting functions ######################################

int ade7953ReportConfig(report_t * psR, ade7953_t * psADE7953) {
	int iRV;
	// Decode DISNOLOAD
	ade7953Read(psADE7953, regDISNOLOAD, (void *) &psADE7953->cfgdnl.val);
	iRV = wprintfx(psR, "DNLOAD\tx%02X\tva=%d  var=%d  ap=%d\r\n", psADE7953->cfgdnl.val,
		psADE7953->cfgdnl.va, psADE7953->cfgdnl.var, psADE7953->cfgdnl.ap);
	// Decode LCYCMODE
	ade7953Read(psADE7953, regLCYCMODE, (void *) &psADE7953->cfglcm.val);
	iRV += wprintfx(psR, "LCMODE\tx%02X\trstr=%d Blva=%d Alva=%d Blvar=%d Alvar=%d Blwatt=%d Alwatt=%d\r\n", psADE7953->cfglcm.val,
		psADE7953->cfglcm.rstr, psADE7953->cfglcm.blva,psADE7953->cfglcm.alva,
		psADE7953->cfglcm.blvar, psADE7953->cfglcm.alvar, psADE7953->cfglcm.blwatt,
		psADE7953->cfglcm.alwatt);

	// Decode CONFIG register
	ade7953Read(psADE7953, regCONFIG, (void *) &psADE7953->config.val);
	iRV += wprintfx(psR, "CONFIG\tx%04X\tCL=%d ZXE=%d ZXI=%d CRCen=%d SWrst=%d ZXlpf=%d ", psADE7953->config.val,
		psADE7953->config.COMM_LOCK, psADE7953->config.ZX_EDGE, psADE7953->config.ZX_I,
		psADE7953->config.CRC_ENABLE, psADE7953->config.SWRST, psADE7953->config.ZXLPF);

	iRV += wprintfx(psR, "REVP_P=%d REVP_CF=%d PFMode=%d HPFen=%d IEB=%d IEA=%d\r\n",
		psADE7953->config.REVP_PULSE, psADE7953->config.REVP_CF, psADE7953->config.PFMODE,
		psADE7953->config.HPFEN, psADE7953->config.INTENB, psADE7953->config.INTENA);

	// CFMODE
	ade7953Read(psADE7953, regCFMODE, (void *) &psADE7953->cfmode.val);
	iRV += wprintfx(psR, "CFMODE\tx%04X\tCF2dis=%d CF1dis=%d CF2sel=%d CF1sel=%d\r\n", psADE7953->cfmode.val,
		psADE7953->cfmode.cf2dis, psADE7953->cfmode.cf1dis, psADE7953->cfmode.cf2sel, psADE7953->cfmode.cf1sel);

	//ALT_OUT
	ade7953Read(psADE7953, regALT_OUTPUT, (void *) &psADE7953->alt_out.val);
	iRV += wprintfx(psR, "ALTOUT\tx%04X\trevp=%X zxi=%X zx=%X\r\n", psADE7953->alt_out.val,
		psADE7953->alt_out.revp_alt, psADE7953->alt_out.zxi_alt, psADE7953->alt_out.zx_alt);

	// ACCMODE
	ade7953Read(psADE7953, regACCMODE, (void *) &psADE7953->accmode.val);
	iRV += wprintfx(psR, "ACCMOD\tx%06X\tBvarnl=%d Bvanl=%d Bactnl=%d Avarnl=%d Avanl=%d Aactnl=%d Bvarsign=%d Avarsign=%d ", psADE7953->accmode.val,
		psADE7953->accmode.bvarnl, psADE7953->accmode.bvanl, psADE7953->accmode.bactnl,
		psADE7953->accmode.avarnl, psADE7953->accmode.avanl, psADE7953->accmode.aactnl,
		psADE7953->accmode.bvarsign, psADE7953->accmode.avarsign);

	iRV += wprintfx(psR, "Bapsign=%d Aapsign=%d Bvaacc=%d Avaacc=%d Bvaracc=%d Avaracc=%d Bwattacc=%d Awattacc=%d\r\n",
		psADE7953->accmode.bapsign, psADE7953->accmode.aapsign, psADE7953->accmode.bvaacc,
		psADE7953->accmode.avaacc, psADE7953->accmode.bvaracc, psADE7953->accmode.avaracc,
		psADE7953->accmode.bwattacc, psADE7953->accmode.awattacc);
	return iRV;
}

int ade7953ReportCalib(report_t * psR, ade7953_t * psADE7953) {
	int iRV = wprintfx(psR, "ADE7953 Calibration:\r\n\txIGAIN\txVGAIN\txWGAIN xVARGAIN\txVAGAIN\tPHCALx\r\n");
	for (int eCh = 0; eCh < ade7953NUM_CHAN; ++eCh) {
		iRV += wprintfx(psR, "Chan%c\t", eCh + CHR_A);
		u8_t * pReg = psADE7953->calib[eCh].buf;
		for( int i = 0; i < NO_ELEM(ade7953_defaults_t, cal); ++i) {
			int Reg = aRegCFG[eCh][i];
			int Size = ade7953Read(psADE7953, Reg, (void *) pReg);
			iRV += wprintfx(psR, "x%0*X\t", ade7953CalcRegSize(Reg) * 2, ade7953CalcRegValue(Reg, pReg));
			pReg += Size;
		}
		iRV += wprintfx(psR, strNL);
	}

	ade7953Read(psADE7953, regPGA_V, (void *) &psADE7953->pga.v);
	ade7953Read(psADE7953, regPGA_IA, (void *) &psADE7953->pga.ia);
	ade7953Read(psADE7953, regPGA_IB, (void *) &psADE7953->pga.ib);
	const u8_t xlatPGA[6] = { 1, 2, 4, 8, 16, 22 };
	iRV += wprintfx(psR, "PGA_X\tV=%d/%d  IA=%d/%d  IB=%d/%d\r\n", psADE7953->pga.v, xlatPGA[psADE7953->pga.v],
	psADE7953->pga.ia, xlatPGA[psADE7953->pga.ia], psADE7953->pga.ib, xlatPGA[psADE7953->pga.ib]);

	ade7953Read(psADE7953, regVRMSOS, (void *) &psADE7953->ofst.v);
	ade7953Read(psADE7953, regAIRMSOS, (void *) &psADE7953->ofst.ia);
	ade7953Read(psADE7953, regBIRMSOS, (void *) &psADE7953->ofst.ib);
	iRV += wprintfx(psR, "OFST_X\tV=x%06X  IA=x%06X  IB=x%06X\r\n",
			psADE7953->ofst.v, psADE7953->ofst.ia, psADE7953->ofst.ib);

	// Add reading of registers to update values in buffers
	iRV += wprintfx(psR, "DNL_X\tAP=x%06X VAR=x%06X VA=x%06X\r\n",
		psADE7953->valdnl.AP_NOLOAD, psADE7953->valdnl.VAR_NOLOAD, psADE7953->valdnl.VA_NOLOAD);

	return iRV;
}

int ade7953ReportIRQs(report_t * psR, ade7953_t * psADE7953) {
	// Decode IRQA registers
	ade7953Read(psADE7953, regIRQENA, (void *) psADE7953->ie_a.u8);
	ade7953Read(psADE7953, regIRQSTATA, (void *) psADE7953->is_a.u8);
	int iRV = wprintfx(psR, "IRQ_A\tEN=x%06X\tSTAT=x%06X", psADE7953->ie_a.val, psADE7953->is_a.val);

	const char caStat1[] = "\tCRC=%d RESET=%d SAG=%d CYCEND=%d WSMP=%d OV=%d ZXV=%d ZXTO=%d\r\n";
	iRV += wprintfx(psR, caStat1, psADE7953->is_a.CRC, psADE7953->is_a.RESET,
		psADE7953->is_a.SAG, psADE7953->is_a.CYCEND, psADE7953->is_a.WSMP,
		psADE7953->is_a.OV, psADE7953->is_a.ZXV, psADE7953->is_a.ZXTO);

	const char caStat2[] = "\tSTAT:\tOIx=%d ZXIx=%d ZXTO_Ix=%d VARSIGNx=%d APSIGNx=%d VA_NLx=%d VAR_NLx=%d ";
	iRV += wprintfx(psR, caStat2, psADE7953->is_a.OIA, psADE7953->is_a.ZXIA, psADE7953->is_a.ZXTO_IA,
		psADE7953->is_a.VARSIGN_A, psADE7953->is_a.APSIGN_A, psADE7953->is_a.VA_NOLOADA, psADE7953->is_a.VAR_NOLOADA);

	const char caStat3[] = "AP_NLx=%d VAEOFx=%d VAREOFx=%d AEOFx=%d VAEHFx=%d VAREHFx=%d AEHFx=%d\r\n";
	iRV += wprintfx(psR, caStat3, psADE7953->is_a.AP_NOLOADA, psADE7953->is_a.VAEOFA, psADE7953->is_a.VAREOFA,
		 psADE7953->is_a.AEOFA, psADE7953->is_a.VAEHFA, psADE7953->is_a.VAREHFA, psADE7953->is_a.AEHFA);


	#if (ade7953USE_CH2 > 0)
	// Decode IRQB registers
	ade7953Read(psADE7953, regIRQENB, (void *) psADE7953->ie_b.u8);
	ade7953Read(psADE7953, regIRQSTATB, (void *) psADE7953->is_b.u8);
	iRV += wprintfx(psR, "IRQ_B\tEN=x%06X\tSTAT=x%06X\r\n", psADE7953->ie_b.val, psADE7953->is_b.val);

	iRV += wprintfx(psR, caStat2, psADE7953->is_b.OIB, psADE7953->is_b.ZXIB, psADE7953->is_b.ZXTO_IB,
		psADE7953->is_b.VARSIGN_B, psADE7953->is_b.APSIGN_B, psADE7953->is_b.VA_NOLOADB, psADE7953->is_b.VAR_NOLOADB);

	iRV += wprintfx(psR, caStat3, psADE7953->is_b.AP_NOLOADB, psADE7953->is_b.VAEOFB, psADE7953->is_b.VAREOFB,
		 psADE7953->is_b.AEOFB, psADE7953->is_b.VAEHFB, psADE7953->is_b.VAREHFB, psADE7953->is_b.AEHFB);

	#endif
	return iRV;
}

int ade7953ReportData(report_t * psR, ade7953_t * psADE7953) { return erFAILURE; }

int ade7953ReportAdjust(report_t * psR, ade7953_t * psADE7953) { return erFAILURE; }

int ade7953Report(report_t * psR) {
	int iRV = PX("## ADE7953 ##\r\n");
	for (int i = 0; i < HAL_ADE7953; ++i) {
		ade7953_t * psADE7953 = &sADE7953[i];
		iRV += halI2C_DeviceReport(psR, psADE7953->psI2C);
		iRV += ade7953ReportConfig(psR, psADE7953);
		iRV += ade7953ReportCalib(psR, psADE7953);
		iRV += ade7953ReportIRQs(psR, psADE7953);
		iRV += wprintfx(psR, strNL);
	}
	return iRV;
}
#endif	// HAL_ADE7953
