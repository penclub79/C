/*
Platform 5 Button tab text id

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoBtnImgTid
#define	_EPC_GrP5UoBtnImgTid

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoBtnImgBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoBtnImgTidPara
{
	St_GrP5FontDrawStyle	TxtPara;
	__u16		TxtId;		// text id
	__u8		FontId;
	__u8		_rsvd0;
	St_GrP5RelRect	TxtRc;
}	*Ptr_GrP5UoBtnImgTidPara;

//====================================================================
//global class

class Cls_GrP5UoBtnImgTid	: public Cls_GrP5UoBtnImgBase
{
	protected:

		St_GrP5UoBtnImgTidPara	m_ParaUoBtnImgTid;

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
		Cls_GrP5UoBtnImgTid(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoBtnImgTid(void);

		// basic information



		// runtime

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

