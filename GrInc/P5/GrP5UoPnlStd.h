/*
Platform 5 Label base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlStd
#define	_EPC_GrP5UoPnlStd

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

typedef struct St_GrP5UoPnlStdPara
{
	__u32		Color;
	BOOL8		IsDrawBg;
	BOOL8		IsSprite;
	__u16		ImgId;
	St_GrP5RelRect	ImgRc;
	__u8		ImgAlignHz;
	__u8		ImgAlignVt;
	__u8		_rsvd0;
	__u8		_rsvd19;
}	*Ptr_GrP5UoPnlStdPara;

//====================================================================
//global class

class Cls_GrP5UoPnlStd	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoPnlStdPara	m_ParaUoPnlStd;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlStd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlStd(void);

		// basic information
		__u16	ImageIdGet(void);

		// control
		void	ImageIdSet(__u16 A_ImgId);

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

