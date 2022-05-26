/*
	GAUSE Platform quad

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoQuad
#define	_EPC_GrP4DoQuad

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//constance
#define E_GrP4DoQuadGridMaxCnt		12

#define E_GrP4DoQuadEvtCnt				5

#define E_GrP4DoQuadEvtRec				0		/* recording */
#define E_GrP4DoQuadEvtMotion			1		/* motion */
#define E_GrP4DoQuadEvtSensor			2		/* sensor */
#define E_GrP4DoQuadEvtRsvd0			3		/* reserved */
#define E_GrP4DoQuadEvtRsvd1			4		/* reserved */

#define E_GrP4DoQuadKeyCmbTimeOut	15

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoQuadPara
{
	__u16	PntEvtTbl[E_GrP4DoQuadEvtCnt];							// event paint table
	__u16	PntCorvert;							// covert paint code
	__u16	PntVloss;								// video loss
	__u16	PntChOff;								// channel not use
	__u8	EvtBssTbl[E_GrP4DoQuadEvtCnt];	// event position basis table
	__u8	CovertBss;							// covert basis
	__u8	VlossBss;								// video loss basis
	__u8	ChOffBss;								// channel off basis
	St_GrSwordRect	EvtPosTbl[E_GrP4DoQuadEvtCnt];		// event position table
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
}	*Ptr_GrP4DoQuadPara;

//====================================================================
//global class
class	Cls_GrP4DoQuad	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoQuadPara	m_ParaDoQuad;
		BOOL8	m_IsIconShow;
		BOOL8	m_IsTitleShow;
		BOOL8	m_IsContShow;

		__u8	m_OldQdMode;

		St_GrRect	m_RcQuad;			// quad screen rect

		BOOL8	m_KeyChCmbTime;	// key channel combine
		__u8	m_VdoChCnt;

		__s32		m_MgnHz;				// horizontal margin
		__s32		m_MgnVt;				// vertical margin

		// runtime
		Ptr_GrRect	m_GridTbl;	// grid table
		__u8	m_GridCnt;		// grid count
		__u8	m_ViewCnt;		// view count
		Ptr_GrRect	m_ViewRcTbl;
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


		// script
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump(void);					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

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
		Cls_GrP4DoQuad(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoQuad( void );

		void	MarginSet( __s32 A_Hz, __s32 A_Vt );

		// inherited
		void	PosSet(Ptr_GrRect A_PtrRect, __u8 A_PosRel);	// instead to paint event

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

