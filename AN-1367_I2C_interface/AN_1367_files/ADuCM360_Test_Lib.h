/*

   This sample code is developed by Fermi Lim(fermi.lim@analog.com).
   Date : 1/30/2015 
   Feature : To access any internal registers of ADE7953.
   Written by Fermi Lim(Korean Name : JongSu Lim.

*/

#include <stdint.h>

typedef enum {
        PASSED = 0,
        FAILED,
} TEST_RESULT;

#define RETURN	'\n'
#define Cap2Low(Ch)	(((Ch) >= 'A') && ((Ch) <= 'Z'))? (Ch-'A'+'a'):Ch

void UART_ISR(void);

// I2C routines :
#define I2CMSTA_RXREQ                  (0x1   << 3  )
#define I2CMSTA_TXREQ                  (0x1   << 2  )
#define I2CMSTA_TCOMP_SET              (0x1   << 8  )

int I2cMWrCfg(unsigned int uiDevAddr);
int I2cMRdCfg(unsigned int uiDevAddr, int iNumBytes, int iExt);

TEST_RESULT ADE7953_Ex(void);

void I2C_Write_ADE7953(void);
void I2C_Read_ADE7953(void);
void I2C0_Master_Int_Handler(void);
TEST_RESULT I2C_Test(void);
void I2C_menu_display(void);
void I2C_MemorySet(char *str1);
void I2C_MemoryGet(char *str1);

extern uint8_t Pulse;
extern uint8_t szTemp[80];            // Used to store string before printing to UART
