#include "../localTypes.h"
#include "../inc/pengum_i.h"

#define CLASS1 0x04
#define WORD_BODY 1
#define STATE_STACK_EMPTY 8
#define STATE_STACK_FULL 0

#define COND_BEQ 0x0
#define COND_BNE 0x1
#define COND_BGE 0xA
#define COND_BLT 0xB
#define COND_BGT 0xC
#define COND_BLE 0xD

#define SET_FLAG(f)	c->flags |= (1<<f)
#define GET_FLAG(f)	((c->flags>>f)&1)
#define CLEAR_FLAG(f)	c->flags &= ~(1<<f)

/*e*/typedef struct PengumContext PengumContext;/*i*/
typedef struct PengumContext {
	u32		localsIndex;
	u32		numParams;
	u32		numItemsReturned;
	s32		stackState;
	s32		replStackState;
	u32		flags;
	Block		*blocks;
	Tree		*globals;
	Tree		*locals;
	Tree		*currentWord;
	u16		*compileBase;
	u16		*compileCursor;
	u32		stackSaveArea[8];
} PengumContext;

static PengumContext c = 
{
	.compileBase = (u16*)__bss_end__,
	.compileCursor = ((u16*)__bss_end__) + 2,
	.stackState = STATE_STACK_EMPTY,
};

#define StringLitBuff_SIZE (1<<10)
typedef struct {
	u32 w;
	u8	b[StringLitBuff_SIZE];
} StringLitBuff;
static StringLitBuff scratch;

enum{
	fError,
	fInParams,
	fHasReturn,
};

enum{
	WORD_FUNCTION,
	WORD_GLOBAL,
	WORD_CONSTANT,
	WORD_LOCAL,
};

enum{
	BLOCK_WORD,
	BLOCK_COND,
	BLOCK_ELSE,
	BLOCK_WHILE,
	BLOCK_WHILE_COND,
};

enum{
	DIV=0*CLASS1, // DIVISION OR FORWARD SLASH
	PRC=1*CLASS1, // PERCENT
	STA=2*CLASS1, // STAR
	LIN=3*CLASS1, // VERTICLE LINE
	CAR=4*CLASS1, // CARROT
	TIL=5*CLASS1, // TILDA
	PLU=6*CLASS1, // PLUS
	AMP=7*CLASS1, // AMPERSAND
	MIN=8*CLASS1+WORD_BODY, // MINUS
	SCO=9*CLASS1, // SEMI-COLON *ONLY AFTER WORD
	COL=10*CLASS1, // COLON *ONLY AFTER WORD
	ATS=11*CLASS1, // AT SIGN *ONLY AFTER WORD
	LPA=12*CLASS1, // LEFT PAREN *ONLY AFTER WORD?
	LTH=13*CLASS1, // LESS THAN START OF COMPARATOR
	GTH=14*CLASS1, // GREATER THAN START OF COMPARATOR
	EQU=15*CLASS1, // EQUALS START OF COMPARATOR
	LBL=16*CLASS1, // LEFT BLOCK START OF ANON WORD
	HAS=17*CLASS1, // HASH OR POUND
	TIC=18*CLASS1, // TICK
	COM=19*CLASS1, // COMMA
	RBL=20*CLASS1, // RIGHT BLOCK END OF BLOCK
	DOL=21*CLASS1, // DOLLAR START OF DEREFERNCE OPERATOR
	LBR=22*CLASS1, // LEFT BRACKET START OF IMMEDIATE MODE
	BNG=23*CLASS1, // BANG(!) START OF COMPARATOR
	QMK=24*CLASS1+WORD_BODY, // QUESTION MARK
	DIG=25*CLASS1+WORD_BODY, // DIGIT
	ALP=26*CLASS1+WORD_BODY, // ALPHABET
	DQO=27*CLASS1, // DOUBLE QUOTE START OF STRING
	SQO=28*CLASS1, // SINGLE QUOTE START OF CHAR LITERAL
	RPA=29*CLASS1, // RIGHT PAREN
	RBR=30*CLASS1, // RIGHT BRACKET END OF IMMEDIATE MODE
	BSL=31*CLASS1, // BACK SLASH
	WSP=32*CLASS1, // WHITE SPACE
	NUL=33*CLASS1, // NULL
	BAD=34*CLASS1, // BAD CHARACTER
	TYP=35*CLASS1, // TYPE FOR PRINTING
	//DOT=15*CLASS1, // DOT OR PERIOD
};

static const unsigned char class[] = {
/*         x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xa  xb  xc  xd  xe  xf */
/* 0x */  NUL,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,WSP,WSP,BAD,WSP,WSP,BAD,BAD,
/* 1x */  BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,BAD,
/* 2x */  WSP,BNG,DQO,HAS,DOL,PRC,AMP,SQO,LPA,RPA,STA,PLU,COM,MIN,ALP,DIV,
/* 3x */  DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,DIG,COL,SCO,LTH,EQU,GTH,QMK,
/* 4x */  ATS,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* 5x */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,LBR,BSL,RBR,CAR,ALP,
/* 6x */  TIC,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* 7x */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,LBL,LIN,RBL,TIL,BAD,
/* 8x */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* 9x */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Ax */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Bx */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Cx */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Dx */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Ex */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,
/* Fx */  ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,ALP,TYP,TYP,TYP
};

// Section Machine Code

/*e*/void
mc_callRelative(u32 target, u32 currentAddr)/*i;*/
{
	u16 *outputCursor = (u16*)currentAddr;
	u32 currentPC = currentAddr + 4;
	u32 jump = (target - currentPC) >> 1;
	u32 imm11 = jump << 21 >> 21;
	u32 imm10 = jump << 11 >> 22;
	u32 i2    = jump << 10 >> 31;
	u32 i1    = jump << 9 >> 31;
	u32 S     = jump << 8 >> 31;
	u32 j2    = (i2^1)^S;
	u32 j1    = (i1^1)^S;
	u32 code = 0xF000;
	code += (S<<10) + imm10;
	*outputCursor++ = code;
	code = 0xD000;
	code += (j2<<11) + (j1<<13) + imm11;
	*outputCursor = code;
}

/*e*/static u32
armMovImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x2000;
	code += val + (dest << 8);
	return code;
}

/*e*/static u32
armMov(u32 dest, u32 src)/*i;*/
{
	u32 code = 0x4600;
	code += ((dest>>3)<<7) + ((dest<<29)>>29) + (src << 3);
	return code;
}

/*e*/static u32
armSpAddImm(u32 immediate)/*i;*/
{
	u32 code = 0xB000;
	code += (immediate);
	return code;
}

/*e*/static u32
armPush(u32 regBits)/*i;*/
{
	if (regBits == 0) { return armMov(8,8); } // safe no-op
	u32 code = 0xB400;
	code += regBits;
	return code;
}

/*e*/static u32
armPop(u32 regBits)/*i;*/
{
	if (regBits == 0) { return armMov(8,8); } // safe no-op
	u32 code = 0xBC00;
	code += regBits;
	return code;
}

static u32
armLdrLit(u32 dest, u32 imm)
{
	u32 code = 0x4800;
	code += imm + (dest << 8);
	return code;
}

static u32
armAdr(u32 dest, u32 imm)
{
	u32 code = 0xA000;
	if (imm > 255)
	{
		io_printsn("Error: string is out of range.");
		imm = 1;
	}
	code += imm + (dest << 8);
	return code;
}

/*e*/static u32
armStrSpOffset(u32 src, u32 offset)/*i;*/
{
	u32 code = 0x9000;
	code += (src<<8) + (offset);
	return code;
}

/*e*/static u32
armLdrSpOffset(u32 dest, u32 offset)/*i;*/
{
	u32 code = 0x9800;
	code += (dest<<8) + (offset);
	return code;
}

static u32
armLdrByteOffset(u32 dest, u32 src, u32 offset)
{
	u32 code = 0x7800;
	code += dest + (src << 3) + (offset << 6);
	return code;
}

static u32
armLdrHwOffset(u32 dest, u32 src, u32 offset)
{
	u32 code = 0x8800;
	code += dest + (src << 3) + (offset << 6);
	return code;
}

static u32
armLdrOffset(u32 dest, u32 src, u32 offset)
{
	u32 code = 0x6800;
	code += dest + (src << 3) + (offset << 6);
	return code;
}

/*e*/static u32
armAdd2(u32 dest, u32 arg1)/*i;*/
{
	u32 code = 0x4400;
	code += dest + (arg1 << 3);
	return code;
}

/*e*/static u32
armAddImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x3000;
	code += val + (dest << 8);
	return code;
}

/*e*/static u32
armSub3(u32 dest, u32 arg1, u32 arg2)/*i;*/
{
	u32 code = 0x1A00;
	code += dest + (arg1 << 3) + (arg2 << 6);
	return code;
}

/*e*/static u32
armSubImm(u32 dest, u32 val)/*i;*/
{
	u32 code = 0x3800;
	code += val + (dest << 8);
	return code;
}

static u32
armLslsImm(u32 dest, u32 src, u32 val)
{
	u32 code = 0x0000;
	code += dest + (src << 3) + ((val&0X1F) << 6);
	return code;
}

static u32
armLsls(u32 dest, u32 arg1)
{
	u32 code = 0x4080;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armLsrsImm(u32 dest, u32 src, u32 val)
{
	u32 code = 0x0800;
	code += dest + (src << 3) + ((val&0X1F) << 6);
	return code;
}

static u32
armLsrs(u32 dest, u32 arg1)
{
	u32 code = 0x40C0;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armMul(u32 dest, u32 arg1)
{
	u32 code = 0x4340;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armOr(u32 dest, u32 arg1)
{
	u32 code = 0x4300;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armXor(u32 dest, u32 arg1)
{
	u32 code = 0x4040;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armAnd(u32 dest, u32 arg1)
{
	u32 code = 0x4000;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armBic(u32 dest, u32 arg1)
{
	u32 code = 0x4380;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armNot(u32 dest, u32 arg1)
{
	u32 code = 0x43C0;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armNeg(u32 dest, u32 arg1)
{
	u32 code = 0x4240;
	code += dest + (arg1 << 3);
	return code;
}

static u32
armStrOffset(u32 src, u32 dest, u32 offset)
{
	u32 code = 0x6000;
	code += src + (dest << 3) + (offset << 6);
	return code;
}

/*e*/static inline u32
armBinOp(u32 dest, u32 arg1, u32 code)/*i;*/
{
	code += dest + (arg1 << 3);
	return code;
}
/*e*/static u16*
alignTo4Bytes(u16 *ptr)/*i;*/
{
	if ((u32)ptr & 0x02) { *ptr++ = armMov(8, 8); }
	return ptr;
}

static u32
armCmp(u32 left, u32 right)
{
	u32 code = 0x4280;
	code += left + (right << 3);
	return code;
}

static u32
armCmpImm(u32 left, u32 val)
{
	u32 code = 0x2800;
	code += val + (left << 8);
	return code;
}

/*e*/static u32
armBranch(PengumContext *c, s32 imm)/*i;*/
{
	u32 code = 0xE000;
	if (imm < -1024 || imm > 1023)
	{
		printError(c, "branch out of range");
		imm = -1;
	}
	u32 encode = imm;
	code += (encode<<21)>>21;
	return code;
}


/*e*/static u32
armCond(PengumContext *c, u32 cond, s32 imm)/*i;*/
{
	u32 code = 0xD000;
	if (imm < -128 || imm > 127)
	{
		printError(c, "conditional branch out of range");
		imm = -1;
	}
	u32 encode = imm;
	code += ((encode<<24)>>24) + (cond << 8);
	return code;
}

//~ /*e*/static void
//~ printStackState(PengumContext *c)/*i;*/
//~ {
	//~ io_prints("c->stackState = "); io_printin(c->stackState);
	//~ for (u32 i = 0; i < 8; i++)
	//~ {
		//~ io_printin(c->stackSaveArea[i]);
	//~ }
//~ }

/*e*/void
printError(PengumContext *c, u8 *msg)/*i;*/
{
	io_prints("[Error] "); io_prints(msg); io_printsn(".");
	SET_FLAG(fError);
}

/*e*/static u8*
consumeNumLit(PengumContext *c, u8 *cursor)/*i;*/
{
	u8 *newCursor;
	s32 val = s2i(cursor - 1, &newCursor);
	compilePushVal(c, val);
	return newCursor;
}

/*e*/
static s32
stackEffect(PengumContext *c, s32 consume, s32 produce)/*i;*/
{
	s32 stackState = c->stackState;
	s32 retVal = 0;
	if (stackState + consume > STATE_STACK_EMPTY)
	{
		printError(c, "Stack Underflow");
		retVal = 1;
	}
	s32 newStackState = stackState + consume - produce;
	if (newStackState < STATE_STACK_FULL)
	{
		printError(c, "Stack Overflow");
		retVal = 1;
	}
	if (retVal == 0) { c->stackState = newStackState; }
	return retVal;
}

/*e*/void
callWord(PengumContext *c, u32 target, u32 consume, u32 produce)/*i;*/
{
	u32 currentStackState = c->stackState;
	if (stackEffect(c, consume, produce)) { return; }
	// if the current stack state does not match the consume we must fix it
	// push current stack state
	//~ io_printin(consume);
	//~ io_printin(produce);
	u32 stackStateAfterCall = c->stackState;
	u32 desiredCallStackState = 8 - consume;
	u32 afterCallStackState   = 8 - produce;
	u32 fixUpNeeded = 0;
	if (currentStackState != desiredCallStackState) { fixUpNeeded = 1; }
	
	if (fixUpNeeded)
	{
		//~ io_printsn("Fix Up Needed!");
		// push everything on stack
		*c->compileCursor++ =
			armPush(0xFF>>currentStackState<<currentStackState);
		// if function takes arguments, pop them
		if (desiredCallStackState != STATE_STACK_EMPTY) {
		*c->compileCursor++ =
		armPop(0xFF>>desiredCallStackState<<desiredCallStackState); }
	}
	u32 currentAddr = (u32)c->compileCursor;
	c->compileCursor += 2;
	mc_callRelative(target, currentAddr);
	if (fixUpNeeded)
	{
		// push everything returned on stack, if any
		if (afterCallStackState != STATE_STACK_EMPTY) {
		*c->compileCursor++ =
		armPush(0xFF>>afterCallStackState<<afterCallStackState);}
		// pop return values AND previous values
		*c->compileCursor++ =
		armPop(0xFF>>stackStateAfterCall<<stackStateAfterCall);
	}
}

/*e*/
void
pengum_compile(u8 *string)/*p;*/
{
	u8 *cursor = string;
	u8 byte;
	PengumContext *tmp = &c;
	PengumContext *c = tmp;

loop:
	byte = class[*cursor++] >> 2;
	switch (byte)
{
	case DIV>>2: { if(*cursor == '/') {goto comment;}
		compileDiv(c); goto executeOrContinue; }
	//~ case PRC>>2: { mc_stackMod(); goto loop; }
	case STA>>2: { compileMul(c); goto executeOrContinue; }
	//~ case LIN>>2: { mc_stackOr(); goto loop; }
	case CAR>>2: { compileReturn(c); goto executeOrContinue; }
	case TIL>>2: { compileRepeat(c); goto loop; }
	case PLU>>2: { compileAdd(c); goto executeOrContinue; }
	//~ case AMP>>2: { cursor = compileAmp(cursor); goto loop; }
case MIN>>2: { cursor = compileMinus(c, cursor); goto executeOrContinue; }
	//~ case SCO>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	//~ case COL>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	//~ case ATS>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	//~ case LPA>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	case LTH>>2: { cursor = compileLth(c, cursor); goto executeOrContinue; }
	case GTH>>2: { cursor = compileGth(c, cursor); goto executeOrContinue; }
	case EQU>>2: { cursor = compileEqu(c, cursor); goto loop; }
	//~ case LBL>>2: { advComileStub(0); goto loop; }
	case HAS>>2: { cursor = compileHas(c, cursor); goto loop; }
	//~ case TIC>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	//~ case COM>>2: { compileComma(); goto loop; }
	case RBL>>2: { cursor = compileRbl(c, cursor); goto executeOrContinue; }
	case DOL>>2: { cursor = compileDol(c, cursor); goto executeOrContinue; }
	//~ case LBR>>2: { advComileStub(77); goto loop; }
	case BNG>>2: { cursor = compileBng(c, cursor); goto executeOrContinue; }
//~ case QMK>>2: { printStackState(c); goto loop; /*break;*/ } // printMemStats();
case DIG>>2: { cursor = consumeNumLit(c, cursor); goto executeOrContinue; }
case ALP>>2: { cursor = consumeAlpha(c, cursor); goto executeOrContinue; }
	case DQO>>2: { cursor = captureStringLit(c, cursor); goto executeOrContinue; }
	case SQO>>2: { u8 myChar; cursor = consumeCharLit(cursor, &myChar);
		if (*cursor++!='\''){ printError(c,"char lit missing ending '"); }
		compilePushVal(c, myChar); goto executeOrContinue; }
case RPA>>2: { if (*cursor == '{') { closeUpParams(c);cursor++;
	} else { printError(c, "Right Paren alone"); } goto loop; }
	//~ case RBR>>2: { io_prints("Invalid starting character, aborting\n"); break; }
	case BSL>>2: { comment: while(*cursor!='\n'&&*cursor!=0){cursor++;} goto loop; }
	case WSP>>2: { goto loop; }
	case NUL>>2: { return; }
	case BAD>>2: { printError(c, "Bad input byte detected"); goto loop; }
}
	executeOrContinue:
	if (c->blocks != 0) { goto loop; }
	if (GET_FLAG(fError)) {
		CLEAR_FLAG(fError);
		resetFunctionState(c);
	} else {
		compileReturn(c);
		u32 addr = (u32)finalizeFunction(c);
		resetFunctionState(c);
		pengumMachineEnter(addr, c->stackSaveArea);
	}
	goto loop;
}

/*e*/static void
compilePushVal(PengumContext *c, s32 val)/*i;*/
{
	if (stackEffect(c, 0, 1)) { return; }
	if (val >= 0 && val <= 255)
	{
		*c->compileCursor++ = armMovImm(c->stackState, val);
	} else if (val >= -255 && val <= -1)
	{
		*c->compileCursor++ = armMovImm(c->stackState, -val);
		*c->compileCursor++ = armNeg(c->stackState, c->stackState);
	} else {
		c->compileCursor = alignTo4Bytes(c->compileCursor);
		u32 code = armLdrLit(c->stackState, 0) + 0xE0010000;
		*(u32*)c->compileCursor = code;
		c->compileCursor += 2;
		*(u32*)c->compileCursor = val;
		c->compileCursor += 2;
	}
}

/*e*/static u8*
consumeCharLit(u8 *cursor, u8 *out)/*i;*/
{
	if (cursor[0] != '\\') { *out = *cursor++; return cursor; }
	switch (cursor[1])
	{
		case '\"': *out = '\"'; break;
		case '\'': *out = '\''; break;
		case 'n' : *out = 0x0A; break;
		case 'r' : *out = 0x0D; break;
		case 't' : *out = 0x09; break;
		case '\\': *out = 0x5C; break;
		default  : *out = 0x5C; break;
	}
	return cursor + 2;
}

/*e*/static u8*
consumeStringLit(u8 *start, u32 length, u8 *out)/*i;*/
{
	u8 *end = start + length;
	while (start < end)
	{
		start = consumeCharLit(start, out++);
	}
	return out;
}

/*e*/static u8*
captureStringLit(PengumContext *c, u8 *cursor)/*i;*/
{
	// record start of string lit
	u8 *start = cursor;
	// find end
	while( (*cursor!='\"'||cursor[-1]=='\\')&&*cursor!=0){cursor++;}
	// if 0 is encountered before closing DQO, error
	if(*cursor==0){printError(c,"string lit missing ending (\")");return cursor;}
	if (c->currentWord == 0)
	{
		u32 length = cursor - start;
		if ((length+1)>(StringLitBuff_SIZE-scratch.w)){ scratch.w = 0;}
		u8 *out = &scratch.b[scratch.w];
		scratch.w = consumeStringLit(start, length, out) - scratch.b;
		scratch.b[scratch.w++] = 0;
		compilePushVal(c, (s32)out);
		// return cursor past the ending DQO
		return cursor+1;
	}
	// check for stack effect
	if (stackEffect(c, 0, 1)) { return cursor; }
	u32 length = cursor - start;
	// align cursor with nop
	c->compileCursor = alignTo4Bytes(c->compileCursor);
	// combine adr instruction and jump
	u32 code = armAdr(c->stackState, 0) + 0xE0000000;
	u32 *adrThenJump = (u32*)c->compileCursor;
	c->compileCursor += 2;
	// copy out string into instruction stream
	u8 *out = (u8*)c->compileCursor;
	u8 *end = consumeStringLit(start, length, out);
	// null terminate the string
	*end = 0;
	if (((u32)end&1) == 0) { end[1] = 0; }
	// calculate the length of the string in instructions
	u32 lengthInInstrs = (end-out + 2) / 2;
	// write out the adr and branch instruction
	*adrThenJump = code + ((lengthInInstrs-1)<<16);
	// increment the compile cursor
	c->compileCursor += lengthInInstrs;
	// return cursor past the ending DQO
	return cursor+1;
}

/*e*/s32
checkForPushedConstant(PengumContext *c)/*i;*/
{
	s32 result = -1;
	if (c->compileCursor[-4]>>11 == 0x09) { return result; }
	u32 instruction = c->compileCursor[-1];
	if (instruction>>11 != 0x04) { return result; }
	result = instruction & 0xFF;
	return result;
}

/*e*/static void
compileAdd(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	s32 pushedConst = checkForPushedConstant(c);
	if (pushedConst == -1) {
		*c->compileCursor++ = armAdd2(c->stackState, c->stackState-1);
		return;
	}
	c->compileCursor -= 1;
	*c->compileCursor++ = armAddImm(c->stackState, pushedConst);
}

/*e*/static u8*
compileMinus(PengumContext *c, u8 *cursor)/*i;*/
{
	if(class[*cursor]>>2==DIG>>2)
	{
		return consumeNumLit(c, cursor);
	}
	if (stackEffect(c, 2, 1)) { return cursor; }
	s32 pushedConst = checkForPushedConstant(c);
	if (pushedConst == -1) {
		*c->compileCursor++ =
			armSub3(c->stackState, c->stackState, c->stackState-1);
		return cursor;
	}
	c->compileCursor -= 1;
	*c->compileCursor++ = armSubImm(c->stackState, pushedConst);
	return cursor;
}

/*e*/static void
compileMul(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armMul(c->stackState, c->stackState-1);
}

/*e*/static void
compileDiv(PengumContext *c)/*i;*/
{
	callWord(c, (u32)pengumDiv, 2, 1);
}

/*e*/static void
compileBwOr(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armOr(c->stackState, c->stackState-1);
}

/*e*/static void
compileBwXor(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armXor(c->stackState, c->stackState-1);
}

/*e*/static void
compileBwAnd(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armAnd(c->stackState, c->stackState-1);
}

/*e*/static void
compileBwClear(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armBic(c->stackState, c->stackState-1);
}

/*e*/static void
compileBwLeftShift(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	s32 pushedConst = checkForPushedConstant(c);
	if (pushedConst == -1) {
		*c->compileCursor++ =
			armLsls(c->stackState, c->stackState-1);
		return;
	}
	c->compileCursor -= 1;
	*c->compileCursor++ = armLslsImm(c->stackState, c->stackState, pushedConst);
	return;
}

/*e*/static void
compileBwRightShift(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	s32 pushedConst = checkForPushedConstant(c);
	if (pushedConst == -1) {
		*c->compileCursor++ =
			armLsrs(c->stackState, c->stackState-1);
		return;
	}
	c->compileCursor -= 1;
	*c->compileCursor++ = armLsrsImm(c->stackState, c->stackState, pushedConst);
	return;
}

/*e*/static void
compileBwNot(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 1)) { return; }
	*c->compileCursor++ = armNot(c->stackState, c->stackState); 
}

/*e*/static void
compileNegate(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 1)) { return; }
	*c->compileCursor++ = armNeg(c->stackState, c->stackState); 
}

/*e*/static void
compileDup(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 2)) { return; }
	*c->compileCursor++ = armMov(c->stackState, c->stackState+1); 
}

/*e*/static void
compileOver(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 3)) { return; }
	*c->compileCursor++ = armMov(c->stackState, c->stackState+2); 
}

/*e*/static void
compileNip(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 1)) { return; }
	*c->compileCursor++ = armMov(c->stackState, c->stackState-1); 
}

/*e*/static void
compileDrop(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 0)) { return; }
}

/*e*/static void
compileSwap(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 2)) { return; }
	*c->compileCursor++ = armMov(12, c->stackState+1);
	*c->compileCursor++ = armMov(c->stackState+1, c->stackState);
	*c->compileCursor++ = armMov(c->stackState, 12);
}

/*e*/static void
compileLoadLocal(PengumContext *c, u32 local)/*i;*/
{
	if (stackEffect(c, 0, 1)) { return; }
	*c->compileCursor++ = armLdrSpOffset(c->stackState, c->localsIndex-local-1);
}

/*e*/static void
compileStore32BitWord(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 2, 0)) { return; }
	*c->compileCursor++ = armStrOffset(c->stackState-1, c->stackState-2, 0);
}

/*e*/static void
compileLoad8BitWord(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 1)) { return; }
	*c->compileCursor++ = armLdrByteOffset(c->stackState, c->stackState, 0);
}

/*e*/static void
compileLoad16BitWord(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 1)) { return; }
	*c->compileCursor++ = armLdrHwOffset(c->stackState, c->stackState, 0);
}

/*e*/static void
compileLoad32BitWord(PengumContext *c)/*i;*/
{
	if (stackEffect(c, 1, 1)) { return; }
	*c->compileCursor++ = armLdrOffset(c->stackState, c->stackState, 0);
}

/*e*/static void
compileReturn(PengumContext *c)/*i;*/
{
	//~ if (c->currentWord == 0){printError(c,"must be inside word to return"); return;}
	u32 numItemsReturned = 8 - c->stackState;
	Block *top = list_getFirst(c->blocks);
	if (top) { top->hasReturn = 1; }
	if (GET_FLAG(fHasReturn) == 0)
	{
		c->numItemsReturned = numItemsReturned;
		// update current word immediatly, for recursion
		if (c->currentWord) {
			c->currentWord->tClass |= c->numItemsReturned<<4; }
		SET_FLAG(fHasReturn);
	} else if (numItemsReturned != c->numItemsReturned) {
		printError(c,
		"Return number of parameters does not match other returns");
		return;
	}
	// restore sp(arguments+locals)
	if (c->localsIndex){*c->compileCursor++ = armSpAddImm(c->localsIndex);}
	*c->compileCursor++ = armPop(0x100);	// pop pc	
}

/*e*/static void
compileRepeat(PengumContext *c)/*i;*/
{
	
	if (c->currentWord == 0){printError(c,"must be inside word to repeat"); return;}
	u32 numItemsNeeded  = c->currentWord->tClass & 0x0F;
	u32 numItemsOnStack = 8 - c->stackState;
	if (numItemsNeeded != numItemsOnStack) {
	printError(c,"num items on stack does not match params for repeat"); return; }
	Block *top = list_getFirst(c->blocks);
	top->hasRepeat = 1;
	// jump to parameter consumption
	if (c->localsIndex){*c->compileCursor++ = armSpAddImm(c->localsIndex);}
	u16 *branchLocation = c->compileCursor++;
	*branchLocation = armBranch(c, (c->compileBase+1) - branchLocation - 2);
}

/*e*/static u8*
compileLth(PengumContext *c, u8 *cursor)/*i;*/
{
	if(*cursor == '{') {
		// less than comparator
		cursor++;
		compileCond(c, COND_BGE);
		return cursor;
	}
	if(*cursor == '<') {
		// bitwise shift left
		cursor++;
		compileBwLeftShift(c);
		return cursor;
	}
	if(*cursor == '=') {
		// less than or equals
		if(cursor[1] == '{')
		{
			cursor+=2;
			compileCond(c, COND_BGT);
			return cursor;
		}
	}
	printError(c, "invalid sequence starting with '<'");
	return cursor;
}

/*e*/static u8*
compileGth(PengumContext *c, u8 *cursor)/*i;*/
{
	if(*cursor == '{') {
		// greater than comparator
		cursor++;
		compileCond(c, COND_BLE);
		return cursor;
	}
	if(*cursor == '>') {
		// bitwise shift right
		cursor++;
		compileBwRightShift(c);
		return cursor;
	}
	if(*cursor == '=') {
		// greater than or equals
		if(cursor[1] == '{')
		{
			cursor+=2;
			compileCond(c, COND_BLT);
			return cursor;
		}
	}
	printError(c, "invalid sequence starting with '>'");
	return cursor;
}

/*e*/static u8*
compileEqu(PengumContext *c, u8 *cursor)/*i;*/
{
	if(*cursor == '{') {
		// equal to comparator
		cursor++;
		compileCond(c, COND_BNE);
		return cursor;
	}
	printError(c, "invalid sequence starting with '='");
	return cursor;
}

/*e*/static u8*
compileHas(PengumContext *c, u8 *cursor)/*i;*/
{
	if(*cursor == '{') {
		// not equal to comparator
		cursor++;
		compileCond(c, COND_BEQ);
		return cursor;
	}
	printError(c, "invalid sequence starting with '#'");
	return cursor;
}

/*e*/static void
compileWhile(PengumContext *c)/*i;*/
{
	Block *newBlock = pushNewBlock(c, BLOCK_WHILE);
	newBlock->target2 = c->compileCursor;
	newBlock->enteredStackState = c->stackState;
}

/*e*/static void
compileCond(PengumContext *c, u32 cond)/*i;*/
{
	if (stackEffect(c, 2, 0)) { return; }
	compileCmp(c);
	Block *top = list_getFirst(c->blocks);
	if (top != 0 && top->blockType == BLOCK_WHILE)
	{
		top->blockType = BLOCK_WHILE_COND;
		top->target = c->compileCursor;
		top->cond = cond;
		top->endCondStackState = c->stackState;
	} else {
		Block *newBlock = pushNewBlock(c, BLOCK_COND);
		newBlock->target = c->compileCursor;
		newBlock->cond = cond;
		newBlock->enteredStackState = c->stackState;
	}
	*c->compileCursor++ = 0;
}

/*e*/static void
compileElse(PengumContext *c)/*i;*/
{
	Block *top = list_getFirst(c->blocks);
	if (top == 0 || top->blockType != BLOCK_COND)
	{
		printError(c, "Else not matched to condition");
		return;
	}
	u16 *ifTarget = top->target;
	top->ifHasExit = top->hasReturn | top->hasRepeat;
	// the if block has an exit, no jump needed
	if (top->ifHasExit)
	{
		top->target = 0;
	} else {
		top->target = c->compileCursor;
		*c->compileCursor++ = -1;
	}
	// output if jump into else on failure
	*ifTarget = armCond(c, top->cond,
		c->compileCursor - ifTarget - 2);
	// transform block into else block
	top->blockType = BLOCK_ELSE;
	top->hasReturn = 0;
	top->hasRepeat = 0;
	top->endCondStackState = c->stackState;
	// set stack state to what it was at the point of cond
	c->stackState = top->enteredStackState;
}

/*e*/static void
compileCmp(PengumContext *c)/*i;*/
{
	s32 pushedConst = checkForPushedConstant(c);
	if (pushedConst == -1) {
		*c->compileCursor++ = armCmp(c->stackState-1, c->stackState-2);
		return;
	}
	c->compileCursor -= 1;
	*c->compileCursor++ = armCmpImm(c->stackState-1, pushedConst);
}

/*e*/static void
resetFunctionState(PengumContext *c)/*i;*/
{
	// reset state
	c->localsIndex = 0;
	c->numParams = 0;
	c->currentWord = 0;
	CLEAR_FLAG(fHasReturn);
	// destroy all locals
	tree_free(c->locals); c->locals = 0;
	// rest compile cursor
	c->compileCursor = c->compileBase + 2;
}

/*e*/static u16*
finalizeFunction(PengumContext *c)/*i;*/
{
	u16 *functionStart = c->compileBase;
	// emit function start
	functionStart[0] = armPush(0x100); // push lr
	u32 pushShiftAmt = 8 - c->numParams;
	// push arguments here!
	if (pushShiftAmt == 8)
	{
		functionStart[1] = armMov(8, 8); // issue no-op
	} else {
		functionStart[1] = armPush(0xFF>>pushShiftAmt<<pushShiftAmt);
	}
	// we do not
	//~ u16 *cursor = &functionStart[0];
	//~ while (cursor < c->compileCursor)
	//~ {
		//~ io_printhn(*cursor++);
	//~ }
	return (void*)((u32)functionStart + 1);
}

/*e*/static u8*
compileRbl(PengumContext *c, u8 *cursor)/*i;*/
{
	if (cursor[0] == '{')
	{
		// handle else statement
		compileElse(c);
		return cursor + 1;
	}
	//~ if(    (cursor[0] == 'e')
		//~ && (cursor[1] == 'l')
		//~ && (cursor[2] == 's')
		//~ && (cursor[3] == 'e')
		//~ && (cursor[4] == '{') )
	//~ {
		//~ // handle else statement
		//~ compileElse();
		//~ return cursor + 5;
	//~ }
	endBlock(c);
	return cursor;
}

/*e*/static u8*
compileBng(PengumContext *c, u8 *cursor)/*i;*/
{
	compileStore32BitWord(c);
	return cursor;
}

/*e*/static u8*
compileDol(PengumContext *c, u8 *cursor)/*i;*/
{
	if (cursor[0] == 'c')
	{
		// handle else statement
		compileLoad8BitWord(c);
		return cursor + 1;
	}
	if (cursor[0] == 'h')
	{
		// handle else statement
		compileLoad16BitWord(c);
		return cursor + 1;
	}
	compileLoad32BitWord(c);
	return cursor;
}

/*e*/static void
endBlock(PengumContext *c)/*i;*/
{
	if (c->blocks == 0) { printError(c, "unmatched '}'"); return; }
	Block *block = list_removeFirst(&c->blocks);
	u32 blockType = block->blockType;
	switch (blockType) {
case BLOCK_WORD:
{
	// button up word and save it off
	Tree *word = block->word;
	if (GET_FLAG(fInParams) != 0) {
		printError(c, "parameters never finished");
		CLEAR_FLAG(fInParams);
	}
	if (GET_FLAG(fHasReturn) == 0) {
		printError(c, "no return in function");
	}
	if (GET_FLAG(fError) != 0)
	{
		// there was an error, cancel definition
		CLEAR_FLAG(fError);
		io_prints(word->key);
		io_printsn(": canceled, there was an error.");
		tree_del(&c->globals, word->key, word->keyLen);
	} else {
		finalizeFunction(c);
		u32 wordSize = ((u32)c->compileCursor - (u32)c->compileBase);
		io_prints(word->key);
		io_prints(": defined, # bytes ");
		io_printin(wordSize);
		//~ word->tClass = c->numParams + (c->numItemsReturned<<4);
		// set up for next definition
		c->compileBase = c->compileCursor;
	}
	resetFunctionState(c);
	// set state back to repl
	c->stackState = c->replStackState;
	break;
}
case BLOCK_COND:
{
	if ((block->hasReturn|block->hasRepeat) == 0) // normal control flow
	{
		if (c->stackState != block->enteredStackState)
		{
			printError(c, "stack state at end of 'if' does not match starting state");
			break;
		}
	}
	c->stackState = block->enteredStackState;
	*block->target = armCond(c, block->cond,
		c->compileCursor - block->target - 2);
	break;
}
case BLOCK_ELSE:
{
	// at this point we have 4 possbilities
	// 1. if and else both do not have an exit
	u32 elseHasExit = block->hasReturn | block->hasRepeat;
	if (block->ifHasExit == 0 && elseHasExit == 0){
	if (c->stackState != block->endCondStackState)
	{
		printError(c, "stack state at end of 'else' does not match state at end of 'if'");
	}} else
	// 2. if and else both have an exit
	if (block->ifHasExit != 0 && elseHasExit != 0){
		block->next->hasRepeat = 1;
		break; }
	else
	// 3. if has an exit, else does not
	if (block->ifHasExit != 0){ break; }
	else {
	// 4. if does not have an exit, else does
	c->stackState = block->endCondStackState;}
	*block->target = armBranch(c, c->compileCursor - block->target - 2);
	break;
}
case BLOCK_WHILE: printError(c, "incomplete while block"); return;
case BLOCK_WHILE_COND:
{
	if ((block->hasReturn|block->hasRepeat) == 0) // normal control flow
	{
		if (c->stackState != block->enteredStackState)
		{
			printError(c, "stack state at end of 'while' does not match starting state");
			break;
		}
	}
	u16 *branchLocation = c->compileCursor++;
	*branchLocation = armBranch(c, block->target2 - branchLocation - 2);
	c->stackState = block->endCondStackState;
	*block->target = armCond(c, block->cond,
		c->compileCursor - block->target - 2);
	break;
}

default: printError(c, "unmatched '}'"); return;
	}
	free(block);
}

/*e*/static void
closeUpParams(PengumContext *c)/*i;*/
{
	if (GET_FLAG(fInParams) == 0){
		printError(c, "not inside params"); return; }
	CLEAR_FLAG(fInParams);
	c->currentWord->tClass = c->numParams;
}

/*e*/static u8*
consumeAlpha(PengumContext *c, u8 *cursor)/*i;*/
{
	u8 *start = cursor - 1;
	u8 byte = *cursor;
	while (class[byte] & WORD_BODY)
	{
		cursor++;
		byte = *cursor;
	}
	// alpha parts of words cannot end with -, it is used as a postfix operator
	while (*(cursor - 1) == '-') { cursor--; }
	u32 wordLength = cursor - start;
	
	// check for built in words
	u8 *newCursor = builtInCompileWord(c, start, wordLength);
	if (newCursor) { cursor = newCursor; goto done; }
	
	byte = class[*cursor] >> 2;
	switch (byte)
{
	case SCO>>2: { cursor++; createVar(c, start, wordLength); goto done; }
	case COL>>2: { cursor++; createConstant(c, start, wordLength); goto done; }
	//~ case ATS>>2: { cursor++; pushAddressOf(start, wordLength); goto done; }
	case LPA>>2: { cursor++; createWordFunction(c, start, wordLength); SET_FLAG(fInParams); goto done; }
	//~ case EQU>>2: { cursor++; assignVar(start, wordLength);goto done; }
	default : break;
}
	// see if we are inside params
	if (GET_FLAG(fInParams))
	{
		createParameter(c, start, wordLength);
		goto done;
	}
	Tree *word = resolveWord(c, start, wordLength);
	if (word != 0)
	{
		switch (word->type)
		{
		case WORD_FUNCTION:{
		if (word == c->currentWord && GET_FLAG(fHasReturn) == 0){
			printError(c,
		"need return prior to recursive call to calculate stack effect");
		break;}
		u32 consume = word->tClass & 0x0F;
		u32 produce = word->tClass >> 4;
		callWord(c, (u32)word->value, consume, produce);
		break;}
		case WORD_GLOBAL:
		compilePushVal(c, (s32)&word->value);
		break;
		case WORD_CONSTANT:
		compilePushVal(c, (s32)word->value);
		break;
		case WORD_LOCAL:
		compileLoadLocal(c, (u32)word->value);
		break;
		}
	}
	done:
	return cursor;
}

/*e*/static void
createVar(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	if (stackEffect(c, 1, 0)) { return; }
	if (c->blocks != 0)
	{
		s32 localNum = createLocal(c, start, length);
		if (c->blocks->next != c->blocks && localNum >= 0){
			printError(c, "locals must be defined one level deep"); return;
		}
		if (localNum >= 0){ // new local
			*c->compileCursor++ = armPush(1 << (c->stackState-1));
		} else { // update local
			localNum = -(localNum + 1);
			*c->compileCursor++ = armStrSpOffset(c->stackState-1, c->localsIndex-localNum-1);
		}
		return;
	}
	Tree *word  = createGlobalWord(c, start, length);
	word->type  = WORD_GLOBAL;
	word->value = (void*)c->stackSaveArea[c->stackState-1];
}

/*e*/static void
createConstant(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	if (c->blocks != 0) {
		printError(c, "Cannot define a constant within a block");
		return; }
	if (stackEffect(c, 1, 0)) { return; }
	Tree *word  = createGlobalWord(c, start, length);
	word->type  = WORD_CONSTANT;
	word->value = (void*)c->stackSaveArea[c->stackState-1];
}

/*e*/static Block *
pushNewBlock(PengumContext *c, u32 type)/*i;*/
{
	Block *newBlock = zalloc(sizeof(Block));
	newBlock->blockType = type;
	c->blocks = list_prepend(newBlock, c->blocks);
	return newBlock;
}

/*e*/Tree *
createGlobalWord(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	Tree *word = tree_find(c->globals, start, length);
	if (word)
	{
		io_printsn("[Warning] Word is being redefined.");
	} else {
		tree_add(&c->globals, start, length, 0);
		word = tree_find(c->globals, start, length);
	}
	return word;
}

/*e*/static void
createWordFunction(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	if (c->blocks != 0) {
		printError(c, "Cannot define a function within a block");
		return; }
	Tree *word = createGlobalWord(c, start, length);
	word->type  = WORD_FUNCTION;
	word->value  = (void*)((u32)c->compileBase+1);
	Block *newBlock = pushNewBlock(c, BLOCK_WORD);
	newBlock->word = word;
	c->currentWord = word;
	c->replStackState = c->stackState;
	c->stackState = STATE_STACK_EMPTY;
	c->flags = 0;
}

/*e*/static s32
createLocal(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	s32 localNum;
	Tree *word = tree_add(&c->locals, start, length, 0);
	if (word)
	{
		// local already exists, return as negative num
		localNum = (s32)word->value;
		localNum = -localNum - 1;
	} else {
		word = tree_find(c->locals, start, length);
		word->value = (void*)c->localsIndex++;
		localNum = (s32)word->value;
	}
	word->type = WORD_LOCAL;
	return localNum;
}

/*e*/static void
createParameter(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	c->numParams++;
	s32 localNum = createLocal(c, start, length);
	if (localNum < 0) { printError(c, "parameter name used again"); }
}

/*e*/static Tree*
resolveWord(PengumContext *c, u8 *start, u32 wordLength)/*i;*/
{
	// check for locals
	Tree *word = tree_find(c->locals, start, wordLength);
	if (word != 0) { return word; }
	word = tree_find(c->globals, start, wordLength);
	if (word == 0) { printError(c, "word definition missing"); }
	return word;
}

/*e*/void
printStackElements(s32 stackState, u32 *sp)/*p;*/
{
	u32 *cursor = sp + 7;
	while (stackState != STATE_STACK_EMPTY)
	{
		//~ io_printi(stackState);
		uart0_outByte('(');
		io_printi(*cursor--);
		uart0_outByte(')');
		stackState++;
	}
	uart0_outByte('\n');
}

/*e*/static void
compilePrintStack(PengumContext *c)/*i;*/
{
	if (c->stackState == STATE_STACK_EMPTY) {
		io_printsn("Stack is currently empty."); return; }
	// push {r0,r1,r2,r3,r4,r5,r6,r7}
	*c->compileCursor++ = 0xB4FF;
	*c->compileCursor++ = armMovImm(0, c->stackState);
	*c->compileCursor++ = armMov(1, 13);
	mc_callRelative((u32)printStackElements, (u32)c->compileCursor);
	c->compileCursor += 2;
	// pop {r0,r1,r2,r3,r4,r5,r6,r7}
	*c->compileCursor++ = 0xBCFF;
}

// Section Built In Words

/*e*/u8*
builtInCompileWord(PengumContext *c, u8 *start, u32 length)/*i;*/
{
	switch (length)
	{
		case 1:  return builtInWord1(c, start);
		case 2:  return builtInWord2(c, start);
		case 3:  return builtInWord3(c, start);
		case 4:  return builtInWord4(c, start);
		case 5:  return builtInWord5(c, start);
		case 6:  return builtInWord6(c, start);
		case 7:  return builtInWord7(c, start);
		default: return 0;
	}
}

/*e*/static u8*
builtInWord1(PengumContext *c, u8 *start)/*i;*/
{
	if(    (start[0] == 'c') )
	{
		c->stackState = STATE_STACK_EMPTY;
		return start + 1;
	}
	if(    (start[0] == '.') )
	{
		compilePrintStack(c);
		return start + 1;
	}
	return 0;
}

/*e*/static u8*
builtInWord2(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'o')
	&& (start[1] == 'r') )
	{
		compileBwOr(c);
		return start + 2;
	}
	if((start[0] == 'p')
	&& (start[1] == 's') )
	{
		callWord(c, (u32)pengumPs, 1, 0);
		return start + 2;
	}
	if((start[0] == 'p')
	&& (start[1] == 'i') )
	{
		callWord(c, (u32)pengumPi, 1, 0);
		return start + 2;
	}
	if((start[0] == 'p')
	&& (start[1] == 'h') )
	{
		callWord(c, (u32)pengumPh, 1, 0);
		return start + 2;
	}
	if((start[0] == 'p')
	&& (start[1] == 'c') )
	{
		callWord(c, (u32)pengumPc, 1, 0);
		return start + 2;
	}
	return 0;
}



/*e*/static u8*
builtInWord3(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'n')
	&& (start[1] == 'e')
	&& (start[2] == 'g') )
	{
		compileNegate(c);
		return start + 3;
	}
	if((start[0] == 'a')
	&& (start[1] == 'n')
	&& (start[2] == 'd') )
	{
		callWord(c, (u32)pengumAnd, 2, 1);
		return start + 3;
	}
	if((start[0] == 'd')
	&& (start[1] == 'u')
	&& (start[2] == 'p') )
	{
		compileDup(c);
		return start + 3;
	}
	if((start[0] == 'a')
	&& (start[1] == 'b')
	&& (start[2] == 's') )
	{
		callWord(c, (u32)pengumAbs, 1, 1);
		return start + 3;
	}
	if((start[0] == 'n')
	&& (start[1] == 'i')
	&& (start[2] == 'p') )
	{
		compileNip(c);
		return start + 3;
	}
	if((start[0] == 'p')
	&& (start[1] == 's')
	&& (start[2] == 'n') )
	{
		callWord(c, (u32)pengumPsn, 1, 0);
		return start + 3;
	}
	if((start[0] == 'p')
	&& (start[1] == 'i')
	&& (start[2] == 'n') )
	{
		callWord(c, (u32)pengumPin, 1, 0);
		return start + 3;
	}
	if((start[0] == 'p')
	&& (start[1] == 'h')
	&& (start[2] == 'n') )
	{
		callWord(c, (u32)pengumPhn, 1, 0);
		return start + 3;
	}
	if((start[0] == 'p')
	&& (start[1] == 'c')
	&& (start[2] == 'h') )
	{
		callWord(c, (u32)pengumPch, 1, 0);
		return start + 3;
	}
	if((start[0] == 'x')
	&& (start[1] == 'o')
	&& (start[2] == 'r') )
	{
		compileBwXor(c);
		return start + 3;
	}
	if((start[0] == 'm')
	&& (start[1] == 'o')
	&& (start[2] == 'd') )
	{
		callWord(c, (u32)pengumMod, 2, 1);
		return start + 3;
	}
	return 0;
}



/*e*/static u8*
builtInWord4(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'd')
	&& (start[1] == 'r')
	&& (start[2] == 'o')
	&& (start[3] == 'p') )
	{
		compileDrop(c);
		return start + 4;
	}
	if((start[0] == 'o')
	&& (start[1] == 'v')
	&& (start[2] == 'e')
	&& (start[3] == 'r') )
	{
		compileOver(c);
		return start + 4;
	}
	if((start[0] == 's')
	&& (start[1] == 'w')
	&& (start[2] == 'a')
	&& (start[3] == 'p') )
	{
		compileSwap(c);
		return start + 4;
	}
	if((start[0] == 'f')
	&& (start[1] == 'r')
	&& (start[2] == 'e')
	&& (start[3] == 'e') )
	{
		callWord(c, (u32)pengumFree, 1, 0);
		return start + 4;
	}
	return 0;
}


/*e*/static u8*
builtInWord5(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'a')
	&& (start[1] == 'l')
	&& (start[2] == 'l')
	&& (start[3] == 'o')
	&& (start[4] == 'c') )
	{
		callWord(c, (u32)pengumZalloc, 1, 1);
		return start + 5;
	}
	if((start[0] == 'w')
	&& (start[1] == 'h')
	&& (start[2] == 'i')
	&& (start[3] == 'l')
	&& (start[4] == 'e') )
	{
		compileWhile(c);
		return start + 5;
	}
	return 0;
}

/*e*/static u8*
builtInWord6(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'r')
	&& (start[1] == 'e')
	&& (start[2] == 'b')
	&& (start[3] == 'o')
	&& (start[4] == 'o')
	&& (start[5] == 't') )
	{
		mc_callRelative((u32)REBOOT, (u32)c->compileCursor);
		c->compileCursor += 2;
		return start + 6;
	}
	if((start[0] == 'b')
	&& (start[1] == 'w')
	&& (start[2] == '-')
	&& (start[3] == 'a')
	&& (start[4] == 'n')
	&& (start[5] == 'd') )
	{
		compileBwAnd(c);
		return start + 6;
	}
	if((start[0] == 'b')
	&& (start[1] == 'w')
	&& (start[2] == '-')
	&& (start[3] == 'c')
	&& (start[4] == 'l')
	&& (start[5] == 'r') )
	{
		compileBwClear(c);
		return start + 6;
	}
	if((start[0] == 'b')
	&& (start[1] == 'w')
	&& (start[2] == '-')
	&& (start[3] == 'n')
	&& (start[4] == 'o')
	&& (start[5] == 't') )
	{
		compileBwNot(c);
		return start + 6;
	}
	return 0;
}

/*e*/static u8*
builtInWord7(PengumContext *c, u8 *start)/*i;*/
{
	if((start[0] == 'r')
	&& (start[1] == 'e')
	&& (start[2] == 'a')
	&& (start[3] == 'l')
	&& (start[4] == 'l')
	&& (start[5] == 'o')
	&& (start[6] == 'c') )
	{
		callWord(c, (u32)pengumRealloc, 2, 1);
		return start + 7;
	}
	return 0;
}

