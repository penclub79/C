/*
Platform 5 grid base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGridBase
#define	_EPC_GrP5UoGridBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGridBasePara
{
	BOOL8	IsDragSel;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP5UoGridBasePara;

//====================================================================
//global class

class Cls_GrP5UoGridBase	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoGridBasePara	m_ParaUoGridBase;
		St_GrP5SkinCtlGridBase	m_SkinCtlGridBase;

		void	DrawReqItemRect(__u32 A_IdxStX,__u32 A_IdxStY, __u32 A_IdxEdX,__u32 A_IdxEdY);
		void	DrawReqItemWaku(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY);
		virtual	void	DrawReqItemDrag(__u32 A_IdxStX, __u32 A_IdxStY, __u32 A_IdxEdX, __u32 A_IdxEdY);

		virtual	void	DoEvtGrpItemOneSel(void);
		virtual	void	DoEvtGrpItemAreaSel(void);

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

		virtual	BOOL8	EvtKeyDown(__u8 A_Key);	/* not need inerite */

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);	/* not need inerite */

		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);	/* not need inerite */
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP5UoGridBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGridBase(void);

		// basic information

		// control
		void	CellXyCntGet(__u32* A_PtrRtCntX, __u32* A_PtrRtCntY);

		virtual	__u32	CellValueGet(__u32 A_IdxX, __u32 A_IdxY)	=	0;

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

