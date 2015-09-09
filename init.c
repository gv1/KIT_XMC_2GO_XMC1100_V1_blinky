/* Original files from : http://eleceng.dit.ie/frank/arm/BareMetalXMC2Go/index.html */

// Initialization and interrupt vectors for the infineon 
// XMC1100 microcontroller.
// No claims are made as to the suitability of this code for any 
// purpose whatsoever.  Use at your own risk!
// Does not include initialization for C++ global/static objects
// so probably best to use this with C projects for now.
// Latest version of this code can be found by visiting
// http://eleceng.dit.ie/frank and follow the links
// Author: Frank Duignan

// void init(void);
void clock_init();
void default_handler(void);
void __attribute__((weak)) Reset_Handler(void);
void __attribute__((weak)) NMI_Handler(void);
void __attribute__((weak)) HardFault_Handler(void);
void __attribute__((weak)) Systick_Handler(void);

// The following are 'declared' in the linker script
extern unsigned char  INIT_DATA_VALUES;
extern unsigned char  INIT_DATA_START;
extern unsigned char  INIT_DATA_END;
extern unsigned char  BSS_START;
extern unsigned char  BSS_END;
// The XMC1100 remaps interrupt vectors to RAM.  The boot loader
// looks for the program entry address at 0x10001004 and the
// initial stack pointer as 0x10001000.  These are defined in
// the "vectors" section below.  The section "vectors" is 
// placed at the beginning of flash by the linker script
// Another useful feature of this chip is that you can 
// pre-program startup considtions such as CPU speed and 
// which periperals.  This is done by writing values in to
// CLK_VAL1 and CLK_VAL2 below

const void * FlashVectors[] __attribute__((section(".vectors"))) ={
	(void *)0x20004000, 	/* Top of stack  @0x10001000 */ 
	Reset_Handler, 		 	  	    /* Reset Handler @0x10001004 */
	(void *)NMI_Handler,				/* @0x10001008 */
	(void *)HardFault_Handler,				/* @0x1000100c */
	(void *)0xffffffff,		/* CLK_VAL1   @0x10001010 */
	(void *)0xffffffff		/* CLK_VAL2   @0x10001014 */
};
// The remaining interrupt vectors are relocated to RAM where a jump
// table should be placed to the actual interrupt handlers.  The jump 
// table takes the following form:
//	ldr R0,=handler_address
//  mov PC,R0
// This pair of instructions fits into 4 bytes which
// fits neatly into a jump table entry.

inline void JumpTable(void) __attribute__(( section(".remapped_vectors")));
inline void JumpTable(void) 
{
	asm(" .long 0 "); // -15 reserved
	// asm(" ldr R0,=Reset_Handler ");
	// asm(" mov PC,R0 ");
	asm(" .long 0 "); // -14 reserved
	// asm(" ldr R0,=NMI_Handler ");
	// asm(" mov PC,R0 ");
	// asm(" .long 0 "); // -13 HardFault
	asm(" ldr R0,=HardFault_Handler ");
	asm(" mov PC,R0 ");

	asm(" .long 0 "); // -12 reserved
	asm(" .long 0 "); // -11 reserved
	asm(" .long 0 "); // -10 reserved
	asm(" .long 0 "); // -9 reserved
	asm(" .long 0 "); // -8 reserved
	asm(" .long 0 "); // -7 reserved
	asm(" .long 0 "); // -6 reserved
	asm(" .long 0 "); // -5 SVCall
	asm(" .long 0 "); // -4 reserved 
	asm(" .long 0 "); // -3 reserved 
	asm(" .long 0 "); // -2 PendSV
	// asm(" .long 0 "); // -1 Systick handler
	asm(" ldr R0,=Systick_Handler ");
	asm(" mov PC,R0 ");
	
	asm(" .long 0 "); // IRQ 0
	asm(" .long 0 "); // IRQ 1
	asm(" .long 0 "); // IRQ 2
	asm(" .long 0 "); // IRQ 3
	asm(" .long 0 "); // IRQ 4
	asm(" .long 0 "); // IRQ 5
	asm(" .long 0 "); // IRQ 6
	asm(" .long 0 "); // IRQ 7
	asm(" .long 0 "); // IRQ 8
	asm(" .long 0 "); // IRQ 9
	asm(" .long 0 "); // IRQ 10
	asm(" .long 0 "); // IRQ 11
	asm(" .long 0 "); // IRQ 12
	asm(" .long 0 "); // IRQ 13
	asm(" .long 0 "); // IRQ 14
	asm(" .long 0 "); // IRQ 15
	asm(" .long 0 "); // IRQ 16
	asm(" .long 0 "); // IRQ 17
	asm(" .long 0 "); // IRQ 18
	asm(" .long 0 "); // IRQ 19
	asm(" .long 0 "); // IRQ 20
	asm(" .long 0 "); // IRQ 21
	asm(" .long 0 "); // IRQ 22
	asm(" .long 0 "); // IRQ 23
	asm(" .long 0 "); // IRQ 24
	asm(" .long 0 "); // IRQ 25
	asm(" .long 0 "); // IRQ 26
	asm(" .long 0 "); // IRQ 27
	asm(" .long 0 "); // IRQ 28
	asm(" .long 0 "); // IRQ 29
	asm(" .long 0 "); // IRQ 30
	asm(" .long 0 "); // IRQ 31
	
};
void Reset_Handler()
{
// do global/static data initialization
// This is will also copy the jump table for remapped IRQ vectors
// to RAM.
	unsigned char *src;
	unsigned char *dest;
	unsigned len;
	src= &INIT_DATA_VALUES;
	dest= &INIT_DATA_START;
	len= &INIT_DATA_END-&INIT_DATA_START;
	while (len--)
		*dest++ = *src++;
// zero out the uninitialized global/static variables
	dest = &BSS_START;
	len = &BSS_END - &BSS_START;
	while (len--)
		*dest++=0;
	
	main();
}

void default_handler()
{
	while(1);
}
void Systick_Handler()
{
	while(1);
}
