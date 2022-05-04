/*
	GAUSE Platform assembly base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4AsmBase
#define	_EPC_GrP4AsmBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrP4AsmIdNone						0
#define E_GrP4AsmIdBase						1

// code
#define E_GrP4AsmMaxIntRegCnt			0x20	/* max integer register count */
#define E_GrP4AsmMaxTxtRegCnt			4			/* max text register count */

#define E_GrP4AsmTxtCharCnt				256		/* text register max character size */

#define E_GrP4AsmFlagCnt					8
#define E_GrP4AsmCallMaxCnt				8

#define E_GrP4AsmRegIdxRslt				0x00	/* register index result  */
#define E_GrP4AsmRegIdxPara				0x01	/* register index parameter  */
#define E_GrP4AsmRegIdxLocal			0x09	/* register index local */

#define E_GrP4AsmRegLocalCnt			22		/* local register count */

// #define E_GrP4AsmOpMaxCnt			0x0D	/* max opcode count */
#define	E_GrP4AsmOpUnitSize				0x10	/* opcode standard size */
#define	E_GrP4AsmOpUnitShft				0x04	/* opcode size shift */

#define E_GrP4AsmOpBrkPntMask			0x80	/* break pointer */
#define E_GrP4AsmOpDeactMask			0x40	/* off */
#define E_GrP4AsmOpCodeMask				0x3F	/* op code mask */

// running status
#define	E_GrP4AsmRunStatStop			0			/* do not run */
#define	E_GrP4AsmRunStatRun				1			/* running */
#define	E_GrP4AsmRunStatSuspend		2			/* waiting */
#define	E_GrP4AsmRunStatWait			3			/* time waiting */

// argument type
#define	E_GrP4AsmArgTypeConst					0			/* constant */
#define	E_GrP4AsmArgTypeUrgst					1			/* registry user */
#define	E_GrP4AsmArgTypeSrgstByte			2			/* registry system byte */
#define	E_GrP4AsmArgTypeSrgstWord			3			/* registry system word */
#define	E_GrP4AsmArgTypeSrgstDword		4			/* registry system dword */
#define	E_GrP4AsmArgTypeIreg					16		/* register integer */
#define	E_GrP4AsmArgTypeGvsByte				32		/* GVS byte */
#define	E_GrP4AsmArgTypeGvsWord				33		/* GVS word */
#define	E_GrP4AsmArgTypeGvsDword			34		/* GVS dword */
#define	E_GrP4AsmArgTypeGvu						48		/* GVU */
#define	E_GrP4AsmArgTypeSetupByte			64		/* Setup byte */
#define	E_GrP4AsmArgTypeSetupWord			65		/* Setup word */
#define	E_GrP4AsmArgTypeSetupDword		66		/* Setup dword */

#define	E_GrP4AsmArgTypeSzByte		0x00	/* argument type size byte */
#define	E_GrP4AsmArgTypeSzWord		0x01	/* argument type size __u16 */
#define	E_GrP4AsmArgTypeSzDword		0x02	/* argument type size __u32 */
// #define	E_GrP4AsmArgTypeSzPtr			0x03	/* argument type size Pointer */

// condition code
#define E_GrP4AsmCondTypeEq			0x00	/* equal */
#define E_GrP4AsmCondTypeNe			0x01	/* not equal */
#define E_GrP4AsmCondTypeGt			0x02	/* left greater then right */
#define E_GrP4AsmCondTypeLt			0x03	/* left little then right */
#define E_GrP4AsmCondTypeGe			0x04	/* left greater then right or equal */
#define E_GrP4AsmCondTypeLe			0x05	/* left little then right or equal */
#define E_GrP4AsmCondTypeAnd		0x06	/* logical and */
#define E_GrP4AsmCondTypeOr			0x07	/* logical or */

// flag update type
#define E_GrP4AsmUpdtFgDirect		0x00	/* direct update */
#define E_GrP4AsmUpdtFgOr				0x01	/* or update */
#define E_GrP4AsmUpdtFgAnd			0x02	/* and update */

// flag setting type
#define E_GrP4AsmSetFgClear			0x00	/* clear flag */
#define E_GrP4AsmSetFgSet				0x01	/* set flag */
#define E_GrP4AsmSetFgReverse		0x02	/* reverse flag */

// calculator code
#define E_GrP4AsmCalcAdd				0x00	/* add */
#define E_GrP4AsmCalcSub				0x01	/* subtract */
#define E_GrP4AsmCalcMul				0x02	/* multiple */
#define E_GrP4AsmCalcDiv				0x03	/* divide */
#define E_GrP4AsmCalcRem				0x04	/* remainder */
#define E_GrP4AsmCalcAnd				0x05	/* bit and */
#define E_GrP4AsmCalcOr					0x06	/* bit or */
#define E_GrP4AsmCalcXor				0x07	/* bit XOR */
#define E_GrP4AsmCalcShl				0x08	/* shift left */
#define E_GrP4AsmCalcShr				0x09	/* shift right */

// opcode
#define E_GrP4AsmOpNop					0x00	/* no operation */
#define E_GrP4AsmOpEnd					0x01	/* quit */
#define E_GrP4AsmOpCond					0x02	/* check condition */
#define E_GrP4AsmOpMove					0x03	/* move data */
#define E_GrP4AsmOpFlag					0x04	/* flag setting */
#define E_GrP4AsmOpCalc					0x05	/* calculation */
#define E_GrP4AsmOpNot					0x06	/* bit or logical not */
#define E_GrP4AsmOpDirJump			0x07	/* jump to function direct */
#define E_GrP4AsmOpRefJump			0x08	/* jump to function reference */
#define E_GrP4AsmOpRelJump			0x09	/* jump relative */
#define E_GrP4AsmOpOfsJump			0x0A	/* jump offset */
#define E_GrP4AsmOpTxtClear			0x0B	/* text clear */
#define E_GrP4AsmOpTxtIdGet			0x0C	/* text copy from text ID */
#define E_GrP4AsmOpTxtDec				0x0D	/* decimal text add */
#define E_GrP4AsmOpTxtHex				0x0E	/* hexadecimal text add */
#define E_GrP4AsmOpTxtChar			0x0F	/* one character add */
#define E_GrP4AsmOpTxtCopy			0x10	/* text register copy */
#define E_GrP4AsmOpTxtSetStp		0x11	/* text register to setup area */
#define E_GrP4AsmOpTxtGetStp		0x12	/* text register add setup text */
#define E_GrP4AsmOpSetupCopy		0x13	/* setup area copy */
#define E_GrP4AsmOpSetupFill		0x14	/* setup area fill */
#define E_GrP4AsmOpWindow				0x15	/* window create */
#define E_GrP4AsmOpUpi					0x16	/* UPI */
#define E_GrP4AsmOpApi					0x17	/* API */
#define E_GrP4AsmOpNotify				0x18	/* NOTIFY */
#define E_GrP4AsmOpQuery				0x19	/* QUERY */
#define E_GrP4AsmOpTxtTime			0x1A	/* time to text */
#define E_GrP4AsmOpTxtCmp				0x1B	/* compare text */
#define E_GrP4AsmOpTxtLen				0x1C	/* get text length */
#define E_GrP4AsmOpWait					0x1D	/* wait */
#define E_GrP4AsmOpLabel				0x1E	/* label */
#define E_GrP4AsmOpLblJump			0x1F	/* jump to label */
#define E_GrP4AsmOpReturn				0x20	/* return */
#define E_GrP4AsmOpDcall				0x21	/* Dispatcher call */
#define E_GrP4AsmOpStpGet				0x22	/* setup get to register */
#define E_GrP4AsmOpStpPut				0x23	/* setup put from register */

//====================================================================
//global macro

//====================================================================
//global type
// ------------------------------

typedef struct	St_GrP4AsmCallStack
{
	__u16	Cs;
	__u16	Ip;
}	*Ptr_GrP4AsmCallStack;

typedef struct	St_GrP4AsmVm
{
	BOOL8	Rstat;												// running status
	__u8	Flag;													// flag
	__u8	Sp;														// call stack position
	__u8	ConIdx;												// console index

	__u32	WaitRamain;										// wait mode remain

	__u16	Ip;														// instruction pointer
	__u16	Cs;														// code segment

	__u16	HostScptId;										// host script id
	__u16	HostWinId;										// host window id

	__s32		RegIntTbl[E_GrP4AsmMaxIntRegCnt];		// integer register table
	WCHAR	RegTxtTbl[E_GrP4AsmMaxTxtRegCnt][E_GrP4AsmTxtCharCnt];	// text register table

	St_GrP4AsmCallStack	CallStack[E_GrP4AsmCallMaxCnt];	// call stack

}	*Ptr_GrP4AsmVm;


// ------------------------------
// op code
typedef	struct	St_GrP4AsmOpBase
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpBase;

typedef	struct	St_GrP4AsmOpNop
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	WCHAR	StrRmk[6];		// remark
}	*Ptr_GrP4AsmOpNop;

typedef	struct	St_GrP4AsmOpCond
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
}	*Ptr_GrP4AsmOpCond;

typedef	struct St_GrP4AsmOpMove
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
}	*Ptr_GrP4AsmOpMove;

typedef	struct St_GrP4AsmOpFlag
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
}	*Ptr_GrP4AsmOpFlag;

typedef	struct St_GrP4AsmOpCalc
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
}	*Ptr_GrP4AsmOpCalc;

typedef	struct St_GrP4AsmOpNot
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
}	*Ptr_GrP4AsmOpNot;

typedef	struct St_GrP4AsmOpDirJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	IsCall;
	__u16	FuncId;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
}	*Ptr_GrP4AsmOpDirJump;

typedef	struct St_GrP4AsmOpRefJump
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
}	*Ptr_GrP4AsmOpRefJump;

typedef	struct St_GrP4AsmOpRelJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	BOOL8	IsRvs;
	__u16	Count;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd2;
}	*Ptr_GrP4AsmOpRelJump;

typedef	struct St_GrP4AsmOpOfsJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;
	__u16	Pos;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpOfsJump;

typedef	struct St_GrP4AsmOpTxtClear
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpTxtClear;

typedef	struct St_GrP4AsmOpTxtIdGet
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
}	*Ptr_GrP4AsmOpTxtIdGet;

typedef	struct St_GrP4AsmOpTxtDec
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
}	*Ptr_GrP4AsmOpTxtDec;

typedef	struct St_GrP4AsmOpTxtHex
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
}	*Ptr_GrP4AsmOpTxtHex;

typedef	struct St_GrP4AsmOpTxtChar
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	RegIdx;
	WCHAR	Char;
	__u16	_rsvd0;
	__u32	_rsvd1;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpTxtChar;

typedef	struct St_GrP4AsmOpTxtCopy
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
}	*Ptr_GrP4AsmOpTxtCopy;

typedef	struct St_GrP4AsmOpTxtSetStp
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
}	*Ptr_GrP4AsmOpTxtSetStp;

typedef	struct St_GrP4AsmOpTxtGetStp
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
}	*Ptr_GrP4AsmOpTxtGetStp;

typedef	struct St_GrP4AsmOpSetupCopy
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
}	*Ptr_GrP4AsmOpSetupCopy;

typedef	struct St_GrP4AsmOpSetupFill
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
}	*Ptr_GrP4AsmOpSetupFill;

typedef	struct St_GrP4AsmOpWindow
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u16	ScptId;
	__u16	RefWinId;

	__u32	ArgVal;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpWindow;

typedef	struct St_GrP4AsmOpUpi
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
}	*Ptr_GrP4AsmOpUpi;

typedef	struct St_GrP4AsmOpApi
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
}	*Ptr_GrP4AsmOpApi;

typedef	struct St_GrP4AsmOpNotify
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u8	ConTxtReg;
	__u8	Time;
	__u16	TitleTxtId;

	__u16	WinScptId;
	__u16	RefWinId;

	__u32	_rsvd4;
}	*Ptr_GrP4AsmOpNotify;

typedef	struct St_GrP4AsmOpQuery
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	ConIdx;

	__u8	ConTxtReg;
	__u8	Type;
	__u16	TitleTxtId;

	__u16	WinScptId;
	__u16	RefWinId;

	__u32	_rsvd4;
}	*Ptr_GrP4AsmOpQuery;

typedef	struct St_GrP4AsmOpTxtTime
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
}	*Ptr_GrP4AsmOpTxtTime;

typedef	struct St_GrP4AsmOpTxtCmp
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
}	*Ptr_GrP4AsmOpTxtCmp;

typedef	struct St_GrP4AsmOpTxtLen
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
}	*Ptr_GrP4AsmOpTxtLen;

typedef	struct St_GrP4AsmOpWait
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	Time;			// waiting time - sec unit
	__u32	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpWait;

typedef	struct St_GrP4AsmOpLabel
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u32	Name0;
	__u32	Name1;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpLabel;

typedef	struct St_GrP4AsmOpLblJump
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u32	Name0;
	__u32	Name1;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpLblJump;

typedef	struct St_GrP4AsmOpDcall
{
	__u8	FlagAble;
	__u8	FlagBan;
	__u8	Code;
	__u8	_rsvd0;			
	__u16	CodeId;
	__u16	_rsvd1;
	__u32	_rsvd2;
	__u32	_rsvd3;
}	*Ptr_GrP4AsmOpDcall;

typedef	struct St_GrP4AsmOpStpGet
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
}	*Ptr_GrP4AsmOpStpGet;

typedef	struct St_GrP4AsmOpStpPut
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
}	*Ptr_GrP4AsmOpStpPut;

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

