/*
cpu aux gh driver
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrvCpuAux
#define	_EPC_GrDrvCpuAux

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrBufTool.h>

#ifdef	LINUX_APP
#ifdef	LINUX_DRV
#include	<linux/ioctl.h>
#else
#include	<sys/ioctl.h>
#endif
#endif

//====================================================================
//constance
#define E_GrDrvCaxStaticMaxCnt		4

//ioctl struct
#pragma pack(push)
#pragma pack(1)

typedef	struct St_GrDrvCaxSysInfo 
{
	__u32		MapSize;			//total mapping size
	__u32		StaticUnit;		//static bus unit size
	__u32		StaticCnt;		//static bus count
	__u32		StaicPhys[E_GrDrvCaxStaticMaxCnt];

}	*Ptr_GrDrvCaxSysInfo;

typedef	struct  St_GrDrvCaxReqGpioRead
{
	__u32	Map[2];			//read value
}	*Ptr_GrDrvCaxReqGpioRead;

typedef	struct  St_GrDrvCaxReqGpioWrite
{
	__u32	High[2];		//high set map
	__u32	Low[2];			//low set map
}	*Ptr_GrDrvCaxReqGpioWrite;

typedef	struct  St_GrDrvCaxReqBuz
{
	__u32	Cnt;				//request buzzer count
}	*Ptr_GrDrvCaxReqBuz;

#pragma pack(pop)

//ioctl command
#define	E_GrDrvCaxIoctlMagic				'j'

#define	E_GrDrvCaxIoctlSysInfo			_IOW( E_GrDrvCaxIoctlMagic, 0 , struct St_GrDrvCaxReqGpioWrite )
#define	E_GrDrvCaxIoctlGpioWrite		_IOW( E_GrDrvCaxIoctlMagic, 1 , struct St_GrDrvCaxReqGpioWrite )
#define	E_GrDrvCaxIoctlGpioRead			_IOR( E_GrDrvCaxIoctlMagic, 2 , struct St_GrDrvCaxReqGpioRead )
#define	E_GrDrvCaxIoctlBuzOut				_IOW( E_GrDrvCaxIoctlMagic, 3 , __u32 )

#define	E_GrDrvCaxIoctlMaxNr				3

//function enumeration
#define E_GrDrvCaxFncBuz						0
#define E_GrDrvCaxFncWatchdog				1

#define E_GrDrvCaxFncMaskBuz				(1 << E_GrDrvCaxFncBuz)
#define E_GrDrvCaxFncMaskWatchdog		(1 << E_GrDrvCaxFncWatchdog)

//====================================================================
//global macro


//====================================================================
//global type

#pragma pack(push)
#pragma pack(1)

typedef	struct St_GrDrvCaxCtrl
{
	//ir
	BOOL8	IsIrEnable;			//ir remocon enable
	struct	St_GrBufQue	QueIr;		//ir remocon queue
	__u32	IrMode;

}	*Ptr_GrDrvCaxCtrl;

#pragma pack(pop)

//====================================================================
//class


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif
