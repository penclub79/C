/*
Platform 5 Button check box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoChkImgTiBase
#define	_EPC_GrP5UoChkImgTiBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnTid.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoChkImgTiBasePara
{
	St_GrP5RelRect	ImgRc;
	__u8	ImgAlignHz;
	__u8	ImgAlignVt;
	BOOL8	IsImgSprite;
	__u8	_rsvd0;
	__u16	ImgIdNormal;
	__u16	ImgIdPush;
	__u16	ImgIdChecked;
	__u16	ImgIdDisable;
}	*Ptr_GrP5UoChkImgTiBasePara;

//====================================================================
//global class

class Cls_GrP5UoChkImgTiBase	: public Cls_GrP5UoBtnTid
{
	protected:

		St_GrP5UoChkImgTiBasePara	m_ParaUoChkImgTiBase;

		virtual	void	LcValueWrite(void);
		virtual	void	LcValueRead(void);

		// inherited
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif

		// draw

		// event system
		virtual	void	DoEvtBtnRelease(void);

		virtual	void	EvtPaintProc(void);
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoChkImgTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoChkImgTiBase(void);

		// basic information

		// control
		void	ValueSet(BOOL8 A_IsSet);
		BOOL8	ValueGet(void);

		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

