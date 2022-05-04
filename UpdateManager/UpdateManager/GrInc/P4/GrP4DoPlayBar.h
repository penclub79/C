/*
	GAUSE Platform grid play time bar
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoPlayBar
#define	_EPC_GrP4DoPlayBar

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P4/GrP4Base.h>
#include <P4/GrP4DoBase.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance
#define E_GrP4DoPlayBarEvtPntCnt			16

//record map grid index
#define E_GrP4DoPlayBarHourMapType		0 //24 hour map index
#define E_GrP4DoPlayBarMinMapType			1 //60 minute map index

//text index count
#define E_GrP4DoPlayBarStrChCnt				4
#define E_GrP4DoPlayBarStrTimeCnt			24

#define E_GrP4DoPlayBarStrTypeNone		0
#define E_GrP4DoPlayBarStrTypeCh			1
#define E_GrP4DoPlayBarStrTypeTime		2

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoPlayBarPara
{
	__u16 PntBgTime;
	__u16	PntBgCh;
	__u16 PntBgGrid;

	__u16 PntRecEvt[E_GrP4DoPlayBarEvtPntCnt];
	__u16 PntLine;
	__u16 PntCus;
	__u16 PntStr;

	__u16 TimeHeight;
	__u16 ChWidth;

	__u8	ListChCnt;

	__u16	GvuDev;
	__u16	GvuTime;

	__u16	AsmTimeSel;
}	*Ptr_GrP4DoPlayBarPara;

typedef	struct	St_GrP4DoPlayBarMap
{
	__u8	MapType;	//record map Type
	__s8	ChIdx;		//record channel list start channel index
	St_GrRect GridRect;
	__u16 GridXCnt;
	__u16 GridYCnt;
	__u16 GridXSize;
	__u16 GridYSize;

	__u16 CusSizeX;
	__u16 CusSizeY;

	__u32	MapTime;
	__u8*	PtrMap;		//ch min map
} *Ptr_GrP4DoPlayBarRecMap;

//====================================================================
//global class
class	Cls_GrP4DoPlayBar	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoPlayBarPara	m_ParaPlayBar;

		__s32*	m_PtrGvu;		// GVU table pointer

		__u32	m_PlayTime;

		__u16	m_Year;
		__u8	m_Month;
		__u8	m_Day;
		__u8	m_Hour;
		__u8	m_Min;

		__u16 m_CusX;
		__u16	m_CusY;

		__u8	m_MaxChCnt;
		BOOL8 m_IsReqRecMap;
		__u8	m_StrChIdx;
		__u8	m_StrTimeIdx;

		__u8	m_StrType;
		Def_WstrNum m_StrCh[E_GrP4DoPlayBarStrChCnt];
		Def_WstrNum m_StrTime[E_GrP4DoPlayBarStrTimeCnt];

		St_GrP4DoPlayBarMap		m_RecMap;

		BOOL8	LcTimeUpdate(__u32 A_Time);
		void	LcGetMinMap(__u16 A_ItmCnt, __u32 A_Time);
		void	LcGetHourMap(__u16 A_ItmCnt, __u32 A_Time);

		BOOL8 LcGridPosToCusPos(__u16 A_X, __u16 A_Y, __u16* A_PtrRtX, __u16* A_PtrRtY);

		void	LcPaintItemGrid();
		void	LcPaintItemGridHour();
		void	LcPaintItemGridMin();
		void	LcPaintItemTime();
		void	LcPaintItemCh();
		void	LcPaintItemGridCursor();

		void	LcProcPlayModeSet(__u32 A_Mode);
		void	LcProcPlayTimeSet(__u32 A_Time);
		void	LcProcChListSet(__u32 A_ListMode);

		virtual	void	LcProcKeyItemSel(__u32 A_SelIdx);

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual void ProcPaint(void);
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

	public:
		Cls_GrP4DoPlayBar(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoPlayBar( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize
		virtual	WCHAR*	RtlInTextGet(__u8 A_Idx, __s32* A_PtrRtCus);
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

