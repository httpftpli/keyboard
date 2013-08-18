/**************************************************
 *
 * Part one of the system initialization code, contains low-level
 * initialization, plain thumb variant.
 *
 * Copyright 2012 IAR Systems. All rights reserved.
 *
 * $Revision: 55913 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start

        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        DATA
__vector_table
        DCD     sfe(CSTACK)                 ; Top of Stack
        DCD     __iar_program_start         ; Reset Handler
        DCD     NMI_Handler                 ; NMI Handler
        DCD     HardFault_Handler           ; Hard Fault Handler
        DCD     MemManage_Handler           ; MPU Fault Handler
        DCD     BusFault_Handler            ; Bus Fault Handler
        DCD     UsageFault_Handler          ; Usage Fault Handler
__vector_table_0x1c
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     DebugMon_Handler            ; Debug Monitor Handler
        DCD     0                           ; Reserved
        DCD     PendSV_Handler              ; PendSV Handler
        DCD     SysTick_Handler             ; SysTick Handler
        DCD     WAKE_UP0_IRQHandler         ; WAKE_UP0 Handler
        DCD     WAKE_UP1_IRQHandler         ; WAKE_UP1 Handler
        DCD     WAKE_UP2_IRQHandler         ; WAKE_UP2 Handler
        DCD     WAKE_UP3_IRQHandler         ; WAKE_UP3 Handler
        DCD     WAKE_UP4_IRQHandler         ; WAKE_UP4 Handler
        DCD     WAKE_UP5_IRQHandler         ; WAKE_UP5 Handler
        DCD     WAKE_UP6_IRQHandler         ; WAKE_UP6 Handler
        DCD     WAKE_UP7_IRQHandler         ; WAKE_UP7 Handler
        DCD     WAKE_UP8_IRQHandler         ; WAKE_UP8 Handler
        DCD     WAKE_UP9_IRQHandler         ; WAKE_UP9 Handler
        DCD     WAKE_UP10_IRQHandler        ; WAKE_UP10 Handler
        DCD     WAKE_UP11_IRQHandler        ; WAKE_UP11 Handler
        DCD     WAKE_UP12_IRQHandler        ; WAKE_UP12 Handler
        DCD     0                           ; Reserved
        DCD     SSP1_IRQHandler             ; SSP1 Handler
        DCD     I2C0_IRQHandler             ; I2C0 Handler
        DCD     CT16B0_IRQHandler           ; CT16B0 Handler
        DCD     CT16B1_IRQHandler           ; CT16B1 Handler
        DCD     CT32B0_IRQHandler           ; CT32B0 Handler
        DCD     CT32B1_IRQHandler           ; CT32B1 Handler
        DCD     SSP0_IRQHandler             ; SSP0 Handler
        DCD     UART0_IRQHandler            ; UART0 Handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     ADC_IRQHandler              ; ADC Handler
        DCD     WDT_IRQHandler              ; WDT Handler
        DCD     BOD_IRQHandler              ; BOD Handler
        DCD     0                           ; Reserved
        DCD     PIO3_IRQHandler             ; PIO3 Interupt Handler
        DCD     PIO2_IRQHandler             ; PIO2 Interupt Handler
        DCD     PIO1_IRQHandler             ; PIO1 Interupt Handler
        DCD     PIO0_IRQHandler             ; PIO0 Interupt Handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        SECTION .text:CODE:REORDER:NOROOT(1)

        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK WAKE_UP0_IRQHandler
        PUBWEAK WAKE_UP1_IRQHandler
        PUBWEAK WAKE_UP2_IRQHandler
        PUBWEAK WAKE_UP3_IRQHandler
        PUBWEAK WAKE_UP4_IRQHandler
        PUBWEAK WAKE_UP5_IRQHandler
        PUBWEAK WAKE_UP6_IRQHandler
        PUBWEAK WAKE_UP7_IRQHandler
        PUBWEAK WAKE_UP8_IRQHandler
        PUBWEAK WAKE_UP9_IRQHandler
        PUBWEAK WAKE_UP10_IRQHandler
        PUBWEAK WAKE_UP11_IRQHandler
        PUBWEAK WAKE_UP12_IRQHandler
        PUBWEAK SSP1_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK CT16B0_IRQHandler
        PUBWEAK CT16B1_IRQHandler
        PUBWEAK CT32B0_IRQHandler
        PUBWEAK CT32B1_IRQHandler
        PUBWEAK SSP0_IRQHandler
        PUBWEAK UART0_IRQHandler
        PUBWEAK ADC_IRQHandler
        PUBWEAK WDT_IRQHandler
        PUBWEAK BOD_IRQHandler
        PUBWEAK PIO3_IRQHandler
        PUBWEAK PIO2_IRQHandler
        PUBWEAK PIO1_IRQHandler
        PUBWEAK PIO0_IRQHandler

NMI_Handler:
        B .
HardFault_Handler:
        B .
MemManage_Handler:
        B .
BusFault_Handler:
        B .
UsageFault_Handler:
        B .
SVC_Handler:
        B .
DebugMon_Handler:
        B .
PendSV_Handler:
        B .
SysTick_Handler:
        B .
WAKE_UP0_IRQHandler:
        B .
WAKE_UP1_IRQHandler:
        B .
WAKE_UP2_IRQHandler:
        B .
WAKE_UP3_IRQHandler:
        B .
WAKE_UP4_IRQHandler:
        B .
WAKE_UP5_IRQHandler:
        B .
WAKE_UP6_IRQHandler:
        B .
WAKE_UP7_IRQHandler:
        B .
WAKE_UP8_IRQHandler:
        B .
WAKE_UP9_IRQHandler:
        B .
WAKE_UP10_IRQHandler:
        B .
WAKE_UP11_IRQHandler:
        B .
WAKE_UP12_IRQHandler:
        B .
SSP1_IRQHandler:
        B .
I2C0_IRQHandler:
        B .
CT16B0_IRQHandler:
        B .
CT16B1_IRQHandler:
        B .
CT32B0_IRQHandler:
        B .
CT32B1_IRQHandler:
        B .
SSP0_IRQHandler:
        B .
UART0_IRQHandler:
        B .
ADC_IRQHandler:
        B .
WDT_IRQHandler:
        B .
BOD_IRQHandler:
        B .
PIO3_IRQHandler:
        B .
PIO2_IRQHandler:
        B .
PIO1_IRQHandler:
        B .
PIO0_IRQHandler:
        B .
Default_Handler:
        B .

        SECTION .crp:CODE:ROOT(2)
        DATA
/* Code Read Protection
NO_ISP  0x4E697370 -  Prevents sampling of pin PIO0_1 for entering ISP mode
CRP1    0x12345678 - Write to RAM command cannot access RAM below 0x10000300.
                   - Copy RAM to flash command can not write to Sector 0.
                   - Erase command can erase Sector 0 only when all sectors
                     are selected for erase.
                   - Compare command is disabled.
                   - Read Memory command is disabled.
CRP2    0x87654321 - Read Memory is disabled.
                   - Write to RAM is disabled.
                   - "Go" command is disabled.
                   - Copy RAM to flash is disabled.
                   - Compare is disabled.
CRP3    0x43218765 - Access to chip via the SWD pins is disabled. ISP entry
                     by pulling PIO0_1 LOW is disabled if a valid user code is
                     present in flash sector 0.
Caution: If CRP3 is selected, no future factory testing can be
performed on the device.
*/
        DCD  0xFFFFFFFF

        END
