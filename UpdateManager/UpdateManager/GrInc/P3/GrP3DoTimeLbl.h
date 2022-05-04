/*
	Gause Platform time label

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoTimeLbl
#define	_EPC_GrP3DoTimeLbl

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoTimeLblIdxUyear			0
#define E_GrP3DoTimeLblIdxUmon			1
#define E_GrP3DoTimeLblIdxUday			2
#define E_GrP3DoTimeLblIdxUhour			3
#define E_GrP3DoTimeLblIdxUmin			4
#define E_GrP3DoTimeLblIdxUsec			5
#define E_GrP3DoTimeLblIdxYear			6
#define E_GrP3DoTimeLblIdxMon				7
#define E_GrP3DoTimeLblIdxDay				8
#define E_GrP3DoTimeLblIdxHour			9
#define E_GrP3DoTimeLblIdxMin				10
#define E_GrP3DoTimeLblIdxSec				11
#define E_GrP3DoTimeLblIdxSm				12
#define E_GrP3DoTimeLblIdxApm				13

#define E_GrP3DoTimeLblItemCnt			14

#define E_GrP3DoTimeLblUnitCnt			6
#define E_GrP3DoTimeLblElemCnt			6

#define E_GrP3DoTimeLblTxtLen				8

#define E_GrP3DoTimeLblFixTime			0
#define E_GrP3DoTimeLblNowTime			1
#define E_GrP3DoTimeLblPlayTime			2


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoTimeLblPara
{
	__u16	PntItem[E_GrP3DoTimeLblItemCnt];
	__u16	ItemWidth[E_GrP3DoTimeLblItemCnt];

	__u16	TxtUnit[E_GrP3DoTimeLblUnitCnt];

	__u16	GapHead;
	__u16	GapMid;	

	__u16	PntBg;					// paint background
	__u8	TimeType;
	BOOL8	IsDateUnitFix;

}	*Ptr_GrP3DoTimeLblPara;

//====================================================================
//global class
class	Cls_GrP3DoTimeLbl	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoTimeLblPara	m_ParaTimeLbl;

		__u8	m_ChkTimer;

		BOOL8	m_IsSm;

		Def_GrTime	m_Time;
		__u16	m_ElemVal[E_GrP3DoTimeLblElemCnt];
		WCHAR	m_ElemTxt[E_GrP3DoTimeLblElemCnt][E_GrP3DoTimeLblTxtLen];

		__u16	m_ItemPosTbl[E_GrP3DoTimeLblItemCnt];

		// paint
		__u8	m_PntTimeMode;

		// local
		BOOL8	LcTimeUpdt( void );
		void	LcItemPosBuild( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		virtual	void	ProcEvtSysStat( __u8 A_Type );

	public:
		Cls_GrP3DoTimeLbl( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoTimeLbl( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

