/*
Platform 5 Window base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5WinBase
#define	_EPC_GrP5WinBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5WinBasePara
{
	BOOL8		IsInputLayerLock;
	BOOL8		IsMseSkip;
	BOOL8		IsKeySkip;
	BOOL8		IsKeyMseQuit;

	__u16		FocusScptId;
	BOOL8		IsMseGlb;			// mouse global
	BOOL8		IsAutoHide;

}	*Ptr_GrP5WinBasePara;

//====================================================================
//global class

class Cls_GrP5WinBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5WinBasePara	m_ParaWinBase;

		BOOL8	m_IsWinHide;
		__u8	m_Layer;
		__u16	m_WinKeyFcsObjId;
		
		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5WinBase(__u16 A_ScptId, __u8 A_ConIdx, __u8 A_LayerIdx, Ptr_GrP5RelRect A_PtrRelRect, Cls_GrP5UoBase* A_ObjPrnt = NULL);		// for window
		virtual	~Cls_GrP5WinBase(void);

		// basic information
		BOOL8	IsInputLayerLock(void);
		BOOL8	IsMseSkip(void);
		BOOL8	IsKeySkip(void);
		__u8	LayerGet(void);
		BOOL8	IsMseGlb(void);

		virtual	BOOL8	IsKeyAble(void);

		// basic control
		virtual	BOOL8	CloseRequest(void);

		// event
		virtual	void	EvtWinKeyFocusGet(void);
		virtual	void	EvtWinKeyFocusLoss(void);

		virtual	BOOL8	EvtKeyUp(__u8 A_Key);

		virtual	BOOL8	EvtMseRbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		// runtime
		__u16	RtlWinKeyFocusSlaveGet(void);
		void	RtlWinKeyFocusSlaveSet(__u16 A_ObjId);
		virtual	Cls_GrP5UoBase*	RtlUoFindKeyFocusAble(void);
		virtual	void	RtlDoEvtScptDecDone(BOOL8 A_IsFocusUpdt = TRUE);
		virtual	void	RtlDoEvtWinClose(void);
		virtual	void	RtlPaintProc(Cls_GrGdib* A_GdibScr, Ptr_GrRect A_PtrRcScr);
		void	RtlAutoHideUpdate(BOOL8 A_IsHide);

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
