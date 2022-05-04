/*
	GAUSE Platform list box catalog

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoLbxCtlg
#define	_EPC_GrP3DoLbxCtlg

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3DoBase.h>
#include <P3/GrP3DoLbxBase.h>

//====================================================================
//constance

#define E_GrP3DoLbxCtlgTextMax						63


//====================================================================
//global macro


//====================================================================
//global type
typedef	struct	St_GrP3DoLbxCtlgPara
{
	__u8	ElemCode[E_GrP3DoLbxElemViewCnt];
	__u32	PageSize;		// request page count

}	*Ptr_GrP3DoLbxCtlgPara;

//====================================================================
//global class
class	Cls_GrP3DoLbxCtlg	:	public	Cls_GrP3DoLbxBase
{
	protected:
		St_GrP3DoLbxCtlgPara	m_ParaLbxCtlg;

		__u8	m_CtlgId;

		Def_WstrTag	m_PntStrTbl[E_GrP3DoLbxItemDispMax][E_GrP3DoLbxElemViewCnt];

		// local
		virtual	BOOL8	LcPntElemValGet( __s32* A_PtrRtVal );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited


	public:
		Cls_GrP3DoLbxCtlg( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoLbxCtlg( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

