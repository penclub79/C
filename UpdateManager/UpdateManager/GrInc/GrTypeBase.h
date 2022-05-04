//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrTypeBase
#define	_EPC_GrTypeBase

//====================================================================
// uses

#ifdef LINUX_APP

#include <asm/types.h>

#endif


//====================================================================
//constance

#if defined(_WIN32) || defined(_WIN64)
#define	__gccna
#else
#define	__gccna	__attribute__((packed))
#endif

#ifdef _WIN64

#define	GR_64BIT

#endif

#ifdef GR_64BIT

#define	Def_GrCalPtr		__u64

#else

#define	Def_GrCalPtr		__u32

#endif


#if defined(_WIN32) && !defined(_WIN64)

#define VOID void
typedef	void	*PVOID;

#endif

#ifndef	NULL
#define	NULL	0
#endif

#define TRUE	1
#define FALSE	0

#define	E_GrFnFullPathLen			1024
#define	E_GrFnLongLen					128
#define	E_GrFnLongWithPathLen	256
#define	E_GrFnShortPathLen		128
#define	E_GrFnShortLen				16

#define E_GrNumStrLen					16
#define E_GrNameStrLen				32
#define E_GrTagStrLen					64
#define	E_GrShortStrLen				256
#define	E_GrLongStrLen				1024
#define	E_GrHugeStrLen				4096

// variable size
#define	E_GrVarSize1byte				0
#define	E_GrVarSize2byte				1
#define	E_GrVarSize4byte				2

// align
#define	E_GrAlignLeft						0
#define	E_GrAlignRight					1
#define	E_GrAlignCenter					2
#define	E_GrAlignTop						0
#define	E_GrAlignBottom					1

// direction
#define	E_GrWayLeft							0
#define	E_GrWayUp								1
#define	E_GrWayRight						2
#define	E_GrWayDown							3

// variable attribute
#define E_GrVarLenVar									0
#define E_GrVarLen1Byte								1
#define E_GrVarLen2Byte								2
#define E_GrVarLen4Byte								3
#define E_GrVarLen8Byte								4

#define	E_GrVarAtbBitPos							3

#define E_GrVarAtbUnsigned						(0x0000 << E_GrVarAtbBitPos)		/* unsigned number */
#define E_GrVarAtbSigned							(0x0001 << E_GrVarAtbBitPos)		/* signed number */
#define E_GrVarAtbHex									(0x0002 << E_GrVarAtbBitPos)		/* hex unsigned number */
#define E_GrVarAtbStr									(0x0003 << E_GrVarAtbBitPos)		/* text */
#define E_GrVarAtbWstr								(0x0004 << E_GrVarAtbBitPos)		/* unicode text */
#define E_GrVarAtbBool								(0x0005 << E_GrVarAtbBitPos)		/* bool */
#define E_GrVarAtbBitmap0							(0x0006 << E_GrVarAtbBitPos)		/* bitmap 0*/
#define E_GrVarAtbBitmap1							(0x0007 << E_GrVarAtbBitPos)		/* bitmap 1*/
#define E_GrVarAtbColor								(0x0008 << E_GrVarAtbBitPos)		/* color */
#define E_GrVarAtbListFixDec					(0x0009 << E_GrVarAtbBitPos)		/* list fix decimal */
#define E_GrVarAtbListFixHex					(0x000A << E_GrVarAtbBitPos)		/* list fix hex */
#define E_GrVarAtbListRangeDec				(0x000B << E_GrVarAtbBitPos)		/* list range decimal */
#define E_GrVarAtbListRangeHex				(0x000C << E_GrVarAtbBitPos)		/* list range hex */
#define E_GrVarAtbIp									(0x000D << E_GrVarAtbBitPos)		/* ip address */
#define E_GrVarAtbBitBool							(0x000E << E_GrVarAtbBitPos)		/* bit bool */
#define E_GrVarAtbPallete							(0x000F << E_GrVarAtbBitPos)		/* pallete */
#define E_GrVarAtbTime								(0x0010 << E_GrVarAtbBitPos)		/* time */
#define E_GrVarAtbAlignHz							(0x0011 << E_GrVarAtbBitPos)		/* align horizontal */
#define E_GrVarAtbAlignVt							(0x0012 << E_GrVarAtbBitPos)		/* align vertical */
#define E_GrVarAtbDirection						(0x0013 << E_GrVarAtbBitPos)		/* direction */
#define E_GrVarAtbVarSize							(0x0014 << E_GrVarAtbBitPos)		/* variable size */
#define E_GrVarAtbMacAdr							(0x0015 << E_GrVarAtbBitPos)		/* mac address */

#define	E_GrVarAtbUserStart						0x1000

// var type
#define	E_GrVarTypeNone							0		/* none */

#define	E_GrVarTypeS8								(E_GrVarAtbSigned | E_GrVarLen1Byte)
#define	E_GrVarTypeS16							(E_GrVarAtbSigned | E_GrVarLen2Byte)
#define	E_GrVarTypeS32							(E_GrVarAtbSigned | E_GrVarLen4Byte)	
#define	E_GrVarTypeS64							(E_GrVarAtbSigned | E_GrVarLen8Byte)	

#define	E_GrVarTypeU8								(E_GrVarAtbUnsigned | E_GrVarLen1Byte)
#define	E_GrVarTypeU16							(E_GrVarAtbUnsigned | E_GrVarLen2Byte)
#define	E_GrVarTypeU32							(E_GrVarAtbUnsigned | E_GrVarLen4Byte)
#define	E_GrVarTypeU64							(E_GrVarAtbUnsigned | E_GrVarLen8Byte)

#define	E_GrVarTypeHexU8						(E_GrVarAtbHex | E_GrVarLen1Byte)
#define	E_GrVarTypeHexU16						(E_GrVarAtbHex | E_GrVarLen2Byte)
#define	E_GrVarTypeHexU32						(E_GrVarAtbHex | E_GrVarLen4Byte)
#define	E_GrVarTypeHexU64						(E_GrVarAtbHex | E_GrVarLen8Byte)

#define	E_GrVarTypeStr							(E_GrVarAtbStr | E_GrVarLenVar)			/* char string */

#define	E_GrVarTypeWchar						(E_GrVarAtbWstr | E_GrVarLen2Byte)		/* wchar one */
#define	E_GrVarTypeWstr							(E_GrVarAtbWstr | E_GrVarLenVar)		/* wchar string */

#define	E_GrVarTypeBool8						(E_GrVarAtbBool | E_GrVarLen1Byte)

#define	E_GrVarTypeBit0U8						(E_GrVarAtbBitmap0 | E_GrVarLen1Byte)
#define	E_GrVarTypeBit1U8						(E_GrVarAtbBitmap1 | E_GrVarLen1Byte)

#define	E_GrVarTypeColor						(E_GrVarAtbColor | E_GrVarLen4Byte)

#define	E_GrVarTypePallete					(E_GrVarAtbPallete | E_GrVarLen4Byte)

#define	E_GrVarTypeIpV4							(E_GrVarAtbIp | E_GrVarLen4Byte)

#define	E_GrVarTypeMacAdr						(E_GrVarAtbMacAdr | E_GrVarLen8Byte)

#define	E_GrVarTypeLstFixDecU8			(E_GrVarAtbListFixDec | E_GrVarLen1Byte)
#define	E_GrVarTypeLstFixDecU16			(E_GrVarAtbListFixDec | E_GrVarLen2Byte)
#define	E_GrVarTypeFixFixDecU32			(E_GrVarAtbListFixDec | E_GrVarLen4Byte)

#define	E_GrVarTypeLstFixHexU8			(E_GrVarAtbListFixHex | E_GrVarLen1Byte)
#define	E_GrVarTypeLstFixHexU16			(E_GrVarAtbListFixHex | E_GrVarLen2Byte)
#define	E_GrVarTypeFixFixHexU32			(E_GrVarAtbListFixHex | E_GrVarLen4Byte)

#define E_GrVarTypeBitBoolBit0			(E_GrVarAtbBitBool | 0)
#define E_GrVarTypeBitBoolBit1			(E_GrVarAtbBitBool | 1)
#define E_GrVarTypeBitBoolBit2			(E_GrVarAtbBitBool | 2)
#define E_GrVarTypeBitBoolBit3			(E_GrVarAtbBitBool | 3)
#define E_GrVarTypeBitBoolBit4			(E_GrVarAtbBitBool | 4)
#define E_GrVarTypeBitBoolBit5			(E_GrVarAtbBitBool | 5)
#define E_GrVarTypeBitBoolBit6			(E_GrVarAtbBitBool | 6)
#define E_GrVarTypeBitBoolBit7			(E_GrVarAtbBitBool | 7)

#define	E_GrVarTypeAlignHz					(E_GrVarAtbAlignHz | E_GrVarLen1Byte)
#define	E_GrVarTypeAlignVt					(E_GrVarAtbAlignVt | E_GrVarLen1Byte)

#define	E_GrVarTypeDirection				(E_GrVarAtbDirection | E_GrVarLen1Byte)

#define	E_GrVarTypeTime							(E_GrVarAtbTime | E_GrVarLen4Byte)

#define	E_GrVarTypeVarSize					(E_GrVarAtbVarSize | E_GrVarLen1Byte)

//====================================================================
//global macro

#define Mac_GrMakeFcc(ch0, ch1, ch2, ch3)	\
				((__u32)(__u8)(ch0) | ((__u32)(__u8)(ch1) << 8) | \
				((__u32)(__u8)(ch2) << 16) | ((__u32)(__u8)(ch3) << 24 ))

#define Mac_GrMakeTcc(ch0, ch1)	\
	((__u16)(__u8)(ch0) | ((__u16)(__u8)(ch1) << 8))

//====================================================================
//global type

#ifndef	_WINDEF_
#undef BOOL
typedef int									BOOL;

#ifdef LINUX_DRV
	typedef unsigned short			WCHAR;
#else
#ifdef LINUX_APP
#ifdef DFLT_WCHAR
	typedef wchar_t	WCHAR;
#else
	typedef unsigned short			WCHAR;
#endif
#else
	typedef wchar_t	WCHAR;
#endif
#endif

#endif

	// new standard type
#if defined(_WIN32) || defined(_WIN64)
 typedef unsigned __int8 __u8;
 typedef unsigned __int16 __u16;
 typedef unsigned __int32 __u32;
 typedef unsigned __int64 __u64;
 typedef __int8 __s8;
 typedef __int16 __s16;
 typedef __int32 __s32;
 typedef __int64 __s64;
#endif

	typedef	__u8	BOOL8;

	//file name
	typedef	char	Def_StrGrFnPath[E_GrFnLongWithPathLen];
	typedef	char	Def_StrGrFnPathLong[E_GrFnFullPathLen];
	typedef	char	Def_StrGrFnPathShort[E_GrFnShortPathLen];

	typedef	WCHAR	Def_WstrGrFnPath[E_GrFnLongWithPathLen];
	typedef	WCHAR	Def_WstrGrFnPathLong[E_GrFnFullPathLen];
	typedef	WCHAR	Def_WstrGrFnPathShort[E_GrFnShortPathLen];

	//standard string
	typedef	char	Def_StrNum[E_GrNumStrLen];
	typedef	char	Def_StrName[E_GrNameStrLen];
	typedef	char	Def_StrTag[E_GrTagStrLen];
	typedef	char	Def_StrShort[E_GrShortStrLen];
	typedef	char	Def_StrLong[E_GrLongStrLen];
	typedef	char	Def_StrHuge[E_GrHugeStrLen];

	typedef	WCHAR	Def_WstrNum[E_GrNumStrLen];
	typedef	WCHAR	Def_WstrName[E_GrNameStrLen];
	typedef	WCHAR	Def_WstrTag[E_GrTagStrLen];
	typedef	WCHAR	Def_WstrShort[E_GrShortStrLen];
	typedef	WCHAR	Def_WstrLong[E_GrLongStrLen];
	typedef	WCHAR	Def_WstrHuge[E_GrHugeStrLen];

	//time
	typedef	__u32	Def_GrTime, *Ptr_GrTime;

	typedef	struct St_GrTimeRng
	{
		Def_GrTime	TimeSt;			//start time
		Def_GrTime	TimeEd;			//end time
	}	*Ptr_GrTimeRng;

	typedef	struct	St_GrDate
	{
		__u8		Day;
		__u8		Month;
		__u16		Year;
	}	*Ptr_GrDate;

	//physical memory mapper
	typedef	struct St_GrPhyMemMap
	{
		__u32		Phys;			//physical address
		void*		PtrApp;		//virtual address
		void*		PtrDrv;		//driver address
		__u32		Size;			//size
	}	*Ptr_GrPhyMemMap;

	//point position
	typedef	struct	St_GrPos
	{
		__s32			x;
		__s32			y;
	}	*Ptr_GrPos;
	//rect position
	typedef	struct	St_GrRect
	{
		__s32		left;
		__s32		top;
		__s32		right;
		__s32		bottom;

	} *Ptr_GrRect;

	typedef	struct St_GrByteRect
	{
		__u8	left;
		__u8	top;
		__u8	right;
		__u8	bottom;
	}	*Ptr_GrByteRect;

	typedef	struct St_GrWordRect
	{
		__u16	left;
		__u16	top;
		__u16	right;
		__u16	bottom;
	}	*Ptr_GrWordRect;

	typedef	struct St_GrSwordRect
	{
		__s16	left;
		__s16	top;
		__s16	right;
		__s16	bottom;
	}	*Ptr_GrSwordRect;

	typedef	struct St_GrWordPos
	{
		__u16	x;
		__u16	y;
	}	*Ptr_GrWordPos;

	typedef	struct St_GrSwordPos
	{
		__s16	x;
		__s16	y;
	}	*Ptr_GrSwordPos;

	typedef	struct St_GrBytePos
	{
		__u8	x;
		__u8	y;
	}	*Ptr_GrBytePos;

	typedef	struct St_GrLinePos 
	{
		struct St_GrPos	St;		//start position
		struct St_GrPos	Ed;		//end positon
	}	*Ptr_GrLinePos;

	typedef	struct St_GrPosSz
	{
		__s32		x;
		__s32		y;
		__s32		Sx;
		__s32		Sy;
	}	*Ptr_GrPosSz;

	typedef struct St_GrMacRng
	{
		__u64	Begin;
		__u64	End;
	}	*Ptr_GrMacRng;

	typedef struct St_GrIpV4Rng
	{
		__u32	Begin;
		__u32	End;
	}	*Ptr_GrIpV4Rng;

	typedef struct St_GrU16Rng
	{
		__u16	Begin;
		__u16	End;
	}	*Ptr_GrU16Rng;

	typedef struct St_GrMemInfo
	{
		void*	Ptr;
		__u32	Size;
	}	*Ptr_GrMemInfo;

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
