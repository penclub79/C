/*
	Gause Platform relate static

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoRelStc
#define	_EPC_GrP3DoRelStc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoRelStcStatNormal		0
#define E_GrP3DoRelStcStatActive		1

#define E_GrP3DoRelStcStatCnt				2


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoRelStcPara
{
	__u16	PntStat[E_GrP3DoRelStcStatCnt];				// paint background
	__u16	TxtId;					// text id
	__u16	_rsvd;					// relate target id
}	*Ptr_GrP3DoRelStcPara;

//====================================================================
//global class
class	Cls_GrP3DoRelStc	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoRelStcPara	m_ParaDoRelStc;

		__u8		m_Stat;

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

		// event

	public:
		Cls_GrP3DoRelStc( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoRelStc( void );

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

