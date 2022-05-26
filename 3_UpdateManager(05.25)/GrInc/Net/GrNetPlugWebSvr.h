/*
	web server

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugWebSvr
#define	_EPC_GrNetPlugWebSvr

//====================================================================
// uses

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrTaskBase.h>
#include	<GrTaskSvr.h>
#include	<GrSockV2.h>
#include	<GrFileCtrl.h>

#include <Net/GrNetSvs.h>
#include <Net/GrNetPlugBase.h>

//====================================================================
//const

#define E_GrNetPlugWebSvrMaxConn					32
#define E_GrNetPlugWebSvrContBufSize			1024
#define E_GrNetPlugWebSvrPtcCnt						2
#define E_GrNetPlugWebSvrPtcBufSize				1024

#define	E_GrNetPlugWebSvrFnLimit					200


#define E_GrNetPlugWebSvrModeNone					0
#define E_GrNetPlugWebSvrModeRecv					1		/* data receive mode */
#define E_GrNetPlugWebSvrModePrepSend			2		/* prepare send */
#define E_GrNetPlugWebSvrModeSendHead			3		/* prepare send */
#define E_GrNetPlugWebSvrModePrepPtc			4		/* prepare PTC send */
#define E_GrNetPlugWebSvrModeSendPtc			5		/* PTC send */
#define E_GrNetPlugWebSvrModePrepFile			6		/* prepare file send */
#define E_GrNetPlugWebSvrModeSendFile			7		/* file send */
#define E_GrNetPlugWebSvrModeFinish				8	/* finish */

#define E_GrNetPlugWebSvrDfltPort					80

#define E_GrNetPlugWebSvrExclId						Mac_GrMakeFcc('w','e','b','s')

#define E_GrNetPlugWebSvrExclCmdPath			1		/* path change - [1] path pointer */
#define E_GrNetPlugWebSvrExclCmdDfltFn		2		/* default file name - [1] file name */
#define E_GrNetPlugWebSvrExclCmdRegPtc		3		/* register patch - [1] patch index, [2] Ptr_GrNetPlugWebSvrExclPtc */
#define E_GrNetPlugWebSvrExclCmdPort			4		/* port change - [1] port */


//====================================================================
//type

typedef	struct St_GrNetPlugWebSvrConn 
{
	Def_GrSock	Sock;		//socket

	void*	PtrBuf;
	__u32	BufSize;			// receive size , send size
	__u32	BufPos;
	char	StrFn[E_GrNetPlugWebSvrContBufSize];			//file name
	__u8	BufCont[E_GrNetPlugWebSvrContBufSize];		//contents buffer

	__u32	Prgs;				// progress size
	__u32	ParsPos;

	// file mode
	Cls_GrFileCtrl*	Fc;
	__u32	FlSize;
	__u8	PtcIdx;
	BOOL8	IsPtc;			// use patch
	__u8	Mode;
	__u8	FndCnt;			// found count
}	*Ptr_GrNetPlugWebSvrConn;

typedef struct St_GrNetPlugWebSvrPtc
{
	BOOL8	IsExist;
	__u32	Size;
	Def_StrLong	StrFn;
	__u8	Buf[E_GrNetPlugWebSvrPtcBufSize];
}	*Ptr_GrNetPlugWebSvrPtc;

typedef struct St_GrNetPlugWebSvrExclPtc
{
	__u32	Size;
	char*	StrFn;
	void*	PtrBuf;
}	*Ptr_GrNetPlugWebSvrExclPtc;

//====================================================================
//class
class Cls_GrNetPlugWebSvr	:	public	Cls_GrNetPlugBase
{
	private:

		Def_StrGrFnPathShort	m_StrPath;
		Def_StrTag	m_StrIdxFn;

		__u16				m_Port;
		Def_GrSock	m_SockListen;

		__u32				m_ConnCnt;

		St_GrNetPlugWebSvrConn	m_ConnTbl[E_GrNetPlugWebSvrMaxConn];

		St_GrNetPlugWebSvrPtc		m_PtcTbl[E_GrNetPlugWebSvrPtcCnt];
	protected:
		//local function
		void	LcSetupScriptPath( char* A_StrPath );								// path setup
		void	LcSetupPort( __u16 A_Port );
		void	LcSetupRegPtc( __u32 A_Idx, Ptr_GrNetPlugWebSvrExclPtc A_PtrPtc );
		void	LcSetupDfltFn( char* A_StrFn );

		void	LcListenSockUpdt( void );
		BOOL8	LcAccept( void );

		BOOL8	LcConnChkCmdRecved( __u8 A_ConnIdx );
		BOOL8	LcConnParsCmd( __u8 A_ConnIdx );
		BOOL8	LcConnGetFileName( __u8 A_ConnIdx, __u32 A_Pos );

		BOOL8	LcConnModeRecv( __u8 A_ConnIdx );
		BOOL8	LcConnModePrepSend( __u8 A_ConnIdx );
		BOOL8	LcConnModeSendHead( __u8 A_ConnIdx );
		BOOL8	LcConnModePrepPtc( __u8 A_ConnIdx );
		BOOL8	LcConnModeSendPtc( __u8 A_ConnIdx );
		BOOL8	LcConnModePrepFile( __u8 A_ConnIdx );
		BOOL8	LcConnModeSendFile( __u8 A_ConnIdx );
		BOOL8	LcConnModeFinish( __u8 A_ConnIdx );

		// event
		virtual	void	ProcCmdDevOff( void );
		virtual	void	ProcCmdDevOn( void );

	public:
		Cls_GrNetPlugWebSvr(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual	~Cls_GrNetPlugWebSvr( void );

		virtual	void	RtlTimer( void );
		virtual	BOOL8	RtlBkgProc( void );
		virtual	BOOL8	RtlExclusive( __u32 A_Id, __u32 A_Cmd, __u32 A_Para1, __u32 A_Para2 );


};


//====================================================================
//function


//====================================================================
//out Environment conditional
#endif
