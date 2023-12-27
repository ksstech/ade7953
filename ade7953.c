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

#include "ade7953.h"

#if (halHAS_ADE7953 > 0)
#include "hal_i2c_common.h"
#include "hal_storage.h"
#include "printfx.h"
#include "syslog.h"
#include "systiming.h"					// timing debugging
#include "x_errors_events.h"

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

const u16_t CfgRegs[ade7953NUM_CHAN * 6] = {
	regAIGAIN, regAVGAIN, regAWGAIN, regAVARGAIN, regAVAGAIN, regPHCALA,
	#if	(ade7953USE_CH2 > 0)		// L2/Neutral
	regBIGAIN, regBVGAIN, regBWGAIN, regBVARGAIN, regBVAGAIN, regPHCALB,
	#endif
};

const i32_t NVSDefaults[ade7953NUM_CHAN * 6] = {
	4194303, 1613194, 2723574, 2723574, 2723574, 200,
	#if	(ade7953USE_CH2 > 0)		// L2/Neutral
	4194303, 1613194, 2723574, 2723574, 2723574, 200,
	#endif
};

// ####################################### Public variables ########################################

u8_t NumADE7953 = 0;
ade7953_t ade7953[halHAS_ADE7953] = { 0 };

// ############################### common support routines #########################################

int ade7953CalcRegSize(u16_t Reg) {
	if (Reg < 0x100 || Reg == regVERSION || Reg == regEX_REF) return 1;
	if (Reg < 0x200) return 2;
	if (Reg < 0x300) return 3;
	return 4;
}

u32_t ade7953CalcRegValue(u16_t Reg, px_t pX) {
	int Size = ade7953CalcRegSize(Reg);
	u32_t U32 = *pX.pu8;
	++pX.pu8;
	if (Size > 1) U32 += *pX.pu8 << 8;
	++pX.pu8;
	if (Size > 2) U32 += *pX.pu8 << 16;
	++pX.pu8;
	if (Size > 3) U32 += *pX.pu8 << 24;
	return U32;
}

/**
 * @brief	Write integer value to specified register
 * @return	number of data bytes written (if no error) or error code
*/
int ade7953Write(ade7953_t * psADE7953, u16_t Reg, i32_t I32) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953));
	int Size = ade7953CalcRegSize(Reg);
	u8_t caBuf[6] = { (Reg >> 8) & 0xFF, Reg & 0xFF };
	int Len = 2;
	while (Size--) caBuf[Len++] = (I32 >> (8 * Size)) & 0xFF;		// correct BE -> LE conversion

	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953W);
	int iRV = halI2C_Queue(psADE7953->psI2C, i2cW_B, caBuf, Len, NULL, 0, (i2cq_p1_t)NULL, (i2cq_p2_t)NULL);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953W);

	if ((iRV < erSUCCESS) && !(Reg == regCONFIG && (I32 & regCONFIG_SWRST)))
		SL_ERR("Error Reg=0x%X iRV=%d", Reg, iRV);
	return iRV < erSUCCESS ? iRV : Len-2;
}

/**
 * @brief	integer value from specified register
 * @return	number of data bytes read (register length) or error code
*/
int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pV) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953));
	u8_t caBuf[2] = { (Reg >> 8) & 0xFF,  Reg & 0xFF };
	int Size = ade7953CalcRegSize(Reg);

	// depending on whether a callback address is set, select I2C opcode
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953R);
	int iRV = halI2C_Queue(psADE7953->psI2C, psADE7953->cb ? i2cWRC_F : i2cWR_B, caBuf,
		sizeof(caBuf), pV, Size, (i2cq_p1_t)psADE7953->cb, (i2cq_p2_t) (void *)psADE7953);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953R);
	return (iRV < erSUCCESS) ? iRV : Size;
}

/**
 * @brief	sensor value ([un]signed integer)
 * @return	if no error, number of bytes read else erFAILURE
*/
int ade7953ReadValue(ade7953_t * psADE7953, u16_t Reg, i32_t * pI32, bool bSign) {
	u8_t caData[4];
	int iRV = ade7953Read(psADE7953, Reg, caData);
	if (iRV < erSUCCESS) return erFAILURE;
	*pI32 = 0;
	// Convert to [un]signed value
	for (int i = 0; i < iRV; i++) *pI32 = (*pI32 << 8) | caData[i];
	// Fix the sign
	if (bSign) {
	    u32_t sign_mask = 0;
		if (iRV == 1) sign_mask = (1 << 7);
		if (iRV == 2) sign_mask = (1 << 15);
		if (iRV == 3) sign_mask = (1 << 23);
		if (iRV == 4) sign_mask = (1 << 31);
		if ((*pI32 & sign_mask) != 0) {
			*pI32 &= ~sign_mask;
			*pI32 |= (1 << 31);
		}
	}
	return iRV;
}

u16_t ade7953ReadConfig(ade7953_t * psADE7953) {
	int iRV = ade7953Read(psADE7953, regCONFIG, &psADE7953->oth.cfg.val);
	return (iRV < erSUCCESS) ? 0 : psADE7953->oth.cfg.val;
}

int ade7953Update(ade7953_t * psADE7953, u16_t Reg, void * pVal, u32_t ANDmask, u32_t ORmask) {
	int Size = ade7953CalcRegSize(Reg);
	u32_t Mask = 0xFFFFFFFF >> ((4 - Size) * 8);
	ANDmask &= Mask;
	ORmask &= Mask;
	int iRV = erFAILURE;
	if (ANDmask || ORmask) {
		iRV = ade7953Read(psADE7953, Reg, pVal);
		IF_EXIT(iRV < erSUCCESS);
		x32_t X32 = { 0 };
		if (Size == 1) X32.x8[0].u8 = *(u8_t *) pVal;
		else if (Size == 2) X32.x16[0].u16 = *(u16_t *) pVal;
		else if (Size == 3) X32.x24 = *(x24_t *) pVal;
		else X32.u32 = *(u32_t*)pVal;

		X32.u32 &= ANDmask;
		X32.u32 |= ORmask;

		if (Size == 1) *(u8_t *) pVal = X32.x8[0].u8;
		else if (Size == 2) *(u16_t *) pVal = X32.x16[0].u16;
		else if (Size == 3) *(x24_t *) pVal = X32.x24;
		else *(u32_t *) pVal = X32.u32;

		iRV = ade7953Write(psADE7953, Reg, X32.u32);
	}
exit:
	return iRV;
}

/*
 * @brief	Callback handler after reading both IRQSTATA & IRQSTATB
 */
void IRAM_ATTR ade7953IRQ_CB(void * Arg) {
	CPTL(); ade7953_t * psADE7953 = (ade7953_t *) Arg;
	ade7953ReportStatus(NULL, psADE7953);
}

/**
 * @brief
 * @param
 * @return
 **/
void IRAM_ATTR ade7953IntHandler(void * Arg) {
	CPTL(); u8_t eDev = (int) Arg;
	IF_myASSERT(debugPARAM, eDev < halHAS_ADE7953);
	ade7953_t * psADE7953 = &ade7953[eDev];

	// schedule 1st read, just to update in SRAM
	psADE7953->cb = NULL;
	ade7953Read(psADE7953, regRSTIRQSTATA, &psADE7953->oth.is_a);

	// schedule 2nd read with callback handler
	psADE7953->cb = &ade7953IRQ_CB;
	ade7953Read(psADE7953, regRSTIRQSTATB, &psADE7953->oth.is_b);
}

void ade7953InitIRQ(int DevIdx) {
	gpio_config_t irq_pin_cfg = {
		.pin_bit_mask = 1ULL << ade7953PIN_IRQ,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE ,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_NEGEDGE,
	};
	ESP_ERROR_CHECK(gpio_config(&irq_pin_cfg));
	halGPIO_IRQconfig(ade7953PIN_IRQ, ade7953IntHandler, (void *) DevIdx);
}

int	ade7953LoadNVSConfig(u8_t eChan, u8_t Idx) {
	IF_myASSERT(debugPARAM, eChan < halHAS_ADE7953 && Idx < ade7953CALIB_NUM);
	size_t	Size = ade7953CALIB_NUM * sizeof(NVSDefaults);
	ade7953nvs_t * psCal = pvRtosMalloc(Size);
	int iRV = halSTORAGE_ReadBlob(halSTORAGE_STORE, ade7953STORAGE_KEY, psCal, &Size, ESP_ERR_NVS_NOT_FOUND);
	if ((iRV != erSUCCESS) || (Size != (ade7953CALIB_NUM * sizeof(NVSDefaults)))) {
		// Clear the (just allocated & read into) blob memory
		memset(psCal, 0, Size = ade7953CALIB_NUM * sizeof(NVSDefaults));
		// Reset the 1st dataset to the NVS defaults, based on #of bytes
		memcpy(psCal, &NVSDefaults, sizeof(NVSDefaults));
		iRV = halSTORAGE_WriteBlob(halSTORAGE_STORE, ade7953STORAGE_KEY, psCal, Size);
		IF_myASSERT(debugRESULT, iRV == erSUCCESS);
		if (Idx) {
			SL_WARN("NVS config (%d) not found, default created/loaded", Idx);
			Idx = 0;
		}
	}
	ade7953_t * psADE7953 = &ade7953[eChan];
	psCal += Idx ;
	for( int i = 0; i < NO_MEM(NVSDefaults); ++i) ade7953Write(psADE7953, CfgRegs[i], psCal->val[i]);
	vRtosFree (psCal);
	return iRV;
}

// ############################## identification & initialization ##################################

int	ade7953Identify(i2c_di_t * psI2C) {
	ade7953_t * psADE7953 = &ade7953[psI2C->DevIdx];
	psADE7953->psI2C = psI2C;
	psI2C->Type = i2cDEV_ADE7953;
	psI2C->Speed = i2cSPEED_400;
	psI2C->TObus = 25;
	psI2C->Test = 1;									// test mode
	int iRV = ade7953Read(psADE7953, regVERSION, &psADE7953->oth.ver);
	if (iRV < erSUCCESS) goto exit;
	SL_DBG("Silicon Version=0x%0hhX", psADE7953->oth.ver);
	psI2C->DevIdx = NumADE7953++;
	psI2C->IDok = 1;
	psI2C->Test = 0;
exit:
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
	ade7953_t * psADE7953 = &ade7953[psI2C->DevIdx];
	psADE7953->psI2C->Test = 1;							// disable handling error we expect to get
	int iRV = ade7953Write(psADE7953, regCONFIG, regCONFIG_SWRST);
	IF_myASSERT(debugRESULT, iRV < erSUCCESS);
	psADE7953->psI2C->Test = 0;
	do {
		vTaskDelay(pdMS_TO_TICKS(10));
		iRV = ade7953Read(psADE7953, regIRQSTATA, &psADE7953->oth.is_a);
	} while (iRV < erSUCCESS || (psADE7953->oth.is_a.RESET == 0));
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953Write(psADE7953, regCONFIG, 0x04);	// Lock comms interface, enable high pass filter
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953Write(psADE7953, regUNLOCK, 0xAD);	// Unlock reg 0x0120
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953Write(psADE7953, regOPTIMUM, 0x30);	// enable optimum settings
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953LoadNVSConfig(psI2C->DevIdx, 0);		// load default calibration
	if (iRV < erSUCCESS) goto exit;

	iRV = ade7953Update(psADE7953, regLCYCMODE, &psADE7953->oth.lcycmode, 0xBF, 0x40);
	if (iRV < erSUCCESS) goto exit;

	psI2C->CFGok = (ade7953ReadConfig(psADE7953) & 0x8000) ? 0 : 1;
	// once off init....
	if (!psI2C->CFGerr) {
		IF_SYSTIMER_INIT(debugTIMING, stADE7953R, stMICROS, "ADE7953RD", 1500, 15000);
		IF_SYSTIMER_INIT(debugTIMING, stADE7953W, stMICROS, "ADE7953WR", 1500, 15000);
		ade7953InitIRQ(psI2C->DevIdx);
	}
exit:
	return iRV;
}

// ###################################### general support ##########################################


// ############################### device reporting functions ######################################

void ade7953ReportAdjust(void) { }

void ade7953ReportData(void) { }

const char CalRegName[] = "xIGAIN   xVGAIN   xWGAIN   xVARGAIN xVAGAIN  PHCALx\r\n";
int ade7953ReportCalib(report_t * psR, ade7953_t * psADE7953) {
	int iRV = wprintfx(psR, CalRegName);
	const int Max = (ade7953USE_CH2 == 0) ? NO_MEM(NVSDefaults) : NO_MEM(NVSDefaults) /2 ;
	static x24_t X24;
	for( int i = 0; i < Max; ++i) {
		ade7953Read(psADE7953, CfgRegs[i], &X24.x8[0]);
		iRV += wprintfx(psR, "0x%0*X ", ade7953CalcRegSize(CfgRegs[i])*2, ade7953CalcRegValue(CfgRegs[i], (px_t)&X24));
	}
	iRV += wprintfx(psR, strCRLF);
	#if (ade7953USE_CH2 > 0)
	for( int i = 0; i < Max; ++i) {
		ade7953Read(psADE7953, CfgRegs[Max+i], &X24.x8[0]);
		iRV += wprintfx(psR, "0x%0*X ", ade7953CalcRegSize(CfgRegs[Max+i])*2, ade7953CalcRegValue(CfgRegs[Max+i], (px_t)&X24));
	}
	iRV += wprintfx(psR, strCRLF);
	#endif
	return iRV;
}

int ade7953ReportStatus(report_t * psR, ade7953_t * psADE7953) {
	int iRV;
	// Decode DISNOLOAD
	iRV = wprintfx(psR, "DNLOAD\tva=%d  var=%d  ap=%d (%03X)\r\n", psADE7953->oth.disnoload.va,
		psADE7953->oth.disnoload.var, psADE7953->oth.disnoload.ap, psADE7953->oth.disnoload.val);
	// Decode LCYCMODE
	iRV += wprintfx(psR, "LCMODE\tsrtr=%d Blva=%d Alva=%d Blvar=%d Alvar=%d Blwatt=%d Alwatt=%d (%03X)\r\n",
		psADE7953->oth.lcycmode.rstr, psADE7953->oth.lcycmode.blva,psADE7953->oth.lcycmode.alva,
		psADE7953->oth.lcycmode.blvar, psADE7953->oth.lcycmode.alvar, psADE7953->oth.lcycmode.blwatt,
		psADE7953->oth.lcycmode.alwatt, psADE7953->oth.lcycmode.val);

	// Decode CONFIG register
	iRV += wprintfx(psR, "CONFIG\tCL=%d ZXE=%d ZXI=%d CRC=%d SWRST=%d ZXLFP=%d ",
		psADE7953->oth.cfg.COMM_LOCK, psADE7953->oth.cfg.ZX_EDGE, psADE7953->oth.cfg.ZX_I,
		psADE7953->oth.cfg.CRC_ENABLE, psADE7953->oth.cfg.SWRST, psADE7953->oth.cfg.ZXLPF);

	iRV += wprintfx(psR, "REVP_P=%d REVP_CF=%d PFM=%d HPF_E=%d IEB=%d IEA=%d (%03X)\r\n",
		psADE7953->oth.cfg.REVP_PULSE, psADE7953->oth.cfg.REVP_CF, psADE7953->oth.cfg.PFMODE, psADE7953->oth.cfg.HPFEN,
		psADE7953->oth.cfg.INTENB, psADE7953->oth.cfg.INTENA, psADE7953->oth.cfg.val);

	// CFMODE
	iRV += wprintfx(psR, "CFMODE\tcf2dis=%d cf1dis=%d cf2sel=%d cf1sel=%d (%03X)\r\n",
		psADE7953->oth.cfmode.cf2dis, psADE7953->oth.cfmode.cf1dis, psADE7953->oth.cfmode.cf2sel,
		psADE7953->oth.cfmode.cf1sel, psADE7953->oth.cfmode.val);

	//ALT_OUT
	iRV += wprintfx(psR, "ALTOUT\trevp=%X zxi=%X zxz=%X (%03X)\r\n", psADE7953->oth.alt_out.revp_alt,
		psADE7953->oth.alt_out.zxi_alt, psADE7953->oth.alt_out.zx_alt, psADE7953->oth.alt_out);

	// ACCMODE
	iRV += wprintfx(psR, "ACCMOD\tBvarnl=%d Bvanl=%d Bactnl=%d Avarnl=%d Avanl=%d Aactnl=%d Bvarsign=%d Avarsign=%d ",
		psADE7953->oth.accmode.bvarnl, psADE7953->oth.accmode.bvanl, psADE7953->oth.accmode.bactnl,
		psADE7953->oth.accmode.avarnl, psADE7953->oth.accmode.avanl, psADE7953->oth.accmode.aactnl,
		psADE7953->oth.accmode.bvarsign, psADE7953->oth.accmode.avarsign);

	iRV += wprintfx(psR, "Bapsign=%d Aapsign=%d Bvaacc=%d Avaacc=%d Bvaracc=%d Avaracc=%d Bwattacc=%d Awattacc=%d (%06X)\r\n",
		psADE7953->oth.accmode.bapsign, psADE7953->oth.accmode.aapsign, psADE7953->oth.accmode.bvaacc,
		psADE7953->oth.accmode.avaacc, psADE7953->oth.accmode.bvaracc, psADE7953->oth.accmode.avaracc,
		psADE7953->oth.accmode.bwattacc, psADE7953->oth.accmode.awattacc, psADE7953->oth.accmode.val);

	// Decode IRQA registers
	iRV += wprintfx(psR, "IRQ_A\tEN=0x%06X  STAT==0x%06X\r\n", psADE7953->oth.ie_a.val, psADE7953->oth.is_a.val);

	const char caStat1[] = "%s\tAEHFx=%d VAREHFx=%d VAEHFx=%d AEOFx=%d VAREOFx=%d VAEOFx=%d AP_NOLOADx=%d ";
	iRV += wprintfx(psR, caStat1, "ENAB_A", psADE7953->oth.is_a.AEHFA, psADE7953->oth.is_a.VAREHFA,
		psADE7953->oth.is_a.VAEHFA, psADE7953->oth.is_a.AEOFA, psADE7953->oth.is_a.VAREOFA,
		psADE7953->oth.is_a.VAEOFA, psADE7953->oth.is_a.AP_NOLOADA);

	const char caStat2[] = "VAR_NOLOADx=%d VA_NOLOADx=%d APSIGNx=%d VARSIGNx=%d ZXTO_Ix=%d ZXIx=%d OIx=%d ";
	iRV += wprintfx(psR, caStat2, psADE7953->oth.is_a.VAR_NOLOADA, psADE7953->oth.is_a.VA_NOLOADA,
		psADE7953->oth.is_a.APSIGN_A, psADE7953->oth.is_a.VARSIGN_A, psADE7953->oth.is_a.ZXTO_IA,
		psADE7953->oth.is_a.ZXIA, psADE7953->oth.is_a.OIA);

	const char caStat3[] = "ZXTO=%d ZXV=%d OV=%d WSMP=%d CYCEND=%d SAG=%d RESET=%d CRC=%d\r\n";
	iRV += wprintfx(psR, caStat3, psADE7953->oth.is_a.ZXTO, psADE7953->oth.is_a.ZXV, psADE7953->oth.is_a.OV,
		psADE7953->oth.is_a.WSMP, psADE7953->oth.is_a.CYCEND, psADE7953->oth.is_a.SAG,
		psADE7953->oth.is_a.RESET, psADE7953->oth.is_a.CRC);

	#if (ade7953USE_CH2 > 0)
	// Decode IRQB registers
	iRV += wprintfx(psR, "IRQ_B\tEN=0x%06X  STAT==0x%06X\r\n", psADE7953->oth.ie_b.val, psADE7953->oth.is_b.val);

	iRV += wprintfx(psR, caStat1, "ENAB_B", psADE7953->oth.is_b.AEHFB, psADE7953->oth.is_b.VAREHFB,
		psADE7953->oth.is_b.VAEHFB, psADE7953->oth.is_b.AEOFB, psADE7953->oth.is_b.VAREOFB,
		psADE7953->oth.is_b.VAEOFB, psADE7953->oth.is_b.AP_NOLOADB);

	iRV += wprintfx(psR, caStat2, psADE7953->oth.is_b.VAR_NOLOADB, psADE7953->oth.is_b.VA_NOLOADB,
		psADE7953->oth.is_b.APSIGN_B,psADE7953->oth.is_b.VARSIGN_B, psADE7953->oth.is_b.ZXTO_IB,
		psADE7953->oth.is_b.ZXIB, psADE7953->oth.is_b.OIB);
	iRV += wprintfx(psR, strCRLF);
	#endif
	return iRV;
}

int ade7953Report(report_t * psR) {
	int iRV = 0;
	for (int i = 0; i < halHAS_ADE7953; ++i) {
		iRV += ade7953ReportStatus(psR, &ade7953[i]);
		iRV += ade7953ReportCalib(psR, &ade7953[i]);
	}
	return iRV;
}
#endif	// halHAS_ADE7953
