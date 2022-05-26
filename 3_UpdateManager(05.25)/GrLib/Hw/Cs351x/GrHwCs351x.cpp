/*
 cs351x misc

 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>

#include	<GrDebug.h>

#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>

#include	<Hw/Cs351x/GrHwCs351x.h>
#include	<stdlib.h>

#ifdef LINUX_APP

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>
#include	<time.h>
#include	<pthread.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>

#else

#include <EmuLnx/GrElaBase.h>

#endif

//====================================================================
//const

typedef	struct St_GrHwCs351xCirData {
	__u32 timeout;
	__u32 length;
	__u32	ret;
	__u32 data;
	__u32 data_ext;
}	*Ptr_GrHwCs351xCirData;

#define	E_GrHwCs351xCIR_IOC_BASE			('I'|'R')
#define E_GrHwCs351xGPIO_IOC_BASE			'Z'

#ifdef LINUX_APP

//cir
#define E_GrHwCs351xIocCIR_GET_DATA				_IOWR (E_GrHwCs351xCIR_IOC_BASE, 14, struct St_GrHwCs351xCirData)

//gpio
#define E_GrHwCs351xGPIO_KPD_GET					_IOR(E_GrHwCs351xGPIO_IOC_BASE,20, __u8)
#define E_GrHwCs351xGPIO_BUZ_SET					_IOW(E_GrHwCs351xGPIO_IOC_BASE,21, __u32)
#define E_GrHwCs351xGPIO_HW_VER_GET				_IOR(E_GrHwCs351xGPIO_IOC_BASE,22, __u32)
#define E_GrHwCs351xGPIO_LED_SET					_IOW(E_GrHwCs351xGPIO_IOC_BASE,23, __u32)
#define E_GrHwCs351xGPIO_SVC_INIT					_IO(E_GrHwCs351xGPIO_IOC_BASE,24)
#define E_GrHwCs351xGPIO_CACHE_FLUSH			_IO(E_GrHwCs351xGPIO_IOC_BASE,25)
#define E_GrHwCs351xGPIO_SYS_REBOOT				_IO(E_GrHwCs351xGPIO_IOC_BASE,26)

#else

//cir
#define E_GrHwCs351xIocCIR_GET_DATA		_LEMU_IOWR (E_GrHwCs351xCIR_IOC_BASE, 14, struct St_GrHwCs351xCirData)

//gpio
#define E_GrHwCs351xGPIO_KPD_GET					_LEMU_IOR(E_GrHwCs351xGPIO_IOC_BASE,20, __u8)
#define E_GrHwCs351xGPIO_BUZ_SET					_LEMU_IOW(E_GrHwCs351xGPIO_IOC_BASE,21, __u32)
#define E_GrHwCs351xGPIO_HW_VER_GET				_LEMU_IOR(E_GrHwCs351xGPIO_IOC_BASE,22, __u32)
#define E_GrHwCs351xGPIO_LED_SET					_LEMU_IOW(E_GrHwCs351xGPIO_IOC_BASE,23, __u32)
#define E_GrHwCs351xGPIO_SVC_INIT					_LEMU_IO(E_GrHwCs351xGPIO_IOC_BASE,24)
#define E_GrHwCs351xGPIO_CACHE_FLUSH			_LEMU_IO(E_GrHwCs351xGPIO_IOC_BASE,25)
#define E_GrHwCs351xGPIO_SYS_REBOOT				_LEMU_IO(E_GrHwCs351xGPIO_IOC_BASE,26)

#endif

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__s8	V_GrHwCs351xUsb0Dev[]	=	"/dev/sl_usb0";
__s8	V_GrHwCs351xUsb1Dev[]	=	"/dev/sl_usb1";

//====================================================================
Cls_GrHwCs351x::Cls_GrHwCs351x( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------

		//open cir
		m_HndlCir	=	open( "/dev/sl_cir", O_RDWR|O_NDELAY );
		m_CirSeq	=	0;

		m_LedMap		=	0;
		m_HndlGpio	=	open( "/dev/sl_gpio", O_RDWR|O_NDELAY );
		if ( 0 <= m_HndlGpio )
		{
			if ( 0 == ioctl( m_HndlGpio, E_GrHwCs351xGPIO_SVC_INIT ) )
			{
				if ( 0 != ioctl( m_HndlGpio, E_GrHwCs351xGPIO_HW_VER_GET, &m_HwVer ) )
				{
					close( m_HndlGpio );
					m_HndlGpio	=	-1;
				}
			}
			else
			{
				close( m_HndlGpio );
				m_HndlGpio	=	-1;
			}
		}

		//usb buf fix
		m_FnTblUsbDev[0]	=	V_GrHwCs351xUsb0Dev;
		m_FnTblUsbDev[1]	=	V_GrHwCs351xUsb1Dev;

		for ( Tv_WkIdx=0;Tv_WkIdx < 2;Tv_WkIdx++ )
		{
			m_HndlUsb[Tv_WkIdx]	=	-1;
		}
		
}
//--------------------------------------------------------------------
Cls_GrHwCs351x::~Cls_GrHwCs351x()
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//close cir
		if ( 0 <= m_HndlCir )
		{
			close( m_HndlCir );
		}

		if ( 0 <= m_HndlGpio )
		{
			close( m_HndlGpio );
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < 2;Tv_WkIdx++ )
		{
			if ( 0 <= m_HndlUsb[Tv_WkIdx] )
			{
				close( m_HndlUsb[Tv_WkIdx] );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrHwCs351x::CirNewCodeGet( void )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrHwCs351xCirData	Tv_Ioc;
	// code --------------------
		Tv_Result	=	0;

		if ( 0 <= m_HndlCir )
		{
			if ( 0 == ioctl( m_HndlCir, E_GrHwCs351xIocCIR_GET_DATA, &Tv_Ioc ) )
			{
				//check newer data
				if ( Tv_Ioc.ret != m_CirSeq )
				{
					Tv_Result	=	Tv_Ioc.data;
					m_CirSeq	=	Tv_Ioc.ret;			//update id
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrHwCs351x::LedSet( __u32 A_Idx, BOOL8 A_IsOn )
{
	// local -------------------
		__u32	Tv_Mask;
	// code --------------------
		//check led index
		if ( (E_GrHwCs351xMaxLedCnt > A_Idx) && (0 <= m_HndlGpio) )
		{
			//check led map - invert
			Tv_Mask	=	1 << A_Idx;
			if ( A_IsOn )
			{
				//check previous status is off
				if ( 0 != ( m_LedMap & Tv_Mask ) )
				{
					m_LedMap	=	m_LedMap & ~Tv_Mask;
					//do
					ioctl( m_HndlGpio, E_GrHwCs351xGPIO_LED_SET, &m_LedMap );
				}
			}
			else
			{
				//check previous status is on
				if ( 0 == ( m_LedMap & Tv_Mask ) )
				{
					m_LedMap	=	m_LedMap | Tv_Mask;
					//do
					ioctl( m_HndlGpio, E_GrHwCs351xGPIO_LED_SET, &m_LedMap );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrHwCs351x::BuzRun( __u32 A_Cnt )
{
		if ( (0 != A_Cnt) && (0 <= m_HndlGpio) )
		{
			//request buzzer
			ioctl( m_HndlGpio, E_GrHwCs351xGPIO_BUZ_SET, &A_Cnt );
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrHwCs351x::HwVersionGet( void )
{
		return	m_HwVer;
}
//--------------------------------------------------------------------
__u8	Cls_GrHwCs351x::KeypadGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( 0 <= m_HndlGpio )
		{
			//request buzzer
			ioctl( m_HndlGpio, E_GrHwCs351xGPIO_KPD_GET, &Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrHwCs351x::UsbBugFixing( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
		__s32		Tv_Rval;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < 2;Tv_WkIdx++ )
		{
			if ( 0 <= m_HndlUsb[Tv_WkIdx] )
			{
				//check freezing usb host
				Tv_Rval	=	ioctl(m_HndlUsb[Tv_WkIdx] , 5);
				if ( 1 == Tv_Rval )
				{
					//close handle
					close( m_HndlUsb[Tv_WkIdx] );
					//reset driver
					DbgMsgPrint( "Cls_GrHwCs351x::UsbBugFixing - USB host %d freezing found! fixing..\n", Tv_WkIdx );
					if ( 0 == Tv_WkIdx )
					{
						system("rmmod ehci-hcd-FOTG2XX");
						GrPrcSleep( 1000 );
						system("modprobe ehci-hcd-FOTG2XX");
					}
					else
					{
						system("rmmod ehci-hcd-FOTG2XX-1");
						GrPrcSleep( 1000 );
						system("modprobe ehci-hcd-FOTG2XX-1");
					}
					//reopen
					m_HndlUsb[Tv_WkIdx]	=	open( m_FnTblUsbDev[Tv_WkIdx], O_RDWR|O_NDELAY );
					if ( 0 > m_HndlUsb[Tv_WkIdx] )
					{
						DbgMsgPrint("Cls_GrHwCs351x::UsbBugFixing - usb host driver cannot reopen.\n");
					}
				}
			}
			else
			{
				//try open
				m_HndlUsb[Tv_WkIdx]	=	open( m_FnTblUsbDev[Tv_WkIdx], O_RDWR|O_NDELAY );
				if ( 0 > m_HndlUsb[Tv_WkIdx] )
				{
					DbgMsgPrint("Cls_GrHwCs351x::UsbBugFixing - usb host driver cannot reopen.\n");
				}
			}
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrHwCs351x::VmCacheFlush( void )
{
		//do
		ioctl( m_HndlGpio, E_GrHwCs351xGPIO_CACHE_FLUSH );
}
//--------------------------------------------------------------------
void	Cls_GrHwCs351x::SysReboot( void )
{
		//do
		ioctl( m_HndlGpio, E_GrHwCs351xGPIO_SYS_REBOOT );
}
//--------------------------------------------------------------------
