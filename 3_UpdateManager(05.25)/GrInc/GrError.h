/*
	standard error

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrError
#define	_EPC_GrError

//====================================================================
// uses

#include <GrLnxApp.h>
#include <GrTypeBase.h>

//====================================================================
//constance

#define E_GrErrNone								0		/* none */

#define	E_GrErrBadParam						1			/* bad parametter */
#define	E_GrErrReqSendFail				2			/* request send fail */
#define	E_GrErrObjectCreateFail		3			/* object create fail */
#define	E_GrErrDataBroken					4			/* bad data contents */
#define	E_GrErrBufFull						5			/* buffer full */
#define	E_GrErrInitFail						6			/* initialize fail */
#define	E_GrErrEndOfData					7			/* end of data */
#define	E_GrErrBusy								8			/* busy */
#define	E_GrErrFail								9			/* fail */

#define	E_GrErrBadIo							10		/* input output error */
#define	E_GrErrNotFound						11		/* data not found */
#define	E_GrErrUserCancel					12		/* stop by user */
#define	E_GrErrBadPasswd					13		/* bad password */
#define	E_GrErrPermisDenied				14		/* permission denied */

#define	E_GrErrAlreadyExist				15		/* already exist */

#define E_GrErrDiskBadSector			101		/* bad sector , b1:disk index */
#define E_GrErrDiskFull						102		/* disk full , b1: group type */
#define E_GrErrDiskStatBad				103		/* disk status bad , b1:disk index */
#define E_GrErrDiskUnformated			104		/* unformatted disk found , b1: disk type */
#define E_GrErrDiskBitLoss				105		/* not enough BIT, so loss, b1:disk index,b2:summer time, d1:time */
#define E_GrErrRecGrpDefective		106		/* record group defective , b1:group type */

#define E_GrErrBkupError					201		/* backup etc error */
#define E_GrErrBkupPlayerExist		202		/* backup player not exist */
#define E_GrErrBkupPlayerMem			203		/* backup player memory malloc failed */
#define E_GrErrBkupOddMediaBad		204		/* backup odd media bad */
#define E_GrErrBkupOddMediaSize		205		/* backup odd media size not enough size */
#define E_GrErrBkupOddFileRegist	206		/* backup odd media prepare file regist failed */
#define E_GrErrBkupOddMediaFull		207		/* backup odd media size full */
#define E_GrErrBkupOddFileIndex		208		/* backup odd file index mismatch */

//#define E_GrErrDiskMemberBad			2		/* disk group member bad , b1:disk index */
//#define E_GrErrDiskExpired				4		/* disk valid cycle end , b1:disk index */
//#define E_GrErrRecBadTimeBlk			7		/* bad time block found , b1:disk , d1:block */
//#define E_GrErrRecBadRecIdBlk			8		/* bad record id block found , b1:disk , d1:block */


//====================================================================
//global macro

//====================================================================
//global type

#if defined(_WIN64)
typedef	__s64		Def_GrErrCode;
#elif defined(_WIN32)
typedef	__s32		Def_GrErrCode;
#else
typedef	__s32		Def_GrErrCode;
#endif

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//global variable

//====================================================================

//out Environment conditional
#endif
