/*
	Gause Platform spin number local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoSpinNumLoc
#define	_EPC_GrP3DoSpinNumLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoSpinNumBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoSpinNumLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;

}	*Ptr_GrP3DoSpinNumLocPara;

//====================================================================
//global class
class	Cls_GrP3DoSpinNumLoc	:	public	Cls_GrP3DoSpinNumBase
{
	protected:
		St_GrP3DoSpinNumLocPara	m_ParaSpinNumLoc;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcNotifyValChg( void );
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP3DoSpinNumLoc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoSpinNumLoc( void );

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

