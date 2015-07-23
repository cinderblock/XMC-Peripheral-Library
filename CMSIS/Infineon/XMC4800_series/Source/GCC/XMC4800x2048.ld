/**
 * @file XMC4800x2048.ld
 * @date 2015-07-13
 *
 * @cond
 *********************************************************************************************************************
 * Linker file for the GNU C Compiler v1.1
 * Supported devices: XMC4800-E196x2048
 *                    XMC4800-F144x2048
 *                    XMC4800-F100x2048
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-07-13:
 *     - Fix warning message 'sh_link not set for section .ARM.exidx' when using GCC ARM 4.9-2015-q2-update
 *     - Fix error '.ARM.exidx has both ordered and unordered sections'
 *     - Removed 'ARM <->THUMB interworking'
 * 
 * 2015-05-22:
 *     - Initial version
 *      
 * @endcond 
 *
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

stack_size = DEFINED(stack_size) ? stack_size : 2048;

MEMORY
{
	FLASH_1_cached(RX) : ORIGIN = 0x08000000, LENGTH = 0x00200000
	FLASH_1_uncached(RX) : ORIGIN = 0x0C000000, LENGTH = 0x00200000
	PSRAM_1(!RX) : ORIGIN = 0x1FFE8000, LENGTH = 0x18000
	DSRAM_1_system(!RX) : ORIGIN = 0x20000000, LENGTH = 0x20000
	DSRAM_2_comm(!RX) : ORIGIN = 0x20020000, LENGTH = 0x20000
	SRAM_combined(!RX) : ORIGIN = 0x1FFEE000, LENGTH = 0x00058000
	EXTMEM_1(!RX) : ORIGIN = 0x60000000, LENGTH = 0x04000000	
	EXTMEM_2(!RX) : ORIGIN = 0x64000000, LENGTH = 0x04000000	
	EXTMEM_3(!RX) : ORIGIN = 0x68000000, LENGTH = 0x04000000	
	EXTMEM_4(!RX) : ORIGIN = 0x6C000000, LENGTH = 0x04000000	
}

SECTIONS
{
	/* TEXT section */

	.text : AT(ORIGIN(FLASH_1_uncached))
	{
		sText = .;
		KEEP(*(.reset));
		*(.text .text.* .gnu.linkonce.t.*);

		/* C++ Support */
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		KEEP(*(.fini))
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;

		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

	} > FLASH_1_cached
    
	/* Exception handling, exidx needs a dedicated section */
	__extab_load = . | 0x04000000;
	.ARM.extab : AT(__extab_load)
    {
		*(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH_1_cached
    
	__exidx_load = . | 0x04000000;
	.ARM.exidx : AT(__exidx_load)
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;
	} > FLASH_1_cached

	/* CONST data section */
	__rodata_load = . | 0x04000000;    
	.rodata : AT(__rodata_load)
	{
		. = ALIGN(4);
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r*)
	} > FLASH_1_cached

    . = ALIGN(16);
		
	/* End of RO-DATA and start of LOAD region for DATA */
	__ram_load = . | 0x04000000;

    /* DSRAM layout (Lowest to highest)*/
	Stack (NOLOAD) : 
	{
		__stack_start = .;
		. = . + stack_size;
		__stack_end = .;
		__initial_sp = .;
	} > SRAM

	/* functions with __attribute__((section(".ram_code"))) */
    .ram_code : AT(__ram_load)
	{
		__ram_code_start = .;
		*(.ram_code)  	
        . = ALIGN(4);
		__ram_code_end = .;
	} > SRAM
	__ram_code_load = LOADADDR (.ram_code);
	__ram_code_size = __ram_code_end - __ram_code_start;

	/* Standard DATA and user defined DATA/BSS/CONST sections */
	.data : AT(LOADADDR(.ram_code)  + SIZEOF(.ram_code))
	{
		__data_start = .;
		* (.data);
		* (.data*);
		*(*.data);
		*(.gnu.linkonce.d*)
        . = ALIGN(4);
        __data_end = .;
	} > SRAM
    __data_load = LOADADDR (.data);
	__data_size = __data_end - __data_start;
        
	/* BSS section */
	.bss (NOLOAD) : 
	{
		__bss_start = .;
		* (.bss);
		* (.bss*);
		* (COMMON);
		*(.gnu.linkonce.b*)
        . = ALIGN(4);
        __bss_end = .;
	} > SRAM
	__bss_size = __bss_end - __bss_start;

    /* Shift location counter, so that ETH_RAM and USB_RAM are located above DSRAM_1_system */    
    __shift_loc =  (__bss_end >= DSRAM_1_system) ? 0 : (DSRAM_1_system - __bss_end);

    USB_RAM  (__bss_end + __shift_loc) (NOLOAD) :
    {
        USB_RAM_start = .;
        *(USB_RAM)
        . = ALIGN(4);
        USB_RAM_end = .;
    } > SRAM_combined
	USB_RAM_size = USB_RAM_end - USB_RAM_start;

    ETH_RAM (USB_RAM_end) (NOLOAD) :
    {
        ETH_RAM_start = .;
        *(ETH_RAM)
        . = ALIGN(4);
        ETH_RAM_end = .;
        . = ALIGN(8);
        Heap_Bank1_Start = .;
    } > SRAM_combined
	ETH_RAM_size = ETH_RAM_end - ETH_RAM_start;

    /* .no_init section */
    .no_init ORIGIN(SRAM_combined) + LENGTH(SRAM_combined) - 0x40 (NOLOAD) : 
    {
        Heap_Bank1_End = .;
        * (.no_init);
    } > SRAM_combined

    .extmem_1 (NOLOAD) : 
    {
        * (.extmem_1);
    } > EXTMEM_1

    .extmem_2 (NOLOAD) : 
    {
        * (.extmem_2);
    } > EXTMEM_2

    .extmem_3 (NOLOAD) : 
    {
        * (.extmem_3);
    } > EXTMEM_3

    .extmem_4 (NOLOAD) : 
    {
        * (.extmem_4);
    } > EXTMEM_4
	
	/* Heap - Bank1*/
	Heap_Bank1_Size  = Heap_Bank1_End - Heap_Bank1_Start;

	/DISCARD/ :
	{
		*(.comment)
	}

	.stab		0 (NOLOAD) : { *(.stab) }
	.stabstr	0 (NOLOAD) : { *(.stabstr) }

	/* DWARF 1 */
	.debug				0 : { *(.debug) }
	.line				0 : { *(.line) }

	/* GNU DWARF 1 extensions */
	.debug_srcinfo		0 : { *(.debug_srcinfo) }
	.debug_sfnames		0 : { *(.debug_sfnames) }

	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges		0 : { *(.debug_aranges) }
	.debug_pubnames		0 : { *(.debug_pubnames) }
	.debug_pubtypes		0 : { *(.debug_pubtypes) }

	/* DWARF 2 */
	.debug_info			0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev		0 : { *(.debug_abbrev) }
	.debug_line			0 : { *(.debug_line) }
	.debug_frame		0 : { *(.debug_frame) }
	.debug_str			0 : { *(.debug_str) }
	.debug_loc			0 : { *(.debug_loc) }
	.debug_macinfo		0 : { *(.debug_macinfo) }

	/* DWARF 2.1 */
	.debug_ranges		0 : { *(.debug_ranges) }

	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames	0 : { *(.debug_weaknames) }
	.debug_funcnames	0 : { *(.debug_funcnames) }
	.debug_typenames	0 : { *(.debug_typenames) }
	.debug_varnames		0 : { *(.debug_varnames) }

	/* Build attributes */
	.build_attributes	0 : { *(.ARM.attributes) }
}
