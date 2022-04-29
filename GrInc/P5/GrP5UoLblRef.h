/*
Platform 5 Label reference

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoLblRef
#define	_EPC_GrP5UoLblRef

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoLblBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoLblRefPara
{
	__u32	ClrCus;
}	*Ptr_GrP5UoLblRefPara;

//====================================================================
//global class

class Cls_GrP5UoLblRef : public Cls_GrP5UoLblBase
{
	protected:

		St_GrP5UoLblRefPara	m_ParaUoLblRef;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoLblRef(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoLblRef(void);

		// basic information

		// control
		void	TextPtrSet(WCHAR* A_StrText, BOOL8 A_IsPasswd);
		void	CursorPosSet(__s32 A_Pos);

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

