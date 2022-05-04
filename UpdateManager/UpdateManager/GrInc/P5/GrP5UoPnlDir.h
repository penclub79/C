/*
Platform 5 panel direction

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlDir
#define	_EPC_GrP5UoPnlDir

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoPnlDirUpdtTimeStep		2

// dir status

#define	E_GrP5UoPnlDirMaskLeft				0x00000001
#define	E_GrP5UoPnlDirMaskRight				0x00000002
#define	E_GrP5UoPnlDirMaskUp					0x00000004
#define	E_GrP5UoPnlDirMaskDown				0x00000008


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoPnlDirPara
{
	__u8		IsDiagonal;
	__u8		_rsvd0;
	__u8		_rsvd2;
	__u8		_rsvd3;
}	*Ptr_GrP5UoPnlDirPara;

//====================================================================
//global class

class Cls_GrP5UoPnlDir	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoPnlDirPara	m_ParaUoPnlDir;
		St_GrP5SkinCtlPnlDir	m_SkinCtlPnlDir;

		BOOL8	m_IsMsePush;
		__u32	m_UpdtStep;

		// local function
		__u32	LcDirCalcByPos(__s32 A_X, __s32 A_Y );

		void	LcDirSet(__u32 A_Left, __u32 A_Right, __u32 A_Up, __u32 A_Down);
		void	LcNotifyEvtByMseStat(void);

		void	LcNotifyDirEvt(BOOL8 A_IsLeft, BOOL8 A_IsRigth, BOOL8 A_IsUp, BOOL8 A_IsDown);

		// event system
		virtual	void	EvtScptDecDone(void);

		virtual	BOOL8	EvtMseMove(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnDown(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	EvtMseLbtnUp(__s32 A_X, __s32 A_Y, __u8 A_BtnMap);
		virtual	void	EvtTimerTick(void);

		virtual	void	EvtMseLeave(void);	/* not need inerite */

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlDir(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlDir(void);

		// basic information
		virtual	BOOL8	IsKeyAble(void);
		virtual	BOOL8	IsMseAble(void);

		// control

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

