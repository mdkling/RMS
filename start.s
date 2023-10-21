
.cpu cortex-m0plus
.thumb
.syntax unified

;@ Section Constants
ATOMIC_XOR         = 0x1000
ATOMIC_SET         = 0x2000
ATOMIC_CLR         = 0x3000
PBB_BASE           = 0xE0000000
SYST_CSR           = PBB_BASE + 0xE010
SYST_RVR           = 0x04
SYST_CVR           = 0x08
SYST_CALIB         = 0x0C
EXTERNAL_INTS_E      = PBB_BASE + 0xE180
EXTERNAL_INTS      = PBB_BASE + 0xE280
ICSR_REG           = PBB_BASE + 0xED04
VTOR               = PBB_BASE + 0xED08
XOSC_BASE          = 0x40024000
XOSC_CTRL_RW       = XOSC_BASE + 0x00
XOSC_CTRL_SET      = XOSC_CTRL_RW + 0x2000
XOSC_STATUS        = XOSC_BASE + 0x04
XOSC_STARTUP       = XOSC_BASE + 0x0C
XOSC_CTRL_RW_OFF       =  0x00
XOSC_STATUS_OFF        =  0x04
XOSC_STARTUP_OFF       =  0x0C
CLOCKS_BASE        = 0x40008000
CLK_REF_CTRL       = CLOCKS_BASE + 0x30
CLK_REF_SELECTED   = CLOCKS_BASE + 0x38
CLK_SYS_CTRL       = CLOCKS_BASE + 0x3C
CLK_SYS_SELECTED   = CLOCKS_BASE + 0x44
CLK_PERI_CTRL      = CLOCKS_BASE + 0x48
CLK_SYS_RESUS_CTRL = CLOCKS_BASE + 0x78
CLK_REF_CTRL_OFF       = 0x30
CLK_REF_SELECTED_OFF   = 0x38
CLK_SYS_CTRL_OFF      = 0x3C
CLK_SYS_SELECTED_OFF   = 0x44
CLK_PERI_CTRL_OFF      = 0x48
CLK_SYS_RESUS_CTRL_OFF = 0x78
PSM_BASE           = 0x40010000
PSM_WDSEL          = PSM_BASE + 0x08
SYSCFG_BASE        = 0x40004000
PROC_0_NMI         = SYSCFG_BASE + 0x00

SIO_BASE               = 0xD0000000
SIO_GPIO_OUT_CLR       = SIO_BASE + 0x18
SIO_GPIO_OUT_XOR       = SIO_BASE + 0x1C 
SIO_GPIO_OE_SET        = SIO_BASE + 0x24
SIO_GPIO_OE_CLR        = SIO_BASE + 0x28
SIO_FIFO_ST            = 0x050
SIO_FIFO_WRITE         = 0x054
SIO_FIFO_READ          = 0x058
SIO_SIGNED_DIVIDEND    = 0x068
SIO_SIGNED_DIVISOR     = 0x06C
SIO_QUOTIENT           = 0x070
SIO_REMAINDER          = 0x074
SIO_DIV_CSR            = 0x078
SIO_SPINLOCK_0         = SIO_BASE + 0x16C

PPB_BASE               = 0xE0000000
PPB_INTERRUPT_PEND     = PPB_BASE + 0xE280


IO_BANK0_BASE          = 0x40014000
IO_GPIO00_CTRL         = IO_BANK0_BASE + 0x004
IO_GPIO01_CTRL         = IO_BANK0_BASE + 0x00C
IO_GPIO12_CTRL         = IO_BANK0_BASE + 0x064
IO_GPIO13_CTRL         = IO_BANK0_BASE + 0x06C


IO_GPIO25_CTRL_RW      = IO_BANK0_BASE + 0x0CC

RESETS_RESET_CLR       = 0x4000C000 + 0x3000
RESETS_RESET_DONE_RW   = 0x4000C000 + 0x8  

WATCHDOG_BASE      = 0x40058000
WATCHDOG_CNTRL     = WATCHDOG_BASE

PLL_SYS_BASE       = 0x40028000
PLL_SYS_CTRL       = PLL_SYS_BASE + 0x00
PLL_SYS_POWER      = PLL_SYS_BASE + 0x04
PLL_SYS_FBDIV      = PLL_SYS_BASE + 0x08
PLL_SYS_PRIM       = PLL_SYS_BASE + 0x0C

UART0_BASE         = 0x40034000
UART0_DR           = 0x000
UART0_FR           = 0x018
UART0_IBRD         = 0x024
UART0_FBRD         = 0x028
UART0_LCR          = 0x02C
UART0_CR           = 0x030
UART0_IRMASK       = 0x038
UART0_IRMASKSTATUS = 0x040
UART0_IRCLEAR      = 0x044
UART0_DMA_CR       = 0x048

DMA_BASE           = 0x50000000
DMA0_READ          = 0x000
DMA0_WRITE         = 0x004
DMA0_TRAN_CNT      = 0x008
DMA0_CTRL          = 0x00C
DMA1_READ          = 0x054
DMA1_WRITE         = 0x058
DMA1_TRAN_CNT      = 0x05C
DMA1_CTRL          = 0x050

DMA_2_BASE         = 0x50000090
DMA2_CTRL          = 0x0
DMA2_READ          = 0x4
DMA2_WRITE         = 0x8
DMA2_TRAN_CNT      = 0xc
DMA3_CTRL          = 0x40
DMA3_READ          = 0x44
DMA3_WRITE         = 0x48
DMA3_TRAN_CNT      = 0x4c

FIFO_BUFF_START  = 0x20040000
FIFO_WRITER_ADDR = 0x20040100

END_OF_RAM				= 0x20042000
CORE0_C_STACK			= 0x20041800
CORE0_FITH_PARAM_STACK	= 0x20042000
CORE0_FITH_RETURN_STACK	= 0x20041C00
CORE0_FITH_GLOBALS1		= 0x20041000
CORE0_FITH_GLOBALS2		= 0x20041100
CORE0_FITH_BLOCKS		= 0x20041200
CORE0_HEAP_TOP		= 0x20040000
CORE1_STACK_START       = 0x20040C00

SIZE_OF_RAM_BUILD = (__bss_start__-0x20000000)


CMD_READ =  0x03

// Value is number of address bits divided by 4
ADDR_L =  6
SSI_CTRLR0_SPI_FRF_VALUE_STD =  0x0
SSI_CTRLR0_SPI_FRF_LSB  =      21
SSI_SPI_CTRLR0_XIP_CMD_LSB = 24
SSI_SPI_CTRLR0_ADDR_L_LSB = 2
SSI_SPI_CTRLR0_INST_L_LSB = 8
SSI_SPI_CTRLR0_TRANS_TYPE_VALUE_1C1A = 0
SSI_SPI_CTRLR0_TRANS_TYPE_LSB = 0
SSI_CTRLR0_DFS_32_LSB = 16
SSI_CTRLR0_TMOD_VALUE_EEPROM_READ = 3
SSI_CTRLR0_TMOD_LSB = 8
CTRLR0_XIP =  (SSI_CTRLR0_SPI_FRF_VALUE_STD << SSI_CTRLR0_SPI_FRF_LSB) + (31 << SSI_CTRLR0_DFS_32_LSB) + (SSI_CTRLR0_TMOD_VALUE_EEPROM_READ  << SSI_CTRLR0_TMOD_LSB)

SPI_CTRLR0_XIP = (CMD_READ << SSI_SPI_CTRLR0_XIP_CMD_LSB) + (ADDR_L << SSI_SPI_CTRLR0_ADDR_L_LSB) + (2 << SSI_SPI_CTRLR0_INST_L_LSB) + (SSI_SPI_CTRLR0_TRANS_TYPE_VALUE_1C1A << SSI_SPI_CTRLR0_TRANS_TYPE_LSB)


;@ Section Register Renaming
TOS .req r0
W   .req r1
RSP .req r4
G1  .req r5
G2  .req r6
G3  .req r7
PSP .req sp

;@ Section Macros


@@ Word header macro

.macro define_builtIn_word name, type
	.balign 2
	.set link, 20b - 20f
20:	
	.hword link
	.set link, 20b
	.byte \type
	.byte 22f - 21f - 1
21:	.asciz "\name"
22:	.balign 2, 0
.endm

;@ Section Vector Table
.global vector_table
vector_table:
	b reset
	.balign 4
	.word reset ;@ has to be offset 4
	.word REBOOT ;@purgatory  ;@ 2 NMI
	.word whoisme ;@purgatory  ;@ 3 HardFault

	.word SIZE_OF_RAM_BUILD ;@ 4 Reserved
	.word flashEntry   ;@ 5 Reserved
	.word 0  ;@ 6 Reserved
	.word REBOOT  ;@ 7 Reserved
	
	.word REBOOT  ;@ 8 Reserved
	.word REBOOT  ;@ 9 Reserved
	.word REBOOT  ;@ 10 Reserved
	.word REBOOT  ;@ 11 SVCall
	
	.word REBOOT  ;@ 12 Reserved
	.word REBOOT  ;@ 13 Reserved
	.word REBOOT  ;@ 14 PendSV isrPendSvCall
	.word REBOOT  ;@ 15 SysTick
	
	.word alarm1ISRx   ;@ 16 external interrupt 0
	.word alarm2ISR
	.word alarm3ISR
	.word alarm4ISR
	
	.word REBOOT   ;@ 4
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 8
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 12
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 16
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 20
	.word REBOOT
	.word REBOOT
	.word REBOOT
	
	.word REBOOT   ;@ 24
	.word REBOOT
	.word SDI_1;@runTasksP0	;@ 26 first software defined interrupt
	.word SDI_2;@runTasksP1	;@ 27
	
	.word SDI_3;@runTasksP2   ;@ 28
	.word REBOOT
	.word REBOOT
	.word REBOOT

.balign 2
.code 16
.thumb_func
.global setZeroWait
.type setZeroWait, %function
setZeroWait:
	ldr  r2,=DMA_2_BASE ;@ base reg
1:
	ldr  r3,[r2, #DMA2_CTRL]
	lsls r3, 7
	bmi  1b
	bx lr

.balign 4
.global boot2Entry
;@~ .thumb_func
;@~ .type boot2Entry, %function
boot2Entry:
	bl   clocksSetup ;@ set up clocks
	bl   xipSetup ;@ set up xip to do flash reads
	;@ r0 = 0, r1 = 1 rest is garbage
	
	lsls r6,r1,#29 ;@ RAM addr
	ldr  r5, =0x10000000 + 4096;@ flash addr
	
	ldr  r1, [r5, #16] ;@ size of code
	;@ r0=0, r1=size of code;r5=flash base;r6=RAM base; 
1:
	ldr  r4, [r5, r0] ;@ load from flash
	str  r4, [r6, r0] ;@ store to ram
	adds r0, 4
	cmp  r0, r1
	bne  1b
	ldr  r4, [r6, #20] ;@ load flash entry vector
	blx  r4

.global clocksSetup
.thumb_func
.type clocksSetup, %function
clocksSetup:
    ldr r2,=XOSC_BASE
	movs r1, 47
	str r1,[r2, #XOSC_STARTUP_OFF] ;@ set timeout, from manual
	ldr r1,=0xFABAA0
	str r1,[r2, #XOSC_CTRL_RW_OFF] ;@ set enable and speed range

1:
	ldr r1,[r2, #XOSC_STATUS_OFF] ;@ read status to see if it is ready
	cmp r1, #0
	bge 1b

	ldr  r2,=CLOCKS_BASE
	movs r1, 0
	str  r1,[r2, #CLK_SYS_RESUS_CTRL_OFF] ;@ disable resus mis-feature
	movs r1, 2
	str  r1,[r2, #CLK_REF_CTRL_OFF] ;@ move reference clock to crystal
	movs r3, 4
1:
	ldr r1,[r2, #CLK_REF_SELECTED_OFF] ;@ read status to see if it is ready
	tst r1, r3
	beq 1b
	
	;@ bring up PLL
	movs r3, 1
	lsls r3,r3,#12
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr  r2,=PLL_SYS_BASE ;@ base reg 
	movs r1, 80
	str  r1,[r2, #PLL_SYS_FBDIV - PLL_SYS_BASE] ;@ set up multiplier
	
	ldr r0,=PLL_SYS_POWER + ATOMIC_CLR ;@ turn on power
	movs r1, 0x21
	str r1,[r0]
	
	movs r3, 1
	lsls r3,r3,#31
1:
	ldr r1,[r2] ;@ wait for resonance cascade
	tst r1, r3
	beq 1b
	
	;@~ ldr r1,=(((4<<16)|(1<<12))>>12)
	movs r1, 0x42
	lsls r1, 12
	str r1,[r2, #PLL_SYS_PRIM - PLL_SYS_BASE] ;@ set post dividers
	
	;@ turn on post dividers
	movs r1, 0x08
	str r1,[r0]
	;@ PLL_SYS is now at 120 mhz, pico max is 133, this is 90% of that
	;@ so nice margin of safety
	
	ldr r2,=CLOCKS_BASE
	movs r1, 1
	str r1,[r2, #CLK_SYS_CTRL_OFF] ;@ set sys clock to PLL_SYS
	movs r3,2
1:
	ldr r1,[r2, #CLK_REF_SELECTED_OFF] ;@ read status to see if it is ready
	tst r1, r3
	bne 1b
	
	movs r1, 0x88
	lsls r1, 4
	str  r1,[r2, #CLK_PERI_CTRL_OFF] ;@ set peripheral clock to the ref clock and enable
	;@ we are done
	bx   lr

.global xipSetup
.thumb_func
.type xipSetup, %function
xipSetup: ;@ r0-r3 are garbage
	;@ try to set up xip
	movs  r3, 0x18                // Use as base address where possible
	lsls r3, 24

    // Disable SSI to allow further config
    movs r0, #0
    str  r0, [r3, #0x08]
    
    // NDF=0 (single 32b read)
    str  r0, [r3, #0x04]

    // Set baud rate
    movs r1, #4
    str  r1, [r3, #0x14]

    ldr  r1, =(CTRLR0_XIP)
    str  r1, [r3, #0x00]

    ldr  r1, =(SPI_CTRLR0_XIP)
    adds r3, #244
    str  r1, [r3]

    // Re-enable SSI
    subs r3, #244
    movs r1, #1
    str  r1, [r3, #0x08]
	
	bx   lr

.global dmaSetup
.thumb_func
dmaSetup:
;@ get DMA out of reset
	movs r3, 4 
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr  r2,=DMA_2_BASE ;@ base reg
	ldr  r1, =(0x3F<<15)|(1<<21)|(2<<11)|(1<<5)|(2<<2)|(1<<0)
	str  r1,[r2, #DMA2_CTRL]
	adr  r1, ZERO_CONSTANT_LOCATION
	str  r1,[r2, #DMA2_READ] ;@ set as DMA read
	
	bx   lr

.balign 2
.code 16
.thumb_func
.global setZero
.type setZero, %function
setZero: ;@ r0 = dst r1 = size
	ldr  r2,=DMA_2_BASE ;@ base reg
	str  r0,[r2, #DMA2_WRITE]
	lsrs r1, 2
	str  r1,[r2, #DMA2_TRAN_CNT]
	bx lr

.balign 4
ZERO_CONSTANT_LOCATION:
.word 0
.balign 4
.ltorg

.global reset
.balign 2
.code 16
.thumb_func
.type reset, %function
reset:
	;@ config clocks
	bl clocksSetup
.global flashEntry
.thumb_func
flashEntry:
	;@ set up DMA to clear RAM
	bl   dmaSetup
	ldr  r0, = __bss_start__
	ldr  r1, = __bss_end__  ;@END_OF_RAM
	subs r1, r0
	bl   setZero
	;@ configure vector table
	ldr  r1, =VTOR
	ldr  r0, =vector_table
	str  r0,[r1]
	;@ set stack
	ldr  r0,=END_OF_RAM  ;@INITIAL_STACK
	msr  MSP, r0
	;@ take devices out of reset
	movs	r2, 1
	bl	resetIOBank
	bl	configUART
	bl	setZeroWait ;@ until there is enough time to rip through both stacks
	;@~ bl   helper_unlock
	bl	memsys5Init
	bl	picoInit
	bl	printHelloBanner
.global resetAllRegs
	ldr  r0,=END_OF_RAM  ;@INITIAL_STACK
	mov  sp, r0
resetAllRegs:
1:	wfe
	bl	task_exec
	b    1b

.thumb_func
.global REBOOT
.type REBOOT, %function
REBOOT:
	;@~ ldr r0,=PSM_WDSEL ;@ ENABLE MASS RESET
	;@~ ldr r1,=0x1FFFC
	;@~ str r1,[r0]
	bl	uart0processAllOutputs
	ldr	r0,=WATCHDOG_CNTRL ;@ reboot
	movs	r1, 1
	lsls	r1, 31
	str	r1,[r0]
	b	purgatory

.balign 2
.code 16
.thumb_func
.type whoisme, %function
whoisme:
	;@~ mrs r0, IPSR
	push	{r4,r5,r6,r7,lr}
	mov	r0, sp
	bl	printStackStrace
	;@~ b	REBOOT
	pop	{r4,r5,r6,r7,pc}

.code 16
.thumb_func
.type purgatory, %function
purgatory:
	b purgatory

.balign 2
.code 16
.thumb_func
.type resetIOBank, %function
resetIOBank:
	lsls r3,r2,#5
	lsls r1,r2,#10
	adds r3, r1
	lsls r1,r2,#11
	adds r3, r1
	lsls r1,r2,#21
	adds r3, r1
	ldr  r0, =RESETS_RESET_CLR
	str  r3, [r0]
	ldr  r0, =RESETS_RESET_DONE_RW
	1:
	ldr  r1,[r0]
	tst  r1, r3
	beq  1b
	lsls r3,r2,25
	ldr  r0, =SIO_GPIO_OE_CLR
	str  r3, [r0]
	ldr  r0, =SIO_GPIO_OUT_CLR
	str  r3, [r0]
	ldr  r0, =SIO_GPIO_OE_SET
	str  r3, [r0]
	
	movs r1, 5  ;@ GPIO control
	;@~ movs r1, 7 ;@ PIO 1 control
	ldr  r0, =IO_GPIO25_CTRL_RW
	str  r1, [r0]
	bx   lr
	
.balign 2
.code 16
.thumb_func
.type configUART, %function
configUART:
	;@ Section UART 
	;@ bring up UART
	lsls r3,r2,#22
	ldr r0, =RESETS_RESET_CLR
	str r3, [r0]
	ldr r0, =RESETS_RESET_DONE_RW ;@ read status to see if it is ready
1:
	ldr r1,[r0]
	tst r1, r3
	beq 1b
	
	ldr r4,=UART0_BASE ;@ base reg
	ldr r1,=6
	str r1,[r4, #UART0_IBRD] ;@ set up baud
	
	ldr r1,=33
	str r1,[r4, #UART0_FBRD] ;@ set up fractional baud
	
	ldr r1,=0x70
	str r1,[r4, #UART0_LCR] ;@ set up fractional baud
	
	ldr r1,=(1<<9)|(1<<8)|(1<<0)
	str r1,[r4, #UART0_CR] ;@ enable UART, TX and RX
	
	;@~ ldr r1,=(1<<6)|(1<<4) ;@ |(1<<5)
	;@~ str r1,[r4, #UART0_IRMASK]
	
	;@~ ldr r0,=IO_GPIO00_CTRL ;@ set gpio 0 to UART 0 TX
	;@~ ldr r1,=2
	;@~ str r1,[r0]
	
	;@~ ldr r0,=IO_GPIO01_CTRL ;@set gpio 1 to UART 0 RX
	;@~ ldr r1,=2
	;@~ str r1,[r0]
	
	ldr r0,=IO_GPIO12_CTRL ;@ set gpio 0 to UART 0 TX
	ldr r1,=2
	str r1,[r0]
	
	ldr r0,=IO_GPIO13_CTRL ;@set gpio 1 to UART 0 RX
	ldr r1,=2
	str r1,[r0]

	
	bx lr

.balign 2
.code 16
.thumb_func
.global readSysTimerVal
.type readSysTimerVal, %function
readSysTimerVal:
	ldr  r1, =SYST_CSR
	ldr  r1, [r1, #SYST_CVR]
	subs r0, r1, r0
	bx lr

.balign 2
.code 16
.thumb_func
.global startSysTimer
.type startSysTimer, %function
startSysTimer:
	ldr  r1, =SYST_CSR
	ldr  r0, =0xFFFFFF
	str  r0, [r1, #SYST_RVR]
	movs r0, 0
	str  r0, [r1, #SYST_CVR]
	movs r0, 5
	str  r0, [r1, #0]
	bx lr

.balign 2
.code 16
.thumb_func
.global endSysTimer
.type endSysTimer, %function
endSysTimer:
	ldr  r1, =SYST_CSR
	ldr  r2, [r1, #SYST_CVR]
	ldr  r0, =0xFFFFFF
	subs r0, r0, r2
	movs r2, 0
	str  r2, [r1, #0]
	bx lr

.thumb_func
.global pengumDiv
pengumDiv:
	push	{r6,r7,lr}
	pop	{r0,r1}
	bl	asmDiv
	movs	r7, r0
	pop	{pc}
.balign 2
.code 16
.thumb_func
.global asmDiv
.type asmDiv, %function
asmDiv: ;@ r0 = DIVISOR r1 = DIVIDEND
	ldr  r2, = SIO_BASE
	str  r1, [r2, #SIO_SIGNED_DIVIDEND]
	str  r0, [r2, #SIO_SIGNED_DIVISOR] ;@ now takes 8 cycles to finish
	b    1f
1:  b    1f
1:  b    1f
1:  b    1f
1:
	ldr  r0, [r2, #SIO_QUOTIENT]
	bx   lr

.thumb_func
.global pengumMod
pengumMod:
	push	{r6,r7,lr}
	pop	{r0,r1}
	bl	asmMod
	movs	r7, r0
	pop	{pc}
.balign 2
.code 16
.thumb_func
.global asmMod
.type asmMod, %function
asmMod: ;@ r0 = DIVISOR r1 = DIVIDEND
	ldr  r2, = SIO_BASE
	str  r1, [r2, #SIO_SIGNED_DIVIDEND]
	str  r0, [r2, #SIO_SIGNED_DIVISOR] ;@ now takes 8 cycles to finish
	b    1f
1:  b    1f
1:  b    1f
1:  b    1f
1:
	ldr  r0, [r2, #SIO_REMAINDER]
	ldr  r1, [r2, #SIO_QUOTIENT]
	bx   lr

.balign 2
.code 16
.thumb_func
.global asmGetDiv
.type asmGetDiv, %function
asmGetDiv: ;@ call right after asmMod
	movs r0, r1
	bx   lr

.balign 4
.ltorg

.balign 2
.code 16
.thumb_func
.global stringLen
.type stringLen, %function
stringLen: ;@ r0 has pointer to start of string, check 4 byte alignment
	movs r1, r0
	movs r0, 0
	movs r2, 3
	tst  r2, r1
	bne  stringLenByte
	movs r2, 0xFF
5:
	ldm  r1!, {r3}
	tst  r3, r2
	beq  1f
	lsrs r3, 8
	tst  r3, r2
	beq  2f
	lsrs r3, 8
	tst  r3, r2
	beq  3f
	lsrs r3, 8
	tst  r3, r2
	beq  4f
	adds r0, 4
	b    5b
4:
	adds r0, 3
	bx   lr
3:
	adds r0, 1
2:
	adds r0, 1
1:
	bx   lr

1:
	adds r0, 1
stringLenByte:
	ldrb r2, [r1, r0]
	cmp  r2, 0
	bne  1b
	bx   lr

;@ ARM implementations for the compiler
.global __gnu_thumb1_case_uqi
.thumb_func
__gnu_thumb1_case_uqi:
	mov     r12, r1
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r1, r1, #1
	ldrb    r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	mov     r1, r12
	bx      lr

.global __gnu_thumb1_case_sqi
.thumb_func
__gnu_thumb1_case_sqi:
	mov     r12, r1
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r1, r1, #1
	ldrsb   r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	mov     r1, r12
	bx      lr

.global __gnu_thumb1_case_uhi
.thumb_func
__gnu_thumb1_case_uhi:
	push    {r0, r1}
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r0, r0, #1
	lsls    r1, r1, #1
	ldrh    r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	pop     {r0, r1}
	bx      lr

.global __gnu_thumb1_case_shi
.thumb_func
__gnu_thumb1_case_shi:
	push    {r0, r1}
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r0, r0, #1
	lsls    r1, r1, #1
	ldrsh   r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	pop     {r0, r1}
	bx      lr

.global __gnu_thumb1_case_si
.thumb_func
__gnu_thumb1_case_si:
	push	{r0, r1}
	mov	r1, lr
	adds.n	r1, r1, #2
	lsrs	r1, r1, #2
	lsls	r0, r0, #2
	lsls	r1, r1, #2
	ldr	r0, [r1, r0]
	adds	r0, r0, r1
	mov	lr, r0
	pop	{r0, r1}
	mov	pc, lr

	
.balign 4
.ltorg

.thumb_func
.global pengumAbs
pengumAbs:
	asrs	r6, r7, 31
	adds	r7, r7, r6
	eors	r7, r6
	bx	lr

.thumb_func
.global pengumAnd
pengumAnd:
	rsbs	r7, r7, 0
	sbcs	r7, r7
	ands	r7, r6
	bx	lr

.thumb_func
.global pengumPs
pengumPs:
	push	{lr}
	movs	r0, r7
	bl	io_prints
	pop	{pc}
.thumb_func
.global pengumPsn
pengumPsn:
	push	{lr}
	movs	r0, r7
	bl	io_printsn
	pop	{pc}

.thumb_func
.global pengumPi
pengumPi:
	push	{lr}
	movs	r0, r7
	bl	io_printi
	pop	{pc}
.thumb_func
.global pengumPin
pengumPin:
	push	{lr}
	movs	r0, r7
	bl	io_printin
	pop	{pc}

.thumb_func
.global pengumPh
pengumPh:
	push	{lr}
	movs	r0, r7
	bl	io_printh
	pop	{pc}
.thumb_func
.global pengumPhn
pengumPhn:
	push	{lr}
	movs	r0, r7
	bl	io_printhn
	pop	{pc}

.thumb_func
.global pengumPc
pengumPc:
	push	{lr}
	movs	r0, r7
	bl	uart0_outByte
	pop	{pc}

.thumb_func
.global pengumFree
pengumFree:
	push	{lr}
	movs	r0, r7
	bl	free
	pop	{pc}

.thumb_func
.global pengumZalloc
pengumZalloc:
	push	{lr}
	movs	r0, r7
	bl	zalloc
	movs	r7, r0
	pop	{pc}

.thumb_func
.global pengumRealloc
pengumRealloc:
	push	{lr}
	movs	r0, r7
	movs	r1, r6
	bl	memsys5Realloc
	movs	r7, r0
	pop	{pc}

TIMER_BASE 		= 0x40054000
TIMER_LOW_RAW 		= 0x28
TIMER_ALARM0 		= 0x10
TIMER_INTR	 	= 0x34
TIMER_1_INCREMENT 	= 500

.thumb_func
.global alarm1ISRx
alarm1ISRx: ;@ interrupt routine. r0-r3,r12 are already preserved
	;@	reset timer interrupt and clear interrupt
	ldr		r0, = TIMER_BASE
	movs		r1, 1
	str		r1, [r0, #TIMER_INTR]
	ldr		r1, [r0, #TIMER_LOW_RAW]
	ldr		r2, = TIMER_1_INCREMENT
	adds		r1, r2
	str		r1, [r0, #TIMER_ALARM0]
	;@~ movs		r1, 1
	;@~ str		r1, [r0, #TIMER_INTR]
	;@	save off interrupted return location in r10
	ldr		r0, [sp, #0x18]
	mov		r10, r0
	;@~ push	{lr}
	;@~ bl		startSysTimer
	;@~ pop		{r0}
	;@~ mov		lr, r0
	;@	store ISR wrap into return location
	adr		r0, alarm1ISRx_wrap
	str		r0, [sp, #0x18]
	bx		lr

.balign 4
.thumb_func
.global alarm1ISRx_wrap
alarm1ISRx_wrap:
	push		{r0,r1,r2,r3,r4,r5} ;@ capture r0-r4, pc
	mov		r0, r10 // r10 is used to store return address
	mrs		r1, APSR	;@ save off flags
	adds		r0, 1		;@ put thumb bit on return addr
	str		r0, [sp, #20]	;@ overwrite r5 with return address
	mov		r0, r12		
	push		{r0,r1,lr}	;@ capture r12, flags, lr
.thumb_func
.global RMS_task0_wrap
RMS_task0_wrap:
	adr		r1, periodCount
	ldr		r0, [r1]
	mov		r4, r0		;@ store in r4 for this period
	adds		r2, r4, 1	;@ increment period count
	str		r2, [r1]
	bl		RMS_task0
	lsls		r0, r4, 32 - 1
	beq		RMS_task1_wrap
alarm1ISRx_wrap_end:
	pop		{r0,r1,r2}
	mov		lr, r2
	msr		APSR, r1
	mov		r12, r0
	pop		{r0,r1,r2,r3,r4,pc}

RMS_task1_wrap:
	bl		RMS_task1
	lsls	r0, r4, 32 - 2
	bne		alarm1ISRx_wrap_end

RMS_task2_wrap:
	;@~ ldr	r0, [sp, #28]
	;@~ bl	io_printhn
	bl		RMS_task2
	lsls	r0, r4, 32 - 3
	bne		alarm1ISRx_wrap_end

RMS_task3_wrap:
	bl		RMS_task3
	lsls	r0, r4, 32 - 4
	bne		alarm1ISRx_wrap_end

RMS_task4_wrap:
	bl		RMS_task4
	lsls	r0, r4, 32 - 5
	bne		alarm1ISRx_wrap_end

RMS_task5_wrap:
	bl		RMS_task5
	lsls	r0, r4, 32 - 6
	bne		alarm1ISRx_wrap_end

RMS_task6_wrap:
	bl		RMS_task6
	lsls	r0, r4, 32 - 7
	bne		alarm1ISRx_wrap_end
	
RMS_task7_wrap:
	bl		RMS_task7
	lsls	r0, r4, 32 - 8
	bne		alarm1ISRx_wrap_end

RMS_task8_wrap:
	bl		RMS_task8
	lsls	r0, r4, 32 - 9
	bne		alarm1ISRx_wrap_end

RMS_task9_wrap:
	bl		RMS_task9
	lsls	r0, r4, 32 - 10
	bne		alarm1ISRx_wrap_end
	
RMS_task10_wrap:
	bl		RMS_task10
	lsls	r0, r4, 32 - 11
	bne		alarm1ISRx_wrap_end

RMS_task11_wrap:
	bl		RMS_task11
	b		alarm1ISRx_wrap_end


.balign 4
periodCount:
.word	0

.thumb_func
.global pengumMachineEnter
pengumMachineEnter:
	push	{r4,r5,r6,r7,lr} 	;@ save all c registers
	mov	r12, r0			;@ store function to call
	mov	r7, r1			;@ move save area over
	push	{r7}			;@ push address of save area
	ldm	r7!, {r0,r1,r2,r3,r4,r5,r6} ;@ restore registers
	ldr	r7, [r7]		;@ restore r7
	blx	r12			;@ jump to function
	push	{r7}			;@ save off r7
	ldr	r7, [sp, #4]		;@ load up save area address
	stm	r7!, {r0,r1,r2,r3,r4,r5,r6} ;@ save off r0-r6
	pop	{r0}			;@ pop r7
	str	r0, [r7]		;@ save off r7
	pop	{r0}			;@ pop address of save area
	pop	{r4,r5,r6,r7,pc}	;@ restore c registers, return


.thumb_func
.global copyForward
copyForward: ;@ r0 = src r1 = dst r2 = size
	adds r0, r2
	adds r1, r2
	rsbs r3, r2, 0
	cmp r3, 0
	b 2f
1:
	ldrb r2,[r0, r3] 
	strb r2,[r1, r3] 
	adds r3, 1
2:
	bne 1b
	bx lr

.thumb_func
.global copyBackward
copyBackward: ;@ r0 = src r1 = dst r2 = size
	b 2f
1:
	ldrb r3,[r0, r2] 
	strb r3,[r1, r2]
2:
	subs r2, 1
	bge 1b
	bx lr
