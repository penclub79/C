/*
scsi opr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrvDvd
#define	_EPC_GrDrvDvd

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrCdrDrv.h>
#include 	<Hw/Odd/GrSCSI.h>


//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function
__s32	format_unit(St_GrScsiIf *scgp,	void *fmt, __s32 length, __s32 list_format, __s32 dgdl,
						__s32 interlv, __s32 pattern, __s32 timeout);

//====================================================================
//out Enviroment conditional
#endif

