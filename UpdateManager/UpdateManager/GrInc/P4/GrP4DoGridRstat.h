/*
	Gause Platform grid record status

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoGridRstat
#define	_EPC_GrP4DoGridRstat

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoGridBase.h>
#include <Dvr/GrDvrBase.h>
#include <GrTimeTool.h>

//====================================================================
//constance

#define E_GrP4DoGridRstatValCnt				16

#define E_GrP4DoGridRstatViewBufSize	(60*16)


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoGridRstatPara
{
	__u8	ChCnt;			// channel count
	BOOL8	IsChMerge;	// channel merge
	BOOL8	IsHourUnit;	// hour unit
	BOOL8	IsFastReact;	// fast react relate control

	__u16	RelUpdtId;		// relate update action control id
	BOOL8	IsRelFcs;			// relate focus
	BOOL8	IsSelPlay;		// when select time do play

	__u16	PntVal[E_GrP4DoGridRstatValCnt];

	__u16	PntSel;
	__u16	AsmTimeSel;

	BOOL8	IsCusSel;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u8	_rsvd3;

	__u16	GvuDev;
	__u16	GvuTime;

}	*Ptr_GrP4DoGridRstatPara;

//====================================================================
//global class
class	Cls_GrP4DoGridRstat	:	public	Cls_GrP4DoGridBase
{
	protected:
		St_GrP4DoGridRstatPara	m_ParaGridRstat;

		BOOL8		m_IsCmdUpdt;

		__u8		m_RecType;		// record type

		__u8		m_SelIdx;
		Def_GrTime	m_ViewTime;
		Def_GrTime	m_SelTime;

		__u8	m_MinMap[E_GrTimeMinOfDay][E_GrDvrMaxChCnt];
		__u8	m_ViewData[E_GrP4DoGridRstatViewBufSize];

		__s32*	m_PtrGvu;

		// local
		void	LcXyCntUpdt( void );
		void	LcProcSelTime( void );
		void	LcViewDataBuild( void );
		void	LcValueUpdt( void );

		virtual	void	LcProcKeyItemSel( void );						// key item select
		virtual	void	LcPaintSelect( void );							// paint select area
		virtual	void	LcPainItems( void );								// paint item area

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

	public:
		Cls_GrP4DoGridRstat(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoGridRstat( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

