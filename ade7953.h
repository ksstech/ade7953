/*
 * ade7953.h
 */

#pragma		once

#include "definitions.h"
#include "struct_union.h"

#ifdef __cplusplus
extern "C" {
#endif

// ##################################### BUILD definitions #########################################

#define	ade7953STORAGE_KEY			"ade7953"
#define ade7953CALIB_NUM			4					// # of sets calibration parameters stored in NVS

#if	(ade7953USE_CH2 > 0)
	#define ade7953NUM_SEN24		(7+7+1)
	#define ade7953NUM_SEN16		(2+2+1)
#else
	#define ade7953NUM_SEN24		(7+1)
	#define ade7953NUM_SEN16		(2+1)
#endif

// ##################################### Register definitions #####################################

#define regSAGCYC			0x000				// rw	u8	0
#define regDISNOLOAD		0x001				// rw	u8	0
#define regLCYCMODE			0x004				// rw	u8	0x40
#define regPGA_V			0x007				// rw	u8	0
#define regPGA_IA			0x008				// rw	u8	0
#define regPGA_IB			0x009				// rw	u8	0
#define regWRITE_PROTECT	0x040				// rw	u8	0
#define regLAST_OP			0x0FD				// ro	u8	0
#define regUNLOCK			0x0FE
#define regLAST_RWDATA8		0x0FF				// ro	u8	0

#define regZXTOUT			0x100				// rw	u16	0xFFFF
#define regLINECYC			0x101				// rw	u16	0
#define regCONFIG			0x102				// rw	u16	0x8004
#define regCF1DEN			0x103				// rw	u16	0x003F
#define regCF2DEN			0x104				// rw	u16	0x003F
#define regCFMODE			0x107				// rw	u16	0x0300
#define regPHCALA			0x108				// rw	u16	0			CALIB
#define regPHCALB			0x109				// rw	u16	0			CALIB
#define regPFA				0x10A				// ro	i16	0
#define regPFB				0x10B				// ro	i16	0
#define regANGLE_A			0x10C				// ro	i16	0
#define regANGLE_B			0x10D				// ro	i16	0
#define regPERIOD			0x10E				// ro	u16	0
#define regALT_OUTPUT		0x110				// rw	u16	0
#define regOPTIMUM			0x120				// rw	u16	0
#define regLAST_ADD16		0x1FE				// ro	u16	0
#define regLAST_RWDATA16	0x1FF				// ro	u16	0

#define regSAGLVL			0x200				// rw	u24	0
#define regACCMODE			0x201				// rw	u24	0
#define regAP_NOLOAD		0x203				// rw	u24	0x00E419
#define regVAR_NOLOAD		0x204				// rw	u24	0x00E419
#define regVA_NOLOAD		0x205				// rw	u24	0x000000
#define regAVA				0x210				// ro	i24	0
#define regBVA				0x211				// ro	i24	0
#define regAWATT			0x212				// ro	i24	0
#define regBWATT			0x213				// ro	i24	0
#define regAVAR				0x214				// ro	i24	0
#define regBVAR				0x215				// ro	i24	0
#define regIA				0x216				// ro	i24	0
#define regIB				0x217				// ro	i24	0
#define regV				0x218				// ro	i24	0
#define regIRMSA			0x21A				// ro	u24	0
#define regIRMSB			0x21B				// ro	u24	0
#define regVRMS				0x21C				// ro	u24	0
#define regAENERGYA			0x21E				// ro	i24	0
#define regAENERGYB			0x21F				// ro	i24	0
#define regRENERGYA			0x220				// ro	i24	0
#define regRENERGYB			0x221				// ro	i24	0
#define regAPENERGYA		0x222				// ro	i24	0
#define regAPENERGYB		0x223				// ro	i24	0
#define regOVLVL			0x224				// rw	u24	0xFFFFFF
#define regOILVL			0x225				// rw	u24	0xFFFFFF
#define regVPEAK			0x226				// ro	u24	0
#define regRSTVPEAK			0x227				// ro	u24	0
#define regIAPEAK			0x228				// ro	u24	0
#define regRSTIAPEAK		0x229				// ro	u24	0
#define regIBPEAK			0x22A				// ro	u24	0
#define regRSTIBPEAK		0x22B				// ro	u24	0
#define regIRQENA			0x22C				// rw	u24	0x100000
#define regIRQSTATA			0x22D				// ro	u24	0x000000
#define regRSTIRQSTATA		0x22E				// ro	u24	0x000000
#define regIRQENB			0x22F				// rw	u24	0x100000
#define regIRQSTATB			0x230				// ro	u24	0
#define regRSTIRQSTATB		0x231				// ro	u24	0
#define regAIGAIN			0x280				// rw	u24	0x400000	CALIB
#define regAVGAIN			0x281				// rw	u24	0x400000	CALIB
#define regAWGAIN			0x282				// rw	u24	0x400000	CALIB
#define regAVARGAIN			0x283				// rw	u24	0x400000	CALIB
#define regAVAGAIN			0x284				// rw	u24	0x400000	CALIB
#define regAIRMSOS			0x286				// rw	i24	0x000000
#define regVRMSOS			0x288				// rw	i24	0x000000
#define regAWATTOS			0x289				// rw	i24	0x000000
#define regAVAROS			0x28A				// rw	i24	0x000000
#define regAVASOS			0x28B				// rw	i24	0x000000
#define regBIGAIN			0x28C				// rw	u24	0x400000	CALIB
#define regBVGAIN			0x28D				// rw	u24	0x400000	CALIB
#define regBWGAIN			0x28E				// rw	u24	0x400000	CALIB
#define regBVARGAIN			0x28F				// rw	u24	0x400000	CALIB
#define regBVAGAIN			0x290				// rw	u24	0x400000	CALIB
#define regBIRMSOS			0x292				// rw	i24	0x000000
#define regBWATTOS			0x295				// rw	i24	0x000000
#define regBVAROS			0x296				// rw	i24	0x000000
#define regBVASOS			0x297				// rw	i24	0x000000
#define regLAST_RWDATA24	0x2FF				// ro	u24	0
// special registers
#define regCRC				0x37F				// ro	u32	0xFFFFFF
#define regVERSION			0x702				// ro	u8	??
#define regEX_REF			0x800				// rw	u8	0

#define regCONFIG_SWRST		(1 << 7)

// ######################################## Enumerations ###########################################

enum ade7953_gain {
  ade7953_GAIN_1 = 0x00,
  ade7953_GAIN_2 = 0x01,
  ade7953_GAIN_4 = 0x02,
  ade7953_GAIN_8 = 0x03,
  ade7953_GAIN_16 = 0x04,
  ade7953_GAIN_22 = 0x05
};

// ######################################### Structures ############################################

typedef union {
	struct  __attribute__((packed)) { u8_t ap:1; u8_t var:1; u8_t va:1; u8_t spare:5; };
	u8_t val;
} ade7953dnl_t; // 8bit DISNOLOAD register
DUMB_STATIC_ASSERT(sizeof(ade7953dnl_t) == 1);

typedef union {
	struct __attribute__((packed)) {
		u8_t alwatt:1; u8_t blwatt:1; u8_t alvar:1; u8_t blvar:1; u8_t alva:1; u8_t blva:1; u8_t rstr:1; u8_t spare:1;
	};
	u8_t val;
} ade7953lcm_t;
DUMB_STATIC_ASSERT(sizeof(ade7953lcm_t) == 1);

typedef union {						// 16bit CONFIG register
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
} ade7953config_t;
DUMB_STATIC_ASSERT(sizeof(ade7953config_t) == 2);

typedef union {
	struct __attribute__((packed)) { u8_t cf1sel:4; u8_t cf2sel:4; u8_t cf1dis:1; u8_t cf2dis:1; u8_t spare:6; };
	u16_t val;
} ade7953cfmode_t;
DUMB_STATIC_ASSERT(sizeof(ade7953cfmode_t) == 2);

typedef union {
	struct __attribute__((packed)) { u8_t zx_alt:4; u8_t zxi_alt:4; u8_t revp_alt:4; u8_t spare:4; };
	u16_t val;
} ade7953alt_out_t;
DUMB_STATIC_ASSERT(sizeof(ade7953alt_out_t) == 2);

typedef union {
	struct __attribute__((packed)) {
	u8_t awattacc:2;	u8_t bwattacc:2;
	u8_t avaracc:2;		u8_t bvaracc:2;
	u8_t avaacc:1;		u8_t bvaacc:1;		u8_t aapsign:1;		u8_t bapsign:1;
	u8_t avarsign:1;	u8_t bvarsign:1;	u8_t rsvd1:2;
	u8_t aactnl:1;		u8_t avanl:1;		u8_t avarnl:1;		u8_t bactnl:1;
	u8_t bvanl:1;		u8_t bvarnl:1;		u8_t rsvd2:2;
	};
	x24_t val;
} ade7953accmode_t;
DUMB_STATIC_ASSERT(sizeof(ade7953accmode_t) == 3);

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
	u8_t U8[3];
} ade7953irqa_t;
DUMB_STATIC_ASSERT(sizeof(ade7953irqa_t) == 3);

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
	u8_t U8[3];
} ade7953irqb_t;
DUMB_STATIC_ASSERT(sizeof(ade7953irqb_t) == 3);

typedef struct __attribute__((packed)) {				// Registers NOT related to endpoints
	ade7953irqa_t ie_a, is_a;		// 6
	#if	(ade7953USE_CH2 > 0)							// Line 2 / Neutral
	ade7953irqb_t ie_b, is_b;		// 6
	#endif
	ade7953config_t cfg;			// 2
	ade7953cfmode_t cfmode;			// 2
	ade7953alt_out_t alt_out;		// 2
	ade7953accmode_t accmode;		// 3
	ade7953dnl_t disnoload;			// 1
	ade7953lcm_t lcycmode;			// 1
	u8_t ver;
} reg_oth_t;

typedef union {											// 24bit sensor registers
	struct __attribute__((packed)) {
		x24_t ae_a, re_a, ape_a, pw_a, pva_a, pvar_a, irms_a;
		#if	(ade7953USE_CH2 > 0)						// Line 2 / Neutral
		x24_t ae_b, re_b, ape_b, pw_b, pva_b, pvar_b, irms_b;
		#endif
		x24_t vrms;
	};
	x24_t ep_x[ade7953NUM_SEN24];
} r24_ep_t;

typedef union {											// 16bit sensor registers
	struct __attribute__((packed)) {
		x16_t pf_a, angle_a;
		#if	(ade7953USE_CH2 > 0)						// Line 2 / Neutral
		x16_t pf_b, angle_b;
		#endif
		x16_t per;
	};
	x16_t ep_x[ade7953NUM_SEN16];
} r16_ep_t;

struct i2c_di_t;
typedef struct __attribute__((packed)) {
	struct i2c_di_t * psI2C;
	void (*cb)(void *);
	i32_t cal[ade7953NUM_CHAN * 6];
	reg_oth_t oth;
	r24_ep_t ep24;
	r16_ep_t ep16;
} ade7953_t;

typedef struct { i32_t val[ade7953NUM_CHAN * 6]; } ade7953nvs_t;
DUMB_STATIC_ASSERT(sizeof(ade7953nvs_t) == (ade7953NUM_CHAN * 24));

/*
typedef struct {
	f32_t Vscale;
	f32_t Vofst;
	f32_t Iscale0;
	f32_t Iscale1;
	f32_t Iosft0;
	f32_t Iofst1;
	f32_t Pscale0
	f32_t Pscale1;
	f32_t Escale0;
	f32_t Escale1;
	f32_t Vgain;
	f32_t Igain0;
	f32_t Igain1;
} ade7953nvs_t;
*/

typedef struct __attribute__((packed)) ade7953X_t {
	struct __attribute__((packed)) {
		u8_t Gain0;										// 1 -> 22
		u8_t Gain1;										// 1 -> 16
	};
} ade7953X_t;

// ####################################### Global variables ########################################

extern u8_t NumADE7953;
extern ade7953_t ade7953[];
extern ade7953nvs_t ade7953nvs[];

// ####################################### Global functions ########################################

int ade7953CalcRegSize(u16_t Reg);
int ade7953Write(ade7953_t * psADE7953, u16_t Reg, i32_t Val);
int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pVal);
int ade7953Update(ade7953_t * psADE7953, u16_t Reg, void * pVal, u32_t ANDmask, u32_t ORmask);
u16_t ade7953ReadConfig(ade7953_t * psADE7953);

int	ade7953Identify(struct i2c_di_t * psI2C);
int ade7953Config(struct i2c_di_t * psI2C);
int	ade7953LoadNVSConfig(u8_t eChan, u8_t Idx);
struct report_t;
int ade7953ReportStatus(struct report_t * psRprt, ade7953_t * psADE7953);
int ade7953Report(struct report_t * psRprt);

#ifdef __cplusplus
}
#endif
