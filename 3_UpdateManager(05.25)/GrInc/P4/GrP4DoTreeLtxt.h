/*
	Gauss Platform tree local text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoTreeLtxt
#define	_EPC_GrP4DoTreeLtxt

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoTreeBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoTreeLtxtPara
{
	__u16	PntValue;		// value paint code
	__u16	_rsvd0;
}	*Ptr_GrP4DoTreeLtxtPara;

typedef struct St_GrP4DoTreeLtxtItem
{
	Def_WstrTag	StrVal;
}	*Ptr_GrP4DoTreeLtxtItem;

//====================================================================
//global class
class	Cls_GrP4DoTreeLtxt	:	public	Cls_GrP4DoTreeBase
{
	protected:
		St_GrP4DoTreeLtxtPara	m_ParaTreeLtxt;

		St_GrP4DoTreeLtxtItem	m_TreeLtxtItemTbl[E_GrP4DoTreeBaseItemMaxCnt];

		// method
		void	LcProcUpiItemTextSet(__u32 A_ItemId, __u32 A_SlotIdx, WCHAR* A_StrVal);

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	LcPntValue(void);

	public:
		Cls_GrP4DoTreeLtxt(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoTreeLtxt( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet(__u8 A_Idx, __s32* A_PtrRtCus);

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

