/*
	Rnd2team - string tools

		uses
			GrTypeBase.h


*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrStrTool
#define	_EPC_GrStrTool

//====================================================================
// uses


//====================================================================
//constance
#define		E_GrStrOutOfString						0xFFFFFFFF


//parsing mode
#define		E_GrStrParseNotUseMultiByte		0x0001
#define		E_GrStrParseNotUseQuotation		0x0100

//find mode
#define		E_GrStrFindNotUseMultiByte		0x0001

//pars
#define		E_GrStrPrsOptCharCnt					16

//korean
#define		E_GrStrUniKorCmbJamoStart			0x3131
#define		E_GrStrUniKorCmbJamoEnd				0x318E

#define		E_GrStrUniKorCodeStart				0xAC00
#define		E_GrStrUniKorCodeEnd					0xD7A3

#define		E_GrStrUniKorFirStep					0x24C		/* first step */
#define		E_GrStrUniKorLastStep					0x1C		/* last step */

#define		E_GrStrUniKorFirCnt						0x13		/* first count */
#define		E_GrStrUniKorMidCnt						0x15		/* middle count */
#define		E_GrStrUniKorLastCnt					0x1C		/* last count */

#define		E_GrStrUniKorNoMidLast				0x00		/* not exist middle or last */

//language
#define		E_GrStrLngEnglish							0
#define		E_GrStrLngJapan								1
#define		E_GrStrLngKorean							2
#define		E_GrStrLngGerman							3
#define		E_GrStrLngPrance							4
#define		E_GrStrLngRussia							5
#define		E_GrStrLngRussia							5
#define		E_GrStrLngItaly								6
#define		E_GrStrLngPortugal						7
#define		E_GrStrLngTurkey							8
#define		E_GrStrLngPoland							9

//date display type
#define		E_GrStrDateDispYyMmDd					0
#define		E_GrStrDateDispMmDdYy					1
#define		E_GrStrDateDispDdMmYy					2

// survelence
#define	E_GrStrPasswdGood					0
#define E_GrStrPasswdShort				1
#define E_GrStrPasswdSameChar			2
#define E_GrStrPasswdSeries				3
#define E_GrStrPasswdSimple				4
#define E_GrStrPasswdBadChar			5

#define E_GrStrPasswdErrCnt				5

//====================================================================
//global macro
#define		GrStrClear(A_Str)		(A_Str)[0]	=	0
//#define		GrStrIsExist(A_Str)	((BOOL)((A_Str)[0]))

#define		GrStrMaxLen(A_Str)	((sizeof((A_Str))/sizeof((A_Str)[0])) -1)

//====================================================================
//global type

typedef	struct St_GrStrPars 
{
	__u32		Last;			//last size
	char*		Ptr;			//working pointer
	__u32		DivCharCnt;
	char		DivTbl[E_GrStrPrsOptCharCnt];		//word only
	__u32		GrpCharCnt;
	char		GrpTbl[E_GrStrPrsOptCharCnt];		//word only
	__u32		IdpCharCnt;										
	char		IdpTbl[E_GrStrPrsOptCharCnt];		//word only
}	*Ptr_GrStrPars;

typedef	struct St_GrStrUniPars
{
	__u32		Last;			//last size
	WCHAR*	Ptr;			//working pointer
	__u32		DivCharCnt;
	WCHAR		DivTbl[E_GrStrPrsOptCharCnt];		//word only
	__u32		GrpCharCnt;
	WCHAR		GrpTbl[E_GrStrPrsOptCharCnt];		//word only
	__u32		IdpCharCnt;										
	WCHAR		IdpTbl[E_GrStrPrsOptCharCnt];		//word only
}	*Ptr_GrStrUniPars;

//====================================================================
//global function

BOOL8		GrStrIsExist(char* A_Str);
BOOL8		GrStrIsExist(WCHAR* A_Str);

__u32		GrStrLen(	char* A_StrSrc, __u32 A_LenLim = 0 );
__u32		GrStrWlen( WCHAR* A_StrSrc,	__u32 A_LenLim = 0 );

char*		GrStrCopy( char* A_StrDst,char* A_StrSrc,__u32 A_LenLim = 0 );		//return = next pointer
WCHAR*	GrStrWcopy( WCHAR* A_StrDst,WCHAR* A_StrSrc, __u32 A_LenLim = 0 );	//return = next pointer

__u32		GrStrParseGetWordLen( __u32 A_Mode, char* A_Str, __u32 A_LenLim = 0 );

__u32		GrStrParseFindWordStart( __u32 A_Mode, char* A_Str, __u32 A_LenLim = 0 );

__u32		GrStrGetOneLineSize( __u32 A_Mode, char* A_Str, __u32* A_PtrRtCrSize, __u32 A_LenLim = 0 );

char*		GrStrFindWordChar( char* A_StrSrc, char* A_StrDiv, char* A_StrRt );		//result = next

__s32			GrStrCmp(	char* A_StrSrc, char* A_StrDst, BOOL8 A_IsCaseSense, __u32 A_LenLim = 0 );
__s32			GrStrWcmp(	WCHAR* A_StrSrc, WCHAR* A_StrDst, BOOL8 A_IsCaseSense, __u32 A_LenLim = 0 );

__u32		GrStrFindChar( char* A_StrDst, char A_FindChar, __u32 A_Mode = 0, __u32 A_LenLim = 0 );
__u32		GrStrFindWchar( WCHAR* A_StrDst, WCHAR A_FindChar, __u32 A_LenLim = 0 );

char*		GrStrFindStr( char* A_StrDst, char* A_StrFind );
WCHAR*	GrStrFindWStr( WCHAR* A_StrDst, WCHAR* A_StrFind, BOOL8 A_IsCaseSense );

__u32		GrStrWcharCnt( WCHAR* A_Str, WCHAR A_Char, __u32 A_LenLim = 0 );	// get charactor count

char*		GrStrCat( char* A_StrDst, char* A_StrSrc, __u32 A_LenLim = 0 );		//return = next pointer
WCHAR*	GrStrWcat( WCHAR* A_StrDst, WCHAR* A_StrSrc, __u32 A_LenLim = 0 );	//return = next pointer

WCHAR*	GrStrAddWchar( WCHAR* A_StrDst, WCHAR A_Wchar, __u32 A_LenLim = 0 );	//return = next pointer

void		GrStrIns( char* A_StrDst, char* A_StrIns, __u32 A_InsPos );
void		GrStrWins( WCHAR* A_StrDst, WCHAR* A_StrIns, __u32 A_InsPos );

void		GrStrDel( char* A_StrDst, __u32 A_DelPos, __u32 A_DelSize );
void		GrStrWdel( WCHAR* A_StrDst, __u32 A_DelPos, __u32 A_DelSize );

void		GrStrChgStrSenseLow( char* A_Str );

BOOL8		GrStrFnGetDrv( char* A_StrFn, char* A_StrRtDrv, __u32 A_LenLim = 0 );

BOOL8		GrStrFnGetExt( char* A_StrFn, char* A_RtStrExt, __u32 A_LenLim = 0 );

BOOL8		GrStrFnAddBkSlsh( char* A_StrFn, __u32 A_LenLim = 0 );
BOOL8		GrStrWfnAddBkSlsh( WCHAR* A_StrFn, __u32 A_LenLim = 0 );

BOOL8		GrStrFnGetPath( char* A_StrFn, char* A_StrRtPath, __u32 A_LenLim = 0 );
BOOL8		GrStrWfnGetPath( WCHAR* A_StrFn, WCHAR* A_StrRtPath, __u32 A_LenLim = 0 );

BOOL8		GrStrFnGetNameOnly( char* A_StrFn, char* A_StrRtName, __u32 A_LenLim = 0 );
__u32		GrStrWfnGetNameOnly(WCHAR* A_StrFn, WCHAR* A_StrRtName, __u32 A_LenLim = 0);

BOOL8		GrStrFnGetFileName( char* A_StrFn, char* A_StrRtName, __u32 A_LenLim = 0 );

BOOL8		GrStrFnReverseSlash( char* A_StrFn, __u32 A_LenLim = 0 );
BOOL8		GrStrWfnReverseSlash( WCHAR* A_StrFn, __u32 A_LenLim = 0 );

BOOL8		GrStrFnIsSameExt( char* A_StrFn, char* A_StrExt );

BOOL8		GrStrIsRelative( char* A_StrFn );

BOOL8		GrStrDivideFilenameAndPara( char* A_StrSr, char* A_StrRtFn, char* A_StrRtPara );

void		GrStrTrimLeft( char* A_StrSrc, char* A_StrRt, __u32 A_LenLim = 0 );

void		GrStrTrimRight( char* A_StrDst, __u32 A_LenLim = 0 );

void		GrStrTrimBoth( char* A_StrSrc, char* A_StrRt, __u32 A_LenLim = 0 );

void		GrStrIpToStr( char* A_StrRt, __u32 A_Ip );
void		GrStrIpToWstr( WCHAR* A_StrRt, __u32 A_Ip );

BOOL8		GrStrStripToIpV4( char* A_Str, void* A_PtrRt );
BOOL8		GrStrWstripToIpV4(WCHAR* A_Str, void* A_PtrRt);

char*		GrStrMacToStr(char* A_StrRt, __u8* A_PtrMac, BOOL8 A_IsUpper, char A_Separater);
WCHAR*	GrStrMacToWstr(WCHAR* A_StrRt, __u8* A_PtrMac, BOOL8 A_IsUpper, WCHAR A_Separater);

void		GrStrStrToMacAdr(__u8* A_PtrRt, char* A_StrMac);
void		GrStrWstrToMacAdr(__u8* A_PtrRt, WCHAR* A_StrMac);

void		GrStrStripQuotation( char* A_StrSrc, char* A_StrRt, __u32 A_LenLim = 0 );

void		GrStrDressQuotation( char* A_StrSrc, char* A_StrRt, __u32 A_LenLim = 0 );

void		GrStrStripQuotationThis( char* A_Str, __u32 A_LenLim = 0 );

void		GrStrDressQuotationThis( char* A_StrDst, __u32 A_LenLim = 0 );

WCHAR*	GrStrFillSpaceWstr( WCHAR* A_Str, __u8 A_Cnt );
char*		GrStrFillSpaceStr( char* A_Str, __u8 A_Cnt );

char*		GrStrNextStrPtr( char* A_Str );
WCHAR*	GrStrNextWstrPtr( WCHAR* A_Str );

WCHAR*	GrStrSkipSpace(WCHAR* A_Str);

__u32	GrStrGetDecNumStr(WCHAR* A_Str);

//number to string
BOOL8		GrStrIntToStr( char* A_StrRt, __s32 A_Value, __u32 A_LenLim = 0, BOOL8 A_IsFix = FALSE, BOOL8 A_IsSign = TRUE );
BOOL8		GrStrIntToWstr( WCHAR* A_StrDst, __s32 A_Value, __u32 A_LenLim = 0,
											 BOOL8 A_IsFix = FALSE, BOOL8 A_IsSign = TRUE );

BOOL8		GrStrDwordToHexStr( char* A_StrRt, __u32 A_Value, __u32 A_LenLim = 0, BOOL8 A_IsFix = FALSE );
BOOL8		GrStrDwordToHexWstr( WCHAR* A_StrRt, __u32 A_Value, __u32 A_LenLim = 0, BOOL8 A_IsFix = FALSE );

BOOL8		GrStrQwordToHexStr( char* A_StrRt, __u64 A_Value, __u32 A_LenLim = 0, BOOL8 A_IsFix = FALSE );
BOOL8		GrStrQwordToHexWstr(WCHAR* A_StrRt, __u64 A_Value, __u32 A_LenLim = 0, BOOL8 A_IsFix = FALSE);

//string to number 
BOOL8    GrStrStrToInt( char* A_StrNum, __s32* A_PtrRtVal, __u32 A_LenLim = 0 );
BOOL8    GrStrWstrToInt( WCHAR* A_StrNum, __s32* A_PtrRtVal, __u32 A_LenLim = 0 );

BOOL8		GrStrHexStrToDword( char* A_StrNum, __u32* A_PtrRtVal, __u32 A_LenLim = 0 );
BOOL8		GrStrHexWstrToDword( WCHAR* A_StrNum, __u32* A_PtrRtVal, __u32 A_LenLim = 0 );

BOOL8		GrStrHexStrToQword( char* A_StrNum, __u64* A_PtrRtVal, __u32 A_LenLim = 0 );
BOOL8		GrStrHexWstrToQword(WCHAR* A_StrNum, __u64* A_PtrRtVal, __u32 A_LenLim = 0);

//string parsing
void		GrStrPrsInit( char* A_Str, Ptr_GrStrPars A_PtrPrs, 
											char* A_PtrDivTbl = NULL, char* A_PtrGrpTbl = NULL, char* A_PtrIdpTbl = NULL );

void		GrStrPrsSetStr( char* A_Str, Ptr_GrStrPars A_PtrPrs );		//change string

BOOL8		GrStrPrsLine( Ptr_GrStrPars A_PtrPrs, char* A_StrRt );

BOOL8		GrStrPrsWord( Ptr_GrStrPars A_PtrPrs, char* A_StrRt );

BOOL8		GrStrPrsIsWdDiv( Ptr_GrStrPars A_PtrPrs, char A_Chr );

BOOL8		GrStrPrsIsWdGrp( Ptr_GrStrPars A_PtrPrs, char A_Chr );		//group text

BOOL8		GrStrPrsIsWdIdp( Ptr_GrStrPars A_PtrPrs, char A_Chr );		//independent text

char*		GrStrPrsGetLast( Ptr_GrStrPars A_PtrPrs );

__u32		GrStrBase64Enc( char* A_StrSrc, char* A_StrRtEnc );

void		GrStrBase64Dec( char* A_StrSrc, char* A_StrRtDec );

void		GrStrWstrToUtf8( char* A_StrRt, WCHAR* A_StrSrc );

//string utility
void		GrStrDateDispTypeSet( __u8 A_Mode );
__u8		GrStrDateDispTypeGet( void );
void		GrStrTimeToWstr( WCHAR* A_StrRt, Def_GrTime A_Time, BOOL8 A_IsNoTime = FALSE, BOOL8 A_IsNoDate = FALSE, 
	BOOL8 A_IsNoSec = FALSE, __u8 A_GapDate = 1, __u8 A_GapTime = 1, __u8 A_DivGap = 2, BOOL8 A_IsDivider = TRUE, 
	BOOL8 A_IsFix = TRUE, BOOL8 A_IsApm = FALSE, WCHAR* A_StrAm = NULL, WCHAR* A_StrPm = NULL );
void		GrStrTimeToStr( char* A_StrRt, Def_GrTime A_Time, BOOL8 A_IsNoTime = FALSE, BOOL8 A_IsNoDate = FALSE, 
	BOOL8 A_IsNoSec = FALSE, __u8 A_GapDate = 1, __u8 A_GapTime = 1, __u8 A_DivGap = 2, BOOL8 A_IsDivider = TRUE, 
	BOOL8 A_IsFix = TRUE, BOOL8 A_IsApm = FALSE, char* A_StrAm = NULL, char* A_StrPm = NULL);

void		GrStrDateToWstr( WCHAR* A_StrRt, __u16 A_Year, __u8 A_Month, __u8 A_Day ,__u8 A_GapDate = 1 );

Def_GrTime	GrStrWstrToTime(WCHAR* A_Str);

// survelence
__u8	GrStrRulePasswdWstr( WCHAR* A_Str, __u32 A_MinCnt = 8 );

//code converting
void		GrStrWstrToStr(char* A_StrRt, WCHAR* A_StrSrc, __u32 A_Language = E_GrStrLngEnglish);
void		GrStrStrToWstr(WCHAR* A_StrRt, char* A_StrSrc, __u32 A_Language = E_GrStrLngEnglish);

//korean
void		GrStrUniKorDec( __u16 A_Code, __u8* A_PtrRtFirst, __u8* A_PtrRtMid, __u8* A_PtrRtLast );
__u16		GrStrUniKorEnc( __u8 A_First, __u8 A_Mid, __u8 A_Last );
__u8		GrStrUniKorJamoToFirst( __u16 A_Code );
__u8		GrStrUniKorJamoToMiddle( __u16 A_Code );
__u8		GrStrUniKorJamoToLast( __u16 A_Code );

//====================================================================
//out Enviroment conditional
#endif
