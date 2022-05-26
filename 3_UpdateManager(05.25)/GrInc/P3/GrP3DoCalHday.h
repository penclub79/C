/*
	GAUSE Platform calendar holiday

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoCalHday
#define	_EPC_GrP3DoCalHday

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoCalBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoCalHdayPara
{
	__u8	SchdCnt;				// holiday schedule count
}	*Ptr_GrP3DoCalHdayPara;

//====================================================================
//global class
class	Cls_GrP3DoCalHday	:	public	Cls_GrP3DoCalBase
{
	protected:
		St_GrP3DoCalHdayPara	m_ParaCalHday;

		__u8	m_DayTbl[32];

		__u8*	m_PtrTbl;
		__u16*	m_PtrYear;

		// local
		void	LcValueUpdt( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	LcCalRebuild( void );		// rebuild calendar - ex : year, month change
		virtual	__u8	LcMarkValueGet( __u8 A_Day );
		virtual	void	LcProcTouchDay( void );	// call when press enter key or left mouse button click
		virtual	void	LcNotifyMarkChg( void );	// notify value change

	public:
		Cls_GrP3DoCalHday( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoCalHday( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

