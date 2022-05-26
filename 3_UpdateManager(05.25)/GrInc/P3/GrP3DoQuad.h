/*
	GAUSE Platform quad

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoQuad
#define	_EPC_GrP3DoQuad

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance
#define E_GrP3DoQuadGridMaxCnt		12

#define E_GrP3DoQuadEvtCnt				5

#define E_GrP3DoQuadEvtRec				0		/* recording */
#define E_GrP3DoQuadEvtMotion			1		/* motion */
#define E_GrP3DoQuadEvtSensor			2		/* sensor */
#define E_GrP3DoQuadEvtRsvd0			3		/* reserved */
#define E_GrP3DoQuadEvtRsvd1			4		/* reserved */

#define E_GrP3DoQuadKeyCmbTimeOut	15

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoQuadPara
{
	__u16	PntEvtTbl[E_GrP3DoQuadEvtCnt];							// event paint table
	__u16	PntCorvert;							// covert paint code
	__u16	PntVloss;								// video loss
	__u16	PntChOff;								// channel not use
	__u8	EvtBssTbl[E_GrP3DoQuadEvtCnt];	// event position basis table
	__u8	CovertBss;							// covert basis
	__u8	VlossBss;								// video loss basis
	__u8	ChOffBss;								// channel off basis
	St_GrSwordRect	EvtPosTbl[E_GrP3DoQuadEvtCnt];		// event position table
	St_GrSwordRect	CovertPos;
	St_GrSwordRect	VlossPos;
	St_GrSwordRect	ChOffPos;
	__u16	PntTitleTbl[E_GrDvrMaxChCnt];		// title paint code table
	__u16	PntContTbl[E_GrDvrMaxChCnt];		// contents paint code table
	St_GrSwordRect	TItlePos;
	St_GrSwordRect	ContPos;
	__u8	TitleBss;
	__u8	ContBss;
	__u8	GridWidth;	// grid width
	BOOL8	IsPlayIcon;	// play icon view
	__u32	ClrGrid;		// grid color
}	*Ptr_GrP3DoQuadPara;

//====================================================================
//global class
class	Cls_GrP3DoQuad	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoQuadPara	m_ParaDoQuad;
		BOOL8	m_IsGridShow;
		BOOL8	m_IsIconShow;
		BOOL8	m_IsTitleShow;
		BOOL8	m_IsContShow;

		__u8	m_OldQdMode;

		BOOL8	m_KeyChCmbTime;	// key channel combine
		__u8	m_VdoChCnt;

		__s32		m_MgnHz;				// horizontal margin
		__s32		m_MgnVt;				// vertical margin

		// runtime
		St_GrRect	m_GridTbl[E_GrP3DoQuadGridMaxCnt];		// grid table
		__u8	m_GridCnt;		// grid count
		__u8	m_ViewCnt;		// view count
		St_GrRect	m_ViewRcTbl[E_GrDvrMaxChCnt];
		__u8*	m_ViewChTbl;
		__u8	m_DispAtbTbl[E_GrDvrMaxChCnt];		// now display attribute

		__u8	m_PntVwCh;				// paint view channel
		
		BOOL8	m_IsPlayMode;
		__u8*	m_SysAtbTbl;

		void*	m_PtrGvs;
		__u16*	m_PtrStpCamUse;

		St_GrRect	m_RcCli;			// client rect

		// local
		void	LcQuadStatUpdt( void );
		void	LcRcCliUpdt( void );
		void	LcSetupUpdt( void );
		void	LcAccountUpdt( void );
		void	LcLivePlayUpdt( void );
		void	LcPaintGrid( void );		// grid line paint
		void	LcPaintView( void );
		void	LcEvtDispUpdt( void );
		__s32		LcCmdSubShowGet( __s32 A_Type );
		void	LcCmdSubShowSet( __s32 A_Type, __s32 A_Val );
		__u8	LcViewIdxByPos( __u16 A_X, __u16 A_Y );
		void	LcTmrKeyChCombine( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	void	ProcEvtMseEnter( void );
		virtual	void	ProcEvtMseLeave( void );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		virtual	void	ProcEvtSysStat( __u8 A_Type );

	public:
		Cls_GrP3DoQuad( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoQuad( void );

		void	MarginSet( __s32 A_Hz, __s32 A_Vt );

		// inherited
		virtual	void	RectSet( Ptr_GrRect A_PtrRect );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

