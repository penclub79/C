/*
	GAUSE Platform grid record schedule setup

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoGridRscStp
#define	_EPC_GrP4DoGridRscStp

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoGridBase.h>

//====================================================================
//constance

#define E_GrP4DoGridRscStpItemCnt		8

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoGridRscStpPara
{
	__u8	WeekCnt;
	__u8	_rsvd1;
	__u16	_rsvd2;

	__u8	ValCnt;			// value count
	BOOL8	IsAfterChg;	// after change
	__u16	MseValIdx;	// mouse value GVU index

	__u16	PntSel;			// paint select code
	__u16	_rsvd0;

	__u16	PntVal[E_GrP4DoGridRscStpItemCnt];		// value paint table

	__u8	ValTbl[E_GrP4DoGridRscStpItemCnt];		// item value count

}	*Ptr_GrP4DoGridRscStpPara;

//====================================================================
//global class
class	Cls_GrP4DoGridRscStp	:	public	Cls_GrP4DoGridBase
{
	protected:
		St_GrP4DoGridRscStpPara	m_ParaGridRscStp;

		__s32*	m_PtrGvu;		// GVU table pointer

		__u8	m_Ch;					// channel
		BOOL8	m_IsSel;		// mouse left button down

		__u16	m_SelStX;
		__u16	m_SelStY;
		__u16	m_SelEdX;
		__u16	m_SelEdY;

		virtual	void	LcProcKeyItemSel( void );						// key item select
		virtual	void	LcPaintSelect( void );							// paint select area
		virtual	void	LcPainItems( void );								// paint item area

		__u8	LcValueGet( __u16 A_X, __u16 A_Y );		// value get
		__u16	LcValueToPst( __u8 A_Value );				// value to paint code
		void	LcValueSet( __u16 A_X, __u16 A_Y, __u8 A_Val );
		__u8	LcValueToItemIdx( __u8 A_Value );
		__u8	LcItemIdxToValue( __u8 A_ItemIdx );

		void	ProcEvtSpinValChg( Cls_GrP4DoBase* A_Obj );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );
		virtual	void	ProcEvtMseLeave( void );
		virtual	BOOL8	ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtFocusLoss( void );

	public:
		Cls_GrP4DoGridRscStp(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoGridRscStp( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

