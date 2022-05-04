/*
	solo record play
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPlaySoloV1
#define	_EPC_GrPlaySoloV1

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Dvr/GrDvrBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<Hw/Solo/GrDevSolo.h>
#include	<GrBufTool.h>

#include	<Dvr/Fs/V1/GrDvrFs1Mng.h>
#include	<Dvr/Fs/V1/GrDvrFs1Play.h>

//====================================================================
//constance
#define E_GrPlaySoloV1RecentFrmCnt		500

#define E_GrPlaySoloV1PlaySpeedMax		4			//max 16 x

//command
#define E_GrPlaySoloV1CmdSrcDevSet		0x00
#define E_GrPlaySoloV1CmdPlay					0x01
#define E_GrPlaySoloV1CmdDirSet				0x02
#define E_GrPlaySoloV1CmdSpeedSet			0x03
#define E_GrPlaySoloV1CmdStepNext			0x04		/* play next frame : St_GrPlaySoloV1MsgBase */
#define E_GrPlaySoloV1CmdStepPrev			0x05		/* play previous frame : St_GrPlaySoloV1MsgBase */
#define E_GrPlaySoloV1CmdStop					0x06		/* play stop : St_GrPlaySoloV1MsgBase */
#define E_GrPlaySoloV1CmdAdoChSet			0x07		/* audio channel set : St_GrPlaySoloV1MsgAdoChSet */
#define E_GrPlaySoloV1CmdGoFirst			0x08		/* go first : St_GrPlaySoloV1MsgBase */
#define E_GrPlaySoloV1CmdGoRecent			0x09		/* go recent : St_GrPlaySoloV1MsgBase */
#define E_GrPlaySoloV1CmdGoTime				0x0A		/* go by time : St_GrPlaySoloV1MsgGoTime */
#define E_GrPlaySoloV1CmdGoHndl				0x0B		/* go by handle : St_GrPlaySoloV1MsgGoByHndl */

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrPlaySoloV1MsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32		PlayCmd;
}	*Ptr_GrPlaySoloV1MsgBase;

typedef	struct St_GrPlaySoloV1MsgRtBool
{
	St_GrPlaySoloV1MsgBase	Hd;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV1MsgRtBool;

typedef	struct St_GrPlaySoloV1MsgSrcSet 
{
	St_GrPlaySoloV1MsgBase	Hd;
	__u8	Src;
}	*Ptr_GrPlaySoloV1MsgSrcSet;

typedef	struct St_GrPlaySoloV1MsgGoTime
{
	St_GrPlaySoloV1MsgBase	Hd;
	Def_GrTime	Time;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV1MsgGoTime;

typedef	struct St_GrPlaySoloV1MsgDirSet
{
	St_GrPlaySoloV1MsgBase	Hd;
	BOOL8	IsBack;
}	*Ptr_GrPlaySoloV1MsgDirSet;

typedef	struct St_GrPlaySoloV1MsgSpdSet
{
	St_GrPlaySoloV1MsgBase	Hd;
	__u32	Speed;
}	*Ptr_GrPlaySoloV1MsgSpdSet;

typedef	struct St_GrPlaySoloV1MsgGoByHndl
{
	St_GrPlaySoloV1MsgBase	Hd;
	__u64	Hndl;
	BOOL8*	PtrRtVal;
}	*Ptr_GrPlaySoloV1MsgGoByHndl;

typedef	struct St_GrPlaySoloV1MsgAdoChSet
{
	St_GrPlaySoloV1MsgBase	Hd;
	__u8	Ch;
}	*Ptr_GrPlaySoloV1MsgAdoChSet;

typedef	struct St_GrPlaySoloV1MsgRsvdWork
{
	St_GrPlaySoloV1MsgBase	Hd;
	__u8	RwkCmd;			//reserved work command
	__u64	Hndl;
	Def_GrTime	Time;
}	*Ptr_GrPlaySoloV1MsgRsvdWork;


//====================================================================
//class

class Cls_GrPlaySoloV1	:	public	Cls_GrDvrFs1Play
{
	private:
		__u32	m_MsgAloc;
		__u32	m_HndlTimer;

		Cls_GrDevSolo*	m_ObjDev;

		__u8	m_MaxChCnt;			// max channel count
		BOOL8	m_IsVdoPal;			// video signal PAL

		St_GrDvrFs1SeekCtrl		m_SeekCtrl;

		// play status
		__u32	m_DecVdoSyncCnt;
		__u32	m_DecAdoSyncCnt;


	protected:
		//local

		void	LcProcSourceSet( __u8 A_Source );
		void	LcProcGoFirst( void );
		void	LcProcGoRecent( void );
		void	LcProcGoByHndl( __u64 A_Hndl );
		void	LcProcGoTime( Def_GrTime A_Time );
		void	LcProcPlay( void );
		void	LcProcDirSet( BOOL8 A_IsBack );
		void	LcProcSpeedSet( __u32 A_Speed );
		void	LcProcStop( void );
		void	LcProcFrmNext( void );
		void	LcProcFrmPrev( void );
		void	LcMprcAdoChSet( __u8 A_Ch );

		void	LcChkPlayStatChg( void );

		void	LcDoDispErase( void );

#ifdef LINUX_EMU
		void	LcEmuChkAdoQue( void );
#endif

		//inherited
		virtual	__u8	DecVdo( Ptr_GrDvrAvHd A_PtrHd, BOOL8 A_IsDisp );
		virtual	__u8	DecAdo( Ptr_GrDvrAvHd A_PtrHd );
		virtual	BOOL8	PrepareLoadFw( void );	// prepare load forward
		virtual	void	AfterLoadFw( void );		// after work load forward
		virtual	BOOL8	PrepareLoadBw( void );	// prepare load backward
		virtual	void	AfterLoadBw( void );		// after work load backward

		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
		Cls_GrPlaySoloV1( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, Cls_GrDvrFs1Mng* A_FsMng, __u8 A_Gop );
		virtual	~Cls_GrPlaySoloV1();

		//control
		void	SourceSet( __u8 A_Source );
		void	GoFirst( void );
		void	GoRecent( void );
		void	GoTime( Def_GrTime A_Time );
		void	GoByHndl( __u64 A_Hnd );

		void	Play( void );
		void	DirectionSet( BOOL8 A_IsBack );
		void	Stop( void );
		void	SpeedSet( __u32 A_Speed );
		void	AudioChSet( __u8 A_Ch );

		void	StepNext( void );
		void	StepPrev( void );

};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
