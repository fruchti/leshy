#include <stdint.h>
#include "stm32f030x6.h"
 
#define UNUSED(x) (void)(x)

void HardFault_HandlerC(unsigned long *hardfault_args);

void SystemInit(void)
{
	// Activate HSI and wait for it to be ready
	RCC->CR = RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

	// Set PLL to x12 (-> 48MHz system clock)
	RCC->CFGR = RCC_CFGR_PLLMUL_3 | RCC_CFGR_PLLMUL_1;

	// Activate PLL and wait
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// Select PLL as clock source
	RCC->CFGR = RCC_CFGR_PLLMUL_3 | RCC_CFGR_PLLMUL_1 | RCC_CFGR_SW_1;

	// Disable all interrupts
	RCC->CIR = 0x00000000;
}

/*
   Unlike Cortex-M3, on Cortex-M0 it is not possible to access the debug
   registers to find out if a debug probe (like LPC-link) is attached. Only
   debug tools can access these registers.

   At the "bottom layer" of the C library, semihosting is implemented by a
   "BKPT 0xAB" instruction. When the debug tools are not connected, this
   instruction triggers a hard fault - and the default hard fault handler
   just contains an infinite loop. This is why your application appears to
   hang.

   But what we can do instead, is provide a hard fault handler which
   instead looks to see what the instruction that caused the hard fault was
   - and if it was a "BKPT 0xAB", then it instead returns back to the user
   application.

   --- CodeRed support June 2011.

*/
// Use the 'naked' attribute so that C stacking is not used.
__attribute__((naked)) void HardFault_Handler(void)
{
	/*
	 * Get the stack pointer that was in use (MSP or PSP), depending on
	 * current operating mode, and pass it as the parameter to the C handler.
	 * This function will never return EXCEPT if it was an unhandled
	 * debugger breakpoint (which happens if no debug probe is attached).
	 */
	__asm(".syntax unified\n"
			// Check which stack is in use
			"MOVS   R0, #4  \n"
			"MOV    R1, LR  \n"
			"TST    R0, R1  \n"
			"BEQ    _MSP    \n"
			"MRS    R0, PSP \n"
			"B      _checkFaultInstruction \n"
			"_MSP:  \n"
			"MRS    R0, MSP \n"
			"_checkFaultInstruction: \n"
			// Load the instruction that triggered hard fault
			"LDR    R1,[R0,#24] \n" // R1 = address of instruction that caused fault
			"LDRH   R2,[r1]     \n"
			// Semihosting instruction is "BKPT 0xAB" (0xBEAB)
			"LDR    R3,=0xBEAB  \n"
			"CMP    R2,R3       \n"
			// If it is NOT a BKT 0xAB, jump to the terminal hardfault handler
			"BNE    HardFault_HandlerC  \n"
			// Was semihosting instruction, so adjust location to
			// return to by 1 instruction (2 bytes), then exit function
			"ADDS   R1,#2       \n"
			"STR    R1,[R0,#24] \n"
			"BX     LR          \n"
			".syntax divided\n");
}

/**
 * HardFaultHandler_C:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
void HardFault_HandlerC(unsigned long *hardfault_args)
{
	volatile unsigned long stacked_r0;
	volatile unsigned long stacked_r1;
	volatile unsigned long stacked_r2;
	volatile unsigned long stacked_r3;
	volatile unsigned long stacked_r12;
	volatile unsigned long stacked_lr;
	volatile unsigned long stacked_pc;
	volatile unsigned long stacked_psr;
	volatile unsigned long _CFSR;
	volatile unsigned long _HFSR;
	volatile unsigned long _DFSR;
	volatile unsigned long _AFSR;
	volatile unsigned long _BFAR;
	volatile unsigned long _MMAR;

	stacked_r0 = ((unsigned long)hardfault_args[0]);
	stacked_r1 = ((unsigned long)hardfault_args[1]);
	stacked_r2 = ((unsigned long)hardfault_args[2]);
	stacked_r3 = ((unsigned long)hardfault_args[3]);
	stacked_r12 = ((unsigned long)hardfault_args[4]);
	stacked_lr = ((unsigned long)hardfault_args[5]);
	stacked_pc = ((unsigned long)hardfault_args[6]);
	stacked_psr = ((unsigned long)hardfault_args[7]);

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;   

	// Hard Fault Status Register
	_HFSR = (*((volatile unsigned long *)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile unsigned long *)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile unsigned long *)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile unsigned long *)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile unsigned long *)(0xE000ED38)));

	__asm("BKPT #0\n"); // Break into the debugger

    UNUSED(stacked_r0);
    UNUSED(stacked_r1);
    UNUSED(stacked_r2);
    UNUSED(stacked_r3);
    UNUSED(stacked_r12);
    UNUSED(stacked_lr);
    UNUSED(stacked_pc);
    UNUSED(stacked_psr);
    UNUSED(_CFSR);
    UNUSED(_HFSR);
    UNUSED(_DFSR);
    UNUSED(_AFSR);
    UNUSED(_BFAR);
    UNUSED(_MMAR);
}
