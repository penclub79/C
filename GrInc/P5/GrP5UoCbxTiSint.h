/*
Platform 5 combo box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxTiSint
#define	_EPC_GrP5UoCbxTiSint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoCbxTiBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCbxTiSintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;

}	*Ptr_GrP5UoCbxTiSintPara;

//====================================================================
//global class

class Cls_GrP5UoCbxTiSint	: public Cls_GrP5UoCbxTiBase
{
	protected:

		St_GrP5UoCbxTiSintPara	m_ParaUoCbxTiSint;

		// local
		__u32	LcValueGet(void);
		void	LcSelIdxUpdtByVal(void);

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx);

	public:
		Cls_GrP5UoCbxTiSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoCbxTiSint(void);

		// basic information

		// control
		void	SetupAdrSet(__u16 A_Adr);
		__u16	SetupAdrGet(__u16 A_Adr);

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

