/*
DB client
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrDbCli
#define	_GrDbCli

//====================================================================
// uses
#include <afxwin.h>

#include <GrTypeBase.h>
#include <GrTaskBase.h>
#include <GrTaskCli.h>
#include <GrTaskSvr.h>

#include <GrError.h>
#include <GrStrTool.h>
#include <GrFileCtrl.h>

#include <Win/Db/GrDbBase.h>
#include <Win/Db/GrDbGrp.h>
#include <Win/Db/GrDbSync.h>


//====================================================================
//constance

#define	E_GrDbCliTimerPeriod							1000		/* 1 sec */


#define	E_GrDbCliUpdtTimeGap							30

#define	E_GrDbCliFlushPeriod							10

#define	E_GrDbCliConnTimeOut							100

#define	E_GrDbCliBufSize									0x20000


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class Cls_GrDbCli
{
	protected:

		__u32							m_SvIp;
		__u16							m_SvPort;

		Def_GrSock				m_Sock;

		__u8							m_Buf[E_GrDbCliBufSize];

		// local function
		__u8	LcConnect(void);
		__u8	LcSend(void* A_PtrBuf, __u32 A_Size);
		__u8	LcRecv(void);
		void	LcClose(void);

	public:
		Cls_GrDbCli(void);
		virtual	~Cls_GrDbCli(void);
	
		// setup
		void	Setup(__u32 A_SvIp, __u16 A_Port);

		__u8	KeyCreate(__u32 A_Grpid, void* A_PtrKey, __u32 A_Ofs = 0, void* A_PtrData = NULL, __u32 A_Size = 0);
		__u8	KeyDelete(__u32 A_GrpId, void* A_PtrKey);

		__u8	DataWrite(__u32 A_GrpId, void* A_PtrKey, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);
		__u8	DataRead(__u32 A_GrpId, void* A_PtrKey, __u32 A_Ofs, void* A_PtrData, __u32 A_Size);

};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


