/*
	Gause Platform check button static gvu mask

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoChkbSgm
#define	_EPC_GrP3DoChkbSgm

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoChkbBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoChkbSgmPara
{
	__u16	TxtIdFalse;
	__u16	TxtIdTrue;

	__u16	ValIdx;			// value index
	__u8	BitPos;
	__u8	_rsvd0;

}	*Ptr_GrP3DoChkbSgmPara;

//====================================================================
//global class
class	Cls_GrP3DoChkbSgm	:	public	Cls_GrP3DoChkbBase
{
	protected:
		St_GrP3DoChkbSgmPara	m_ParaChkbSgm;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoChkbSgm( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoChkbSgm( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

