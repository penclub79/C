/*
Platform 5 Window standard

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinPopMenu
#define	_EPC_GrP5WinPopMenu

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinPopItemBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//global class

class Cls_GrP5WinPopMenu : public Cls_GrP5WinPopItemBase
{
	protected:

		St_GrP5SkinCtlWinPopMenu	m_SkinCtlWinPopMenu;

		__u16	m_Squad;			// menu squad
		__u16	m_CtrlId;			// event control id

		__u8	m_PopDepth;		// popup depth

		void	LcBuildMenu(void);

		void	LcItemSelect(__u32 A_ItemIdx);
		BOOL8	LcDoCtlEvtItemClick(__u32 A_ItemIdx);

		// inherited

		// event system
		virtual	void	EvtClickItem(__u32 A_ItmIdx);

		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtWinKeyFocusLoss(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinPopMenu(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, __u16 A_CtrlEvt, __u16 A_Squad);
		virtual	~Cls_GrP5WinPopMenu(void);

		// basic information

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

		// event
		//virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		//virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		//virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		//virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		//virtual	BOOL8	EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

