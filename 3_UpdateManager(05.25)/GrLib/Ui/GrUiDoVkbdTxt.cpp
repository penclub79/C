/*
 osd object

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<Ui/GrUiDoVkbdTxt.h>
#include	<GrStrTool.h>
#include	<GrTimeTool.h>
#include	<Ui/GrUiMng.h>
#include	<Ui/GrUiConsole.h>
#include	<Ui/GrUiScptDec.h>

//====================================================================
//local const

//====================================================================
//local type

typedef	struct St_GrUiDoVkbdTxtKorLastDiv
{
	__u16	Last;			//new last
	__u16	First;		//new first
}	*Ptr_GrUiDoVkbdTxtKorLastDiv;


//====================================================================
//local macro
#define m_ObjConsole		((Cls_GrUiConsole*)m_ObjCon)

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrUiMng*				V_GrUiMngObj;

St_GrUiDoVkbdTxtKorLastDiv	V_GrUiDoVkbdTxtKorLastDivTbl[28]	=	
{
	//0			1				2				3				4				5				6				7				8				9
	{ 0, 0},{ 0, 1},{ 0, 2},{ 1,10},{ 0, 3},{ 4,13},{ 4,19},{ 0, 4},{ 0, 6},{ 8, 1},
	{ 8, 7},{ 8, 8},{ 8,10},{ 8,17},{ 8,18},{ 8,19},{ 0, 7},{ 0, 8},{17,10},{ 0,10},
	{ 0,11},{ 0,12},{ 0,13},{ 0,15},{ 0,16},{ 0,17},{ 0,18},{ 0,19}
};


//====================================================================
//--------------------------------------------------------------------
Cls_GrUiDoVkbdTxt::Cls_GrUiDoVkbdTxt(	__u16 A_WinId, __u16 A_ObjId, BOOL8 A_IsPasswd, __u32 A_VkId, WCHAR* A_StrEdit, __u32 A_LenMax, BOOL8 A_IsPassChk,
																		 BOOL8 A_IsLangLock, __u8 A_ConIdx, BOOL8 A_IsUpdtParm ):
Cls_GrUiDoVkbdBase( A_WinId, A_ObjId, A_IsPasswd, A_VkId, A_StrEdit, A_LenMax, A_IsPassChk, A_ConIdx, FALSE )
{
	// local -------------------
		BOOL8		Tv_IsSlave;
	// code --------------------
		//init
		m_FrmPage			=	NULL;

		m_ObjId				=	E_GrUiObjIdWinVkbdText;

		V_GrUiMngObj->ScptAtbGet( m_ObjId, &m_Layer, &m_Rect, &Tv_IsSlave  );
		m_CombStage		=	E_GrUiDoVkbdTxtCombNone;
		m_IsPassChk		=	A_IsPassChk;
		m_IsLangLock	=	A_IsLangLock;

		if ( A_IsUpdtParm )
		{
			UpdateWinParam();
		}
		//cursor end
		VkCusEnd();
}
//--------------------------------------------------------------------
Cls_GrUiDoVkbdTxt::~Cls_GrUiDoVkbdTxt()
{
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcUpdtPageInfo( void )
{
	// local -------------------
		__u16		Tv_StIdx;
		__u16		Tv_EdIdx;
		__u16		Tv_WkIdx;
		__u8		Tv_TabIdx;
	// code --------------------
		//get page count
		m_FrmPage->FrmPageClear();
		Tv_StIdx		=	E_GrUiObjIdPageVkbdLang0A + ( E_GrUiVkbdPageMax * (__u16)V_GrUiMngObj->LanguageGet() );
		Tv_EdIdx		=	Tv_StIdx + E_GrUiVkbdPageMax;
		Tv_TabIdx		=	0;
		for ( Tv_WkIdx=Tv_StIdx;Tv_WkIdx < Tv_EdIdx;Tv_WkIdx++ )
		{
			if ( !V_GrUiMngObj->IsScptExist( Tv_WkIdx ) )
			{
				break;
			}
			//add page
			m_FrmPage->FrmPageObjIdSet( Tv_TabIdx, Tv_WkIdx );
			Tv_TabIdx ++;
		}
		m_FrmPage->FrmPageViewFirst();
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcProcTxtKfBtn( Cls_GrUiDoBtn* A_Btn )
{
	// local -------------------
		__u16	Tv_CodeIn;
	// code --------------------
		//get code
		Tv_CodeIn		=	VkBtnCodeGet( A_Btn );
		//insert new code
		VkCodeIns( Tv_CodeIn, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcProcTxtKmBtn( Cls_GrUiDoBtn* A_Btn )
{
	// local -------------------
		__u16	Tv_CodeIn;
		__u8	Tv_KorFir;
		__u8	Tv_KorMid;
		__u8	Tv_KorLast;
	// code --------------------
		//get code
		Tv_CodeIn		=	VkBtnCodeGet( A_Btn );
		//check modifyable
		
		GrStrUniKorDec( m_StrEdt[m_CusPos], &Tv_KorFir, &Tv_KorMid, &Tv_KorLast );
		//change code
		Tv_KorMid	=	GrStrUniKorJamoToMiddle( Tv_CodeIn );
		Tv_CodeIn	=	GrStrUniKorEnc( Tv_KorFir, Tv_KorMid, Tv_KorLast );
		VkCodeOvwt( Tv_CodeIn );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcProcTxtKlBtn( Cls_GrUiDoBtn* A_Btn )
{
	// local -------------------
		__u16	Tv_CodeIn;
		__u8	Tv_KorFir;
		__u8	Tv_KorMid;
		__u8	Tv_KorLast;
	// code --------------------
		//get code
		Tv_CodeIn		=	VkBtnCodeGet( A_Btn );
		//check modifyable

		GrStrUniKorDec( m_StrEdt[m_CusPos], &Tv_KorFir, &Tv_KorMid, &Tv_KorLast );
		//change code
		Tv_KorLast	=	GrStrUniKorJamoToLast( Tv_CodeIn );
		Tv_CodeIn		=	GrStrUniKorEnc( Tv_KorFir, Tv_KorMid, Tv_KorLast );
		VkCodeOvwt( Tv_CodeIn, TRUE );
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::ProcEvtWinCreate( void )
{
	// local -------------------
	// code --------------------
		//inherited
		Cls_GrUiDoVkbdBase::ProcEvtWinCreate();
		//get frame address
		m_FrmPage		=	(Cls_GrUiDoFrm*)FindSlaveObjByObjId( (__u16)m_UserDataTbl[1] );
		//check exist frame
		if ( NULL != m_FrmPage )
		{
			//update page info
			LcUpdtPageInfo();
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::ProcEvtGlbStatChg( __u8 A_Type )
{
		//inherited
		Cls_GrUiDoVkbdBase::ProcEvtGlbStatChg( A_Type );
		//check language change
		if ( E_GrUiChgLanguage == A_Type )
		{
			//check exist frame
			if ( NULL != m_FrmPage )
			{
				//update page info
				LcUpdtPageInfo();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::ProcEvtBtnUp( Cls_GrUiDoBase* A_Obj )
{
	// local -------------------
		BOOL8		Tv_IsDfltProc;
		__u16		Tv_Code;
		__u32		Tv_UserData;
		__u32		Tv_Type;
		__u16		Tv_First;
		__u16		Tv_Mid;
		__u16		Tv_Last;
		__u16		Tv_Ovwt;
		__u16		Tv_Cmb;
		__u16		Tv_Nxt;
	// code --------------------
		//check button type
		Tv_IsDfltProc	=	TRUE;
		Tv_UserData		=	A_Obj->UserDataGet();
		Tv_Type	=	E_GrUiDoVkbdUdcTypeMask & Tv_UserData;
		//if (E_GrUiDoVkbdUdcTypeNormal == Tv_Type)
		if ( (E_GrUiDoVkbdUdcTypeNormal == Tv_Type) || (E_GrUiDoVkbdUdcTypeKorFst == Tv_Type) || (E_GrUiDoVkbdUdcTypeKorMid == Tv_Type) )
		{
			Tv_Code	=	VkBtnCodeGet( (Cls_GrUiDoBtn*)A_Obj );
			//check korean code
			if ( ((0x3131 <= Tv_Code) && (0x3163 >= Tv_Code)) || ((0xAC00 <= Tv_Code) && (0xD7A3 >= Tv_Code)) )
			{
				//korean
				if ( E_GrUiDoVkbdTxtCombNone != m_CombStage )
				{
					LcKorDivCode( m_StrEdt[m_CusPos], &Tv_First, &Tv_Mid, &Tv_Last );
					switch( m_CombStage )
					{
						case	E_GrUiDoVkbdTxtCombMid:
							Tv_Mid	=	LcKorMidCodeGet(Tv_Code);
							if ( 0 != Tv_Mid )
							{
								Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
								//combination
								VkCodeOvwt( Tv_Ovwt );
								m_CombStage	=	LcKorNextStageGet( Tv_Code );	//go next stage
							}
							else
							{
								//move next
								VkCusRight();
								VkCodeIns( Tv_Code, FALSE );
							}
							break;
						case	E_GrUiDoVkbdTxtCombDblMid:
							Tv_Cmb	=	LcKorMidCodeGet( Tv_Code );
							if ( 0 != Tv_Cmb )
							{
								Tv_Cmb	=	LcKorMidCombine( Tv_Mid, Tv_Cmb );
							}
							if ( 0 != Tv_Cmb )
							{
								//update code
								Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Cmb, Tv_Last );
								//combination
								VkCodeOvwt( Tv_Ovwt );
								m_CombStage	=	E_GrUiDoVkbdTxtCombLast;
							}
							else
							{
								//tail
								Tv_Last	=	LcKorLastCodeGet( Tv_Code );
								if ( 0 != Tv_Last )
								{
									//update code
									Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
									//combination
									VkCodeOvwt( Tv_Ovwt );
									m_CombStage	=	E_GrUiDoVkbdTxtCombLast;
								}
								else
								{
									//move next
									VkCusRight();
									VkCodeIns( Tv_Code, FALSE );
								}
							}
							break;
						case	E_GrUiDoVkbdTxtCombLast:
							//check exist last code
							Tv_Cmb	=	LcKorLastCodeGet( Tv_Code );
							if ( 0 != Tv_Cmb )
							{
								//check last code
								if ( 0 == Tv_Last )
								{
									Tv_Last	=	Tv_Cmb;
									//update code
									Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
									//combination
									VkCodeOvwt( Tv_Ovwt );
								}
								else
								{
									//try combination
									Tv_Cmb	=	LcKorLastCombine( Tv_Last, Tv_Cmb );
									if ( 0 != Tv_Cmb )
									{
										//update code
										Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Cmb );
										//combination
										VkCodeOvwt( Tv_Ovwt );
									}
									else
									{
										//write first code
										VkCusRight();
										VkCodeIns( Tv_Code, FALSE );
										m_CombStage	=	LcKorNextStageGet( Tv_Code );
									}
								}
							}
							else
							{
								//check middle code
								Tv_Nxt	=	LcKorMidCodeGet( Tv_Code );
								if ( 0 != Tv_Nxt )
								{
									//divide last code
									LcKorLastDiv( Tv_Last, &Tv_Last, &Tv_Cmb );
									Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
									VkCodeOvwt( Tv_Ovwt, TRUE );
									Tv_Ovwt	=	LcKorCombine( Tv_Cmb, Tv_Nxt, 0 );
									VkCodeIns( Tv_Ovwt, FALSE );
								}
								else
								{
									//go next
									//move next
									VkCusRight();
									VkCodeIns( Tv_Code, FALSE );
								}
								m_CombStage	=	LcKorNextStageGet( Tv_Code );
							}
							break;
					}
				}
				else
				{
					//combinationalble code
					m_CombStage	=	LcKorNextStageGet( Tv_Code );
					VkCodeIns( Tv_Code, FALSE );
				}
				Tv_IsDfltProc	=	FALSE;
			}
			else
			{
				//other
				if ( E_GrUiDoVkbdTxtCombNone != m_CombStage )
				{
					VkCusRight();
				}
				m_CombStage	=	E_GrUiDoVkbdTxtCombNone;
			}
		}
		else
		{
			//special key - reset stage
			if ( E_GrUiDoVkbdTxtCombNone != m_CombStage )
			{
				VkCusRight();
			}
			m_CombStage	=	E_GrUiDoVkbdTxtCombNone;
		}
		if ( Tv_IsDfltProc )
		{
			Cls_GrUiDoVkbdBase::ProcEvtBtnUp( A_Obj );
		}
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcKorDivCode( __u16 A_Code, __u16* A_PtrRtFirst, __u16* A_PtrRtMid, __u16* A_PtrRtLast )
{
	// local -------------------
		__u16	Tv_Code;
	// code --------------------
		*A_PtrRtFirst	=	0;
		*A_PtrRtMid		=	0;
		*A_PtrRtLast	=	0;	
		if ( 0xAC00 <= A_Code )
		{
			//johap
			Tv_Code	=	A_Code - 0xAC00;
			*A_PtrRtFirst	=	(Tv_Code / 0x24C) + 1;
			Tv_Code	=	Tv_Code % 0x24C;
			*A_PtrRtMid		=	(Tv_Code / 28) + 1;
			*A_PtrRtLast	=	Tv_Code % 28;
		}
		else
		{
			if ( 0x314F > A_Code )
			{
				//first only
				Tv_Code	=	A_Code - 0x3131;
				if ( 0x3134 <= A_Code )
				{
					Tv_Code --;
				}
				if ( 0x3137 <= A_Code )
				{
					Tv_Code	=	Tv_Code - 2;
				}
				if ( 0x3141 <= A_Code )
				{
					Tv_Code	=	Tv_Code - 7;
				}
				if ( 0x3145 <= A_Code )
				{
					Tv_Code	--;
				}
				*A_PtrRtFirst	=	Tv_Code + 1;
			}
			else
			{
				//middle only
				*A_PtrRtMid	=	A_Code - 0x314F + 1;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrUiDoVkbdTxt::LcIsKorCode( __u16 A_Code )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( ((0xAC00 <= A_Code) && (0xD7A3 >= A_Code)) || ((0x3131 <= A_Code) && (0x318E >= A_Code)) )
		{
			Tv_Result	=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrUiDoVkbdTxt::LcKorNextStageGet( __u16 A_Code )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrUiDoVkbdTxtCombNone;

		if ( (0x3131 <= A_Code) && (0x3163 >= A_Code) )
		{
			Tv_Result	=	E_GrUiDoVkbdTxtCombMid;
			if ( 0x314F <= A_Code )
			{
				Tv_Result	=	E_GrUiDoVkbdTxtCombLast;
				if ( (0x3157 == A_Code) || (0x315C == A_Code) || (0x3161 == A_Code) )
				{
					Tv_Result	=	E_GrUiDoVkbdTxtCombDblMid;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdTxt::LcKorMidCodeGet( __u16 A_Code )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( (0x314F <= A_Code) && (0x3163 >= A_Code) )
		{
			Tv_Result	=	A_Code - 0x314F + 1;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdTxt::LcKorCombine( __u16 A_First, __u16 A_Mid, __u16 A_Last )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		
		if ( (0 == A_Last) && ( (0 == A_First) || (0 == A_Mid) ) )
		{
			//jamo mode
			if ( 0 != A_First )
			{
				//jaum mode
				Tv_Result	=	0x3131 + A_First - 1;
				if ( 3 <= A_First )
				{
					Tv_Result	++;
				}
				if ( 4 <= A_First )
				{
					Tv_Result	=	Tv_Result + 2;
				}
				if ( 7 <= A_First )
				{
					Tv_Result	=	Tv_Result + 7;
				}
				if ( 10 <= A_First )
				{
					Tv_Result ++;
				}
			}
			else
			{
				//moum mode
				Tv_Result	=	0x314F + A_Mid - 1;
			}
		}
		else
		{
			Tv_Result	=	0xAC00 + ((A_First - 1) * 0x24C) + ((A_Mid - 1) * 28) + A_Last;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdTxt::LcKorMidCombine( __u16 A_Org, __u16 A_Add )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		//check 
		switch( A_Org )
		{
			case	9:
				if ( 1 == A_Add )
				{
					Tv_Result	=	10;
				}
				else if ( 2 == A_Add )
				{
					Tv_Result	=	11;
				}
				else if ( 21 == A_Add )
				{
					Tv_Result	=	12;
				}
				break;
			case	14:
				if ( 5 == A_Add )
				{
					Tv_Result	=	15;
				}
				else if ( 6 == A_Add )
				{
					Tv_Result	=	16;
				}
				else if ( 21 == A_Add )
				{
					Tv_Result	=	17;
				}
				break;
			case	19:
				if ( 21 == A_Add )
				{
					Tv_Result	=	20;
				}
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdTxt::LcKorLastCodeGet( __u16 A_Code )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( (0x3131 <= A_Code) && (0x314E >= A_Code) && ( 0x3138 != A_Code ) && ( 0x3143 != A_Code ) && ( 0x3149 != A_Code ) )
		{
			Tv_Result	=	A_Code - 0x3131 + 1;
			if ( 0x3138 < A_Code)
			{
				Tv_Result --;
			}
			if ( 0x3143 <= A_Code)
			{
				Tv_Result --;
			}
			if ( 0x3149 < A_Code)
			{
				Tv_Result --;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrUiDoVkbdTxt::LcKorLastCombine( __u16 A_Org, __u16 A_Add )
{
	// local -------------------
		__u16	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		switch( A_Org )
		{
			case	1:
				if (19 == A_Add)
				{
					Tv_Result	=	3;
				}
				break;
			case	4:
				if (22 == A_Add)
				{
					Tv_Result	=	5;
				}
				else if ( 27 == A_Add )
				{
					Tv_Result	=	6;
				}
				break;
			case	8:
				if ( 1 == A_Add )
				{
					Tv_Result	=	9;
				}
				else if ( 16 == A_Add )
				{
					Tv_Result	=	10;
				}
				else if ( 17 == A_Add )
				{
					Tv_Result	=	11;
				}
				else if ( 19 == A_Add )
				{
					Tv_Result	=	12;
				}
				else if ( 25 == A_Add )
				{
					Tv_Result	=	13;
				}
				else if ( 26 == A_Add )
				{
					Tv_Result	=	14;
				}
				else if ( 27 == A_Add )
				{
					Tv_Result	=	15;
				}
				break;
			case	17:
				if ( 19 == A_Add )
				{
					Tv_Result	=	18;
				}
				break;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::LcKorLastDiv( __u16 A_Org, __u16* A_PtrRtLast, __u16* A_PtrRtFirst )
{
	// local -------------------
	// code --------------------
		*A_PtrRtLast	=	V_GrUiDoVkbdTxtKorLastDivTbl[A_Org].Last;
		*A_PtrRtFirst	=	V_GrUiDoVkbdTxtKorLastDivTbl[A_Org].First;
}
//--------------------------------------------------------------------
void	Cls_GrUiDoVkbdTxt::ProcEvtPageChg( Cls_GrUiDoBase* A_ObjFrm, __u16 A_PageId )
{
	// local -------------------
		__u16*	Tv_PtrKeyTbl;
		__u32		Tv_KeyCnt;
		__u32		Tv_KeyIdx;
		Cls_GrUiDoBase*	Tv_Obj;
	// code --------------------
		// inherited
		Cls_GrUiDoVkbdBase::ProcEvtPageChg( A_ObjFrm, A_PageId );

		if ( m_IsPasswd || m_IsLangLock )
		{
			Tv_PtrKeyTbl	=	V_GrUiMngObj->LangKeyTblGet();
			// check language change object
			if ( NULL != Tv_PtrKeyTbl )
			{
				Tv_KeyCnt			=	V_GrUiMngObj->LangKeyCnt();

				// find language key
				for (Tv_KeyIdx=0;Tv_KeyIdx < Tv_KeyCnt;Tv_KeyIdx++)
				{
					Tv_Obj	=	FindSlaveObjByObjId( (__u16)Tv_PtrKeyTbl[Tv_KeyIdx] );
					if ( NULL != Tv_Obj )
					{
						//Tv_Obj->VisibleSet( FALSE );
						Tv_Obj->EnableSet( FALSE );
					}
				}

			}
		}
}
//--------------------------------------------------------------------
