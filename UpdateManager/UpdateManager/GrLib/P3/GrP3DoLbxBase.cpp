/*
		platform list box base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P3/GrP3DoLbxBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P3/GrP3Script.h>
#include	<P3/GrP3ConBase.h>
#include	<P3/GrP3AsmBase.h>
#include	<P3/GrP3Mng.h>
#include	<P3/GrP3UiCmd.h>

#ifdef GR_P3_EDITOR
#include	<P3/Dvl/GrP3DvlBase.h>
#include	<P3/Dvl/GrP3DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local macro

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP3Mng*	V_GrP3Mng;

#ifdef GR_P3_EDITOR

#define E_GrP3DoLbxBaseParaCnt		69

St_GrP3DvlDoPrInfoData	V_GrP3DoLbxBaseParaInfo[E_GrP3DoLbxBaseParaCnt]	=	
{
	{	L"PntBgStatDis",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntBgStat[E_GrP3DoLbxBgStatDisable],NULL,	L"Paint background status disabled"	},
	{	L"PntBgStatNrm",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntBgStat[E_GrP3DoLbxBgStatNormal],NULL,	L"Paint background status normal"	},
	{	L"PntBgStatFcs",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntBgStat[E_GrP3DoLbxBgStatFocus],NULL,	L"Paint background status focus"	},
	{	L"PntBgStatCtrl",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntBgStat[E_GrP3DoLbxBgStatControl],NULL,	L"Paint background status control"	},
	{	L"PntItemStatDis",	TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntItmStat[E_GrP3DoLbxItemStatDisable],NULL,	L"Paint item status disabled"	},
	{	L"PntItemStatNrm",	TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntItmStat[E_GrP3DoLbxItemStatNormal],NULL,	L"Paint item status normal"	},
	{	L"PntItemStatCus",	TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntItmStat[E_GrP3DoLbxItemStatCursor],NULL,	L"Paint item status cursor"	},
	{	L"PntItemStatSel",	TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntItmStat[E_GrP3DoLbxItemStatSel],NULL,	L"Paint item status select"	},

	{	L"PntElem0",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntElem[0],NULL,	L"Paint elemental 0"	},
	{	L"PntElem1",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntElem[1],NULL,	L"Paint elemental 1"	},
	{	L"PntElem2",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntElem[2],NULL,	L"Paint elemental 2"	},
	{	L"PntElem3",		TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntElem[3],NULL,	L"Paint elemental 3"	},

	{	L"PntIcon0",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[0],NULL,	L"Paint icon 0"	},
	{	L"PntIcon1",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[1],NULL,	L"Paint icon 1"	},
	{	L"PntIcon2",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[2],NULL,	L"Paint icon 2"	},
	{	L"PntIcon3",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[3],NULL,	L"Paint icon 3"	},
	{	L"PntIcon4",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[4],NULL,	L"Paint icon 4"	},
	{	L"PntIcon5",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[5],NULL,	L"Paint icon 5"	},
	{	L"PntIcon6",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[6],NULL,	L"Paint icon 6"	},
	{	L"PntIcon7",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[7],NULL,	L"Paint icon 7"	},
	{	L"PntIcon8",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[8],NULL,	L"Paint icon 8"	},
	{	L"PntIcon9",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[9],NULL,	L"Paint icon 9"	},
	{	L"PntIcon10",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[10],NULL,	L"Paint icon 10"	},
	{	L"PntIcon11",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[11],NULL,	L"Paint icon 11"	},
	{	L"PntIcon12",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[12],NULL,	L"Paint icon 12"	},
	{	L"PntIcon13",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[13],NULL,	L"Paint icon 13"	},
	{	L"PntIcon14",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[14],NULL,	L"Paint icon 14"	},
	{	L"PntIcon15",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[15],NULL,	L"Paint icon 15"	},
	{	L"PntIcon16",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[16],NULL,	L"Paint icon 16"	},
	{	L"PntIcon17",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[17],NULL,	L"Paint icon 17"	},
	{	L"PntIcon18",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[18],NULL,	L"Paint icon 18"	},
	{	L"PntIcon19",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[19],NULL,	L"Paint icon 19"	},
	{	L"PntIcon20",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[20],NULL,	L"Paint icon 20"	},
	{	L"PntIcon21",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[21],NULL,	L"Paint icon 21"	},
	{	L"PntIcon22",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[22],NULL,	L"Paint icon 22"	},
	{	L"PntIcon23",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[23],NULL,	L"Paint icon 23"	},
	{	L"PntIcon24",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[24],NULL,	L"Paint icon 24"	},
	{	L"PntIcon25",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[25],NULL,	L"Paint icon 25"	},
	{	L"PntIcon26",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[26],NULL,	L"Paint icon 26"	},
	{	L"PntIcon27",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[27],NULL,	L"Paint icon 27"	},
	{	L"PntIcon28",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[28],NULL,	L"Paint icon 28"	},
	{	L"PntIcon29",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[29],NULL,	L"Paint icon 29"	},
	{	L"PntIcon30",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[30],NULL,	L"Paint icon 30"	},
	{	L"PntIcon31",				TRUE,		E_GrP3DvlParaTypePntCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->PntIcon[31],NULL,	L"Paint icon 31"	},

	{	L"ContStX",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ContStX,NULL,	L"contents start x"	},
	{	L"ContStY",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ContStY,NULL,	L"contents start y"	},
	{	L"ItemSizeX",				TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ItemSizeX,NULL,	L"item size x"	},
	{	L"ItemSizeY",				TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ItemSizeY,NULL,	L"item size y"	},
	{	L"ItemViewCnt",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ItemViewCnt,NULL,	L"item display count"	},
	{	L"ItemGap",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ItemGap,NULL,	L"item vertical gap size"	},

	{	L"ElemStX",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemStX,NULL,	L"element start x"	},
	{	L"ElemStY",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemStY,NULL,	L"element start y"	},
	{	L"ElemGap",					TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemGap,NULL,	L"element horizontal gap size"	},
	{	L"ElemCnt",					TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemCnt,NULL,	L"element count"	},

	{	L"ElemSizeY",				TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemSizeY,NULL,	L"element size y"	},
	{	L"ElemSizeX0",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemSizeX[0],NULL,	L"element size x 0"	},
	{	L"ElemSizeX1",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemSizeX[1],NULL,	L"element size x 1"	},
	{	L"ElemSizeX2",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemSizeX[2],NULL,	L"element size x 2"	},
	{	L"ElemSizeX3",			TRUE,		E_GrP3DvlParaTypeWord,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemSizeX[3],NULL,	L"element size x 3"	},

	{	L"ElemView0",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemView[0],NULL,	L"element view type 0\n0=text,1=time,2=icon"	},
	{	L"ElemView1",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemView[1],NULL,	L"element view type 1\n0=text,1=time,2=icon"	},
	{	L"ElemView2",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemView[2],NULL,	L"element view type 2\n0=text,1=time,2=icon"	},
	{	L"ElemView3",				TRUE,		E_GrP3DvlParaTypeByte,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemView[3],NULL,	L"element view type 3\n0=text,1=time,2=icon"	},

	{	L"ElemValAdd0",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemValAdd[0],NULL,	L"element additional value 0"	},
	{	L"ElemValAdd1",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemValAdd[1],NULL,	L"element additional value 1"	},
	{	L"ElemValAdd2",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemValAdd[2],NULL,	L"element additional value 2"	},
	{	L"ElemValAdd3",			TRUE,		E_GrP3DvlParaTypeInt,						1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->ElemValAdd[3],NULL,	L"element additional value 3"	},

	{	L"IsOnKeyCtrl",			TRUE,		E_GrP3DvlParaTypeBool8,					1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->IsOnKeyCtrl,NULL,	L"internal cursor control without enter control mode"	},
	{	L"AsmEvtSel",				TRUE,		E_GrP3DvlParaTypeAsmCode,				1,	(void*)&((Ptr_GrP3DoLbxBasePara)0)->AsmEvtSel,NULL,	L"ASM code for select event"	},

};
#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP3DoLbxBase::Cls_GrP3DoLbxBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt ):
Cls_GrP3DoBase( A_ScptId, A_PtrRect, A_ObjPrnt )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//init
		m_ClassId		=	E_GrP3DoClassLbxBase;
		m_ParaSize	=	m_ParaSize + sizeof(m_ParaLbxBase) + sizeof(St_GrP3ScptParaHd);

		m_IsKeyAble	=	TRUE;
		m_IsMouseAble	=	TRUE;

		GrDumyZeroMem( &m_ParaLbxBase, sizeof(m_ParaLbxBase) );
		m_ParaLbxBase.ItemViewCnt	=	1;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrP3DoLbxBgStatCnt;Tv_WkIdx++ )
		{
			m_ParaLbxBase.PntBgStat[Tv_WkIdx]	=	E_GrP3ScptPstIdDflt;
		}

		m_ViewPos			=	0;
		m_CusIdx			=	0;
		m_SelIdx			=	-1;
		m_IsCtrlMode	=	FALSE;

		m_IsPntText		=	FALSE;
		m_ItemCnt			=	0;
		m_ItemTotal		=	0;
		m_PageCnt			=	0;
		m_PagePos			=	0;

}
//--------------------------------------------------------------------
Cls_GrP3DoLbxBase::~Cls_GrP3DoLbxBase( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::RtlScptParaSubDump( void )
{
#ifdef GR_P3_EDITOR
	// local -------------------
		Ptr_GrP3ScptParaHd	Tv_PtrHd;
		void*	Tv_PtrCont;
	// code --------------------
		// inherited
		Cls_GrP3DoBase::RtlScptParaSubDump();
		//build head
		Tv_PtrHd			=	(Ptr_GrP3ScptParaHd)( (__u32)m_PtrScpt + m_ScptPrgs );
		Tv_PtrHd->Stx				=	E_GrP3ScptParaStx;
		Tv_PtrHd->ClassId		=	E_GrP3DoClassLbxBase;
		Tv_PtrHd->ParaSize	=	sizeof(m_ParaLbxBase);
		Tv_PtrCont		=	(void*)( (__u32)m_PtrScpt + m_ScptPrgs + sizeof(St_GrP3ScptParaHd) );
		GrDumyCopyMem( Tv_PtrCont, &m_ParaLbxBase, sizeof(m_ParaLbxBase) );
		//update progress
		m_ScptPrgs	=	m_ScptPrgs + sizeof(m_ParaLbxBase)	+ sizeof(St_GrP3ScptParaHd);
		// patch
		if ( 0 == m_ParaLbxBase.ItemViewCnt )
		{
			m_ParaLbxBase.ItemViewCnt	=	1;
		}
		else if ( E_GrP3DoLbxItemDispMax < m_ParaLbxBase.ItemViewCnt )
		{
			m_ParaLbxBase.ItemViewCnt	=	E_GrP3DoLbxItemDispMax;
		}
		
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::RtlScptParaSubUpdt( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP3DoBase::RtlScptParaSubUpdt();
		ParaCopyFromScpt( &m_ParaLbxBase, sizeof(m_ParaLbxBase) , m_PtrScpt , E_GrP3DoClassLbxBase );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::ProcPaint( void )
{
	// local -------------------
	// code --------------------
		LcPaintCont();	// contents paint		
		LcPaintBg();	// background paint
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::RtlDvlParaInfoSubDump( void )
{
#ifdef GR_P3_EDITOR
		// inherited
		Cls_GrP3DoBase::RtlDvlParaInfoSubDump();
		// dump
		LcDvlParaInfoSaveAdd( &m_ParaLbxBase, V_GrP3DoLbxBaseParaInfo, E_GrP3DoLbxBaseParaCnt );
#endif
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::LcPaintBg( void )
{
	// local -------------------
		__u8	Tv_Stat;
	// code --------------------
		// get background status
		Tv_Stat	=	E_GrP3DoLbxBgStatDisable;
		if ( IsCanEnable() )
		{
			Tv_Stat	=	E_GrP3DoLbxBgStatNormal;
			if ( m_IsCtrlMode )
			{
				Tv_Stat	=	E_GrP3DoLbxBgStatControl;
			}
			else if ( m_IsOnMouse || IsFocused() )
			{
				Tv_Stat	=	E_GrP3DoLbxBgStatFocus;
			}
		}
		// paint
		m_IsPntText	=	FALSE;
		PaintScriptPush( m_ParaLbxBase.PntBgStat[Tv_Stat], &m_PntRcScr );
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::LcPaintCont( void )
{
	// local -------------------
		__u8	Tv_ItemStat;
		BOOL8	Tv_IsCanEable;
	// code --------------------
		if ( (0 != m_ParaLbxBase.ItemViewCnt) && (0 != m_ParaLbxBase.ElemCnt) )
		{
			// init
			Tv_IsCanEable	=	IsCanEnable();
			// calculate contents area
			m_PntRcItemArea.left	=	m_PntRcScr.left + (__s32)m_ParaLbxBase.ContStX;
			m_PntRcItemArea.top		=	m_PntRcScr.top + (__s32)m_ParaLbxBase.ContStY;
			m_PntRcItemArea.right	=	m_PntRcItemArea.left + (__s32)m_ParaLbxBase.ItemSizeX;

			for ( m_PntViewIdx=0;m_PntViewIdx < m_ParaLbxBase.ItemViewCnt;m_PntViewIdx++ )
			{
				// check item index
				m_PntItemIdx	=	m_ViewPos + (__s32)m_PntViewIdx;
				if ( m_PntItemIdx < (__s32)m_ItemCnt )
				{
					// calculate item area
					m_PntRcItemArea.bottom	=	m_PntRcItemArea.top + (__s32)m_ParaLbxBase.ItemSizeY;
					// calculate update area
					m_PntRcItemUpdt	=	m_PntRcItemArea;
					if ( m_PntRcItemUpdt.left < m_PntRcUpdt.left )
					{
						m_PntRcItemUpdt.left	=	m_PntRcUpdt.left;
					}
					if ( m_PntRcItemUpdt.right > m_PntRcUpdt.right )
					{
						m_PntRcItemUpdt.right	=	m_PntRcUpdt.right;
					}
					if ( m_PntRcItemUpdt.top < m_PntRcUpdt.top )
					{
						m_PntRcItemUpdt.top	=	m_PntRcUpdt.top;
					}
					if ( m_PntRcItemUpdt.bottom > m_PntRcUpdt.bottom )
					{
						m_PntRcItemUpdt.bottom	=	m_PntRcUpdt.bottom;
					}
					// check able display
					if ( (m_PntRcItemUpdt.left < m_PntRcItemUpdt.right) && (m_PntRcItemUpdt.top < m_PntRcItemUpdt.bottom) )
					{
						// init element paint
						m_PntRcElemArea.left		=	m_PntRcItemArea.left + (__s32)m_ParaLbxBase.ElemStX;
						m_PntRcElemArea.top			=	m_PntRcItemArea.top + (__s32)m_ParaLbxBase.ElemStY;
						m_PntRcElemArea.bottom	=	m_PntRcElemArea.top + (__s32)m_ParaLbxBase.ElemSizeY;
						// paint element
						for ( m_PntElemIdx=0;m_PntElemIdx < (__s32)m_ParaLbxBase.ElemCnt;m_PntElemIdx++ )
						{
							// check element paint able
							if ( m_PntElemIdx >= E_GrP3DoLbxElemViewCnt )
							{
								break;
							}
							// paint element
							m_PntRcElemArea.right		=	m_PntRcElemArea.left + (__s32)m_ParaLbxBase.ElemSizeX[m_PntElemIdx];
							LcPaintElem();
							// next
							m_PntRcElemArea.left		=	m_PntRcElemArea.right + (__s32)m_ParaLbxBase.ElemGap;
						}

						// get item status
						Tv_ItemStat	=	E_GrP3DoLbxItemStatDisable;
						if ( Tv_IsCanEable )
						{
							Tv_ItemStat	=	E_GrP3DoLbxItemStatNormal;
							if ( m_CusIdx == m_PntItemIdx )
							{
								Tv_ItemStat	=	E_GrP3DoLbxItemStatCursor;
							}
							if ( m_SelIdx == m_PntItemIdx )
							{
								Tv_ItemStat	=	E_GrP3DoLbxItemStatSel;
							}
						}
						// paint item background
						m_IsPntText	=	FALSE;
						PaintScriptPush( m_ParaLbxBase.PntItmStat[Tv_ItemStat], &m_PntRcItemArea );
					}
				}
				// next
				m_PntRcItemArea.top	=	m_PntRcItemArea.top + (__s32)m_ParaLbxBase.ItemSizeY + (__s32)m_ParaLbxBase.ItemGap;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::LcPaintElem( void )
{
	// local -------------------
		__s32		Tv_ElemVal;
	// code --------------------
		// calculate element update area
		m_PntRcElemUpdt	=	m_PntRcElemArea;
		if ( m_PntRcElemUpdt.left < m_PntRcItemUpdt.left )
		{
			m_PntRcElemUpdt.left	=	m_PntRcItemUpdt.left;
		}
		if ( m_PntRcElemUpdt.right > m_PntRcItemUpdt.right )
		{
			m_PntRcElemUpdt.right	=	m_PntRcItemUpdt.right;
		}
		if ( m_PntRcElemUpdt.top < m_PntRcItemUpdt.top )
		{
			m_PntRcElemUpdt.top	=	m_PntRcItemUpdt.top;
		}
		if ( m_PntRcElemUpdt.bottom > m_PntRcItemUpdt.bottom )
		{
			m_PntRcElemUpdt.bottom	=	m_PntRcItemUpdt.bottom;
		}
		// check able display
		if ( (m_PntRcElemUpdt.left < m_PntRcElemUpdt.right) && (m_PntRcElemUpdt.top < m_PntRcElemUpdt.bottom) )
		{
			// init
			m_IsPntText	=	TRUE;
			// check icon mode
			if ( E_GrP3DoLbxElemDispIcon == m_ParaLbxBase.ElemView[m_PntElemIdx] )
			{
				// element value get
				if ( LcPntElemValGet( &Tv_ElemVal ) )
				{
					Tv_ElemVal	=	Tv_ElemVal + m_ParaLbxBase.ElemValAdd[m_PntElemIdx];
					// check range icon
					if ( (0 <= Tv_ElemVal) && (E_GrP3DoLbxIconMaxCnt > Tv_ElemVal) )
					{
						// draw icon
						m_IsPntText	=	FALSE;	// no text display
						PaintScriptPush( m_ParaLbxBase.PntIcon[Tv_ElemVal], &m_PntRcElemArea );
					}
				}
				m_IsPntText	=	FALSE;	// no text display
			}
			// draw element back ground
			PaintScriptPush( m_ParaLbxBase.PntElem[m_PntElemIdx], &m_PntRcElemArea );
		}

}
//--------------------------------------------------------------------
__s32		Cls_GrP3DoLbxBase::LcScrPosToItemIdx( __u16 A_X, __u16 A_Y )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_StX;
		__s32		Tv_EdX;
		__s32		Tv_StY;
		__s32		Tv_UnitY;
		__s32		Tv_ItmIdx;
		__s32		Tv_ItemY;
	// code --------------------
		Tv_Result	=	-1;

		// check horizontal
		Tv_StX	=	m_PntRcScr.left + (__s32)m_ParaLbxBase.ContStX;
		Tv_EdX	=	Tv_StX + (__s32)m_ParaLbxBase.ItemSizeX;
		if ( (Tv_StX <= (__s32)A_X) && (Tv_EdX > (__s32)A_X) )
		{
			// check start position
			Tv_StY	=	m_PntRcScr.top + (__s32)m_ParaLbxBase.ContStY;
			if ( Tv_StY <= (__s32)A_Y )
			{
				// check valid item height
				Tv_UnitY	=	(__s32)m_ParaLbxBase.ItemSizeY + (__s32)m_ParaLbxBase.ItemGap;
				if ( 0 != Tv_UnitY )
				{
					// check count limit
					Tv_ItmIdx	=	((__s32)A_Y - Tv_StY) / Tv_UnitY;
					if ( m_ItemCnt > (__u32)Tv_ItmIdx )
					{
						// check gap area
						Tv_ItemY	=	(Tv_ItmIdx * Tv_UnitY) + Tv_StY + (__s32)m_ParaLbxBase.ItemSizeY;
						if ( Tv_ItemY > (__s32)A_Y )
						{
							// found
							Tv_Result	=	Tv_ItmIdx;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::ProcEvtFocusGet( void )
{
		Cls_GrP3DoBase::ProcEvtFocusGet();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::ProcEvtFocusLoss( void )
{
		m_IsCtrlMode	=	FALSE;
		Cls_GrP3DoBase::ProcEvtFocusLoss();
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::LcViewPosPatch( void )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_ParaLbxBase.ItemViewCnt )
		{
			// check now count
			if ( 0 != m_ItemCnt )
			{
				// patch cursor position
				if ( m_CusIdx >= (__s32)m_ItemCnt )
				{
					m_CusIdx	=	(__s32)m_ItemCnt - 1;
				}
				// check out of view
				if ( m_CusIdx < m_ViewPos  )
				{
					m_ViewPos	=	m_CusIdx;
				}
				else if ( m_CusIdx >= (m_ViewPos + (__s32)m_ParaLbxBase.ItemViewCnt) )
				{
					if ( m_CusIdx >= (__s32)m_ParaLbxBase.ItemViewCnt )
					{
						m_ViewPos	=	m_CusIdx + 1 - (__s32)m_ParaLbxBase.ItemViewCnt;
					}
					else
					{
						m_ViewPos	=	0;
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxBase::ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_ParaLbxBase.IsOnKeyCtrl || m_IsCtrlMode )
		{
			switch ( A_Key )
			{
				case E_GrP3KeyUp:
					if ( 0 != m_ItemCnt )
					{
						if ( 0 != m_CusIdx )
						{
							m_CusIdx --;
							Tv_Result	=	TRUE;
						}
						else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxUp] )
						{
							m_CusIdx	=	m_ItemCnt - 1;
							Tv_Result	=	TRUE;
						}
						// patch view position
						LcViewPosPatch();
					}
					break;
				case E_GrP3KeyDown:
					if ( 0 != m_ItemCnt )
					{
						if ( (m_CusIdx + 1) < (__s32)m_ItemCnt )
						{
							m_CusIdx ++;
							Tv_Result	=	TRUE;
						}
						else if ( m_ScptId == m_ParaDoBase.FcsMvTbl[E_GrP3DoFocusMvIdxDown] )
						{
							m_CusIdx	=	0;
							Tv_Result	=	TRUE;
						}
						// patch view position
						LcViewPosPatch();
					}
					break;
			}
		}

		// change event
		if ( Tv_Result )
		{
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxBase::ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key )
{
	// local -------------------
		BOOL8	Tv_Result;
		BOOL8	Tv_IsSel;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_IsSel	=	FALSE;
		if ( E_GrP3KeyEnter == A_Key )
		{
			if ( (0 != m_ItemCnt) && (m_CusIdx < (__s32)m_ItemCnt) )
			{
				// check already control mode
				if ( m_ParaLbxBase.IsOnKeyCtrl )
				{
					Tv_IsSel	=	TRUE;
				}
				else
				{
					if ( m_IsCtrlMode )
					{
						Tv_IsSel	=	TRUE;
					}
					else
					{
						m_IsCtrlMode	=	TRUE;
					}
				}
				// select mode
				if ( Tv_IsSel )
				{
					LcProcCusSel();
				}
			}
			Tv_Result	=	TRUE;
		}
		else if ( (E_GrP3KeyEsc == A_Key) || (E_GrP3KeyMenu == A_Key) )
		{
			if ( (!m_ParaLbxBase.IsOnKeyCtrl) && m_IsCtrlMode )
			{
				m_IsCtrlMode	=	FALSE;
				Tv_Result	=	TRUE;
			}
		}
		// redraw
		if ( Tv_Result )
		{
			ReqDraw();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxBase::ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap )
{
	// local -------------------
		__s32		Tv_ItmIdx;
		__s32		Tv_CusIdx;
	// code --------------------
		// check able
		if ( 0 != m_ItemCnt )
		{
			// move cursor
			Tv_ItmIdx	=	LcScrPosToItemIdx( A_X, A_Y );
			if ( 0 <= Tv_ItmIdx )
			{
				Tv_CusIdx	=	m_ViewPos + Tv_ItmIdx;
				if ( Tv_CusIdx < (__s32)m_ItemCnt )
				{
					// update
					m_CusIdx	=	Tv_CusIdx;
					LcProcCusSel();
					ReqDraw();
				}
			}
		}

		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrP3DoLbxBase::LcProcCusSel( void )
{
	// local -------------------
	// code --------------------
		// update select
		m_SelIdx	=	m_CusIdx;
		// call event 
		if ( E_GrP3AsmIdNone != m_ParaLbxBase.AsmEvtSel )
		{
			V_GrP3Mng->VmRunCode( m_ParaLbxBase.AsmEvtSel, this, NULL );
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3DoLbxBase::LcPntElemValGet( __s32* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
