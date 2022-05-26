/*
Platform 5 Window virtual keyboard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinVkbd
#define	_EPC_GrP5WinVkbd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5WinVkbdMaxTxtLen				63

#define E_GrP5WinVkbdKorCombNone			0
#define E_GrP5WinVkbdKorCombMid				1
#define E_GrP5WinVkbdKorCombDblMid		2			/* middle last able */
#define E_GrP5WinVkbdKorCombLast			3

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class

class Cls_GrP5WinVkbd	: public Cls_GrP5WinBase
{
	protected:

		__u16		m_HostObjId;
		__u16		m_EvtCtlId;
		__u32		m_UserData;
		WCHAR*	m_StrRslt;	// reseult
		__u32		m_MaxLen;
		BOOL8		m_IsPasswd;

		BOOL8		m_IsUpdtRslt;
		__u8		m_KorComb;
		__u32		m_CusPos;
		__u32		m_Length;
		
		WCHAR*	m_StrTxt;

		__u32		m_LangIdx;
		__u32		m_ShftIdx;

		// label object

		// local
		void	LcVkeyLangUpdate(void);
		void	LcSubVkeyLangSet(Cls_GrP5UoBase* A_UiObj);
		void	LcProcSpcKey(__u32 A_Cmd, __u32 A_Para);
		void	LcLangPrivous(void);
		void	LcLangNext(void);
		void	LcLangSet(__u32 A_LangIdx);
		void	LcShiftSet(void);

		void	LcKorDivCode(__u16 A_Code, __u16* A_PtrRtFirst, __u16* A_PtrRtMid, __u16* A_PtrRtLast);
		__u16	LcKorMidCodeGet(__u16 A_Code);
		__u16	LcKorCombine(__u16 A_First, __u16 A_Mid, __u16 A_Last);
		__u8	LcKorNextStageGet(__u16 A_Code);
		__u16	LcKorMidCombine(__u16 A_Org, __u16 A_Add);
		__u16	LcKorLastCodeGet(__u16 A_Code);
		__u16	LcKorLastCombine(__u16 A_Org, __u16 A_Add);
		void	LcKorLastDiv(__u16 A_Org, __u16* A_PtrRtLast, __u16* A_PtrRtFirst);

		BOOL8	LcJpnCharDakInfo(__u16 A_Code, __u16* A_PtrRtStd, __u16* A_PtrRtDak);
		BOOL8	LcJpnCharBidInfo(__u16 A_Code, __u16* A_PtrRtStd, __u16* A_PtrRtBid);
		void	LcJpnProcDakuon(void);
		void	LcJpnProcBidakuon(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	void	EvtBtnPress(__u16 A_ObjId);
		virtual	void	EvtBtnRelease(__u16 A_ObjId);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinVkbd(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, 
			WCHAR* A_StrTxt, __u32 A_MaxLen, BOOL8 A_IsPassword, __u16 A_HostObjId, __u16 A_EvtCtlId, __u32 A_UserData );
		virtual	~Cls_GrP5WinVkbd(void);

		// basic information

		// control
		void	VkAddChar(WCHAR A_Code);
		void	VkLabelRedraw(void);

		void	VkClear(void);
		void	VkCusHome(void);
		void	VkCusEnd(void);
		void	VkCusLeft(void);
		void	VkCusRight(void);
		void	VkCodeIns(WCHAR A_Code, BOOL8 A_IsMoveCus);
		void	VkCodeOvwt(WCHAR A_Code, BOOL8 A_IsMoveCus);
		void	VkBackspace(void);
		void	VkDelete(void);

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

