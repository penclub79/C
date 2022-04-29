/*
Platform 5 TAB Text id relate

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoTabTiRel
#define	_EPC_GrP5UoTabTiRel

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoTabBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoTabTiRelPara
{
	St_GrP5FontDrawStyle	LblTxtPara;
	St_GrP5RelRect	LblTxtRc;		// label text rect
	St_GrP5RelRect	LblBgRc;		// label background rect
	St_GrP5RelRect	IconRc;			// icon rect
	__u8	LblFontId;						// label font id
	__u8	IconAlignHz;
	__u8	IconAlignVt;
	BOOL8 IsIconSprite;
}	*Ptr_GrP5UoTabTiRelPara;

//====================================================================
//global class

class Cls_GrP5UoTabTiRel : public Cls_GrP5UoTabBase
{
	protected:

		St_GrP5UoTabTiRelPara	m_ParaUoTabTiRel;
		St_GrP5SkinCtlTabTiRel	m_SkinCtlTabTiRel;

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoTabTiRel(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoTabTiRel(void);

		// basic information

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

