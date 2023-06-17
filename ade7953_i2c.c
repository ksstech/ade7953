/*
 * ade7953_i2c.c - Copyright (c) 2023 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#include "hal_variables.h"

#if (ade7953USE_I2C == 1)			// ONLY 1 I2C device supported, single address
#include "ade7953.h"
#include "hal_i2cm.h"
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


// ###################################### Public variables ########################################


// ############################### common support routines #########################################

int ade7953WriteI2C(ade7953_t * psADE7953, u16_t Reg, u8_t Size, i32_t Val) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953->psI2C));
	u8_t caBuf[6];
	caBuf[0] = (Reg >> 8) & 0xFF;
	caBuf[1] = Reg & 0xFF;
	int Len = 2;
	while (Size--)
		caBuf[Len++] = (Val >> (8 * Size)) & 0xFF;		// correct LE -> LE conversion
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953W);
	int iRV = halI2CM_Queue(psADE7953->psI2C,
			i2cW_D,
			caBuf, Len,
			NULL, 0,
			(i2cq_p1_t)NULL, (i2cq_p2_t)NULL);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953W);
	return iRV;
}

int ade7953ReadI2C(ade7953_t * psADE7953, u16_t Reg, u8_t Size, u8_t * pVal) {
	IF_myASSERT(debugPARAM, halCONFIG_inSRAM(psADE7953->psI2C));
	u8_t caBuf[2];
	caBuf[0] = (Reg >> 8) & 0xFF;
	caBuf[1] = Reg & 0xFF;
	IF_EXEC_1(debugTIMING, xSysTimerStart, stADE7953R);
	int iRV = halI2CM_Queue(psADE7953->psI2C,
			psADE7953->CallBack ? i2cWRC_F : i2cWR_BD,
			caBuf, sizeof(caBuf),
			pVal, Size,
			(i2cq_p1_t)psADE7953->CallBack, (i2cq_p2_t) (void *)psADE7953);
	IF_EXEC_1(debugTIMING, xSysTimerStop, stADE7953R);
	return iRV;
}

#endif	// (ade7953USE_I2C > 0)
