/*
Copyright 2016 Fred Sundvik <fsundvik@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "keyboard.h"
#include "action_layer.h"
#include "visualizer.h"
#include "host.h"
#include "hook.h"
#include "usb_main.h"
#include "suspend.h"
#include "ch.h"

void hook_early_init(void) {
    visualizer_init();
}

void hook_keyboard_loop(void) {
    visualizer_update(default_layer_state, layer_state, host_keyboard_leds());
}

void hook_usb_suspend_entry(void) {
    visualizer_suspend();
}

void hook_usb_wakeup(void) {
    visualizer_resume();
}

void hook_usb_suspend_loop(void) {
    visualizer_update(default_layer_state, layer_state, host_keyboard_leds());
    /* Do this in the suspended state */
    suspend_power_down(); // on AVR this deep sleeps for 15ms
    /* Remote wakeup */
    if((USB_DRIVER.status & 2) && suspend_wakeup_condition()) {
        send_remote_wakeup(&USB_DRIVER);
    }
}

/* The prototype shows it is a naked function - in effect this is just an
assembly function. */
void HardFault_Handler( void ) __attribute__( ( naked ) );

/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
void HardFault_Handler(void)
{
    __asm volatile
    (
        " MOVS  r0, #4                                              \n"
        " MOV   r1, LR                                              \n"
        " TST   r0, r1                                              \n"
        " BEQ   stacking_used_MSP                                   \n"
        " MRS   r0, psp // first param was using PSP                \n"
        " B     get_LR_and_branch                                   \n"
        " stacking_used_MSP:                                        \n"
        " MRS   r0, MSP // first param was using MSP                \n"
        " get_LR_and_branch:                                        \n"
        " MOV   r1, LR  // second param is LR current value         \n"
        " LDR   r2, handler2_address_const                          \n"
        " BX    r2                                                  \n"
        " handler2_address_const: .word hard_fault_handler_c        \n"
    );
}

// Receiveds stack frame location and LR value extracted from the assembly
// wrapper as input parameters.
void hard_fault_handler_c(unsigned int *hardfault_args, unsigned lr_value)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
       away as the variables never actually get used.  If the debugger won't show the
       values of the variables, make them global my moving their declaration outside
       of this function. */
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr; /* Link register. */
    volatile uint32_t pc; /* Program counter. */
    volatile uint32_t psr;/* Program status register. */

    r0 = hardfault_args[ 0 ];
    r1 = hardfault_args[ 1 ];
    r2 = hardfault_args[ 2 ];
    r3 = hardfault_args[ 3 ];
    r12 = hardfault_args[ 4 ];
    lr = hardfault_args[ 5 ];
    pc = hardfault_args[ 6 ];
    psr = hardfault_args[ 7 ];

    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}
