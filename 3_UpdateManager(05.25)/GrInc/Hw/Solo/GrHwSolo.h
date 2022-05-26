/*
	solo manager
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwSolo
#define	_EPC_GrHwSolo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<Hw/Solo/RegSolo.h>
#include	<Hw/Solo/GrDrvSolo.h>
#include	<Hw/Solo/GrDevSolo.h>

//====================================================================
//constance
#define E_GrHwSoloMaxDevCnt			2			//device count
#define E_GrHwSoloTimerPeriod		1000	

//====================================================================
//global macro


//====================================================================
//global type


#ifndef LINUX_APP


#endif	//ifndef LINUX_APP

//====================================================================
//class

class Cls_GrHwSolo	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;		//task server
		__s32		m_HndlDrv;			//driver handle
		
		St_GrDrvSoloSysInfo	m_SysInfo;		//solo sys info
		void*	m_PtrMmap;			//mmap pointer

		Cls_GrDevSolo*	m_DevTbl[E_GrHwSoloMaxDevCnt];

	protected:

		//inherited

	public:
		Cls_GrHwSolo( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrHwSolo();

		//info
		__u32		DeviceCount( void );
		__u32		DrvBufSizeGet( void );		//driver memory size get

		//interface
		Cls_GrDevSolo*	DeviceGet( __u8 A_Idx );		//device object get

		//device config


};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
