/*
 * ade7953.h - Copyright (c) 2023-24 Andre M. Maree / KSS Technologies (Pty) Ltd.
 */

#pragma		once

#include "hal_config.h"
#include "definitions.h"
#include "struct_union.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calibration info
 * Tasmota:
 * "rms":{"current_a":4194303,"current_b":4194303,"voltage":1613194}
 * "angles":{"angle0":200,"angle1":200}
 * "powers":{
 * 	"totactive":{"a":2723574,"b":2723574},
 * 	"apparent":{"a":2723574,"b":2723574},
 * 	"reactive":{"a":2723574,"b":2723574}
 * }
*/

// ##################################### BUILD definitions #########################################

#define	ade7953STORAGE_KEY			"ade7953"
#define ade7953_NUM_CONFIGS			4					// sets of calibration parameters stored in NVS

#if	(ade7953USE_CH2 > 0)
	#define ade7953NUM_SEN24		(7+7+1)
	#define ade7953NUM_SEN16		(2+2+1)
#else
	#define ade7953NUM_SEN24		(7+1)
	#define ade7953NUM_SEN16		(2+1)
#endif

#define ADE7953_PREF				1540				// 4194304 / (1540 / 1000) = 2723574 (= WGAIN, VAGAIN and VARGAIN)
#define ADE7953_UREF				26000				// 4194304 / (26000 / 10000) = 1613194 (= VGAIN)
#define ADE7953_IREF				10000				// 4194304 / (10000 / 10000) = 4194303 (= IGAIN, needs to be different than 4194304 in order to use calib.dat)
#define ade7953_NOLOAD_10W			58393				// default
#define ade7953_NOLOAD_5W			29196				// more sensitive

// ######################################### 8bit registers ########################################
#define regSAGCYC			0x000		// rw	u8	0		Sag line cycles
#define regDISNOLOAD		0x001		// rw	u8	0		No-load detection disable Table 16

#define regLCYCMODE			0x004		// rw	u8	0x40	Line cycle accum mode Table 17

#define regPGA_V			0x007		// rw	u8	0		Voltage gain config Bits[2:0]
#define regPGA_IA			0x008		// rw	u8	0		Current(0) gain config Bits[2:0]
#define regPGA_IB			0x009		// rw	u8	0		Current(1) gain config Bits[2:0]

#define regWRITE_PROTECT	0x040		// rw	u8	0

#define regLAST_OP			0x0FD		// ro	u8	0
#define regUNLOCK			0x0FE
#define regLAST_RWDATA8		0x0FF		// ro	u8	0
// ######################################### 16bit registers #######################################
#define regZXTOUT			0x100		// rw	u16	xFFFF	zero crossing timeout
#define regLINECYC			0x101		// rw	u16	0		Number of half line cycles for line cycle energy accumulation mode
#define regCONFIG			0x102		// rw	u16	0x8004	Configuration register (see Table 18)
#define regCF1DEN			0x103		// rw	u16	0x003F	CF1 frequency divider denominator. When modifying this register, two sequential write operations must be performed to ensure that the write is successful.
#define regCF2DEN			0x104		// rw	u16	0x003F	CF2 frequency divider denominator. When modifying this register, two sequential write operations must be performed to ensure that the write is successful.

#define regCFMODE			0x107		// rw	u16	0x0300	CF output selection (see Table 19)
#define regPHCALA			0x108		// rw	u16	0		Phase calibration register (Current0). This register is in sign magnitude format.
#define regPHCALB			0x109		// rw	u16	0		Phase calibration register (Current1). This register is in sign magnitude format.
#define regPFA				0x10A		// ro	i16	0		Power factor (Current0)
#define regPFB				0x10B		// ro	i16	0		Power factor (Current1)
#define regANGLE_A			0x10C		// ro	i16	0		Angle between the voltage input and the Current0 input
#define regANGLE_B			0x10D		// ro	i16	0		Angle between the voltage input and the Current1 input
#define regPERIOD			0x10E		// ro	u16	0		Period/frequency register
#define regALT_OUTPUT		0x110		// rw	u16	0		

#define regOPTIMUM			0x120		// rw	u16	0		This register should be set to 30h to meet the performance specified in Table 1. To modify this register, it must be unlocked by setting Register Address 0xFE to 0xAD immediately prior

#define regLAST_ADD16		0x1FE		// ro	u16	0
#define regLAST_RWDATA16	0x1FF		// ro	u16	0
// ######################################### 24/32 registers #######################################
#define regSAGLVL			0x200		// rw	u24	0		Sag voltage level
#define regACCMODE			0x201		// rw	u24	0		Accumulation mode (see Table 21)
#define regAP_NOLOAD		0x203		// rw	u24	x00E419	No load threshold for actual power
#define regVAR_NOLOAD		0x204		// rw	u24	x00E419	No load threshold for reactive power
#define regVA_NOLOAD		0x205		// rw	u24	x000000	No load threshold for apparent power
#define regAVA				0x210		// ro	i24	0		Instantaneous apparent power (Current0)
#define regBVA				0x211		// ro	i24	0		Instantaneous apparent power (Current1)
#define regAWATT			0x212		// ro	i24	0		Instantaneous actual power (Current0)
#define regBWATT			0x213		// ro	i24	0		Instantaneous actual power (Current1)
#define regAVAR				0x214		// ro	i24	0		Instantaneous reactive power (Current0)
#define regBVAR				0x215		// ro	i24	0		Instantaneous reactive power (Current1)
#define regIA				0x216		// ro	i24	0		Instantaneous current0
#define regIB				0x217		// ro	i24	0		Instantaneous current1
#define regV				0x218		// ro	i24	0		Instantaneous voltage

#define regIRMSA			0x21A		// ro	u24	0		RMS current0
#define regIRMSB			0x21B		// ro	u24	0		RMS current1
#define regVRMS				0x21C		// ro	u24	0		RMS voltage

#define regAENERGYA			0x21E		// ro	i24	0		Active energy channel0
#define regAENERGYB			0x21F		// ro	i24	0		Active energy channel1
#define regRENERGYA			0x220		// ro	i24	0		Reactive energy channel0
#define regRENERGYB			0x221		// ro	i24	0		Reactive energy channel1
#define regAPENERGYA		0x222		// ro	i24	0		Apparent energy channel0
#define regAPENERGYB		0x223		// ro	i24	0		Apparent energy channel1
#define regOVLVL			0x224		// rw	u24	xFFFFFF	Over voltage level
#define regOILVL			0x225		// rw	u24	xFFFFFF	Over current level
#define regVPEAK			0x226		// ro	u24	0		Voltage channel peak
#define regRSTVPEAK			0x227		// ro	u24	0		Voltage channel peak with reset
#define regIAPEAK			0x228		// ro	u24	0		Current channel0 peak
#define regRSTIAPEAK		0x229		// ro	u24	0		Current channel0 peak with reset
#define regIBPEAK			0x22A		// ro	u24	0		Current channel1 peak
#define regRSTIBPEAK		0x22B		// ro	u24	0		Current channel1 peak with reset
#define regIRQENA			0x22C		// rw	u24	x100000	Interrupt enable (Current0, see Table 22)
#define regIRQSTATA			0x22D		// ro	u24	0		Interrupt status (Current0, see Table 23)
#define regRSTIRQSTATA		0x22E		// ro	u24	0		Interrupt status read with reset
#define regIRQENB			0x22F		// rw	u24	x100000
#define regIRQSTATB			0x230		// ro	u24	0
#define regRSTIRQSTATB		0x231		// ro	u24	0

#define regAIGAIN			0x280		// rw	u24	x400000	CALIB Current0 gain
#define regAVGAIN			0x281		// rw	u24	x400000	CALIB Voltage0 gain
#define regAWGAIN			0x282		// rw	u24	x400000	CALIB Active power0 gain
#define regAVARGAIN			0x283		// rw	u24	x400000	CALIB Reactive power0 gain
#define regAVAGAIN			0x284		// rw	u24	x400000	CALIB Apparent power0 gain

#define regAIRMSOS			0x286		// rw	i24	x000000	CALIB RMS Current0 offset

#define regVRMSOS			0x288		// rw	i24	x000000	CALIB RMS Voltage offset
#define regAWATTOS			0x289		// rw	i24	x000000	CALIB Active power0 offset corrrection
#define regAVAROS			0x28A		// rw	i24	x000000 CALIB Reactive power0 offset corrrection
#define regAVASOS			0x28B		// rw	i24	x000000	CALIB Apparent power0 offset corrrection
#define regBIGAIN			0x28C		// rw	u24	x400000	CALIB Current1 gain
#define regBVGAIN			0x28D		// rw	u24	x400000	CALIB Voltage1 gain DO NOT MODIFY !!!!
#define regBWGAIN			0x28E		// rw	u24	x400000	CALIB Active power1 gain
#define regBVARGAIN			0x28F		// rw	u24	x400000	CALIB Reactive power1 gain
#define regBVAGAIN			0x290		// rw	u24	x400000	CALIB Apparent power1 gain

#define regBIRMSOS			0x292		// rw	i24	x000000 CALIB RMS Current1 offset

#define regBWATTOS			0x295		// rw	i24	x000000 CALIB Active power1 offset
#define regBVAROS			0x296		// rw	i24	x000000 CALIB Reactive power1 offset
#define regBVAOS			0x297		// rw	i24	x000000 CALIB Apparent power1 offset

#define regLAST_RWDATA24	0x2FF		// ro	u24	0
// special registers
#define regCRC				0x37F		// ro	u32	0xFFFFFFFF
#define regVERSION			0x702		// ro	u8	??
#define regEX_REF			0x800		// rw	u8	0

#define regCONFIG_SWRST		(1 << 7)

// ######################################## Enumerations ###########################################

enum ade7953_gain { ade7953_GAIN_1, ade7953_GAIN_2, ade7953_GAIN_4, ade7953_GAIN_8, ade7953_GAIN_16, ade7953_GAIN_22 };

// ######################################### Structures ############################################

typedef union ade7953_pga_x_t {		// 8bit PGA_V/IA/IB
	struct  __attribute__((packed)) { u8_t v; u8_t ia; u8_t ib; };
	u8_t val[3];
} ade7953_pga_x_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_pga_x_t) == 3);

typedef union ade7953_cfgdnl_t {	// 8bit DISNOLOAD
	struct  __attribute__((packed)) { u8_t ap:1; u8_t var:1; u8_t va:1; u8_t spare:5; };
	u8_t val;
} ade7953_cfgdnl_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_cfgdnl_t) == 1);

typedef union {
	struct __attribute__((packed)) {
		u8_t alwatt:1; u8_t blwatt:1; u8_t alvar:1; u8_t blvar:1; u8_t alva:1; u8_t blva:1; u8_t rstr:1; u8_t spare:1;
	};
	u8_t val;
} ade7953_cfglcm_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_cfglcm_t) == 1);

typedef union {						// 16bit CONFIG
	struct __attribute__((packed)) {
		u8_t INTENA : 1;
		u8_t INTENB : 1;
		u8_t HPFEN : 1;
		u8_t PFMODE : 1;
		u8_t REVP_CF : 1;
		u8_t REVP_PULSE : 1;
		u8_t ZXLPF : 1;
		u8_t SWRST : 1;
		u8_t CRC_ENABLE : 1;
		u8_t RSVD1 : 2;
		u8_t ZX_I : 1;
		u8_t ZX_EDGE : 2;
		u8_t RSVD2 : 1;
		u8_t COMM_LOCK : 1;
	};
	u16_t val;
} ade7953_config_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_config_t) == 2);

typedef union {						// 16bit CF Mode
	struct __attribute__((packed)) { u8_t cf1sel:4; u8_t cf2sel:4; u8_t cf1dis:1; u8_t cf2dis:1; u8_t spare:6; };
	u16_t val;
} ade7953_cfmode_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_cfmode_t) == 2);

typedef union {						// 16bit ALTernative OUTput
	struct __attribute__((packed)) { u8_t zx_alt:4; u8_t zxi_alt:4; u8_t revp_alt:4; u8_t spare:4; };
	u16_t val;
} ade7953_altout_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_altout_t) == 2);

typedef union {						// 24bit ACCumulator Mode
	struct __attribute__((packed)) {
	u8_t awattacc:2;	u8_t bwattacc:2;
	u8_t avaracc:2;		u8_t bvaracc:2;
	u8_t avaacc:1;		u8_t bvaacc:1;
	u8_t aapsign:1;		u8_t bapsign:1;
	u8_t avarsign:1;	u8_t bvarsign:1;
	u8_t rsvd1:2;
	u8_t aactnl:1;		u8_t avanl:1;		u8_t avarnl:1;
	u8_t bactnl:1;		u8_t bvanl:1;		u8_t bvarnl:1;		
	u8_t rsvd2:2;
	};
	x24_t val;
} ade7953_accmode_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_accmode_t) == 3);

typedef union {						// 24bit IRQENA / IRQSTATA registers
	struct __attribute__((packed)) {
		u8_t	AEHFA : 1;			// LSB
		u8_t	VAREHFA : 1;
		u8_t	VAEHFA : 1;
		u8_t	AEOFA : 1;
		u8_t	VAREOFA : 1;
		u8_t	VAEOFA : 1;
		u8_t	AP_NOLOADA : 1;
		u8_t	VAR_NOLOADA : 1;
		u8_t	VA_NOLOADA : 1;
		u8_t	APSIGN_A : 1;
		u8_t	VARSIGN_A : 1;
		u8_t	ZXTO_IA: 1;
		u8_t	ZXIA : 1;
		u8_t	OIA : 1;
		u8_t	ZXTO : 1;
		u8_t	ZXV : 1;
		u8_t	OV : 1;
		u8_t	WSMP : 1;
		u8_t	CYCEND : 1;
		u8_t	SAG : 1;
		u8_t	RESET : 1;
		u8_t	CRC : 1;
		u8_t	SPARE : 2;			// MSB
	};
	struct __attribute__((packed)) { u32_t val : 24; };
	u8_t u8[3];
} ade7953_enirqa_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_enirqa_t) == 3);

typedef union {						// 24bit IRQENB / IRQSTATB registers
	struct __attribute__((packed)) {
		u8_t	AEHFB : 1;			// LSB
		u8_t	VAREHFB : 1;
		u8_t	VAEHFB : 1;
		u8_t	AEOFB : 1;
		u8_t	VAREOFB : 1;
		u8_t	VAEOFB : 1;
		u8_t	AP_NOLOADB : 1;
		u8_t	VAR_NOLOADB : 1;
		u8_t	VA_NOLOADB : 1;
		u8_t	APSIGN_B : 1;
		u8_t	VARSIGN_B : 1;
		u8_t	ZXTO_IB : 1;
		u8_t	ZXIB : 1;
		u8_t	OIB : 1;
		u16_t	SPARE : 10;			// MSB
	};
	struct __attribute__((packed)) { u32_t val : 24; };
	u8_t u8[3];
} ade7953_enirqb_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_enirqb_t) == 3);

typedef union {						// 24bit x3 NoLoad levels
	struct __attribute__((packed)) {
		x24_t AP_NOLOAD;
		x24_t VAR_NOLOAD;
		x24_t VA_NOLOAD;
	};
	x24_t reg[3];
} ade7953_valdnl_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_valdnl_t) == 9);

typedef union ade7953_calib_t {		// 5x24bit + 1x16bit calibration per channel
	struct __attribute__((packed)) ade7953_calib_s {
		x24_t IGAINA;
		x24_t VGAINA;
		x24_t WGAINA;
		x24_t VARGAINA;
		x24_t VAGAINA;
		x16_t PHCALA;
	} ade7953_calib_s;
	u8_t buf[sizeof(struct ade7953_calib_s)];
} ade7953_calib_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_calib_t) == 17);

typedef union ade7953_ofst_x_t {	// 3x 24bit
	struct __attribute__((packed)) { x24_t v; x24_t ia; x24_t ib; };
	x24_t regs[3];
//	u8_t buf[sizeof(struct ade7953_ofst_x_s)];
} ade7953_ofst_x_t;
DUMB_STATIC_ASSERT(sizeof(ade7953_ofst_x_t) == 9);

struct i2c_di_t;
typedef struct __attribute__((packed)) {
	struct i2c_di_t * psI2C;
	void (*cb)(void *);
	// ### Status registers ###
	ade7953_enirqa_t ie_a, is_a;	// 6
	#if(ade7953USE_CH2 > 0)								// Line 2 / Neutral
	ade7953_enirqb_t ie_b, is_b;	// 6
	#endif
	ade7953_config_t config;		// 2
	ade7953_cfmode_t cfmode;		// 2
	ade7953_altout_t alt_out;		// 2
	ade7953_cfgdnl_t cfgdnl;		// 1
	ade7953_cfglcm_t cfglcm;		// 1
	ade7953_accmode_t accmode;		// 3
	ade7953_valdnl_t valdnl;		// 9
	ade7953_ofst_x_t ofst;			// 9
	ade7953_pga_x_t pga;			// 3
	u8_t ver;
	ade7953_calib_t calib[ade7953NUM_CHAN];
} ade7953_t;

typedef struct ade7953_defaults_t {	// contain values to be used for [initial] config
	i32_t cal[6]; 
	i32_t dnl[3];
	f32_t Iscale[ade7953NUM_CHAN];
	f32_t Iofst[ade7953NUM_CHAN];
	f32_t Igain[ade7953NUM_CHAN];
	f32_t Pscale[ade7953NUM_CHAN];
	f32_t Escale[ade7953NUM_CHAN];
	f32_t Vscale;
	f32_t Vofst;
	f32_t Vgain;
} ade7953_defaults_t;

// ####################################### Global variables ########################################

extern u8_t NumADE7953;
extern ade7953_t sADE7953[halHAS_ADE7953];
extern const ade7953_defaults_t sADE7953Defaults;

// ####################################### Global functions ########################################

int ade7953CalcRegSize(u16_t Reg);
int ade7953Write(ade7953_t * psADE7953, u16_t Reg, void * pV);
int ade7953WriteValue(ade7953_t * psADE7953, u16_t Reg, void * pV, i32_t I32);
int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pV);
int ade7953ReadValue(ade7953_t * psADE7953, u16_t Reg, void * pV, i32_t * pI32, bool bSign);
u16_t ade7953ReadConfig(ade7953_t * psADE7953);
int ade7953Update(ade7953_t * psADE7953, u16_t Reg, void * pV, u32_t ANDmask, u32_t ORmask);

int	ade7953Identify(struct i2c_di_t * psI2C);
int ade7953Config(struct i2c_di_t * psI2C);
int	ade7953LoadNVSCalib(u8_t Idx);
struct report_t;
int ade7953ReportConfig(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953ReportCalib(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953ReportIRQs(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953ReportData(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953ReportAdjust(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953Report(struct report_t * psRprt);

/*
typedef union r24_ep_t {								// 24bit sensor registers
	struct __attribute__((packed)) {
		x24_t ae_a, re_a, ape_a, pw_a, pva_a, pvar_a, irms_a;
		#if	(ade7953USE_CH2 > 0)						// Line 2 / Neutral
		x24_t ae_b, re_b, ape_b, pw_b, pva_b, pvar_b, irms_b;
		#endif
		x24_t vrms;
	};
	x24_t ep_x[ade7953NUM_SEN24];
} r24_ep_t;

typedef union r16_ep_t {								// 16bit sensor registers
	struct __attribute__((packed)) {
		x16_t pf_a, angle_a;
		#if	(ade7953USE_CH2 > 0)						// Line 2 / Neutral
		x16_t pf_b, angle_b;
		#endif
		x16_t per;
	};
	x16_t ep_x[ade7953NUM_SEN16];
} r16_ep_t;
*/

#ifdef __cplusplus
}
#endif
