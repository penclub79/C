/*
	Gause Platform spin text local

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoSpinTxtLoc
#define	_EPC_GrP4DoSpinTxtLoc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoSpinTxtBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoSpinTxtLocPara
{
	__u16	EvtHostId;
	__u16	_rsvd0;
}	*Ptr_GrP4DoSpinTxtLocPara;

//====================================================================
//global class
class	Cls_GrP4DoSpinTxtLoc	:	public	Cls_GrP4DoSpinTxtBase
{
	protected:
		St_GrP4DoSpinTxtLocPara	m_ParaSpinTxtLoc;

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
		Cls_GrP4DoSpinTxtLoc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoSpinTxtLoc( void );

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

