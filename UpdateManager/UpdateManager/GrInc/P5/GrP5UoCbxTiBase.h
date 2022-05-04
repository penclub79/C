/*
Platform 5 combo box base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoCbxTiBase
#define	_EPC_GrP5UoCbxTiBase

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

#define	E_GrP5UoCbxTiMaxItemCnt				32

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoCbxTiItemInfo
{
	__u16	TxtId;		// text id
	__u16	_rsvd0;
	__s32	UserData;
}	*Ptr_GrP5UoCbxTiItemInfo;


typedef struct St_GrP5UoCbxTiBasePara
{
	__u8	ItemCnt;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	St_GrP5UoCbxTiItemInfo	ItemInfo[E_GrP5UoCbxTiMaxItemCnt];
}	*Ptr_GrP5UoCbxTiBasePara;

//====================================================================
//global class

class Cls_GrP5UoCbxTiBase	: public Cls_GrP5UoCbxBase
{
	protected:

		St_GrP5UoCbxTiBasePara	m_ParaUoCbxTiBase;

		// paint

		// inherited
		virtual	void	PopupItemBuild(void);

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoCbxTiBase(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);
		virtual	~Cls_GrP5UoCbxTiBase(void);

		// basic information

		// contorl

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

