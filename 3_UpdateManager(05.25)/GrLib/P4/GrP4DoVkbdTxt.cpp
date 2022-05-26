/*
		platform virtual keyboard text

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<P4/GrP4DoVkbdTxt.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<P4/GrP4Script.h>
#include	<P4/GrP4ConBase.h>
#include	<P4/GrP4AsmBase.h>
#include	<P4/GrP4Mng.h>
#include	<P4/GrP4UiCmd.h>
#include	<P4/GrP4DoBtnLbl.h>

#ifdef GR_P4_EDITOR
#include	<P4/Dvl/GrP4DvlBase.h>
#include	<P4/Dvl/GrP4DvlMng.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

typedef	struct St_GrP4DoVkbdTxtKorLastDiv
{
	__u16	Last;			//new last
	__u16	First;		//new first
}	*Ptr_GrP4DoVkbdTxtKorLastDiv;

//====================================================================
//local macro
#define E_GrP4DoVkbdTxtPassChkErr			459

//====================================================================
//local function

//====================================================================
//var
extern	Cls_GrP4Mng*	V_GrP4Mng;

St_GrP4DoVkbdTxtKorLastDiv	V_GrP4DoVkbdTxtKorLastDivTbl[28]	=	
{
	//0			1				2				3				4				5				6				7				8				9
	{ 0, 0},{ 0, 1},{ 0, 2},{ 1,10},{ 0, 3},{ 4,13},{ 4,19},{ 0, 4},{ 0, 6},{ 8, 1},
	{ 8, 7},{ 8, 8},{ 8,10},{ 8,17},{ 8,18},{ 8,19},{ 0, 7},{ 0, 8},{17,10},{ 0,10},
	{ 0,11},{ 0,12},{ 0,13},{ 0,15},{ 0,16},{ 0,17},{ 0,18},{ 0,19}
};

//====================================================================
//--------------------------------------------------------------------
Cls_GrP4DoVkbdTxt::Cls_GrP4DoVkbdTxt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
	WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd ):
Cls_GrP4DoVkbdBase( A_ScptId, A_PtrRect, A_PosRel, A_Layer, A_Host,A_StrPtr, A_MaxLen, A_IsPasswd )
{
	// local -------------------
	// code --------------------
		//init
		m_Lang		=	0;
		m_Page		=	0;

		m_CombStage	=	E_GrP4DoVkbdTxtCombNone;

		m_IsAdvPasswd	=	A_IsAdvPasswd;

		VkCusEnd();		// go end cursor
}
//--------------------------------------------------------------------
Cls_GrP4DoVkbdTxt::~Cls_GrP4DoVkbdTxt( void )
{
		//update modal result
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::RtlInit( void )
{
	// local -------------------
	// code --------------------
		Cls_GrP4DoVkbdBase::RtlInit();
		// find key button
		FindSlaveWithCallback();
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::RtlCallbackSlave( Cls_GrP4DoBase* A_Obj )
{
	// local -------------------
		__u32	Tv_KeyIdx;
		Def_WstrNum	Tv_StrKey;
	// code --------------------
		// check object class type
		if ( E_GrP4DoClassBtnLbl == A_Obj->ClassIdGet() )
		{
			// get key index
			Tv_KeyIdx	=	A_Obj->UserDataGet( 0 );
			if ( (0 != Tv_KeyIdx) && (E_GrP4VkeyTxtCnt >= Tv_KeyIdx) )
			{
				Tv_KeyIdx --;		// patch key index
				Tv_StrKey[1]	=	0;
				Tv_StrKey[0]	=	V_GrP4Mng->VkbdTxtKeyCodeGet( m_Page, (__u8)Tv_KeyIdx, m_Lang );
				((Cls_GrP4DoBtnLbl*)A_Obj)->TextSet( Tv_StrKey );
				A_Obj->ReqDraw();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::ProcVkbdKey( __u32 A_KeyIdx )
{
	// local -------------------
		__u8	Tv_KeyIdx;
		WCHAR	Tv_Code;
	// code --------------------
		if ( E_GrP4VkeyTxtCnt >= A_KeyIdx )
		{
			// text key
			Tv_KeyIdx	=	(__u8)A_KeyIdx - 1;
			Tv_Code	=	V_GrP4Mng->VkbdTxtKeyCodeGet( m_Page, (__u8)Tv_KeyIdx, m_Lang );
			LcProcTextKey( (__u16)Tv_Code );
		}
		else if ( (E_GrP4VkeyLang0 <= A_KeyIdx) && ((E_GrP4VkeyLang0 + E_GrP4ScptLangMaxCnt) > A_KeyIdx) )
		{
			// language key
			m_Lang	=	(__u8)A_KeyIdx - E_GrP4VkeyLang0;
			m_Page	=	0;
			FindSlaveWithCallback();	// rebuild text button
		}
		else
		{
			// special key
			switch ( A_KeyIdx )
			{
				case E_GrP4VkeyPagePlus:
					m_Page ++;
					if ( m_Page >= V_GrP4Mng->VkbdTxtPageCntGet( m_Lang ) )
					{
						m_Page	=	0;
					}
					FindSlaveWithCallback();
					break;
				case E_GrP4VkeyPageMinus:
					if ( 0 == m_Page )
					{
						m_Page	=	V_GrP4Mng->VkbdTxtPageCntGet( m_Lang );
						if ( 0 != m_Page )
						{
							m_Page --;
						}
					}
					else
					{
						m_Page --;
					}
					FindSlaveWithCallback();
					break;
				default:
					// check combination mode
					if ( E_GrP4DoVkbdTxtCombNone != m_CombStage )
					{
						VkCusRight();
						m_CombStage	=	E_GrP4DoVkbdTxtCombNone;
					}
					Cls_GrP4DoVkbdBase::ProcVkbdKey(A_KeyIdx);
					break;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::LcKorDivCode( __u16 A_Code, __u16* A_PtrRtFirst, __u16* A_PtrRtMid, __u16* A_PtrRtLast )
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
__u16	Cls_GrP4DoVkbdTxt::LcKorMidCodeGet( __u16 A_Code )
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
__u16	Cls_GrP4DoVkbdTxt::LcKorCombine( __u16 A_First, __u16 A_Mid, __u16 A_Last )
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
__u8	Cls_GrP4DoVkbdTxt::LcKorNextStageGet( __u16 A_Code )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrP4DoVkbdTxtCombNone;

		if ( (0x3131 <= A_Code) && (0x3163 >= A_Code) )
		{
			Tv_Result	=	E_GrP4DoVkbdTxtCombMid;
			if ( 0x314F <= A_Code )
			{
				Tv_Result	=	E_GrP4DoVkbdTxtCombLast;
				if ( (0x3157 == A_Code) || (0x315C == A_Code) || (0x3161 == A_Code) )
				{
					Tv_Result	=	E_GrP4DoVkbdTxtCombDblMid;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u16	Cls_GrP4DoVkbdTxt::LcKorMidCombine( __u16 A_Org, __u16 A_Add )
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
__u16	Cls_GrP4DoVkbdTxt::LcKorLastCodeGet( __u16 A_Code )
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
__u16	Cls_GrP4DoVkbdTxt::LcKorLastCombine( __u16 A_Org, __u16 A_Add )
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
void	Cls_GrP4DoVkbdTxt::LcKorLastDiv( __u16 A_Org, __u16* A_PtrRtLast, __u16* A_PtrRtFirst )
{
		*A_PtrRtLast	=	V_GrP4DoVkbdTxtKorLastDivTbl[A_Org].Last;
		*A_PtrRtFirst	=	V_GrP4DoVkbdTxtKorLastDivTbl[A_Org].First;
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::LcProcTextKey( __u16 A_Code )
{
	// local -------------------
		BOOL8	Tv_IsDfltProc;
		__u16	Tv_First;
		__u16	Tv_Mid;
		__u16	Tv_Last;
		__u16	Tv_Ovwt;
		__u16	Tv_Cmb;
		__u16	Tv_Nxt;
	// code --------------------
		// init
		Tv_IsDfltProc	=	TRUE;
		// check Korean code
		if ( ((0x3131 <= A_Code) && (0x3163 >= A_Code)) || ((0xAC00 <= A_Code) && (0xD7A3 >= A_Code)) )
		{
			//Korean
			if ( E_GrP4DoVkbdTxtCombNone != m_CombStage )
			{
				LcKorDivCode( m_StrPtr[m_CusPos], &Tv_First, &Tv_Mid, &Tv_Last );
				switch( m_CombStage )
				{
					case	E_GrP4DoVkbdTxtCombMid:
						Tv_Mid	=	LcKorMidCodeGet(A_Code);
						if ( 0 != Tv_Mid )
						{
							Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
							//combination
							VkCodeOvwt( (WCHAR)Tv_Ovwt, FALSE );
							m_CombStage	=	LcKorNextStageGet( A_Code );	//go next stage
						}
						else
						{
							//move next
							VkCusRight();
							VkCodeIns( A_Code, FALSE );
						}
						break;
					case	E_GrP4DoVkbdTxtCombDblMid:
						Tv_Cmb	=	LcKorMidCodeGet( A_Code );
						if ( 0 != Tv_Cmb )
						{
							Tv_Cmb	=	LcKorMidCombine( Tv_Mid, Tv_Cmb );
						}
						if ( 0 != Tv_Cmb )
						{
							//update code
							Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Cmb, Tv_Last );
							//combination
							VkCodeOvwt( (WCHAR)Tv_Ovwt, FALSE );
							m_CombStage	=	E_GrP4DoVkbdTxtCombLast;
						}
						else
						{
							//tail
							Tv_Last	=	LcKorLastCodeGet( A_Code );
							if ( 0 != Tv_Last )
							{
								//update code
								Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
								//combination
								VkCodeOvwt( (WCHAR)Tv_Ovwt, FALSE );
								m_CombStage	=	E_GrP4DoVkbdTxtCombLast;
							}
							else
							{
								//move next
								VkCusRight();
								VkCodeIns( (WCHAR)A_Code, FALSE );
							}
						}
						break;
					case	E_GrP4DoVkbdTxtCombLast:
						//check exist last code
						Tv_Cmb	=	LcKorLastCodeGet( A_Code );
						if ( 0 != Tv_Cmb )
						{
							//check last code
							if ( 0 == Tv_Last )
							{
								Tv_Last	=	Tv_Cmb;
								//update code
								Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
								//combination
								VkCodeOvwt( (WCHAR)Tv_Ovwt, FALSE );
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
									VkCodeOvwt( (WCHAR)Tv_Ovwt, FALSE );
								}
								else
								{
									//write first code
									VkCusRight();
									VkCodeIns( (WCHAR)A_Code, FALSE );
									m_CombStage	=	LcKorNextStageGet( A_Code );
								}
							}
						}
						else
						{
							//check middle code
							Tv_Nxt	=	LcKorMidCodeGet( A_Code );
							if ( 0 != Tv_Nxt )
							{
								//divide last code
								LcKorLastDiv( Tv_Last, &Tv_Last, &Tv_Cmb );
								Tv_Ovwt	=	LcKorCombine( Tv_First, Tv_Mid, Tv_Last );
								VkCodeOvwt( Tv_Ovwt, TRUE );
								Tv_Ovwt	=	LcKorCombine( Tv_Cmb, Tv_Nxt, 0 );
								VkCodeIns( (WCHAR)Tv_Ovwt, FALSE );
							}
							else
							{
								//go next
								//move next
								VkCusRight();
								VkCodeIns( (WCHAR)A_Code, FALSE );
							}
							m_CombStage	=	LcKorNextStageGet( A_Code );
						}
						break;
				}
			}
			else
			{
				//combinational code
				m_CombStage	=	LcKorNextStageGet( A_Code );
				VkCodeIns( (WCHAR)A_Code, FALSE );
			}
			Tv_IsDfltProc	=	FALSE;
		}
		else
		{
			// check processed combination stage
			if ( E_GrP4DoVkbdTxtCombNone != m_CombStage )
			{
				VkCusRight();
				m_CombStage	=	E_GrP4DoVkbdTxtCombNone;
			}
		}
		if ( Tv_IsDfltProc )
		{
			VkCodeIns( (WCHAR)A_Code, TRUE );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4DoVkbdTxt::ProcTxtUpdt( void )
{
	// local -------------------
		__u8	Tv_PassStat;
		Ptr_GrP4ScptCfgMdl	Tv_PtrCfgMdl;
	// code --------------------
		// check need check
		if ( m_IsAdvPasswd && V_GrP4Mng->IsAdvPassUse() )
		{
			Tv_PassStat	=	GrStrRulePasswdWstr( m_StrPtr );
			if((E_GrStrPasswdGood != Tv_PassStat))
			{
				//error text
				Tv_PtrCfgMdl	=	V_GrP4Mng->ConfigMdlAtbGet();
				V_GrP4Mng->NotifyBox(E_GrP4ConIdxFocus, Tv_PtrCfgMdl->NotifyId, NULL, V_GrP4Mng->TextPtrGet(E_GrP4DoVkbdTxtPassChkErr), 0, E_GrP4ScptIdNone);
				VkClear();
			}
		}
}
//--------------------------------------------------------------------

