/*
	Gause Platform relate static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoRelStc
#define	_EPC_GrP4DoRelStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoRelStcStatNormal		0
#define E_GrP4DoRelStcStatActive		1

#define E_GrP4DoRelStcStatCnt				2


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoRelStcPara
{
	__u16	PntStat[E_GrP4DoRelStcStatCnt];				// paint background
	__u16	TxtId;					// text id
	__u16	_rsvd;					// relate target id
}	*Ptr_GrP4DoRelStcPara;

//====================================================================
//global class
class	Cls_GrP4DoRelStc	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoRelStcPara	m_ParaDoRelStc;

		__u8		m_Stat;

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

		// event

	public:
		Cls_GrP4DoRelStc(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoRelStc( void );

		void	ActiveSet( BOOL8 A_IsActive );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

