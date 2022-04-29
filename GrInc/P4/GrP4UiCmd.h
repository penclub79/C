/*
	GAUSE Platform UI command

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4UiCmd
#define	_EPC_GrP4UiCmd

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>

//====================================================================
//constance

// relate UPI type
#define E_GrP4RelUpiCnt						4

#define E_GrP4RelUpiNone					0
#define E_GrP4RelUpiValue					1
#define E_GrP4RelUpiRecType				2
#define E_GrP4RelUpiCh						3


// UPI
#define E_GrP4UpiCnt							56
#define E_GrP4UpiMaxCnt						256

// UOI command
#define E_GrP4UpiClassIdGet				1			/* class id get */
#define E_GrP4UpiScptIdGet				2			/* script id get */
#define E_GrP4UpiPosGet						3			/* position get */
#define E_GrP4UpiPosMovAbs				4			/* position move absolute */
#define E_GrP4UpiSizeGet					5			/* size get */
#define E_GrP4UpiSizeSetAbs				6			/* size set absolute */
#define E_GrP4UpiVisibleGet				7			/* visible status get */
#define E_GrP4UpiVisibleSet				8			/* visible status set */
#define E_GrP4UpiEnableGet				9			/* enable status get */
#define E_GrP4UpiEnableSet				10		/* enable status set */
#define E_GrP4UpiFcsMvTblGet			11		/* focus move table value get */
#define E_GrP4UpiFcsMvTblSet			12		/* focus move table value set */
#define E_GrP4UpiUserDataGet			13		/* user data get */
#define E_GrP4UpiUserDataSet			14		/* user data set */
#define E_GrP4UpiReqDraw					15		/* redraw */
#define E_GrP4UpiWinClose					16		/* window close */
#define E_GrP4UpiValueGet					17		/* value get */
#define E_GrP4UpiValueSet					18		/* value set */
#define E_GrP4UpiTextGet					19		/* text get */
#define E_GrP4UpiTextSet					20		/* text set */
#define E_GrP4UpiMinValueGet			21		/* minimal value get */
#define E_GrP4UpiMinValueSet			22		/* minimal value set */
#define E_GrP4UpiMaxValueGet			23		/* max value get */
#define E_GrP4UpiMaxValueSet			24		/* max value set */
#define E_GrP4UpiEvtHostIdGet			25		/* event host id get */
#define E_GrP4UpiEvtHostIdSet			26		/* event host id set */
#define	E_GrP4UpiTextIdGet				27		/* text id get */
#define	E_GrP4UpiTextIdSet				28		/* text id get */
#define E_GrP4UpiRecTypeGet				29		/* record type get */
#define E_GrP4UpiRecTypeSet				30		/* record type set */
#define E_GrP4UpiFrmPageSet				31		/* frame page set */
#define E_GrP4UpiFrmPageBack			32		/* frame page back */
#define E_GrP4UpiChGet						33		/* channel get */
#define E_GrP4UpiChSet						34		/* channel set */
#define E_GrP4UpiSubShowGet				35		/* sub control show status get */
#define E_GrP4UpiSubShowSet				36		/* sub control show status set */
#define E_GrP4UpiFocusIdSet				37		/* focused object id set */
#define E_GrP4UpiCtlgLoad					38		/* catalog data load */
#define E_GrP4UpiPageNext					39		/* page next */
#define E_GrP4UpiPagePrev					40		/* page previous */
#define E_GrP4UpiSetupAdrGet			41		/* setup address get */
#define E_GrP4UpiSetupAdrSet			42		/* setup address set */
#define E_GrP4UpiUpdate						43		/* update */
#define E_GrP4UpiTextAdd					44		/* text add */
#define E_GrP4UpiItemCntGet				45		/* item count get */
#define E_GrP4UpiItemTxtIdSet			46		/* item text id set */
#define E_GrP4UpiItemSizeSet			47		/* item size set */
#define E_GrP4UpiItemSizeGet			48		/* item size get */
#define E_GrP4UpiItemCntSet				49		/* item count set */
#define E_GrP4UpiPosMovRel				50		/* position move relate */
#define E_GrP4UpiSizeSetRel				51		/* size change relate */
#define E_GrP4UpiItemAdd					52		/* item add */
#define E_GrP4UpiItemDel					53		/* item delete */
#define E_GrP4UpiItemIconSet			54		/* item icon set */
#define E_GrP4UpiItemTextSet			55		/* item text set */
#define E_GrP4UpiModeSet					56		/* mode set */


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

