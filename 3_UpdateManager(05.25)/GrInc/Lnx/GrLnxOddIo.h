/*
linux Odd io

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrLnxOddIo
#define	_EPC_GrLnxOddIo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Lnx/GrLnxScsi.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>

#ifdef LINUX_APP
	#include	<sys/ioctl.h>
	#include	<linux/cdrom.h>
	#include	<mntent.h>
	#include	<sys/wait.h>
	#include	<sys/utsname.h>
	#include	<scsi/sg.h>
#else
	#include	<LnxW32Inc/mntent.h>
	#include	<LnxW32Inc/errno.h>
	#include	<LnxW32Inc/linux/cdrom.h>
	#include	<LnxW32Inc/sys/ioctl.h>
	#include	<LnxW32Inc/scsi/sg.h>
#endif


//====================================================================
//constance
#define E_GrLnxOddIoEvtRecPre				0x01
#define E_GrLnxOddIoEvtRecErr				0x02		/* no parametter */


#ifndef FATAL_START
#define	FATAL_START(er)	(0x80|(er))
#endif
#define	SK(errcode)	(((errcode)>>16)&0xF)
#define	ASC(errcode)	(((errcode)>>8)&0xFF)
#define ASCQ(errcode)	((errcode)&0xFF)

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrLnxOddIoMgpRecEvt		/* record event */
{
	__u32		EvtCode;				//event code
	__u32		Idx;						//section index
	BOOL8*		PtrIsQuit;			//mark quit
	__s8*		StrFl;					//file name list
}	*Ptr_GrLnxOddIoMgpRecEvt;

//====================================================================
//class
class	Cls_GrLnxOddIo	:	public Cls_GrTaskCli
{
	private:
		__u32			m_MsgAloc;
		Cls_GrTaskSvr*		m_ObjTask;
		Cls_GrLnxScsi*		m_ObjScsi;

		//record
		BOOL8	m_IsRecording;		//recording
		BOOL8	m_IsRecDvd;				//dvd recording
		BOOL8	m_IsRecNeedFmt;		//need format
		Cls_GrTaskCli*		m_ObjRevt;			//record event
		__u32							m_RevtFcc;			//record event fcc code
		__u32							m_RecIdx;				//record index
		Def_StrLong				m_StrRecFl;			//record file list
		Def_StrLong				m_StrRecCmd;		//record command
	protected:
		__u32	LcHndlEvent( void );		/* result = event map */
		BOOL8	LcWaitForUnit( void );
		BOOL8	LcPioneerStop( void );
		BOOL8	LcPage5Setup( __u16 A_Profile = 0 );
		BOOL8	LcGetMediaInfo( __u16* A_PtrRtType, BOOL8* A_PtrRtIsFormated );
		BOOL8	LcFormat( void );

		BOOL8	LcSetupFds( __s8* A_StrInDev, __s8* A_StrRtDev );
		BOOL8	LcDvdRec( void );
		BOOL8	LcCdRec( void );

		//background process
		BOOL8	LcProcBkRec( void );

		//message
		void	LcProcMsgRec( Ptr_GrTaskMsg A_PtrMsg );

		//task 
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsg A_PtrMsg );
		virtual	BOOL8	RtlBkgProc( void );
		
	public:
		Cls_GrLnxOddIo( Cls_GrTaskSvr* A_ObjTask, __s8* A_StrDev );
		virtual	~Cls_GrLnxOddIo();
		
		//info
		BOOL8	IsUsable( void );		//usable odd
		BOOL8	GetMediaInfo( __u16* A_PtrRtType, BOOL8* A_PtrRtIsFormated );
		BOOL8	IsExistMedia( void );
		BOOL8	IsTrayOpen( void );

		//control
		BOOL8	Eject( BOOL8 A_IsTrayOpen = TRUE );

		//maintanence
		BOOL8	Format( void );

		//record
		BOOL8	Record( Cls_GrTaskCli* A_ObjEvt, __u32 A_EvtFcc, BOOL8 A_IsFmt = TRUE );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

