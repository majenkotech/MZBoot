/* Default linker script, for normal executables */
OUTPUT_FORMAT("elf32-tradlittlemips")
OUTPUT_ARCH(pic32mx)
ENTRY(_reset)
/*
 * Provide for a minimum stack and heap size
 * - _min_stack_size - represents the minimum space that must be made
 *                     available for the stack.  Can be overridden from
 *                     the command line using the linker's --defsym option.
 * - _min_heap_size  - represents the minimum space that must be made
 *                     available for the heap.  Can be overridden from
 *                     the command line using the linker's --defsym option.
 */
EXTERN (_min_stack_size _min_heap_size)
PROVIDE(_min_stack_size = 0x400) ;
PROVIDE(_min_heap_size = 0x800) ;
/*************************************************************************
 * Processor-specific object file.  Contains SFR definitions.
 *************************************************************************/
INPUT("processor.o")

/*************************************************************************
 * For interrupt vector handling
 *************************************************************************/
PROVIDE(_vector_spacing = 0x00000001);

/*************************************************************************
 * Memory Regions
 *
 * Memory regions without attributes cannot be used for orphaned sections.
 * Only sections specifically assigned to these regions can be allocated
 * into these regions.
 *************************************************************************/
MEMORY
{
    kseg0_boot_mem          : ORIGIN = 0x9fc00000, LENGTH = 0x00000000
    kseg1_boot_mem          : ORIGIN = 0xbfc00000, LENGTH = 0x00000040

    kseg0_vector_mem        : ORIGIN = 0x9fc00040, LENGTH = 0xfc0
    kseg1_vector_mem        : ORIGIN = 0xbfc00040, LENGTH = 0xfc0

    kseg0_program_mem  (rx) : ORIGIN = 0xbfc01000, LENGTH = 0x0000f000
    kseg1_program_mem  (rx) : ORIGIN = 0xbfc01000, LENGTH = 0x0

    kseg1_debug_mem         : ORIGIN = 0xbfc00300, LENGTH = 0x000001B0

    kseg1_eeprom_mem        : ORIGIN = 0xBD1FC000, LENGTH = 0x4000

    configsfrs              : ORIGIN = 0xBFC0FFC0, LENGTH = 0x10
    config_BFC0FF40         : ORIGIN = 0xbfc0ff40, LENGTH = 0x00000004
    config_BFC0FF44         : ORIGIN = 0xbfc0ff44, LENGTH = 0x00000004
    config_BFC0FF48         : ORIGIN = 0xbfc0ff48, LENGTH = 0x00000004
    config_BFC0FF4C         : ORIGIN = 0xbfc0ff4c, LENGTH = 0x00000004
    config_BFC0FF50         : ORIGIN = 0xbfc0ff50, LENGTH = 0x00000004
    config_BFC0FF54         : ORIGIN = 0xbfc0ff54, LENGTH = 0x00000004
    config_BFC0FF58         : ORIGIN = 0xbfc0ff58, LENGTH = 0x00000004
    config_BFC0FF5C         : ORIGIN = 0xbfc0ff5c, LENGTH = 0x00000004
    config_BFC0FF60         : ORIGIN = 0xbfc0ff60, LENGTH = 0x00000004
    config_BFC0FF64         : ORIGIN = 0xbfc0ff64, LENGTH = 0x00000004
    config_BFC0FF68         : ORIGIN = 0xbfc0ff68, LENGTH = 0x00000004
    config_BFC0FF6C         : ORIGIN = 0xbfc0ff6c, LENGTH = 0x00000004
    config_BFC0FF70         : ORIGIN = 0xbfc0ff70, LENGTH = 0x00000004
    config_BFC0FF74         : ORIGIN = 0xbfc0ff74, LENGTH = 0x00000004
    config_BFC0FF78         : ORIGIN = 0xbfc0ff78, LENGTH = 0x00000004
    config_BFC0FF7C         : ORIGIN = 0xbfc0ff7c, LENGTH = 0x00000004
    config_BFC0FFC0         : ORIGIN = 0xbfc0ffc0, LENGTH = 0x00000004
    config_BFC0FFC4         : ORIGIN = 0xbfc0ffc4, LENGTH = 0x00000004
    config_BFC0FFC8         : ORIGIN = 0xbfc0ffc8, LENGTH = 0x00000004
    config_BFC0FFCC         : ORIGIN = 0xbfc0ffcc, LENGTH = 0x00000004
    config_BFC0FFD0         : ORIGIN = 0xbfc0ffd0, LENGTH = 0x00000004
    config_BFC0FFD4         : ORIGIN = 0xbfc0ffd4, LENGTH = 0x00000004
    config_BFC0FFD8         : ORIGIN = 0xbfc0ffd8, LENGTH = 0x00000004
    config_BFC0FFDC         : ORIGIN = 0xbfc0ffdc, LENGTH = 0x00000004
    config_BFC0FFE0         : ORIGIN = 0xbfc0ffe0, LENGTH = 0x00000004
    config_BFC0FFE4         : ORIGIN = 0xbfc0ffe4, LENGTH = 0x00000004
    config_BFC0FFE8         : ORIGIN = 0xbfc0ffe8, LENGTH = 0x00000004
    config_BFC0FFEC         : ORIGIN = 0xbfc0ffec, LENGTH = 0x00000004
    config_BFC0FFF0         : ORIGIN = 0xbfc0fff0, LENGTH = 0x00000004
    config_BFC0FFF4         : ORIGIN = 0xbfc0fff4, LENGTH = 0x00000004
    config_BFC0FFF8         : ORIGIN = 0xbfc0fff8, LENGTH = 0x00000004
    config_BFC0FFFC         : ORIGIN = 0xbfc0fffc, LENGTH = 0x00000004
    lowerbootaliaslastpage  : ORIGIN = 0xbfc10000, LENGTH = 0x00004000
    upperbootalias          : ORIGIN = 0xbfc20000, LENGTH = 0x00014000
    boot1                   : ORIGIN = 0xbfc40000, LENGTH = 0x0000ff00
    config_BFC4FF40         : ORIGIN = 0xbfc4ff40, LENGTH = 0x00000004
    config_BFC4FF44         : ORIGIN = 0xbfc4ff44, LENGTH = 0x00000004
    config_BFC4FF48         : ORIGIN = 0xbfc4ff48, LENGTH = 0x00000004
    config_BFC4FF4C         : ORIGIN = 0xbfc4ff4c, LENGTH = 0x00000004
    config_BFC4FF50         : ORIGIN = 0xbfc4ff50, LENGTH = 0x00000004
    config_BFC4FF54         : ORIGIN = 0xbfc4ff54, LENGTH = 0x00000004
    config_BFC4FF58         : ORIGIN = 0xbfc4ff58, LENGTH = 0x00000004
    config_BFC4FF5C         : ORIGIN = 0xbfc4ff5c, LENGTH = 0x00000004
    config_BFC4FF60         : ORIGIN = 0xbfc4ff60, LENGTH = 0x00000004
    config_BFC4FF64         : ORIGIN = 0xbfc4ff64, LENGTH = 0x00000004
    config_BFC4FF68         : ORIGIN = 0xbfc4ff68, LENGTH = 0x00000004
    config_BFC4FF6C         : ORIGIN = 0xbfc4ff6c, LENGTH = 0x00000004
    config_BFC4FF70         : ORIGIN = 0xbfc4ff70, LENGTH = 0x00000004
    config_BFC4FF74         : ORIGIN = 0xbfc4ff74, LENGTH = 0x00000004
    config_BFC4FF78         : ORIGIN = 0xbfc4ff78, LENGTH = 0x00000004
    config_BFC4FF7C         : ORIGIN = 0xbfc4ff7c, LENGTH = 0x00000004
    config_BFC4FFC0         : ORIGIN = 0xbfc4ffc0, LENGTH = 0x00000004
    config_BFC4FFC4         : ORIGIN = 0xbfc4ffc4, LENGTH = 0x00000004
    config_BFC4FFC8         : ORIGIN = 0xbfc4ffc8, LENGTH = 0x00000004
    config_BFC4FFCC         : ORIGIN = 0xbfc4ffcc, LENGTH = 0x00000004
    config_BFC4FFD0         : ORIGIN = 0xbfc4ffd0, LENGTH = 0x00000004
    config_BFC4FFD4         : ORIGIN = 0xbfc4ffd4, LENGTH = 0x00000004
    config_BFC4FFD8         : ORIGIN = 0xbfc4ffd8, LENGTH = 0x00000004
    config_BFC4FFDC         : ORIGIN = 0xbfc4ffdc, LENGTH = 0x00000004
    config_BFC4FFE0         : ORIGIN = 0xbfc4ffe0, LENGTH = 0x00000004
    config_BFC4FFE4         : ORIGIN = 0xbfc4ffe4, LENGTH = 0x00000004
    config_BFC4FFE8         : ORIGIN = 0xbfc4ffe8, LENGTH = 0x00000004
    config_BFC4FFEC         : ORIGIN = 0xbfc4ffec, LENGTH = 0x00000004
    config_BFC4FFF0         : ORIGIN = 0xbfc4fff0, LENGTH = 0x00000004
    config_BFC4FFF4         : ORIGIN = 0xbfc4fff4, LENGTH = 0x00000004
    config_BFC4FFF8         : ORIGIN = 0xbfc4fff8, LENGTH = 0x00000004
    config_BFC4FFFC         : ORIGIN = 0xbfc4fffc, LENGTH = 0x00000004
    boot1lastpage           : ORIGIN = 0xbfc50000, LENGTH = 0x00004000
    config_BFC54000         : ORIGIN = 0xbfc54000, LENGTH = 0x00000004
    config_BFC54004         : ORIGIN = 0xbfc54004, LENGTH = 0x00000004
    config_BFC54008         : ORIGIN = 0xbfc54008, LENGTH = 0x00000004
    config_BFC5400C         : ORIGIN = 0xbfc5400c, LENGTH = 0x00000004
    config_BFC54010         : ORIGIN = 0xbfc54010, LENGTH = 0x00000004
    config_BFC54020         : ORIGIN = 0xbfc54020, LENGTH = 0x00000004
    config_BFC54024         : ORIGIN = 0xbfc54024, LENGTH = 0x00000004
    boot2                   : ORIGIN = 0xbfc60000, LENGTH = 0x0000ff00
    config_BFC6FF40         : ORIGIN = 0xbfc6ff40, LENGTH = 0x00000004
    config_BFC6FF44         : ORIGIN = 0xbfc6ff44, LENGTH = 0x00000004
    config_BFC6FF48         : ORIGIN = 0xbfc6ff48, LENGTH = 0x00000004
    config_BFC6FF4C         : ORIGIN = 0xbfc6ff4c, LENGTH = 0x00000004
    config_BFC6FF50         : ORIGIN = 0xbfc6ff50, LENGTH = 0x00000004
    config_BFC6FF54         : ORIGIN = 0xbfc6ff54, LENGTH = 0x00000004
    config_BFC6FF58         : ORIGIN = 0xbfc6ff58, LENGTH = 0x00000004
    config_BFC6FF5C         : ORIGIN = 0xbfc6ff5c, LENGTH = 0x00000004
    config_BFC6FF60         : ORIGIN = 0xbfc6ff60, LENGTH = 0x00000004
    config_BFC6FF64         : ORIGIN = 0xbfc6ff64, LENGTH = 0x00000004
    config_BFC6FF68         : ORIGIN = 0xbfc6ff68, LENGTH = 0x00000004
    config_BFC6FF6C         : ORIGIN = 0xbfc6ff6c, LENGTH = 0x00000004
    config_BFC6FF70         : ORIGIN = 0xbfc6ff70, LENGTH = 0x00000004
    config_BFC6FF74         : ORIGIN = 0xbfc6ff74, LENGTH = 0x00000004
    config_BFC6FF78         : ORIGIN = 0xbfc6ff78, LENGTH = 0x00000004
    config_BFC6FF7C         : ORIGIN = 0xbfc6ff7c, LENGTH = 0x00000004
    config_BFC6FFC0         : ORIGIN = 0xbfc6ffc0, LENGTH = 0x00000004
    config_BFC6FFC4         : ORIGIN = 0xbfc6ffc4, LENGTH = 0x00000004
    config_BFC6FFC8         : ORIGIN = 0xbfc6ffc8, LENGTH = 0x00000004
    config_BFC6FFCC         : ORIGIN = 0xbfc6ffcc, LENGTH = 0x00000004
    config_BFC6FFD0         : ORIGIN = 0xbfc6ffd0, LENGTH = 0x00000004
    config_BFC6FFD4         : ORIGIN = 0xbfc6ffd4, LENGTH = 0x00000004
    config_BFC6FFD8         : ORIGIN = 0xbfc6ffd8, LENGTH = 0x00000004
    config_BFC6FFDC         : ORIGIN = 0xbfc6ffdc, LENGTH = 0x00000004
    config_BFC6FFE0         : ORIGIN = 0xbfc6ffe0, LENGTH = 0x00000004
    config_BFC6FFE4         : ORIGIN = 0xbfc6ffe4, LENGTH = 0x00000004
    config_BFC6FFE8         : ORIGIN = 0xbfc6ffe8, LENGTH = 0x00000004
    config_BFC6FFEC         : ORIGIN = 0xbfc6ffec, LENGTH = 0x00000004
    config_BFC6FFF0         : ORIGIN = 0xbfc6fff0, LENGTH = 0x00000004
    config_BFC6FFF4         : ORIGIN = 0xbfc6fff4, LENGTH = 0x00000004
    config_BFC6FFF8         : ORIGIN = 0xbfc6fff8, LENGTH = 0x00000004
    config_BFC6FFFC         : ORIGIN = 0xbfc6fffc, LENGTH = 0x00000004
    boot2lastpage           : ORIGIN = 0xbfc70000, LENGTH = 0x00004000
    kseg0_data_mem  (w!x)   : ORIGIN = 0x80000000, LENGTH = 0x00080000
    sfrs                    : ORIGIN = 0xbf800000, LENGTH = 0x00100000
    configsfrs_BFC0FF40     : ORIGIN = 0xbfc0ff40, LENGTH = 0x00000040
    configsfrs_BFC0FFC0     : ORIGIN = 0xbfc0ffc0, LENGTH = 0x00000040
    configsfrs_BFC4FF40     : ORIGIN = 0xbfc4ff40, LENGTH = 0x00000040
    configsfrs_BFC4FFC0     : ORIGIN = 0xbfc4ffc0, LENGTH = 0x00000040
    configsfrs_BFC54000     : ORIGIN = 0xbfc54000, LENGTH = 0x00000014
    configsfrs_BFC54020     : ORIGIN = 0xbfc54020, LENGTH = 0x00000008
    configsfrs_BFC6FF40     : ORIGIN = 0xbfc6ff40, LENGTH = 0x00000040
    configsfrs_BFC6FFC0     : ORIGIN = 0xbfc6ffc0, LENGTH = 0x00000040
    kseg2_ebi_data_mem      : ORIGIN = 0xc0000000, LENGTH = 0x04000000
    kseg2_sqi_data_mem      : ORIGIN = 0xd0000000, LENGTH = 0x04000000
    kseg3_ebi_data_mem      : ORIGIN = 0xe0000000, LENGTH = 0x04000000
    kseg3_sqi_data_mem      : ORIGIN = 0xf0000000, LENGTH = 0x04000000
}


/*************************************************************************
 * Memory Address Equates
 *************************************************************************/

_BEV_EXCPT_ADDR                 = 0xBFC00380;
_DBG_EXCPT_ADDR                 = 0xBFC00480;

_SIMPLE_TLB_REFILL_EXCPT_OFFSET = 0x040;
_TLB_TABLE_OFFSET               = 0x0A0;
_IMAGE_PTR_TABLE_OFFSET         = 0x0F8;
_IMAGE_HEADER_OFFSET            = 0x0FC;
_CACHE_ERR_EXCPT_OFFSET         = 0x100;
_GEN_EXCPT_OFFSET               = 0x180;


_ebase_address  			= ORIGIN(kseg1_boot_mem);
_RESET_ADDR             	= ORIGIN(kseg1_boot_mem);
_STARTUP_ADDR               = ORIGIN(kseg1_program_mem);
_RAM_SKIP_SIZE				= 0x600;
_EEPROM_ADDR                    = ORIGIN(kseg1_eeprom_mem);
_SIMPLE_TLB_REFILL_EXCPT_ADDR   = _ebase_address + _SIMPLE_TLB_REFILL_EXCPT_OFFSET;
_TLB_TABLE                      = ORIGIN(kseg1_vector_mem) + _TLB_TABLE_OFFSET;
_CACHE_ERR_EXCPT_ADDR           = _ebase_address + _CACHE_ERR_EXCPT_OFFSET;
_GEN_EXCPT_ADDR                 = _ebase_address + _GEN_EXCPT_OFFSET;
_IMAGE_PTR_TABLE                = _ebase_address + _IMAGE_PTR_TABLE_OFFSET;
_IMAGE_HEADER_ADDR              = _ebase_address + _IMAGE_HEADER_OFFSET;

/*************************************************************************
 *  Bootloader program directives.
 *  
 * _IMAGE_TYPE
 *
 *  image type:
 */

_imageReserved                      = 0x00000000 ;
_imageMPIDE                         = 0x00000001 ;  /* This is a normal MPIDE sketch                                                                                                    */
_imageBootFlashBootloader           = 0x00000002 ;  /* This is a boot flash bootloader                                                                                                  */
_imageProgramFlashBootloader        = 0x00000004 ;  /* This is a program flash bootloader                                                                                               */
_imageSplitFlashBootloader          = 0x00000008 ;  /* This has bootloader code in both boot and program flash                                                                          */

 /*
 *  Instructions for the bootloader
 */

_imageFullFlashEraseLess4KEEProm    = 0x00010000 ;  /* The original bootloader method of erasing all of program flash except the last 4K reserved for eeprom                            */
_imageJustInTimeFlashErase          = 0x00020000 ;  /* Only flash pages written too needed by the sketch is erased                                                                      */
_imageLinkerSpecifiedFlashErase     = 0x00040000 ;  /* The linker defines the flash range to erase                                                                                      */
_imageFullFlashErase                = 0x00080000 ;  /* All of flash is erased                                                                                                           */
_imageExecutionJumpAddress          = 0x01000000 ;  /* the bootloader will jump to the execution jump address immediately after programming                                             */
_imageExecutionJumpToFirstInFlash   = 0x02000000 ;  /* the bootloader will jump to the first sketch loaded in flash ignoring the execution jump address immediately after programming   */

/*  
 * _IMAGE_FLASH_SIZE 
 *
 *      Typically _imageJustInTimeFlashErase is selected to just erase the pages
 *      of flash that code is written too; thus leaving all other flash pages untouched.
 *  
 *      If _imageLinkerSpecifiedFlashErase set, then the range
 *      starting from _ebase_address for _IMAGE_FLASH_SIZE bytes are erased.
 *
 *      If _imageFullFlashErase is specified, than the whole flash
 *      as known by the bootloader will be erased. This will erase eeprom as well
 *
 *      if _imageFullFlashEraseLess4KEEProm is set, all of flash less the last 4K is
 *      erased, this is the old default. This bit could be set to make a program flash bootloader
 *      erasing everything but the old flash. If NOTHING is set, this will be the default as this is the old behavior.
 *  
 *  _JUMP_ADDR
 *  
 *      This is the address that the bootloader will jump to start execution
 *      of the sketch. This is almost always _RESET_ADDR.
 *
 *      However, you can specify an alternate entry execution point for example
 *      if you have alternate starup code that, say, shared
 *      the runtime with other sketches or needed some kind of specific handling
 *
 *      Immediately after programming (avrdude upload) the bootloader will typically
 *      jump to the just loaded sketch, no matter where it was loaded in flash.
 *      _imageExecutionJumpToFirstInFlash will tell the bootloader to jump to the first
 *      sketch in flash even if the just loaded one is not at the beginning of flash.
 *      This is useful when programming sketches in slots of flash and then always
 *      jumping to the program-flash loader (vector sketch) as if the board was just reset.
 *      This bit does not effect jumping to a sketch already in flash after reset.
 *      As of today, after reset, the first program in flash will always be jumped to.
 *
 *************************************************************************/
 _IMAGE_TYPE            = _imageMPIDE | _imageJustInTimeFlashErase | _imageExecutionJumpAddress;
 _IMAGE_FLASH_SIZE      = LENGTH(kseg0_vector_mem) + LENGTH(kseg0_program_mem);
 _JUMP_ADDR             = _RESET_ADDR;




/* these are not used */
/* _BEV_EXCPT_ADDR             = 0xbfc00380; */
/* _GEN_EXCPT_ADDR             = (_ebase_address + 0x180); */
/* _CACHE_ERR_EXCPT_ADDR       = (_ebase_address + 0x100); */
/* _SIMPLE_TLB_REFILL_EXCPT_ADDR = (_ebase_address + 0x0); */
/* end of not used */

SECTIONS
{
    .config_BFC0FF40 : {
     KEEP(*(.config_BFC0FF40))
     } > config_BFC0FF40

    .config_BFC0FF44 : {
     KEEP(*(.config_BFC0FF44))
     } > config_BFC0FF44

    .config_BFC0FF48 : {
     KEEP(*(.config_BFC0FF48))
     } > config_BFC0FF48

    .config_BFC0FF4C : {
     KEEP(*(.config_BFC0FF4C))
     } > config_BFC0FF4C

    .config_BFC0FF50 : {
     KEEP(*(.config_BFC0FF50))
     } > config_BFC0FF50

    .config_BFC0FF54 : {
     KEEP(*(.config_BFC0FF54))
     } > config_BFC0FF54

    .config_BFC0FF58 : {
     KEEP(*(.config_BFC0FF58))
     } > config_BFC0FF58

    .config_BFC0FF5C : {
     KEEP(*(.config_BFC0FF5C))
     } > config_BFC0FF5C

    .config_BFC0FF60 : {
     KEEP(*(.config_BFC0FF60))
     } > config_BFC0FF60

    .config_BFC0FF64 : {
     KEEP(*(.config_BFC0FF64))
     } > config_BFC0FF64

    .config_BFC0FF68 : {
     KEEP(*(.config_BFC0FF68))
     } > config_BFC0FF68

    .config_BFC0FF6C : {
     KEEP(*(.config_BFC0FF6C))
     } > config_BFC0FF6C

    .config_BFC0FF70 : {
     KEEP(*(.config_BFC0FF70))
     } > config_BFC0FF70

    .config_BFC0FF74 : {
     KEEP(*(.config_BFC0FF74))
     } > config_BFC0FF74

    .config_BFC0FF78 : {
     KEEP(*(.config_BFC0FF78))
     } > config_BFC0FF78

    .config_BFC0FF7C : {
     KEEP(*(.config_BFC0FF7C))
     } > config_BFC0FF7C

    .config_BFC0FFC0 : {
     KEEP(*(.config_BFC0FFC0))
     } > config_BFC0FFC0

    .config_BFC0FFC4 : {
     KEEP(*(.config_BFC0FFC4))
     } > config_BFC0FFC4

    .config_BFC0FFC8 : {
     KEEP(*(.config_BFC0FFC8))
     } > config_BFC0FFC8

    .config_BFC0FFCC : {
     KEEP(*(.config_BFC0FFCC))
     } > config_BFC0FFCC

    .config_BFC0FFD0 : {
     KEEP(*(.config_BFC0FFD0))
     } > config_BFC0FFD0

    .config_BFC0FFD4 : {
     KEEP(*(.config_BFC0FFD4))
     } > config_BFC0FFD4

    .config_BFC0FFD8 : {
     KEEP(*(.config_BFC0FFD8))
     } > config_BFC0FFD8

    .config_BFC0FFDC : {
     KEEP(*(.config_BFC0FFDC))
     } > config_BFC0FFDC

    .config_BFC0FFE0 : {
     KEEP(*(.config_BFC0FFE0))
     } > config_BFC0FFE0

    .config_BFC0FFE4 : {
     KEEP(*(.config_BFC0FFE4))
     } > config_BFC0FFE4

    .config_BFC0FFE8 : {
     KEEP(*(.config_BFC0FFE8))
     } > config_BFC0FFE8

    .config_BFC0FFEC : {
     KEEP(*(.config_BFC0FFEC))
     } > config_BFC0FFEC

    .config_BFC0FFF0 : {
     KEEP(*(.config_BFC0FFF0))
     } > config_BFC0FFF0

    .config_BFC0FFF4 : {
     KEEP(*(.config_BFC0FFF4))
     } > config_BFC0FFF4

    .config_BFC0FFF8 : {
     KEEP(*(.config_BFC0FFF8))
     } > config_BFC0FFF8

    .config_BFC0FFFC : {
     KEEP(*(.config_BFC0FFFC))
     } > config_BFC0FFFC

    .config_BFC4FF40 : {
     KEEP(*(.config_BFC4FF40))
     } > config_BFC4FF40

    .config_BFC4FF44 : {
     KEEP(*(.config_BFC4FF44))
     } > config_BFC4FF44

    .config_BFC4FF48 : {
     KEEP(*(.config_BFC4FF48))
     } > config_BFC4FF48

    .config_BFC4FF4C : {
     KEEP(*(.config_BFC4FF4C))
     } > config_BFC4FF4C

    .config_BFC4FF50 : {
     KEEP(*(.config_BFC4FF50))
     } > config_BFC4FF50

    .config_BFC4FF54 : {
     KEEP(*(.config_BFC4FF54))
     } > config_BFC4FF54

    .config_BFC4FF58 : {
     KEEP(*(.config_BFC4FF58))
     } > config_BFC4FF58

    .config_BFC4FF5C : {
     KEEP(*(.config_BFC4FF5C))
     } > config_BFC4FF5C

    .config_BFC4FF60 : {
     KEEP(*(.config_BFC4FF60))
     } > config_BFC4FF60

    .config_BFC4FF64 : {
     KEEP(*(.config_BFC4FF64))
     } > config_BFC4FF64

    .config_BFC4FF68 : {
     KEEP(*(.config_BFC4FF68))
     } > config_BFC4FF68

    .config_BFC4FF6C : {
     KEEP(*(.config_BFC4FF6C))
     } > config_BFC4FF6C

    .config_BFC4FF70 : {
     KEEP(*(.config_BFC4FF70))
     } > config_BFC4FF70

    .config_BFC4FF74 : {
     KEEP(*(.config_BFC4FF74))
     } > config_BFC4FF74

    .config_BFC4FF78 : {
     KEEP(*(.config_BFC4FF78))
     } > config_BFC4FF78

    .config_BFC4FF7C : {
     KEEP(*(.config_BFC4FF7C))
     } > config_BFC4FF7C

    .config_BFC4FFC0 : {
     KEEP(*(.config_BFC4FFC0))
     } > config_BFC4FFC0

    .config_BFC4FFC4 : {
     KEEP(*(.config_BFC4FFC4))
     } > config_BFC4FFC4

    .config_BFC4FFC8 : {
     KEEP(*(.config_BFC4FFC8))
     } > config_BFC4FFC8

    .config_BFC4FFCC : {
     KEEP(*(.config_BFC4FFCC))
     } > config_BFC4FFCC

    .config_BFC4FFD0 : {
     KEEP(*(.config_BFC4FFD0))
     } > config_BFC4FFD0

    .config_BFC4FFD4 : {
     KEEP(*(.config_BFC4FFD4))
     } > config_BFC4FFD4

    .config_BFC4FFD8 : {
     KEEP(*(.config_BFC4FFD8))
     } > config_BFC4FFD8

    .config_BFC4FFDC : {
     KEEP(*(.config_BFC4FFDC))
     } > config_BFC4FFDC

    .config_BFC4FFE0 : {
     KEEP(*(.config_BFC4FFE0))
     } > config_BFC4FFE0

    .config_BFC4FFE4 : {
     KEEP(*(.config_BFC4FFE4))
     } > config_BFC4FFE4

    .config_BFC4FFE8 : {
     KEEP(*(.config_BFC4FFE8))
     } > config_BFC4FFE8

    .config_BFC4FFEC : {
     KEEP(*(.config_BFC4FFEC))
     } > config_BFC4FFEC

    .config_BFC4FFF0 : {
     KEEP(*(.config_BFC4FFF0))
     } > config_BFC4FFF0

    .config_BFC4FFF4 : {
     KEEP(*(.config_BFC4FFF4))
     } > config_BFC4FFF4

    .config_BFC4FFF8 : {
     KEEP(*(.config_BFC4FFF8))
     } > config_BFC4FFF8

    .config_BFC4FFFC : {
     KEEP(*(.config_BFC4FFFC))
     } > config_BFC4FFFC

    .config_BFC54000 : {
     KEEP(*(.config_BFC54000))
     } > config_BFC54000

    .config_BFC54004 : {
     KEEP(*(.config_BFC54004))
     } > config_BFC54004

    .config_BFC54008 : {
     KEEP(*(.config_BFC54008))
     } > config_BFC54008

    .config_BFC5400C : {
     KEEP(*(.config_BFC5400C))
     } > config_BFC5400C

    .config_BFC54010 : {
     KEEP(*(.config_BFC54010))
     } > config_BFC54010

    .config_BFC54020 : {
     KEEP(*(.config_BFC54020))
     } > config_BFC54020

    .config_BFC54024 : {
     KEEP(*(.config_BFC54024))
     } > config_BFC54024

    .config_BFC6FF40 : {
     KEEP(*(.config_BFC6FF40))
     } > config_BFC6FF40

    .config_BFC6FF44 : {
     KEEP(*(.config_BFC6FF44))
     } > config_BFC6FF44

    .config_BFC6FF48 : {
     KEEP(*(.config_BFC6FF48))
     } > config_BFC6FF48

    .config_BFC6FF4C : {
     KEEP(*(.config_BFC6FF4C))
     } > config_BFC6FF4C

    .config_BFC6FF50 : {
     KEEP(*(.config_BFC6FF50))
     } > config_BFC6FF50

    .config_BFC6FF54 : {
     KEEP(*(.config_BFC6FF54))
     } > config_BFC6FF54

    .config_BFC6FF58 : {
     KEEP(*(.config_BFC6FF58))
     } > config_BFC6FF58

    .config_BFC6FF5C : {
     KEEP(*(.config_BFC6FF5C))
     } > config_BFC6FF5C

    .config_BFC6FF60 : {
     KEEP(*(.config_BFC6FF60))
     } > config_BFC6FF60

    .config_BFC6FF64 : {
     KEEP(*(.config_BFC6FF64))
     } > config_BFC6FF64

    .config_BFC6FF68 : {
     KEEP(*(.config_BFC6FF68))
     } > config_BFC6FF68

    .config_BFC6FF6C : {
     KEEP(*(.config_BFC6FF6C))
     } > config_BFC6FF6C

    .config_BFC6FF70 : {
     KEEP(*(.config_BFC6FF70))
     } > config_BFC6FF70

    .config_BFC6FF74 : {
     KEEP(*(.config_BFC6FF74))
     } > config_BFC6FF74

    .config_BFC6FF78 : {
     KEEP(*(.config_BFC6FF78))
     } > config_BFC6FF78

    .config_BFC6FF7C : {
     KEEP(*(.config_BFC6FF7C))
     } > config_BFC6FF7C

    .config_BFC6FFC0 : {
     KEEP(*(.config_BFC6FFC0))
     } > config_BFC6FFC0

    .config_BFC6FFC4 : {
     KEEP(*(.config_BFC6FFC4))
     } > config_BFC6FFC4

    .config_BFC6FFC8 : {
     KEEP(*(.config_BFC6FFC8))
     } > config_BFC6FFC8

    .config_BFC6FFCC : {
     KEEP(*(.config_BFC6FFCC))
     } > config_BFC6FFCC

    .config_BFC6FFD0 : {
     KEEP(*(.config_BFC6FFD0))
     } > config_BFC6FFD0

    .config_BFC6FFD4 : {
     KEEP(*(.config_BFC6FFD4))
     } > config_BFC6FFD4

    .config_BFC6FFD8 : {
     KEEP(*(.config_BFC6FFD8))
     } > config_BFC6FFD8

    .config_BFC6FFDC : {
     KEEP(*(.config_BFC6FFDC))
     } > config_BFC6FFDC

    .config_BFC6FFE0 : {
     KEEP(*(.config_BFC6FFE0))
     } > config_BFC6FFE0

    .config_BFC6FFE4 : {
     KEEP(*(.config_BFC6FFE4))
     } > config_BFC6FFE4

    .config_BFC6FFE8 : {
     KEEP(*(.config_BFC6FFE8))
     } > config_BFC6FFE8

    .config_BFC6FFEC : {
     KEEP(*(.config_BFC6FFEC))
     } > config_BFC6FFEC

    .config_BFC6FFF0 : {
     KEEP(*(.config_BFC6FFF0))
     } > config_BFC6FFF0

    .config_BFC6FFF4 : {
     KEEP(*(.config_BFC6FFF4))
     } > config_BFC6FFF4

    .config_BFC6FFF8 : {
     KEEP(*(.config_BFC6FFF8))
     } > config_BFC6FFF8

    .config_BFC6FFFC : {
     KEEP(*(.config_BFC6FFFC))
     } > config_BFC6FFFC
}
