/*
	Gause Platform time label

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoTimeLbl
#define	_EPC_GrP4DoTimeLbl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoTimeLblIdxUyear			0
#define E_GrP4DoTimeLblIdxUmon			1
#define E_GrP4DoTimeLblIdxUday			2
#define E_GrP4DoTimeLblIdxUhour			3
#define E_GrP4DoTimeLblIdxUmin			4
#define E_GrP4DoTimeLblIdxUsec			5
#define E_GrP4DoTimeLblIdxYear			6
#define E_GrP4DoTimeLblIdxMon				7
#define E_GrP4DoTimeLblIdxDay				8
#define E_GrP4DoTimeLblIdxHour			9
#define E_GrP4DoTimeLblIdxMin				10
#define E_GrP4DoTimeLblIdxSec				11
#define E_GrP4DoTimeLblIdxSm				12
#define E_GrP4DoTimeLblIdxApm				13

#define E_GrP4DoTimeLblItemCnt			14

#define E_GrP4DoTimeLblUnitCnt			6
#define E_GrP4DoTimeLblElemCnt			6

#define E_GrP4DoTimeLblTxtLen				8

#define E_GrP4DoTimeLblFixTime			0
#define E_GrP4DoTimeLblNowTime			1
#define E_GrP4DoTimeLblPlayTime			2


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoTimeLblPara
{
	__u16	PntItem[E_GrP4DoTimeLblItemCnt];
	__u16	ItemWidth[E_GrP4DoTimeLblItemCnt];

	__u16	TxtUnit[E_GrP4DoTimeLblUnitCnt];

	__u16	GapHead;
	__u16	GapMid;	

	__u16	PntBg;					// paint background
	__u8	TimeType;
	BOOL8	IsDateUnitFix;

}	*Ptr_GrP4DoTimeLblPara;

//====================================================================
//global class
class	Cls_GrP4DoTimeLbl	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoTimeLblPara	m_ParaTimeLbl;

		__u8	m_ChkTimer;

		BOOL8	m_IsSm;

		Def_GrTime	m_Time;
		__u16	m_ElemVal[E_GrP4DoTimeLblElemCnt];
		WCHAR	m_ElemTxt[E_GrP4DoTimeLblElemCnt][E_GrP4DoTimeLblTxtLen];

		__u16	m_ItemPosTbl[E_GrP4DoTimeLblItemCnt];

		// paint
		__u8	m_PntTimeMode;

		// local
		BOOL8	LcTimeUpdt( void );
		void	LcItemPosBuild( void );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		virtual	void	ProcEvtSysStat( __u8 A_Type );

	public:
		Cls_GrP4DoTimeLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt );
		virtual	~Cls_GrP4DoTimeLbl( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

