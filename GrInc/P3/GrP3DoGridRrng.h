/*
	GAUSE Platform grid record range select

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoGridRrng
#define	_EPC_GrP3DoGridRrng

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3DoGridBase.h>
#include <Dvr/GrDvrBase.h>
#include <GrTimeTool.h>

//====================================================================
//constance

#define E_GrP3DoGridRrngValCnt				16

#define E_GrP3DoGridRrngViewBufSize	(60*16)


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoGridRrngPara
{
	__u8	ChCnt;				// channel count
	BOOL8	IsChMerge;		// channel merge
	BOOL8	IsHourUnit;		// hour unit
	BOOL8	IsFastReact;	// fast react relate control

	__u16	PntVal[E_GrP3DoGridRrngValCnt];

	__u16	PntSel;
	__u16	RelCtrlId;		// relate control id

	__u16	AsmSelSt;
	__u16	AsmSelEd;

	__u16	GvuDev;
	__u16	GvuTimeVw;

	__u16	GvuTimeSt;
	__u16	GvuTimeEd;

}	*Ptr_GrP3DoGridRrngPara;

//====================================================================
//global class
class	Cls_GrP3DoGridRrng	:	public	Cls_GrP3DoGridBase
{
	protected:
		St_GrP3DoGridRrngPara	m_ParaGridRrng;

		BOOL8		m_IsCmdUpdt;

		BOOL8		m_IsUpdtPntSel;
		__u8		m_SelIdx;
		__u8		m_RecType;		// record type
		Def_GrTime	m_SelTime[2];
		
		Def_GrTime	m_ViewTime;

		__u8	m_MinMap[E_GrTimeMinOfDay][E_GrDvrMaxChCnt];
		__u8	m_ViewData[E_GrP3DoGridRrngViewBufSize];

		__s32*	m_PtrGvu;

		__s32		m_PntSelIdx[2];

		// local
		void	LcXyCntUpdt( void );
		void	LcProcSelTime( void );
		void	LcViewDataBuild( void );
		void	LcValueUpdt( void );
		void	LcSelRngCrct( void );

		virtual	void	LcProcKeyItemSel( void );						// key item select
		virtual	void	LcPaintSelect( void );							// paint select area
		virtual	void	LcPainItems( void );								// paint item area

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

	public:
		Cls_GrP3DoGridRrng( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoGridRrng( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

