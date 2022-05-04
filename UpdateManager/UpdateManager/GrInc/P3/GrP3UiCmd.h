/*
	GAUSE Platform UI command

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3UiCmd
#define	_EPC_GrP3UiCmd

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>

//====================================================================
//constance

// relate UPI type
#define E_GrP3RelUpiCnt						4

#define E_GrP3RelUpiNone					0
#define E_GrP3RelUpiValue					1
#define E_GrP3RelUpiRecType				2
#define E_GrP3RelUpiCh						3


// UPI
#define E_GrP3UpiCnt							48

// UOI command
#define E_GrP3UpiClassIdGet				1			/* class id get */
#define E_GrP3UpiScptIdGet				2			/* script id get */
#define E_GrP3UpiPosGet						3			/* position get */
#define E_GrP3UpiPosSet						4			/* position set */
#define E_GrP3UpiSizeGet					5			/* size get */
#define E_GrP3UpiSizeSet					6			/* size set */
#define E_GrP3UpiVisibleGet				7			/* visible status get */
#define E_GrP3UpiVisibleSet				8			/* visible status set */
#define E_GrP3UpiEnableGet				9			/* enable status get */
#define E_GrP3UpiEnableSet				10		/* enable status set */
#define E_GrP3UpiFcsMvTblGet			11		/* focus move table value get */
#define E_GrP3UpiFcsMvTblSet			12		/* focus move table value set */
#define E_GrP3UpiUserDataGet			13		/* user data get */
#define E_GrP3UpiUserDataSet			14		/* user data set */
#define E_GrP3UpiReqDraw					15		/* redraw */
#define E_GrP3UpiWinClose					16		/* window close */
#define E_GrP3UpiValueGet					17		/* value get */
#define E_GrP3UpiValueSet					18		/* value set */
#define E_GrP3UpiTextGet					19		/* text get */
#define E_GrP3UpiTextSet					20		/* text set */
#define E_GrP3UpiMinValueGet			21		/* minimal value get */
#define E_GrP3UpiMinValueSet			22		/* minimal value set */
#define E_GrP3UpiMaxValueGet			23		/* max value get */
#define E_GrP3UpiMaxValueSet			24		/* max value set */
#define E_GrP3UpiEvtHostIdGet			25		/* event host id get */
#define E_GrP3UpiEvtHostIdSet			26		/* event host id set */
#define	E_GrP3UpiTextIdGet				27		/* text id get */
#define	E_GrP3UpiTextIdSet				28		/* text id get */
#define E_GrP3UpiRecTypeGet				29		/* record type get */
#define E_GrP3UpiRecTypeSet				30		/* record type set */
#define E_GrP3UpiFrmPageSet				31		/* frame page set */
#define E_GrP3UpiFrmPageBack			32		/* frame page back */
#define E_GrP3UpiChGet						33		/* channel get */
#define E_GrP3UpiChSet						34		/* channel set */
#define E_GrP3UpiSubShowGet				35		/* sub control show status get */
#define E_GrP3UpiSubShowSet				36		/* sub control show status set */
#define E_GrP3UpiFocusIdSet				37		/* focused object id set */
#define E_GrP3UpiCtlgLoad					38		/* catalog data load */
#define E_GrP3UpiPageNext					39		/* page next */
#define E_GrP3UpiPagePrev					40		/* page previous */
#define E_GrP3UpiSetupAdrGet			41		/* setup address get */
#define E_GrP3UpiSetupAdrSet			42		/* setup address set */
#define E_GrP3UpiUpdate						43		/* update */
#define E_GrP3UpiTextAdd					44		/* text add */
#define E_GrP3UpiItemCntGet				45		/* item count get */
#define E_GrP3UpiItemTxtIdSet			46		/* item text id set */
#define E_GrP3UpiItemSizeSet			47		/* item size set */
#define E_GrP3UpiItemSizeGet			48		/* item size get */

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

