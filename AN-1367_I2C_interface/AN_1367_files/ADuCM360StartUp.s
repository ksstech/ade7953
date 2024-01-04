;
;
;   This sample code is developed by Fermi Lim(fermi.lim@analog.com).
;   Date : 1/30/2015 
;   Feature : To access any internal registers of ADE7953.
;   Written by Fermi Lim(Korean Name : JongSu Lim.
;
;

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


Heap_Size       EQU     0x00000200


                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


	IMPORT	|Load$$RW_IRAM1$$Base| [WEAK]
;   In case of the Execution View	:	
	IMPORT	|Image$$RW_IRAM1$$Base| [WEAK]
	IMPORT	|Image$$RW_IRAM1$$Length| [WEAK]	

	IMPORT	|Image$$RW_IRAM1$$ZI$$Base| [WEAK]
	IMPORT	|Image$$RW_IRAM1$$ZI$$Length| [WEAK]

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                area    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; The NMI handler        
                DCD     HardFault_Handler              ; The hard fault handler 
                DCD     MemManage_Handler          ; The MPU fault handler  
                DCD     BusFault_Handler           ; The bus fault handler  
                DCD     UsageFault_Handler         ; The usage fault handler
                DCD     0                          ; Reserved               
                DCD     0                          ; Reserved               
                DCD     0                          ; Reserved               
                DCD     0                          ; Reserved               
                DCD     SVC_Handler                ; SVCall handler         
                DCD     DebugMon_Handler           ; Debug monitor handler  
                DCD     0                          ; Reserved               
                DCD     PendSV_Handler             ; The PendSV handler     
                DCD     SysTick_Handler            ; The SysTick handler    
            
	        DCD     Wake_UpTimer_ISR         	; Wake-up timer					[0]
			; External Interrupts :			
	        DCD     External_ISR0          		; External Interrupt 0	[1]
	        DCD     External_ISR1          		; External Interrupt 1  [2]
	        DCD     External_ISR2          		; External Interrupt 2  [3]
	        DCD     External_ISR3          		; External Interrupt 3  [4]
	        DCD     External_ISR4          		; External Interrupt 4  [5]
	        DCD     External_ISR5          		; External Interrupt 5  [6]
	        DCD     External_ISR6          		; External Interrupt 6  [7]
	        DCD     External_ISR7          		; External Interrupt 7  [8]
			; Peripheral Interrupts :
					DCD			WatchDog_Timer_ISR				;												[9]
					DCD			0													; Reserved							[10]
					DCD			Timer0_ISR                ;                        [11]
					DCD			Timer1_ISR                ;                        [12]
					DCD			ADC0_ISR                  ;                        [13]
					DCD			ADC1_ISR                  ;                        [14]
					DCD			SINC2_ISR                 ;                        [15]
					DCD			Flash_Ctrl_ISR            ;                        [16]
					DCD			UART_ISR                  ;                        [17]
	        DCD     SPI0_Int_Handler          ; SPI 0                      [18]
	        DCD     SPI1_Int_Handler          ; SPI 1                      [19]
	        DCD     I2C0_Slave_Int_Handler    ; I2C0 Slave                 [20]
	        DCD     I2C0_Master_Int_Handler   ; I2C0 Master                [21]
	        DCD     DMA_Err_Int_Handler       ; DMA Error interrupt        [22]
	        DCD     DMA_SPI1_TX_Int_Handler   ; DMA SPI1 TX                [23]
	        DCD     DMA_SPI1_RX_Int_Handler   ; DMA SPI1 RX                [24]
	        DCD     DMA_UART_TX_Int_Handler   ; DMA UART TX                [25]
	        DCD     DMA_UART_RX_Int_Handler   ; DMA UART RX                [26]
	        DCD     DMA_I2C0_STX_Int_Handler  ; DMA I2C0 Slave TX          [27]
	        DCD     DMA_I2C0_SRX_Int_Handler  ; DMA I2C0 Slave RX          [28]
	        DCD     DMA_I2C0_MTX_Int_Handler  ; DMA I2C0 Master TX         [29]
	        DCD     DMA_I2C0_MRX_Int_Handler  ; DMA I2C0 Master RX         [30]
	        DCD     DMA_DAC_Out_Int_Handler   ; DMA DAC out                [31]
	        DCD     DMA_ADC0_Int_Handler      ; DMA ADC0                   [32]
	        DCD     DMA_ADC1_Int_Handler      ; DMA ADC1                   [33]
	        DCD     DMA_SINC2_Int_Handler     ; SINC2                      [34]
	        DCD     PWMTRIP_Int_Handler       ; PWMTRIP                    [35]
	        DCD     PWM0_Int_Handler          ; PWM0                       [36]
	
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors


                AREA    |.text|, CODE, READONLY
; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  __main

		BL	data_init

        LDR     R0, =__main
        BX      R0
		
data_init
; 1. copy rw data to RAM & initialize zi region
	MOV 	r5, lr	
	LDR 	r0, =|Load$$RW_IRAM1$$Base|
	LDR 	r1, =|Image$$RW_IRAM1$$Base|

	CMP 	r0, r1
	BEQ	ram_zi_init

	MOV 	r2, r1
	LDR 	r4, = |Image$$RW_IRAM1$$Length|
	ADD	r2, r2, r4
	BL	data_copy

ram_zi_init
	LDR	r1, = |Image$$RW_IRAM1$$ZI$$Base|
	MOV	r2, r1 
	LDR	r4, = |Image$$RW_IRAM1$$ZI$$Length|
	ADD 	r2, r2, r4

	MOV 	r3, #0
	BL  	zi_init
	MOV 	lr, r5
	BX LR
data_copy
	CMP 	r1, r2			
	LDRCC 	r3, [r0], #4
	STRCC 	r3, [r1], #4
	BCC 	data_copy

	BX LR

zi_init
	CMP 	r1, r2
	STRCC 	r3, [r1], #4
	BCC 	zi_init

	BX LR
		
        ENDP

; Dummy Exception Handlers (infinite loops which can be modified)


NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

		EXPORT Wake_UpTimer_ISR		[WEAK]
			; External Interrupts :			
		EXPORT External_ISR0			[WEAK]                                         
	  EXPORT External_ISR1			[WEAK]                                         
	  EXPORT External_ISR2			[WEAK]                                         
	  EXPORT External_ISR3			[WEAK]                                         
	  EXPORT External_ISR4			[WEAK]                                         
	  EXPORT External_ISR5			[WEAK]                                         
	  EXPORT External_ISR6			[WEAK]                                         
	  EXPORT External_ISR7			[WEAK]                                         
			; Peripheral Interrupts :
		EXPORT WatchDog_Timer_ISR	[WEAK]
		EXPORT Timer0_ISR         [WEAK]
		EXPORT Timer1_ISR         [WEAK]
		EXPORT ADC0_ISR           [WEAK]
		EXPORT ADC1_ISR           [WEAK]
		EXPORT SINC2_ISR          [WEAK]
		EXPORT Flash_Ctrl_ISR     [WEAK]
		EXPORT UART_ISR           [WEAK]
		EXPORT SPI0_Int_Handler            [WEAK]
		EXPORT SPI1_Int_Handler            [WEAK]
		EXPORT I2C0_Slave_Int_Handler      [WEAK]
		EXPORT I2C0_Master_Int_Handler     [WEAK]
		EXPORT DMA_Err_Int_Handler         [WEAK]
		EXPORT DMA_SPI1_TX_Int_Handler     [WEAK]
		EXPORT DMA_SPI1_RX_Int_Handler     [WEAK]
		EXPORT DMA_UART_TX_Int_Handler     [WEAK]
		EXPORT DMA_UART_RX_Int_Handler     [WEAK]
		EXPORT DMA_I2C0_STX_Int_Handler    [WEAK]
		EXPORT DMA_I2C0_SRX_Int_Handler    [WEAK]
		EXPORT DMA_I2C0_MTX_Int_Handler    [WEAK]
		EXPORT DMA_I2C0_MRX_Int_Handler    [WEAK]
		EXPORT DMA_DAC_Out_Int_Handler     [WEAK]
		EXPORT DMA_ADC0_Int_Handler        [WEAK]
		EXPORT DMA_ADC1_Int_Handler        [WEAK]
		EXPORT DMA_SINC2_Int_Handler       [WEAK]
		EXPORT PWMTRIP_Int_Handler         [WEAK]
		EXPORT PWM0_Int_Handler            [WEAK]


Wake_UpTimer_ISR		
External_ISR0
External_ISR1
External_ISR2
External_ISR3
External_ISR4
External_ISR5
External_ISR6
External_ISR7
WatchDog_Timer_ISR
Timer0_ISR        
Timer1_ISR        
ADC0_ISR          
ADC1_ISR          
SINC2_ISR         
Flash_Ctrl_ISR    
UART_ISR 
SPI0_Int_Handler        
SPI1_Int_Handler        
I2C0_Slave_Int_Handler  
I2C0_Master_Int_Handler 
DMA_Err_Int_Handler     
DMA_SPI1_TX_Int_Handler 
DMA_SPI1_RX_Int_Handler 
DMA_UART_TX_Int_Handler 
DMA_UART_RX_Int_Handler 
DMA_I2C0_STX_Int_Handler
DMA_I2C0_SRX_Int_Handler
DMA_I2C0_MTX_Int_Handler
DMA_I2C0_MRX_Int_Handler
DMA_DAC_Out_Int_Handler 
DMA_ADC0_Int_Handler    
DMA_ADC1_Int_Handler    
DMA_SINC2_Int_Handler   
PWMTRIP_Int_Handler     
PWM0_Int_Handler        
      
      
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF
				 
				 END