/*
	Gause Platform grid base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoGridBase
#define	_EPC_GrP4DoGridBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoGridBgStatDisable	0
#define E_GrP4DoGridBgStatNormal	1
#define E_GrP4DoGridBgStatOnCus		2

#define E_GrP4DoGridBgStatCnt			3

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoGridBasePara
{
	__u16	PntBg[E_GrP4DoGridBgStatCnt];
	__u16	PntCusItem;

	__u16	PntCusVu;
	__u16	PntCusVd;
	__u16	PntCusHl;
	__u16	PntCusHr;

	__u16	ItemPosX;
	__u16	ItemPosY;
	__u16	ItemSizeX;
	__u16	ItemSizeY;

	BOOL8	IsOnKeyCtrl;
	BOOL8	IsChSelAble;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP4DoGridBasePara;

//====================================================================
//global class
class	Cls_GrP4DoGridBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoGridBasePara	m_ParaGridBase;

		BOOL8	m_IsCtrlMode;
		BOOL8	m_BgStat;			// last background status

		__u16	m_ItemCntX;		// item count x
		__u16	m_ItemCntY;		// item count y

		__u16	m_CusX;
		__u16	m_CusY;

		// paint

		// local
		BOOL8	LcScrPosToItemPos( __u16 A_X, __u16 A_Y, __u16* A_PtrRtX, __u16* A_PtrRtY );

		virtual	void	LcProcKeyItemSel( void );						// key item select
		virtual	void	LcPaintSelect( void );							// paint select area
		virtual	void	LcPainItems( void );								// paint item area

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb );
	public:
		Cls_GrP4DoGridBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoGridBase( void );


		// inherited

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

