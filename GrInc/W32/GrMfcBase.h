/*
	MFC Form base
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrMfcBase
#define	_EPC_GrMfcBase

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrMfcFrameMaxCnt			64
#define E_GrMfcFrmGrpMaxCnt			16
#define E_GrMfcFormMaxCnt				4

// direction
#define E_GrMfcDirCnt						4
#define E_GrMfcDirLeft					0
#define E_GrMfcDirUp						1
#define E_GrMfcDirRight					2
#define E_GrMfcDirDown					3

#define E_GrMfcChainWall				0xFF

// split bar type
#define E_GrMfcSplTypeOff				0
#define E_GrMfcSplTypeHz				1		/* horizontal */
#define E_GrMfcSplTypeVt				2		/* vertical */

// form mode
#define E_GrMfcFomModeOff				0
#define E_GrMfcFomModeView			1			/* view  */

// frame mode
#define E_GrMfcFrmModeOff				0
#define E_GrMfcFrmModeView			1			/* view  */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrMfcSplitCtrl
{
	__u8	Type;
	__s32		Pos;
}	*Ptr_GrMfcSplitCtrl;

typedef struct St_GrMfcFrmCtrl
{
	__u8	Mode;
	__u8	RelSpl[E_GrMfcDirCnt];	// relate split bar
}	*Ptr_GrMfcFrmCtrl;

typedef struct St_GrMfcFomCtrl
{
	__u8	Mode;
	St_GrRect	FormRc;
	__s32		BasisX;		// basis size x
	__s32		BasisY;		// basis size y
	St_GrMfcSplitCtrl	Split[E_GrMfcFrameMaxCnt];	// split bar
	St_GrMfcFrmCtrl	Frame[E_GrMfcFrameMaxCnt];

}	*Ptr_GrMfcFomCtrl;

typedef struct St_GrMfcGrpMng
{
	St_GrMfcFomCtrl	FormMng[E_GrMfcFormMaxCnt];
}	*Ptr_GrMfcGrpMng;

typedef	struct St_GrMfcMng
{
	__u8	NowGrp;			// now group
	St_GrMfcGrpMng	GrpMng[E_GrMfcFrmGrpMaxCnt];
}	*Ptr_GrMfcMng;

//====================================================================
//class


//====================================================================
//global function

//====================================================================
//out Environment conditional
#endif

