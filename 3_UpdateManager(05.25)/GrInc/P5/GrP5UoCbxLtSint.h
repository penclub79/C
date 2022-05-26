/*
Platform 5 combo box local text setup integer

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxLtSint
#define	_EPC_GrP5UoCbxLtSint

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoCbxLtBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCbxLtSintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;
	__u16	TxtIdOutVal;
	__u16	_rsvd1;
}	*Ptr_GrP5UoCbxLtSintPara;

//====================================================================
//global class

class Cls_GrP5UoCbxLtSint	: public Cls_GrP5UoCbxLtBase
{
	protected:

		St_GrP5UoCbxLtSintPara	m_ParaUoCbxLtSint;

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
		Cls_GrP5UoCbxLtSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoCbxLtSint(void);

		// basic information

		// control
		void	SetupAdrSet(__u16 A_Adr);
		__u16	SetupAdrGet(__u16 A_Adr);

		BOOL8	ItemAdd(WCHAR* A_StrTxt, __u32 A_Data);
		void	ItemClear(void);

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

