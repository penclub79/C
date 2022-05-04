/*
	win32 backup play
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrW32PlayM1
#define	_EPC_GrW32PlayM1

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Dvr/GrDvrPlay.h>
#include	<Dvr/GrDvrRiBase.h>
#include	<Mpeg4/GrXvidDec.h>
#include	<Dvr/GrDvrFls16v1.h>
#include	<MMSystem.h>
#include	<GrG723D24.h>
#include	<GrBufTool.h>

//====================================================================
//constance
#define E_GrW32PlayM1SpdMax				6
#define E_GrW32PlayM1SpdMin				-6
#define E_GrW32PlayM1SpdIonly			4

#define E_GrW32PlayM1MaxChCnt			16

#define E_GrW32PlayM1MaxFrmCnt		0x10000	/*	max frame count */
#define E_GrW32PlayM1WaitLim			1000		/*	max wait time		*/

#define E_GrW32PlayM1BufSkipLim		0x00600000
#define E_GrW32PlayM1BufChkStep		1000
#define E_GrW32PlayM1IonlyLim			5

#define E_GrW32PlayM1BufTime			250

#define E_GrW32PlayM1RsvRto				(16 / E_GrHwS6010VencGopDivRto)

#define E_GrW32PlayM1BufSize			0x200000

#define E_GrW32PlayM1MaxResX			720
#define E_GrW32PlayM1MaxResY			576

#define E_GrW32PlayM1VdoFrmSize		(E_GrW32PlayM1MaxResX * E_GrW32PlayM1MaxResY * 2) 

#define E_GrW32PlayM1BadCh				0xFFFFFFFF

#define E_GrW32PlayM1AdoBufCnt		4
#define E_GrW32PlayM1AdoSltCnt		2			/*	silent buffer count */
#define E_GrW32PlayM1AdoChDataSize	(48*16)
#define E_GrW32PlayM1AdoBufSize		(((48*16*8)/3) * 2)

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrW32PlayM1BufInfo 
{
	__u8			Buf[E_GrW32PlayM1BufSize];
	St_GrTimeRng	TimeRng;
	__u32		FrmCnt;						//exist
	Ptr_GrDvrFls16v1FrmHd			PtrFrms[E_GrW32PlayM1MaxFrmCnt];
}	*Ptr_GrW32PlayM1BufInfo;

typedef	struct St_GrW32PlayM1ChInfo 
{
	Cls_GrXvidDec*					ObjMp4;
	void*		PtrFrm;
	BOOL8		IsPlayOn;				//play on
	BOOL8		IsDecoded;			//decoded
	__u8		RecType;
	__u8		SkipCnt;				//skip frame count
	__u8		SkipPos;				//now skip position
	__u32		Fps;						//frame rate
	__u16		ResX;
	__u16		ResY;
}	*Ptr_GrW32PlayM1ChInfo;

typedef	struct St_GrW32PlayM1AdoItm 
{
	WAVEHDR*		PtrHdr;
	void*				PtrBuf;
	BOOL8				IsQued;
}	*Ptr_GrW32PlayM1AdoItm;

typedef	struct St_GrW32PlayM1AdoQue 
{
	Ptr_GrW32PlayM1AdoItm	PtrItm;
}	*Ptr_GrW32PlayM1AdoQue;

//====================================================================
//class
class Cls_GrW32PlayM1	:	public	Cls_GrDvrPlay
{
	private:
		//object
		Cls_GrDvrRiBase*				m_ObjRi;

		//gdi
		BITMAPINFOHEADER				m_VdecBi;			//bitmap info

		//buffer manager
		St_GrW32PlayM1BufInfo		m_BufMng;
		__u32			m_PlayFrmIdx;		//play frame index
		__u32			m_Mp4DecPitch;

		St_GrW32PlayM1ChInfo		m_ChMng[E_GrW32PlayM1MaxChCnt];

		//speed
		__u32			m_SpdShl;
		__u32			m_SpdShr;
		BOOL8			m_IsIfrmOnly;				//iframe play mode
		__u32			m_FpsCap;						//play speed capacity
		__u32			m_Fps;							//normal play speed

		Def_GrTime	m_TmReq;					//play request time
		Def_GrTime	m_LastUpdtTime;		//last update time
		Def_GrTime	m_LastChkTime;		//last buffer check time
		BOOL8			m_IsStepMode;				//step play mode

		//audio
		__u32			m_AdoCh;
		BOOL8			m_IsAdoAble;
		BOOL8			m_IsAdoExit;
		HWAVEOUT	m_HndlAdo;					//audio handle
		Cls_GrG723d24*	m_ObjAdec;

		St_GrW32PlayM1AdoItm	m_AdoBufTbl[E_GrW32PlayM1AdoBufCnt];
		St_GrW32PlayM1AdoItm	m_AdoSilent[E_GrW32PlayM1AdoSltCnt];
		St_GrBufQue						m_AdoQue;
		St_GrW32PlayM1AdoQue	m_AdoBufQue[E_GrW32PlayM1AdoBufCnt];

	protected:
		BOOL8		LoadChuk( Def_GrTime A_Time );
		BOOL8		LcGoFrmByTime( Def_GrTime A_Time );
		BOOL8		LcGoNextFrm( void );
		BOOL8		LcDecNowFrm( BOOL8* A_PtrIsDec, __u32* A_PtrRtVdoCh );
		BOOL8		LcGetNowFrmTime( Ptr_GrTime A_PtrRtTime );
		void		LcDecFlagsReset( void );
		void		LcUpdtPlaySpd( void );
		BOOL8		LcAdoDec( Ptr_GrDvrFls16v1AfrmHd A_PtrFrm );
		Ptr_GrW32PlayM1AdoItm	LcAdoGetBufInsAble( void );
	public:
		Cls_GrW32PlayM1( Cls_GrDvrRiBase* A_ObjRi );
		virtual	~Cls_GrW32PlayM1();

		//info
		virtual	__u8	GetChRecType(__u32 A_Ch);

		//gdi
		BITMAPINFO*		GetBitmapInfo( void );
		void		GetChRes( __u32 A_Ch, __u16* A_PtrRtResX, __u16* A_PtrRtResY );
		void*		GetChDib( __u32 A_Ch );

		//maintenance
		virtual	void	SetPlaySpeed( __s32 A_Speed );
		virtual	void	SetPlayDir( BOOL8 A_IsBack );

		virtual	BOOL8	ShowNextFrame( void );		//one frame step mode
		virtual	void	SetPlayPos( Def_GrTime A_Time );
		virtual	void	SetInstMode( BOOL8 A_IsInst );
		virtual	void	Play( BOOL8 A_IsRun );
		virtual	void	SetChPlayOn( __u32 A_Ch, BOOL8 A_IsOn );

		//custom
		void		SetAdoCh( __u32 A_Ch );

		//runtime
		__u32		RtlUpdt( Def_GrTime A_Time );
		void		RtlAdoCb( WAVEHDR* A_PtrWhdr );
};

//====================================================================
//global function
void CALLBACK GrW32PlayM1AdoCb( HWAVEOUT A_Hndl, UINT A_Type, DWORD_PTR A_Obj,  
													__u32 A_Parm1, __u32 A_Parm2 );


//====================================================================
//out Enviroment conditional
#endif

