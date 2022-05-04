/*
	cdrecord class
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrOddRec
#define	_EPC_GrOddRec

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef LINUX_APP
	#include 	<fcntl.h>
	#include 	<sys/ioctl.h>
	#include	<scsi/sg.h>
	#include 	<scsi/scsi.h>
#else
	#include <GrFileCtrl.h>
	#include <GrFileTool.h>
#endif

#include	<Hw/Odd/GrSCSI.h>
#include	<Hw/Odd/GrIso9660.h>


//====================================================================
//constance
#define E_GrOddRecMaxFileCnt		8
#define E_GrOddImgHdBufSize			0x0D000

#define	E_GrOddRecMaxTrack			99	/* Red Book track limit */

#define	TOC_DA		0	/* CD-DA				*/
#define	TOC_ROM		1	/* CD-ROM				*/
#define	TOC_XA1		2	/* CD_ROM XA with first track in mode 1 */
#define	TOC_XA2		3	/* CD_ROM XA with first track in mode 2 */
#define	TOC_CDI		4	/* CDI					*/

/*
 * Definitions for read TOC/PMA/ATIP command
 */
#define	FMT_TOC		0
#define	FMT_SINFO	1
#define	FMT_FULLTOC	2
#define	FMT_PMA		3
#define	FMT_ATIP	4
#define	FMT_CDTEXT	5

#define	TOC_MASK	7	/* Mask needed for toctname[]		*/

#define	MT_NONE		0	/* Unknown or unsupported	*/
#define	MT_CD		1	/* CD type media		*/
#define	MT_DVD		2	/* DVD type media		*/
#define	MT_BD		3	/* Blu Ray type media		*/
#define	MT_HDDVD	4	/* HD-DVD type media		*/

#define	TOCF_DUMMY	0x10	/* Write in dummy (simulation) mode	*/
#define	TOCF_MULTI	0x20	/* Multisession (Open Next Programarea) */

#define	TOCF_MASK	0xF0	/* All possible flags in tracktype	*/

#define	track_base(tp)	((tp) - (tp)->track)
/*
 * Defines for command line option flags
 */
#define	F_DUMMY		0x00000001L	/* Do dummy (simulation) writes */
#define	F_TOC		0x00000002L	/* Get TOC */
#define	F_EJECT		0x00000004L	/* Eject disk after doing the work */
#define	F_LOAD		0x00000008L	/* Load disk only */
#define	F_MULTI		0x00000010L	/* Create linkable TOC/multi-session */
#define	F_MSINFO	0x00000020L	/* Get multi-session info */
#define	F_FIX		0x00000040L	/* Fixate disk only */
#define	F_NOFIX		0x00000080L	/* Do not fixate disk */
#define	F_VERSION	0x00000100L	/* Print version info */
#define	F_CHECKDRIVE	0x00000200L	/* Check for driver */
#define	F_INQUIRY	0x00000400L	/* Do inquiry */
#define	F_PRCAP		0x00000800L	/* Print capabilities */
#define	F_SCANBUS	0x00001000L	/* Scan SCSI Bus */
#define	F_RESET		0x00002000L	/* Reset SCSI Bus */
#define	F_BLANK		0x00004000L	/* Blank CD-RW */
#define	F_PRATIP	0x00008000L	/* Print ATIP info */
#define	F_PRDINFO	0x00010000L	/* Print disk info (XXX not yet used)  */
#define	F_IGNSIZE	0x00020000L	/* Ignore disk size */
#define	F_SAO		0x00040000L	/* Session at once */
#define	F_RAW		0x00080000L	/* Raw mode */
#define	F_WRITE		0x00100000L	/* Disk is going to be written */
#define	F_FORCE		0x00200000L	/* Force things (e.g. blank on dead disk)  */
#define	F_WAITI		0x00400000L	/* Wait until data is available on stdin */
#define	F_OVERBURN	0x00800000L	/* Allow oveburning */
#define	F_CLONE		0x01000000L	/* Do clone writing */
#define	F_STDIN		0x02000000L	/* We are using stdin as CD data */
#define	F_IMMED		0x04000000L	/* Try tu use IMMED SCSI flag if possible */
#define	F_DLCK		0x08000000L	/* Load disk and lock door */
#define	F_SETDROPTS	0x10000000L	/* Set driver opts and exit */
#define	F_FORMAT	0x20000000L	/* Format media */
#define	F_ABORT		0x40000000L	/* Send abort sequence to drive */
#define	F_MEDIAINFO	0x80000000UL	/* Print media info */

#define	DATA_SEC_SIZE	2048		/* 2048 bytes */
#define	MODE2_SEC_SIZE	2336		/* 2336 bytes */
#define	AUDIO_SEC_SIZE	2352		/* 2352 bytes */
#define	RAW16_SEC_SIZE	(2352+16)	/* 2368 bytes */
#define	RAW96_SEC_SIZE	(2352+96)	/* 2448 bytes */

/*
 * Defines for multi session type (from SCSI-3/mmc)
 */
#define	MS_NONE		0	/* No B0 pointer. Next session not allowed*/
#define	MS_FINAL	1	/* B0 = FF:FF:FF. Next session not allowed*/
#define	MS_RES		2	/* Reserved				  */
#define	MS_MULTI	3	/* B0 = Next PA.  Next session allowed	  */

/*
 * Defines for session format (from SCSI-3/mmc)
 */
#define	SES_DA_ROM	0x00	/* CD-DA or CD-ROM disk			  */
#define	SES_CDI		0x10	/* CD-I disk				  */
#define	SES_XA		0x20	/* CD-ROM XA disk			  */
#define	SES_UNDEF	0xFF	/* Undefined disk type (read disk info)	  */

/*
 * Defines for blanking of CD-RW discs (from SCSI-3/mmc)
 */
#define	BLANK_DISC	0x00	/* Erase the entire disc		  */
#define	BLANK_MINIMAL	0x01	/* Erase the PMA, 1st session TOC, pregap */
#define	BLANK_TRACK	0x02	/* Erase an incomplete track		  */
#define	BLANK_UNRESERVE	0x03	/* Unreserve a track			  */
#define	BLANK_TAIL	0x04	/* Erase the tail of a track		  */
#define	BLANK_UNCLOSE	0x05	/* Unclose the last session		  */
#define	BLANK_SESSION	0x06	/* Erase the last session		  */

/*
 * Definitions for read disk information "recording flags"
 * used in UInt16_t "ds_cdrflags".
 */
#define	RF_WRITE	0x0001	/* Disk is going to be written		*/
#define	RF_BLANK	0x0002	/* Disk is going to be erased		*/
#define	RF_PRATIP	0x0004	/* Print ATIP info			*/
#define	RF_LEADIN	0x0008	/* Lead-in has been "manually" written	*/
#define	RF_BURNFREE	0x0010	/* BUFFER underrun free recording	*/
#define	RF_VARIREC	0x0020	/* Plextor VariRec			*/
#define	RF_AUDIOMASTER	0x0040	/* Yamaha AudioMaster			*/
#define	RF_FORCESPEED	0x0080	/* WriteSpeed forced high		*/
#define	RF_DID_STAT	0x0100	/* Already did call cdrstats()		*/
#define	RF_DID_CDRSTAT	0x0200	/* Already did call (*dp->cdr_stats)()	*/
#define	RF_WR_WAIT	0x0400	/* Wait during writing to free bus	*/
#define	RF_SINGLESESS	0x0800	/* Plextor single sess. mode		*/
#define	RF_HIDE_CDR	0x1000	/* Plextor hide CDR features		*/
#define	RF_SPEEDREAD	0x2000	/* Plextor SpeedReed			*/
#define	RF_GIGAREC	0x4000	/* Plextor GigaRec			*/

#define	CDR_TAO		0x01		/* Drive supports Track at once	*/
#define	CDR_SAO		0x02		/* Drive supports Sess at once	*/
#define	CDR_PACKET	0x04		/* Drive supports packet writing*/
#define	CDR_RAW		0x08		/* Drive supports raw writing	*/
#define	CDR_RAW16	0x10		/* Drive supports RAW raw16	*/
#define	CDR_RAW96P	0x20		/* Drive supports RAW raw96 pak	*/
#define	CDR_RAW96R	0x40		/* Drive supports RAW raw96 raw	*/
#ifdef	__needed__
#define	CDR_SRAW16	0x100		/* Drive supports SAO raw16	*/
#endif
#define	CDR_SRAW96P	0x200		/* Drive supports SAO raw96 pak	*/
#define	CDR_SRAW96R	0x400		/* Drive supports SAO raw96 raw */
#define	CDR_LAYER_JUMP	0x800		/* Drive s. DVD-R/DL Layer jump	*/
#define	CDR_SWABAUDIO	0x1000		/* Drive swabs audio data	*/
#define	CDR_ISREADER	0x2000		/* Drive is s CD-ROM reader	*/
#define	CDR_TRAYLOAD	0x4000		/* Drive loads CD with tray	*/
#define	CDR_CADDYLOAD	0x8000		/* Drive loads CD with caddy	*/
#define	CDR_NO_LOLIMIT	0x10000		/* Drive ignores lead-out limit	*/
#define	CDR_DVD		0x20000		/* Drive is a DVD drive		*/
#define	CDR_SIMUL	0x40000		/* Drive is simulated		*/
#define	CDR_BURNFREE	0x80000		/* Drive sup. BUFund. free rec.	*/
#define	CDR_VARIREC	0x100000	/* Drive sup. VariRec	 Plex.	*/
#define	CDR_AUDIOMASTER	0x200000	/* Drive sup. AudioMaster Yamah.*/
#define	CDR_FORCESPEED	0x400000	/* Drive sup. WriteSpeed ctl.	*/
#define	CDR_DISKTATTOO	0x800000	/* Drive sup. Yamaha DiskT@2	*/
#define	CDR_SINGLESESS	0x1000000	/* Drive sup. single sess. mode */
#define	CDR_HIDE_CDR	0x2000000	/* Drive sup. hide CDR features	*/
#define	CDR_SPEEDREAD	0x4000000	/* Drive sup. SpeedReed		*/
#define	CDR_GIGAREC	0x8000000	/* Drive sup. GigaRec	 Plex.	*/
#define	CDR_MMC		0x10000000	/* Drive is MMC compliant	*/
#define	CDR_MMC2	0x20000000	/* Drive is MMC-2 compliant	*/
#define	CDR_MMC3	0x40000000	/* Drive is MMC-3 compliant	*/
/*
 * Definitions for read disk information "disk status"
 * used in "ds_diskstat".
 */
#define	DS_EMPTY	0	/* Empty disk				*/
#define	DS_APPENDABLE	1	/* Incomplete disk (appendable)		*/
#define	DS_COMPLETE	2	/* Complete disk (closed/no B0 pointer)	*/
#define	DS_RESERVED	3	/* Reserved				*/

/*
 * Definitions for read disk information "session status"
 * used in "ds_sessstat".
 */
#define	SS_EMPTY	0	/* Empty session			*/
#define	SS_APPENDABLE	1	/* Incomplete session			*/
#define	SS_RESERVED	2	/* Reserved				*/
#define	SS_COMPLETE	3	/* Complete session (needs DS_COMPLETE)	*/

/*
 * Adr nibble:
 */
#define	ADR_NONE	0	/* Sub-Q mode info not supplied		*/
#define	ADR_POS		1	/* Sub-Q encodes position data		*/
#define	ADR_MCN		2	/* Sub-Q encodes Media Catalog Number	*/
#define	ADR_ISRC	3	/* Sub-Q encodes ISRC			*/

/*
 * Defines for write type (from SCSI-3/mmc)
 */
#define	WT_PACKET	0x0	/* Packet writing	*/
#define	WT_TAO		0x1	/* Track at once	*/
#define	WT_SAO		0x2	/* Session at once	*/
#define	WT_RAW		0x3	/* Raw			*/
#define	WT_LAYER_JUMP	0x4	/* DVD-R/DL Layer jump	*/
#define	WT_RES_5	0x5	/* Reserved		*/
#define	WT_RES_6	0x6	/* Reserved		*/
#define	WT_RES_7	0x7	/* Reserved		*/
#define	WT_RES_8	0x8	/* Reserved		*/
#define	WT_RES_9	0x9	/* Reserved		*/
#define	WT_RES_A	0xA	/* Reserved		*/
#define	WT_RES_B	0xB	/* Reserved		*/
#define	WT_RES_C	0xC	/* Reserved		*/
#define	WT_RES_D	0xD	/* Reserved		*/
#define	WT_RES_E	0xE	/* Reserved		*/
#define	WT_RES_F	0xF	/* Reserved		*/

/*
 * Definitions for cdr_flags
 */
#define	CDR_TAO		0x01		/* Drive supports Track at once	*/
#define	CDR_SAO		0x02		/* Drive supports Sess at once	*/
#define	CDR_PACKET	0x04		/* Drive supports packet writing*/
#define	CDR_RAW		0x08		/* Drive supports raw writing	*/
#define	CDR_RAW16	0x10		/* Drive supports RAW raw16	*/
#define	CDR_RAW96P	0x20		/* Drive supports RAW raw96 pak	*/
#define	CDR_RAW96R	0x40		/* Drive supports RAW raw96 raw	*/

#define	CDR_SRAW96P	0x200		/* Drive supports SAO raw96 pak	*/
#define	CDR_SRAW96R	0x400		/* Drive supports SAO raw96 raw */
#define	CDR_LAYER_JUMP	0x800		/* Drive s. DVD-R/DL Layer jump	*/
#define	CDR_SWABAUDIO	0x1000		/* Drive swabs audio data	*/
#define	CDR_ISREADER	0x2000		/* Drive is s CD-ROM reader	*/
#define	CDR_TRAYLOAD	0x4000		/* Drive loads CD with tray	*/
#define	CDR_CADDYLOAD	0x8000		/* Drive loads CD with caddy	*/
#define	CDR_NO_LOLIMIT	0x10000		/* Drive ignores lead-out limit	*/
#define	CDR_DVD		0x20000		/* Drive is a DVD drive		*/
#define	CDR_SIMUL	0x40000		/* Drive is simulated		*/
#define	CDR_BURNFREE	0x80000		/* Drive sup. BUFund. free rec.	*/
#define	CDR_VARIREC	0x100000	/* Drive sup. VariRec	 Plex.	*/
#define	CDR_AUDIOMASTER	0x200000	/* Drive sup. AudioMaster Yamah.*/
#define	CDR_FORCESPEED	0x400000	/* Drive sup. WriteSpeed ctl.	*/
#define	CDR_DISKTATTOO	0x800000	/* Drive sup. Yamaha DiskT@2	*/
#define	CDR_SINGLESESS	0x1000000	/* Drive sup. single sess. mode */
#define	CDR_HIDE_CDR	0x2000000	/* Drive sup. hide CDR features	*/
#define	CDR_SPEEDREAD	0x4000000	/* Drive sup. SpeedReed		*/
#define	CDR_GIGAREC	0x8000000	/* Drive sup. GigaRec	 Plex.	*/
#define	CDR_MMC		0x10000000	/* Drive is MMC compliant	*/
#define	CDR_MMC2	0x20000000	/* Drive is MMC-2 compliant	*/
#define	CDR_MMC3	0x40000000	/* Drive is MMC-3 compliant	*/
#define	CDR_ALLOC	0x80000000UL	/* structure is allocated	*/

/*
 * Definitions for cdr_flags
 */
#define	CDR2_NOCD	0x01		/* Drive not operating on a CD	*/
#define	CDR2_BD		0x02		/* Drive is a BluRay drive	*/
/*
 * Definitions for cdr_cdrw_support
 */
#define	CDR_CDRW_NONE	0x00		/* CD-RW writing not supported */
#define	CDR_CDRW_MULTI	0x01		/* CD-RW multi speed supported */
#define	CDR_CDRW_HIGH	0x02		/* CD-RW high speed supported */
#define	CDR_CDRW_ULTRA	0x04		/* CD-RW ultra high speed supported */
#define	CDR_CDRW_ULTRAP	0x08		/* CD-RW ultra high speed+ supported */
#define	CDR_CDRW_ALL	0xFF		/* All bits set: unknown - support all */

/*
 * Definitions for disk_status write mode
 * used in "ds_wrmode".
 */
#define	WM_NONE		0	/* No write mode selected		*/
#define	WM_BLANK	1	/* Blanking mode			*/
#define	WM_FORMAT	2	/* Formatting				*/
#define	WM_PACKET	4	/* Packet writing			*/
#define	WM_TAO		8	/* Track at Once			*/
#define	WM_SAO		12	/* Session at Once w/ cooked sectors	*/
#define	WM_SAO_RAW16	13	/* Session at Once RAW+16 byte sectors	*/
#define	WM_SAO_RAW96P	14	/* Session at Once RAW+96P byte sectors	*/
#define	WM_SAO_RAW96R	15	/* Session at Once RAW+96R byte sectors	*/
#define	WM_RAW		16	/* RAW with cooked sectors is impossible*/
#define	WM_RAW_RAW16	17	/* RAW with RAW+16 byte sectors		*/
#define	WM_RAW_RAW96P	18	/* RAW with RAW+96P byte sectors	*/
#define	WM_RAW_RAW96R	19	/* RAW with RAW+96R byte sectors	*/

#define	wm_base(wm)	((wm)/4*4) /* The basic write mode for this mode */

/*
 * Definitions for disk_status flags
 * used in UInt16_t "ds_flags".
 */
#define	DSF_DID_V	0x0001	/* Disk id valid			*/
#define	DSF_DBC_V	0x0002	/* Disk bar code valid			*/
#define	DSF_URU		0x0004	/* Disk is for unrestricted use		*/
#define	DSF_ERA		0x0008	/* Disk is erasable			*/
#define	DSF_HIGHSP_ERA	0x0010	/* Disk is high speed erasable		*/
#define	DSF_ULTRASP_ERA	0x0020	/* Disk is ultra speed erasable		*/
#define	DSF_ULTRASPP_ERA 0x0040	/* Disk is ultra speed+ erasable	*/
#define	DSF_NOCD	0x0080	/* Disk is not a CD			*/

#define	DSF_DVD		0x0100	/* Disk is a DVD			*/
#define	DSF_DVD_PLUS_R	0x0200	/* Disk is a DVD+R			*/
#define	DSF_DVD_PLUS_RW	0x0400	/* Disk is a DVD+RW			*/
#define	DSF_NEED_FORMAT	0x0800	/* Disk needs to be formatted		*/
#define	DSF_BD		0x1000	/* Disk is a BD				*/
#define	DSF_BD_RE	0x2000	/* Disk is a BD-RE			*/

/*
 * Definitions for disk_status disk type
 * used in "ds_type".
 */
#define	DST_UNKNOWN	0
#define	DST_CD_ROM	0x08
#define	DST_CD_R	0x09
#define	DST_CD_RW	0x0A
#define	DST_DVD_ROM	0x10
#define	DST_DVD_R	0x11
#define	DST_DVD_RAM	0x12
#define	DST_DVD_RW	0x13
#define	DST_DVD_RW_SEQ	0x14
#define	DST_DVD_R_DL	0x15
#define	DST_DVD_R_DL_LJ	0x16
#define	DST_DVD_PLUS_RW	0x1A
#define	DST_DVD_PLUS_R	0x1B
#define	DST_DVD_PLUS_R_DL 0x2B
#define	DST_BD_ROM	0x40
#define	DST_BD_R_SEQ	0x41
#define	DST_BD_R	0x42
#define	DST_BD_RE	0x43

/*
 * Defines for tp->flags
 */
#define	TI_AUDIO	0x00001	/* File is an audio track		*/
#define	TI_PREEMP	0x00002	/* Audio track recorded w/preemphasis	*/
#define	TI_MIX		0x00004	/* This is a mixed mode track		*/
#define	TI_RAW		0x00008	/* Write this track in raw mode		*/
#define	TI_PAD		0x00010	/* Pad data track			*/
#define	TI_SWAB		0x00020	/* Swab audio data			*/
#define	TI_ISOSIZE	0x00040	/* Use iso size for track		*/
#define	TI_NOAUHDR	0x00080	/* Don't look for audio header		*/
#define	TI_FIRST	0x00100	/* This is the first track		*/
#define	TI_LAST		0x00200	/* This is the last track		*/
#define	TI_PACKET	0x00400	/* Fixed- or variable-packet track	*/
#define	TI_NOCLOSE	0x00800	/* Don't close the track after writing	*/
#define	TI_TAO		0x01000	/* This track is written in TAO mode	*/
#define	TI_PREGAP	0x02000	/* Prev. track incl. pregap of this tr. */
#define	TI_SCMS		0x04000	/* Force to follow the SCMS rules	*/
#define	TI_COPY		0x08000	/* Allow digital copy			*/
#define	TI_SHORT_TRACK	0x10000	/* Ignore min 4 second Red Book	std.	*/
#define	TI_RAW16	0x20000	/* This track uses 16 bytes subch.	*/
#define	TI_RAW96R	0x40000	/* This track uses 96 bytes RAW subch.	*/
#define	TI_CLONE	0x80000	/* Special clone treatment needed	*/
#define	TI_TEXT		0x100000 /* This track holds CD-Text information */
#define	TI_NOCD		0x200000 /* We are not writing a CD track	*/
#define	TI_SAO		0x400000 /* This track is written in SAO mode	*/
#define	TI_USEINFO	0x800000 /* Use information from *.inf files	*/
#define	TI_QUADRO	0x1000000 /* Four Channel Audio Data		*/

#define	is_audio(tp)	(((tp)->flags & TI_AUDIO) != 0)
#define	is_preemp(tp)	(((tp)->flags & TI_PREEMP) != 0)
#define	is_pad(tp)	(((tp)->flags & TI_PAD) != 0)
#define	is_swab(tp)	(((tp)->flags & TI_SWAB) != 0)
#define	is_first(tp)	(((tp)->flags & TI_FIRST) != 0)
#define	is_last(tp)	(((tp)->flags & TI_LAST) != 0)
#define	is_packet(tp)	(((tp)->flags & TI_PACKET) != 0)
#define	is_noclose(tp)	(((tp)->flags & TI_NOCLOSE) != 0)
#define	is_tao(tp)	(((tp)->flags & TI_TAO) != 0)
#define	is_sao(tp)	(((tp)->flags & TI_SAO) != 0)
#define	is_raw(tp)	(((tp)->flags & TI_RAW) != 0)
#define	is_raw16(tp)	(((tp)->flags & TI_RAW16) != 0)
#define	is_raw96(tp)	(((tp)->flags & (TI_RAW|TI_RAW16)) == TI_RAW)
#define	is_raw96p(tp)	(((tp)->flags & (TI_RAW|TI_RAW16|TI_RAW96R)) == TI_RAW)
#define	is_raw96r(tp)	(((tp)->flags & (TI_RAW|TI_RAW16|TI_RAW96R)) == (TI_RAW|TI_RAW96R))
#define	is_pregap(tp)	(((tp)->flags & TI_PREGAP) != 0)
#define	is_scms(tp)	(((tp)->flags & TI_SCMS) != 0)
#define	is_copy(tp)	(((tp)->flags & TI_COPY) != 0)
#define	is_shorttrk(tp)	(((tp)->flags & TI_SHORT_TRACK) != 0)
#define	is_clone(tp)	(((tp)->flags & TI_CLONE) != 0)
#define	is_text(tp)	(((tp)->flags & TI_TEXT) != 0)
#define	is_quadro(tp)	(((tp)->flags & TI_QUADRO) != 0


#define	DB_RAW		0	/* 2352 bytes of raw data		  */
#define	DB_RAW_PQ	1	/* 2368 bytes (raw data + P/Q Subchannel) */
#define	DB_RAW_PW	2	/* 2448 bytes (raw data + P-W Subchannel) */
#define	DB_RAW_PW_R	3	/* 2448 bytes (raw data + P-W raw Subchannel)*/
#define	DB_RES_4	4	/* -	Reserved			  */
#define	DB_RES_5	5	/* -	Reserved			  */
#define	DB_RES_6	6	/* -	Reserved			  */
#define	DB_VU_7		7	/* -	Vendor specific			  */
#define	DB_ROM_MODE1	8	/* 2048 bytes Mode 1 (ISO/IEC 10149)	  */
#define	DB_ROM_MODE2	9	/* 2336 bytes Mode 2 (ISO/IEC 10149)	  */
#define	DB_XA_MODE2	10	/* 2048 bytes Mode 2 (CD-ROM XA form 1)   */
#define	DB_XA_MODE2_F1	11	/* 2056 bytes Mode 2 (CD-ROM XA form 1)	  */
#define	DB_XA_MODE2_F2	12	/* 2324 bytes Mode 2 (CD-ROM XA form 2)	  */
#define	DB_XA_MODE2_MIX	13	/* 2332 bytes Mode 2 (CD-ROM XA 1/2+subhdr) */
#define	DB_RES_14	14	/* -	Reserved			  */
#define	DB_VU_15	15	/* -	Vendor specific			  */

#define	g0_cdbaddr(cdb, a)	((cdb)->high_addr   = (a) >> 16,\
				    (cdb)->mid_addr = ((a) >> 8) & 0xFF,\
				    (cdb)->low_addr = (a) & 0xFF)

#define	g1_cdbaddr(cdb, a)	((cdb)->addr[0]    = (a) >> 24,\
				    (cdb)->addr[1] = ((a) >> 16)& 0xFF,\
				    (cdb)->addr[2] = ((a) >> 8) & 0xFF,\
				    (cdb)->addr[3] = (a) & 0xFF)

#define	g5_cdbaddr(cdb, a)	g1_cdbaddr(cdb, a)


#define	g0_cdblen(cdb, len)	((cdb)->count = (len))

#define	g1_cdblen(cdb, len)	((cdb)->count[0]    = ((len) >> 8) & 0xFF,\
				    (cdb)->count[1] = (len) & 0xFF)

#define	g5_cdblen(cdb, len)	((cdb)->count[0]    = (len) >> 24L,\
				    (cdb)->count[1] = ((len) >> 16L)& 0xFF,\
				    (cdb)->count[2] = ((len) >> 8L) & 0xFF,\
				    (cdb)->count[3] = (len) & 0xFF)

#define	TM_PREEM	0x1				/* Audio track with preemphasis	*/
#define	TM_INCREMENTAL 0x1	/* Incremental data track	*/
#define	TM_ALLOW_COPY	0x2		/* Digital copy permitted	*/
#define	TM_DATA		0x4				/* This is a data track		*/
#define	TM_QUADRO	0x8				/* Four channel audio		*/

#define	ROTCTL_CLV	0	/* CLV or PCAV	*/
#define	ROTCTL_CAV	1	/* True CAV	*/

#define	TI_TYPE_LBA	0	/* Address is LBA */
#define	TI_TYPE_TRACK	1	/* Address: 0 -> TOC, xx -> Track xx, 0xFF -> Inv Track */
#define	TI_TYPE_SESS	2	/* Address is session # */

#define	CL_TYPE_STOP_DEICE	0	/* Stop De-icing a DVD+RW Media */
#define	CL_TYPE_TRACK		1	/* Close Track # */
#define	CL_TYPE_SESSION		2	/* Close Session/Border / Stop backgrnd. format */
#define	CL_TYPE_INTER_BORDER	3	/* Close intermediate Border */

#define	ST_ROM_MODE1	1	/* CD-ROM in mode 1 (vanilla cdrom)	*/
#define	ST_ROM_MODE2	2	/* CD-ROM in mode 2			*/
#define	ST_AUDIO_NOPRE	4	/* CD-DA stereo without preemphasis	*/
#define	ST_AUDIO_PRE	5	/* CD-DA stereo with preemphasis	*/

#define	ST_PREEMPMASK	0x01	/* Mask for preemphasis bit		*/
#define	ST_AUDIOMASK	0x04	/* Mask for audio bit			*/
#define	ST_MODEMASK	0x03	/* Mask for mode bits in sector type	*/
#define	ST_MASK		0x07	/* Mask needed for sectname[]		*/

#define	ST_MODE_AUDIO	 0x00	/* Generic Audio mode			*/
#define	ST_MODE_0	 0x10	/* Generic Zero mode			*/
#define	ST_MODE_1	 0x20	/* Generic CD-ROM mode	(ISO/IEC 10149)	*/
#define	ST_MODE_2	 0x30	/* Generic Mode 2	(ISO/IEC 10149)	*/
#define	ST_MODE_2_FORM_1 0x40	/* Generic Mode 2 form 1		*/
#define	ST_MODE_2_FORM_2 0x50	/* Generic Mode 2 form 2		*/
#define	ST_MODE_2_MIXED	 0x60	/* Generic Mode 2 mixed form (1/2)	*/

#define	ST_MODE_MASK	 0x70	/* Mask needed to get generic sectype	*/

#define	ST_MODE_RAW	 0x08	/* Do not touch EDC & subchannels	*/
#define	ST_NOSCRAMBLE	 0x80	/* Do not srcamble sectors 		*/

#define	SECT_AUDIO	(ST_AUDIO_NOPRE  | ST_MODE_AUDIO)
#define	SECT_AUDIO_NOPRE (ST_AUDIO_NOPRE | ST_MODE_AUDIO)
#define	SECT_AUDIO_PRE	(ST_AUDIO_PRE    | ST_MODE_AUDIO)
#define	SECT_MODE_0	(ST_ROM_MODE1    | ST_MODE_0)
#define	SECT_ROM	(ST_ROM_MODE1    | ST_MODE_1)
#define	SECT_MODE_2	(ST_ROM_MODE2    | ST_MODE_2)
#define	SECT_MODE_2_F1	(ST_ROM_MODE2    | ST_MODE_2_FORM_1)
#define	SECT_MODE_2_F2	(ST_ROM_MODE2    | ST_MODE_2_FORM_2)
#define	SECT_MODE_2_MIX	(ST_ROM_MODE2    | ST_MODE_2_MIXED)

#define	CDR_BUF_SIZE		(63*1024)
#define	CDR_OLD_BUF_SIZE	(63*1024)
#define	CDR_MAX_BUF_SIZE	(256*1024)

//isoinfo
#define E_GrCdRecFileCnt		8

#define E_GrCdRecIsoStSec 	0x18

//====================================================================
//global macro

//====================================================================
//global type

typedef	void	(*Fnc_GrOddPageFlush)( void );

typedef struct msf {
	__s8	msf_min;
	__s8	msf_sec;
	__s8	msf_frame;
} msf_t;

typedef	struct St_GrOddFileInfo 
{
	__u32	SctPos;						//sector position
	Def_WstrName	StrFn;		//file name - name only, can not include directory.	
	__u32	Allocated;				//allocated size
	__u32	Size;							//file size
	__u32	Prgs;							//progress size
}	*Ptr_GrOddFileInfo;

//====================================================================
//class
class Cls_GrOddRec
{
	private:

		St_GrScsiIf*		m_ObjScgp;
		cdr_t*	m_ObjDp;
		Def_StrName	m_StrDev;

		BOOL8		m_IsFinished;

		__u32		m_Stage;
		Def_WstrName	m_StrVolName;
		__u32		m_FileCnt;
		St_GrOddFileInfo	m_FileTbl[E_GrIsoFileMaxCnt];

		__u32		m_PrgSctPos;		//progress sector position
		__u32		m_FilePrgIdx;		//file progress index
		__u64		m_IsoTtSize;		//iso total size
		__u32		m_IsoTtSct;			//iso total sector count

		__u32		m_BufSctCnt;		//buffer sector count
		__u32		m_BufAbleSize;	//buffer useable size

		__u32		m_HeadStage;
		__u32		m_HeadWtSctResid;		//head write sector reside
		Def_StrName		m_StrVolTime;	//volume descriptor time
		__u8		m_DirRecTime[7];		//direct record time
		__u32		m_RecTblPriSize;
		__u32		m_RecTblSplSize;
		__u32		m_HeadWtSize;				//head write size

		__u32		m_BufInSize;				//buffer in size
		__u32		m_TailGapResid;

		Fnc_GrOddPageFlush	m_FncPageFlush;
		__u8		m_ImgHdBuf[E_GrOddImgHdBufSize];

#ifndef LINUX_APP
		Cls_GrFileCtrl*	m_IsoFc;
#endif

		dstat_t	m_DskStat;

		__u32 	m_CmdFlag;
		__s32			m_TimeOut;
		__s32			m_Speed;
		__s32			m_DmaSpeed;
		__s32			m_RawSpeed;

		__s32			m_BufSize;
		__s8*		m_PtrBuf;

		__s32			m_blanktype;
		long		m_startsec;
		__u32		m_StartSec;
		__s32			m_ByteSpt;

		track_t*	m_PtrTrack;

	protected:
		//loca
		void		LcReleaseObj( void );
		void		LcInitVar( __s8* A_StrDev );
		BOOL8		LcInitDev( __u64 A_IsoSize, BOOL8 A_IsWrite );

		//iso 2
		void		LcIsoBldTime( void );
		void		LcIsoFillJolietSpace( void* A_PtrBuf, __u32 A_Size );
		__u32		LcIsoStrChr( void* A_PtrBuf, WCHAR* A_Str, BOOL8 A_IsTypeA, __u32 A_Limite, BOOL8 A_IsFixSize );
		__u32		LcIsoStrJoliet( void* A_PtrBuf, WCHAR* A_Str );
		__u32		LcIsoDirRecItm( void* A_PtrBuf, __u32 A_SctPos, __u32 A_FileSize, BOOL8 A_IsDir, 
						WCHAR* A_StrFn, BOOL8 A_IsJoliet );
		__u32		LcIsoDirRecTbl( void* A_PtrBuf, BOOL8 A_IsSpl );
		void		LcIsoMakeVolDes( void* A_PtrBuf, __u8 A_Type, WCHAR* A_StrVolume );
		__u32		LcIsoLpathItm( void* A_PtrBuf, __u32 A_DirSctPos );
		__u32		LcIsoMpathItm( void* A_PtrBuf, __u32 A_DirSctPos );
		void		LcHeadWriteGap( void );
		void		LcHeadBuildImg( void );
		void		LcHeadWriteImg( void );
		BOOL8		LcBufWriteData( void* A_PtrData, __u32 A_Size );
		BOOL8		LcBufWriteZero( __u32 A_Size );
		BOOL8		LcBufFlush( void );

		BOOL8		LcPatchWriteMode( void );		//check writeable and mode


	public:
		Cls_GrOddRec( Fnc_GrOddPageFlush A_FncPageFlush );	//iso offset

		virtual	~Cls_GrOddRec();

		void		Eject(BOOL8 A_IsOpen);
		BOOL8 		IsExistCdr();

		//normal
		__u64		MediaSizeGet( __s8* A_StrDev );

		//record
		BOOL8		RecInit( __s8* A_StrDev );		//record initialize
		BOOL8		RecFileRegist( WCHAR* A_StrFn, __u32 A_Size );
		BOOL8		RecPrepare( WCHAR* A_StrVol );
		BOOL8		RecHeadWrite( void );
		BOOL8		RecFileWrite( void* A_PtrBuf, __u32 A_Size );
		BOOL8		RecFileZeroWrite( __u32 A_Size );
		BOOL8		RecFileClose( void );
		BOOL8		RecTailWrite( void );
		BOOL8		RecFinish( BOOL8 A_IsFixate = TRUE );
		__u32		RecFileInfoGet( __u32* A_PtrRtIdx, __u32* A_PtrRtResid );
		__u32		RecStageGet( void );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

