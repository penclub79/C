/*
	linux pci resource tool
*/

//====================================================================
// uses
#include	<Lnx/GrLnxOddIo.h>
#include	<GrStrTool.h>
#ifdef LINUX_APP
	#include <poll.h>
	#include <fcntl.h>
#else
	#include <LnxW32Inc/poll.h>
	#include <LnxW32Inc/fcntl.h>
#endif
#include <GrTimeTool.h>
#include <stdlib.h>
#include <GrDumyTool.h>

//====================================================================
//local const

#define E_GrLnxOddIoP32			0xC0

//message
#define E_GrLnxOddIoMsgReqRec		0

#define E_GrLnxOddIoMsgCnt			1


//====================================================================
//local macro

//====================================================================
//local type

typedef	struct St_GrLnxOddIoMgpRec		
{
	Cls_GrTaskCli*	ObjEvt;		//event object
	BOOL8		IsFmt;						//foramt media
	__u32		FccEvt;						//event fourcc
	BOOL8*		PtrRtRslt;
}	*Ptr_GrLnxOddIoMgpRec;

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrLnxOddIo::Cls_GrLnxOddIo( Cls_GrTaskSvr* A_ObjTask, __s8* A_StrDev )
{
	// local -------------------
		BOOL8		Tv_IsOk;
		St_GrLnxScsiInquiry	Tv_Inq;
	// code --------------------
		//init
		m_ObjTask			=	A_ObjTask;
		m_ObjScsi			=	NULL;
		m_IsRecording	=	FALSE;
		m_IsRecDvd		=	FALSE;
		Tv_IsOk				=	FALSE;
		m_RecIdx			=	0;

		m_MsgAloc			=	m_ObjTask->MsgCmdAlloc( E_GrLnxOddIoMsgCnt );

		GrStrClear( m_StrRecFl );

		//try
		m_ObjScsi	=	(Cls_GrLnxScsi*) new Cls_GrLnxScsi( A_StrDev );
		if ( m_ObjScsi->IsUsable() )
		{
			//check device is cd or dvd
			if ( m_ObjScsi->Inquiry( &Tv_Inq, 36 ) )
			{
				//check cd or dvd device
				if ( E_GrLnxScsiPpDevCdOrDvd == ( 0x1F & Tv_Inq.DevType ) )
				{
					//ok
					Tv_IsOk		=	TRUE;
				}
			}
		}

		//check ok
		if ( !Tv_IsOk )
		{
			delete	m_ObjScsi;
			m_ObjScsi	=	NULL;
		}
}
//--------------------------------------------------------------------
Cls_GrLnxOddIo::~Cls_GrLnxOddIo( )
{
		if ( NULL != m_ObjScsi )
		{
			delete	m_ObjScsi;
			m_ObjScsi	=	NULL;
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::IsUsable( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( NULL != m_ObjScsi )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		Cls_GrLnxOddIo::LcHndlEvent( void )
{
	// local -------------------
		__u32		Tv_Result;
		__u8		Tv_EvtTbl[8];
		__u16		Tv_Profile;
		BOOL8		Tv_IsStarted;
		__s32			Tv_Err;
		__u32		Tv_Descript;
		__u8		Tv_EvtMask;
		Ptr_GrLnxScsiEvtHd	Tv_PtrEvtHd;
		Ptr_GrLnxScsiConfHd	Tv_PtrConfHd;
	// code --------------------
		//init
		Tv_Result		=	0;
		Tv_Err			=	0;
		Tv_Profile	=	0;
		Tv_IsStarted	=	FALSE;
		Tv_EvtMask	=	E_GrLnxScsiEvtMaskAll;		//all event
		Tv_PtrEvtHd	=	(Ptr_GrLnxScsiEvtHd)Tv_EvtTbl;

		while ( 0 != Tv_EvtMask )
		{
			if ( m_ObjScsi->GetEventStatus( Tv_EvtMask, (void*)Tv_EvtTbl, sizeof(Tv_EvtTbl), TRUE ) )
			{
				//update event mask
				Tv_EvtMask	=	Tv_PtrEvtHd->EvtMask;
				//check not change
				if (	( 0 == ( 0x7 & Tv_PtrEvtHd->Atb ) ) ||
							( (0 == Tv_PtrEvtHd->EvtDescLenMsb) && (2 == Tv_PtrEvtHd->EvtDescLenLsb) ) ||
							( 0 == ( 0xF & Tv_EvtTbl[4] ) )
					  )
				{
					//not changed quit
					break;
				}
				//make small endian event descriptor 
				Tv_Descript	=	(Tv_EvtTbl[4]<<24) | (Tv_EvtTbl[5]<<16) | (Tv_EvtTbl[6]<<8 ) | Tv_EvtTbl[7];
				
				//class check
				//not support
				if ( E_GrLnxScsiEvtClsNotSupport == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//not support quit
					break;
				}
				else if ( E_GrLnxScsiEvtClsOpChgReq == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//opration change
					Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskOpChg;		//set reset
					//check change code
					if ( 3 <= ( 0xFFFF & Tv_Descript ) )
					{
						if ( !Tv_IsStarted )
						{
							m_ObjScsi->StartStopUnit( E_GrLnxScsiSsuCmdStart );
							Tv_IsStarted		=	TRUE;
							Tv_Profile			=	0;	
						}
					}
					//read profile
					if ( 0 == Tv_Profile )
					{
						if ( m_ObjScsi->GetConfiguration( Tv_EvtTbl, sizeof(Tv_EvtTbl) ) )
						{
							Tv_PtrConfHd	=	(Ptr_GrLnxScsiConfHd)Tv_EvtTbl;
							Tv_Profile		=	Tv_PtrConfHd->ProfileLsb | ( Tv_PtrConfHd->ProfileMsb << 8 );
						}
					}
				}
				else if ( E_GrLnxScsiEvtClsPwrMng == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//power management
					Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskPwrMng;		
					if ( !Tv_IsStarted )
					{
						m_ObjScsi->StartStopUnit( E_GrLnxScsiSsuCmdStart );
						Tv_IsStarted		=	TRUE;
						Tv_Profile			=	0;	
					}
					if ( 0 == Tv_Profile )
					{
						if ( m_ObjScsi->GetConfiguration( Tv_EvtTbl, sizeof(Tv_EvtTbl) ) )
						{
							Tv_PtrConfHd	=	(Ptr_GrLnxScsiConfHd)Tv_EvtTbl;
							Tv_Profile		=	Tv_PtrConfHd->ProfileLsb | ( Tv_PtrConfHd->ProfileMsb << 8 );
						}
					}
				}
				else if ( E_GrLnxScsiEvtClsExtReq == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//externel request
					Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskExtReq;		//set result
				}
				else if ( E_GrLnxScsiEvtClsMedia == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//media
					Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskMedia;		
					//check media in
					if ( 0 != ( 2 & Tv_EvtTbl[5] ) )
					{
						if ( !Tv_IsStarted )
						{
							m_ObjScsi->StartStopUnit( E_GrLnxScsiSsuCmdStart );
							Tv_IsStarted		=	TRUE;
							Tv_Profile			=	0;	
						}
						if ( 0 == Tv_Profile )
						{
							if ( m_ObjScsi->GetConfiguration( Tv_EvtTbl, sizeof(Tv_EvtTbl) ) )
							{
								Tv_PtrConfHd	=	(Ptr_GrLnxScsiConfHd)Tv_EvtTbl;
								Tv_Profile		=	Tv_PtrConfHd->ProfileLsb | ( Tv_PtrConfHd->ProfileMsb << 8 );
							}
						}
					}
				}
				else if ( E_GrLnxScsiEvtClsMultHost == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//multiple initiator
					Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskMultHost;		
				}
				else if ( E_GrLnxScsiEvtClsDevBusy == (0x7 & Tv_PtrEvtHd->Atb) )
				{
					//device busy
					//timeout occured
					if ( (1 == ( 0xF & Tv_EvtTbl[4] ) ) &&	( 0 != ( 0x3 & Tv_EvtTbl[5] ) ) )
					{
						poll(NULL,0, ((Tv_Descript & 0xFFFF)*100) + 100);
						m_ObjScsi->TestUnitReady();
						Tv_Result		=	Tv_Result | E_GrLnxScsiEvtMaskDevBusy;		
					}
				}
				else
				{
					//reserved
					Tv_Result		=	Tv_Result | (1 << 7 );		
				}
			}
			else
			{
				Tv_EvtMask	=	0;
				break;
			}
		}
	
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcWaitForUnit( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_MsecWait;
		__u32	Tv_TickSt;
		__u32	Tv_TickGap;
		__s32		Tv_Rt;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		Tv_MsecWait	=	1000;

		//wait
		while ( TRUE )
		{
			//test must be 1 sec unit
			if ( 0 < Tv_MsecWait )
			{
				poll( NULL, 0, (__s32)Tv_MsecWait );
			}
			//check start time
			Tv_TickSt	=	GrTimeGetTick();
			//check spend time
			Tv_Rt		=	m_ObjScsi->TestUnitReady();
			if ( E_GrLnxScsiApiOk == Tv_Rt )
			{
				//success
				break;
			}
			else if ( ( E_GrLnxScsiApiNotSupport == Tv_Rt ) || ( E_GrLnxScsiApiNoMedia == Tv_Rt ) )
			{
				//error
				Tv_Result		=	FALSE;
				break;
			}
			
			Tv_TickGap	=	GrTimeTickRange( Tv_TickSt, GrTimeGetTick() );
			Tv_MsecWait	=	0;
			if ( 1000 > Tv_TickGap )
			{
				Tv_MsecWait	=	1000 - Tv_TickGap;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcPioneerStop( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_Rt;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		
		while ( TRUE )
		{
			Tv_Rt		=	m_ObjScsi->StartStopUnit( E_GrLnxScsiSsuCmdStop, TRUE );
			if ( E_GrLnxScsiApiOpInPrograss == Tv_Rt )
			{
				//retry
				poll( NULL, 0, 333 );
			}
			else
			{
				if ( E_GrLnxScsiApiOk == Tv_Rt )
				{
					Tv_Result		=	TRUE;
				}
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcPage5Setup( __u16 A_Profile )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_Profile;
		__u8*		Tv_PtrPage;
		__u8*		Tv_WkPtr;
		__u16		Tv_PageLen;
		__u16		Tv_BlkDiscrptLen;
		__u8		Tv_PageHd[12];
		__u8		Tv_TrackInfo[32];
		St_GrLnxScsiConfHd	Tv_ConfHd;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		Tv_Profile	=	A_Profile;
		Tv_PtrPage	=	NULL;

		//check profile zero
		if ( 0 == Tv_Profile )
		{
			Tv_Result	=	m_ObjScsi->GetConfiguration( &Tv_ConfHd, sizeof(Tv_ConfHd) );
			if ( Tv_Result )
			{
				//get profile
				Tv_Profile	=	( Tv_ConfHd.ProfileMsb << 8 ) | Tv_ConfHd.ProfileLsb;
			}
		}

		if ( Tv_Result )
		{
			if ( m_ObjScsi->ReadTrackInformation( Tv_TrackInfo, sizeof(Tv_TrackInfo), 
							E_GrLnxScsiRtiAdrTrack, 1 ) )
			{
				//read page header
				if ( m_ObjScsi->ModeSense( Tv_PageHd, sizeof(Tv_PageHd), 5 ) )
				{
					//get len
					Tv_PageLen	=	((Tv_PageHd[0] << 8) | Tv_PageHd[1] ) + 2;
					Tv_BlkDiscrptLen	=	(Tv_PageHd[6] << 8) | Tv_PageHd[7];
					if ( 0 != Tv_BlkDiscrptLen)	// should never happen as we set "DBD" above
					{	
						if ( Tv_PageLen <= (8+ Tv_BlkDiscrptLen +14) )
						{
							//error  - LUN is impossible to bear with
							Tv_Result		=	FALSE;
						}
					}
					else if (Tv_PageLen < ( 8 + 2 + (__u32)Tv_PageHd[9] )) // SANYO does this.
					{
						Tv_PageLen	= 8 + 2 + (__u16)Tv_PageHd[9];
					}

					//check able
					if ( Tv_Result )
					{
						//memory allocate
						Tv_PtrPage	=	(__u8*)malloc( Tv_PageLen );
						if ( NULL != Tv_PtrPage )
						{
							//read page
							if ( m_ObjScsi->ModeSense( Tv_PtrPage, Tv_PageLen, 5 ) )
							{
								//patch size
								Tv_PageLen	=	Tv_PageLen - 2;
								if (Tv_PageLen < ( ((__u32)Tv_PtrPage[0]<<8) | Tv_PtrPage[1] ) )	// maybe not need
								{
									Tv_PtrPage[0]		= (__u8)(Tv_PageLen >> 8);
									Tv_PtrPage[1]		= (__u8)(Tv_PageLen & 0xFF );
								}

								//recalculate len
								Tv_PageLen	=	((Tv_PtrPage[0] << 8) | Tv_PtrPage[1] ) + 2;
								Tv_BlkDiscrptLen	=	(Tv_PtrPage[6] << 8) | Tv_PtrPage[7];
								Tv_PageLen	=	Tv_PageLen - Tv_BlkDiscrptLen;
								if ( (8+14) <= Tv_PageLen )		//check LUN is impossible
								{
									Tv_WkPtr	=	Tv_PtrPage + Tv_BlkDiscrptLen;
									//clear head 8 byte
									GrDumyZeroMem( Tv_WkPtr, 8 );
									//set
									Tv_WkPtr			=	Tv_WkPtr + 8;
									Tv_WkPtr[0]		=	Tv_WkPtr[0] & 0x7F;
									// copy "Test Write" and "Write Type" from p32
									Tv_WkPtr[2]		=	Tv_WkPtr[2] & ~0x1F;
									Tv_WkPtr[2]		=	Tv_WkPtr[2] | E_GrLnxOddIoP32 & 0x1F;	
									Tv_WkPtr[2]		=	Tv_WkPtr[2] | 0x40;	// insist on BUFE on		
									// setup Preferred Link Size
									// Preferred Link Size...
									if ( ( 0x11 == Tv_Profile ) || ( 0x14 == Tv_Profile ) )	// Sequential recordings...
									{
										Tv_WkPtr[2]	=	Tv_WkPtr[2]	| 0x20;
										Tv_WkPtr[5] = 0x10;
									}
									else
									{
										Tv_WkPtr[2]	=	Tv_WkPtr[2] & ~0x20;
										Tv_WkPtr[5] = 0;
									}

									// copy Track Mode from TRACK INFORMATION
									// [some DVD-R units (most notably Panasonic LF-D310), insist
									// on Track Mode 5, even though it's effectively ignored]
									Tv_WkPtr[3]	=	Tv_WkPtr[3] & ~0x0F; 
									if ( 0x11 == Tv_Profile )
									{
										Tv_WkPtr[3]	=	Tv_WkPtr[3] | 5;
									}
									else
									{
										Tv_WkPtr[3]	=	Tv_WkPtr[3] | (Tv_TrackInfo[5] & 0x0F);
									}

									// copy "Multi-session" bits from p32
									Tv_WkPtr[3]	=	Tv_WkPtr[3] & ~0xC0;
									Tv_WkPtr[3]	=	Tv_WkPtr[3] | E_GrLnxOddIoP32 & 0xC0;
									if ( 0x13 == Tv_Profile )	// DVD-RW Restricted Overwrite
									{
										Tv_WkPtr[3]	=	Tv_WkPtr[3] & 0x3F;		// always Single-session?
									}

									// setup Data Block Type
									// Some units [e.g. Toshiba/Samsung TS-H542A] return "unknown Data
									// Block Type" in track[6]&0x0F field. Essentially it's a firmware
									// glitch, yet it makes certain sense, as track may not be written
									// yet...
									//check mode 1 track
									if ( ( 1 == ( 0x0F & Tv_TrackInfo[6] ) ) || ( 0x0F == ( 0x0F & Tv_TrackInfo[6] ) ) )
									{
										//mode 1 track ok
										Tv_WkPtr[4] = 8;
										// setup Packet Size
										// [some DVD-R units (most notably Panasonic LF-D310), insist
										// on fixed Packet Size of 16 blocks, even though it's effectively
										// ignored]
										Tv_WkPtr[3]	=	Tv_WkPtr[3] | 0x20;
										Tv_WkPtr[10]	=	0;
										Tv_WkPtr[11]	=	0;
										Tv_WkPtr[12]	=	0;
										Tv_WkPtr[13]	= 0x10;
										if ( 0 != ( 0x10 & Tv_TrackInfo[6] ) )
										{
											//fix
											Tv_WkPtr[10]	=	Tv_TrackInfo[20];		
											Tv_WkPtr[11]	=	Tv_TrackInfo[21];
											Tv_WkPtr[12]	=	Tv_TrackInfo[22];
											Tv_WkPtr[13]	=	Tv_TrackInfo[23];
										}
										else if ( 0x11 != Tv_Profile )
										{
											Tv_WkPtr[3]		=	 Tv_WkPtr[3] & ~0x20;
											Tv_WkPtr[13]	= 0;	// Variable
										}

										switch ( Tv_Profile )
										{	
											case 0x13:	// DVD-RW Restricted Overwrite
												if ( 0 == ( 0x10 & Tv_TrackInfo[6] ) )
												{
													Tv_Result		=	FALSE;
												}
												break;
											case 0x14:	// DVD-RW Sequential Recording
											case 0x11:	// DVD-R  Sequential Recording
												if ( 0 != ( 0x10 & Tv_TrackInfo[6] ) )
												{
													Tv_Result		=	FALSE;
												}
												break;
											default:
												break;
										}

										Tv_WkPtr[8] = 0;		// "Session Format" should be ignored, but
										// I reset it just in case...
										Tv_WkPtr	=	Tv_WkPtr - 8;

										//write
										Tv_Result	=	m_ObjScsi->ModeSelect( Tv_WkPtr, Tv_PageLen );

									}
								}
							}
						}
					}
				}
			}
		}

		if ( NULL != Tv_PtrPage )
		{
			free( Tv_PtrPage );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcGetMediaInfo( __u16* A_PtrRtType, BOOL8* A_PtrRtIsFormated )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrLnxScsiConfHd	Tv_ConfHd;
		St_GrLnxScsiReadDiscInfoStd	Tv_DiscInfo;
	// code --------------------
		//init
		Tv_Result			=	FALSE;
		*A_PtrRtType	=	E_GrLnxScsiPfNone;
		*A_PtrRtIsFormated	=	FALSE;

		//check able
		if ( E_GrLnxScsiApiOk == m_ObjScsi->TestUnitReady() )
		{
			//get profile
			if ( m_ObjScsi->GetConfiguration( &Tv_ConfHd, sizeof(Tv_ConfHd) ) )
			{
				*A_PtrRtType	=	((__u16)Tv_ConfHd.ProfileMsb << 8) | Tv_ConfHd.ProfileLsb;
				//check foramt able
				switch( *A_PtrRtType )
				{
				case E_GrLnxScsiPfDvdMnRwRstOverwt:
					*A_PtrRtIsFormated	=	TRUE;
					break;
				case	E_GrLnxScsiPfCdRw:
				case	E_GrLnxScsiPfDvdPlRw:
				case	E_GrLnxScsiPfDvdPlRwDl:
					if ( m_ObjScsi->ReadDiscInformation( &Tv_DiscInfo, sizeof(Tv_DiscInfo) ) )
					{
						if ( E_GrLnxScsiRdiDiscStatEmpty != ( 0x03 & Tv_DiscInfo.Atb1 ) )
						{
							*A_PtrRtIsFormated	=	TRUE;
						}
					}
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::GetMediaInfo( __u16* A_PtrRtType, BOOL8* A_PtrRtIsFormated )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result			=	FALSE;
		*A_PtrRtType	=	E_GrLnxScsiPfNone;
		*A_PtrRtIsFormated	=	FALSE;

		//check able
		if ( (NULL != m_ObjScsi) && (!m_IsRecording) )
		{
			Tv_Result	=	LcGetMediaInfo( A_PtrRtType, A_PtrRtIsFormated );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::Eject( BOOL8 A_IsTrayOpen )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u8		Tv_Cmd;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//check able
		if ( (NULL != m_ObjScsi) && (!m_IsRecording) )
		{
			Tv_Cmd		=	E_GrLnxScsiSsuCmdLoad;
			if ( A_IsTrayOpen )
			{
				Tv_Cmd		=	E_GrLnxScsiSsuCmdEject;
			}
			if ( E_GrLnxScsiApiOk == m_ObjScsi->StartStopUnit( Tv_Cmd, TRUE ) )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcFormat( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u16		Tv_OddMdType;				//odd media type
		BOOL8		Tv_IsFormatted;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( !m_IsRecording )
		{
			if ( GetMediaInfo( &Tv_OddMdType, &Tv_IsFormatted ) )
			{
				//check type
				if (	( E_GrLnxScsiPfDvdMnRwRstOverwt == Tv_OddMdType) ||
							( E_GrLnxScsiPfDvdMnRwSeq == Tv_OddMdType) ||  
							( E_GrLnxScsiPfDvdPlRw == Tv_OddMdType) || 
							( E_GrLnxScsiPfDvdPlRwDl == Tv_OddMdType) )
				{
					//dvd rw
					if ( Tv_IsFormatted )
					{
						//erase
						if ( 0 == system( "dvd+rw-format -lead-out /dev/dvd" ) )
						{
							Tv_Result		=	TRUE;
						}
					}
					else
					{
						//format
						if ( 0 == system( "dvd+rw-format -force /dev/dvd" ) )
						{
							Tv_Result		=	TRUE;
						}
					}
				}
				else if ( E_GrLnxScsiPfCdRw == Tv_OddMdType )
				{
					//cd erase
					if ( 0 == system( "cdrecord gracetime=0 -blank=fast" ) )
					{
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::Format( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( !m_IsRecording )
		{
			Tv_Result	=	LcFormat();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::IsExistMedia( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=		m_IsRecording;

		if ( (!Tv_Result) && (NULL != m_ObjScsi) )
		{
			Tv_Result		=	m_ObjScsi->IsOddMediaExist();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::IsTrayOpen( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=		FALSE;

		if ( (!m_IsRecording) && (NULL != m_ObjScsi) )
		{
			Tv_Result		=	m_ObjScsi->IsOddTrayOpen();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcProcBkRec( void )
{
	// local -------------------
		St_GrTaskMsg	Tv_Msg;
		Ptr_GrLnxOddIoMgpRecEvt	Tv_PtrMgp;
		BOOL8	Tv_IsQuit;
	// code --------------------
		if ( m_IsRecording )
		{
			//prepare event
			Tv_IsQuit	=	FALSE;
			GrStrClear( m_StrRecFl );
			Tv_Msg.Cmd	=	m_RevtFcc;
			Tv_PtrMgp	=	(Ptr_GrLnxOddIoMgpRecEvt)Tv_Msg.Para;
			Tv_PtrMgp->EvtCode		=	E_GrLnxOddIoEvtRecPre;
			Tv_PtrMgp->PtrIsQuit	=	&Tv_IsQuit;
			Tv_PtrMgp->Idx				=	m_RecIdx;
			Tv_PtrMgp->StrFl			=	m_StrRecFl;
			if ( !m_ObjRevt->RtlCallback( &Tv_Msg ) )
			{
				Tv_IsQuit	=	TRUE;
			}

			//check quit
			if ( !Tv_IsQuit )
			{
				//check media
				if ( m_IsRecDvd )
				{
					//dvd
					Tv_IsQuit	=	!LcDvdRec();
				}
				else
				{
					//cd
					Tv_IsQuit	=	!LcCdRec();
				}
				//check success
				if ( Tv_IsQuit )
				{
					//error event
					Tv_Msg.Cmd	=	m_RevtFcc;
					Tv_PtrMgp	=	(Ptr_GrLnxOddIoMgpRecEvt)Tv_Msg.Para;
					Tv_PtrMgp->EvtCode		=	E_GrLnxOddIoEvtRecErr;
					m_ObjRevt->RtlCallback( &Tv_Msg );
					//stop recording
					m_IsRecording		=	FALSE;
				}
				else
				{
					//next
					m_RecIdx ++;
				}
			}
			else
			{
				//stop record
				m_IsRecording		=	FALSE;
			}
		}

		return	m_IsRecording;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcDvdRec( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( 0 == m_RecIdx )
		{
			GrStrCopy( m_StrRecCmd, "growisofs -Z /dev/dvd -R -J " );
		}
		else
		{
			GrStrCopy( m_StrRecCmd, "growisofs -M /dev/dvd -R -J " );
		}
		//add file name
		GrStrCat( m_StrRecCmd, m_StrRecFl );

		if ( 0 <= system( m_StrRecCmd ) )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::LcCdRec( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( 0 == m_RecIdx )
		{
			GrStrCopy( m_StrRecCmd, "mkisofs -R -J " );
		}
		else
		{
			GrStrCopy( m_StrRecCmd, "mkisofs -R -J -C `cdrecord -msinfo` -M /dev/dvd " );
		}
		//add file name
		GrStrCat( m_StrRecCmd, m_StrRecFl );
		//add write command
		GrStrCat( m_StrRecCmd, " | cdrecord gracetime=0 driveropts=burnfree dev=/dev/dvd -multi -");
		if ( 0 <= system( m_StrRecCmd ) )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrLnxOddIo::LcProcMsgRec( Ptr_GrTaskMsg A_PtrMsg )
{
	// local -------------------
		Ptr_GrLnxOddIoMgpRec	Tv_PtrMgp;
		__u16		Tv_MdType;
		BOOL8		Tv_IsFmted;
		BOOL8		Tv_IsAble;
	// code --------------------
		//init
		Tv_PtrMgp		=	(Ptr_GrLnxOddIoMgpRec)A_PtrMsg->Para;
		*(Tv_PtrMgp->PtrRtRslt)	=	FALSE;

		//check able
		if ( !m_IsRecording )
		{
			//check media
			if ( LcGetMediaInfo( &Tv_MdType, &Tv_IsFmted ) )
			{
				//check media type
				Tv_IsAble	=	TRUE;
				switch( Tv_MdType )
				{
					//dvd - not need foramt
					case E_GrLnxScsiPfDvdMnRSeq:
					case E_GrLnxScsiPfDvdMnRwRstOverwt:
					case E_GrLnxScsiPfDvdMnRDlSeq:
					case E_GrLnxScsiPfDvdMnRDlJmp:
					case E_GrLnxScsiPfDvdPlR:
					case E_GrLnxScsiPfDvdPlRDl:
						m_IsRecDvd			=	TRUE;
						m_IsRecNeedFmt	=	FALSE;
						break;
					//dvd - must need foramt
					case E_GrLnxScsiPfDvdMnRwSeq:
						m_IsRecDvd			=	TRUE;
						m_IsRecNeedFmt	=	TRUE;
						break;
					//dvd - check foramt
					case E_GrLnxScsiPfDvdPlRw:
					case E_GrLnxScsiPfDvdPlRwDl:
						m_IsRecDvd			=	TRUE;
						m_IsRecNeedFmt	=	!Tv_IsFmted;
						break;
					//cd
					case E_GrLnxScsiPfCdR:
						m_IsRecDvd			=	FALSE;
						m_IsRecNeedFmt	=	FALSE;
						break;
					case E_GrLnxScsiPfCdRw:
						m_IsRecDvd			=	FALSE;
						m_IsRecNeedFmt	=	TRUE;
						break;
					default:
						Tv_IsAble		=	FALSE;
						break;
				}
				//check able
				if ( Tv_IsAble )
				{
					//build enviroment
					m_ObjRevt			=	Tv_PtrMgp->ObjEvt;
					m_RevtFcc			=	Tv_PtrMgp->FccEvt;
					m_RecIdx			=	0;
					m_IsRecording	=	TRUE;

					*(Tv_PtrMgp->PtrRtRslt)	=	TRUE;
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::Record( Cls_GrTaskCli* A_ObjEvt, __u32 A_EvtFcc, BOOL8 A_IsFmt )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrLnxOddIoMgpRec	Tv_Mgp;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//set para
		Tv_Mgp.FccEvt			=	A_EvtFcc;
		Tv_Mgp.IsFmt			=	A_IsFmt;
		Tv_Mgp.ObjEvt			=	A_ObjEvt;
		Tv_Mgp.PtrRtRslt	=	&Tv_Result;
				
		m_ObjTask->MsgSend( m_RevtFcc, TRUE, &Tv_Mgp, sizeof(Tv_Mgp) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::RtlMsgProc( Ptr_GrTaskMsg A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (E_GrLnxOddIoMsgReqRec + m_MsgAloc) == A_PtrMsg->Cmd )
		{
			LcProcMsgRec( A_PtrMsg );
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrLnxOddIo::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	LcProcBkRec();

		return	Tv_Result;
}
//--------------------------------------------------------------------


