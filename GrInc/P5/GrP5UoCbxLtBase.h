/*
Platform 5 combo box local text base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxLtBase
#define	_EPC_GrP5UoCbxLtBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoCbxBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoCbxLtMaxItemCnt			32

//====================================================================
//global macro

//====================================================================
//global type

#if 0
typedef struct St_GrP5UoCbxLtBasePara
{
	__u8	ItemMaxCnt;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP5UoCbxLtBasePara;
#endif

typedef struct St_GrP5UoCbxLtItem
{
	WCHAR	StrBuf[E_GrP5ItemStrBufLen + 1];
	__u32	UserData;
}	*Ptr_GrP5UoCbxLtItem;

//====================================================================
//global class

class Cls_GrP5UoCbxLtBase	: public Cls_GrP5UoCbxBase
{
	protected:

		//St_GrP5UoCbxLtBasePara	m_ParaUoCbxLtBase;

		St_GrP5UoCbxLtItem	m_ItemTbl[E_GrP5UoCbxLtMaxItemCnt];
		__u32	m_ItemCnt;


		// inherited
		virtual	void	PopupItemBuild(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoCbxLtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoCbxLtBase(void);

		// basic information
		WCHAR*	ItemTextGet(__u32 A_ItemIdx);
		__u32		ItemUserDataGet(__u32 A_ItemIdx);

		// control

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

