/*****

This header file includes some registers of ADuCM360 related to GPIO_Ex1_Blink.
Some registers mean the followings :
		1. Watchdog timer MMRs.
		2. Clocking MMRs.
		3. GPIO MMRs.

2013, 10, 12, Developed by JongSu Lim(Fermi Lim)

*****/

#include <stdint.h>

#if FermiEmulation_Mode

// Cortex-M3/4 System Control Register(SCR)
extern uint32_t SCR, *pSCR;

// Watchdog Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2580
//											Address Offset		Description								Access Type		Default(Reset Value)
extern uint16_t T3LD,	*pT3LD;			//		0x0000,			Load value.									RW,			0x1000
extern uint16_t T3VAL,	*pT3VAL;		//		0x0004,			Current count value, read only.				R			0x1000
extern uint16_t T3CON,	*pT3CON;		//		0x0008,			Control Register.							RW			0x00E9
extern uint16_t T3CLRI, *pT3CLRI;		//		0x000C			Clear Register.								W			N/A
extern uint16_t T3STA,	*pT3STA;		//		0x0018			Status Register.							R			0x0000

// Clocking Memory Mapped Registers(MMRs) : Base Address : 0x4000_2000
//											Address Offset		Description								Access Type		Default(Reset Value)
extern uint16_t CLKCON0,	*pCLKCON0;	//		0x0000,			System Clocks Control Reg. #0.				RW,			0x0004
extern uint16_t CLKCON1,	*pCLKCON1;  //		0x0004,			System Clocks Control Reg. #1.				RW,			0x0000
extern uint16_t CLKDIS,		*pCLKDIS;	//		0x002C,			System clock to peripherals enable/disable.	RW			0x01FF
extern uint16_t XOSCCON,	*pXOSCCON;	//		0x0410			Crystal oscillator control register.		RW			0x0000
extern uint16_t CLKSYSDIV,	*pCLKSYSDIV;//		0x0444			System clock divide-by-2 control register.	RW			0x0001

// GPIO Memory Mapped Registers(MMRs) : Base Address : 0x4000_6000
//											Address Offset		Description								Access Type		Default(Reset Value)
extern uint16_t GP0CON,	*pGP0CON;		//		0x0000,			GPIO Port 0 configuration.					RW,			0x0000
extern uint8_t GP0OEN,	*pGP0OEN;		//		0x0004,			GPIO Port 0 output enable.					RW,			0x00
extern uint8_t GP0PUL,	*pGP0PUL;		//		0x0008,			GPIO Port 0 output pull-up enable.			RW			0xFF
extern uint8_t GP0OCE,	*pGP0OCE;		//		0x000C			GPIO Port 0 open-circuit enable.			RW			0x00
extern uint8_t GP0IN,	*pGP0IN;		//		0x0014			GPIO Port 0 data input.						R			N/A
extern uint8_t GP0OUT,	*pGP0OUT;		//		0x0018			GPIO Port 0 data output.					RW			0x00
extern uint8_t GP0SET,	*pGP0SET;		//		0x001C			GPIO Port 0 data out set.					W			0x00
extern uint8_t GP0CLR,	*pGP0CLR;		//		0x0020			GPIO Port 0 data out clear.					W			0x00
extern uint8_t GP0TGL,	*pGP0TGL;		//		0x0024			GPIO Port 0 data out toggle.				W			0x00

extern uint16_t GP1CON,	*pGP1CON;		//		0x0030,			GPIO Port 1 configuration.					RW,			0x0000
extern uint8_t GP1OEN,	*pGP1OEN;		//		0x0034,			GPIO Port 1 output enable.					RW,			0x00
extern uint8_t GP1PUL,	*pGP1PUL;		//		0x0038,			GPIO Port 1 output pull-up enable.			RW			0xFF
extern uint8_t GP1OCE,	*pGP1OCE;		//		0x003C			GPIO Port 1 open-circuit enable.			RW			0x00
extern uint8_t GP1IN,	*pGP1IN;		//		0x0044			GPIO Port 1 data input.						R			N/A
extern uint8_t GP1OUT,	*pGP1OUT;		//		0x0048			GPIO Port 1 data output.					RW			0x00
extern uint8_t GP1SET,	*pGP1SET;		//		0x004C			GPIO Port 1 data out set.					W			0x00
extern uint8_t GP1CLR,	*pGP1CLR;		//		0x0050			GPIO Port 1 data out clear.					W			0x00
extern uint8_t GP1TGL,	*pGP1TGL;		//		0x0054			GPIO Port 1 data out toggle.				W			0x00

extern uint16_t GP2CON,	*pGP2CON;		//		0x0060,			GPIO Port 2 configuration.					RW,			0x0000
extern uint8_t GP2OEN,	*pGP2OEN;		//		0x0064,			GPIO Port 2 output enable.					RW,			0x00
extern uint8_t GP2PUL,	*pGP2PUL;		//		0x0068,			GPIO Port 2 output pull-up enable.			RW			0xFF
extern uint8_t GP2OCE,	*pGP2OCE;		//		0x006C			GPIO Port 2 open-circuit enable.			RW			0x00
extern uint8_t GP2IN,	*pGP2IN;		//		0x0074			GPIO Port 2 data input.						R			N/A
extern uint8_t GP2OUT,	*pGP2OUT;		//		0x0078			GPIO Port 2 data output.					RW			0x00
extern uint8_t GP2SET,	*pGP2SET;		//		0x007C			GPIO Port 2 data out set.					W			0x00
extern uint8_t GP2CLR,	*pGP2CLR;		//		0x0080			GPIO Port 2 data out clear.					W			0x00
extern uint8_t GP2TGL,	*pGP2TGL;		//		0x0084			GPIO Port 2 data out toggle.				W			0x00

// UART Memory Mapped Registers(MMRs) : Base Address : 0x4000_5000
//										Address Offset		Description							Access Type		Default(Reset Value)
  
extern uint8_t COMTX_RX, *pCOMTX_RX; //    0x0000        Transmit/Receive Holding register          W/R			0x0000	
extern uint8_t COMIEN, *pCOMIEN;     //    0x0004        Interrupt Enable register                  RW          0x0000
extern uint8_t COMIIR, *pCOMIIR;     //    0x0008        Interrupt Identification register          R           0x0000
extern uint8_t COMLCR, *pCOMLCR;     //    0x000C        Line Control register                      RW          0x0000
extern uint8_t COMMCR, *pCOMMCR;     //    0x0010        Module Control register                    RW          0x0000
extern uint8_t COMLSR, *pCOMLSR;     //    0x0014        Line Status register                       R           0x0000
extern uint8_t COMMSR, *pCOMMSR;     //    0x0018        Modem Status register                      R           0x0000
extern uint16_t COMFBR, *pCOMFBR;    //    0x0024        Fractional baud rate divider register.     RW          0x0000
extern uint16_t COMDIV, *pCOMDIV;    //    0x0028        Baud rate Divisor register                 RW          0x0000
extern uint8_t  COMCON, *pCOMCON;    //    0x0030        UART control register                      RW          0x0000

// Wake-Up Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2500
//										Address Offset		Description							Access Type		Default(Reset Value)  
extern uint16_t  T2VAL0, *pT2VAL0;   // 0x0000			Current count value LSB						R			0x0000
extern uint16_t  T2VAL1, *pT2VAL1;   // 0x0004			Current count value MSB                     R			0x0000
extern uint16_t  T2CON, *pT2CON;     // 0x0008			Control Register							RW			0x0040
extern uint16_t  T2INC, *pT2INC;     // 0x000C			12-bit register. Wake up field A			RW			0x00C8
extern uint16_t  T2WUFB0, *pT2WUFB0; // 0x0010			Wake up field B  LSB						RW			0x1FFF
extern uint16_t  T2WUFB1, *pT2WUFB1; // 0x0014			Wake up field B  MSB						RW			0x0000
extern uint16_t  T2WUFC0, *pT2WUFC0; // 0x0018			Wake up field C  LSB						RW			0x2FFF
extern uint16_t  T2WUFC1, *pT2WUFC1; // 0x001C			Wake up field C  MSB						RW			0x0000
extern uint16_t  T2WUFD0, *pT2WUFD0; // 0x0020			Wake up field D  LSB						RW			0x3FFF
extern uint16_t  T2WUFD1, *pT2WUFD1; // 0x0024			Wake up field D  MSB						RW			0x0000
extern uint16_t  T2IEN, *pT2IEN;     // 0x0028			Interrupt enable							RW			0x0000
extern uint16_t  T2STA, *pT2STA;     // 0x002C			Status										R			0x0000
extern uint16_t  T2CLRI, *pT2CLRI;   // 0x0030			Clear interrupts. Write only.				W			N/A
extern uint16_t  T2WUFA0, *pT2WUFA0; // 0x003C			Wake up field A  LSB.						RW			0x1900
extern uint16_t  T2WUFA1, *pT2WUFA1; // 0x0040			Wake up field A MSB.						RW			0x0000

// GP0 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0000
//										               Address Offset		Description							Access Type		Default(Reset Value)  
extern uint16_t  GPT0LD, *pGPT0LD;     // 0x0000      16-bit load value           RW            0x0000
extern uint16_t  GPT0VAL, *pGPT0VAL;   // 0x0004      16-bit timer value          R             0x0000
extern uint16_t  GPT0CON, *pGPT0CON;   // 0x0008      Control Register            RW            0x000A
extern uint16_t  GPT0CLRI, *pGPT0CLRI; // 0x000C      Clear interrupt register    RW            0x0000
extern uint16_t  GPT0CAP, *pGPT0CAP;   // 0x0010      Capture Register            R             0x0000
extern uint16_t  GPT0STA, *pGPT0STA;   // 0x001C      Status Register             R             0x0000

// GP1 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0400
//										               Address Offset		Description							Access Type		Default(Reset Value)  
extern uint16_t  GPT1LD, *pGPT1LD;     // 0x0000      16-bit load value           RW            0x0000
extern uint16_t  GPT1VAL, *pGPT1VAL;   // 0x0004      16-bit timer value          R             0x0000
extern uint16_t  GPT1CON, *pGPT1CON;   // 0x0008      Control Register            RW            0x000A
extern uint16_t  GPT1CLRI, *pGPT1CLRI; // 0x000C      Clear interrupt register    RW            0x0000
extern uint16_t  GPT1CAP, *pGPT1CAP;   // 0x0010      Capture Register            R             0x0000
extern uint16_t  GPT1STA, *pGPT1STA;   // 0x001C      Status Register             R             0x0000

// PMU Memory Mapped Registers(MMRs) : Base Address : 0x4000_2400
//																	 Address Offset		Description							Access Type		Default(Reset Value)  
extern uint8_t PWR_MOD, *pPWR_MOD; 	// 0x0000				Power management Unit, 			RW						0x0000
extern uint16_t PWR_KEY, *pPWR_KEY; // 0x0004				Power Management Unit Key,	RW						N/A

// PWM Memory Mapped Registers(MMRs) : Base Address : 0x4000_1000
//																	 Address Offset		Description														Access Type		Default(Reset Value) 
extern uint16_t  PWMCON0 , *pPWMCON0 ;  // 0x000    PWM Control register                       RW              0x0012
extern uint8_t   PWMCON1 , *pPWMCON1 ;  // 0x004    Trip control register                      RW8             0x0000
extern uint16_t  PWMCLRI , *pPWMCLRI ;  // 0x008    PWM interrupt clear.                       W               0x0000
extern uint16_t  PWM0COM0, *pPWM0COM0;  // 0x010    Compare Register 0 for PWM0 and PWM1       RW              0x0000
extern uint16_t  PWM0COM1, *pPWM0COM1;  // 0x014    Compare Register 1 for PWM0 and PWM1       RW              0x0000
extern uint16_t  PWM0COM2, *pPWM0COM2;  // 0x018    Compare Register 2 for PWM0 and PWM1       RW              0x0000
extern uint16_t  PWM0LEN , *pPWM0LEN ;  // 0x01C    Period Value register for PWM0 and PWM1    RW              0x0000
extern uint16_t  PWM1COM0, *pPWM1COM0;  // 0x020    Compare Register 0 for PWM2 and PWM3       RW              0x0000
extern uint16_t  PWM1COM1, *pPWM1COM1;  // 0x024    Compare Register 1 for PWM2 and PWM3       RW              0x0000
extern uint16_t  PWM1COM2, *pPWM1COM2;  // 0x028    Compare Register 2 for PWM2 and PWM3       RW              0x0000
extern uint16_t  PWM1LEN , *pPWM1LEN ;  // 0x02C    Period Value register for PWM2 and PWM3    RW              0x0000
extern uint16_t  PWM2COM0, *pPWM2COM0;  // 0x030    Compare Register 0 for PWM4 and PWM5       RW              0x0000
extern uint16_t  PWM2COM1, *pPWM2COM1;  // 0x034    Compare Register 1 for PWM4 and PWM5       RW              0x0000
extern uint16_t  PWM2COM2, *pPWM2COM2;  // 0x038    Compare Register 2 for PWM4 and PWM5       RW              0x0000
extern uint16_t  PWM2LEN , *pPWM2LEN ;  // 0x03C    Period Value register for PWM4 and PWM5    RW              0x0000

// Flash Controller Memory Mapped Registers(MMRs) : Base Address : 0x4000_2800
//											                Address Offset		   Description								                                  Access Type		Default(Reset Value)
extern uint16_t  FEESTA, *pFEESTA;      // 0x0000       Status Register                                                     R             0x0000
extern uint16_t  FEECON0, *pFEECON0;     // 0x0004       Command Control Register                                            RW            0x0000
extern uint16_t  FEECMD, *pFEECMD;      // 0x0008       Command register                                                    RW            0x0000
extern uint16_t  FEEADR0L, *pFEEADR0L;    // 0x0010       Low Page address  (Lower 16 bits)                                   RW            0x0000
extern uint16_t  FEEADR0H, *pFEEADR0H;    // 0x0014       Low Page address  (Upper 16 bits)                                   RW            0x0000
extern uint16_t  FEEADR1L, *pFEEADR1L;    // 0x0018       Hi Page  address (Lower 16 bits)                                    RW            0x0000
extern uint16_t  FEEADR1H, *pFEEADR1H;    // 0x001C       Hi Page  address (Upper 16 bits)                                    RW            0x0000
extern uint16_t  FEEKEY, *pFEEKEY;      // 0x0020       Key                                                                 W             0x0000
extern uint16_t  FEEPROL, *pFEEPROL;     // 0x0028       Write Protection (Lower 16 bits)                                    RW            0xFFFF
extern uint16_t  FEEPROH, *pFEEPROH;     // 0x002C       Write Protection (Upper 16 bits)                                    RW            0xFFFF
extern uint16_t  FEESIGL, *pFEESIGL;     // 0x0030       Signature (Lower 16 bits)                                           R             0x0000
extern uint16_t  FEESIGH, *pFEESIGH;     // 0x0034       Signature (Upper 16 bits)                                           R             0x0000
extern uint16_t  FEECON1, *pFEECON1;     // 0x0038       User Setup register                                                 RW            0x0000
extern uint16_t  FEEADRAL, *pFEEADRAL;    // 0x0048       Lower Abort address (Lower 16 bits)                                 R             0x0800
extern uint16_t  FEEADRAH, *pFEEADRAH;    // 0x004C       Upper Abort address (Upper 16 bits)                                 R             0x0002
extern uint16_t  FEEAEN0, *pFEEAEN0;     // 0x0078       Interrupt abort enable register. Interrupt 15 to Interrupt 0.       RW            0x0000
extern uint16_t  FEEAEN1, *pFEEAEN1;     // 0x007C       Interrupt abort enable register. Interrupt 31 to Interrupt 16.      RW            0x0000
extern uint16_t  FEEAEN2, *pFEEAEN2;     // 0x0080       Interrupt abort enable register. Interrupt 32 to Interrupt 38.      RW            0x0000

// ADC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4003_0000
//											                   Address Offset		       Description								                                                          Access Type		Default(Reset Value)
// Only ADuCM360                          
extern uint8_t   ADC0STA, *pADC0STA;      // 0x3_0000          ADC Status register                                                                               R             0x0000  
extern uint8_t   ADC0MSKI, *pADC0MSKI;    // 0x3_0004          Interrupt control register                                                                        RW            0x0000
extern uint32_t  ADC0CON, *pADC0CON;      // 0x3_0008          Main control register                                                                             RW            0x3038C
extern uint16_t  ADC0OF, *pADC0OF;        // 0x3_000C          Offset calibration register                                                                       RW            0x0000
extern uint16_t  ADC0INTGN, *pADC0INTGN;  // 0x3_0010          Gain calibration register when using internal reference                                           RW            0x0000
extern uint16_t  ADC0EXTGN, *pADC0EXTGN;  // 0x3_0014          Gain calibration register when using external reference                                           RW            0x0000
extern uint16_t  ADC0VDDGN, *pADC0VDDGN;  // 0x3_0018          Gain calibration register when using AVDD as the ADC reference                                    RW            0x5555
extern uint16_t  ADC0CFG, *pADC0CFG;      // 0x3_001C          Control register for the VBIAS voltage generator, ground switch and external reference buffer     RW            0x0000
extern uint16_t  ADC0FLT, *pADC0FLT;      // 0x3_0020          Filter configuration register                                                                     RW            0x007D
extern uint16_t  ADC0MDE, *pADC0MDE;      // 0x3_0024          mode control register                                                                             RW            0x0003
extern uint16_t  ADC0RCR, *pADC0RCR;      // 0x3_0028          Number of ADC0 conversions before an ADC interrupt is generated.                                  RW            0x0001
extern uint16_t  ADC0RCV, *pADC0RCV;      // 0x3_002C          This 16-bit, read-only MMR holds the current number of ADC0 conversion results                    R             0x0000
extern uint16_t  ADC0TH, *pADC0TH;        // 0x3_0030          Sets the threshold                                                                                RW            0x0000
extern uint8_t   ADC0THC, *pADC0THC;      // 0x3_0034          Determines how many cumulative ADC0 conversion result readings above ADC0TH must occur            RW            0x0001
extern uint8_t   ADC0THV, *pADC0THV;      // 0x3_0038          8-bit threshold exceeded counter register                                                         R             0x0000
extern uint32_t  ADC0ACC, *pADC0ACC;      // 0x3_003C          32-bit accumulator register                                                                       R             0x0000
extern uint32_t  ADC0ATH, *pADC0ATH;      // 0x3_0040          Holds the threshold value for the accumulator comparator                                          RW            0x0000
extern uint8_t   ADC0PRO, *pADC0PRO;      // 0x3_0044          Configuration register for Post processing of ADC0 results                                        RW            0x0000
extern uint32_t  ADC0DAT, *pADC0DAT;      // 0x3_0048          conversion result register                                                                        R             0x0000
// Both ADuCM360/ADuCM361                                                                                                                                  
extern uint8_t   ADC1STA, *pADC1STA;      // 0x3_0080          ADC Status register                                                                               R             0x0000
extern uint8_t   ADC1MSKI, *pADC1MSKI;    // 0x3_0084          Interrupt control register                                                                        RW            0x0000
extern uint32_t  ADC1CON, *pADC1CON;      // 0x3_0088          Main control register                                                                             RW            0x303FF
extern uint16_t  ADC1OF, *pADC1OF;        // 0x3_008C          Offset calibration register                                                                       RW            0x0000
extern uint16_t  ADC1INTGN, *pADC1INTGN;  // 0x3_0090          Gain calibration register when using internal reference                                           RW            0x0000
extern uint16_t  ADC1EXTGN, *pADC1EXTGN;  // 0x3_0094          Gain calibration register when using external reference                                           RW            0x0000
extern uint16_t  ADC1VDDGN, *pADC1VDDGN;  // 0x3_0098          Gain calibration register when using AVDD as the ADC reference                                    RW            0x5555
extern uint16_t  ADC1CFG, *pADC1CFG;      // 0x3_009C          Control register for the VBIAS voltage generator, ground switch and external reference buffer     RW            0x0000
extern uint16_t  ADC1FLT, *pADC1FLT;      // 0x3_00A0          Filter configuration register                                                                     RW            0x007D
extern uint16_t  ADC1MDE, *pADC1MDE;      // 0x3_00A4          mode control register                                                                             RW            0x0003
extern uint16_t  ADC1RCR, *pADC1RCR;      // 0x3_00A8          Number of ADC0 conversions before an ADC interrupt is generated.                                  RW            0x0001
extern uint16_t  ADC1RCV, *pADC1RCV;      // 0x3_00AC          This 16-bit, read-only MMR holds the current number of ADC0 conversion results                    R             0x0000
extern uint16_t  ADC1TH, *pADC1TH;        // 0x3_00B0          Sets the threshold                                                                                RW            0x0000
extern uint8_t   ADC1THC, *pADC1THC;      // 0x3_00B4          Determines how many cumulative ADC0 conversion result readings above ADC0TH must occur            RW            0x0001
extern uint8_t   ADC1THV, *pADC1THV;      // 0x3_00B8          8-bit threshold exceeded counter register                                                         R             0x0000
extern uint32_t  ADC1ACC, *pADC1ACC;      // 0x3_00BC          32-bit accumulator register                                                                       R             0x0000
extern uint32_t  ADC1ATH, *pADC1ATH;      // 0x3_00C0          Holds the threshold value for the accumulator comparator                                          RW            0x0000
extern uint8_t   ADC1PRO, *pADC1PRO;      // 0x3_00C4          Configuration register for Post processing of ADC0 results                                        RW            0x0000
extern uint32_t  ADC1DAT, *pADC1DAT;      // 0x3_00C8          conversion result register                                                                        R             0x0000
// ADCSTEP Registers :                                                                                                                                     
extern uint16_t  DETCON, *pDETCON;        // 0x3_00E0          Control register for reference detection and the step detection filter                            RW            0x0000
extern uint8_t   DETSTA, *pDETSTA;        // 0x3_00E4          Status register for detection                                                                     RW            0x0000
extern uint16_t  STEPTH, *pSTEPTH;        // 0x3_00E8          Threshold for step detection filter                                                               RW            0x0000
extern uint32_t  STEPDAT, *pSTEPDAT;      // 0x3_00EC          Offers coarse data from the output of the step detection filter                                   RW            0x0000
extern uint32_t  ADCDMACON, *pADCDMACON;  // 0x3_00F8          ADC DMA mode Configuratrion register                                                              RW            0x0000
// Other ADC Registers :                                                                                                                                   
extern uint16_t  REFCTRL, *pREFCTRL;      // 0x8840          Internal Reference Control register                                                                 RW            0x0000
extern uint8_t   IEXCCON, *pIEXCCON;      // 0x8850          Controls the on-chip Excitation Current Sources                                                     RW            0x00C0
extern uint8_t   IEXCDAT, *pIEXCDAT;      // 0x8854          Sets the output current setting for both Excitation Current sources                                 RW            0x0006

// DAC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4002_0000
//											                   Address Offset		       Description								                                                          Access Type		Default(Reset Value)
extern uint16_t  DACCON, *pDACCON;       // 0x0000             Control Register                      																												RW						 0x0200     
extern uint32_t  DACDAT, *pDACDAT;       // 0x0004             Data Register                         																												R 						 0x0000_0000	

// DMA Controller Registers(MMRs) :  Base Address : 0x4001_0000
//											                   Address Offset		       Description								                                                          Access Type		Default(Reset Value)
extern uint32_t  DMASTA, *pDMASTA;            // 0x0000       Returns the status of the controller when not in the reset state. 														R             0x000B_0000
extern uint32_t  DMACFG, *pDMACFG;            // 0x0004       Configuraton                                                                                  RW            0x0000_0000
extern uint32_t  DMAPDBPTR, *pDMAPDBPTR;      // 0x0008       Channel primary control database pointer                                                      RW            0x0000_0000
extern uint32_t  DMAADBPTR, *pDMAADBPTR;      // 0x000C       Channel alternate control database pointer                                                    R             0x0000_0100
extern uint32_t  DMASWREQ, *pDMASWREQ;        // 0x0014       Channel Software Request                                                                      R             0x0000_0000
extern uint32_t  DMARMSKSET, *pDMARMSKSET;    // 0x0020       Channel Request Mask Set                                                                      RW            0x0000_0000
extern uint32_t  DMARMSKCLR, *pDMARMSKCLR;    // 0x0024       Channel Request Mask Clear                                                                    R             0x0000_0000
extern uint32_t  DMAENSET, *pDMAENSET;        // 0x0028       Channel Enable Set                                                                            RW            0x0000_0000
extern uint32_t  DMAENCLR, *pDMAENCLR;        // 0x002C       Channel Enable Clear                                                                          W             0x0000_0000
extern uint32_t  DMAALTSET, *pDMAALTSET;      // 0x0030       Channel Primary-Alternate Set                                                                 RW            0x0000_0000
extern uint32_t  DMAALTCLR, *pDMAALTCLR;      // 0x0034       Channel Primary-Alternate Clear                                                               W             0x0000_0000
extern uint32_t  DMAPRISET, *pDMAPRISET;      // 0x0038       Channel Priority Set                                                                          RW            0x0000_0000
extern uint32_t  DMAPRICLR, *pDMAPRICLR;      // 0x003C       Channel Priority Clear                                                                        W             0x0000_0000
extern uint32_t  DMAERRCLR, *pDMAERRCLR;      // 0x004C       Bus Error Clear                                                                               RW            0x0000_0000
extern uint8_t   DMAPERID4, *pDMAPERID4;      // 0x0FD0       DMA Peripheral ID 4                                                                           R             0x0000_0004
extern uint8_t   DMAPERID0, *pDMAPERID0;      // 0x0FE0       DMA Peripheral ID 0                                                                           R             0x0000_0030
extern uint8_t   DMAPERID1, *pDMAPERID1;      // 0x0FE4       DMA Peripheral ID 1                                                                           R             0x0000_00B2
extern uint8_t   DMAPERID2, *pDMAPERID2;      // 0x0FE8       DMA Peripheral ID 2                                                                           R             0x0000_000B
extern uint8_t   DMAPERID3, *pDMAPERID3;      // 0x0FEC       DMA Peripheral ID 3                                                                           R             0x0000_0000
extern uint8_t   DMAPCELLID0, *pDMAPCELLID0;  // 0x0FF0       DMA PrimeCell ID 0                                                                            R             0x0000_000D
extern uint8_t   DMAPCELLID1, *pDMAPCELLID1;  // 0x0FF4       DMA PrimeCell ID 1                                                                            R             0x0000_00F0
extern uint8_t   DMAPCELLID2, *pDMAPCELLID2;  // 0x0FF8       DMA PrimeCell ID 2                                                                            R             0x0000_0005
extern uint8_t   DMAPCELLID3, *pDMAPCELLID3;  // 0x0FFC       DMA PrimeCell ID 3                                                                            R             0x0000_00B1

// I2C Controller Registers(MMRs) :  Base Address : 0x4000_3000
//											                   Address Offset		       Description								                                                          Access Type		Default(Reset Value)
extern uint16_t  I2CMCON, *pI2CMCON;         // 0x0000     		Master Control Register               																												RW						0x0000_0000
extern uint16_t  I2CMSTA, *pI2CMSTA;         // 0x0004     		Master Status Register                                                                        R             0x0000_0000
extern uint8_t   I2CMRX, *pI2CMRX;           // 0x0008     		Master Receive Data                                                                           R             0x0000_0000
extern uint8_t   I2CMTX, *pI2CMTX;           // 0x000C     		Master Transmit Data                                                                          W             0x0000_0000
extern uint16_t  I2CMRXCNT, *pI2CMRXCNT;     // 0x0010     		Master Receive Data Count                                                                     RW            0x0000_0000
extern uint16_t  I2CMCRXCNT, *pI2CMCRXCNT;   // 0x0014     		Master Current Receive Data Count                                                             R             0x0000_0000
extern uint8_t   I2CADR0, *pI2CADR0;         // 0x0018     		1st Master Address Byte                                                                       RW            0x0000_0000
extern uint8_t   I2CADR1, *pI2CADR1;         // 0x001C     		2nd Master Address Byte                                                                       RW            0x0000_0000
extern uint16_t  I2CDIV, *pI2CDIV;           // 0x0024     		Serial clock period divisor register                                                          RW            0x1F1F_1F1F
extern uint16_t  I2CSCON, *pI2CSCON;         // 0x0028     		Slave Control Register                                                                        RW            0x0000_0000
extern uint16_t  I2CSSTA, *pI2CSSTA;         // 0x002C     		"Slave I2C Status, Error and IRQ Register"                                                    R             0x0001_0001
extern uint16_t  I2CSRX, *pI2CSRX;           // 0x0030     		Slave Receive Data Register                                                                   R             0x0000_0000
extern uint16_t  I2CSTX, *pI2CSTX;           // 0x0034     		Slave Transmit Data Register                                                                  W             0x0000_0000
extern uint16_t  I2CALT, *pI2CALT;           // 0x0038     		Hardware General Call ID                                                                      RW            0x0000_0000
extern uint16_t  I2CID0, *pI2CID0;           // 0x003C     		1st Slave Address Device ID                                                                   RW            0x0000_0000
extern uint16_t  I2CID1, *pI2CID1;           // 0x0040     		2nd Slave Address Device ID                                                                   RW            0x0000_0000
extern uint16_t  I2CID2, *pI2CID2;           // 0x0044     		3rd Slave Address Device ID                                                                   RW            0x0000_0000
extern uint16_t  I2CID3, *pI2CID3;           // 0x0048     		4th Slave Address Device ID                                                                   RW            0x0000_0000
extern uint16_t  I2CFSTA, *pI2CFSTA;         // 0x004C     		Master and Slave Rx/Tx FIFO Status Register                                                   RW            0x0000_0000
  

void ADuCM360_RegsInit(void);
#else

// Cortex-M3/4 System Control Register(SCR)
extern volatile uint32_t *pSCR;

// Watchdog Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2580
extern volatile uint16_t *pT3LD;			
extern volatile uint16_t *pT3VAL;		
extern volatile uint16_t *pT3CON;		
extern volatile uint16_t *pT3CLRI;	
extern volatile uint16_t *pT3STA;		

// Clocking Memory Mapped Registers(MMRs) : Base Address : 0x4000_2000
extern volatile uint16_t *pCLKCON0;	
extern volatile uint16_t *pCLKCON1; 
extern volatile uint16_t *pCLKDIS;	
extern volatile uint16_t *pXOSCCON;	
extern volatile uint16_t *pCLKSYSDIV;

// GPIO Memory Mapped Registers(MMRs) : Base Address : 0x4000_6000
extern volatile uint16_t *pGP0CON;	
extern volatile uint8_t *pGP0OEN;		
extern volatile uint8_t *pGP0PUL;		
extern volatile uint8_t *pGP0OCE;		
extern volatile uint8_t *pGP0IN;		
extern volatile uint8_t *pGP0OUT;		
extern volatile uint8_t *pGP0SET;		
extern volatile uint8_t *pGP0CLR;		
extern volatile uint8_t *pGP0TGL;		

extern volatile uint16_t *pGP1CON;	
extern volatile uint8_t *pGP1OEN;		
extern volatile uint8_t *pGP1PUL;		
extern volatile uint8_t *pGP1OCE;		
extern volatile uint8_t *pGP1IN;		
extern volatile uint8_t *pGP1OUT;		
extern volatile uint8_t *pGP1SET;		
extern volatile uint8_t *pGP1CLR;		
extern volatile uint8_t *pGP1TGL;		

extern volatile uint16_t *pGP2CON;	
extern volatile uint8_t *pGP2OEN;		
extern volatile uint8_t *pGP2PUL;		
extern volatile uint8_t *pGP2OCE;		
extern volatile uint8_t *pGP2IN;		
extern volatile uint8_t *pGP2OUT;		
extern volatile uint8_t *pGP2SET;		
extern volatile uint8_t *pGP2CLR;		
extern volatile uint8_t *pGP2TGL;		

// UART Memory Mapped Registers(MMRs) : Base Address : 0x4000_5000
extern volatile uint8_t COMTX_RX, *pCOMTX_RX;
extern volatile uint8_t COMIEN, *pCOMIEN;
extern volatile uint8_t COMIIR, *pCOMIIR;
extern volatile uint8_t COMLCR, *pCOMLCR;
extern volatile uint8_t COMMCR, *pCOMMCR;
extern volatile uint8_t COMLSR, *pCOMLSR;
extern volatile uint8_t COMMSR, *pCOMMSR;
extern volatile uint16_t COMFBR, *pCOMFBR;
extern volatile uint16_t COMDIV, *pCOMDIV;
extern volatile uint8_t  COMCON, *pCOMCON;

// Wake-Up Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2500
extern volatile uint16_t  T2VAL0, *pT2VAL0;   
extern volatile uint16_t  T2VAL1, *pT2VAL1;   
extern volatile uint16_t  T2CON, *pT2CON;     
extern volatile uint16_t  T2INC, *pT2INC;     
extern volatile uint16_t  T2WUFB0, *pT2WUFB0; 
extern volatile uint16_t  T2WUFB1, *pT2WUFB1; 
extern volatile uint16_t  T2WUFC0, *pT2WUFC0; 
extern volatile uint16_t  T2WUFC1, *pT2WUFC1; 
extern volatile uint16_t  T2WUFD0, *pT2WUFD0; 
extern volatile uint16_t  T2WUFD1, *pT2WUFD1; 
extern volatile uint16_t  T2IEN, *pT2IEN;     
extern volatile uint16_t  T2STA, *pT2STA;     
extern volatile uint16_t  T2CLRI, *pT2CLRI;   
extern volatile uint16_t  T2WUFA0, *pT2WUFA0; 
extern volatile uint16_t  T2WUFA1, *pT2WUFA1; 

// GP0 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0000
extern volatile uint16_t  GPT0LD, *pGPT0LD;     
extern volatile uint16_t  GPT0VAL, *pGPT0VAL;   
extern volatile uint16_t  GPT0CON, *pGPT0CON;   
extern volatile uint16_t  GPT0CLRI, *pGPT0CLRI; 
extern volatile uint16_t  GPT0CAP, *pGPT0CAP;   
extern volatile uint16_t  GPT0STA, *pGPT0STA;   

// GP1 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0400
extern volatile uint16_t  GPT1LD, *pGPT1LD;     
extern volatile uint16_t  GPT1VAL, *pGPT1VAL;   
extern volatile uint16_t  GPT1CON, *pGPT1CON;   
extern volatile uint16_t  GPT1CLRI, *pGPT1CLRI; 
extern volatile uint16_t  GPT1CAP, *pGPT1CAP;   
extern volatile uint16_t  GPT1STA, *pGPT1STA;   


// PMU Memory Mapped Registers(MMRs) : Base Address : 0x4000_2400
extern volatile uint8_t PWR_MOD, *pPWR_MOD;
extern volatile uint16_t PWR_KEY, *pPWR_KEY;

// PWM Memory Mapped Registers(MMRs) : Base Address : 0x4000_1000
extern volatile uint16_t  PWMCON0 , *pPWMCON0 ;
extern volatile uint8_t   PWMCON1 , *pPWMCON1 ;
extern volatile uint16_t  PWMCLRI , *pPWMCLRI ;
extern volatile uint16_t  PWM0COM0, *pPWM0COM0;
extern volatile uint16_t  PWM0COM1, *pPWM0COM1;
extern volatile uint16_t  PWM0COM2, *pPWM0COM2;
extern volatile uint16_t  PWM0LEN , *pPWM0LEN ;
extern volatile uint16_t  PWM1COM0, *pPWM1COM0;
extern volatile uint16_t  PWM1COM1, *pPWM1COM1;
extern volatile uint16_t  PWM1COM2, *pPWM1COM2;
extern volatile uint16_t  PWM1LEN , *pPWM1LEN ;
extern volatile uint16_t  PWM2COM0, *pPWM2COM0;
extern volatile uint16_t  PWM2COM1, *pPWM2COM1;
extern volatile uint16_t  PWM2COM2, *pPWM2COM2;
extern volatile uint16_t  PWM2LEN , *pPWM2LEN ;

// Flash Controller Memory Mapped Registers(MMRs) : Base Address : 0x4000_2800
extern volatile uint16_t  FEESTA, *pFEESTA;     
extern volatile uint16_t  FEECON0, *pFEECON0;    
extern volatile uint16_t  FEECMD, *pFEECMD;     
extern volatile uint16_t  FEEADR0L, *pFEEADR0L;  
extern volatile uint16_t  FEEADR0H, *pFEEADR0H;  
extern volatile uint16_t  FEEADR1L, *pFEEADR1L;  
extern volatile uint16_t  FEEADR1H, *pFEEADR1H;  
extern volatile uint16_t  FEEKEY, *pFEEKEY;     
extern volatile uint16_t  FEEPROL, *pFEEPROL;    
extern volatile uint16_t  FEEPROH, *pFEEPROH;    
extern volatile uint16_t  FEESIGL, *pFEESIGL;    
extern volatile uint16_t  FEESIGH, *pFEESIGH;    
extern volatile uint16_t  FEECON1, *pFEECON1;    
extern volatile uint16_t  FEEADRAL, *pFEEADRAL;  
extern volatile uint16_t  FEEADRAH, *pFEEADRAH;  
extern volatile uint16_t  FEEAEN0, *pFEEAEN0;    
extern volatile uint16_t  FEEAEN1, *pFEEAEN1;    
extern volatile uint16_t  FEEAEN2, *pFEEAEN2;    

// ADC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4003_0000
// Only ADuCM360                          
extern volatile uint8_t   ADC0STA, *pADC0STA;       
extern volatile uint8_t   ADC0MSKI, *pADC0MSKI;    
extern volatile uint32_t  ADC0CON, *pADC0CON;      
extern volatile uint16_t  ADC0OF, *pADC0OF;        
extern volatile uint16_t  ADC0INTGN, *pADC0INTGN;  
extern volatile uint16_t  ADC0EXTGN, *pADC0EXTGN;  
extern volatile uint16_t  ADC0VDDGN, *pADC0VDDGN;  
extern volatile uint16_t  ADC0CFG, *pADC0CFG;      
extern volatile uint16_t  ADC0FLT, *pADC0FLT;      
extern volatile uint16_t  ADC0MDE, *pADC0MDE;      
extern volatile uint16_t  ADC0RCR, *pADC0RCR;      
extern volatile uint16_t  ADC0RCV, *pADC0RCV;      
extern volatile uint16_t  ADC0TH, *pADC0TH;        
extern volatile uint8_t   ADC0THC, *pADC0THC;      
extern volatile uint8_t   ADC0THV, *pADC0THV;      
extern volatile uint32_t  ADC0ACC, *pADC0ACC;      
extern volatile uint32_t  ADC0ATH, *pADC0ATH;      
extern volatile uint8_t   ADC0PRO, *pADC0PRO;      
extern volatile uint32_t  ADC0DAT, *pADC0DAT;      
// Both ADuCM360/ADuCM361                                                                                                                                  
extern volatile int8_t   ADC1STA, *pADC1STA;     
extern volatile int8_t   ADC1MSKI, *pADC1MSKI;   
extern volatile int32_t  ADC1CON, *pADC1CON;     
extern volatile int16_t  ADC1OF, *pADC1OF;       
extern volatile int16_t  ADC1INTGN, *pADC1INTGN; 
extern volatile int16_t  ADC1EXTGN, *pADC1EXTGN; 
extern volatile int16_t  ADC1VDDGN, *pADC1VDDGN; 
extern volatile int16_t  ADC1CFG, *pADC1CFG;     
extern volatile int16_t  ADC1FLT, *pADC1FLT;     
extern volatile int16_t  ADC1MDE, *pADC1MDE;     
extern volatile int16_t  ADC1RCR, *pADC1RCR;     
extern volatile int16_t  ADC1RCV, *pADC1RCV;     
extern volatile int16_t  ADC1TH, *pADC1TH;       
extern volatile int8_t   ADC1THC, *pADC1THC;     
extern volatile int8_t   ADC1THV, *pADC1THV;     
extern volatile int32_t  ADC1ACC, *pADC1ACC;     
extern volatile int32_t  ADC1ATH, *pADC1ATH;     
extern volatile int8_t   ADC1PRO, *pADC1PRO;     
extern volatile int32_t  ADC1DAT, *pADC1DAT;     
// ADCSTEP Registers :                                                                                                                                     
extern volatile uint16_t  DETCON, *pDETCON;        
extern volatile uint8_t   DETSTA, *pDETSTA;        
extern volatile uint16_t  STEPTH, *pSTEPTH;        
extern volatile uint32_t  STEPDAT, *pSTEPDAT;      
extern volatile uint32_t  ADCDMACON, *pADCDMACON;  
// Other ADC Registers :                                                                                                                                   
extern volatile uint16_t  REFCTRL, *pREFCTRL;     
extern volatile uint8_t   IEXCCON, *pIEXCCON;     
extern volatile uint8_t   IEXCDAT, *pIEXCDAT;     

// DAC Registers :                                                                                                                                   
extern volatile uint16_t  DACCON, *pDACCON; 
extern volatile uint32_t  DACDAT, *pDACDAT; 

// DMA Controller Registers(MMRs) :  Base Address : 0x4001_0000
extern volatile uint32_t  DMASTA, *pDMASTA;            
extern volatile uint32_t  DMACFG, *pDMACFG;            
extern volatile uint32_t  DMAPDBPTR, *pDMAPDBPTR;      
extern volatile uint32_t  DMAADBPTR, *pDMAADBPTR;      
extern volatile uint32_t  DMASWREQ, *pDMASWREQ;        
extern volatile uint32_t  DMARMSKSET, *pDMARMSKSET;    
extern volatile uint32_t  DMARMSKCLR, *pDMARMSKCLR;    
extern volatile uint32_t  DMAENSET, *pDMAENSET;        
extern volatile uint32_t  DMAENCLR, *pDMAENCLR;        
extern volatile uint32_t  DMAALTSET, *pDMAALTSET;      
extern volatile uint32_t  DMAALTCLR, *pDMAALTCLR;      
extern volatile uint32_t  DMAPRISET, *pDMAPRISET;      
extern volatile uint32_t  DMAPRICLR, *pDMAPRICLR;      
extern volatile uint32_t  DMAERRCLR, *pDMAERRCLR;      
extern volatile uint8_t   DMAPERID4, *pDMAPERID4;      
extern volatile uint8_t   DMAPERID0, *pDMAPERID0;      
extern volatile uint8_t   DMAPERID1, *pDMAPERID1;      
extern volatile uint8_t   DMAPERID2, *pDMAPERID2;      
extern volatile uint8_t   DMAPERID3, *pDMAPERID3;      
extern volatile uint8_t   DMAPCELLID0, *pDMAPCELLID0;  
extern volatile uint8_t   DMAPCELLID1, *pDMAPCELLID1;  
extern volatile uint8_t   DMAPCELLID2, *pDMAPCELLID2;  
extern volatile uint8_t   DMAPCELLID3, *pDMAPCELLID3;  

// I2C Controller Registers(MMRs) :  Base Address : 0x4000_3000                                                                                                                                     
extern volatile uint16_t  I2CMCON, *pI2CMCON;         
extern volatile uint16_t  I2CMSTA, *pI2CMSTA;         
extern volatile uint8_t   I2CMRX, *pI2CMRX;           
extern volatile uint8_t   I2CMTX, *pI2CMTX;           
extern volatile uint16_t  I2CMRXCNT, *pI2CMRXCNT;     
extern volatile uint16_t  I2CMCRXCNT, *pI2CMCRXCNT;   
extern volatile uint8_t   I2CADR0, *pI2CADR0;         
extern volatile uint8_t   I2CADR1, *pI2CADR1;         
extern volatile uint16_t  I2CDIV, *pI2CDIV;           
extern volatile uint16_t  I2CSCON, *pI2CSCON;         
extern volatile uint16_t  I2CSSTA, *pI2CSSTA;         
extern volatile uint16_t  I2CSRX, *pI2CSRX;           
extern volatile uint16_t  I2CSTX, *pI2CSTX;           
extern volatile uint16_t  I2CALT, *pI2CALT;           
extern volatile uint16_t  I2CID0, *pI2CID0;           
extern volatile uint16_t  I2CID1, *pI2CID1;           
extern volatile uint16_t  I2CID2, *pI2CID2;           
extern volatile uint16_t  I2CID3, *pI2CID3;           
extern volatile uint16_t  I2CFSTA, *pI2CFSTA;         


#endif
