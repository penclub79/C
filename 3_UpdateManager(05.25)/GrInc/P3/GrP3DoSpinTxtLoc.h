/*
	Gause Platform spin text local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinTxtLoc
#define	_EPC_GrP3DoSpinTxtLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoSpinTxtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinTxtLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;
}	*Ptr_GrP3DoSpinTxtLocPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinTxtLoc	:	public	Cls_GrP3DoSpinTxtBase
{
	protected:
		St_GrP3DoSpinTxtLocPara	m_ParaSpinTxtLoc;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );

	public:
		Cls_GrP3DoSpinTxtLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinTxtLoc( void );

		void	EvtHostIdSet( __u16 A_ObjId );
		__u16	EvtHostIdGet( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

