/*
	GAUSE Platform list box catalog

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoLbxCtlg
#define	_EPC_GrP4DoLbxCtlg

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoBase.h>
#include <P4/GrP4DoLbxBase.h>

//====================================================================
//constance

#define E_GrP4DoLbxCtlgTextMax						63


//====================================================================
//global macro


//====================================================================
//global type
typedef	struct	St_GrP4DoLbxCtlgPara
{
	__u8	ElemCode[E_GrP4DoLbxElemViewCnt];
	__u32	PageSize;		// request page count

}	*Ptr_GrP4DoLbxCtlgPara;

//====================================================================
//global class
class	Cls_GrP4DoLbxCtlg	:	public	Cls_GrP4DoLbxBase
{
	protected:
		St_GrP4DoLbxCtlgPara	m_ParaLbxCtlg;

		__u8	m_CtlgId;

		Def_WstrTag	m_PntStrTbl[E_GrP4DoLbxItemDispMax][E_GrP4DoLbxElemViewCnt];

		// local
		virtual	BOOL8	LcPntElemValGet( __s32* A_PtrRtVal );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited


	public:
		Cls_GrP4DoLbxCtlg(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoLbxCtlg( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

