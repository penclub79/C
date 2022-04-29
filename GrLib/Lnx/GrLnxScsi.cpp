/*
	linux scsi
*/

//====================================================================
// uses
#include	<Lnx/GrLnxScsi.h>
#include	<GrStrTool.h>
#include	<GrDumyTool.h>
#ifdef LINUX_APP
	#include <fcntl.h>
	#include <scsi/scsi.h>
#else
	#include <LnxW32Inc/fcntl.h>
	#include <LnxW32Inc/scsi/scsi.h>
#endif

//====================================================================
//local const

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrLnxScsi::Cls_GrLnxScsi( __s8* A_StrDev )
{
	// local -------------------
		struct stat Tv_StStat;
	// code --------------------
		//init
		GrStrClear( m_StrDev );
		m_IsUsable	=	FALSE;
		m_HndlFl	=	open( A_StrDev,O_RDWR|O_NONBLOCK );
		if ( 0 > m_HndlFl )
		{
			m_HndlFl	=	open( A_StrDev,O_RDONLY|O_NONBLOCK );
		}

		//check open success
		if ( 0 <= m_HndlFl )
		{
			//check attribute
			if ( 0 <= fstat( m_HndlFl, &Tv_StStat ) )
			{
				//check block device
				if ( S_ISBLK(Tv_StStat.st_mode) )		
				{
					m_IsUsable	=	TRUE;
					GrStrCopy( m_StrDev, A_StrDev );
				}
			}
			//bad status
			if ( !m_IsUsable )
			{
				close( m_HndlFl );
				m_HndlFl	=	-1;
			}
		}
}
//--------------------------------------------------------------------
Cls_GrLnxScsi::~Cls_GrLnxScsi()
{
		if ( m_IsUsable )
		{
			close( m_HndlFl );
			m_HndlFl	=	-1;

			m_IsUsable	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrLnxScsi::LcCmdInit( void )
{
		GrDumyZeroMem( m_CmdBuf, sizeof(m_CmdBuf) );
		GrDumyZeroMem( m_SensBuf, sizeof(m_SensBuf) );
		GrDumyZeroMem( &m_SgIo, sizeof(m_SgIo) );
		m_SgIo.interface_id	= 'S';
		m_SgIo.mx_sb_len		= sizeof(m_SensBuf);
		m_SgIo.cmdp					= m_CmdBuf;
		m_SgIo.sbp					= m_SensBuf;
		m_SgIo.flags				= SG_FLAG_LUN_INHIBIT|SG_FLAG_DIRECT_IO;
}
//--------------------------------------------------------------------
__s32		Cls_GrLnxScsi::LcMakeRetErrCode( void )
{
		return	(((m_SgIo.sbp[2] & 0x0F)<<16) | (m_SgIo.sbp[12] << 8) | (m_SgIo.sbp[13]));
}
//--------------------------------------------------------------------
void	Cls_GrLnxScsi::LcBuildErrno( void )
{
		switch ( m_SgIo.sbp[12] )
		{	
			case 0x04:	
				errno	=	EAGAIN;	
				break;
			case 0x20:	
				errno	=	ENODEV;	
				break;
			case 0x21:	
				if ( 0 == m_SgIo.sbp[13] )	
				{
					errno	=	ENOSPC;
				}
				else		
				{
					errno	=	EINVAL;
				}
				break;
			case 0x30:	
				errno	=	EMEDIUMTYPE;	
				break;
			case 0x3A:	
				errno	=	ENOMEDIUM;	
				break;
		}
}
//--------------------------------------------------------------------
__s32		Cls_GrLnxScsi::LcSendCmd( __u32 A_Dir, void* A_PtrBuf, __u32 A_BufSize )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	-1;
		m_SgIo.dxferp			= A_PtrBuf;
		m_SgIo.dxfer_len	= A_BufSize;
		m_SgIo.dxfer_direction	= A_Dir;
		//run
		if ( 0 == ioctl( m_HndlFl, SG_IO, &m_SgIo ) )
		{
			//check error status
			if ( SG_INFO_OK != (SG_INFO_OK_MASK & m_SgIo.info ) )
			{
				errno		=	EIO;
				if ( 0 != ( CHECK_CONDITION & m_SgIo.masked_status ) )
				{
					Tv_Result = LcMakeRetErrCode();
					if ( 0 == Tv_Result ) 
					{
						Tv_Result	=	-1;
					}
					else	
					{
						LcBuildErrno();
					}
				}
			}
			else
			{
				//success
				Tv_Result		=	0;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::IsUsable( void )
{
		return	m_IsUsable;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::IsOddMediaChanged( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( m_IsUsable )
		{
			if ( 0 == ioctl ( m_HndlFl,CDROM_MEDIA_CHANGED,CDSL_CURRENT) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::GetEventStatus( __u8 A_EvtMask, void* A_PtrRtBuf, __u16 A_BufSize, BOOL8 A_IsPoll )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x4A;		// GET EVENT
			if ( A_IsPoll )
			{
				m_CmdBuf[1]		= 1;		// "Polled"
			}
			m_CmdBuf[4]		= A_EvtMask;
			m_CmdBuf[7]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[8]		= (__u8)( A_BufSize & 0xFF );
			m_CmdBuf[9]		= 0;
			m_SgIo.cmd_len	=	10;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirRead, A_PtrRtBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::GetConfiguration( void* A_PtrRtBuf, __u16 A_BufSize, __u8 A_RtType, __u16 A_StIdx )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		
		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x46;				 
			m_CmdBuf[1]		= A_RtType;		
			m_CmdBuf[2]		=	(__u8)( (A_StIdx >> 8) & 0xFF );
			m_CmdBuf[3]		= (__u8)( A_StIdx & 0xFF );
			m_CmdBuf[7]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[8]		= (__u8)( A_BufSize & 0xFF );
			m_CmdBuf[9]		= 0;
			m_SgIo.cmd_len	=	10;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirRead, A_PtrRtBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrLnxScsi::StartStopUnit( __u8 A_SsuCmd, BOOL8 A_IsImmed, __u8 A_Power )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_Rt;
	// code --------------------
		//init
		Tv_Result		=	E_GrLnxScsiApiError;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x1B;				 
			if ( A_IsImmed )
			{
				m_CmdBuf[1]		=	1;
			}
			m_CmdBuf[4]		=	((A_Power & 0xF) << 4) | ( A_SsuCmd & 3 );
			m_CmdBuf[5]		= 0;
			m_SgIo.cmd_len	=	6;			

			Tv_Rt		=	LcSendCmd( );
			if ( 0 == Tv_Rt )
			{
				Tv_Result		=	E_GrLnxScsiApiOk;
			}
			else if ( 0x20407 == Tv_Rt )
			{
				Tv_Result		=	E_GrLnxScsiApiOpInPrograss;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32		Cls_GrLnxScsi::TestUnitReady( void )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrLnxScsiApiError;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x00;				 
			m_SgIo.cmd_len	=	6;				

			if ( 0 == LcSendCmd( ) )
			{
				Tv_Result		=	E_GrLnxScsiApiOk;
			}
			else
			{
				if ( 0 == ( 0x70 & m_SensBuf[0] ) )
				{
					Tv_Result		=	E_GrLnxScsiApiNotSupport;
				}
				else if ( 0x3A == m_SensBuf[12] )
				{
					Tv_Result		=	E_GrLnxScsiApiNoMedia;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::ReadTrackInformation( void* A_PtrRtBuf, __u16 A_BufSize, 
									__u8 A_AdrType, __u32 A_Adr, BOOL8 A_IsAdrOpen )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x52;				 
			m_CmdBuf[1]		= 0x3 & A_AdrType;		
			if ( A_IsAdrOpen )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x4;
			}
			m_CmdBuf[2]		=	(__u8)( (A_Adr >> 24) & 0xFF );
			m_CmdBuf[3]		=	(__u8)( (A_Adr >> 16) & 0xFF );
			m_CmdBuf[4]		=	(__u8)( (A_Adr >> 8) & 0xFF );
			m_CmdBuf[4]		=	(__u8)( A_Adr & 0xFF );

			m_CmdBuf[7]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[8]		= (__u8)( A_BufSize & 0xFF );

			m_CmdBuf[9]		= 0;
			m_SgIo.cmd_len	=	10;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirRead, A_PtrRtBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::ModeSense(	void* A_PtrRtBuf, __u16 A_BufSize, __u8 A_PageCode, 
															__u8 A_SubPage = 0, BOOL8 A_IsDbd, __u8 A_PageCtrl, 
															BOOL8 A_IsLba48 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x5A;
			if ( A_IsDbd )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x8;
			}
			if ( A_IsLba48 )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x10;
			}
			m_CmdBuf[2]	=	( (A_PageCtrl & 0x3) << 6) | ( A_PageCode & 0x3F );
			m_CmdBuf[3]	=	A_SubPage;

			m_CmdBuf[7]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[8]		= (__u8)( A_BufSize & 0xFF );

			m_CmdBuf[9]		= 0;
			m_SgIo.cmd_len	=	10;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirRead, A_PtrRtBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::ModeSelect( void* A_PtrBuf, __u16 A_BufSize, BOOL8 A_IsCompatible, BOOL8 A_IsSavePage )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x55;
			if ( A_IsCompatible )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x10;
			}
			if ( A_IsSavePage )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x01;
			}

			m_CmdBuf[7]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[8]		= (__u8)( A_BufSize & 0xFF );

			m_CmdBuf[9]		= 0;
			m_SgIo.cmd_len	=	10;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirWrite, A_PtrBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::Inquiry( void* A_PtrRtBuf, __u16 A_BufSize, BOOL8 A_IsVpd,__u8 A_PageCode )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check able
		if ( m_IsUsable )
		{
			LcCmdInit();

			m_CmdBuf[0]		= 0x12;
			if ( A_IsVpd )
			{
				m_CmdBuf[1]	= m_CmdBuf[1] | 0x01;
			}
			m_CmdBuf[2]		= A_PageCode;
			m_CmdBuf[3]		=	(__u8)( (A_BufSize >> 8) & 0xFF );
			m_CmdBuf[4]		= (__u8)( A_BufSize & 0xFF );

			m_CmdBuf[5]		= 0;
			m_SgIo.cmd_len	=	6;			

			if ( 0 == LcSendCmd( E_GrLnxScsiDirRead, A_PtrRtBuf, A_BufSize ) )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::IsOddMediaExist( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( m_IsUsable )
		{
			if ( CDS_DISC_OK == ioctl( m_HndlFl, CDROM_DRIVE_STATUS, 0 ) )
			{
				Tv_Result		=	TRUE;
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxScsi::IsOddTrayOpen( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( m_IsUsable )
		{
			if ( CDS_TRAY_OPEN == ioctl( m_HndlFl, CDROM_DRIVE_STATUS, 0 ) )
			{
				Tv_Result		=	TRUE;
			}
		}
}
//--------------------------------------------------------------------



