/*
	GAUSE Platform assembly base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3AsmBase
#define	_EPC_GrP3AsmBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP3AsmIdNone						0
#define E_GrP3AsmIdBase						1

// code
#define E_GrP3AsmMaxIntRegCnt			0x20	/* max integer register count */
#define E_GrP3AsmMaxTxtRegCnt			4			/* max text register count */

#define E_GrP3AsmTxtCharCnt				256		/* text register max character size */

#define E_GrP3AsmFlagCnt					8
#define E_GrP3AsmCallMaxCnt				8

#define E_GrP3AsmRegIdxRslt				0x00	/* register index result  */
#define E_GrP3AsmRegIdxPara				0x01	/* register index parameter  */
#define E_GrP3AsmRegIdxLocal			0x09	/* register index local */

#define E_GrP3AsmRegLocalCnt			22		/* local register count */

// #define E_GrP3AsmOpMaxCnt			0x0D	/* max opcode count */
#define	E_GrP3AsmOpUnitSize				0x10	/* opcode standard size */
#define	E_GrP3AsmOpUnitShft				0x04	/* opcode size shift */

#define E_GrP3AsmOpBrkPntMask			0x80	/* break pointer */
#define E_GrP3AsmOpDeactMask			0x40	/* off */
#define E_GrP3AsmOpCodeMask				0x3F	/* op code mask */

// argument type
#define	E_GrP3AsmArgTypeAtbMask		0xF0	/* argument type attribute mask */
#define	E_GrP3AsmArgTypeAtbConst	0x00	/* argument type attribute constant */
#define E_GrP3AsmArgTypeAtbReg		0x10	/* argument type attribute integer register */
#define E_GrP3AsmArgTypeAtbGvs		0x20	/* argument type attribute Global variable system area */
#define E_GrP3AsmArgTypeAtbGvu		0x30	/* argument type attribute Global variable user area */
#define E_GrP3AsmArgTypeAtbSetup	0x40	/* argument type attribute setup */

#define	E_GrP3AsmArgTypeSzMask		0x03	/* argument type size mask */
#define	E_GrP3AsmArgTypeSzByte		0x00	/* argument type size byte */
#define	E_GrP3AsmArgTypeSzWord		0x01	/* argument type size __u16 */
#define	E_GrP3AsmArgTypeSzDword		0x02	/* argument type size __u32 */
// #define	E_GrP3AsmArgTypeSzPtr			0x03	/* argument type size Pointer */

// condition code
#define E_GrP3AsmCondTypeEq			0x00	/* equal */
#define E_GrP3AsmCondTypeNe			0x01	/* not equal */
#define E_GrP3AsmCondTypeGt			0x02	/* left greater then right */
#define E_GrP3AsmCondTypeLt			0x03	/* left little then right */
#define E_GrP3AsmCondTypeGe			0x04	/* left greater then right or equal */
#define E_GrP3AsmCondTypeLe			0x05	/* left little then right or equal */
#define E_GrP3AsmCondTypeAnd		0x06	/* logical and */
#define E_GrP3AsmCondTypeOr			0x07	/* logical or */

// flag update type
#define E_GrP3AsmUpdtFgDirect		0x00	/* direct update */
#define E_GrP3AsmUpdtFgOr				0x01	/* or update */
#define E_GrP3AsmUpdtFgAnd			0x02	/* and update */

// flag setting type
#define E_GrP3AsmSetFgClear			0x00	/* clear flag */
#define E_GrP3AsmSetFgSet				0x01	/* set flag */
#define E_GrP3AsmSetFgReverse		0x02	/* reverse flag */

// calculator code
#define E_GrP3AsmCalcAdd				0x00	/* add */
#define E_GrP3AsmCalcSub				0x01	/* subtract */
#define E_GrP3AsmCalcMul				0x02	/* multiple */
#define E_GrP3AsmCalcDiv				0x03	/* divide */
#define E_GrP3AsmCalcRem				0x04	/* remainder */
#define E_GrP3AsmCalcAnd				0x05	/* bit and */
#define E_GrP3AsmCalcOr					0x06	/* bit or */
#define E_GrP3AsmCalcXor				0x07	/* bit XOR */
#define E_GrP3AsmCalcShl				0x08	/* shift left */
#define E_GrP3AsmCalcShr				0x09	/* shift right */

// opcode
#define E_GrP3AsmOpNop					0x00	/* no operation */
#define E_GrP3AsmOpEnd					0x01	/* quit */
#define E_GrP3AsmOpCond					0x02	/* check condition */
#define E_GrP3AsmOpMove					0x03	/* move data */
#define E_GrP3AsmOpFlag					0x04	/* flag setting */
#define E_GrP3AsmOpCalc					0x05	/* calculation */
#define E_GrP3AsmOpNot					0x06	/* bit or logical not */
#define E_GrP3AsmOpDirJump			0x07	/* jump to function direct */
#define E_GrP3AsmOpRefJump			0x08	/* jump to function reference */
#define E_GrP3AsmOpRelJump			0x09	/* jump relative */
#define E_GrP3AsmOpOfsJump			0x0A	/* jump offset */
#define E_GrP3AsmOpTxtClear			0x0B	/* text clear */
#define E_GrP3AsmOpTxtIdGet			0x0C	/* text copy from text ID */
#define E_GrP3AsmOpTxtDec				0x0D	/* decimal text add */
#define E_GrP3AsmOpTxtHex				0x0E	/* hexadecimal text add */
#define E_GrP3AsmOpTxtChar			0x0F	/* one character add */
#define E_GrP3AsmOpTxtCopy			0x10	/* text register copy */
#define E_GrP3AsmOpTxtSetStp		0x11	/* text register to setup area */
#define E_GrP3AsmOpTxtGetStp		0x12	/* text register add setup text */
#define E_GrP3AsmOpSetupCopy		0x13	/* setup area copy */
#define E_GrP3AsmOpSetupFill		0x14	/* setup area fill */
#define E_GrP3AsmOpWindow				0x15	/* window create */
#define E_GrP3AsmOpUpi					0x16	/* UPI */
#define E_GrP3AsmOpApi					0x17	/* API */
#define E_GrP3AsmOpNotify				0x18	/* NOTIFY */
#define E_GrP3AsmOpQuery				0x19	/* QUERY */
#define E_GrP3AsmOpTxtTime			0x1A	/* time to text */
#define E_GrP3AsmOpTxtCmp				0x1B	/* compare text */
#define E_GrP3AsmOpTxtLen				0x1C	/* get text length */
#define E_GrP3AsmOpWait					0x1D	/* wait */
#define E_GrP3AsmOpLabel				0x1E	/* label */
#define E_GrP3AsmOpLblJump			0x1F	/* jump to label */
#define E_GrP3AsmOpReturn				0x20	/* return */
#define E_GrP3AsmOpDcall				0x21	/* Dispatcher call */
#define E_GrP3AsmOpStpGet				0x22	/* setup get to register */
#define E_GrP3AsmOpStpPut				0x23	/* setup put from register */

//====================================================================
//global macro

//====================================================================
//global type
// ------------------------------

typedef struct	St_GrP3AsmCallStack
{
	__u16	Cs;
	__u16	Ip;
}	*Ptr_GrP3AsmCallStack;

typedef struct	St_GrP3AsmVm
{
	BOOL8	IsRun;												// now running
	__u8	Flag;													// flag
	__u8	Sp;														// call stack position
	__u8	ConIdx;												// console index

	BOOL8	IsModalWait;									// modal wait
	BOOL8	IsOpWait;											// opcode wait
	__u8	_rsvd1;
	__u8	_rsvd2;

	__u32	WaitTick;
	__u32	WaitRamain;

	__u16	Ip;														// instruction pointer
	__u16	Cs;														// code segment

	__u16	HostScptId;										// host script id
	__u16	HostWinId;										// host window id

	__s32		RegIntTbl[E_GrP3AsmMaxIntRegCnt];		// integer register table
	WCHAR	RegTxtTbl[E_GrP3AsmMaxTxtRegCnt][E_GrP3AsmTxtCharCnt];	// text register table

	St_GrP3AsmCallStack	CallStack[E_GrP3AsmCallMaxCnt];	// call stack

}	*Ptr_GrP3AsmVm;


// ------------------------------
// op code
typedef	struct	St_GrP3AsmOpBase
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpBase;

typedef	struct	St_GrP3AsmOpNop
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	WCHAR	StrRmk[6];		// remark
}	*Ptr_GrP3AsmOpNop;

typedef	struct	St_GrP3AsmOpCond
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	CondCode;
	__u8	FlagIdxRslt;
	__u8	ArgTypeLeft;
	__u8	ArgTypeRight;
	__u8	FlagUpdtType;
	__u32	ArgLeftVal;
	__u32	ArgRightVal;
}	*Ptr_GrP3AsmOpCond;

typedef	struct St_GrP3AsmOpMove
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	SrArgType;
	__u8	TgArgType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	SrArgVal;
	__u32	TgArgVal;
}	*Ptr_GrP3AsmOpMove;

typedef	struct St_GrP3AsmOpFlag
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	SetType;
	__u8	ArgType;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpFlag;

typedef	struct St_GrP3AsmOpCalc
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	SrArgType;
	__u8	TgArgType;
	__u8	CalcCode;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	SrArgVal;
	__u32	TgArgVal;
}	*Ptr_GrP3AsmOpCalc;

typedef	struct St_GrP3AsmOpNot
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	SrArgType;
	__u8	TgArgType;
	BOOL8	IsLogical;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	SrArgVal;
	__u32	TgArgVal;
}	*Ptr_GrP3AsmOpNot;

typedef	struct St_GrP3AsmOpDirJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	IsCall;
	__u16	FuncId;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
}	*Ptr_GrP3AsmOpDirJump;

typedef	struct St_GrP3AsmOpRefJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	BOOL8	IsCall;
	__u32	ArgVal;
	__u8	ArgType;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	_rsvd2;
}	*Ptr_GrP3AsmOpRefJump;

typedef	struct St_GrP3AsmOpRelJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	BOOL8	IsRvs;
	__u16	Count;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
}	*Ptr_GrP3AsmOpRelJump;

typedef	struct St_GrP3AsmOpOfsJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	__u16	Pos;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpOfsJump;

typedef	struct St_GrP3AsmOpTxtClear
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtClear;

typedef	struct St_GrP3AsmOpTxtIdGet
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u8	ArgType;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtIdGet;

typedef	struct St_GrP3AsmOpTxtDec
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u8	ArgType;
	BOOL8	IsFix;
	__u8	Digit;
	BOOL8	IsSign;
	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtDec;

typedef	struct St_GrP3AsmOpTxtHex
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u8	ArgType;
	BOOL8	IsFix;
	__u8	Digit;
	__u8	_rsvd0;
	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtHex;

typedef	struct St_GrP3AsmOpTxtChar
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	WCHAR	Char;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtChar;

typedef	struct St_GrP3AsmOpTxtCopy
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	TgRegIdx;
	__u8	SrRegIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtCopy;

typedef	struct St_GrP3AsmOpTxtSetStp
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	TregIdx;
	__u8	AddRegIdx;
	BOOL8	IsAdd;
	__u16	AdrSetup;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtSetStp;

typedef	struct St_GrP3AsmOpTxtGetStp
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	TregIdx;
	__u8	AddRegIdx;
	BOOL8	IsAdd;
	__u16	AdrSetup;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtGetStp;

typedef	struct St_GrP3AsmOpSetupCopy
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	SrRegIdx;
	__u8	TgRegIdx;
	BOOL8	IsSrAdd;
	__u16	AdrSr;
	__u16	AdrTg;
	__u16	Size;
	BOOL8	IsTgAdd;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP3AsmOpSetupCopy;

typedef	struct St_GrP3AsmOpSetupFill
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	AddRegIdx;
	__u8	FillRegIdx;
	BOOL8	IsAdd;
	__u16	AdrSetup;
	__u16	Size;
	__u16	_rsvd1;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpSetupFill;

typedef	struct St_GrP3AsmOpWindow
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u16	ScptId;
	__u16	_rsvd0;

	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpWindow;

typedef	struct St_GrP3AsmOpUpi
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u16	UpiCmd;
	__u16	WinScptId;

	__u16	ObjScptId;
	__u16	_rsvd0;

	__u32	_rsvd4;
}	*Ptr_GrP3AsmOpUpi;

typedef	struct St_GrP3AsmOpApi
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	__u16	ApiCmd;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	_rsvd3;
	__u32	_rsvd4;
}	*Ptr_GrP3AsmOpApi;

typedef	struct St_GrP3AsmOpNotify
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u8	ConTxtReg;
	__u8	Time;
	__u16	TitleTxtId;

	__u32	_rsvd3;
	__u32	_rsvd4;
}	*Ptr_GrP3AsmOpNotify;

typedef	struct St_GrP3AsmOpQuery
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u8	ConTxtReg;
	__u8	Type;
	__u16	TitleTxtId;

	__u32	_rsvd3;
	__u32	_rsvd4;
}	*Ptr_GrP3AsmOpQuery;

typedef	struct St_GrP3AsmOpTxtTime
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u8	ArgType;
	BOOL8	IsNoTime;
	BOOL8	IsNoDate;
	BOOL8	IsNoSec;
	__u32	ArgVal;
	__u8	DayGap;
	__u8	TimeGap;
	__u8	DivGap;
	BOOL8	IsNoDiv;
}	*Ptr_GrP3AsmOpTxtTime;

typedef	struct St_GrP3AsmOpTxtCmp
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	Treg1Idx;
	__u8	Treg2Idx;
	__u8	FlagIdxRslt;
	__u8	FlagUpdtType;
	BOOL8	IsCaseSense;
	__u32	_rsvd0;
	__u32	_rsvd1;
}	*Ptr_GrP3AsmOpTxtCmp;

typedef	struct St_GrP3AsmOpTxtLen
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	TregIdx;
	__u8	RegIdx;
	__u8	_rsvd0;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpTxtLen;

typedef	struct St_GrP3AsmOpWait
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	Time;			// waiting time - sec unit
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpWait;

typedef	struct St_GrP3AsmOpLabel
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u32	Name0;
	__u32	Name1;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpLabel;

typedef	struct St_GrP3AsmOpLblJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u32	Name0;
	__u32	Name1;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpLblJump;

typedef	struct St_GrP3AsmOpDcall
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u16	CodeId;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpDcall;

typedef	struct St_GrP3AsmOpStpGet
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	AdrReg;			
	__u8	Type;
	__u8	ValReg;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpStpGet;

typedef	struct St_GrP3AsmOpStpPut
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	AdrReg;			
	__u8	Type;
	__u8	ValReg;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP3AsmOpStpPut;

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

