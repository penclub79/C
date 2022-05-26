/*
Platform 5 combo box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxBase
#define	_EPC_GrP5UoCbxBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoCbxPopLayer		6

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCbxBasePara
{
	St_GrP5FontDrawStyle	CbxTpara;
	__u8	CbxFontId;
	__u8	PopDispCnt;
	__u16	_rsvd0;		// not use
	St_GrP5RelRect	CbxTxtRc;		// text rect

}	*Ptr_GrP5UoCbxBasePara;

//====================================================================
//global class

class Cls_GrP5UoCbxBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoCbxBasePara	m_ParaUoCbxBase;

		St_GrP5SkinCtlCbxBase	m_SkinCtlCbxBase;

		__s32			m_SelIdx;					// selected item index

		virtual	void	PopupItemBuild(void) = 0;

		void	LcDoPopup(void);

		// inherited
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	void	EvtKeyFocusGet(__u32 A_Para);
		virtual	void	EvtKeyFocusLoss(void);
		virtual	void	EvtMseEnter(void);
		virtual	void	EvtMseLeave(void);

		virtual	BOOL8	EvtKeyUp(__u8 A_Key);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

		virtual	void	EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx);
		virtual	void	EvtPopSelCancel(void);

	public:
		Cls_GrP5UoCbxBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoCbxBase(void);

		// basic information
		
		__s32	SelectIdxGet(void);

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

