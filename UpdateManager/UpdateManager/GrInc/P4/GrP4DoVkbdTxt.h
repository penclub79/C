/*
	Gause Platform virtual keyboard text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoVkbdTxt
#define	_EPC_GrP4DoVkbdTxt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>
#include	<P4/GrP4DoVkbdBase.h>

//====================================================================
//constance

#define E_GrP4DoVkbdTxtCombNone			0
#define E_GrP4DoVkbdTxtCombMid			1
#define E_GrP4DoVkbdTxtCombDblMid		2			/* middle last able */
#define E_GrP4DoVkbdTxtCombLast			3

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP4DoVkbdTxt	:	public	Cls_GrP4DoVkbdBase
{
	protected:
		__u8		m_Lang;
		__u8		m_Page;
		__u8		m_CombStage;
		BOOL8		m_IsAdvPasswd;		// advance password

		// local
		void	LcKorDivCode( __u16 A_Code, __u16* A_PtrRtFirst, __u16* A_PtrRtMid, __u16* A_PtrRtLast );
		__u16	LcKorMidCodeGet( __u16 A_Code );
		__u16	LcKorCombine( __u16 A_First, __u16 A_Mid, __u16 A_Last );
		__u8	LcKorNextStageGet( __u16 A_Code );
		__u16	LcKorMidCombine( __u16 A_Org, __u16 A_Add );
		__u16	LcKorLastCodeGet( __u16 A_Code );
		__u16	LcKorLastCombine( __u16 A_Org, __u16 A_Add );
		void	LcKorLastDiv( __u16 A_Org, __u16* A_PtrRtLast, __u16* A_PtrRtFirst );
		void	LcProcTextKey( __u16 A_Code );

		// event


		// editor only

		// inherited
		virtual	void	ProcVkbdKey( __u32 A_KeyIdx );		// virtual key process
		virtual	void	ProcTxtUpdt( void );							// text updated

	public:
		Cls_GrP4DoVkbdTxt( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, Cls_GrP4DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd );
		virtual	~Cls_GrP4DoVkbdTxt( void );

		//runtime
		virtual	void	RtlInit( void );	// initialize
		virtual	void	RtlCallbackSlave( Cls_GrP4DoBase* A_Obj );	// call by slave

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

