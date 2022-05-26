/*
Platform 5 Window popup item base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinPopItemBase
#define	_EPC_GrP5WinPopItemBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5WinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

// mouse mode
#define	E_GrP5WinPopItemMseNormal			0
#define	E_GrP5WinPopItemMsePushSclUp	1
#define	E_GrP5WinPopItemMsePushSclDn	2
#define	E_GrP5WinPopItemMsePushTrack	3
#define	E_GrP5WinPopItemMsePushThumb	4

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5WinPopItemBaseSkinCtl
{
	__u32	ItemHeight;
	St_GrRect	RcCont;		// contents
	//St_GrP5SkinSbarCtl	Sbar;	// vertical only
}	*Ptr_GrP5WinPopItemBaseSkinCtl;

//====================================================================
//global class

class Cls_GrP5WinPopItemBase : public Cls_GrP5WinBase
{
	protected:
		St_GrP5SkinCtlWinPopItemBase	m_SkinCtlWinPopItemBase;

		__u8		m_MseMode;

		__s32		m_SbarVtBasePos;			// scroll vertical base

		// local function
		void	LcSclBarSetByMsePos(__s32 A_ScrY);

		// need inherted
		virtual	void	EvtClickItem(__u32 A_ItmIdx) = 0;

		// event system
		virtual	void	EvtScptDecDone(void);

		//virtual	void	EvtKeyFocusGet(__u32 A_Para);	/* not need inerite */
		virtual	void	EvtWinKeyFocusLoss(void);

		virtual	void	EvtMseLeave(void);

		//virtual	BOOL8	EvtKeyDown(__u8 A_Key);
		//virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		
#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinPopItemBase(__u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5WinPopItemBase(void);

		// basic information
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

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

