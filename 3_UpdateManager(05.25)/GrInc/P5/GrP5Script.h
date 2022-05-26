/*
	Platform 5 Script

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Script
#define	_EPC_GrP5Script

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <P5/GrP5Base.h>

//====================================================================
//constance

#define	E_GrP5ScriptFcc				Mac_GrMakeFcc('S','C','P','5')		/* script fcc */

// tcc

#define	E_GrP5ScptObjHdTcc		Mac_GrMakeTcc('O','S')		/* object start */
#define	E_GrP5ScptObjEdTcc		Mac_GrMakeTcc('O','E')		/* object end */

#define	E_GrP5ScptParaTcc			Mac_GrMakeTcc('P','R')		/* parametter */

// script header


//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrP5ScptObjHead
{
	__u16	Tcc;
	__u16	ClassId;
	__u16	ScptId;
	__u16	ParaSize;
	__u32	HdPara;
	__u32	TotalSize;	// include head , para, slave, end
	St_GrP5RelRect	RelRect;
}*Ptr_GrP5ScptObjHead;

typedef struct St_GrP5ScptObjEnd
{
	__u16	Tcc;
	__u16	ScptId;
}	*Ptr_GrP5ScptObjEnd;

typedef struct St_GrP5ScptParaHead
{
	__u16	Tcc;
	__u16	_rsvd0;
	__u16	ClassId;
	__u16	Size;			// parametter size only
}	*Ptr_GrP5ScptParaHead;

typedef struct St_GrP5ScptUiInfoBasic
{
	__u32	FccScpt;		// script fcc
	__u32	FccScr;			// screen fourcc
}	*Ptr_GrP5ScptUiInfoBasic;


//====================================================================
//global class


//====================================================================
//global function



//====================================================================
//out Environment conditional
#endif

