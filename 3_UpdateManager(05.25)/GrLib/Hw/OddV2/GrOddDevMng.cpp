/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevMng.h>
#include <Hw/OddV2/GrOddDevMmc.h>
#include <Hw/OddV2/GrOddDevMmcSony.h>
#include <Hw/OddV2/GrOddDevCd.h>
#include <Hw/OddV2/GrOddDevOldCd.h>
#include <Hw/OddV2/GrOddDevDvd.h>
#include <Hw/OddV2/GrOddDevDvdPlus.h>
#include <Hw/OddV2/GrOddDevDvdPlusR.h>
#include <Hw/OddV2/GrOddDevDvdPlusRW.h>
#include <Hw/OddV2/GrOddDevCdDvd.h>
#include <Hw/OddV2/GrOddDevBd.h>
#include <Hw/OddV2/GrOddDevBdR.h>
#include <Hw/OddV2/GrOddDevBdRE.h>
#include <Hw/OddV2/GrOddDevBdRom.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrProcess.h>

#include <errno.h>

#ifdef LINUX_APP
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#endif

//====================================================================
//local const
#define E_GrOddRecFlushWaitMs			20

#define	MIN_GRACE_TIME	3		/* 3 seconds */
#define	GRACE_TIME	9		/* 9 seconds */

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
Cls_GrOddDevBase* V_PtrDevMmc;
Cls_GrOddDevBase* V_PtrDevMmcSony;
Cls_GrOddDevBase* V_PtrDevCd;
Cls_GrOddDevBase* V_PtrDevOldCd;
Cls_GrOddDevBase* V_PtrDevDvd;
Cls_GrOddDevBase* V_PtrDevDvdPlus;
Cls_GrOddDevBase* V_PtrDevDvdPlusR;
Cls_GrOddDevBase* V_PtrDevDvdPlusRW;
Cls_GrOddDevBase* V_PtrCdDvd;
Cls_GrOddDevBase* V_PtrDevBd;
Cls_GrOddDevBase* V_PtrDevBdRom;
Cls_GrOddDevBase* V_PtrDevBdR;
Cls_GrOddDevBase* V_PtrDevBdRE;

struct timeval	starttime;
struct timeval	wstarttime;
struct timeval	stoptime;
struct timeval	fixtime;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevMng::Cls_GrOddDevMng()
{
	// var --------------------
	__u32 		Tv_WkCnt;
	// code -------------------
		//init
		m_PtrScg			=	NULL;
		m_PtrDev			=	NULL;
		GrStrClear(m_StrDev);

		m_Stage				=	E_GrOddStageNone;
		m_IsFinish	=	FALSE;

		m_TimeOut 	= 200;
		m_PtrBuf		=	NULL;
		m_BufSize 	= -1;
		m_Speed			= -1;	
		m_DmaSpeed	= -1;
		m_Tracks 			= 0;
		m_RawSpeed	= -1;
		GrDumyZeroMem(m_Track, sizeof(m_Track));
		m_startsec	= 0L;
		m_blanktype = 0;


		m_Tracks = 1;
		for (int i = 0; i < E_GrOddDevMngMaxTrack+2; i++)
			m_Track[i].tracks = m_Tracks;

		m_Track[0].tracktype = TOC_ROM;
		m_Track[0].dbtype = DB_ROM_MODE1;
		m_Track[0].isecsize = DATA_SEC_SIZE;
		m_Track[0].secsize = DATA_SEC_SIZE;

		m_Track[1].trackstart 	= 0L;
		m_Track[1].itracksize		= -1;
		m_Track[1].tracksize		= -1;
		m_Track[1].tracksecs		= -1L;
		m_Track[1].pregapsize		= -1;
		m_Track[2].pregapsize		= -1;
		m_Track[1].padsecs			= -1;
		m_Track[1].isecsize			= DATA_SEC_SIZE;
		m_Track[1].secsize			= DATA_SEC_SIZE;
		m_Track[1].secspt				= 0;
		m_Track[1].pktsize			= 0;
		m_Track[1].trackno			= 1;
		m_Track[1].sectype			= 33;
		m_Track[1].dataoff			= 16;
		m_Track[1].tracktype		= 1;
		m_Track[1].dbtype				= 8;
		m_Track[1].nindex				= 1;
		m_Track[1].tindex				= 0;
		m_Track[1].flags				= F_WAITI | F_NOFIX;

		if (m_BufSize < 0L)
			m_BufSize = CDR_BUF_SIZE;
		if (m_BufSize > CDR_MAX_BUF_SIZE)
			m_BufSize = CDR_MAX_BUF_SIZE;

		for(Tv_WkCnt = 0;Tv_WkCnt < E_GrOddDevMngMaxTrack + 2;Tv_WkCnt++)
		{
			m_Track[Tv_WkCnt].track = m_Track[Tv_WkCnt].trackno = (__u8)Tv_WkCnt;
			m_Track[0].tracktype = TOC_MASK;
		}

		V_PtrDevMmc					= (Cls_GrOddDevBase*)new Cls_GrOddDevMmc;
		V_PtrDevMmcSony			= (Cls_GrOddDevBase*)new Cls_GrOddDevMmcSony;
		V_PtrDevCd					= (Cls_GrOddDevBase*)new Cls_GrOddDevCd;
		V_PtrDevOldCd				= (Cls_GrOddDevBase*)new Cls_GrOddDevOldCd;
		V_PtrDevDvd					= (Cls_GrOddDevBase*)new Cls_GrOddDevDvd;
		V_PtrDevDvdPlus			= (Cls_GrOddDevBase*)new Cls_GrOddDevDvdPlus;
		V_PtrDevDvdPlusR		= (Cls_GrOddDevBase*)new Cls_GrOddDevDvdPlusR;
		V_PtrDevDvdPlusRW		= (Cls_GrOddDevBase*)new Cls_GrOddDevDvdPlusRW;
		V_PtrCdDvd					= (Cls_GrOddDevBase*)new Cls_GrOddDevCdDvd;
		V_PtrDevBd					= (Cls_GrOddDevBase*)new Cls_GrOddDevBd;
		V_PtrDevBdR					= (Cls_GrOddDevBase*)new Cls_GrOddDevBdR;
		V_PtrDevBdRE				= (Cls_GrOddDevBase*)new Cls_GrOddDevBdRE;
		V_PtrDevBdRom				= (Cls_GrOddDevBase*)new Cls_GrOddDevBdRom;
}
//--------------------------------------------------------------------
Cls_GrOddDevMng::~Cls_GrOddDevMng()
{

}
//--------------------------------------------------------------------
BOOL Cls_GrOddDevMng::Init(char* A_StrDev)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check initialized
		if ( E_GrOddStageNone == m_Stage )
		{
			//init variable
			LcInitVar( A_StrDev );

			m_FileCnt			=	0;
			GrDumyZeroMem( m_FileTbl, sizeof(m_FileTbl) );
			m_PrgSctPos		=	0;
			m_BufSctCnt		=	0;
			m_FilePrgIdx	=	0;
			m_BufInSize		=	0;
			m_IsoTtSize		=	(E_GrIsoPosFileData << E_GrIsoSectorShift);
			m_IsoTtSct		=	E_GrIsoPosFileData;
			m_TailGapResid	=	(E_GrIsoLastGapSctCnt - 0x1E) << E_GrIsoSectorShift;	//need buf fix

#ifndef LINUX_APP
			//emulation file create
			GrFileDelete( L"E:\\EmuGh\\Ramdisk\\backup.iso", TRUE );
			m_IsoFc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( L"E:\\EmuGh\\Ramdisk\\backup.iso", TRUE,
				TRUE, FALSE, TRUE );
			m_PtrBuf	=	(char*)malloc( 0x10000 );
#endif

			//initialized
			m_Stage ++;
			Tv_Result			=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcInitVar( char* A_StrDev )
{
	// local -------------------
		int		Tv_WkIdx;
	// code --------------------
		m_TimeOut 	= 200;
		m_BufSize		= -1;
		GrStrCopy( m_StrDev , A_StrDev );

		m_PtrDev	 	= NULL;
		m_Speed			= -1;	
		m_DmaSpeed	= -1;
		m_Tracks 		= 0;
		m_RawSpeed	= -1;
		GrDumyZeroMem(m_Track, sizeof(m_Track));
		m_startsec	= 0L;
		m_blanktype = 0;

		//track init

		m_Tracks = 1;
		for (int i = 0; i < E_GrOddDevMngMaxTrack+2; i++)
			m_Track[i].tracks = m_Tracks;

		m_Track[0].tracktype = TOC_ROM;
		m_Track[0].dbtype = DB_ROM_MODE1;
		m_Track[0].isecsize = DATA_SEC_SIZE;
		m_Track[0].secsize = DATA_SEC_SIZE;

		m_Track[1].trackstart 	= 0L;
		m_Track[1].itracksize		= -1;
		m_Track[1].tracksize		= -1;
		m_Track[1].tracksecs		= -1L;
		m_Track[1].pregapsize		= -1;
		m_Track[2].pregapsize		= -1;
		m_Track[1].padsecs			= -1;
		m_Track[1].isecsize			= DATA_SEC_SIZE;
		m_Track[1].secsize			= DATA_SEC_SIZE;
		m_Track[1].secspt				= 0;
		m_Track[1].pktsize			= 0;
		m_Track[1].trackno			= 1;
		m_Track[1].sectype			= 33;
		m_Track[1].dataoff			= 16;
		m_Track[1].tracktype		= 1;
		m_Track[1].dbtype				= 8;
		m_Track[1].nindex				= 1;
		m_Track[1].tindex				= 0;
		m_Track[1].flags				= F_WAITI | F_NOFIX;

		if (m_BufSize < 0L)
			m_BufSize = CDR_BUF_SIZE;
		if (m_BufSize > CDR_MAX_BUF_SIZE)
			m_BufSize = CDR_MAX_BUF_SIZE;

		for(Tv_WkIdx = 0;Tv_WkIdx < E_GrOddDevMngMaxTrack + 2;Tv_WkIdx++)
		{
			m_Track[Tv_WkIdx].track = m_Track[Tv_WkIdx].trackno = Tv_WkIdx;
			m_Track[0].tracktype = TOC_MASK;
		}

}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcInitDev(__u64 A_IsoSize, BOOL A_IsWrite)
{
	// local -------------------
		BOOL	Tv_Result;
		int 	Tv_WkCnt;
		int		Tv_Flags;
	// code --------------------
		Tv_Result	=	FALSE;

		m_CmdFlag		= F_WRITE | F_SAO | F_EJECT;
		m_Speed 		= -1;

		m_PtrScg		= (Cls_GrOddScg*) new Cls_GrOddScg((m_CmdFlag & F_MSINFO) == 0);
		if(NULL != m_PtrScg)
		{
			if(m_PtrScg->Init(m_StrDev))
			{
				DbgMsgPrint("Cls_GrOddRec::LcInitDev() - scg_open init\n");
			}
			else
			{
				DbgMsgPrint("Cls_GrOddRec::LcInitDev() - scg_open init failed\n");
			}
			m_PtrScg->TimeOut(m_TimeOut);
			m_PtrScg->cap.c_bsize = DATA_SEC_SIZE;
			m_BufSize = m_PtrScg->BufSize(m_BufSize);
			m_PtrBuf	= (char*)m_PtrScg->scg_getbuf(m_BufSize);

			if(NULL != m_PtrBuf)
			{
				if((0 > scg_scsibus(m_PtrScg)) && (0 > scg_target(m_PtrScg)) && 
					(0 > scg_lun(m_PtrScg)))
				{
					Tv_WkCnt = find_drive(m_PtrScg, m_StrDev, m_CmdFlag);
				}
				errno = 0;

				Tv_WkCnt = test_unit_ready(m_PtrScg);

				if(0 > Tv_WkCnt)
				{
					Tv_WkCnt = errno;
					if((EPERM == Tv_WkCnt) || (EACCES == Tv_WkCnt))
					{
						DbgMsgPrint( "Cls_GrOddRec::LcInitDev() - cannot open device\n" );
						return	FALSE;
					}
				}
				do_inquiry(m_PtrScg, (m_CmdFlag & F_MSINFO) == 0 );

				//allocate media driver
				m_PtrDev = (Cls_GrOddDevBase*) get_cdrcmds(m_PtrScg);
				if(NULL == m_PtrDev)
				{
					DbgMsgPrint("Cls_GrOddDevMng::LcInitDev() - media driver not find\n");
					return FALSE;
				}

				//check work able
				if (is_cddrive(m_PtrScg))
				{
					Tv_Flags = m_PtrDev->GetFlags();
					Tv_Flags |= CDR_ALLOC;
					m_PtrDev->SetFlags(Tv_Flags);
					m_PtrDev->SetCmdFlags(m_CmdFlag);

					GrDumyZeroMem( &m_DskStat, sizeof(m_DskStat) );
					m_DskStat.ds_trackp		= m_Track;
					m_DskStat.ds_minbuf		= 0xFFFF;
					m_DskStat.ds_layer_break	= -1;
					m_PtrDev->SetDstat(&m_DskStat);

					if (!is_mmc(m_PtrScg, NULL, NULL) && m_BufSize > CDR_OLD_BUF_SIZE)
						m_BufSize = CDR_OLD_BUF_SIZE;

					if ((m_CmdFlag & (F_MSINFO|F_TOC|F_LOAD|F_DLCK|F_EJECT)) == 0 ||
						m_Tracks > 0)
					{
						if ((m_PtrDev->GetFlags() & CDR_ISREADER) != 0)
						{
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - this drive can not writing.\n");
							return	FALSE;
						}
					}

					if (m_PtrDev->attach(m_PtrScg) != 0)
					{
						DbgMsgPrint("Cls_GrOddRec::LcInitDev - cdr attach error.\n");
						return	FALSE;
					}

					if (m_Speed > (int)m_PtrDev->GetSpeedMax() && (m_CmdFlag & F_FORCE) == 0)
						m_Speed = m_PtrDev->GetSpeedMax();
					if (m_Speed < 0)
						m_Speed = m_PtrDev->GetSpeedDef();
					m_DskStat.ds_wspeed		= m_Speed;

					exargs.scgp		= m_PtrScg;
					exargs.dp			= m_PtrDev;
					exargs.old_secsize = -1;
					exargs.flags	   = m_CmdFlag;

					m_DmaSpeed = get_dmaspeed(m_PtrScg, m_PtrDev, m_BufSize );
					if (m_DmaSpeed <= 0)
						printf("Warning: The DMA speed test has been skipped.\n");

					if ((m_CmdFlag & F_RAW) != 0 && (m_DskStat.ds_flags & DSF_NOCD) == 0)
					{
						m_RawSpeed = encspeed();
					}

					if (m_PtrDev->GetFlags2() & CDR2_NOCD)
					{
						for (Tv_WkCnt = 0;Tv_WkCnt < E_GrOddDevMngMaxTrack+2;Tv_WkCnt++)
							m_Track[Tv_WkCnt].flags |= TI_NOCD;

						if (m_CmdFlag & F_MULTI)
						{
							if ((m_PtrDev->GetFlags() & CDR_PACKET) == 0)
							{
								printf("Drive does not support packet writing (needed for -multi).\n");
							}

							m_CmdFlag |= F_SAO;
							m_PtrDev->SetCmdFlags(m_CmdFlag);
							for (Tv_WkCnt = 0;Tv_WkCnt < E_GrOddDevMngMaxTrack+2;Tv_WkCnt++)
							{
								m_Track[Tv_WkCnt].flags &= ~TI_TAO;
								m_Track[Tv_WkCnt].flags |= TI_SAO;
							}
						}
					}

					if ((m_CmdFlag & F_ABORT) != 0)
					{
						scsi_flush_cache(m_PtrScg, FALSE);
						m_PtrDev->abort_session(m_PtrScg);

						return	TRUE;		//abort session only
					}

					opentracks(m_Track, A_IsoSize);

					if (m_Tracks > 1)
						sleep(2);	/* Let the user watch the inquiry messages */

					if (m_Tracks > 0 && !check_wrmode(m_PtrDev, m_CmdFlag, m_Track[1].flags))
						printf("Illegal write mode for this drive.\n");

					if ((m_Track[0].flags & TI_TEXT) == 0 &&	/* CD-Text not yet processed */
						(m_Track[E_GrOddDevMngMaxTrack+1].flags & TI_TEXT) != 0) 
					{
						packtext(m_Tracks, m_Track);
						m_Track[0].flags |= TI_TEXT;
					}

					setleadinout(m_Tracks, m_Track);
					set_trsizes(m_PtrDev, m_Tracks, m_Track, m_BufSize );
					setpregaps(m_Tracks, m_Track);
					m_Tsize = checktsize(m_Tracks, m_Track);

					if (m_Tracks == 0 && (m_CmdFlag & F_BLANK) != 0)
						m_DskStat.ds_wrmode = WM_BLANK;
					else if (m_Tracks == 0 && (m_CmdFlag & F_FORMAT) != 0)
						m_DskStat.ds_wrmode	= WM_FORMAT;
					else
						set_wrmode(m_PtrDev, m_CmdFlag, m_Track[1].flags);

					void	*cp = NULL;

					m_PtrDev->gen_cue(m_Track, &cp, FALSE);
					if (cp)
						free(cp);

					do_leadin(m_Track);

					if ((m_CmdFlag & F_FORCE) == 0)
					{
						load_media(m_PtrScg, m_PtrDev, TRUE);
					}

					exargs.old_secsize = sense_secsize(m_PtrScg, 1);
					if (exargs.old_secsize < 0)
						exargs.old_secsize = sense_secsize(m_PtrScg, 0);

					if (read_capacity(m_PtrScg) < 0)
					{
						if (exargs.old_secsize > 0)
							m_PtrScg->cap.c_bsize = exargs.old_secsize;
					}
					if (exargs.old_secsize < 0)
						exargs.old_secsize = m_PtrScg->cap.c_bsize;

					if (exargs.old_secsize != m_PtrScg->cap.c_bsize)
					{
						DbgMsgPrint("Cls_GrOddDevMng::LcInitDev - blockdesc secsize %d differs from cap secsize %d.\n",
							exargs.old_secsize, m_PtrScg->cap.c_bsize);
					}

					if (exargs.old_secsize > 0 && exargs.old_secsize != DATA_SEC_SIZE)
					{
						set_secsize(m_PtrScg, DATA_SEC_SIZE);
					}
					check_recovery(m_PtrScg, m_PtrDev, m_CmdFlag);

					if (m_CmdFlag & F_WRITE)
					{
						m_DskStat.ds_cdrflags |= RF_WRITE;
					}

					if (m_CmdFlag & F_BLANK)
					{
						m_DskStat.ds_cdrflags |= RF_BLANK;
					}
					if (m_CmdFlag & F_PRATIP )
					{
						m_DskStat.ds_cdrflags |= RF_PRATIP;
					}

					//different dminbuf
					if (m_PtrDev->getdisktype(m_PtrScg) < 0)
					{
						printf("Cls_GrOddRec::LcInitDev - Cannot get disk type.\n");
						return FALSE;
					}

					//write mode
					if ( A_IsWrite )
					{
						if (m_DmaSpeed > 0)
						{
							if ((m_DskStat.ds_flags & DSF_BD) != 0)
								m_DmaSpeed /= 4495;
							else
								if ((m_DskStat.ds_flags & DSF_DVD) == 0)
									m_DmaSpeed /= 176;
								else
									m_DmaSpeed /= 1385;
						}

						if (m_PtrDev->init(m_PtrScg)  < 0)
						{
							printf("Cls_GrOddRec::LcInitDev - Cannot init drive.\n");
						}

						/* diffrent
						if (flags & F_MEDIAINFO)
						{
							(*dp->cdr_prdiskstatus)(scgp, dp);
							comexit(0);
						}
						if (flags & F_MSINFO) {
							print_msinfo(scgp, dp);
							comexit(0);
						}
						if (flags & F_TOC) {
							print_toc(scgp, dp);
							comexit(0);
						}*/

						if ((m_CmdFlag & F_FORMAT) || (m_DskStat.ds_flags & DSF_NEED_FORMAT))
						{
							int	omode = m_DskStat.ds_wrmode;

							m_DskStat.ds_wrmode = WM_FORMAT;

							wait_unit_ready(m_PtrScg, 120);
							if (gettimeofday(&starttime, (struct timezone *)0) < 0)
								printf("Cls_GrOddRec::LcInitDev - Cannot get start time.\n");

							if (m_PtrDev->format(m_PtrScg, 0) < 0)
							{
								printf("Cls_GrOddRec::LcInitDev - Cannot format medium.\n");
								//Tv_Result 	= FALSE;
								return FALSE;
							}

							if (gettimeofday(&fixtime, (struct timezone *)0) < 0)
								printf("Cls_GrOddRec::LcInitDev - Cannot get format time.\n");

							if (!wait_unit_ready(m_PtrScg, 240) || m_Tracks == 0)
							{
								printf("Cls_GrOddRec::LcInitDev - not wait unit ready or tracks is zero.\n");
								return FALSE;
								//Tv_Result 	= FALSE;
							}
							m_DskStat.ds_wrmode = omode;

							if (m_Tracks > 0)
							{
								int	cdrflags = m_DskStat.ds_cdrflags;

								m_DskStat.ds_cdrflags &= ~RF_PRATIP;
								if (m_PtrDev->getdisktype(m_PtrScg) < 0)
								{
									printf("Cls_GrOddRec::LcInitDev - Cannot get disk type.\n");
									if ((m_CmdFlag & F_FORCE) == 0)
										Tv_Result 	= FALSE;
								}
								m_DskStat.ds_cdrflags = cdrflags;
							}
						}

						__u32 omb = m_DskStat.ds_maxblocks;

						if (m_PtrDev->opt1(m_PtrScg) < 0)
						{
							printf("Cls_GrOddRec::LcInitDev - Cannot set up 1st set of driver options.\n");
						}
						if (m_Tsize > 0 && omb != m_DskStat.ds_maxblocks)
						{
							printf("Cls_GrOddRec::LcInitDev - Disk size changed by user options.\n");
							printf("Cls_GrOddRec::LcInitDev - Checking disk capacity according to new values.\n");
						}

						if (m_Tsize == 0)
						{
							if (m_Tracks > 0)
							{
								printf("WARNING: Total disk size unknown. Data may not fit on disk.\n");
							}
						}
						else if (m_Tracks > 0)
						{
							if (((m_CmdFlag & F_BLANK) == 0) &&	!checkdsize(m_PtrScg, m_PtrDev, m_Tsize, m_CmdFlag))
							{
								printf("Cls_GrOddRec::LcInitDev - Checksize error\n");
								return FALSE;
								//Tv_Result = FALSE;
							}
						}
						if(MT_DVD <= get_mediatype(m_PtrScg))
						{
							printf("Cls_GrOddRec::LcInitDev - different code media type\n");
						}
#if 0 //different
						/*
						 * Hack to support DVD+R/DL and the firmware problems
						 * for BD-R found in the Lite-ON BD B LH-2B1S/AL09
						 */
						if (get_mediatype(scgp) >= MT_DVD) {
							int	bls = get_blf(get_mediatype(scgp));
							long	nbs;

							bls *= 2048;			/* Count in bytes */
							nbs = bufsize / bls * bls;
							if (nbs == 0) {
								for (nbs = bls; nbs > 2048; nbs /= 2)
									if (nbs <= bufsize)
										break;
							}
							if (nbs != bufsize) {
								if (lverbose) {
									printf(
									_("Reducing transfer size from %ld to %ld bytes.\n"),
													bufsize, nbs);
								}
								bufsize = nbs;
								set_trsizes(dp, tracks, track);
							}
						}
						/*
						 * fork() here to start the extra process needed for
						 * improved buffering.
						 */
						if (!init_faio(track, bufsize))
							fs = 0L;
						else
							on_comerr(excdr, &exargs); /* Will be called first */
#endif
						if (m_PtrDev->select_speed(m_PtrScg, &m_Speed) < 0)
						{
							if ((m_CmdFlag & F_FORCE) == 0)
							{
								return FALSE;
							}
							//comexit(EX_BAD);
						}

						m_DskStat.ds_wspeed = m_Speed;
						if ((m_CmdFlag & F_WRITE) != 0 && m_RawSpeed >= 0)
						{
							int	max_raw = (m_CmdFlag & F_FORCE) != 0 ? m_RawSpeed:m_RawSpeed/2;

							//Check for Clone tracks
							for (Tv_WkCnt = 1;Tv_WkCnt <= E_GrOddDevMngMaxTrack;Tv_WkCnt++)
							{
								if ((m_Track[Tv_WkCnt].sectype & ST_MODE_RAW) != 0)
									continue;

								if ((m_Track[Tv_WkCnt].sectype & ST_MODE_MASK) == ST_MODE_AUDIO)
									continue;

								if (m_Speed > max_raw)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Processor too slow. Cannot write RAW data at speed %d.\n", m_Speed);
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Max RAW data speed on this processor is %d.\n",	max_raw);
								}
								break;
							}
						}


						if (m_Tracks > 0 && (m_CmdFlag & F_WRITE) != 0 && m_DmaSpeed > 0)
						{
							int	max_dma = (m_CmdFlag & F_FORCE) != 0 ? m_DmaSpeed:(m_DmaSpeed+1)*4/5;
							char	*p = NULL;

							if (m_Speed > max_dma)
							{
								DbgMsgPrint("Cls_GrOddRec::LcInitDev - DMA speed too slow (OK for %dx). Cannot write at speed %dx.\n",	max_dma, m_Speed);
								if ((m_DskStat.ds_cdrflags & RF_BURNFREE) == 0)
								{
									DbgMsgPrint("Max DMA data speed is %d.\n", max_dma);
									if (p == NULL || !streql(p, "any"))
										DbgMsgPrint("Cls_GrOddRec::LcInitDev - Try to use 'driveropts=burnfree'.\n");
								}
							}
						}

						if ((m_CmdFlag & (F_WRITE|F_BLANK)) != 0 &&	(m_DskStat.ds_flags & DSF_ERA) != 0)
						{
							if (m_DskStat.ds_at_max_speed > 0 && m_Speed <= 8 &&	m_Speed > (int)m_DskStat.ds_at_max_speed)
							{
								if ((m_CmdFlag & F_FORCE) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Write speed %d of medium not sufficient for this writer.\n", m_DskStat.ds_at_max_speed);
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - You may have used an ultra low speed medium on a high speed writer.\n");
								}
							}

							if ((m_DskStat.ds_flags & DSF_ULTRASPP_ERA) != 0 && (m_Speed < 16 || (m_PtrDev->GetCdrwSupport() & CDR_CDRW_ULTRAP) == 0))
							{
								if ((m_PtrDev->GetCdrwSupport() & CDR_CDRW_ULTRAP) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Trying to use ultra high speed+ medium on a writer which is not\ncompatible with ultra high speed+ media.\n");
								}
								else if ((m_CmdFlag & F_FORCE) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Probably trying to use ultra high speed+ medium on improper writer.\n");
								}
							}
							else if ((m_DskStat.ds_flags & DSF_ULTRASP_ERA) != 0 && (m_Speed < 10 || (m_PtrDev->GetCdrwSupport() & CDR_CDRW_ULTRA) == 0))
							{
								if ((m_PtrDev->GetCdrwSupport() & CDR_CDRW_ULTRA) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Trying to use ultra high speed medium on a writer which is not\ncompatible with ultra high speed media.\n");
								}
								else if ((m_CmdFlag & F_FORCE) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Probably trying to use ultra high speed medium on improper writer.\n");
								}
							}
							if (m_DskStat.ds_at_min_speed >= 4 && m_DskStat.ds_at_max_speed > 4 && m_DskStat.ds_dr_max_wspeed <= 4)
							{
								if ((m_CmdFlag & F_FORCE) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Trying to use high speed medium on low speed writer.\n");
								}
							}
							if ((int)m_DskStat.ds_at_min_speed > m_Speed)
							{
								if ((m_CmdFlag & F_FORCE) == 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Write speed %d of writer not sufficient for this medium.\n", m_Speed);
									DbgMsgPrint("You did use a %s speed medium on an improper writer or\n", m_DskStat.ds_flags & DSF_ULTRASP_ERA ?	"ultra high": "high");
									DbgMsgPrint("you used a speed=# option with a speed too low for this medium.\n");
								}
							}
						}

						if ((m_CmdFlag & (F_BLANK|F_FORCE)) == (F_BLANK|F_FORCE))
						{
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - Waiting for drive to calm down.\n");
							wait_unit_ready(m_PtrScg, 120);
							scsi_blank(m_PtrScg, 0L, m_blanktype, FALSE);
						}

						wait_unit_ready(m_PtrScg, 120);

						starttime.tv_sec = 0;
						wstarttime.tv_sec = 0;
						stoptime.tv_sec = 0;
						fixtime.tv_sec = 0;
						if (gettimeofday(&starttime, (struct timezone *)0) < 0)
							printf("Cls_GrOddRec::LcInitDev - Cannot get start time.\n");

						if (m_CmdFlag & F_BLANK)
						{
							do_opc(m_PtrScg, m_PtrDev, m_CmdFlag);
							wait_unit_ready(m_PtrScg, 120);
							if (gettimeofday(&starttime, (struct timezone *)0) < 0)
								DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot get start time.\n");

							if (m_PtrDev->blank(m_PtrScg, 0L, m_blanktype) < 0)
							{
								DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot blank disk, aborting.\n");
								if (m_blanktype != BLANK_DISC)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Some drives do not support all blank types.\n");
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Try again with cdrecord blank=all.\n");
								}
								//comexit(EX_BAD);
							}
							if (gettimeofday(&fixtime, (struct timezone *)0) < 0)
								DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot get blank time.\n");

							if (!wait_unit_ready(m_PtrScg, 240) || m_Tracks == 0)
							{
								//comexit(0);
								return FALSE;
							}

							if (m_Tracks > 0)
							{
								int	cdrflags = m_DskStat.ds_cdrflags;

								m_DskStat.ds_cdrflags &= ~RF_PRATIP;
								if (m_PtrDev->getdisktype(m_PtrScg) < 0)
								{
									DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot get disk type.\n");
									if ((m_CmdFlag & F_FORCE) == 0)
									{
										//comexit(EX_BAD);
										return 	FALSE;
									}
								}
								if (!checkdsize(m_PtrScg, m_PtrDev, m_Tsize, m_CmdFlag))
								{
									//comexit(EX_BAD);
									return FALSE;
								}

								m_DskStat.ds_cdrflags = cdrflags;
							}

							if (gettimeofday(&starttime, (struct timezone *)0) < 0)
								printf("Cls_GrOddRec::LcInitDev - Cannot get start time.\n");
						}



						if (m_Tracks == 0 && (m_CmdFlag & F_FIX) == 0)
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - No tracks found.\n");

						if (read_tochdr(m_PtrScg, m_PtrDev, NULL, &m_Trackno) < 0)
						{
							m_Trackno = 0;
						}

						if ((m_Tracks + m_Trackno) > E_GrOddDevMngMaxTrack)
						{
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - Too many tracks for this disk, last track number is %d.\n",	m_Tracks + m_Trackno);
						}

						for (Tv_WkCnt = 0;Tv_WkCnt <= m_Tracks+1;Tv_WkCnt++)
						{
							m_Track[Tv_WkCnt].trackno = Tv_WkCnt + m_Trackno;

							if ((m_PtrDev->GetFlags2() & CDR2_NOCD) && Tv_WkCnt > 0)
								m_Track[Tv_WkCnt].trackno = m_Trackno + 1;
						}

						if (m_PtrDev->opt2(m_PtrScg) < 0)
						{
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot set up 2nd set of driver options.\n");
						}

						if (m_PtrDev->open_session(m_PtrScg, m_Track) < 0)
						{
							DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot open new session.\n");
						}

						if (!do_opc(m_PtrScg, m_PtrDev, m_CmdFlag))
							return FALSE;

						if (m_PtrDev->write_leadin(m_PtrScg, m_Track) < 0)
							printf("Cls_GrOddRec::LcInitDev - Could not write Lead-in.\n");

						if (gettimeofday(&wstarttime, (struct timezone *)0) < 0)
							printf("Cls_GrOddRec::LcInitDev - Cannot get start time.\n");

						for (Tv_WkCnt = 1;Tv_WkCnt <= m_Tracks;Tv_WkCnt++)
						{
							m_StartSec = 0L;

							if (m_PtrDev->open_track(m_PtrScg, &m_Track[Tv_WkCnt]) < 0)
							{
								DbgMsgPrint("Cls_GrOddRec::LcInitDev - Cannot open next track.\n");
								//errs++;
								break;
							}

							if ((m_CmdFlag & (F_SAO|F_RAW)) == 0 ||	((m_DskStat.ds_flags & DSF_DVD) &&	(m_CmdFlag & F_MULTI) != 0))
							{
								if (m_PtrDev->next_wr_address(m_PtrScg, &m_Track[Tv_WkCnt], (long int*)&m_StartSec) < 0)
								{
									printf("Cls_GrOddRec::LcInitDev - Cannot get next writable address.\n");
									//errs++;
									break;
								}
								m_Track[Tv_WkCnt].trackstart = m_StartSec;
							}

							if (m_Track[Tv_WkCnt].trackno != m_Track[Tv_WkCnt - 1].trackno)
								printf("Cls_GrOddRec::LcInitDev - Starting new track at sector: %ld\n",	m_Track[Tv_WkCnt].trackstart);
							else
								printf("Cls_GrOddRec::LcInitDev - Continuing track at sector: %ld\n",	m_Track[Tv_WkCnt].trackstart);
						}

						m_PtrTrack 	= &m_Track[1];

						m_StartSec 	= m_PtrTrack->trackstart;
						m_ByteSpt		= m_PtrTrack->secsize * m_PtrTrack->secspt;
					}

					//success
					Tv_Result		=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrOddRec::LcInitDev - cannot get scsi io buffer\n" );
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrOddRec::LcInitDev - scg_open error\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcReleaseObj( void )
{
	if(NULL != V_PtrDevMmc)
	{
		delete(V_PtrDevMmc);
		V_PtrDevMmc = NULL;
	}
	if(NULL != V_PtrDevMmcSony)
	{
		delete(V_PtrDevMmcSony);
		V_PtrDevMmcSony = NULL;
	}
	if(NULL != V_PtrDevCd)
	{
		delete(V_PtrDevCd);
		V_PtrDevCd = NULL;
	}
	if(NULL != V_PtrDevOldCd)
	{
		delete(V_PtrDevOldCd);
		V_PtrDevOldCd = NULL;
	}
	if(NULL != V_PtrDevDvd)
	{
		delete(V_PtrDevDvd);
		V_PtrDevDvd = NULL;
	}
	if(NULL != V_PtrDevDvdPlus)
	{
		delete(V_PtrDevDvdPlus);
		V_PtrDevDvdPlus = NULL;
	}
	if(NULL != V_PtrDevDvdPlusR)
	{
		delete(V_PtrDevDvdPlusR);
		V_PtrDevDvdPlusR = NULL;
	}
	if(NULL != V_PtrDevDvdPlusRW)
	{
		delete(V_PtrDevDvdPlusRW);
		V_PtrDevDvdPlusRW = NULL;
	}
	if(NULL != V_PtrCdDvd)
	{
		delete(V_PtrCdDvd);
		V_PtrCdDvd = NULL;
	}

	if ( NULL != m_PtrDev )
	{
		//free(m_PtrDev);
		m_PtrDev		=	NULL;
	}
	if ( NULL != m_PtrScg )
	{
		delete(m_PtrScg);
		m_PtrScg		=	NULL;
	}
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcIsoBldTime( void )
{
	// local -------------------
		__u16	Tv_Year;
		__u8	Tv_Mon;
		__u8	Tv_Day;
		__u8	Tv_Hour;
		__u8	Tv_Min;
		__u8	Tv_Sec;
		__u32	Tv_Mili;
		Def_GrTime	Tv_Time;
		Def_StrNum	Tv_StrNum;
		char*	Tv_StrWk;
	// code --------------------
		Tv_Time	=	GrTimeSysTimeGet( &Tv_Mili );
		GrTimeDec( Tv_Time, &Tv_Year, &Tv_Mon, &Tv_Day, &Tv_Hour, &Tv_Min, &Tv_Sec );

		//volume descriptor time
		GrStrIntToStr( m_StrVolTime, (int)Tv_Year, 4, TRUE );
		GrStrIntToStr( Tv_StrNum, (int)Tv_Mon, 2, TRUE );
		Tv_StrWk	=	GrStrCat( m_StrVolTime, Tv_StrNum );
		GrStrIntToStr( Tv_StrWk, (int)Tv_Day, 2, TRUE );
		GrStrIntToStr( Tv_StrNum, (int)Tv_Hour, 2, TRUE );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, Tv_StrNum );
		GrStrIntToStr( Tv_StrWk, (int)Tv_Min, 2, TRUE );
		GrStrIntToStr( Tv_StrNum, (int)Tv_Sec, 2, TRUE );
		Tv_StrWk	=	GrStrCat( Tv_StrWk, Tv_StrNum );
		GrStrIntToStr( Tv_StrWk, (int)(Tv_Mili / 10), 2, TRUE );
		m_StrVolTime[16]	=	36;		//korea = 9 x 4 = 36

		//record time
		m_DirRecTime[0]		=	(__u8)(Tv_Year - 1900);
		m_DirRecTime[1]		=	Tv_Mon;
		m_DirRecTime[2]		=	Tv_Day;
		m_DirRecTime[3]		=	Tv_Hour;
		m_DirRecTime[4]		=	Tv_Min;
		m_DirRecTime[5]		=	Tv_Sec;
		m_DirRecTime[6]		=	36;		//korea = 9 x 4 = 36
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcIsoFillJolietSpace(void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		__u32	Tv_Prcs;
		__u8*	Tv_PtrTg;
	// code --------------------
		Tv_Prcs		=	0;
		Tv_PtrTg	=	(__u8*)A_PtrBuf;
		while ( A_Size > Tv_Prcs )
		{
			if ( 0 == ( 0x01 & Tv_Prcs ) )
			{
				*Tv_PtrTg	=	0;
			}
			else
			{
				*Tv_PtrTg	=	0x20;
			}
			Tv_PtrTg ++;
			Tv_Prcs ++;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoStrChr(void* A_PtrBuf, WCHAR* A_Str, BOOL A_IsTypeA,
	__u32 A_Limite, BOOL A_IsFixSize)
{
	// local -------------------
	__u32		Tv_Result;
		char*		Tv_PtrTg;
		WCHAR*	Tv_PtrSr;
		char		Tv_Val;
		__u32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	0;
		Tv_PtrSr	=	A_Str;
		Tv_PtrTg	=	(char*)A_PtrBuf;

		for ( Tv_WkIdx=0;Tv_WkIdx < A_Limite;Tv_WkIdx++ )
		{
			Tv_Val	=	0;
			//check contents finished
			if ( 0 != (*Tv_PtrSr) )
			{
				//check current char set
				if ( (L'A' <= (*Tv_PtrSr)) &&  (L'Z' >= (*Tv_PtrSr)) )
				{
					Tv_Val	=	(char)(*Tv_PtrSr);
				}
				if ( (L'a' <= (*Tv_PtrSr)) &&  (L'z' >= (*Tv_PtrSr)) )
				{
					Tv_Val	=	(char)(*Tv_PtrSr) - 0x20;
				}
				if ( (L'0' <= (*Tv_PtrSr)) &&  (L'9' >= (*Tv_PtrSr)) )
				{
					Tv_Val	=	(char)(*Tv_PtrSr);
				}
				if ( L'_' == (*Tv_PtrSr) )
				{
					Tv_Val	=	(char)(*Tv_PtrSr);
				}
				if ( L'.' == (*Tv_PtrSr) )
				{
					Tv_Val	=	(char)(*Tv_PtrSr);
				}
				if ( A_IsTypeA )
				{
					if ( 0 == Tv_Val )
					{
						Tv_Val	=	0x20;
					}
				}
				//next source
				Tv_PtrSr ++;
			}
			else
			{
				//check fix
				if ( !A_IsFixSize )
				{
					break;
				}
			}
			//patch file name mode
			if ( A_IsTypeA )
			{
				if ( 0 == Tv_Val )
				{
					Tv_Val	=	0x20;
				}
			}
			else
			{
				if ( 0 == Tv_Val )
				{
					Tv_Val	=	'_';
				}
			}
			//write
			*Tv_PtrTg	=	Tv_Val;
			//next
			Tv_PtrTg ++;
			Tv_Result ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoStrJoliet(void* A_PtrBuf, WCHAR* A_Str)
{
	// local -------------------
		__u32		Tv_Result;
		__u8*		Tv_PtrTg;
		WCHAR*	Tv_PtrSr;
	// code --------------------
		Tv_Result		=	0;
		Tv_PtrSr		=	A_Str;
		Tv_PtrTg		=	(__u8*)A_PtrBuf;
		while ( 0 != (*Tv_PtrSr) )
		{
			*Tv_PtrTg	=	(__u8)(0xFF & ((*Tv_PtrSr) >> 8));
			Tv_PtrTg ++;
			*Tv_PtrTg	=	(__u8)(0xFF & (*Tv_PtrSr));
			Tv_PtrTg ++;
			//next
			Tv_PtrSr ++;
			Tv_Result =	Tv_Result + 2;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoDirRecItm(void* A_PtrBuf, __u32 A_SctPos, __u32 A_FileSize,
	BOOL A_IsDir, WCHAR* A_StrFn, BOOL A_IsJoliet)
{
	// local -------------------
		__u32	Tv_Result;
		Ptr_GrIsoDirRecHead	Tv_PtrRec;
		__u8*	Tv_PtrTg;
		__u32	Tv_StrLen;
		Def_StrName	Tv_StrFn;
		Def_StrName	Tv_StrExt;
		Def_StrName	Tv_StrName;

	// code --------------------
		Tv_Result	=	E_GrIsoDirRecHeadSize;
		Tv_PtrRec	=	(Ptr_GrIsoDirRecHead)A_PtrBuf;

		//build header
		Tv_PtrRec->ExtDirRecLen		=	0;
		GrDumyWrite4Byte( Tv_PtrRec->LSBLoExtent, A_SctPos );
		GrDumyEndian4byteSet( Tv_PtrRec->MSBLoExtent, A_SctPos );
		GrDumyWrite4Byte( Tv_PtrRec->LSBDataLength, A_FileSize );
		GrDumyEndian4byteSet( Tv_PtrRec->MSBDataLength, A_FileSize );
		GrDumyCopyMem( Tv_PtrRec->Time, m_DirRecTime, 7 );
		Tv_PtrRec->Flags	=	0x00;
		if ( A_IsDir )
		{
			Tv_PtrRec->Flags	=	0x02;
		}
		Tv_PtrRec->UnitSize		=	0;
		Tv_PtrRec->GapSize		=	0;
		GrDumyWrite2Byte( Tv_PtrRec->LSBVolSeqNum, 0x01 );
		GrDumyEndian2byteSet( Tv_PtrRec->MSBVolSeqNum, 0x01 );

		//check parent
		Tv_PtrTg	=	(__u8*)((__u32)A_PtrBuf + E_GrIsoDirRecHeadSize);
		if ( NULL == A_StrFn )
		{
			*Tv_PtrTg	=	0;
			Tv_StrLen	=	1;
		}
		else
		{
			//write text
			if ( A_IsJoliet )
			{
				Tv_StrLen	=	LcIsoStrJoliet( Tv_PtrTg , A_StrFn );
			}
			else
			{
				//get file name
				GrStrWstrToStr( Tv_StrFn, A_StrFn );
				GrStrFnGetNameOnly( Tv_StrFn, Tv_StrName );
				GrStrFnGetExt( Tv_StrFn, Tv_StrExt );
				GrStrCopy( Tv_StrFn, Tv_StrName, 8 );
				GrStrCat( Tv_StrFn, Tv_StrExt );
				Tv_StrLen	=	GrStrLen( Tv_StrFn );
				GrStrCopy( (char*)Tv_PtrTg, Tv_StrFn );
				/*
				Tv_StrLen	=	LcIsoStrChr( Tv_PtrTg , A_StrFn, FALSE, 12, FALSE );
				Tv_PtrTg	=	(__u8*)( (__u32)Tv_PtrTg + Tv_StrLen );
				Tv_PtrTg[0]	=	';';
				Tv_PtrTg[1]	=	'1';
				Tv_StrLen		=	Tv_StrLen + 2;
				*/
			}
		}
		Tv_Result	=	Tv_Result +	Tv_StrLen;
		Tv_PtrRec->LenFileIdt	=	(__u8)Tv_StrLen;

		//patch odd size
		if ( 0 != (1 & Tv_Result) )
		{
			Tv_PtrTg	=	(__u8*)((__u32)A_PtrBuf + Tv_Result);
			*Tv_PtrTg	=	0;
			Tv_Result ++;
		}

		//update size
		Tv_PtrRec->DirRecLen	=	(__u8)Tv_Result;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoDirRecTbl(void* A_PtrBuf, BOOL A_IsSpl)
{
	// local -------------------
	__u32	Tv_Result;
	void*	Tv_PtrRec;
	__u32	Tv_FileIdx;
	__u32	Tv_ItemSize;
	__u32	Tv_SubCnt;
	__u32	Tv_RootSctPos;
	// code --------------------
	Tv_Result	=	(E_GrIsoDirRecRootSize * 2);		//root direct size

	//file start position = 2
	//files
	Tv_SubCnt	=	0;
	for ( Tv_FileIdx=0;Tv_FileIdx < m_FileCnt;Tv_FileIdx++ )
	{
		//get address
		Tv_PtrRec	=	(void*)((__u32)A_PtrBuf + Tv_Result);
		//write data
		Tv_ItemSize	=	LcIsoDirRecItm( Tv_PtrRec, m_FileTbl[Tv_FileIdx].SctPos, 
			m_FileTbl[Tv_FileIdx].Size, FALSE, m_FileTbl[Tv_FileIdx].StrFn, A_IsSpl );
		Tv_Result	=	Tv_Result + Tv_ItemSize;
		Tv_SubCnt ++;
		if ( 8 <= Tv_SubCnt )
		{
			Tv_SubCnt	=	0;
			Tv_Result	=	(Tv_Result + 0x7FF) & 0xFFFFF800;		//realign
		}
	}

	//root directory
	Tv_RootSctPos	=	E_GrIsoPosDirRecPri;
	if ( A_IsSpl )
	{
		Tv_RootSctPos	=	E_GrIsoPosDirRecSpl;
	}
	//check size
	if ( E_GrIsoSectorSize > Tv_Result )
	{
		Tv_Result	=	E_GrIsoSectorSize;
	}
	//my directory
	LcIsoDirRecItm( A_PtrBuf, Tv_RootSctPos, Tv_Result, TRUE, NULL, A_IsSpl );
	//parent directory
	Tv_PtrRec	=	(void*)((__u32)A_PtrBuf + E_GrIsoDirRecRootSize);
	LcIsoDirRecItm( Tv_PtrRec, Tv_RootSctPos, Tv_Result, TRUE, NULL, A_IsSpl );

	return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcIsoMakeVolDes(void* A_PtrBuf, __u8 A_Type, WCHAR* A_StrVolume)
{
	// local -------------------
		Ptr_GrIsoVolDes	Tv_PtrVd;
		Def_WstrName Tv_Str;
	// code --------------------
		Tv_PtrVd	=	(Ptr_GrIsoVolDes)A_PtrBuf;

		Tv_PtrVd->VolType		=	A_Type;
		Tv_PtrVd->StdIdt[0]	=	'C';
		Tv_PtrVd->StdIdt[1]	=	'D';
		Tv_PtrVd->StdIdt[2]	=	'0';
		Tv_PtrVd->StdIdt[3]	=	'0';
		Tv_PtrVd->StdIdt[4]	=	'1';
		Tv_PtrVd->VolVer		=	1;
		if ( E_GrIsoVoldscTypeSetTerminator != A_Type )
		{
			Tv_PtrVd->VolFlags	=	0;
			if ( E_GrIsoVoldscTypePrimary ==  A_Type )
			{
				GrStrStrToWstr(Tv_Str, "GAUSE RESERCH");
				LcIsoStrChr( Tv_PtrVd->SysIdt, Tv_Str, TRUE, 32, TRUE );
				//volume identifier
				LcIsoStrChr( Tv_PtrVd->VolIdt, A_StrVolume, TRUE, 32, TRUE );
			}
			else
			{
				//system identifier
				GrStrStrToWstr(Tv_Str, "GAUSE RESERCH   ");
				LcIsoStrJoliet( Tv_PtrVd->SysIdt, Tv_Str);
				//LcIsoStrJoliet( Tv_PtrVd->SysIdt, L"GAUSE RESERCH   ");
				//volume identifier
				LcIsoFillJolietSpace( Tv_PtrVd->VolIdt, 32 );
				LcIsoStrJoliet( Tv_PtrVd->VolIdt, A_StrVolume );
			}
			//volume size
			GrDumyWrite4Byte( Tv_PtrVd->LSBVolSize, m_IsoTtSct );
			GrDumyEndian4byteSet( Tv_PtrVd->MSBVolSize, m_IsoTtSct );

			//escape code
			if ( E_GrIsoVoldscTypeSupplementary == A_Type )
			{
				Tv_PtrVd->EscSeq[0]		=	0x25;
				Tv_PtrVd->EscSeq[1]		=	0x2F;
				Tv_PtrVd->EscSeq[2]		=	0x45;
			}

			//volume set size
			GrDumyWrite2Byte( Tv_PtrVd->LSBVolSetSize, 0x01 );
			GrDumyEndian2byteSet( Tv_PtrVd->MSBVolSetSize, 0x01 );
			//volume sequence number
			GrDumyWrite2Byte( Tv_PtrVd->LSBVolSeqNum, 0x01 );
			GrDumyEndian2byteSet( Tv_PtrVd->MSBVolSeqNum, 0x01 );
			//logical block size
			GrDumyWrite2Byte( Tv_PtrVd->LSBLgcBlkSize, E_GrIsoSectorSize );
			GrDumyEndian2byteSet( Tv_PtrVd->MSBLgcBlkSize, E_GrIsoSectorSize );
			//path table size
			GrDumyWrite4Byte( Tv_PtrVd->LSBPathTblSize, E_GrIsoPathRootSize );
			GrDumyEndian4byteSet( Tv_PtrVd->MSBPathTblSize, E_GrIsoPathRootSize );
			//path table position
			if ( E_GrIsoVoldscTypePrimary == A_Type )
			{
				GrDumyWrite4Byte( Tv_PtrVd->OcurLPath, E_GrIsoPosLpathPri );
				GrDumyEndian4byteSet( Tv_PtrVd->OcurMPath, E_GrIsoPosMpathPri );
			}
			else if ( E_GrIsoVoldscTypeSupplementary == A_Type )
			{
				GrDumyWrite4Byte( Tv_PtrVd->OcurLPath, E_GrIsoPosLpathSpl );
				GrDumyEndian4byteSet( Tv_PtrVd->OcurMPath, E_GrIsoPosMpathSpl );
			}
			//optialnal path table
			GrDumyWrite4Byte( Tv_PtrVd->OptLPath, 0 );
			GrDumyEndian4byteSet( Tv_PtrVd->OptMPath, 0 );

			//directory record
			if ( E_GrIsoVoldscTypePrimary == A_Type )
			{
				LcIsoDirRecItm( Tv_PtrVd->DirRec, E_GrIsoPosDirRecPri, E_GrIsoSectorSize, TRUE, NULL, FALSE );
			}
			else if ( E_GrIsoVoldscTypeSupplementary == A_Type )
			{
				LcIsoDirRecItm( Tv_PtrVd->DirRec, E_GrIsoPosDirRecSpl, E_GrIsoSectorSize, TRUE, NULL, FALSE );
			}

			if ( E_GrIsoVoldscTypePrimary ==  A_Type )
			{
				GrDumyFillB( Tv_PtrVd->VolSetIdt, 128, 0x20 );
				GrDumyFillB( Tv_PtrVd->PubIdt, 128, 0x20 );
				GrDumyFillB( Tv_PtrVd->DataPreIdt, 128, 0x20 );
				GrDumyFillB( Tv_PtrVd->AppIdt, 128, 0x20 );
				GrDumyFillB( Tv_PtrVd->CopyFileIdt, 37, 0x20 );
				GrDumyFillB( Tv_PtrVd->AbsFileIdt, 37, 0x20 );
				GrDumyFillB( Tv_PtrVd->BibFileIdt, 37, 0x20 );
			}
			else
			{
				LcIsoFillJolietSpace( Tv_PtrVd->VolSetIdt, 128 );
				LcIsoFillJolietSpace( Tv_PtrVd->PubIdt, 128 );
				LcIsoFillJolietSpace( Tv_PtrVd->DataPreIdt, 128 );
				LcIsoFillJolietSpace( Tv_PtrVd->AppIdt, 128 );
				LcIsoFillJolietSpace( Tv_PtrVd->CopyFileIdt, 37 );
				LcIsoFillJolietSpace( Tv_PtrVd->AbsFileIdt, 37 );
				LcIsoFillJolietSpace( Tv_PtrVd->BibFileIdt, 37 );
			}

			GrDumyCopyMem( Tv_PtrVd->VolCreateTime, m_StrVolTime, 17 );
			GrDumyCopyMem( Tv_PtrVd->VolModiTime, m_StrVolTime, 17 );
			GrDumyFillB( Tv_PtrVd->VolExpTime, 16, '0' );
			GrDumyFillB( Tv_PtrVd->VolEffTime, 16, '0' );
			Tv_PtrVd->FileVer		=	0x01;
			Tv_PtrVd->Reserved1	=	0;
			GrDumyFillB( Tv_PtrVd->AppUse, 512, 0x20 );
			GrDumyZeroMem( Tv_PtrVd->Reserved2, 653 );

		}
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoLpathItm(void* A_PtrBuf, __u32 A_DirSctPos)
{
	// local -------------------
		Ptr_GrIsoPathHead	Tv_PtrPath;
	// code --------------------
		Tv_PtrPath	=	(Ptr_GrIsoPathHead)A_PtrBuf;

		Tv_PtrPath->LenDirIdt			=	1;
		Tv_PtrPath->ExtAttRecLen	=	0;
		GrDumyWrite4Byte( Tv_PtrPath->LocatExt, A_DirSctPos );
		GrDumyWrite2Byte( Tv_PtrPath->ParentDirNum, 1 );

		return	E_GrIsoPathRootSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::LcIsoMpathItm(void* A_PtrBuf, __u32 A_DirSctPos)
{
	// local -------------------
		Ptr_GrIsoPathHead	Tv_PtrPath;
	// code --------------------
		Tv_PtrPath	=	(Ptr_GrIsoPathHead)A_PtrBuf;

		Tv_PtrPath->LenDirIdt			=	1;
		Tv_PtrPath->ExtAttRecLen	=	0;
		GrDumyEndian4byteSet( Tv_PtrPath->LocatExt, A_DirSctPos );
		GrDumyEndian2byteSet( Tv_PtrPath->ParentDirNum, 1 );

		return	E_GrIsoPathRootSize;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcHeadWriteGap( void )
{
	// local -------------------
		int	Tv_TrySize;
		int	Tv_TrySctCnt;
		int	Tv_Writed;
		__u32		Tv_WriteSctCnt;
		BOOL		Tv_Result;
	// code --------------------
		Tv_Result		= TRUE;

		Tv_TrySctCnt	=	m_HeadWtSctResid;
		if (m_BufSctCnt < (__u32)Tv_TrySctCnt)
		{
			Tv_TrySctCnt	=	m_BufSctCnt;
		}

		Tv_TrySize	=	( Tv_TrySctCnt << E_GrIsoSectorShift );
#ifdef LINUX_APP
		Tv_Writed		= write_secs( m_PtrScg, m_PtrDev, m_PtrBuf, (int)m_PrgSctPos, Tv_TrySize, Tv_TrySctCnt, FALSE );
#else
		__u32	Tv_FcPos	=	(__u32)m_IsoFc->GetPos();

		if ( (m_PrgSctPos << E_GrIsoSectorShift) != Tv_FcPos )
		{
			DbgMsgPrint( "ODD sect write position mismatch - sct = %8X fcpos= %8X \n", m_PrgSctPos, Tv_FcPos );
		}
		Tv_Writed		=	m_IsoFc->Write( m_PtrBuf, Tv_TrySize );
#endif
		//update
		if ( 0 > Tv_Writed )
		{
			DbgMsgPrint( "Cls_GrOddRec::LcHeadStageGap - sector write error\n" );
			Tv_Result		= FALSE;
		}
		else
		{
			Tv_WriteSctCnt		=	(__u32)Tv_Writed >> E_GrIsoSectorShift;
			m_PrgSctPos				=	m_PrgSctPos + Tv_WriteSctCnt;
			m_HeadWtSctResid	=	m_HeadWtSctResid - Tv_WriteSctCnt;

			if ( 0 == m_HeadWtSctResid )
			{
				m_HeadStage ++;		//go next stage
			}
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::LcHeadBuildImg( void )
{
	// local -------------------
		void*		Tv_PtrBuf;
	// code --------------------
		LcIsoBldTime();

		GrDumyZeroMem( m_ImgHdBuf, sizeof(m_ImgHdBuf) );

		//build primary directory record table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosDirRecPri - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		m_RecTblPriSize	=	LcIsoDirRecTbl( Tv_PtrBuf, FALSE );
		//build supplementary directory record table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosDirRecSpl - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		m_RecTblSplSize	=	LcIsoDirRecTbl( Tv_PtrBuf, TRUE );

		//pri Lpath table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosLpathPri - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoLpathItm( Tv_PtrBuf, E_GrIsoPosDirRecPri );
		//pri Mpath table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosMpathPri - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoMpathItm( Tv_PtrBuf, E_GrIsoPosDirRecPri );

		//spl Lpath table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosLpathSpl - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoLpathItm( Tv_PtrBuf, E_GrIsoPosDirRecSpl );
		//spl Mpath table
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosMpathSpl - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoMpathItm( Tv_PtrBuf, E_GrIsoPosDirRecSpl );

		//build PVD
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosPvd - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoMakeVolDes( Tv_PtrBuf, E_GrIsoVoldscTypePrimary, m_StrVolName );
		//build SVD
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosSvd - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoMakeVolDes( Tv_PtrBuf, E_GrIsoVoldscTypeSupplementary, m_StrVolName );
		//build VDT
		Tv_PtrBuf	=	(void*)((__u32)m_ImgHdBuf + ((E_GrIsoPosVdt - E_GrIsoPosPvd) << E_GrIsoSectorShift));
		LcIsoMakeVolDes( Tv_PtrBuf, E_GrIsoVoldscTypeSetTerminator, NULL );

		//build head size
		m_HeadWtSctResid	=	E_GrIsoPosFileData - E_GrIsoPosPvd;
		m_HeadWtSize			=	0;

		//go next
		m_HeadStage ++;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcHeadWriteImg( void )
{
	// local -------------------
	__u32		Tv_TrySize;
	__u32		Tv_WriteSctCnt;
		void*		Tv_PtrBuf;
		BOOL		Tv_Result;
	// code --------------------
		Tv_Result		= TRUE;
		Tv_PtrBuf		=	(void*)((__u32)m_ImgHdBuf + m_HeadWtSize);
		Tv_TrySize	=	( m_HeadWtSctResid << E_GrIsoSectorShift );

		if ( Tv_TrySize > m_BufAbleSize )
		{
			Tv_TrySize	=	m_BufAbleSize;
		}

		if ( LcBufWriteData( Tv_PtrBuf, Tv_TrySize ) )
		{
			m_HeadWtSize			=	m_HeadWtSize + Tv_TrySize;
			Tv_WriteSctCnt		=	Tv_TrySize >> E_GrIsoSectorShift;
			//m_PrgSctPos				=	m_PrgSctPos + Tv_WriteSctCnt;
			m_HeadWtSctResid	=	m_HeadWtSctResid - Tv_WriteSctCnt;
			if ( 0 == m_HeadWtSctResid )
			{
				m_Stage = E_GrOddStageFileWrite;		//head stage finished, go next
			}
		}
		else
		{
			Tv_Result		= FALSE;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcBufWriteData(void* A_PtrData, __u32 A_Size)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_Prgs;
		__u32	Tv_CpSize;
		__u32	Tv_FreeSize;
		void*	Tv_PtrSr;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( 0 != A_Size )
		{
			Tv_Result	=	TRUE;
			Tv_Prgs	=	0;
			while ( Tv_Prgs < A_Size )
			{
				Tv_FreeSize	=	m_BufAbleSize - m_BufInSize;
				if ( 0 != Tv_FreeSize )
				{
					//try copy data
					Tv_CpSize	=	A_Size - Tv_Prgs;
					if ( Tv_FreeSize < Tv_CpSize )
					{
						Tv_CpSize	=	Tv_FreeSize;
					}
					Tv_PtrSr	=	(void*)((__u32)A_PtrData + Tv_Prgs);
					Tv_PtrTg	=	(void*)((__u32)m_PtrBuf + m_BufInSize);
					GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, Tv_CpSize );
					Tv_Prgs		=	Tv_Prgs + Tv_CpSize;
					m_BufInSize	=	m_BufInSize + Tv_CpSize;
				}
				//check need flush
				if ( m_BufInSize >= m_BufAbleSize )
				{
					if ( !LcBufFlush() )
					{
						DbgMsgPrint("Cls_GrOddDevMng::LcBufWriteData() - Error\n");
						//flushing error, quit
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcBufWriteZero(__u32 A_Size)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_Prgs;
		__u32	Tv_CpSize;
		__u32	Tv_FreeSize;
		void*	Tv_PtrTg;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( 0 != A_Size )
		{
			Tv_Result	=	TRUE;
			Tv_Prgs	=	0;
			while ( Tv_Prgs < A_Size )
			{
				Tv_FreeSize	=	m_BufAbleSize - m_BufInSize;
				if ( 0 != Tv_FreeSize )
				{
					//try copy data
					Tv_CpSize	=	A_Size - Tv_Prgs;
					if ( Tv_FreeSize < Tv_CpSize )
					{
						Tv_CpSize	=	Tv_FreeSize;
					}
					Tv_PtrTg	=	(void*)((__u32)m_PtrBuf + m_BufInSize);
					GrDumyZeroMem( Tv_PtrTg, Tv_CpSize );
					Tv_Prgs		=	Tv_Prgs + Tv_CpSize;
					m_BufInSize	=	m_BufInSize + Tv_CpSize;
				}
				//check need flush
				if ( m_BufInSize >= m_BufAbleSize )
				{
					if ( !LcBufFlush() )
					{
						DbgMsgPrint("Cls_GrOddDevMng::LcBufWriteZero() - Error\n");
						//flushing error, quit
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcBufFlush(void)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_Prgs;
		void*	Tv_PtrBuf;
		int		Tv_TrySize;
		int		Tv_Writed;
	// code --------------------
		Tv_Result		=	FALSE;

		//check buffer in size
		if ( 0 != ( E_GrIsoSectorModMask & m_BufInSize ) )
		{
			DbgMsgPrint( "Cls_GrOddRec::LcBufFlush - not aligned data size - %8X.\n", m_BufInSize );
		}
		else
		{
			Tv_Prgs	=	0;
			while ( Tv_Prgs < m_BufInSize )
			{
				Tv_TrySize	=	(int)( m_BufInSize - Tv_Prgs );
				Tv_PtrBuf		=	(void*)((__u32)m_PtrBuf + Tv_Prgs);
#ifdef LINUX_APP
				Tv_Writed		= write_secs( m_PtrScg, m_PtrDev, (char*)Tv_PtrBuf, (int)m_PrgSctPos, 
					Tv_TrySize, (Tv_TrySize >> E_GrIsoSectorShift), FALSE );
#else
				__u32	Tv_FcPos	=	(__u32)m_IsoFc->GetPos();
				if ( (m_PrgSctPos << E_GrIsoSectorShift) != Tv_FcPos )
				{
					DbgMsgPrint( "ODD sect write position mismatch - sct = %8X fcpos= %8X \n", m_PrgSctPos, Tv_FcPos );
				}
				Tv_Writed		=	m_IsoFc->Write( Tv_PtrBuf, Tv_TrySize );
#endif
				if ( 0 > Tv_Writed )
				{
					DbgMsgPrint( "Cls_GrOddRec::LcBufFlush - write error - sct=%X size=%X\n", m_PrgSctPos, Tv_TrySize );

					m_Stage		=	E_GrOddStageError;
					m_IsError = TRUE;
					break;
				}
				Tv_Prgs	=	Tv_Prgs + (__u32)Tv_Writed;
				m_PrgSctPos	=	m_PrgSctPos + ((__u32)Tv_Writed >> E_GrIsoSectorShift);
				if ( Tv_Prgs < m_BufInSize )
				{
					GrPrcSleep( E_GrOddRecFlushWaitMs );
				}
			}
			//reset buffer
			m_BufInSize	=	0;
			if(!m_IsError)
			{
				Tv_Result		=	TRUE;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::LcPatchWriteMode(void)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		//check rw
		if ( 0 != (DSF_ERA & m_DskStat.ds_flags) )
		{
			//check need format
			if ( 0 != (DSF_NEED_FORMAT & m_DskStat.ds_flags) )
			{
				//m_CmdFlag	=	m_CmdFlag | F_FORMAT;
				Tv_Result	=	FALSE;
			}
			//check data exist
			if ( (0 != m_DskStat.ds_sessstat) || (0 != m_DskStat.ds_diskstat) )
			{
				//m_CmdFlag	=	m_CmdFlag | F_FORMAT;
				Tv_Result	=	FALSE;
			}
		}
		else
		{
			//check already writed media
			if ( (0 != m_DskStat.ds_sessstat) || (0 != m_DskStat.ds_diskstat) )
			{
				Tv_Result	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecFileRegist(char* A_StrFn, __u32 A_Size)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_AllocSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrOddStageInit == m_Stage )
		{
			//check file count
			if ( E_GrIsoFileMaxCnt > m_FileCnt )
			{
				Tv_AllocSize	=	A_Size & (~E_GrIsoSectorModMask);
				if ( 0 != ( E_GrIsoSectorModMask & A_Size) )
				{
					Tv_AllocSize	=	Tv_AllocSize + E_GrIsoSectorSize;
				}
				m_FileTbl[m_FileCnt].Allocated	=	Tv_AllocSize;
				m_FileTbl[m_FileCnt].SctPos			=	m_IsoTtSct;
				m_FileTbl[m_FileCnt].Size				=	A_Size;
				m_FileTbl[m_FileCnt].Prgs				=	0;
				GrStrStrToWstr(m_FileTbl[m_FileCnt].StrFn, A_StrFn);

				m_IsoTtSize	=	m_IsoTtSize + (__u64)Tv_AllocSize;
				m_IsoTtSct	=	m_IsoTtSct + (Tv_AllocSize >> E_GrIsoSectorShift);

				m_FileCnt ++;

				//success
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecPrepare(char* A_StrVol)
{
	// local -------------------
		BOOL	Tv_Result;
		__u64	Tv_IsoTtSize;
	// code --------------------
		Tv_Result	=	FALSE;
		m_IsError	= FALSE;

		if ( E_GrOddStageInit == m_Stage )
		{
			//check already exist
			if ( NULL == m_PtrScg )
			{
				//prepare 
				//GrStrWcopy( m_StrVolName, A_StrVol );
				GrStrStrToWstr(m_StrVolName, A_StrVol);
				//get iso size
				Tv_IsoTtSize	=	m_IsoTtSize + (__u64)(E_GrIsoLastGapSctCnt << E_GrIsoSectorShift);
#ifdef LINUX_APP
				if ( LcInitDev( Tv_IsoTtSize, TRUE ) )
				{
					if ( LcPatchWriteMode() )
					{
						//get buffer sector count
						m_BufSctCnt	=	m_BufSize >> E_GrIsoSectorShift;
						if ( 0 != (E_GrIsoSectorModMask & m_BufSize) )
						{
							DbgMsgPrint("Cls_GrOddRec::RecPrepare - buffer size not aligned! - %8X\n", m_BufSize );
						}
						m_BufAbleSize	=	m_BufSctCnt << E_GrIsoSectorShift;
						m_BufAbleSize	=	0x2000;
						DbgMsgPrint("Cls_GrOddRec::RecPrepare - MAX DMA size fix - %8X !\n", m_BufAbleSize );

						DbgMsgPrint("Cls_GrOddRec::RecPrepare - start sec = %d, \n", m_StartSec );
						//init
						m_PrgSctPos				=	m_StartSec;			//update start sector
						m_IsoTtSize				=	Tv_IsoTtSize;
						m_IsoTtSct				=	(__u32)(Tv_IsoTtSize >> E_GrIsoSectorShift);
						m_HeadStage				=	E_GrOddHeadStgGap;
						m_HeadWtSctResid	=	E_GrIsoPosPvd;
						//prepare buffer
						GrDumyZeroMem( m_PtrBuf, m_BufAbleSize );	//prepare head gap
						//prepare ready
						//m_Stage ++;
						m_Stage	=	E_GrOddStageHeadWrite;
						//success
						Tv_Result	=	TRUE;
					}
				}
#else
				//get buffer sector count
				m_BufSize		=	0xFC00;
				m_StartSec	=	0;

				//sec
				m_BufSctCnt	=	m_BufSize >> E_GrIsoSectorShift;
				if ( 0 != (E_GrIsoSectorModMask & m_BufSize) )
				{
					DbgMsgPrint("Cls_GrOddRec::RecPrepare - buffer size not aligned! - %8X\n", m_BufSize );
				}
				if ( 0xD000 > m_BufSize )
				{
					DbgMsgPrint("Cls_GrOddRec::RecPrepare - buffer size small! - %8X\n", m_BufSize );
					m_Stage		=	E_GrOddStageTailEnd;		//buffer small , quit
				}
				m_BufAbleSize	=	m_BufSctCnt << E_GrIsoSectorShift;
				//init
				m_PrgSctPos				=	m_StartSec;			//update start sector
				m_IsoTtSize				=	Tv_IsoTtSize;
				m_IsoTtSct				=	(__u32)(Tv_IsoTtSize >> E_GrIsoSectorShift);
				m_HeadStage				=	E_GrOddHeadStgGap;
				m_HeadWtSctResid	=	E_GrIsoPosPvd;
				//prepare buffer
				GrDumyZeroMem( m_PtrBuf, m_BufAbleSize );	//prepare head gap
				//prepare ready
				//m_Stage ++;
				m_Stage	=	E_GrOddStageHeadWrite;
				//success
				Tv_Result	=	TRUE;
#endif
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecHeadWrite(void)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrOddStageHeadWrite == m_Stage )
		{
			switch( m_HeadStage )
			{
				case	E_GrOddHeadStgGap:
					Tv_Result = LcHeadWriteGap();
					break;
				case	E_GrOddHeadStgImgBuild:
					LcHeadBuildImg();
					Tv_Result		= TRUE;
					break;
				case	E_GrOddHeadStgWrite:
					Tv_Result = LcHeadWriteImg();
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecFileWrite(void* A_PtrBuf, __u32 A_Size, __u8* A_PtrErr)
{
	// local -------------------
		__u32	Tv_Size;
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(E_GrOddStageFileWrite == m_Stage)
		{
			//check file progress
			if ( m_FileCnt > m_FilePrgIdx )
			{
				//check write able
				if ( m_FileTbl[m_FilePrgIdx].Size >= (m_FileTbl[m_FilePrgIdx].Prgs + A_Size) )
				{
					//write
					if ( LcBufWriteData( A_PtrBuf, A_Size ) )
					{
						m_FileTbl[m_FilePrgIdx].Prgs	=	m_FileTbl[m_FilePrgIdx].Prgs + A_Size;
						Tv_Result	=	TRUE;
					}
					else
					{
						DbgMsgPrint("Cls_GrOddDevMng::RecFileWrite() - write error\n");
						*A_PtrErr = E_GrOddDevMngErrWriteErr;
					}
				}
				else
				{
					Tv_Size	= m_FileTbl[m_FilePrgIdx].Size - m_FileTbl[m_FilePrgIdx].Prgs;

					if(LcBufWriteZero(Tv_Size))
					{
						m_FileTbl[m_FilePrgIdx].Prgs	= m_FileTbl[m_FilePrgIdx].Prgs + Tv_Size;
						
					}
					*A_PtrErr = E_GrOddDevMngErrWriteOver;
					Tv_Result	= FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecFileZeroWrite(__u32 A_Size)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	LcBufWriteZero( A_Size );
		//check file progress
		if ( m_FileCnt > m_FilePrgIdx )
		{
			// add progress
			m_FileTbl[m_FilePrgIdx].Prgs	=	m_FileTbl[m_FilePrgIdx].Prgs + A_Size;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecFileClose(void)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_ResidSize;
	// code --------------------
		Tv_Result	=	FALSE;
		if(E_GrOddStageFileWrite == m_Stage)
		{
			//check file progress
			if ( m_FileCnt > m_FilePrgIdx )
			{
				//check resid file
				Tv_ResidSize	=	m_FileTbl[m_FilePrgIdx].Allocated - m_FileTbl[m_FilePrgIdx].Prgs;
				if ( 0 != Tv_ResidSize )
				{
					LcBufWriteZero( Tv_ResidSize );
				}

				//next file 
				m_FilePrgIdx ++;
				if ( m_FileCnt <= m_FilePrgIdx )
				{
					//go tail stage
					m_Stage = E_GrOddStageTailWrite;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecTailWrite(void)
{
	// local -------------------
		BOOL	Tv_Result;
		__u32	Tv_FreeSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrOddStageTailWrite == m_Stage )
		{
			//get buffer free size
			Tv_FreeSize	=	m_BufAbleSize -	m_BufInSize;
			if ( 0 != Tv_FreeSize )
			{
				if ( Tv_FreeSize > m_TailGapResid )
				{
					Tv_FreeSize	=	m_TailGapResid;
				}
				LcBufWriteZero( Tv_FreeSize );
				m_TailGapResid	=	m_TailGapResid - Tv_FreeSize;
			}
			//buffer flush
			if ( 0 != m_BufInSize )
			{
				LcBufFlush();
			}
			//check finished
			if ( 0 == m_TailGapResid )
			{
				m_Stage = E_GrOddStageTailFinish;
			}
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::RecFinish(BOOL A_IsFixate)
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrOddStageTailFinish == m_Stage )
		{
#ifdef LINUX_APP
			if ( A_IsFixate )
			{
				DbgMsgPrint( "Cls_GrOddRec::RecFinish - Fixate..\n" );
				m_PtrDev->fixate(m_PtrScg, m_Track);
			}
			//eject
			DbgMsgPrint( "Cls_GrOddRec::RecFinish - Eject\n" );
			unload_media(m_PtrScg, m_PtrDev, F_EJECT);
#else			
			if ( NULL != m_IsoFc )
			{
				delete	m_IsoFc;
				m_IsoFc	=	NULL;
				if(NULL != m_PtrBuf)
				{
					free( m_PtrBuf );
					m_PtrBuf	=	NULL;
				}
			}
#endif
			//finish
			Tv_Result	=	TRUE;
			m_Stage = E_GrOddStageTailEnd;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::RecFileInfoGet(__u32* A_PtrRtIdx, __u32* A_PtrRtResid)
{
	// local -------------------
	__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if(E_GrOddStageFileWrite == m_Stage)
		{
			if ( m_FileCnt > m_FilePrgIdx )
			{
				*A_PtrRtIdx		=	m_FilePrgIdx;
				*A_PtrRtResid	=	m_FileTbl[m_FilePrgIdx].Size - m_FileTbl[m_FilePrgIdx].Prgs;
				Tv_Result			=	m_FileTbl[m_FilePrgIdx].Size;
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMng::RecStageGet(void)
{
		return	m_Stage;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::Eject(BOOL A_IsOpen)
{
		if((NULL != m_PtrScg) && (NULL != m_PtrDev))
		{
			if(A_IsOpen)
			{
				load_media(m_PtrScg, m_PtrDev, TRUE);
			}
			else
			{
				unload_media(m_PtrScg, m_PtrDev, F_EJECT);
			}
		}
}
//--------------------------------------------------------------------
BOOL 	Cls_GrOddDevMng::IsExistCdr()
{
	// var --------------------
	BOOL		Tv_Result;
	// code -------------------
		Tv_Result 	= FALSE;

		Tv_Result = is_cddrive(m_PtrScg);

		return Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrOddDevMng::MediaSizeGet( char* A_StrDev )
{
	// local -------------------
		__u64			Tv_Result;
		dstat_t*	Tv_PtrDstat;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL == m_PtrScg )
		{
			LcInitVar( A_StrDev );

			if ( LcInitDev( 0x10000, FALSE ) )
			{
				Tv_PtrDstat  = m_PtrDev->GetDstat();
				Tv_Result	=	(__u64)Tv_PtrDstat->ds_maxblocks << 11;		//sector size

				Tv_Result	=	Tv_Result - 0x200000;		//basic header size
			}
			if ( NULL != m_PtrScg )
			{
				unload_media(m_PtrScg, m_PtrDev, 0 );
			}
			LcReleaseObj();
		}
#ifndef LINUX_APP
		Tv_Result = (1024 * 1024) * 1024;
#endif
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrOddDevMng::SetStage(__u32 A_Stage)
{
		m_Stage		= A_Stage;
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMng::IsError()
{
		return m_IsError;
}
//--------------------------------------------------------------------
