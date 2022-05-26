/*
	GAUSE Platform calendar holiday

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoCalHday
#define	_EPC_GrP4DoCalHday

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoCalBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoCalHdayPara
{
	__u8	SchdCnt;				// holiday schedule count
}	*Ptr_GrP4DoCalHdayPara;

//====================================================================
//global class
class	Cls_GrP4DoCalHday	:	public	Cls_GrP4DoCalBase
{
	protected:
		St_GrP4DoCalHdayPara	m_ParaCalHday;

		__u8	m_DayTbl[32];

		__u8*	m_PtrTbl;
		__u16*	m_PtrYear;

		// local
		void	LcValueUpdt( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	LcCalRebuild( void );		// rebuild calendar - ex : year, month change
		virtual	__u8	LcMarkValueGet( __u8 A_Day );
		virtual	void	LcProcTouchDay( void );	// call when press enter key or left mouse button click
		virtual	void	LcNotifyMarkChg( void );	// notify value change

	public:
		Cls_GrP4DoCalHday(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoCalHday( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

