/*
	Gause Platform virtual keyboard text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoVkbdTxt
#define	_EPC_GrP3DoVkbdTxt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>
#include	<P3/GrP3DoVkbdBase.h>

//====================================================================
//constance

#define E_GrP3DoVkbdTxtCombNone			0
#define E_GrP3DoVkbdTxtCombMid			1
#define E_GrP3DoVkbdTxtCombDblMid		2			/* middle last able */
#define E_GrP3DoVkbdTxtCombLast			3

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class	Cls_GrP3DoVkbdTxt	:	public	Cls_GrP3DoVkbdBase
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
		Cls_GrP3DoVkbdTxt( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, Cls_GrP3DoBase* A_Host,
			WCHAR* A_StrPtr, __u32 A_MaxLen, BOOL8 A_IsPasswd, BOOL8 A_IsAdvPasswd );
		virtual	~Cls_GrP3DoVkbdTxt( void );

		//runtime
		virtual	void	RtlInit( void );	// initialize
		virtual	void	RtlCallbackSlave( Cls_GrP3DoBase* A_Obj );	// call by slave

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

