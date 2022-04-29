/*
Platform 5 spin local text base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoSpinLtBase
#define	_EPC_GrP5UoSpinLtBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoSpinBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5SpinLtItemMaxCnt			32

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoSpinLtBasePara
{
	BOOL8	IsEdit;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
}	*Ptr_GrP5UoSpinLtBasePara;

typedef struct St_GrP5UoSpinLtItem
{
	__u32	UserData;
	WCHAR*	StrTxt;
}	*Ptr_GrP5UoSpinLtItem;

//====================================================================
//global class

class Cls_GrP5UoSpinLtBase	: public Cls_GrP5UoSpinBase
{
	protected:

		St_GrP5UoSpinLtBasePara	m_ParaUoSpinLtBase;

		St_GrP5UoSpinLtItem	m_Items[E_GrP5SpinLtItemMaxCnt];
		__u32	m_ItemCnt;
		__u32	m_SelIdx;

		// inherited
		virtual	BOOL8	DoIncrease(void);
		virtual	BOOL8	DoDecrease(void);
		virtual	void	ValueRead(void);

		virtual	void	ProcSpinClickCont(void);
				
#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);
		virtual	void	EvtVkbdInput(void);
		virtual	void	EvtVkbdCancel(void);

	public:
		Cls_GrP5UoSpinLtBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoSpinLtBase(void);

		// basic information
		
		// control
		__u32	SelIdxGet(void);
		void	SelIdxSet(__u32 A_Idx);

		__s32	ItemAdd(WCHAR* A_Str, __u32 A_UserData = 0);
		BOOL8	ItemTextGet(__u32 A_Idx, WCHAR* A_StrRt);
		BOOL8	ItemTextSet(__u32 A_Idx, WCHAR* A_Str);
		BOOL8	ItemUserDataGet(__u32 A_Idx, __u32* A_PtrRtUserData);
		BOOL8	ItemUserDataSet(__u32 A_Idx, __u32 A_UserData);

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

