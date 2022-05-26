/*
	Gause Platform calendar base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoCalBase
#define	_EPC_GrP4DoCalBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoCalPntIdxDisable		0
#define E_GrP4DoCalPntIdxNormal			1
#define E_GrP4DoCalPntIdxOnCus			2
#define E_GrP4DoCalPntIdxMark				3
#define E_GrP4DoCalPntIdxCusAndMark	4

#define E_GrP4DoCalBgPntCnt					2
#define E_GrP4DoCalItemPntCnt				5
#define E_GrP4DoCalMarkCnt					8

#define E_GrP4DoCalTimeIdxYear			0
#define E_GrP4DoCalTimeIdxMonth			1
#define E_GrP4DoCalTimeIdxDay				2
#define E_GrP4DoCalTimeIdxHour			3
#define E_GrP4DoCalTimeIdxMin				4
#define E_GrP4DoCalTimeIdxSec				5

#define E_GrP4DoCalTimeElementCnt		6

#define E_GrP4DoCalPntTypeTMC				0
#define E_GrP4DoCalPntTypeMTC				1
#define E_GrP4DoCalPntTypeMCT				2
#define E_GrP4DoCalPntTypeCMT				3

#define E_GrP4DoCalCtrlIdxYear			0
#define E_GrP4DoCalCtrlIdxMonth			1

#define E_GrP4DoCalCtrlCnt					2

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoCalBasePara
{
	__u16	PntBg[E_GrP4DoCalBgPntCnt];
	__u16	PntItem[E_GrP4DoCalItemPntCnt];
	__u16	PntCus;
	__u16	PntLvl[E_GrP4DoCalMarkCnt];

	BOOL8	IsAlgWeek;		// week align
	__u8	PntType;			// paint type
	__u16	AsmEvtMarkChg;

	__u16	StartX;
	__u16	StartY;
	__u16	SizeX;
	__u16	SizeY;

	__u16	GapX;
	__u16	GapY;

	__u16	DateCtrl[E_GrP4DoCalCtrlCnt];

}	*Ptr_GrP4DoCalBasePara;

//====================================================================
//global class
class	Cls_GrP4DoCalBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoCalBasePara	m_ParaDoCalBase;

		WCHAR	m_StrTxt[31][4];

		__u16	m_ElemTime[E_GrP4DoCalTimeElementCnt];	// time element
		__u8	m_StWeek;					// start week
		__u8	m_DayCnt;

		void*	m_PtrMap;					// mark map

		BOOL8	m_IsBitMode;			// bit mode
		__u8	m_LvlCnt;					// mark level count
		__u8	m_CusDay;					// cursor position

		// paint
		BOOL8	m_IsPntStatEn;		// paint status enable
		__u8	m_PntDay;					// paint day
		St_GrRect	m_PntRcItem;	// paint item rect

		// local
		virtual	__u8	LcMarkValueGet( __u8 A_Day );
		virtual	void	LcNotifyMarkChg( void );
		virtual	void	LcCalRebuild( void );		// rebuild calendar - ex : year, month change
		virtual	void	LcProcTouchDay( void );	// call when press enter key or left mouse button click
		virtual	void	LcViewTimeChg( void );	// view time change
		virtual	BOOL8	LcProcEvtSpinValChg( __u16 A_ObjId );	// spin button event

		void	LcElemTmUpdtByCtrl( void );			// time element update by Controls
		void	LcCtrlUpdtByElemTm( void );			// controls update by element time

		void	LcPntText( void );
		void	LcPntMark( void );
		void	LcPntCus( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// event
		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );

	public:
		Cls_GrP4DoCalBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoCalBase( void );

		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

