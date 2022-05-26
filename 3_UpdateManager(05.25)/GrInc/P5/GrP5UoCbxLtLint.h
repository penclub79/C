/*
Platform 5 combo box local text local integer

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxLtLint
#define	_EPC_GrP5UoCbxLtLint

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

#if 0
typedef struct St_GrP5UoCbxLtLintPara
{
	__u16	StpAdr;
	__u8	VarSize;
	__u8	_rsvd0;

}	*Ptr_GrP5UoCbxLtLintPara;
#endif

//====================================================================
//global class

class Cls_GrP5UoCbxLtLint	: public Cls_GrP5UoCbxLtBase
{
	protected:

		//St_GrP5UoCbxLtLintPara	m_ParaUoCbxLtLint;

		// inherited
		virtual	void	EvtListItemSel(__u16 A_ObjId, __s32 A_ItemIdx);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoCbxLtLint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoCbxLtLint(void);

		// basic information

		// control
		BOOL8	ItemAdd(WCHAR* A_StrTxt, __u32 A_Data);
		void	ItemClear(void);
		void	SelectIdxSet(__s32 A_Idx);

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

