/*

   This sample code is developed by Fermi Lim(fermi.lim@analog.com).
   Date : 1/30/2015 
   Feature : To access any internal registers of ADE7953.
   Written by Fermi Lim(Korean Name : JongSu Lim.

*/

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "ADuCM360_Regs.h"
#include "JongSuLib_V2.h"
#include "ADuCM360_Test_Lib.h"

void menu_display(void);

int main()
{
	uint8_t d, addr;
	int state;	

#ifdef FermiEmulation_Mode
	// Fermi 20131003, to initialize all of the ADuCM360 register set.	
	ADuCM360_RegsInit(); 
#endif

#ifndef FermiEmulation_Mode
		write_reg(0xE000E100,0x00020000);  // ISER = 0xE000E100;  Enable  UART interrupt enabling
#endif

	// Step 1. We should first disable watchdog timer.
	*pT3CON=0x0;

	// Step 2. We should set up clock domain according to your plan.  
	// UART speed rate will be 115200[bps] based on UCLK/DIV = 16[MHz].
	// COMDIV = 2, DIVM=2, DIVN= 348.
	*pCLKDIS=0x03F7;  // UART, Timer1, DAC, and DMA Clock Enable
	*pCLKSYSDIV=0x00; // HFOSC isn't divided by 1/2. That means UCLK is 16[MHz].
	*pCLKCON0=0x0000; // UCLK/DIV=16[MHz], That means UARTCLK is 16[MHz].
	*pCLKCON1=0x0000; // UCLK/DIV=16[MHz], That means UARTCLK is 16[MHz].

	// Step 3. We can set up the behavior of UART you chose. 	
	*pCOMCON=0x00;		// UART Peripheral is enabling.
	*pGP0CON=0x003C;	// P0.2 will be UART TX, P0.1 will be UART RX.	
	*pCOMDIV=0x0002;	// COMDIV =2;                                                                       
	*pCOMFBR=0x915C;	// DIVM=2, DIVN= 348. In order to be 115200[bps].									
	*pCOMIEN=0x0000;	// COMTX_RX interrupt are enabling.
	*pCOMLCR=0x0003;	// WordLength = 8 bits, stop bit=1 bit, No Parity check.						
	// UART, Timer0/1 enabling.
	*pCOMIEN=0x0003;	// COMTX_RX interrupt are enabling.	

#ifndef FermiEmulation_Mode
	write_reg(0xE000E100,0x00020000);  // ISER = 0xE000E100;  Enable  UART interrupt enabling
#endif

	while(1)
	{
		printf("LoveJongSu]"); 
		state = gets(szTemp);
		d=szTemp[0];

		if (d == 'm')	{
			addr = d;
		} else if (d == RETURN ) {			
			addr = 'r';
		} else if (Cap2Low(d) == 'g') {
			MemoryDump((char *)szTemp);
			addr = 'r';
		} else if (Cap2Low(d) == 's') {
			MemorySet((char *)szTemp);
			addr='r';
		} else
		{
			addr = hextoint(szTemp);
		}
		szTemp[0] = 0;
		switch(addr){				
		case 0x1 :
			I2C_Test();
			break;			
		case 'm'  :	
			menu_display();								
			break;
		default:														   
			break;
		}	
		state &=state;		
	}
	return 1;
}

void menu_display(void)
{
	printf("*************************************************************\n");
	printf("**                                                           \n");		
	printf("**   Note :                                                  \n");					 
	printf("**   ADuCM360 EVM board Test Program                       \n");		
	printf("**                                                           \n");		
	printf("** Usage :                                                   \n");					 
	printf("** LoveJongSu]set addr val                                   \n");					 
	printf("** LoveJongSu]get addr                                       \n");					 			 
	printf("**                                                           \n");		
	printf("**                       2014, 11, 11  By Lim Jong Su        \n");	 
	printf("**                                                           \n");		
	printf("*************************************************************\n");
	printf("*           1.  I2C Test                                    *\n");               
	printf("**                                                          *\n");		
	printf("            ขั m.  Main Menu display                        \n");
	printf("***********************************************************\n\n");
}
