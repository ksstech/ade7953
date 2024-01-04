; generated by ARM C/C++ Compiler, 5.03 [Build 76]
; commandline ArmCC [--debug -c --asm -o.\debug\jongsulib_v2.o --depend=.\debug\jongsulib_v2.d --cpu=Cortex-M3 --apcs=interwork -O0 --omf_browse=.\debug\jongsulib_v2.crf JongSuLib_V2.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

putc PROC
        CMP      r0,#0xa
        BNE      |L1.28|
        MOVS     r1,#0
        LDR      r2,|L1.1008|
        STRB     r1,[r2,#0]
        MOVS     r1,#0xa
        LDR      r2,|L1.1012|
        LDR      r2,[r2,#0]  ; pCOMTX_RX
        STRB     r1,[r2,#0]
        NOP      
|L1.20|
        LDR      r1,|L1.1008|
        LDRB     r1,[r1,#0]  ; ucTxBufferEmpty
        CMP      r1,#0
        BEQ      |L1.20|
|L1.28|
        MOVS     r1,#0
        LDR      r2,|L1.1008|
        STRB     r1,[r2,#0]
        LDR      r1,|L1.1012|
        LDR      r1,[r1,#0]  ; pCOMTX_RX
        STRB     r0,[r1,#0]
        NOP      
|L1.42|
        LDR      r1,|L1.1008|
        LDRB     r1,[r1,#0]  ; ucTxBufferEmpty
        CMP      r1,#0
        BEQ      |L1.42|
        DSB      
        BX       lr
        ENDP

puts PROC
        PUSH     {lr}
        MOV      r3,r0
        B        |L1.70|
|L1.62|
        LDRB     r0,[r3,#0]
        BL       putc
        ADDS     r3,r3,#1
|L1.70|
        LDRB     r0,[r3,#0]
        CMP      r0,#0
        BNE      |L1.62|
        POP      {pc}
        ENDP

unsigned_divide PROC
        PUSH     {r4-r9,lr}
        MOV      r8,#0
        MOV      r9,r8
        STR      r9,[r3,#0]
        STR      r9,[r2,#0]
        CBNZ     r1,|L1.102|
|L1.98|
        POP      {r4-r9,pc}
|L1.102|
        CMP      r1,r0
        BLS      |L1.110|
        STR      r0,[r3,#0]
        B        |L1.98|
|L1.110|
        CMP      r1,r0
        BNE      |L1.124|
        MOV      r9,#1
        STR      r9,[r2,#0]
        B        |L1.98|
|L1.124|
        MOVS     r4,#0x20
        B        |L1.148|
|L1.128|
        LSRS     r7,r0,#31
        LDR      r9,[r3,#0]
        ORR      r9,r7,r9,LSL #1
        STR      r9,[r3,#0]
        MOV      r8,r0
        LSLS     r0,r0,#1
        SUBS     r4,r4,#1
|L1.148|
        LDR      r9,[r3,#0]
        CMP      r9,r1
        BCC      |L1.128|
        MOV      r0,r8
        LDR      r9,[r3,#0]
        LSR      r9,r9,#1
        STR      r9,[r3,#0]
        ADDS     r4,r4,#1
        MOV      r12,#0
        B        |L1.230|
|L1.178|
        LSRS     r7,r0,#31
        LDR      r9,[r3,#0]
        ORR      r9,r7,r9,LSL #1
        STR      r9,[r3,#0]
        LDR      r9,[r3,#0]
        SUB      r5,r9,r1
        MOV      r9,#1
        EOR      r6,r9,r5,LSR #31
        LSLS     r0,r0,#1
        LDR      r9,[r2,#0]
        ORR      r9,r6,r9,LSL #1
        STR      r9,[r2,#0]
        CBZ      r6,|L1.226|
        STR      r5,[r3,#0]
|L1.226|
        ADD      r12,r12,#1
|L1.230|
        CMP      r12,r4
        BCC      |L1.178|
        NOP      
        B        |L1.98|
        ENDP

do_div PROC
        PUSH     {r2-r5,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r3,sp
        ADD      r2,sp,#4
        MOV      r1,r5
        LDR      r0,[r4,#0]
        BL       unsigned_divide
        LDR      r0,[sp,#4]
        STR      r0,[r4,#0]
        LDR      r0,[sp,#0]
        POP      {r2-r5,pc}
        ENDP

number PROC
        PUSH     {r0-r10,lr}
        SUB      sp,sp,#0x44
        MOV      r5,r0
        MOV      r7,r2
        MOV      r6,r3
        LDR      r8,[sp,#0x74]
        ADR      r9,|L1.1016|
        MOV      r10,#8
        TST      r8,#0x40
        BEQ      |L1.298|
        ADR      r9,|L1.1056|
|L1.298|
        SUBS     r6,r6,#2
        MOVS     r4,#0
        LDR      r0,[sp,#0x48]
        CBNZ     r0,|L1.318|
        MOVS     r2,#0x30
        MOV      r0,r4
        ADDS     r4,r4,#1
        STRB     r2,[sp,r0]
        B        |L1.346|
|L1.318|
        B        |L1.340|
|L1.320|
        MOV      r1,r7
        ADD      r0,sp,#0x48
        BL       do_div
        LDRB     r2,[r9,r0]
        MOV      r0,r4
        ADDS     r4,r4,#1
        STRB     r2,[sp,r0]
|L1.340|
        LDR      r0,[sp,#0x48]
        CMP      r0,#0
        BNE      |L1.320|
|L1.346|
        B        |L1.358|
|L1.348|
        MOVS     r2,#0x30
        MOV      r0,r4
        ADDS     r4,r4,#1
        STRB     r2,[sp,r0]
|L1.358|
        SUBS     r0,r10,r4
        BNE      |L1.348|
        B        |L1.374|
|L1.366|
        LDRB     r0,[sp,r4]
        STRB     r0,[r5],#1
|L1.374|
        SUBS     r0,r4,#0
        SUB      r4,r4,#1
        BGT      |L1.366|
        B        |L1.390|
|L1.384|
        MOVS     r0,#0x20
        STRB     r0,[r5],#1
|L1.390|
        SUBS     r0,r6,#0
        SUB      r6,r6,#1
        BGT      |L1.384|
        MOV      r0,r5
        ADD      sp,sp,#0x54
        POP      {r4-r10,pc}
        ENDP

sprintf PROC
        PUSH     {r0-r2,r4-r10,lr}
        SUB      sp,sp,#4
        MOV      r6,r0
        MOV      r5,r1
        MOV      r10,#0xffffffff
        MOV      r4,r6
        B        |L1.536|
|L1.424|
        LDRB     r0,[r5,#0]
        CMP      r0,#0x25
        BEQ      |L1.438|
        LDRB     r0,[r5,#0]
        STRB     r0,[r4],#1
        B        |L1.534|
|L1.438|
        MOV      r8,#0
        ADDS     r5,r5,#1
        MOV      r9,#0x10
        LDRB     r0,[r5,#0]
        CMP      r0,#0x25
        BEQ      |L1.464|
        CMP      r0,#0x58
        BEQ      |L1.472|
        CMP      r0,#0x78
        BNE      |L1.484|
        B        |L1.478|
|L1.464|
        MOVS     r0,#0x25
        STRB     r0,[r4],#1
        B        |L1.534|
|L1.472|
        ORR      r8,r8,#0x40
        NOP      
|L1.478|
        MOV      r9,#0x10
        B        |L1.506|
|L1.484|
        MOVS     r0,#0x25
        STRB     r0,[r4],#1
        LDRB     r0,[r5,#0]
        CBZ      r0,|L1.502|
        LDRB     r0,[r5,#0]
        STRB     r0,[r4],#1
        B        |L1.504|
|L1.502|
        SUBS     r5,r5,#1
|L1.504|
        B        |L1.534|
|L1.506|
        NOP      
        LDR      r0,[sp,#0xc]
        LDM      r0!,{r7}
        STR      r0,[sp,#0xc]
        MOV      r3,r10
        MOV      r2,r9
        MOV      r1,r7
        MOV      r0,r4
        STR      r8,[sp,#0]
        BL       number
        MOV      r4,r0
        NOP      
|L1.534|
        ADDS     r5,r5,#1
|L1.536|
        LDRB     r0,[r5,#0]
        CMP      r0,#0
        BNE      |L1.424|
        STRB     r0,[r4,#0]
        SUBS     r0,r4,r6
        ADD      sp,sp,#0x10
        POP      {r4-r10,pc}
        ENDP

printf PROC
        PUSH     {r0-r3}
        PUSH     {lr}
        SUB      sp,sp,#0x104
        ADD      r0,sp,#0x10c
        STR      r0,[sp,#0x100]
        MOV      r0,sp
        LDR      r2,[sp,#0x100]
        LDR      r1,[sp,#0x108]
        BL       sprintf
        MOVS     r0,#0
        STR      r0,[sp,#0x100]
        MOV      r0,sp
        BL       puts
        ADD      sp,sp,#0x104
        LDR      pc,[sp],#0x14
        ENDP

write_reg PROC
        PUSH     {r0,r1,r4-r6,lr}
        MOV      r4,r1
        MOVS     r5,#0
        MOVS     r6,#0
        LDR      r6,[sp,#0]
        LDR      r0,[sp,#0]
        CMP      r0,#0x20000
        BCS      |L1.736|
        NOP      
        NOP      
        ADR      r0,|L1.1096|
        BL       printf
        LDR      r0,[sp,#0]
        LSRS     r0,r0,#9
        LSLS     r5,r0,#9
        NOP      
|L1.624|
        LDR      r0,|L1.1140|
        LDR      r0,[r0,#0]  ; pFEESTA
        LDRH     r0,[r0,#0]
        CMP      r0,#1
        BEQ      |L1.624|
        MOV      r0,#0xf456
        LDR      r1,|L1.1144|
        LDR      r1,[r1,#0]  ; pFEEKEY
        STRH     r0,[r1,#0]
        MOV      r0,#0xf123
        LDR      r1,|L1.1144|
        LDR      r1,[r1,#0]  ; pFEEKEY
        STRH     r0,[r1,#0]
        LDR      r1,|L1.1148|
        LDR      r1,[r1,#0]  ; pFEEADR0L
        STRH     r5,[r1,#0]
        LSRS     r0,r5,#16
        LDR      r1,|L1.1152|
        LDR      r1,[r1,#0]  ; pFEEADR0H
        STRH     r0,[r1,#0]
        MOVS     r0,#1
        LDR      r1,|L1.1156|
        LDR      r1,[r1,#0]  ; pFEECMD
        STRH     r0,[r1,#0]
        LDR      r0,|L1.1160|
        LDR      r0,[r0,#0]  ; pFEECON0
        LDRH     r0,[r0,#0]
        ORR      r0,r0,#4
        LDR      r1,|L1.1160|
        LDR      r1,[r1,#0]  ; pFEECON0
        STRH     r0,[r1,#0]
        NOP      
|L1.694|
        LDR      r0,|L1.1140|
        LDR      r0,[r0,#0]  ; pFEESTA
        LDRH     r0,[r0,#0]
        CMP      r0,#1
        BEQ      |L1.694|
        STR      r4,[r6,#0]
        LDR      r0,|L1.1160|
        LDR      r0,[r0,#0]  ; pFEECON0
        LDRH     r0,[r0,#0]
        AND      r0,r0,#0xfb
        LDR      r1,|L1.1160|
        LDR      r1,[r1,#0]  ; pFEECON0
        STRH     r0,[r1,#0]
        NOP      
|L1.724|
        LDR      r0,|L1.1140|
        LDR      r0,[r0,#0]  ; pFEESTA
        LDRH     r0,[r0,#0]
        CMP      r0,#1
        BEQ      |L1.724|
        B        |L1.738|
|L1.736|
        STR      r4,[r6,#0]
|L1.738|
        POP      {r2-r6,pc}
        ENDP

read_reg PROC
        PUSH     {r0,lr}
        MOVS     r1,#0
        LDR      r1,[sp,#0]
        LDR      r0,[r1,#0]
        POP      {r3,pc}
        ENDP

set_bit PROC
        PUSH     {r0,r1,r4,lr}
        MOVS     r0,#0
        MOVS     r3,#1
        LDR      r0,[sp,#0]
        LDR      r2,[r0,#0]
        LSL      r4,r3,r1
        ORRS     r2,r2,r4
        STR      r2,[r0,#0]
        POP      {r2-r4,pc}
        ENDP

clear_bit PROC
        PUSH     {r0,r1,r4,lr}
        MOVS     r0,#0
        MOVS     r3,#1
        LDR      r0,[sp,#0]
        LDR      r2,[r0,#0]
        LSL      r4,r3,r1
        BICS     r2,r2,r4
        STR      r2,[r0,#0]
        POP      {r2-r4,pc}
        ENDP

get_bit PROC
        PUSH     {r0,r1,r4,lr}
        MOVS     r2,#1
        MOVS     r3,#0
        LDR      r3,[sp,#0]
        LDR      r0,[r3,#0]
        LSL      r4,r2,r1
        ANDS     r4,r4,r0
        LSR      r0,r4,r1
        POP      {r2-r4,pc}
        ENDP

write_mask PROC
        PUSH     {r0-r2,r4-r7,lr}
        MOV      r4,r1
        MOV      r5,r2
        BIC      r6,r4,r5
        LDR      r0,[sp,#0]
        BL       read_reg
        ORR      r7,r0,r6
        MOV      r1,r7
        LDR      r0,[sp,#0]
        BL       write_reg
        POP      {r1-r7,pc}
        ENDP

read_mask PROC
        PUSH     {r0,r1,lr}
        MOV      r2,r1
        LDR      r0,[sp,#0]
        BL       read_reg
        BICS     r0,r0,r2
        POP      {r2,r3,pc}
        ENDP

poll_on_set PROC
        PUSH     {r0,r1,r4,lr}
        MOVS     r3,#0
        MOVS     r4,#1
        LDR      r3,[sp,#0]
        NOP      
|L1.866|
        LDR      r2,[r3,#0]
        LSL      r0,r4,r1
        ANDS     r0,r0,r2
        LSR      r2,r0,r1
        CMP      r2,#0
        BEQ      |L1.866|
        CMP      r2,#1
        BNE      |L1.890|
        MOVS     r0,#1
|L1.888|
        POP      {r2-r4,pc}
|L1.890|
        MOVS     r0,#0
        B        |L1.888|
        ENDP

poll_on_set_limit PROC
        PUSH     {r0-r2,r4-r6,lr}
        MOV      r3,r2
        MOVS     r5,#0
        MOVS     r6,#1
        MOVS     r4,#0
        LDR      r5,[sp,#0]
        NOP      
|L1.908|
        LDR      r2,[r5,#0]
        LSL      r0,r6,r1
        ANDS     r0,r0,r2
        LSR      r2,r0,r1
        ADDS     r4,r4,#1
        CBNZ     r2,|L1.928|
        CMP      r4,r3
        BCC      |L1.908|
|L1.928|
        CMP      r2,#1
        BNE      |L1.936|
        MOVS     r0,#1
|L1.934|
        POP      {r1-r6,pc}
|L1.936|
        MOVS     r0,#0
        B        |L1.934|
        ENDP

poll_on_reset PROC
        PUSH     {r0,r1,r4,r5,lr}
        MOV      r5,r1
        NOP      
|L1.946|
        MOV      r1,r5
        LDR      r0,[sp,#0]
        BL       get_bit
        MOV      r4,r0
        CMP      r4,#1
        BEQ      |L1.946|
        CBNZ     r4,|L1.966|
        MOVS     r0,#1
|L1.964|
        POP      {r2-r5,pc}
|L1.966|
        MOVS     r0,#0
        B        |L1.964|
        ENDP

hextoint PROC
        MOV      r3,r0
        MOVS     r2,#0
        B        |L1.1206|
|L1.976|
        LDRB     r1,[r3],#1
        CBNZ     r1,|L1.984|
        B        |L1.1208|
|L1.984|
        LSLS     r0,r2,#28
        LSRS     r2,r0,#24
        CMP      r1,#0x30
        BLT      |L1.1164|
        CMP      r1,#0x39
        BGT      |L1.1164|
        SUB      r0,r1,#0x30
        ADD      r0,r0,r2
        UXTB     r2,r0
        B        |L1.1206|
        DCW      0x0000
|L1.1008|
        DCD      ucTxBufferEmpty
|L1.1012|
        DCD      pCOMTX_RX
|L1.1016|
        DCB      "0123456789abcdefghijklmnopqrstuvwxyz",0
        DCB      0
        DCB      0
        DCB      0
|L1.1056|
        DCB      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",0
        DCB      0
        DCB      0
        DCB      0
|L1.1096|
        DCB      "The address you typed is in the Flash Area\n",0
|L1.1140|
        DCD      pFEESTA
|L1.1144|
        DCD      pFEEKEY
|L1.1148|
        DCD      pFEEADR0L
|L1.1152|
        DCD      pFEEADR0H
|L1.1156|
        DCD      pFEECMD
|L1.1160|
        DCD      pFEECON0
|L1.1164|
        CMP      r1,#0x41
        BLT      |L1.1182|
        CMP      r1,#0x46
        BGT      |L1.1182|
        SUB      r0,r1,#0x37
        ADD      r0,r0,r2
        UXTB     r2,r0
        B        |L1.1206|
|L1.1182|
        CMP      r1,#0x61
        BLT      |L1.1200|
        CMP      r1,#0x66
        BGT      |L1.1200|
        SUB      r0,r1,#0x57
        ADD      r0,r0,r2
        UXTB     r2,r0
        B        |L1.1206|
|L1.1200|
        MOV      r0,#0xffffffff
|L1.1204|
        BX       lr
|L1.1206|
        B        |L1.976|
|L1.1208|
        NOP      
        MOV      r0,r2
        B        |L1.1204|
        ENDP

getc PROC
        MOVS     r1,#0
        LDR      r2,|L1.2220|
        STRB     r1,[r2,#0]
        NOP      
|L1.1222|
        LDR      r1,|L1.2220|
        LDRB     r1,[r1,#0]  ; ucWaitForUart
        CMP      r1,#0
        BEQ      |L1.1222|
        LDR      r1,|L1.2224|
        LDRB     r0,[r1,#0]  ; GetStrTmp
        DSB      
        BX       lr
        ENDP

gets PROC
        PUSH     {r4,r5,lr}
        MOV      r5,r0
        MOVS     r4,#0
        B        |L1.1318|
|L1.1248|
        BL       getc
        STRB     r0,[r5,r4]
        LDRB     r0,[r5,r4]
        CMP      r0,#0xa
        BEQ      |L1.1266|
        LDRB     r0,[r5,r4]
        CMP      r0,#0xd
        BNE      |L1.1284|
|L1.1266|
        MOVS     r0,#0xd
        BL       putc
        MOVS     r0,#0xa
        BL       putc
        MOVS     r0,#0
        STRB     r0,[r5,r4]
        B        |L1.1320|
|L1.1284|
        LDRB     r0,[r5,r4]
        CMP      r0,#0x7f
        BEQ      |L1.1296|
        LDRB     r0,[r5,r4]
        CMP      r0,#8
        BNE      |L1.1310|
|L1.1296|
        CMP      r4,#0
        BLE      |L1.1318|
        ADR      r0,|L1.2228|
        BL       puts
        SUBS     r4,r4,#1
        B        |L1.1318|
|L1.1310|
        LDRB     r0,[r5,r4]
        BL       putc
        ADDS     r4,r4,#1
|L1.1318|
        B        |L1.1248|
|L1.1320|
        NOP      
        MOVS     r0,#1
        POP      {r4,r5,pc}
        ENDP

DispMemData PROC
        PUSH     {r4-r8,lr}
        MOV      r7,r0
        MOV      r5,r1
        MOVS     r4,#0
        MOV      r6,r7
        LSRS     r5,r5,#2
        NOP      
        B        |L1.1390|
|L1.1344|
        TST      r4,#7
        BNE      |L1.1358|
        MOV      r1,r6
        ADR      r0,|L1.2232|
        BL       printf
|L1.1358|
        LDR      r8,[r6],#4
        AND      r0,r4,#7
        CMP      r0,#7
        BNE      |L1.1380|
        MOV      r1,r8
        ADR      r0,|L1.2240|
        BL       printf
        B        |L1.1388|
|L1.1380|
        MOV      r1,r8
        ADR      r0,|L1.2248|
        BL       printf
|L1.1388|
        ADDS     r4,r4,#1
|L1.1390|
        CMP      r4,r5
        BCC      |L1.1344|
        ADR      r0,|L1.2244|
        BL       printf
        POP      {r4-r8,pc}
        ENDP

stoi PROC
        PUSH     {r4-r7,lr}
        SUB      sp,sp,#0x24
        MOV      r5,r0
        MOVS     r1,#0x20
        ADD      r0,sp,#4
        BL       __aeabi_memclr4
        MOV      r0,r5
        BL       strlen
        MOV      r7,r0
        CMP      r7,#8
        BLS      |L1.1432|
        MOVS     r7,#8
|L1.1432|
        MOVS     r6,#0
        B        |L1.1484|
|L1.1436|
        LDRB     r0,[r5,#0]
        ORR      r0,r0,#0x20
        STRB     r0,[r5,#0]
        LDRB     r0,[r5,#0]
        CMP      r0,#0x41
        BLT      |L1.1466|
        LDRB     r0,[r5,#0]
        SUBS     r0,r0,#0x57
        SUBS     r1,r7,#1
        SUBS     r1,r1,r6
        ADD      r2,sp,#4
        STR      r0,[r2,r1,LSL #2]
        B        |L1.1480|
|L1.1466|
        LDRB     r0,[r5,#0]
        SUBS     r0,r0,#0x30
        SUBS     r1,r7,#1
        SUBS     r1,r1,r6
        ADD      r2,sp,#4
        STR      r0,[r2,r1,LSL #2]
|L1.1480|
        ADDS     r5,r5,#1
        ADDS     r6,r6,#1
|L1.1484|
        CMP      r6,r7
        BCC      |L1.1436|
        MOVS     r4,#0
        MOV      r1,#0x10000000
        LDR      r0,[sp,#0x20]
        MLA      r4,r1,r0,r4
        ASRS     r1,r1,#4
        LDR      r0,[sp,#0x1c]
        MLA      r4,r1,r0,r4
        ASRS     r1,r1,#4
        LDR      r0,[sp,#0x18]
        MLA      r4,r1,r0,r4
        ASRS     r1,r1,#4
        LDR      r0,[sp,#0x14]
        MLA      r4,r1,r0,r4
        ASRS     r1,r1,#4
        LDR      r0,[sp,#0x10]
        MLA      r4,r1,r0,r4
        ASRS     r1,r1,#4
        LDR      r0,[sp,#0xc]
        MLA      r4,r1,r0,r4
        MOVS     r1,#0x10
        LDR      r0,[sp,#8]
        MLA      r4,r1,r0,r4
        LDR      r0,[sp,#4]
        ADD      r4,r4,r0
        MOV      r0,r4
        ADD      sp,sp,#0x24
        POP      {r4-r7,pc}
        ENDP

MemoryDump PROC
        PUSH     {r2-r10,lr}
        MOV      r8,r0
        MOV      r9,#0x40
        MOVS     r0,#0
        STR      r0,[sp,#4]
        STR      r0,[sp,#0]
        MOVS     r7,#0x20
        MOVS     r5,#0
        MOV      r1,r7
        MOV      r0,r8
        BL       strchr
        MOV      r5,r0
        CBNZ     r5,|L1.1594|
|L1.1590|
        POP      {r2-r10,pc}
|L1.1594|
        LDRB     r0,[r5,#2]
        BL       tolower
        UXTB     r7,r0
        CMP      r7,#0x78
        BNE      |L1.1664|
        MOVS     r4,#0
        B        |L1.1638|
|L1.1610|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.1628|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1636|
|L1.1628|
        MOVS     r0,#0
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1642|
|L1.1636|
        ADDS     r4,r4,#1
|L1.1638|
        CMP      r4,#8
        BLT      |L1.1610|
|L1.1642|
        NOP      
        ADD      r0,sp,#4
        BL       stoi
        MOV      r6,r0
        ADDS     r0,r5,#3
        ADDS     r1,r0,r4
        ADD      r0,sp,#4
        BL       strcpy
        B        |L1.1720|
|L1.1664|
        MOVS     r4,#0
        B        |L1.1696|
|L1.1668|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.1686|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1694|
|L1.1686|
        MOVS     r0,#0
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1700|
|L1.1694|
        ADDS     r4,r4,#1
|L1.1696|
        CMP      r4,#8
        BLT      |L1.1668|
|L1.1700|
        NOP      
        ADD      r0,sp,#4
        BL       stoi
        MOV      r6,r0
        ADDS     r0,r5,#1
        ADDS     r1,r0,r4
        ADD      r0,sp,#4
        BL       strcpy
|L1.1720|
        MOVS     r7,#0x20
        MOV      r1,r7
        ADD      r0,sp,#4
        BL       strchr
        MOV      r5,r0
        LDRB     r0,[r5,#2]
        BL       tolower
        UXTB     r7,r0
        CMP      r7,#0x78
        BNE      |L1.1798|
        MOVS     r4,#0
        B        |L1.1782|
|L1.1748|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.1772|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CBZ      r0,|L1.1772|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        STRB     r0,[sp,r4]
        B        |L1.1780|
|L1.1772|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.1786|
|L1.1780|
        ADDS     r4,r4,#1
|L1.1782|
        CMP      r4,#8
        BLT      |L1.1748|
|L1.1786|
        NOP      
        MOV      r0,sp
        BL       stoi
        MOV      r9,r0
        B        |L1.1850|
|L1.1798|
        MOVS     r4,#0
        B        |L1.1836|
|L1.1802|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.1826|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CBZ      r0,|L1.1826|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        STRB     r0,[sp,r4]
        B        |L1.1834|
|L1.1826|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.1840|
|L1.1834|
        ADDS     r4,r4,#1
|L1.1836|
        CMP      r4,#8
        BLT      |L1.1802|
|L1.1840|
        NOP      
        MOV      r0,sp
        BL       atoi
        MOV      r9,r0
|L1.1850|
        CMP      r6,#0x20000000
        BCS      |L1.1878|
        CMP      r6,#0x20000
        BLS      |L1.1878|
        ADR      r0,|L1.2256|
        BL       printf
        MOV      r1,r6
        ADR      r0,|L1.2312|
        BL       printf
        B        |L1.1590|
|L1.1878|
        MOV      r1,r9
        MOV      r0,r6
        BL       DispMemData
        NOP      
        B        |L1.1590|
        ENDP

MemorySet PROC
        PUSH     {r2-r10,lr}
        MOV      r8,r0
        MOVS     r0,#0
        STR      r0,[sp,#4]
        STR      r0,[sp,#0]
        MOVS     r7,#0x20
        MOVS     r5,#0
        MOV      r1,r7
        MOV      r0,r8
        BL       strchr
        MOV      r5,r0
        CBNZ     r5,|L1.1922|
|L1.1918|
        POP      {r2-r10,pc}
|L1.1922|
        LDRB     r0,[r5,#2]
        BL       tolower
        UXTB     r7,r0
        CMP      r7,#0x78
        BNE      |L1.1992|
        MOVS     r4,#0
        B        |L1.1966|
|L1.1938|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.1956|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1964|
|L1.1956|
        MOVS     r0,#0
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.1970|
|L1.1964|
        ADDS     r4,r4,#1
|L1.1966|
        CMP      r4,#8
        BLT      |L1.1938|
|L1.1970|
        NOP      
        ADD      r0,sp,#4
        BL       stoi
        MOV      r6,r0
        ADDS     r0,r5,#3
        ADDS     r1,r0,r4
        ADD      r0,sp,#4
        BL       strcpy
        B        |L1.2048|
|L1.1992|
        MOVS     r4,#0
        B        |L1.2024|
|L1.1996|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.2014|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.2022|
|L1.2014|
        MOVS     r0,#0
        ADD      r1,sp,#4
        STRB     r0,[r1,r4]
        B        |L1.2028|
|L1.2022|
        ADDS     r4,r4,#1
|L1.2024|
        CMP      r4,#8
        BLT      |L1.1996|
|L1.2028|
        NOP      
        ADD      r0,sp,#4
        BL       stoi
        MOV      r6,r0
        ADDS     r0,r5,#1
        ADDS     r1,r0,r4
        ADD      r0,sp,#4
        BL       strcpy
|L1.2048|
        CMP      r6,#0x20000000
        BCS      |L1.2076|
        CMP      r6,#0x20000
        BLS      |L1.2076|
        ADR      r0,|L1.2344|
        BL       printf
        MOV      r1,r6
        ADR      r0,|L1.2312|
        BL       printf
        B        |L1.1918|
|L1.2076|
        MOVS     r7,#0x20
        MOV      r1,r7
        ADD      r0,sp,#4
        BL       strchr
        MOV      r5,r0
        LDRB     r0,[r5,#2]
        BL       tolower
        UXTB     r7,r0
        CMP      r7,#0x78
        BNE      |L1.2154|
        MOVS     r4,#0
        B        |L1.2138|
|L1.2104|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.2128|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        CBZ      r0,|L1.2128|
        ADDS     r0,r5,#3
        LDRB     r0,[r0,r4]
        STRB     r0,[sp,r4]
        B        |L1.2136|
|L1.2128|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.2142|
|L1.2136|
        ADDS     r4,r4,#1
|L1.2138|
        CMP      r4,#8
        BLT      |L1.2104|
|L1.2142|
        NOP      
        MOV      r0,sp
        BL       stoi
        MOV      r9,r0
        B        |L1.2206|
|L1.2154|
        MOVS     r4,#0
        B        |L1.2192|
|L1.2158|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.2182|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        CBZ      r0,|L1.2182|
        ADDS     r0,r5,#1
        LDRB     r0,[r0,r4]
        STRB     r0,[sp,r4]
        B        |L1.2190|
|L1.2182|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.2196|
|L1.2190|
        ADDS     r4,r4,#1
|L1.2192|
        CMP      r4,#8
        BLT      |L1.2158|
|L1.2196|
        NOP      
        MOV      r0,sp
        BL       atoi
        MOV      r9,r0
|L1.2206|
        MOV      r1,r9
        MOV      r0,r6
        BL       write_reg
        NOP      
        B        |L1.1918|
        DCW      0x0000
|L1.2220|
        DCD      ucWaitForUart
|L1.2224|
        DCD      GetStrTmp
|L1.2228|
        DCB      "\b \b",0
|L1.2232|
        DCB      "0x%x   ",0
|L1.2240|
        DCB      "0x%x"
|L1.2244|
        DCB      "\n",0
        DCB      0
        DCB      0
|L1.2248|
        DCB      "0x%x ",0
        DCB      0
        DCB      0
|L1.2256|
        DCB      "We can't read 0x2000_0000 ~ 0x0002_0000 due to 128[KB]\n"
        DCB      0
|L1.2312|
        DCB      "The address you typed is 0x%x.\n",0
|L1.2344|
        DCB      "We can't write 0x2000_0000 ~ 0x0002_0000 due to 128[KB]"
        DCB      "\n",0
        DCB      0
        DCB      0
        DCB      0
        ENDP

MemoryDump_Sub PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOVS     r0,#0
        STR      r0,[sp,#0]
        MOVS     r6,#0x78
        LDRB     r0,[r5,#1]
        BL       tolower
        UXTB     r6,r0
        CMP      r6,#0x78
        BNE      |L1.2474|
        MOVS     r4,#0
        B        |L1.2458|
|L1.2430|
        ADDS     r0,r5,#2
        LDRB     r0,[r0,r4]
        CMP      r0,#0x20
        BEQ      |L1.2448|
        ADDS     r0,r5,#2
        LDRB     r0,[r0,r4]
        STRB     r0,[sp,r4]
        B        |L1.2456|
|L1.2448|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.2462|
|L1.2456|
        ADDS     r4,r4,#1
|L1.2458|
        CMP      r4,#8
        BLT      |L1.2430|
|L1.2462|
        NOP      
        MOV      r0,sp
        BL       stoi
        MOV      r7,r0
        B        |L1.2516|
|L1.2474|
        MOVS     r4,#0
        B        |L1.2502|
|L1.2478|
        LDRB     r0,[r5,r4]
        CMP      r0,#0x20
        BEQ      |L1.2492|
        LDRB     r0,[r5,r4]
        STRB     r0,[sp,r4]
        B        |L1.2500|
|L1.2492|
        MOVS     r0,#0
        STRB     r0,[sp,r4]
        B        |L1.2506|
|L1.2500|
        ADDS     r4,r4,#1
|L1.2502|
        CMP      r4,#8
        BLT      |L1.2478|
|L1.2506|
        NOP      
        MOV      r0,sp
        BL       atoi
        MOV      r7,r0
|L1.2516|
        MOV      r0,r7
        POP      {r3-r7,pc}
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=0

ucTxBufferEmpty
        DCB      0x00
ucWaitForUart
        DCB      0x00
GetStrTmp
        DCB      0x00

        EXPORT putc [CODE]
        EXPORT puts [CODE]
        EXPORT unsigned_divide [CODE]
        EXPORT do_div [CODE]
        EXPORT number [CODE]
        EXPORT sprintf [CODE]
        EXPORT printf [CODE]
        EXPORT write_reg [CODE]
        EXPORT read_reg [CODE]
        EXPORT set_bit [CODE]
        EXPORT clear_bit [CODE]
        EXPORT get_bit [CODE]
        EXPORT write_mask [CODE]
        EXPORT read_mask [CODE]
        EXPORT poll_on_set [CODE]
        EXPORT poll_on_set_limit [CODE]
        EXPORT poll_on_reset [CODE]
        EXPORT hextoint [CODE]
        EXPORT getc [CODE]
        EXPORT gets [CODE]
        EXPORT DispMemData [CODE]
        EXPORT stoi [CODE]
        EXPORT MemoryDump [CODE]
        EXPORT MemorySet [CODE]
        EXPORT MemoryDump_Sub [CODE]
        EXPORT ucTxBufferEmpty [DATA,SIZE=1]
        EXPORT ucWaitForUart [DATA,SIZE=1]
        EXPORT GetStrTmp [DATA,SIZE=1]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT pCOMTX_RX [DATA]
        IMPORT pFEESTA [DATA]
        IMPORT pFEEKEY [DATA]
        IMPORT pFEEADR0L [DATA]
        IMPORT pFEEADR0H [DATA]
        IMPORT pFEECMD [DATA]
        IMPORT pFEECON0 [DATA]
        IMPORT __aeabi_memclr4 [CODE]
        IMPORT strlen [CODE]
        IMPORT strchr [CODE]
        IMPORT tolower [CODE]
        IMPORT strcpy [CODE]
        IMPORT atoi [CODE]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.06"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
