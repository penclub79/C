/*
	GAUSE Platform list box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoLbxBase
#define	_EPC_GrP4DoLbxBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoLbxItemStatDisable		0	/* disabled */
#define E_GrP4DoLbxItemStatNormal			1	/* normal */
#define E_GrP4DoLbxItemStatCursor			2	/* Cursor */
#define E_GrP4DoLbxItemStatSel				3	/* select */

#define E_GrP4DoLbxItemStatCnt				4

#define E_GrP4DoLbxItemDispMax				32

#define E_GrP4DoLbxBgStatDisable			0	/* disabled */
#define E_GrP4DoLbxBgStatNormal				1	/* normal */
#define E_GrP4DoLbxBgStatFocus				2	/* focus */
#define E_GrP4DoLbxBgStatControl			3	/* control */

#define E_GrP4DoLbxBgStatCnt					4

#define E_GrP4DoLbxIconMaxCnt					32

#define E_GrP4DoLbxElemViewCnt				4

#define E_GrP4DoLbxElemDispText				0
#define E_GrP4DoLbxElemDispIcon				1

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoLbxBasePara
{
	__u16	PntItmStat[E_GrP4DoLbxItemStatCnt];
	__u16	PntBgStat[E_GrP4DoLbxBgStatCnt];					// paint background

	__u16	ItemSizeX;
	__u16	ItemSizeY;
	__u16	ContStX;
	__u16	ContStY;

	__u16	ItemViewCnt;
	__u16	AsmEvtSel;

	__u16	ElemStX;
	__u16	ElemStY;
	__u16	ElemSizeY;
	__u16	ElemGap;

	__u16	ItemGap;
	__u8	ElemCnt;
	BOOL8	IsOnKeyCtrl;

	__u8	ElemView[E_GrP4DoLbxElemViewCnt];
	__u16	ElemSizeX[E_GrP4DoLbxElemViewCnt];
	__u16	PntElem[E_GrP4DoLbxElemViewCnt];
	__s32		ElemValAdd[E_GrP4DoLbxElemViewCnt];

	__u16	PntIcon[E_GrP4DoLbxIconMaxCnt];


}	*Ptr_GrP4DoLbxBasePara;

//====================================================================
//global class
class	Cls_GrP4DoLbxBase	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoLbxBasePara	m_ParaLbxBase;

		BOOL8	m_IsCtrlMode;

		// scroll position
		__s32		m_ViewPos;				// view start item index
		__s32		m_CusIdx;					// cursor index
		__s32		m_SelIdx;

		// paint
		St_GrRect	m_PntRcItemArea;
		St_GrRect	m_PntRcItemUpdt;		// update area
		__u32	m_PntViewIdx;
		__s32		m_PntItemIdx;						// painting item index
		St_GrRect	m_PntRcElemArea;
		St_GrRect	m_PntRcElemUpdt;		// update area
		__s32		m_PntElemIdx;						// painting element index
		BOOL8	m_IsPntText;						// text able

		// runtime
		__u32	m_ItemCnt;
		__u32	m_ItemTotal;
		__u32	m_PageCnt;							// item page unit
		__u32	m_PagePos;

		// local
		void	LcPaintElem( void );
		void	LcPaintCont( void );
		void	LcPaintBg( void );

		__s32		LcScrPosToItemIdx( __u16 A_X, __u16 A_Y );
		void	LcViewPosPatch( void );
		void	LcProcCusSel( void );

		virtual	BOOL8	LcPntElemValGet( __s32* A_PtrRtVal );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

	public:
		Cls_GrP4DoLbxBase(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoLbxBase( void );

		//runtime

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

