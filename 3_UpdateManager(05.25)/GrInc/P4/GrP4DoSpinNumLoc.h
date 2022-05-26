/*
	Gause Platform spin number local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinNumLoc
#define	_EPC_GrP4DoSpinNumLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoSpinNumBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinNumLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;

}	*Ptr_GrP4DoSpinNumLocPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinNumLoc	:	public	Cls_GrP4DoSpinNumBase
{
	protected:
		St_GrP4DoSpinNumLocPara	m_ParaSpinNumLoc;

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	LcNotifyValChg( void );
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

	public:
		Cls_GrP4DoSpinNumLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinNumLoc( void );

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

