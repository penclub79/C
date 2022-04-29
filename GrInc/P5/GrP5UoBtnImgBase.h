/*
Platform 5 Button image base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnImgBase
#define	_EPC_GrP5UoBtnImgBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoBtnImgBasePara
{
	St_GrP5RelRect	ImgRc;

	__u8		ImgAlignHz;
	__u8		ImgAlignVt;
	BOOL8		IsImgSprite;
	BOOL8		IsSkinDraw;

	__u16		ImgNormal;
	__u16		ImgDisable;
	__u16		ImgPush;
	__u16		ImgOnFocus;
	__u16		ImgOnSelected;
	__u16		_rsvd0;

}	*Ptr_GrP5UoBtnImgBasePara;

//====================================================================
//global class

class Cls_GrP5UoBtnImgBase	: public Cls_GrP5UoBtnBase
{
	protected:

		St_GrP5UoBtnImgBasePara	m_ParaUoBtnImgBase;

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoBtnImgBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnImgBase(void);

		// basic information



		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

