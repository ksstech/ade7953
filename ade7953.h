/*
 * ade7953.h
 */

#pragma		once

#include "definitions.h"
#include "endpoints.h"
#include "rules.h"
#include "hal_gpio.h"
#if (ade7953USE_I2C > 0)
	#include "hal_i2c.h"
#elif (ade7953USE_SPI > 0)
	#include "hal_spi.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ##################################### BUILD definitions #########################################

#define ade7953READ_32BIT			0					// 0 = 24bit, 1 = 32bit data
#define	ade7953USE_CH_2				1
#define	ADE7953_T_SNS				1000
#define	halSTORAGE_KEY_ADE7953		"ade7953"
#define ade7953CALIB_NUM			4					// # of sets calibration parameters stored in NVS

#define	ADE7953_NUMURI				(URI_ADE7953_LAST - URI_ADE7953_PFA + 1)

// ##################################### Register definitions #####################################

#define ade7953REG_SAGCYC			0x000				// rw	u8	0
#define ade7953REG_DISNOLOAD		0x001				// rw	u8	0
#define ade7953REG_LCYCMODE			0x004				// rw	u8	0x40
#define ade7953REG_PGA_V			0x007				// rw	u8	0
#define ade7953REG_PGA_IA			0x008				// rw	u8	0
#define ade7953REG_PGA_IB			0x009				// rw	u8	0
#define ade7953REG_WRITE_PROTECT	0x040				// rw	u8	0
#define ade7953REG_LAST_OP			0x0FD				// ro	u8	0
#define ade7953REG_UNLOCK			0x0FE
#define ade7953REG_LAST_RWDATA8		0x0FF				// ro	u8	0

#define ade7953REG_VERSION			0x702				// ro	u8	??
#define ade7953REG_EX_REF			0x800				// rw	u8	0

#define ade7953REG_ZXTOUT			0x100				// rw	u16	0xFFFF
#define ade7953REG_LINECYC			0x101				// rw	u16	0
#define ade7953REG_CONFIG			0x102				// rw	u16	0x8004
#define ade7953REG_CF1DEN			0x103				// rw	u16	0x003F
#define ade7953REG_CF2DEN			0x104				// rw	u16	0x003F
#define ade7953REG_CFMODE			0x107				// rw	u16	0x0300
#define ade7953REG_PHCALA			0x108				// rw	u16	0
#define ade7953REG_PHCALB			0x109				// rw	u16	0
#define ade7953REG_PFA				0x10A				// ro	i16	0
#define ade7953REG_PFB				0x10B				// ro	i16	0
#define ade7953REG_ANGLE_A			0x10C				// ro	i16	0
#define ade7953REG_ANGLE_B			0x10D				// ro	i16	0
#define ade7953REG_PERIOD			0x10E				// ro	u16	0
#define ade7953REG_ALT_OUTPUT		0x110				// rw	u16	0
#define ade7953REG_OPTIMUM			0x120				// rw	u16	0
#define ade7953REG_LAST_ADD16		0x1FE				// ro	u16	0
#define ade7953REG_LAST_RWDATA16	0x1FF				// ro	u16	0

#define ade7953REG_SAGLVL			0x200				// rw	u24	0
#define ade7953REG_ACCMODE			0x201				// rw	u24	0
#define ade7953REG_AP_NOLOAD		0x203				// rw	u24	0x00E419
#define ade7953REG_VAR_NOLOAD		0x204				// rw	u24	0x00E419
#define ade7953REG_VA_NOLOAD		0x205				// rw	u24	0x000000
#define ade7953REG_AVA				0x210				// ro	i24	0
#define ade7953REG_BVA				0x211				// ro	i24	0
#define ade7953REG_AWATT			0x212				// ro	i24	0
#define ade7953REG_BWATT			0x213				// ro	i24	0
#define ade7953REG_AVAR				0x214				// ro	i24	0
#define ade7953REG_BVAR				0x215				// ro	i24	0
#define ade7953REG_IA				0x216				// ro	i24	0
#define ade7953REG_IB				0x217				// ro	i24	0
#define ade7953REG_V				0x218				// ro	i24	0
#define ade7953REG_IRMSA			0x21A				// ro	u24	0
#define ade7953REG_IRMSB			0x21B				// ro	u24	0
#define ade7953REG_VRMS				0x21C				// ro	u24	0

#define ade7953REG_AENERGYA			0x21E				// ro	i24	0
#define ade7953REG_AENERGYB			0x21F				// ro	i24	0
#define ade7953REG_RENERGYA			0x220				// ro	i24	0
#define ade7953REG_RENERGYB			0x221				// ro	i24	0
#define ade7953REG_APENERGYA		0x222				// ro	i24	0
#define ade7953REG_APENERGYB		0x223				// ro	i24	0

#define ade7953REG_OVLVL			0x224				// rw	u24	0xFFFFFF
#define ade7953REG_OILVL			0x225				// rw	u24	0xFFFFFF

#define ade7953REG_VPEAK			0x226				// ro	u24	0
#define ade7953REG_RSTVPEAK			0x227				// ro	u24	0
#define ade7953REG_IAPEAK			0x228				// ro	u24	0
#define ade7953REG_RSTIAPEAK		0x229				// ro	u24	0
#define ade7953REG_IBPEAK			0x22A				// ro	u24	0
#define ade7953REG_RSTIBPEAK		0x22B				// ro	u24	0

#define ade7953REG_IRQENA			0x22C				// rw	u24	0x100000
#define ade7953REG_IRQSTATA			0x22D				// ro	u24	0
#define ade7953REG_RSTIRQSTATA		0x22E				// ro	u24	0

#define ade7953REG_IRQENB			0x22F				// rw	u24	0x100000
#define ade7953REG_IRQSTATB			0x230				// ro	u24	0
#define ade7953REG_RSTIRQSTATB		0x231				// ro	u24	0

#define ade7953REG_AIGAIN			0x280				// rw	u24	0x400000
#define ade7953REG_AVGAIN			0x281				// rw	u24	0x400000
#define ade7953REG_AWGAIN			0x282				// rw	u24	0x400000
#define ade7953REG_AVARGAIN			0x283				// rw	u24	0x400000
#define ade7953REG_AVAGAIN			0x284				// rw	u24	0x400000

#define ade7953REG_AIRMSOS			0x286				// rw	i24	0x000000
#define ade7953REG_VRMSOS			0x288				// rw	i24	0x000000
#define ade7953REG_AWATTOS			0x289				// rw	i24	0x000000
#define ade7953REG_AVAROS			0x28A				// rw	i24	0x000000
#define ade7953REG_AVASOS			0x28B				// rw	i24	0x000000

#define ade7953REG_BIGAIN			0x28C				// rw	u24	0x400000
#define ade7953REG_BVGAIN			0x28D				// rw	u24	0x400000
#define ade7953REG_BWGAIN			0x28E				// rw	u24	0x400000
#define ade7953REG_BVARGAIN			0x28F				// rw	u24	0x400000
#define ade7953REG_BVAGAIN			0x290				// rw	u24	0x400000

#define ade7953REG_BIRMSOS			0x292				// rw	i24	0x000000
#define ade7953REG_BWATTOS			0x295				// rw	i24	0x000000
#define ade7953REG_BVAROS			0x296				// rw	i24	0x000000
#define ade7953REG_BVASOS			0x297				// rw	i24	0x000000

#define ade7953REG_LAST_RWDATA24	0x2FF				// ro	u24	0

#define ade7953REG_CRC				0x37F				// ro	u32	0xFFFFFF

#define ade7953REG_IRQSTATA_RESET	(1 << 20)
#define ade7953REG_CONFIG_HPFEN		(1 << 2)
#define ade7953REG_CONFIG_SWRST		(1 << 7)

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
	u16_t	val;
} ade7953config_t;
DUMB_STATIC_ASSERT(sizeof(ade7953config_t) == 2);

typedef union {
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
} ade7953irqstata_t;
DUMB_STATIC_ASSERT(sizeof(ade7953irqstata_t) == 3);

typedef union {
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
} ade7953irqstatb_t;
DUMB_STATIC_ASSERT(sizeof(ade7953irqstatb_t) == 3);

typedef struct __attribute__((packed)) ade7953nvs_t {
	f32_t	CalibV;
	f32_t	CalibI[2];
} ade7953nvs_t;
DUMB_STATIC_ASSERT(sizeof(ade7953nvs_t) == 12);

typedef struct __attribute__((packed)) ade7953_t {
	#if (ade7953USE_I2C > 0)
	i2c_di_t * psI2C;
	#elif (ade7953USE_SPI > 0)
	spi_device_handle_t	hndl;
	#endif
	// device register storage
	ade7953config_t sCONFIG;
	ade7953irqstata_t sIRQENA;
	ade7953irqstata_t sIRQSTATA;
	ade7953irqstatb_t sIRQENB;
	ade7953irqstatb_t sIRQSTATB;
	// non register storage
	void (*CallBack)(void *);
	ade7953nvs_t sCurCalib;
	float ScaleV, ScaleI[2], ScaleP[2], ScaleE[2];
	struct __attribute__((packed)) {
		gpio_num_t pin;									// 0 -> 40
		u8_t Gain0;										// 1 -> 22
		u8_t Gain1;										// 1 -> 16
	};
} ade7953_t;

// ####################################### Global variables ########################################

extern u8_t NumADE7953;
extern ade7953_t sADE7953[halHAS_ADE7953];
extern ade7953nvs_t ade7953nvs[halHAS_ADE7953];

// ####################################### Global functions ########################################

int ade7953CalcRegSize(u16_t reg);
int ade7953Write(ade7953_t * psADE7953, u16_t Reg, i32_t Val);
int ade7953Read(ade7953_t * psADE7953, u16_t Reg, void * pVal);

#if(ade7953USE_I2C > 0)
int ade7953WriteI2C(ade7953_t * psADE7953, u16_t Reg, u8_t Size, i32_t Val);
int ade7953ReadI2C(ade7953_t * psADE7953, u16_t Reg, u8_t Size, u8_t * pVal);
#elif (ade7953USE_SPI > 0)
int ade7953WriteSPI(ade7953_t * psADE7953, u16_t Reg, u8_t Size, i32_t Val);
int ade7953ReadSPI(ade7953_t * psADE7953, u16_t Reg, u8_t Size, u8_t * pVal);
#endif

int	ade7953Identify(void * pVoid);
int ade7953Config(void * pVoid);
int ade7953ReConfig(void * pVoid);

u16_t ade7953ReadConfig(ade7953_t * psADE7953);

void ade7953ReportStatus(report_t * psRprt, ade7953_t * psADE7953);
void ade7953Report(void);

#ifdef __cplusplus
}
#endif
