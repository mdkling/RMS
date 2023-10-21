#ifndef LOCALTYPES_HEADER
#define LOCALTYPES_HEADER

typedef unsigned char  u8;
typedef signed char    s8;
typedef unsigned short u16;
typedef signed short   s16;
typedef unsigned int   u32;
typedef signed int     s32;
typedef void (*genericFunction)(void *);
typedef void (*wordReceiver)(void *start, u32 length, void *context);

// assembly related symbols

s32 asmDiv(s32 divisor, s32 dividend);
s32 asmMod(s32 divisor, s32 dividend);
s32 fithDiv(s32 divisor, s32 dividend);
s32 fithMod(s32 divisor, s32 dividend);
s32 asmGetDiv(void);
void os_takeSpinLock(u32 lockNum);
void os_giveSpinLock(u32 lockNum);
void setZero(void *dst, s32 size);
void setZeroWait(void);
void core1ServerSetup(void) __attribute__((noreturn));
void xipSetup(void);
void REBOOT(void);
void startSysTimer(void);
void printFithStack(void);
u32 endSysTimer(void);
void threadFithEnter(void *c, void *target);
void threadFithExit(void);
void fith_logicalAnd(void);
void fithFree(void);
void fithAlloc(void);
void fithRealloc(void);
void fithPrintsn(void);
void fithPrints(void);
void fithPrinti(void);
void fithPrintin(void);
void fithPrinth(void);
void fithPrinthn(void);
void fithOnce(void);
void armFithWrapper(void);
void* co_yieldWrapper(void *arg, void *toCoroutine);
void co_yield(void);
void* co_getFrom(void);
void suspendUartTxOutput(void);
void printSP(void);
void fithClearStack(void);
void checkedPopStack(void);
void fithAddEqualsGlobal(void);
void fithSubEqualsGlobal(void);
void fithMulEqualsGlobal(void);
void fithOrrEqualsGlobal(void);
void fithAndEqualsGlobal(void);
void fithBicEqualsGlobal(void);
void fithXorEqualsGlobal(void);
void fithLslsEqualsGlobal(void);
void fithLsrsEqualsGlobal(void);
void fithCommaS32(void);
void fithCommaU16(void);
void fithCommaU8(void);
void co_call(void);
void asm_wrapper(void);
void* co_return(void*);
void* fithCoCreate(void*);
void fithLit(void);
void fithEnter(void);
void fithExit(void);
void fithDup(void);
void fithDrop(void);
void fithSwap(void);
void fithSub(void);
void fithAdd(void);
void fithMul(void);
void fithJump(void);
void fithBE(void);
void fithBNE(void);
void fithBGE(void);
void fithBLT(void);
void fithBGT(void);
void fithBLE(void);
void fithPrintMemStats(void);
void fithDefineFunction(void);
void fithDefineVar(void);
void fithDefineCon(void);
void fithEndBlock(void);
void fithSet(void);
void fithStoreToAddr(void);
void fithLoadFromAddr(void);
void fithCompileCondBE(void);
void fithCompileCondBNE(void);
void fithCompileCondBGE(void);
void fithCompileCondBLT(void);
void fithCompileCondBGT(void);
void fithCompileCondBLE(void);
void fithCompileElse(void);
void fithCompileWhile(void);
void fithCreateLocalVar(void);
void fithLocalVar(void);
void fithInitLocalVar(void);
void fithLParen(void);
void fithRParen(void);
void fSmallFithE(void);
long long _WORD(void);
void ENTER(void);
void EXIT(void);
long long stackMachineEnter(u32 tos, void *sp, u32 addr);
void pengumMachineEnter(u32 addr, void *sp);
void pengumPs(void);
void pengumPsn(void);
void pengumPi(void);
void pengumPin(void);
void pengumPh(void);
void pengumPhn(void);
void pengumPc(void);
void pengumPch(void);
void pengumAbs(void);
void pengumDiv(void);
void pengumMod(void);
void pengumFree(void);
void pengumZalloc(void);
void pengumRealloc(void);
void pengumAnd(void);
void copyBackward(void *src, void* dst, u32 size);
void copyForward(void *src, void* dst, u32 size);



extern u32 vector_table[];
extern u32 __bss_end__[];
extern u32 __bss_start__[];
extern u32 boot2Entry[];
extern u16 testThreadData[];
extern u16 testPrintThread[];
extern u16 fith_defs[];
extern u8  enter[];
//~ typedef struct FithState FithState;

void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
);
void *ParseAlloc(void *(*mallocProc)(u32));
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  void *yyminor       /* The value for the token */
);

void IRtoMachineFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
);
void *IRtoMachineAlloc(void *(*mallocProc)(u32));
void IRtoMachine(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  void *yyminor       /* The value for the token */
);

#define LOCK_MEMORY_ALLOC  0
#define LOCK_PROCESS_QUEUE 1
#define LOCK_UART0_OUT     2
#define LOCK_UART0_IN      3
#define LOCK_ALARM         4
#define LOCK_FLASH         5
#define LOCK_COPY_DMA      6


#define SIZE_TASK_QUEUE 32
#define SIZE_PRINT_BUFF (1<<9)

#define ATOMIC_XOR         = 0x1000
#define ATOMIC_SET         = 0x2000
#define ATOMIC_CLR         = 0x3000

#define SIO_BASE        0xD0000000
#define SIO_FIFO_ST    (SIO_BASE+0x50)
#define SIO_SPINLOCK_0 (SIO_BASE + 0x16C)

#define PPB_BASE              0xE0000000
#define PPB_SYSTICK              (PPB_BASE+0xE010)
#define PPB_INTERRUPT_ENABLE     (PPB_BASE+0xE100)
#define PPB_INTERRUPT_DISABLE    (PPB_BASE+0xE180)
#define PPB_INTERRUPT_SET_PEND   (PPB_BASE+0xE200)
#define PPB_INTERRUPT_CLEAR_PEND (PPB_BASE+0xE280)
#define PPB_NVIC_IPR6            (PPB_BASE+0xE418)
#define PPB_NVIC_IPR7            (PPB_BASE+0xE41C)
#define PPB_SYS_ICSR             (PPB_BASE+0xED04)
#define PPB_SYS_CNTRL            (PPB_BASE+0xED10)
#define PPB_SYS_SHPR2            (PPB_BASE+0xED20)


#define FIFO_BUFFER      0x20040000
#define FIFO_WRITER_ADDR 0x20040100
#define FIFO_READER_ADDR 0x20040104

#define HEAP_TOP_ADDR      0x20040000

#define UART0_BASE         0x40034000
#define UART0_DR           0x000
#define UART0_FR           0x018
#define UART0_IBRD         0x024
#define UART0_FBRD         0x028
#define UART0_LCR          0x02C
#define UART0_CR           0x030
#define UART0_IRMASK       0x038
#define UART0_IRMASKSTATUS 0x040
#define UART0_IRCLEAR      0x044
#define UART0_DMA_CR       0x048

#define TIMER_BASE        0x40054000

#define FLASH_BASE        0x10000000

#define END_OF_RAM        ((u32)0x20042000)
#define SECOND_STACK      ((u32)0x20041D00)
#define FITH_EXPR_STACK   ((u32)0x20040C00)
#define FITH_RETURN_STACK ((u32)0x20040804)
//~ #define END_OF_REG_RAM    ((u32)0x20040000)
#define END_OF_REG_RAM    ((u32)0x20041C20)
#define START_OF_REG_RAM  ((u32)0x20010000)
//~ #define START_OF_REG_RAM  ((u32)0x2000FA90)

typedef struct Uart0MemMap {
	volatile u32 data;
	volatile u32 errorStatus;
	u32 pad1; // 8
	u32 pad2; // C
	u32 pad3; // 10
	u32 pad4; // 14
	volatile u32 flags; // 18
	u32 pad5; // 1C
	volatile u32 lowPower; // 20
	volatile u32 integerBaud; // 24
	volatile u32 fracBaud; // 28
	volatile u32 lineControl; // 2C
	volatile u32 controlReg; // 30
	volatile u32 fifoLevelSelect; // 34
	volatile u32 intMaskSet; // 38
	volatile u32 rawIntStatus; // 3C
	volatile u32 maskedIntStatus; // 40
	volatile u32 intClear; // 44
	volatile u32 dmaCntrl; // 48
} Uart0MemMap;

typedef struct TimerMemMap {
	volatile u32 timeWriteHigh;
	volatile u32 timeWriteLow;
	volatile u32 timeReadHigh;
	volatile u32 timeReadLow;
	volatile u32 alarm[4];
	volatile u32 armed;
	volatile u32 timeReadHighRaw;
	volatile u32 timeReadLowRaw;
	volatile u32 debugPause;
	volatile u32 pause;
	volatile u32 intr;
	volatile u32 inte;
	volatile u32 intf;
	volatile u32 ints;
} TimerMemMap;

typedef struct Tree {
	struct Tree	*next[2];
	void		*value;
	u8		type;
	u8		tClass;
	u8		level;
	u8		keyLen;
	u8		key[4];
} Tree;

typedef struct Word {
	s16			next;
	u8			type;
	u8			keyLen;
	u8			key[2];
} Word;

typedef struct Token {
	s32  type;
	u8  *string;
	s32  length;
	u32  lineNumber;
} Token;

typedef struct Expr {
	s32  type;
	s32  exprType;
	s32  value;
	u32  lineNumber;
} Expr;

typedef struct Ident {
	s32   type;
	Tree *var;
	s32   pad;
	u32   lineNumber;
} Ident;

typedef struct {
	s32  type;
	s32  regNum;
	s32  lineNumber;
	u32 *location;
} Variable;

typedef struct Function {
	s32  type;
	s32  returnType;
	u16 *startOfFunction;
	s32  argType[4];
} Function;

typedef struct {
	s32  type; // must always be first
	u8  *string;
	s32  length;
	u32  lineNumber;
} Literal;

typedef union NonTerminal {
	Token     tok;
	Literal   lit;
	Expr      exp;
	Variable  var;
	Ident     idn;
} NonTerminal;

typedef struct VirtualCode {
	u8  type;
	u8  flags;
	u8  varNum;
	u8  regNum;
	u32 value;
} VirtualCode;

typedef struct LargeConst {
	struct LargeConst *next;
	u16               *target;
	u32                value;
	u32                putInScratch;
} LargeConst;

typedef struct Block {
	struct Block	*next;
	u8		hasReturn;
	u8		hasRepeat;
	u8		ifHasExit;
	u8		cond;
	u16		blockType;
	s32		enteredStackState;
	s32		endCondStackState;
	Tree		*word;
	u16		*target;
	//~ struct Block *caseList;
} Block;

typedef struct String {
	struct String *next;
	u16           *target;
	u32            length;
	u8             string[4];
} String;

typedef struct StringBuff {
	struct StringBuff *next;
	u8                 string[4];
} StringBuff;

typedef struct {
	Tree 	*symbols;
	u32		varNum;
} LocalScope;

typedef struct SmallContext {
	u8          error;
	u8          notLeaf;
	u8          combineWordState;
	u8          inParams;
	Word		*words;
	Block       *blockBase;
	Block       *blockCursor;
	u32			globalsIndex;
	u16			*compileBase;
	u16			*compileCursor;
	u32			varNumHighWater;
	LocalScope	*scope;
	u32			tos;
	u32			*exprStack;
	LocalScope	scopes[8];
} SmallContext;

extern SmallContext s;

typedef struct Routine {
	struct Routine *next;
	void           *routine;
} Routine;

typedef struct Event {
	void *function;
	void *data;
} Event;

typedef struct ByteStream {
	struct ByteStream *next;
	u8                 data[124];
} ByteStream;

enum {
	EXPR_NULL,
	EXPR_SMALL_LIT,
	EXPR_BIG_LIT,
	TYPE_IDENT,
};

typedef struct {
	u32 csr;
	u32 rvr;
	u32 cvr;
} SYS_TICK_S;

typedef struct {
	void (*_connect_internal_flash)(void);
	void (*_flash_exit_xip)(void);
	void (*_flash_range_erase)(u32 addr, u32 count, u32 block_size, u8 block_cmd);
	void (*flash_range_program)(u32 addr, u8 *data, u32 count);
	void (*_flash_flush_cache)(void);
	u8*  (*memcpy)(void *dest, void *src, u32 count);
	u8*  (*memset)(void *dest, u32 val, u32 count);
} ROMFunctions;

extern ROMFunctions rom_func;
extern void (*lineHandler)(u8 *in);

extern u32 resetAllRegs[];
extern char *Gkernel;

void printWord(u32 data);
//~ void io_prints(u8 *string);
void asm_processChar(u32 in);
u32 stringLen(u8 *string);
u8* lex(u8 *sourceCode, Token *t);

#include "inc/memory.h"
#include "inc/helperCpu.h"
#include "inc/io.h"
#include "inc/tree.h"
#include "inc/list.h"
#include "inc/timer.h"
#include "inc/taskQueue.h"
#include "inc/pengum.h"
#include "inc/terminal.h"

#endif
