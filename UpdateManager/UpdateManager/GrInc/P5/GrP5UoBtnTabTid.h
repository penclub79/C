/*
Platform 5 Button tab text id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnTabTid
#define	_EPC_GrP5UoBtnTabTid

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnTabBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoBtnTabTidPara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u16		TxtId;		// text id
	__u8		_rsvd0;
	__u8		FontId;
	St_GrP5RelRect	TxtRc;

	St_GrP5RelRect	ImgRc;

	__u8		ImgAlignHz;
	__u8		ImgAlignVt;
	BOOL8		IsImgSprite;
	__u8		_rsvd1;

	__u16		ImgNormal;
	__u16		ImgDisable;
	__u16		ImgPush;
	__u16		ImgOnFocus;
	__u16		ImgOnSelected;
	__u16		_rsvd2;

}	*Ptr_GrP5UoBtnTabTidPara;

//====================================================================
//global class

class Cls_GrP5UoBtnTabTid	: public Cls_GrP5UoBtnTabBase
{
	protected:

		St_GrP5UoBtnTabTidPara	m_ParaUoBtnTabTid;

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
		Cls_GrP5UoBtnTabTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnTabTid(void);

		// basic information



		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

