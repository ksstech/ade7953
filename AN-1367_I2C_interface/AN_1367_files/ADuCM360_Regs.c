/*

   This sample code is developed by Fermi Lim(fermi.lim@analog.com).
   Date : 1/30/2015 
   Feature : To access any internal registers of ADE7953.
   Written by Fermi Lim(Korean Name : JongSu Lim.

*/


#include <stdint.h>

#if FermiEmulation_Mode

#include "ADuCM360_Regs.h"

// Cortex-M3/4 System Control Register(SCR)
uint32_t  SCR = 0x00000000, *pSCR;

// Watchdog Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2580
uint16_t T3LD	=0x1000,	*pT3LD;			
uint16_t T3VAL	=0x1000,	*pT3VAL;		
uint16_t T3CON	=0x00E9,	*pT3CON;		
uint16_t T3CLRI	=0x0000,	*pT3CLRI;		// Reset Value is not available!
uint16_t T3STA	=0x0000,	*pT3STA;		

// Clocking Memory Mapped Registers(MMRs) : Base Address : 0x4000_2000
uint16_t CLKCON0	=0x0004,	*pCLKCON0;		
uint16_t CLKCON1	=0x0000,	*pCLKCON1;  	
uint16_t CLKDIS		=0x01FF,	*pCLKDIS;		
uint16_t XOSCCON	=0x0000,	*pXOSCCON;		
uint16_t CLKSYSDIV	=0x0001,	*pCLKSYSDIV;

// GPIO Memory Mapped Registers(MMRs) : Base Address : 0x4000_6000
uint16_t GP0CON	=0x0000,*pGP0CON;	
uint8_t GP0OEN	=0x00,	*pGP0OEN;	
uint8_t GP0PUL	=0xFF,	*pGP0PUL;	
uint8_t GP0OCE	=0x00,	*pGP0OCE;	
uint8_t GP0IN	=0x00,	*pGP0IN;			// Reset Value is not available!
uint8_t GP0OUT	=0x00,	*pGP0OUT;	
uint8_t GP0SET	=0x00,	*pGP0SET;	
uint8_t GP0CLR	=0x00,	*pGP0CLR;	
uint8_t GP0TGL	=0x00,	*pGP0TGL;	

uint16_t GP1CON	=0x0000,*pGP1CON;	
uint8_t GP1OEN	=0x00,	*pGP1OEN;		
uint8_t GP1PUL	=0xFF,	*pGP1PUL;		
uint8_t GP1OCE	=0x00,	*pGP1OCE;		
uint8_t GP1IN	=0x00,	*pGP1IN;			// Reset Value is not available!	
uint8_t GP1OUT	=0x00,	*pGP1OUT;		
uint8_t GP1SET	=0x00,	*pGP1SET;		
uint8_t GP1CLR	=0x00,	*pGP1CLR;		
uint8_t GP1TGL	=0x00,	*pGP1TGL;		

uint16_t GP2CON	=0x0000,*pGP2CON;	
uint8_t GP2OEN	=0x00,	*pGP2OEN;		
uint8_t GP2PUL	=0xFF,	*pGP2PUL;		
uint8_t GP2OCE	=0x00,	*pGP2OCE;		
uint8_t GP2IN	=0x00,	*pGP2IN;			// Reset Value is not available!	
uint8_t GP2OUT	=0x00,	*pGP2OUT;		
uint8_t GP2SET	=0x00,	*pGP2SET;		
uint8_t GP2CLR	=0x00,	*pGP2CLR;		
uint8_t GP2TGL	=0x00,	*pGP2TGL;			

// UART Memory Mapped Registers(MMRs) : Base Address : 0x4000_5000
uint8_t COMTX_RX=0x0000, *pCOMTX_RX;	
uint8_t COMIEN=0x0000, *pCOMIEN;    
uint8_t COMIIR=0x0000, *pCOMIIR;    
uint8_t COMLCR=0x0000, *pCOMLCR;    
uint8_t COMMCR=0x0000, *pCOMMCR;    
uint8_t COMLSR=0x0000, *pCOMLSR;    
uint8_t COMMSR=0x0000, *pCOMMSR;    
uint16_t COMFBR=0x0000, *pCOMFBR;   
uint16_t COMDIV=0x0000, *pCOMDIV;   
uint8_t  COMCON=0x0000, *pCOMCON;   

// Wake-Up Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2500
uint16_t  T2VAL0=0x0000, *pT2VAL0;   
uint16_t  T2VAL1=0x0000, *pT2VAL1;   
uint16_t  T2CON=0x0040, *pT2CON;     
uint16_t  T2INC=0x00C8, *pT2INC;     
uint16_t  T2WUFB0=0x1FFF, *pT2WUFB0; 
uint16_t  T2WUFB1=0x0000, *pT2WUFB1; 
uint16_t  T2WUFC0=0x2FFF, *pT2WUFC0; 
uint16_t  T2WUFC1=0x0000, *pT2WUFC1; 
uint16_t  T2WUFD0=0x3FFF, *pT2WUFD0; 
uint16_t  T2WUFD1=0x0000, *pT2WUFD1; 
uint16_t  T2IEN=0x0000, *pT2IEN;     
uint16_t  T2STA=0x0000, *pT2STA;     
uint16_t  T2CLRI=0x0000, *pT2CLRI;   
uint16_t  T2WUFA0=0x1900, *pT2WUFA0; 
uint16_t  T2WUFA1=0x0000, *pT2WUFA1; 

// GP0 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0000
uint16_t  GPT0LD  =0x0000, *pGPT0LD;         
uint16_t  GPT0VAL =0x0000, *pGPT0VAL;       
uint16_t  GPT0CON =0x000A, *pGPT0CON;       
uint16_t  GPT0CLRI=0x0000, *pGPT0CLRI;     
uint16_t  GPT0CAP =0x0000, *pGPT0CAP;       
uint16_t  GPT0STA =0x0000, *pGPT0STA;       

// GP1 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0400
uint16_t  GPT1LD  =0x0000, *pGPT1LD;         
uint16_t  GPT1VAL =0x0000, *pGPT1VAL;       
uint16_t  GPT1CON =0x000A, *pGPT1CON;       
uint16_t  GPT1CLRI=0x0000, *pGPT1CLRI;     
uint16_t  GPT1CAP =0x0000, *pGPT1CAP;       
uint16_t  GPT1STA =0x0000, *pGPT1STA;       


// PMU Memory Mapped Registers(MMRs) : Base Address : 0x4000_2400
uint8_t  PWR_MOD = 0x0000, *pPWR_MOD;
uint16_t  PWR_KEY = 0x0000, *pPWR_KEY;

// PWM Memory Mapped Registers(MMRs) : Base Address : 0x4000_1000
uint16_t  PWMCON0 =0x0012, *pPWMCON0 ;
uint8_t   PWMCON1 =0x0000, *pPWMCON1 ;
uint16_t  PWMCLRI =0x0000, *pPWMCLRI ;
uint16_t  PWM0COM0=0x0000, *pPWM0COM0;
uint16_t  PWM0COM1=0x0000, *pPWM0COM1;
uint16_t  PWM0COM2=0x0000, *pPWM0COM2;
uint16_t  PWM0LEN =0x0000, *pPWM0LEN ;
uint16_t  PWM1COM0=0x0000, *pPWM1COM0;
uint16_t  PWM1COM1=0x0000, *pPWM1COM1;
uint16_t  PWM1COM2=0x0000, *pPWM1COM2;
uint16_t  PWM1LEN =0x0000, *pPWM1LEN ;
uint16_t  PWM2COM0=0x0000, *pPWM2COM0;
uint16_t  PWM2COM1=0x0000, *pPWM2COM1;
uint16_t  PWM2COM2=0x0000, *pPWM2COM2;
uint16_t  PWM2LEN =0x0000, *pPWM2LEN ;

// Flash Controller Memory Mapped Registers(MMRs) : Base Address : 0x4000_2800
uint16_t  FEESTA = 0x0000, *pFEESTA;                
uint16_t  FEECON0 =0x0000, *pFEECON0;              
uint16_t  FEECMD =0x0000, *pFEECMD;                
uint16_t  FEEADR0L =0x0000, *pFEEADR0L;            
uint16_t  FEEADR0H =0x0000, *pFEEADR0H;            
uint16_t  FEEADR1L =0x0000, *pFEEADR1L;            
uint16_t  FEEADR1H =0x0000, *pFEEADR1H;            
uint16_t  FEEKEY =0x0000, *pFEEKEY;                
uint16_t  FEEPROL =0xFFFF, *pFEEPROL;              
uint16_t  FEEPROH =0xFFFF, *pFEEPROH;              
uint16_t  FEESIGL =0x0000, *pFEESIGL;              
uint16_t  FEESIGH =0x0000, *pFEESIGH;              
uint16_t  FEECON1 =0x0000, *pFEECON1;              
uint16_t  FEEADRAL =0x0800, *pFEEADRAL;            
uint16_t  FEEADRAH =0x0002, *pFEEADRAH;            
uint16_t  FEEAEN0 =0x0000, *pFEEAEN0;              
uint16_t  FEEAEN1 =0x0000, *pFEEAEN1;              
uint16_t  FEEAEN2 =0x0000, *pFEEAEN2;              

// ADC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4003_0000                                                                                                         
// Only ADuCM360                                                                                                                                                                     
uint8_t   ADC0STA =0x0000, *pADC0STA;    
uint8_t   ADC0MSKI =0x0000, *pADC0MSKI;  
uint32_t  ADC0CON =0x80000, *pADC0CON;    
uint16_t  ADC0OF =0x0000,  *pADC0OF;      
uint16_t  ADC0INTGN =0x0000, *pADC0INTGN;
uint16_t  ADC0EXTGN =0x0000, *pADC0EXTGN;
uint16_t  ADC0VDDGN =0x5555, *pADC0VDDGN;
uint16_t  ADC0CFG =0x0000, *pADC0CFG;    
uint16_t  ADC0FLT =0x007D, *pADC0FLT;    
uint16_t  ADC0MDE =0x0000, *pADC0MDE;    
uint16_t  ADC0RCR =0x0001, *pADC0RCR;    
uint16_t  ADC0RCV =0x0000, *pADC0RCV;    
uint16_t  ADC0TH =0x0000, *pADC0TH;      
uint8_t   ADC0THC =0x0001, *pADC0THC;    
uint8_t   ADC0THV =0x0000, *pADC0THV;    
uint32_t  ADC0ACC =0x0000, *pADC0ACC;    
uint32_t  ADC0ATH =0x0000, *pADC0ATH;    
uint8_t   ADC0PRO =0x0000, *pADC0PRO;    
uint32_t  ADC0DAT =0x0000, *pADC0DAT;    
// Both ADuCM360/ADuCM361                                                                                                                                                            
uint8_t   ADC1STA =0x0000, *pADC1STA;     
uint8_t   ADC1MSKI =0x0000, *pADC1MSKI;   
uint32_t  ADC1CON =0x80000, *pADC1CON;     
uint16_t  ADC1OF =0x0000, *pADC1OF;       
uint16_t  ADC1INTGN =0x0000, *pADC1INTGN; 
uint16_t  ADC1EXTGN =0x0000, *pADC1EXTGN; 
uint16_t  ADC1VDDGN =0x5555, *pADC1VDDGN; 
uint16_t  ADC1CFG =0x0000, *pADC1CFG;     
uint16_t  ADC1FLT =0x007D, *pADC1FLT;     
uint16_t  ADC1MDE =0x0000, *pADC1MDE;     
uint16_t  ADC1RCR =0x0001, *pADC1RCR;     
uint16_t  ADC1RCV =0x0000, *pADC1RCV;     
uint16_t  ADC1TH =0x0000, *pADC1TH;       
uint8_t   ADC1THC =0x0001, *pADC1THC;     
uint8_t   ADC1THV =0x0000, *pADC1THV;     
uint32_t  ADC1ACC =0x0000, *pADC1ACC;     
uint32_t  ADC1ATH =0x0000, *pADC1ATH;     
uint8_t   ADC1PRO =0x0000, *pADC1PRO;     
uint32_t  ADC1DAT =0x0000, *pADC1DAT;     
// ADCSTEP Registers :                                                                                                                                                               
uint16_t  DETCON =0x0000, *pDETCON;       
uint8_t   DETSTA =0x0000, *pDETSTA;       
uint16_t  STEPTH =0x0000, *pSTEPTH;       
uint32_t  STEPDAT =0x0000, *pSTEPDAT;     
uint32_t  ADCDMACON =0x0000, *pADCDMACON; 
// Other ADC Registers :                                                                                                                                                             
uint16_t  REFCTRL =0x0000, *pREFCTRL;      
uint8_t   IEXCCON =0x00C0, *pIEXCCON;      
uint8_t   IEXCDAT =0x0006, *pIEXCDAT;      

// DAC Controller Registers(MMRs) :
uint16_t  DACCON = 0x0200, *pDACCON;   
uint32_t  DACDAT = 0x00000000, *pDACDAT;   

// DMA Controller Registers(MMRs) :  Base Address : 0x4001_0000
uint32_t  DMASTA = 0x000B0000, *pDMASTA;          
uint32_t  DMACFG = 0x00000000, *pDMACFG;          
uint32_t  DMAPDBPTR = 0x00000000, *pDMAPDBPTR;    
uint32_t  DMAADBPTR = 0x00000100, *pDMAADBPTR;    
uint32_t  DMASWREQ = 0x00000000, *pDMASWREQ;      
uint32_t  DMARMSKSET = 0x00000000, *pDMARMSKSET;  
uint32_t  DMARMSKCLR = 0x00000000, *pDMARMSKCLR;  
uint32_t  DMAENSET = 0x00000000, *pDMAENSET;      
uint32_t  DMAENCLR = 0x00000000, *pDMAENCLR;      
uint32_t  DMAALTSET = 0x00000000, *pDMAALTSET;    
uint32_t  DMAALTCLR = 0x00000000, *pDMAALTCLR;    
uint32_t  DMAPRISET = 0x00000000, *pDMAPRISET;    
uint32_t  DMAPRICLR = 0x00000000, *pDMAPRICLR;    
uint32_t  DMAERRCLR = 0x00000000, *pDMAERRCLR;    
uint8_t   DMAPERID4 = 0x00000004, *pDMAPERID4;    
uint8_t   DMAPERID0 = 0x00000030, *pDMAPERID0;    
uint8_t   DMAPERID1 = 0x000000B2, *pDMAPERID1;    
uint8_t   DMAPERID2 = 0x0000000B, *pDMAPERID2;    
uint8_t   DMAPERID3 = 0x00000000, *pDMAPERID3;    
uint8_t   DMAPCELLID0 = 0x0000000D, *pDMAPCELLID0;
uint8_t   DMAPCELLID1 = 0x000000F0, *pDMAPCELLID1;
uint8_t   DMAPCELLID2 = 0x00000005, *pDMAPCELLID2;
uint8_t   DMAPCELLID3 = 0x000000B1, *pDMAPCELLID3;

// I2C Controller Registers(MMRs) :  Base Address : 0x4000_3000
uint16_t  I2CMCON = 0x00000000, *pI2CMCON;              /* Master Control Register               */
uint16_t  I2CMSTA = 0x00000000, *pI2CMSTA;              /* Master Status Register                */
uint8_t   I2CMRX = 0x00000000, *pI2CMRX;                /* Master Receive Data                   */
uint8_t   I2CMTX = 0x00000000, *pI2CMTX;                /* Master Transmit Data                  */
uint16_t  I2CMRXCNT = 0x00000000, *pI2CMRXCNT;          /* Master Receive Data Count             */
uint16_t  I2CMCRXCNT = 0x00000000, *pI2CMCRXCNT;        /* Master Current Receive Data Count     */
uint8_t   I2CADR0 = 0x00000000, *pI2CADR0;              /* 1st Master Address Byte               */
uint8_t   I2CADR1 = 0x00000000, *pI2CADR1;              /* 2nd Master Address Byte               */
uint16_t  I2CDIV = 0x1F1F, *pI2CDIV;                /* Serial clock period divisor register  */
uint16_t  I2CSCON = 0x00000000, *pI2CSCON;              /* Slave Control Register                */
uint16_t  I2CSSTA = 0x0001, *pI2CSSTA;              /* "Slave I2C Status, Error and IRQ Register" */
uint16_t  I2CSRX = 0x00000000, *pI2CSRX;                /* Slave Receive Data Register           */
uint16_t  I2CSTX = 0x00000000, *pI2CSTX;                /* Slave Transmit Data Register          */
uint16_t  I2CALT = 0x00000000, *pI2CALT;                /* Hardware General Call ID              */
uint16_t  I2CID0 = 0x00000000, *pI2CID0;                /* 1st Slave Address Device ID           */
uint16_t  I2CID1 = 0x00000000, *pI2CID1;                /* 2nd Slave Address Device ID           */
uint16_t  I2CID2 = 0x00000000, *pI2CID2;                /* 3rd Slave Address Device ID           */
uint16_t  I2CID3 = 0x00000000, *pI2CID3;                /* 4th Slave Address Device ID           */
uint16_t  I2CFSTA = 0x00000000, *pI2CFSTA;              /* Master and Slave Rx/Tx FIFO Status Register */

void ADuCM360_RegsInit(void)
{
	// Cortex-M3/4 System Control Register(SCR)
  pSCR = &SCR;
	
	// Watchdog Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2580
	pT3LD	=&T3LD;			
	pT3VAL	=&T3VAL;		
	pT3CON	=&T3CON;		
	pT3CLRI	=&T3CLRI;	
	pT3STA	=&T3STA;		

	// Clocking Memory Mapped Registers(MMRs) : Base Address : 0x4000_2000
	pCLKCON0	=&CLKCON0;	
	pCLKCON1	=&CLKCON1; 
	pCLKDIS		=&CLKDIS;	
	pXOSCCON	=&XOSCCON;	
	pCLKSYSDIV	=&CLKSYSDIV;

	// GPIO Memory Mapped Registers(MMRs) : Base Address : 0x4000_6000
	pGP0CON =&GP0CON;	
	pGP0OEN =&GP0OEN;		
	pGP0PUL =&GP0PUL;		
	pGP0OCE =&GP0OCE;		
	pGP0IN  =&GP0IN;		
	pGP0OUT =&GP0OUT;		
	pGP0SET =&GP0SET;		
	pGP0CLR =&GP0CLR;		
	pGP0TGL =&GP0TGL;		

	pGP1CON=&GP1CON;	
	pGP1OEN=&GP1OEN;		
	pGP1PUL=&GP1PUL;		
	pGP1OCE=&GP1OCE;		
	pGP1IN =&GP1IN;		
	pGP1OUT=&GP1OUT;		
	pGP1SET=&GP1SET;		
	pGP1CLR=&GP1CLR;		
	pGP1TGL=&GP1TGL;		

	pGP2CON=&GP2CON;	
	pGP2OEN=&GP2OEN;		
	pGP2PUL=&GP2PUL;		
	pGP2OCE=&GP2OCE;		
	pGP2IN =&GP2IN;		
	pGP2OUT=&GP2OUT;		
	pGP2SET=&GP2SET;		
	pGP2CLR=&GP2CLR;		
	pGP2TGL=&GP2TGL;		
	
	// UART Memory Mapped Registers(MMRs) : Base Address : 0x4000_5000
  pCOMTX_RX=&COMTX_RX;
  pCOMIEN  =&COMIEN; 
  pCOMIIR  =&COMIIR; 
  pCOMLCR  =&COMLCR; 
  pCOMMCR  =&COMMCR; 
  pCOMLSR  =&COMLSR; 
  pCOMMSR  =&COMMSR; 
  pCOMFBR  =&COMFBR;
  pCOMDIV  =&COMDIV;
  pCOMCON  =&COMCON;

  // Wake-Up Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2500
  pT2VAL0=&T2VAL0;
  pT2VAL1=&T2VAL1;
  pT2CON=&T2CON;
  pT2INC=&T2INC; 
  pT2WUFB0=&T2WUFB0;
  pT2WUFB1=&T2WUFB1;
  pT2WUFC0=&T2WUFC0;
  pT2WUFC1=&T2WUFC1;
  pT2WUFD0=&T2WUFD0;
  pT2WUFD1=&T2WUFD1;
  pT2IEN=&T2IEN;
  pT2STA=&T2STA;
  pT2CLRI=&T2CLRI;
  pT2WUFA0=&T2WUFA0;
  pT2WUFA1=&T2WUFA1;

// GP0 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0000
  pGPT0LD  =&GPT0LD;         
  pGPT0VAL =&GPT0VAL;       
  pGPT0CON =&GPT0CON;       
  pGPT0CLRI=&GPT0CLRI;     
  pGPT0CAP =&GPT0CAP;       
  pGPT0STA =&GPT0STA;       

// GP1 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0400
  pGPT1LD  =&GPT1LD;         
  pGPT1VAL =&GPT1VAL;       
  pGPT1CON =&GPT1CON;       
  pGPT1CLRI=&GPT1CLRI;     
  pGPT1CAP =&GPT1CAP;       
  pGPT1STA =&GPT1STA;      
  
// PMU Memory Mapped Registers(MMRs) : Base Address : 0x4000_2400
  pPWR_MOD =&PWR_MOD;
  pPWR_KEY =&PWR_KEY;

// PWM Memory Mapped Registers(MMRs) : Base Address : 0x4000_1000  
  pPWMCON0 =&PWMCON0 ;
  pPWMCON1 =&PWMCON1 ;
  pPWMCLRI =&PWMCLRI ;
  pPWM0COM0=&PWM0COM0;
  pPWM0COM1=&PWM0COM1;
  pPWM0COM2=&PWM0COM2;
  pPWM0LEN =&PWM0LEN ;
  pPWM1COM0=&PWM1COM0;
  pPWM1COM1=&PWM1COM1;
  pPWM1COM2=&PWM1COM2;
  pPWM1LEN =&PWM1LEN ;
  pPWM2COM0=&PWM2COM0;
  pPWM2COM1=&PWM2COM1;
  pPWM2COM2=&PWM2COM2;
  pPWM2LEN =&PWM2LEN ;
  
// Flash Controller Memory Mapped Registers(MMRs) : Base Address : 0x4000_2800
  pFEESTA = &FEESTA;             
  pFEECON0 = &FEECON0;             
  pFEECMD = &FEECMD;
  pFEEADR0L = &FEEADR0L;            
  pFEEADR0H = &FEEADR0H;            
  pFEEADR1L = &FEEADR1L;            
  pFEEADR1H = &FEEADR1H;            
  pFEEKEY = &FEEKEY;
  pFEEPROL = &FEEPROL;             
  pFEEPROH = &FEEPROH;             
  pFEESIGL = &FEESIGL;             
  pFEESIGH = &FEESIGH;             
  pFEECON1 = &FEECON1;             
  pFEEADRAL = &FEEADRAL;            
  pFEEADRAH = &FEEADRAH;            
  pFEEAEN0 = &FEEAEN0;             
  pFEEAEN1 = &FEEAEN1;             
  pFEEAEN2 = &FEEAEN2;  
  
  // ADC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4003_0000                      
// Only ADuCM360                                                                                  
  pADC0STA = &ADC0STA;                                                             
  pADC0MSKI = &ADC0MSKI;                                                           
  pADC0CON = &ADC0CON;                                                            
  pADC0OF = &ADC0OF;                                                              
  pADC0INTGN = &ADC0INTGN;                                                         
  pADC0EXTGN = &ADC0EXTGN;                                                         
  pADC0VDDGN = &ADC0VDDGN;                                                         
  pADC0CFG = &ADC0CFG;                                                             
  pADC0FLT = &ADC0FLT;                                                             
  pADC0MDE = &ADC0MDE;                                                             
  pADC0RCR = &ADC0RCR;                                                             
  pADC0RCV = &ADC0RCV;                                                             
  pADC0TH = &ADC0TH;                                                               
  pADC0THC = &ADC0THC;                                                             
  pADC0THV = &ADC0THV;                                                             
  pADC0ACC = &ADC0ACC;                                                             
  pADC0ATH = &ADC0ATH;                                                             
  pADC0PRO = &ADC0PRO;                                                             
  pADC0DAT = &ADC0DAT;                                                             
// Both ADuCM360/ADuCM361                                                                         
  pADC1STA = &ADC1STA;                                                             
  pADC1MSKI = &ADC1MSKI;                                                           
  pADC1CON = &ADC1CON;                                                            
  pADC1OF = &ADC1OF;                                                               
  pADC1INTGN = &ADC1INTGN;                                                         
  pADC1EXTGN = &ADC1EXTGN;                                                         
  pADC1VDDGN = &ADC1VDDGN;                                                         
  pADC1CFG = &ADC1CFG;                                                             
  pADC1FLT = &ADC1FLT;                                                             
  pADC1MDE = &ADC1MDE;                                                             
  pADC1RCR = &ADC1RCR;                                                             
  pADC1RCV = &ADC1RCV;                                                             
  pADC1TH = &ADC1TH;                                                               
  pADC1THC = &ADC1THC;                                                             
  pADC1THV = &ADC1THV;                                                             
  pADC1ACC = &ADC1ACC;                                                             
  pADC1ATH = &ADC1ATH;                                                             
  pADC1PRO = &ADC1PRO;                                                             
  pADC1DAT = &ADC1DAT;                                                             
// ADCSTEP Registers :                                                                            
  pDETCON = &DETCON;                                                               
  pDETSTA = &DETSTA;                                                               
  pSTEPTH = &STEPTH;                                                               
  pSTEPDAT = &STEPDAT;                                                             
  pADCDMACON = &ADCDMACON;                                                         
// Other ADC Registers :                                                                          
  pREFCTRL = &REFCTRL;                                                             
  pIEXCCON = &IEXCCON;                                                             
  pIEXCDAT = &IEXCDAT;                                                             
  
// DAC Controller Registers(MMRs) :
  pDACCON = &DACCON;   
  pDACDAT = &DACDAT;     
  
// DMA Controller Registers(MMRs) :  Base Address : 0x4001_0000
  pDMASTA = &DMASTA;       
  pDMACFG = &DMACFG;       
  pDMAPDBPTR = &DMAPDBPTR;    
  pDMAADBPTR = &DMAADBPTR;    
  pDMASWREQ = &DMASWREQ;     
  pDMARMSKSET = &DMARMSKSET;   
  pDMARMSKCLR = &DMARMSKCLR;   
  pDMAENSET = &DMAENSET;     
  pDMAENCLR = &DMAENCLR;     
  pDMAALTSET = &DMAALTSET;    
  pDMAALTCLR = &DMAALTCLR;    
  pDMAPRISET = &DMAPRISET;    
  pDMAPRICLR = &DMAPRICLR;    
  pDMAERRCLR = &DMAERRCLR;    
  pDMAPERID4 = &DMAPERID4;    
  pDMAPERID0 = &DMAPERID0;    
  pDMAPERID1 = &DMAPERID1;    
  pDMAPERID2 = &DMAPERID2;    
  pDMAPERID3 = &DMAPERID3;    
  pDMAPCELLID0 = &DMAPCELLID0;  
  pDMAPCELLID1 = &DMAPCELLID1;  
  pDMAPCELLID2 = &DMAPCELLID2;  
  pDMAPCELLID3 = &DMAPCELLID3;  

// I2C Controller Registers(MMRs) :  Base Address : 0x4000_3000
  pI2CMCON = &I2CMCON;              
  pI2CMSTA = &I2CMSTA;              
  pI2CMRX = &I2CMRX;                
  pI2CMTX = &I2CMTX;                
  pI2CMRXCNT = &I2CMRXCNT;          
  pI2CMCRXCNT = &I2CMCRXCNT;        
  pI2CADR0 = &I2CADR0;              
  pI2CADR1 = &I2CADR1;              
  pI2CDIV = &I2CDIV;                
  pI2CSCON = &I2CSCON;              
  pI2CSSTA = &I2CSSTA;              
  pI2CSRX = &I2CSRX;                
  pI2CSTX = &I2CSTX;                
  pI2CALT = &I2CALT;                
  pI2CID0 = &I2CID0;                
  pI2CID1 = &I2CID1;                
  pI2CID2 = &I2CID2;                
  pI2CID3 = &I2CID3;                
  pI2CFSTA = &I2CFSTA;              
  
}
#else

// Cortex-M3/4 System Control Register(SCR)
volatile uint32_t *pSCR =(volatile uint32_t *)0xE000ED10;

// Watchdog Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2580
volatile uint16_t *pT3LD	=(volatile uint16_t *)0x40002580;			
volatile uint16_t *pT3VAL	=(volatile uint16_t *)0x40002584;		
volatile uint16_t *pT3CON	=(volatile uint16_t *)0x40002588;		
volatile uint16_t *pT3CLRI=(volatile uint16_t *)0x4000258C;	
volatile uint16_t *pT3STA	=(volatile uint16_t *)0x40002598;		

// Clocking Memory Mapped Registers(MMRs) : Base Address : 0x4000_2000
volatile uint16_t *pCLKCON0	=(volatile uint16_t *)0x40002000;	
volatile uint16_t *pCLKCON1	=(volatile uint16_t *)0x40002004; 
volatile uint16_t *pCLKDIS	=(volatile uint16_t *)0x4000202C;	
volatile uint16_t *pXOSCCON	=(volatile uint16_t *)0x40002410;	
volatile uint16_t *pCLKSYSDIV=(volatile uint16_t *)0x40002444;

// GPIO Memory Mapped Registers(MMRs) : Base Address : 0x4000_6000
volatile uint16_t *pGP0CON=(volatile uint16_t *)0x40006000;	
volatile uint8_t *pGP0OEN	=(volatile uint8_t *)0x40006004;		
volatile uint8_t *pGP0PUL	=(volatile uint8_t *)0x40006008;		
volatile uint8_t *pGP0OCE	=(volatile uint8_t *)0x4000600C;		
volatile uint8_t *pGP0IN	=(volatile uint8_t *)0x40006014;		
volatile uint8_t *pGP0OUT	=(volatile uint8_t *)0x40006018;		
volatile uint8_t *pGP0SET	=(volatile uint8_t *)0x4000601C;		
volatile uint8_t *pGP0CLR	=(volatile uint8_t *)0x40006020;		
volatile uint8_t *pGP0TGL	=(volatile uint8_t *)0x40006024;		

volatile uint16_t *pGP1CON=(volatile uint16_t *)0x40006030;	
volatile uint8_t *pGP1OEN	=(volatile uint8_t *)0x40006034;		
volatile uint8_t *pGP1PUL	=(volatile uint8_t *)0x40006038;		
volatile uint8_t *pGP1OCE	=(volatile uint8_t *)0x4000603C;		
volatile uint8_t *pGP1IN	=(volatile uint8_t *)0x40006044;		
volatile uint8_t *pGP1OUT	=(volatile uint8_t *)0x40006048;		
volatile uint8_t *pGP1SET	=(volatile uint8_t *)0x4000604C;		
volatile uint8_t *pGP1CLR	=(volatile uint8_t *)0x40006050;		
volatile uint8_t *pGP1TGL	=(volatile uint8_t *)0x40006054;		

volatile uint16_t *pGP2CON=(volatile uint16_t *)0x40006060;	
volatile uint8_t *pGP2OEN	=(volatile uint8_t *)0x40006064;		
volatile uint8_t *pGP2PUL	=(volatile uint8_t *)0x40006068;		
volatile uint8_t *pGP2OCE	=(volatile uint8_t *)0x4000606C;		
volatile uint8_t *pGP2IN	=(volatile uint8_t *)0x40006074;		
volatile uint8_t *pGP2OUT	=(volatile uint8_t *)0x40006078;		
volatile uint8_t *pGP2SET	=(volatile uint8_t *)0x4000607C;		
volatile uint8_t *pGP2CLR	=(volatile uint8_t *)0x40006080;		
volatile uint8_t *pGP2TGL	=(volatile uint8_t *)0x40006084;		


volatile uint8_t *pCOMTX_RX =(volatile uint8_t *)0x40005000; 
volatile uint8_t *pCOMIEN =(volatile uint8_t *)0x40005004;
volatile uint8_t *pCOMIIR =(volatile uint8_t *)0x40005008;
volatile uint8_t *pCOMLCR =(volatile uint8_t *)0x4000500C;
volatile uint8_t *pCOMMCR =(volatile uint8_t *)0x40005010;
volatile uint8_t *pCOMLSR =(volatile uint8_t *)0x40005014;
volatile uint8_t *pCOMMSR =(volatile uint8_t *)0x40005018;
volatile uint16_t *pCOMFBR =(volatile uint16_t *)0x40005024; 
volatile uint16_t *pCOMDIV =(volatile uint16_t *)0x40005028; 
volatile uint8_t *pCOMCON =(volatile uint8_t *)0x40005030; 

  // Wake-Up Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_2500
volatile uint16_t *pT2VAL0  =(volatile uint16_t *) 0x40002500;
volatile uint16_t *pT2VAL1  =(volatile uint16_t *) 0x40002504;
volatile uint16_t *pT2CON   =(volatile uint16_t *) 0x40002508;
volatile uint16_t *pT2INC   =(volatile uint16_t *) 0x4000250C;
volatile uint16_t *pT2WUFB0 =(volatile uint16_t *) 0x40002510;
volatile uint16_t *pT2WUFB1 =(volatile uint16_t *) 0x40002514;
volatile uint16_t *pT2WUFC0 =(volatile uint16_t *) 0x40002518;
volatile uint16_t *pT2WUFC1 =(volatile uint16_t *) 0x4000251C;
volatile uint16_t *pT2WUFD0 =(volatile uint16_t *) 0x40002520;
volatile uint16_t *pT2WUFD1 =(volatile uint16_t *) 0x40002524;
volatile uint16_t *pT2IEN   =(volatile uint16_t *) 0x40002528;
volatile uint16_t *pT2STA   =(volatile uint16_t *) 0x4000252C;
volatile uint16_t *pT2CLRI  =(volatile uint16_t *) 0x40002530;
volatile uint16_t *pT2WUFA0 =(volatile uint16_t *) 0x4000253C;
volatile uint16_t *pT2WUFA1 =(volatile uint16_t *) 0x40002540;

// GP0 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0000
volatile uint16_t *pGPT0LD  =(volatile uint16_t *) 0x40000000;         
volatile uint16_t *pGPT0VAL =(volatile uint16_t *) 0x40000004;       
volatile uint16_t *pGPT0CON =(volatile uint16_t *) 0x40000008;       
volatile uint16_t *pGPT0CLRI=(volatile uint16_t *) 0x4000000C;     
volatile uint16_t *pGPT0CAP =(volatile uint16_t *) 0x40000010;       
volatile uint16_t *pGPT0STA =(volatile uint16_t *) 0x4000001C;       

// GP1 Timer Memory Mapped Registers(MMRs) : Base Address : 0x4000_0400
volatile uint16_t *pGPT1LD  =(volatile uint16_t *) 0x40000400;         
volatile uint16_t *pGPT1VAL =(volatile uint16_t *) 0x40000404;       
volatile uint16_t *pGPT1CON =(volatile uint16_t *) 0x40000408;       
volatile uint16_t *pGPT1CLRI=(volatile uint16_t *) 0x4000040C;     
volatile uint16_t *pGPT1CAP =(volatile uint16_t *) 0x40000410;       
volatile uint16_t *pGPT1STA =(volatile uint16_t *) 0x4000041C;      

// PMU Memory Mapped Registers(MMRs) : Base Address : 0x4000_2400
volatile uint8_t *pPWR_MOD =(volatile uint8_t *) 0x40002400;
volatile uint16_t *pPWR_KEY =(volatile uint16_t *) 0x40002404;

// PWM Memory Mapped Registers(MMRs) : Base Address : 0x4000_1000
volatile uint16_t  *pPWMCON0 =(volatile uint16_t  *)0x40001000;
volatile uint8_t   *pPWMCON1 =(volatile uint8_t   *)0x40001004;
volatile uint16_t  *pPWMCLRI =(volatile uint16_t  *)0x40001008;
volatile uint16_t  *pPWM0COM0=(volatile uint16_t  *)0x40001010;
volatile uint16_t  *pPWM0COM1=(volatile uint16_t  *)0x40001014;
volatile uint16_t  *pPWM0COM2=(volatile uint16_t  *)0x40001018;
volatile uint16_t  *pPWM0LEN =(volatile uint16_t  *)0x4000101C;
volatile uint16_t  *pPWM1COM0=(volatile uint16_t  *)0x40001020;
volatile uint16_t  *pPWM1COM1=(volatile uint16_t  *)0x40001024;
volatile uint16_t  *pPWM1COM2=(volatile uint16_t  *)0x40001028;
volatile uint16_t  *pPWM1LEN =(volatile uint16_t  *)0x4000102C;
volatile uint16_t  *pPWM2COM0=(volatile uint16_t  *)0x40001030;
volatile uint16_t  *pPWM2COM1=(volatile uint16_t  *)0x40001034;
volatile uint16_t  *pPWM2COM2=(volatile uint16_t  *)0x40001038;
volatile uint16_t  *pPWM2LEN =(volatile uint16_t  *)0x4000103C;

// Flash Controller Memory Mapped Registers(MMRs) : Base Address : 0x4000_2800
volatile uint16_t  *pFEESTA  = (volatile uint16_t *)  0x40002800;
volatile uint16_t  *pFEECON0 = (volatile uint16_t *)  0x40002804;
volatile uint16_t  *pFEECMD  = (volatile uint16_t *)  0x40002808;
volatile uint16_t  *pFEEADR0L = (volatile uint16_t *) 0x40002810;
volatile uint16_t  *pFEEADR0H = (volatile uint16_t *) 0x40002814;
volatile uint16_t  *pFEEADR1L = (volatile uint16_t *) 0x40002818;
volatile uint16_t  *pFEEADR1H = (volatile uint16_t *) 0x4000281C;
volatile uint16_t  *pFEEKEY  = (volatile uint16_t *)  0x40002820;
volatile uint16_t  *pFEEPROL = (volatile uint16_t *)  0x40002828;
volatile uint16_t  *pFEEPROH = (volatile uint16_t *)  0x4000282C;
volatile uint16_t  *pFEESIGL = (volatile uint16_t *)  0x40002830;
volatile uint16_t  *pFEESIGH = (volatile uint16_t *)  0x40002834;
volatile uint16_t  *pFEECON1 = (volatile uint16_t *)  0x40002838;
volatile uint16_t  *pFEEADRAL = (volatile uint16_t *) 0x40002848;
volatile uint16_t  *pFEEADRAH = (volatile uint16_t *) 0x4000284C;
volatile uint16_t  *pFEEAEN0 = (volatile uint16_t *)  0x40002878;
volatile uint16_t  *pFEEAEN1 = (volatile uint16_t *)  0x4000287C;
volatile uint16_t  *pFEEAEN2 = (volatile uint16_t *)  0x40002880;

// ADC Controller Memory Mapped Registers(MMRs) : Base Address : 0x4003_0000                                                                                                           
// Only ADuCM360                                                                                                                                                                       
volatile uint8_t   *pADC0STA   = (volatile uint8_t  *) 0x40030000;
volatile uint8_t   *pADC0MSKI  = (volatile uint8_t  *) 0x40030004;
volatile uint32_t  *pADC0CON   = (volatile uint32_t *) 0x40030008;
volatile uint16_t  *pADC0OF    = (volatile uint16_t *) 0x4003000C;
volatile uint16_t  *pADC0INTGN = (volatile uint16_t *) 0x40030010;
volatile uint16_t  *pADC0EXTGN = (volatile uint16_t *) 0x40030014;
volatile uint16_t  *pADC0VDDGN = (volatile uint16_t *) 0x40030018;
volatile uint16_t  *pADC0CFG   = (volatile uint16_t *) 0x4003001C;
volatile uint16_t  *pADC0FLT   = (volatile uint16_t *) 0x40030020;
volatile uint16_t  *pADC0MDE   = (volatile uint16_t *) 0x40030024;
volatile uint16_t  *pADC0RCR   = (volatile uint16_t *) 0x40030028;
volatile uint16_t  *pADC0RCV   = (volatile uint16_t *) 0x4003002C;
volatile uint16_t  *pADC0TH    = (volatile uint16_t *) 0x40030030;
volatile uint8_t   *pADC0THC   = (volatile uint8_t  *) 0x40030034;
volatile uint8_t   *pADC0THV   = (volatile uint8_t  *) 0x40030038;
volatile uint32_t  *pADC0ACC   = (volatile uint32_t *) 0x4003003C;
volatile uint32_t  *pADC0ATH   = (volatile uint32_t *) 0x40030040;
volatile uint8_t   *pADC0PRO   = (volatile uint8_t  *) 0x40030044;
volatile uint32_t  *pADC0DAT   = (volatile uint32_t *) 0x40030048;
// Both ADuCM360/ADuCM361                                                                                                                                                              
volatile uint8_t   *pADC1STA   = (volatile uint8_t  *) 0x40030080;
volatile uint8_t   *pADC1MSKI  = (volatile uint8_t  *) 0x40030084;
volatile uint32_t  *pADC1CON   = (volatile uint32_t *) 0x40030088;
volatile uint16_t  *pADC1OF    = (volatile uint16_t *) 0x4003008C;
volatile uint16_t  *pADC1INTGN = (volatile uint16_t *) 0x40030090;
volatile uint16_t  *pADC1EXTGN = (volatile uint16_t *) 0x40030094;
volatile uint16_t  *pADC1VDDGN = (volatile uint16_t *) 0x40030098;
volatile uint16_t  *pADC1CFG   = (volatile uint16_t *) 0x4003009C;
volatile uint16_t  *pADC1FLT   = (volatile uint16_t *) 0x400300A0;
volatile uint16_t  *pADC1MDE   = (volatile uint16_t *) 0x400300A4;
volatile uint16_t  *pADC1RCR   = (volatile uint16_t *) 0x400300A8;
volatile uint16_t  *pADC1RCV   = (volatile uint16_t *) 0x400300AC;
volatile uint16_t  *pADC1TH    = (volatile uint16_t *) 0x400300B0;
volatile uint8_t   *pADC1THC   = (volatile uint8_t  *) 0x400300B4;
volatile uint8_t   *pADC1THV   = (volatile uint8_t  *) 0x400300B8;
volatile uint32_t  *pADC1ACC   = (volatile uint32_t *) 0x400300BC;
volatile uint32_t  *pADC1ATH   = (volatile uint32_t *) 0x400300C0;
volatile uint8_t   *pADC1PRO   = (volatile uint8_t  *) 0x400300C4;
volatile uint32_t  *pADC1DAT   = (volatile uint32_t *) 0x400300C8;
// ADCSTEP Registers :                                                                                                                                                                 
volatile uint16_t  *pDETCON    = (volatile uint16_t *) 0x400300E0;
volatile uint8_t   *pDETSTA    = (volatile uint8_t  *) 0x400300E4;
volatile uint16_t  *pSTEPTH    = (volatile uint16_t *) 0x400300E8;
volatile uint32_t  *pSTEPDAT   = (volatile uint32_t *) 0x400300EC;
volatile uint32_t  *pADCDMACON = (volatile uint32_t *) 0x400300F8;
// Other ADC Registers :                                                                                                                                                               
volatile uint16_t  *pREFCTRL = (volatile uint16_t *) 0x40008840;
volatile uint8_t   *pIEXCCON = (volatile uint8_t  *) 0x40008850;
volatile uint8_t   *pIEXCDAT = (volatile uint8_t  *) 0x40008854;

// DAC Controller Registers(MMRs) :
volatile uint16_t  *pDACCON = (volatile uint16_t *)0x40020000;
volatile uint32_t  *pDACDAT = (volatile uint32_t *)0x40020004;

// DMA Controller Registers(MMRs) :  Base Address : 0x4001_0000
volatile uint32_t  *pDMASTA = (volatile uint32_t *)0x40010000;
volatile uint32_t  *pDMACFG = (volatile uint32_t *)0x40010004;
volatile uint32_t  *pDMAPDBPTR = (volatile uint32_t *)0x40010008;
volatile uint32_t  *pDMAADBPTR = (volatile uint32_t *)0x4001000C;
volatile uint32_t  *pDMASWREQ = (volatile uint32_t *)0x40010014;
volatile uint32_t  *pDMARMSKSET = (volatile uint32_t *)0x40010020;
volatile uint32_t  *pDMARMSKCLR = (volatile uint32_t *)0x40010024;
volatile uint32_t  *pDMAENSET = (volatile uint32_t *)0x40010028;
volatile uint32_t  *pDMAENCLR = (volatile uint32_t *)0x4001002C;
volatile uint32_t  *pDMAALTSET = (volatile uint32_t *)0x40010030;
volatile uint32_t  *pDMAALTCLR = (volatile uint32_t *)0x40010034;
volatile uint32_t  *pDMAPRISET = (volatile uint32_t *)0x40010038;
volatile uint32_t  *pDMAPRICLR = (volatile uint32_t *)0x4001003C;
volatile uint32_t  *pDMAERRCLR = (volatile uint32_t *)0x4001004C;
volatile uint8_t   *pDMAPERID4 = (volatile uint8_t *)0x40010FD0;
volatile uint8_t   *pDMAPERID0 = (volatile uint8_t *)0x40010FE0;
volatile uint8_t   *pDMAPERID1 = (volatile uint8_t *)0x40010FE4;
volatile uint8_t   *pDMAPERID2 = (volatile uint8_t *)0x40010FE8;
volatile uint8_t   *pDMAPERID3 = (volatile uint8_t *)0x40010FEC;
volatile uint8_t   *pDMAPCELLID0 = (volatile uint8_t *)0x40010FF0;
volatile uint8_t   *pDMAPCELLID1 = (volatile uint8_t *)0x40010FF4;
volatile uint8_t   *pDMAPCELLID2 = (volatile uint8_t *)0x40010FF8;
volatile uint8_t   *pDMAPCELLID3 = (volatile uint8_t *)0x40010FFC;

// I2C Controller Registers(MMRs) :  Base Address : 0x4000_3000
volatile uint16_t  *pI2CMCON    = (volatile uint16_t *)0x40003000; 
volatile uint16_t  *pI2CMSTA    = (volatile uint16_t *)0x40003004;
volatile uint8_t   *pI2CMRX     = (volatile uint8_t  *)0x40003008;
volatile uint8_t   *pI2CMTX     = (volatile uint8_t  *)0x4000300C;
volatile uint16_t  *pI2CMRXCNT  = (volatile uint16_t *)0x40003010;
volatile uint16_t  *pI2CMCRXCNT = (volatile uint16_t *)0x40003014;
volatile uint8_t   *pI2CADR0    = (volatile uint8_t  *)0x40003018;
volatile uint8_t   *pI2CADR1    = (volatile uint8_t  *)0x4000301C;
volatile uint16_t  *pI2CDIV     = (volatile uint16_t *)0x40003024;

volatile uint16_t  *pI2CSCON    = (volatile uint16_t *)0x40003028;
volatile uint16_t  *pI2CSSTA    = (volatile uint16_t *)0x4000302C;
volatile uint16_t  *pI2CSRX     = (volatile uint16_t *)0x40003030;
volatile uint16_t  *pI2CSTX     = (volatile uint16_t *)0x40003034;
volatile uint16_t  *pI2CALT     = (volatile uint16_t *)0x40003038;
volatile uint16_t  *pI2CID0     = (volatile uint16_t *)0x4000303C;
volatile uint16_t  *pI2CID1     = (volatile uint16_t *)0x40003040;
volatile uint16_t  *pI2CID2     = (volatile uint16_t *)0x40003044;
volatile uint16_t  *pI2CID3     = (volatile uint16_t *)0x40003048;
volatile uint16_t  *pI2CFSTA    = (volatile uint16_t *)0x4000304C;

#endif
