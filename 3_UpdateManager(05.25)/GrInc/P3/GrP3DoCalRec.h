/*
	GAUSE Platform calendar record

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoCalRec
#define	_EPC_GrP3DoCalRec

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
typedef	struct	St_GrP3DoCalRecPara
{
	__u16	GvuDev;				// record device GVU
	__u16	RelCtrlId;		// relate control id
	BOOL8	IsRelFcsMove;	// when select day, move focus to relate control
	__u8	_rsvd0;
	__u16	GvuTime;			// time GVU
}	*Ptr_GrP3DoCalRecPara;

//====================================================================
//global class
class	Cls_GrP3DoCalRec	:	public	Cls_GrP3DoCalBase
{
	protected:
		St_GrP3DoCalRecPara	m_ParaCalRec;

		__s32*		m_PtrGvu;

		__u8	m_RecType;		// record type

		__u8	m_SelDay;
		__u32	m_DayMap;			// day map

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
		virtual	void	LcViewTimeChg( void );	// view time change

	public:
		Cls_GrP3DoCalRec( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoCalRec( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

