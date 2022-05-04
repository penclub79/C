/*
Platform 5 List base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoTblStd
#define	_EPC_GrP5UoTblStd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoTblBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoTblStdPara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u8	FontId;
	__u8	ItemHeight;
	BOOL8	_rsvd0;
	__u8	ColumCnt;

	__u16	ItemMaxCnt;
	BOOL8	IsLineSel;
	BOOL8	_rsvd1;

	__u16	ColSize[E_GrP5TblColumMaxCnt];
	BOOL8	IsCanSel[E_GrP5TblColumMaxCnt];

}	*Ptr_GrP5UoTblStdPara;
#endif

//====================================================================
//global class

class Cls_GrP5UoTblStd	: public Cls_GrP5UoTblBase
{
	protected:

		//St_GrP5UoTblStdPara	m_ParaUoTblStd;


		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoTblStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		
		virtual	~Cls_GrP5UoTblStd(void);

		// basic information


		// control
		virtual	__s32	ItemAdd(void);

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

