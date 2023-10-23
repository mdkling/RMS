// io.c
#include "../localTypes.h"

ROMFunctions    rom_func;

#define UART0_OUT_BUFF_SIZE (1<<11)
#define UART0_OUT_BUFF_MASK (UART0_OUT_BUFF_SIZE-1)

typedef struct {
	u32 read;
	u32 write;
	u8	b[UART0_OUT_BUFF_SIZE];
} Uart0Output;

static Uart0Output out;
//~ static u8       uart0Buffer[256];
//~ static u32      uart0BuffStartIndex;
//~ static u32      uart0BuffIndex;

/*e*/
s32
s2i(u8 *b, u8 **e)/*p;*/
{
	s32 result     = 0;
	s32 isNegative = 1;
	if (*b == '-')
	{
		b++;
		isNegative = -1;
	}
	
	if (*b == '0' && *(b+1) == 'x')
	{
		// process hex numbers
		b += 2;
		tryAnotherByte:
		if ( (*b >= '0') && (*b <= '9') )
		{
			result = (result * 16) + (*b - '0');
			b++;
			goto tryAnotherByte;
		} else if ( (*b >= 'A') && (*b <= 'F') ) {
			result = (result * 16) + (*b - ('A' - 10));
			b++;
			goto tryAnotherByte;
		} else {
			goto end;
		}
	}
	
	while( (*b >= '0') && (*b <= '9') )
	{
		result = (result * 10) + (*b - '0');
		b++;
	}
	
	end:
	result = isNegative * result;
	if (e) { *e = b; }
	
	return result;
}

#if 0
s32
s2iOld(u8 *b)
{
	s32 result     = 0;
	s32 isNegative = 1;
	if (*b == '-')
	{
		b++;
		isNegative = -1;
	}
	
	if (*b == '0' && *(b+1) == 'x')
	{
		// process hex numbers
		b += 2;
		tryAnotherByte:
		if ( (*b >= '0') && (*b <= '9') )
		{
			result = (result * 16) + (*b - '0');
			b++;
			goto tryAnotherByte;
		} else if ( (*b >= 'A') && (*b <= 'F') ) {
			result = (result * 16) + (*b - ('A' - 10));
			b++;
			goto tryAnotherByte;
		} else {
			goto end;
		}
	}
	
	while( (*b >= '0') && (*b <= '9') )
	{
		result = (result * 10) + (*b - '0');
		b++;
	}
	
	end:
	result = isNegative * result;
	
	return result;
}
#endif

/*e*/
u8*
i2s(s32 in, u8 *out)/*p;*/
{
	u8 number[16];
	s32 remainder = 0;
	s32 index = 0;
	s32 isNegative = 0;
	if (in >> 31)
	{
		isNegative = 1;
		in = -in;
	}
	do {
		remainder = asmMod(10, in);
		in        = asmGetDiv();
		number[index++] = remainder + '0';
	} while (in != 0);
	index--;
	// number is now consumed as a string
	if (isNegative)
	{
		*out++ = '-';
	}
	do {
		*out++ = number[index--];
	} while (index >= 0);
	*out = 0;
	return out;
}

/*e*/
u8*
i2sh(s32 in, u8 *out, u32 numDigits)/*p;*/
{
	// generate number directly in output
	s32 byte;
	for (s32 x = 8-numDigits; x < 8; x++)
	{
		byte = (in>>((7-x)*4))&0x0F;
		byte += '0';
		if (byte > '9')
		{
			byte += 7;
		}
		*out++ = byte;
	}
	*out = 0;
	return out;
}

//~ static void bufferAndSend(u32 data)
//~ {
	//~ uart0Buffer[uart0BuffIndex++] = data;
	//~ if (data == 0x0D || uart0BuffIndex == 254) // finished line
	//~ {
		//~ // add new line and null terminate
		//~ uart0Buffer[uart0BuffIndex++] = '\n';
		//~ uart0Buffer[uart0BuffIndex++] = 0;
		//~ u8* startOfLine = &uart0Buffer[uart0BuffStartIndex];
		//~ if (uart0BuffIndex > 128)
		//~ {
			//~ uart0BuffStartIndex = 0;
			//~ uart0BuffIndex = 0;
		//~ } else {
			//~ uart0BuffStartIndex = uart0BuffIndex;
		//~ }
		//~ task_enqueue(pengum_compile, startOfLine, 0);
	//~ }
//~ }

/*e*/void
uart0processInputs(void)/*p;*/
{
	Uart0MemMap *uart = (void*)UART0_BASE;
	while ( (uart->flags & (1<<4)) == 0)
	{
		//~ bufferAndSend(uart->data);
		term_processCharacter(uart->data);
	}
}

/*e*/void
uart0processOutputs(void)/*p;*/
{
	Uart0MemMap *uart = (void*)UART0_BASE;
	u32 read = out.read;
	u32 write = out.write;
	while (read != write && (uart->flags&(1<<5)) == 0 )
	{
		uart->data = out.b[read];
		read = (read+1) & UART0_OUT_BUFF_MASK;
	}
	out.read = read;
}

/*e*/void
uart0processAllOutputs(void)/*p;*/
{
	Uart0MemMap *uart = (void*)UART0_BASE;
	while (out.read != out.write || (uart->flags&(1<<3)) != 0)
	{
		uart0processOutputs();
	}
}

/*e*/void
uart0_outByte(u32 byte)/*p;*/
{
	// busy wait if full
	if (out.write + 1 == out.read) { io_busyWait(); }
	asm("CPSID i");  // disable interrupts
	printAnotherChar:
	out.b[out.write] = byte;
	out.write = (out.write+1) & UART0_OUT_BUFF_MASK;
	if (byte == '\n') { byte = '\r'; goto printAnotherChar; }
	asm("CPSIE i"); // enable interrupts
}

/*e*/
void io_prints(u8 *string)/*p;*/
{
	u32 byte = *string++;
	if (byte != 0)
	{
		do {
			uart0_outByte(byte);
			byte = *string++;
		} while (byte != 0);
	}
}

/*e*/
void io_printsn(u8 *string)/*p;*/
{
	io_prints(string);
	uart0_outByte('\n');
}

/*e*/
void io_printsl(u8 *string, u32 len)/*p;*/
{
	for (u32 i = 0; i < len; i++)
	{
		uart0_outByte(string[i]);
	}
}

/*e*/
void io_printsln(u8 *string, u32 len)/*p;*/
{
	io_printsl(string, len);
	uart0_outByte('\n');
}

/*e*/
void io_printi(s32 in)/*p;*/
{
	u8 number[16];
	i2s(in, number);
	io_prints(number);
}

/*e*/
void io_printin(s32 in)/*p;*/
{
	io_printi(in);
	uart0_outByte('\n');
}

/*e*/
void io_printh(s32 in)/*p;*/
{
	u8 number[16];
	number[0] = '0';
	number[1] = 'x';
	i2sh(in, &number[2], 8);
	io_prints(number);
}

/*e*/
void io_printhn(s32 in)/*p;*/
{
	io_printh(in);
	uart0_outByte('\n');
}

/*e*/
void io_printch(s32 in)/*p;*/
{
	u8 number[8];
	i2sh(in, &number[0], 2);
	io_prints(number);
}

/*e*/
s32 io_programFlash(void *data, u32 size, u32 targetBlockNum)/*p;*/
{
	// reserve the first 67 blocks for RAM mirror
	//~ if (targetBlockNum < 67) { return -1; }
	// round up size to 256
	
	size = (size + 255) / 256 * 256;
	u32 blockAddr = targetBlockNum * 4096;
	u32 blockSize = (size + 4095) / 4096 * 4096;
	asm("CPSID i");
	//~ os_takeSpinLock(LOCK_FLASH);
	rom_func._connect_internal_flash();
	rom_func._flash_exit_xip();
	rom_func._flash_range_erase(blockAddr, blockSize, 4096, 0x20);
	rom_func.flash_range_program(blockAddr, data, size);
	rom_func._flash_flush_cache();
	xipSetup();
	//~ os_giveSpinLock(LOCK_FLASH);
	asm("CPSIE i");
	return 0;
}

#define SIO_GPIO_OUT_XOR (SIO_BASE + 0x1C) 

/*e*/
void io_ledToggle(void)/*p;*/
{
	u32 *gpioXor = (void*)SIO_GPIO_OUT_XOR;
	*gpioXor = (1<<25);
}

/*e*/
void printHelloBanner(void)/*p;*/
{
	io_prints("Hello from rms-kling!\n");
	io_prints("Built on ");
	io_prints(__DATE__);
	io_prints(" at ");
	io_prints(__TIME__);
	io_prints("\n>");

}

/*e*/
void printStackStrace(u32 *stack)/*p;*/
{
	u32 coreNum = *(u32*)SIO_BASE;
	io_prints("\nHARD_FAULT CORE ");
	io_printin(coreNum);
	if ((u32)&stack[13] >= END_OF_RAM)
	{
		io_printsn("STACK UNDERFLOW!");
	}
	io_prints("sp = ");
	io_printhn((u32)&stack[13]);
	io_prints("r4 = ");
	io_printhn(stack[0]);
	io_prints("r5 = ");
	io_printhn(stack[1]);
	io_prints("r6 = ");
	io_printhn(stack[2]);
	io_prints("r7 = ");
	io_printhn(stack[3]);
	io_prints("Elr= ");
	io_printhn(stack[4]);
	io_prints("r0 = ");
	io_printhn(stack[5]);
	io_prints("r1 = ");
	io_printhn(stack[6]);
	io_prints("r2 = ");
	io_printhn(stack[7]);
	io_prints("r3 = ");
	io_printhn(stack[8]);
	io_prints("r12= ");
	io_printhn(stack[9]);
	io_prints("lr = ");
	io_printhn(stack[10]);
	io_prints("pc = ");
	io_printhn(stack[11]);
	stack[11] = (u32)resetAllRegs;
	io_prints("psr= ");
	io_printhn(stack[12]);
	io_prints("\n");			//newline to visually separate dump from fresh reset
	uart0processAllOutputs();
}

typedef void *(*rom_table_lookup_fn)(u16 *table, u32 code);

static void*
rom_func_lookup(u32 code)
{
	rom_table_lookup_fn *rom_table_lookup = (rom_table_lookup_fn*)0x18;
	u16 *func_table = (u16 *)0x14;
	return (*rom_table_lookup)((u16 *)(u32)(*func_table), code);
}

//~ static void*
//~ rom_data_lookup(u32 code)
//~ {
	//~ rom_table_lookup_fn *rom_table_lookup = (rom_table_lookup_fn*)0x18;
	//~ u16 *data_table = (u16 *)0x16;
	//~ return (*rom_table_lookup)((u16 *)(u32)(*data_table), code);
//~ }

static u32 rom_table_code(u8 c1, u8 c2)
{
	return (c2 << 8) | c1;
}

static void
loadRomFuncAddrs(void)
{
	// grab all function addresses
	rom_func._connect_internal_flash = rom_func_lookup(rom_table_code('I','F'));
	rom_func._flash_exit_xip = rom_func_lookup(rom_table_code('E','X'));
	rom_func._flash_range_erase = rom_func_lookup(rom_table_code('R','E'));
	rom_func.flash_range_program = rom_func_lookup(rom_table_code('R','P'));
	rom_func._flash_flush_cache = rom_func_lookup(rom_table_code('F','C'));
	rom_func.memcpy = rom_func_lookup(rom_table_code('M','C'));
	rom_func.memset = rom_func_lookup(rom_table_code('M','S'));
}

/*e*/
void picoInit(void)/*p;*/
{
	// clear error flags in FIFO
	u32 *fifoStatus = (void*)SIO_FIFO_ST;
	*fifoStatus = 0x0C;
	// disable all interrupts
	u32 *intDisable = (void*)PPB_INTERRUPT_DISABLE;
	*intDisable = 0xFFFFFFFF;
	// clear all pending interrupts
	u32 *intPend = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intPend = 0xFFFFFFFF;
	// enable NVIC interrupts
	u32 *intEnable = (void*)PPB_INTERRUPT_ENABLE;
	*intEnable = 
	(1<<0)|(1<<1)|(1<<2)|(1<<3) // 4 timer interrupts
	//~ |(1<<20)                    // uart interrupt
	|(1<<26)|(1<<27)|(1<<28);   // software defined interrupts
	// set interrupt priorities
	u32 *intPriority = (void*)PPB_NVIC_IPR6;
	*intPriority++ = (1<<22)|(2<<30); // Vec 26 = 1 Vec 27 = 2
	*intPriority = (3<<6);            // Vec 28 = 3
	// disable deep sleep and disable "wake up on every possible event"
	u32 *sysCtrl = (void*)PPB_SYS_CNTRL;
	*sysCtrl = 0;
	// set sysTick to lowest priority and PendSV
	//~ intPriority = (void*)PPB_SYS_SHPR2;
	//~ *intPriority = (3<<30)|(3<<22);
	//~ intPriority--; *intPriority = (3<<30); // SVC is also lowest priority
	enableWatchDogTick();
	timerInit();
	loadRomFuncAddrs();
	
	//~ io_StreamInit();
	// set up flash
	rom_func._connect_internal_flash();
	rom_func._flash_exit_xip();
	rom_func._flash_flush_cache();
	xipSetup();
	// we can now read flash addresses
	// set timer 0 to kick off RMS
	timer_set(0, 10);
	//~ treeTest();
	// process fith kernel stuff
	startSysTimer();
	pengum_compile(Gkernel);
	io_printi(asmDiv(120, endSysTimer()));
	io_printsn(" microseconds to compile/execute the fith kernel.");
	// changes the mode of putty (not good)
	//~ io_prints("\x1B[20h");
	//~ timer_set(0, 2500);
	//~ setAlarmClockValue(2500);
	//~ timer_set(1, 5*1000*1000);
}
