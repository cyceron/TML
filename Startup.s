#/***********************************************************************/
#/*  This file is part of the uVision/ARM development tools             */
#/*  Copyright KEIL ELEKTRONIK GmbH 2002-2004                           */
#/***********************************************************************/
#/*                                                                     */
#/*  STARTUP.S:  Startup file                                           */
#/*                                                                     */
#/***********************************************************************/


/* 
//*** <<< Use Configuration Wizard in Context Menu >>> *** 
*/


# *** Startup Code (executed after Reset) ***


# Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

        .equ    Mode_USR,       0x10
        .equ    Mode_FIQ,       0x11
        .equ    Mode_IRQ,       0x12
        .equ    Mode_SVC,       0x13
        .equ    Mode_ABT,       0x17
        .equ    Mode_UND,       0x1B
        .equ    Mode_SYS,       0x1F

        .equ    I_Bit,          0x80    /* when I bit is set, IRQ is disabled */
        .equ    F_Bit,          0x40    /* when F bit is set, FIQ is disabled */


/*
// <h> Stack Configuration
//   <o>  Top of Stack Address  <0x0-0xFFFFFFFF:4>
//   <h>  Stack Sizes (in Bytes)
//     <o1> Undefined Mode      <0x0-0xFFFFFFFF:4>
//     <o2> Supervisor Mode     <0x0-0xFFFFFFFF:4>
//     <o3> Abort Mode          <0x0-0xFFFFFFFF:4>
//     <o4> Fast Interrupt Mode <0x0-0xFFFFFFFF:4>
//     <o5> Interrupt Mode      <0x0-0xFFFFFFFF:4>
//     <o6> User/System Mode    <0x0-0xFFFFFFFF:4>
//   </h>
// </h>
*/
        .equ    Top_Stack,      0x40004000
        .equ    UND_Stack_Size, 0x00000004
        .equ    SVC_Stack_Size, 0x00000004
        .equ    ABT_Stack_Size, 0x00000004
        .equ    FIQ_Stack_Size, 0x00000004
        .equ    IRQ_Stack_Size, 0x00000080
        .equ    USR_Stack_Size, 0x00000400


# VPBDIV definitions
        .equ    VPBDIV,         0xE01FC100  /* VPBDIV Address */

/*
// <e> VPBDIV Setup
// <i> Peripheral Bus Clock Rate
//   <o1.0..1>   VPBDIV: VPB Clock
//               <0=> VPB Clock = CPU Clock / 4
//               <1=> VPB Clock = CPU Clock
//               <2=> VPB Clock = CPU Clock / 2
//   <o1.4..5>   XCLKDIV: XCLK Pin
//               <0=> XCLK Pin = CPU Clock / 4
//               <1=> XCLK Pin = CPU Clock
//               <2=> XCLK Pin = CPU Clock / 2
// </e>
*/
        .equ    VPBDIV_SETUP,   0
        .equ    VPBDIV_Val,     0x00000000


# Phase Locked Loop (PLL) definitions
        .equ    PLL_BASE,       0xE01FC080  /* PLL Base Address */
        .equ    PLLCON_OFS,     0x00        /* PLL Control Offset*/
        .equ    PLLCFG_OFS,     0x04        /* PLL Configuration Offset */
        .equ    PLLSTAT_OFS,    0x08        /* PLL Status Offset */
        .equ    PLLFEED_OFS,    0x0C        /* PLL Feed Offset */
        .equ    PLLCON_PLLE,    (1<<0)      /* PLL Enable */
        .equ    PLLCON_PLLC,    (1<<1)      /* PLL Connect */
        .equ    PLLCFG_MSEL,    (0x04<<0)   /* PLL Multiplier */
        .equ    PLLCFG_PSEL,    (0x02<<5)   /* PLL Divider */
        .equ    PLLSTAT_PLOCK,  (1<<10)     /* PLL Lock Status */

/*
// <e> PLL Setup
//   <o1.0..4>   MSEL: PLL Multiplier Selection
//               <1-32><#-1>
//               <i> M Value
//   <o1.5..6>   PSEL: PLL Divider Selection
//               <0=> 1   <1=> 2   <2=> 4   <3=> 8
//               <i> P Value
// </e>
*/
        .equ    PLL_SETUP,      1
        .equ    PLLCFG_Val,     0x00000024


# Memory Accelerator Module (MAM) definitions
        .equ    MAM_BASE,       0xE01FC000  /* MAM Base Address */
        .equ    MAMCR_OFS,      0x00        /* MAM Control Offset*/
        .equ    MAMTIM_OFS,     0x04        /* MAM Timing Offset */

/*
// <e> MAM Setup
//   <o1.0..1>   MAM Control
//               <0=> Disabled
//               <1=> Partially Enabled
//               <2=> Fully Enabled
//               <i> Mode
//   <o2.0..2>   MAM Timing
//               <0=> Reserved  <1=> 1   <2=> 2   <3=> 3
//               <4=> 4         <5=> 5   <6=> 6   <7=> 7
//               <i> Fetch Cycles
// </e>
*/
        .equ    MAM_SETUP,      1
        .equ    MAMCR_Val,      0x00000002
        .equ    MAMTIM_Val,     0x00000004


# External Memory Controller (EMC) definitions
        .equ    EMC_BASE,       0xFFE00000  /* EMC Base Address */
        .equ    BCFG0_OFS,      0x00        /* BCFG0 Offset */
        .equ    BCFG1_OFS,      0x04        /* BCFG1 Offset */
        .equ    BCFG2_OFS,      0x08        /* BCFG2 Offset */
        .equ    BCFG3_OFS,      0x0C        /* BCFG3 Offset */

/*
// <e> External Memory Controller (EMC)
*/
        .equ    EMC_SETUP,      0

/*
//   <e> Bank Configuration 0 (BCFG0)
//     <o1.0..3>   IDCY: Idle Cycles <0-15>
//     <o1.5..9>   WST1: Wait States 1 <0-31>
//     <o1.11..15> WST2: Wait States 2 <0-31>
//     <o1.10>     RBLE: Read Byte Lane Enable
//     <o1.26>     WP: Write Protect
//     <o1.27>     BM: Burst ROM
//     <o1.28..29> MW: Memory Width  <0=>  8-bit  <1=> 16-bit
//                                   <2=> 32-bit  <3=> Reserved
//   </e>
*/
        .equ    BCFG0_SETUP,    0
        .equ    BCFG0_Val,      0x0000FBEF

/*
//   <e> Bank Configuration 1 (BCFG1)
//     <o1.0..3>   IDCY: Idle Cycles <0-15>
//     <o1.5..9>   WST1: Wait States 1 <0-31>
//     <o1.11..15> WST2: Wait States 2 <0-31>
//     <o1.10>     RBLE: Read Byte Lane Enable
//     <o1.26>     WP: Write Protect
//     <o1.27>     BM: Burst ROM
//     <o1.28..29> MW: Memory Width  <0=>  8-bit  <1=> 16-bit
//                                   <2=> 32-bit  <3=> Reserved
//   </e>
*/
        .equ    BCFG1_SETUP,    0
        .equ    BCFG1_Val,      0x0000FBEF

/*
//   <e> Bank Configuration 2 (BCFG2)
//     <o1.0..3>   IDCY: Idle Cycles <0-15>
//     <o1.5..9>   WST1: Wait States 1 <0-31>
//     <o1.11..15> WST2: Wait States 2 <0-31>
//     <o1.10>     RBLE: Read Byte Lane Enable
//     <o1.26>     WP: Write Protect
//     <o1.27>     BM: Burst ROM
//     <o1.28..29> MW: Memory Width  <0=>  8-bit  <1=> 16-bit
//                                   <2=> 32-bit  <3=> Reserved
//   </e>
*/
        .equ    BCFG2_SETUP,    0
        .equ    BCFG2_Val,      0x0000FBEF

/*
//   <e> Bank Configuration 3 (BCFG3)
//     <o1.0..3>   IDCY: Idle Cycles <0-15>
//     <o1.5..9>   WST1: Wait States 1 <0-31>
//     <o1.11..15> WST2: Wait States 2 <0-31>
//     <o1.10>     RBLE: Read Byte Lane Enable
//     <o1.26>     WP: Write Protect
//     <o1.27>     BM: Burst ROM
//     <o1.28..29> MW: Memory Width  <0=>  8-bit  <1=> 16-bit
//                                   <2=> 32-bit  <3=> Reserved
//   </e>
*/
        .equ    BCFG3_SETUP,    0
        .equ    BCFG3_Val,      0x0000FBEF

/*
// </e> End of EMC
*/


# External Memory Pins definitions
        .equ    PINSEL2,        0xE002C014  /* PINSEL2 Address */
        .equ    PINSEL2_Val,    0x0E6149E4  /* CS0..3, OE, WE, BLS0..3, 
                                               D0..31, A2..23, JTAG Pins */


# Starupt Code must be linked first at Address at which it expects to run.

        .text
        .arm

        .global _startup
        .func   _startup
_startup:


# Exception Vectors
#  Mapped to Address 0.
#  Absolute addressing mode must be used.
#  Dummy Handlers are implemented as infinite loops which can be modified.

Vectors:        LDR     PC, Reset_Addr         
                LDR     PC, Undef_Addr
                LDR     PC, SWI_Addr
                LDR     PC, PAbt_Addr
                LDR     PC, DAbt_Addr
                NOP                            /* Reserved Vector */
                /*LDR     PC, IRQ_Addr*/
                LDR     PC, [PC, #-0x0FF0]     /* Vector from VicVectAddr */
                LDR     PC, FIQ_Addr

Reset_Addr:     .word   Reset_Handler
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
                .word   0                      /* Reserved Address */
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler

Undef_Handler:  B       Undef_Handler
SWI_Handler:    B       SWI_Handler
PAbt_Handler:   B       PAbt_Handler
DAbt_Handler:   B       DAbt_Handler
IRQ_Handler:    B       IRQ_Handler
FIQ_Handler:    B       FIQ_Handler


# Reset Handler

Reset_Handler:  


.ifdef EXTERNAL_MODE
                LDR     R0, =PINSEL2
                LDR     R1, =PINSEL2_Val
                STR     R1, [R0]
.endif


.if EMC_SETUP
                LDR     R0, =EMC_BASE

.if BCFG0_SETUP
                LDR     R1, =BCFG0_Val
                STR     R1, [R0, #BCFG0_OFS]
.endif

.if BCFG1_SETUP
                LDR     R1, =BCFG1_Val
                STR     R1, [R0, #BCFG1_OFS]
.endif

.if BCFG2_SETUP
                LDR     R1, =BCFG2_Val
                STR     R1, [R0, #BCFG2_OFS]
.endif

.if BCFG3_SETUP
                LDR     R1, =BCFG3_Val
                STR     R1, [R0, #BCFG3_OFS]
.endif

.endif


.if VPBDIV_SETUP
                LDR     R0, =VPBDIV
                LDR     R1, =VPBDIV_Val
                STR     R1, [R0]
.endif


.if PLL_SETUP
                LDR     R0, =PLL_BASE
                MOV     R1, #0xAA
                MOV     R2, #0x55

# Configure and Enable PLL
                MOV     R3, #PLLCFG_Val
                STR     R3, [R0, #PLLCFG_OFS] 
                MOV     R3, #PLLCON_PLLE
                STR     R3, [R0, #PLLCON_OFS]
                STR     R1, [R0, #PLLFEED_OFS]
                STR     R2, [R0, #PLLFEED_OFS]

# Wait until PLL Locked
PLL_Loop:       LDR     R3, [R0, #PLLSTAT_OFS]
                ANDS    R3, R3, #PLLSTAT_PLOCK
                BEQ     PLL_Loop

# Switch to PLL Clock
                MOV     R3, #(PLLCON_PLLE | PLLCON_PLLC)
                STR     R3, [R0, #PLLCON_OFS]
                STR     R1, [R0, #PLLFEED_OFS]
                STR     R2, [R0, #PLLFEED_OFS]
.endif


.if MAM_SETUP
                LDR     R0, =MAM_BASE
                MOV     R1, #MAMTIM_Val
                STR     R1, [R0, #MAMTIM_OFS] 
                MOV     R1, #MAMCR_Val
                STR     R1, [R0, #MAMCR_OFS] 
.endif


# Memory Mapping (when Interrupt Vectors are in RAM)
        .equ    MEMMAP, 0xE01FC040  /* Memory Mapping Control */
        
.ifdef RAM_INTVEC
                LDR     R0, =MEMMAP
                MOV     R1, #2
                STR     R1, [R0]
.endif


# Initialise Interrupt System
#  ...


# Setup Stack for each mode

                LDR     R0, =Top_Stack

#  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

#  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

#  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

#  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

#  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

#  Enter User Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_USR
                MOV     SP, R0

#  Setup a default Stack Limit (when compiled with "-mapcs-stack-check")
                SUB     SL, SP, #USR_Stack_Size


# Relocate .data section (Copy from ROM to RAM)
                LDR     R1, =_etext
                LDR     R2, =_data
                LDR     R3, =_edata
LoopRel:        CMP     R2, R3
                LDRLO   R0, [R1], #4
                STRLO   R0, [R2], #4
                BLO     LoopRel


# Clear .bss section (Zero init)
                MOV     R0, #0
                LDR     R1, =__bss_start__
                LDR     R2, =__bss_end__
LoopZI:         CMP     R1, R2
                STRLO   R0, [R1], #4
                BLO     LoopZI


# Enter the C code
           
                ADR     LR, __main_exit
                LDR     R0, =main
                BX      R0

__main_exit:    B       __main_exit


        .size   _startup, . - _startup
        .endfunc


        .end
