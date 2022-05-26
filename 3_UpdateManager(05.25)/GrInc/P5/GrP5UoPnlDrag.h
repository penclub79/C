/*
Platform 5 panel direction

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlDrag
#define	_EPC_GrP5UoPnlDrag

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

#if 0
typedef struct St_GrP5UoPnlDragPara
{
	__u8		IsDiagonal;
	__u8		_rsvd0;
	__u8		_rsvd2;
	__u8		_rsvd3;
}	*Ptr_GrP5UoPnlDragPara;
#endif

//====================================================================
//global class

class Cls_GrP5UoPnlDrag	: public Cls_GrP5UoBase
{
	protected:

		//St_GrP5UoPnlDragPara	m_ParaUoPnlDrag;
		St_GrP5SkinCtlPnlDrag	m_SkinCtlPnlDrag;

		BOOL8	m_IsMsePush;
		BOOL8	m_IsDragAble;

		__s32	m_StX;
		__s32	m_StY;

		// local function
		void	LcDragRectSet(__s32 A_X, __s32 A_Y);

		void	LcNotifyDragEvt(void);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);


#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlDrag(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlDrag(void);

		// basic information
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		// control
		void	DragAbleSet(BOOL8 A_IsAble);

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

