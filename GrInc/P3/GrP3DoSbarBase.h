/*
	Gause Platform slider bar base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSbarBase
#define	_EPC_GrP3DoSbarBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoSbarBaseStatDis			0		/* disable */
#define E_GrP3DoSbarBaseStatNrm			1		/* normal */
#define E_GrP3DoSbarBaseStatCus			2		/* cursor */
#define E_GrP3DoSbarBaseStatCtrl		3		/* control */

#define E_GrP3DoSbarBaseStatCnt			4

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSbarBasePara
{
	__u16	PntBtnStat[E_GrP3DoSbarBaseStatCnt];
	__u16	PntBgStat[E_GrP3DoSbarBaseStatCnt];

	__u16	BtnSize;						// button size
	__u8	BtnBss;							// button area basis
	BOOL8	IsShowVal;					// value show

	BOOL8	IsVertical;
	BOOL8	IsReverse;					// reverse position
	BOOL8	IsOnKeyCtrl;
	__u8	_rsvd1;

	St_GrSwordRect	BtnArea;	// button area			

	__s32		ValMin;
	__s32		ValMax;

	__u16	EvtHostId;					// event host id
	__u16	AsmValChg;					// assembly run at change value

}	*Ptr_GrP3DoSbarBasePara;

//====================================================================
//global class
class	Cls_GrP3DoSbarBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoSbarBasePara	m_ParaSbarBase;

		__s32		m_Value;					// last value
		Def_WstrNum	m_StrVal;		// value text

		BOOL8	m_IsCtrlMode;

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

		void	LcValDispUpdt( void );		// value text and graphic update
		void	LcValInc( void );					// value increase
		void	LcValDec( void );					// value decrease
		void	LcValChgNotify( void );		// notify value changed


		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );

	public:
		Cls_GrP3DoSbarBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSbarBase( void );


		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

