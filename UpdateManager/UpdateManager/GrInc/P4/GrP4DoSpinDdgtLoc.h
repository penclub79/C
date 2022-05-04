/*
	Gause Platform spin decimal digit local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinDdgtLoc
#define	_EPC_GrP4DoSpinDdgtLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoSpinDdgtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinDdgtLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;
}	*Ptr_GrP4DoSpinDdgtLocPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinDdgtLoc	:	public	Cls_GrP4DoSpinDdgtBase
{
	protected:
		St_GrP4DoSpinDdgtLocPara	m_ParaSpinDdgtLoc;

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );
		virtual	void	LcNotifyValChg( void );

	public:
		Cls_GrP4DoSpinDdgtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinDdgtLoc( void );

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

