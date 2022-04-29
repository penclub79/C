/*
	GAUSE Platform edit base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoEdtBase
#define	_EPC_GrP3DoEdtBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance
#define E_GrP3DoEdtBaseStatDisable		0
#define E_GrP3DoEdtBaseStatNoraml			1
#define E_GrP3DoEdtBaseStatFocus			2

#define E_GrP3DoEdtBaseStatCnt				3

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoEdtBasePara
{
	__u16	PntStat[E_GrP3DoEdtBaseStatCnt];				// paint status
	__u8	MaxLen;					// value length
	BOOL8	IsResetOver;		// when overflow input text, do reset

	BOOL8	IsKeyVkbd;			// key virtual keyboard use
	BOOL8	IsPasswd;				// password type
	BOOL8	IsVkbdNum;			// number type virtual keyboard
	BOOL8	IsAdvPasswd;		// advanced password

	__u16	AsmVkbdEdtEnd;			// edit end funtion
	__u16	_rsvd1;
}	*Ptr_GrP3DoEdtBasePara;

//====================================================================
//global class
class	Cls_GrP3DoEdtBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoEdtBasePara	m_ParaEdtBase;

		// local
		virtual	WCHAR*	LcValAdrGet( void );		// value address get
		void	LcVkbdRun( void );
		virtual	void	ProcValChg( void );				// value change

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script
		virtual	void	ProcEvtVkbdEditEnd( void );

	public:
		Cls_GrP3DoEdtBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoEdtBase( void );

		// runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

