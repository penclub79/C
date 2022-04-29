/*
	odd scsi define
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddDef
#define _EPC_GrOddDef

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef LINUX_APP
#include 	<sys/file.h>
#else
#include	<EmuLnx/GrElaBase.h>
#endif

#include 	<stdio.h>

//====================================================================
//constance
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
#define	is_quadro(tp)	(((tp)->flags & TI_QUADRO) != 0)


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

#define	INQ_DEV_PRESENT	0x00		/* Physical device present */
#define	INQ_DEV_NOTPR	0x01		/* Physical device not present */
#define	INQ_DEV_RES	0x02		/* Reserved */
#define	INQ_DEV_NOTSUP	0x03		/* Logical unit not supported */

/* Peripheral Device Type */

#define	INQ_DASD	0x00		/* Direct-access device (disk) */
#define	INQ_SEQD	0x01		/* Sequential-access device (tape) */
#define	INQ_PRTD	0x02 		/* Printer device */
#define	INQ_PROCD	0x03 		/* Processor device */
#define	INQ_OPTD	0x04		/* Write once device (optical disk) */
#define	INQ_WORM	0x04		/* Write once device (optical disk) */
#define	INQ_ROMD	0x05		/* CD-ROM device */
#define	INQ_SCAN	0x06		/* Scanner device */
#define	INQ_OMEM	0x07		/* Optical Memory device */
#define	INQ_JUKE	0x08		/* Medium Changer device (jukebox) */
#define	INQ_COMM	0x09		/* Communications device */
#define	INQ_IT8_1	0x0A		/* IT8 */
#define	INQ_IT8_2	0x0B		/* IT8 */
#define	INQ_STARR	0x0C		/* Storage array device */
#define	INQ_ENCL	0x0D		/* Enclosure services device */
#define	INQ_SDAD	0x0E		/* Simplyfied direct-access device */
#define	INQ_OCRW	0x0F		/* Optical card reader/writer device */
#define	INQ_BRIDGE	0x10		/* Bridging expander device */
#define	INQ_OSD		0x11		/* Object based storage device */
#define	INQ_ADC		0x12		/* Automation/Drive interface */
#define	INQ_WELLKNOWN	0x1E		/* Well known logical unit */
#define	INQ_NODEV	0x1F		/* Unknown or no device */
#define	INQ_NOTPR	0x1F		/* Logical unit not present (SCSI-1) */

#define	MP_P_CODE			\
	unsigned char	p_code		: 6;	\
	unsigned char	p_res		: 1;	\
	unsigned char	parsave		: 1

#define	DTDC_DATADONE	0x01
#define	DTDC_CMDDONE	0x03

#define	PDEV_SCSI	0x0000		/* scsi interface */
#define	PDEV_SMD	0x0001		/* SMD interface */
#define	PDEV_ESDI	0x0002		/* ESDI interface */
#define	PDEV_IPI2	0x0003		/* IPI-2 interface */
#define	PDEV_IPI3	0x0004		/* IPI-3 interface */

#define	CTRL_QMOD_RESTRICT	0x0
#define	CTRL_QMOD_UNRESTRICT	0x1

#define	LT_CADDY	0
#define	LT_TRAY		1
#define	LT_POP_UP	2
#define	LT_RES3		3
#define	LT_CHANGER_IND	4
#define	LT_CHANGER_CART	5
#define	LT_RES6		6
#define	LT_RES7		7

#define	def_block	def_list.list_block
#define	def_bfi		def_list.list_bfi
#define	def_phys	def_list.list_phys

#define	SC_DEF_BLOCK	0
#define	SC_DEF_BFI	4
#define	SC_DEF_PHYS	5
#define	SC_DEF_VU	6
#define	SC_DEF_RES	7

/*
 * Defines for 'fmt_type'.
 */
#define	FCAP_TYPE_DVDPLUS_FULL	0x26	/* DVD+RW Full Format		*/

/*
 * Defines for 'desc_type'.
 * In case of FCAP_DESC_RES, the descriptor is a formatted capacity descriptor
 * and the 'blen' field is type dependent.
 * For all other cases, this is the Current/Maximum Capacity descriptor and
 * the value of 'fmt_type' is reserved and must be zero.
 */
#define	FCAP_DESC_RES		0	/* Reserved			*/
#define	FCAP_DESC_UNFORM	1	/* Unformatted Media		*/
#define	FCAP_DESC_FORM		2	/* Formatted Media		*/
#define	FCAP_DESC_NOMEDIA	3	/* No Media			*/

//====================================================================
//global macro

//====================================================================
//global type
typedef struct St_GrOddTrack 
{
	__s64	itracksize;	/* Size of track bytes (-1 == until EOF)*/
	/* This is in units of isecsize		*/
	__s64	tracksize;	/* Size of track bytes (-1 == until EOF)*/
	/* This is in units of secsize		*/

	long	trackstart;	/* Start sector # of this track		*/
	long	tracksecs;	/* Size of this track (sectors)		*/
	long	padsecs;	/* Pad size for this track (sectors)	*/
	long	pregapsize;	/* Pre-gap size for this track (sectors)*/
	long	index0start;	/* Index 0 start within this tr(sectors)*/
	int	isecsize;	/* Read input sector size for this track*/
	int	secsize;	/* Sector size for this track (bytes)	*/
	int	secspt;		/* # of sectors to copy for one transfer*/
	int	pktsize;	/* # of blocks per write packet		*/
	unsigned char	dataoff;	/* offset of user data in raw sector	*/
	unsigned char	tracks;		/* Number of tracks on this disk	*/
	unsigned char	track;		/* Track # as offset in track[] array	*/
	unsigned char	trackno;	/* Track # on disk for this track	*/
	unsigned char	tracktype;	/* Track type (toc type)		*/
	unsigned char	dbtype;		/* Data block type for this track	*/
	int	sectype;	/* Sector type				*/
	int	flags;		/* Flags (see below)			*/
	int	nindex;		/* Number of indices for track		*/
	long	*tindex;	/* Track index descriptor		*/
	char	*isrc;		/* ISRC code for this track / disk MCN	*/
	void	*text;		/* Opaque CD-Text data (txtptr_t *)	*/
} track_t, *Ptr_GrOddTrack;

struct disk_status {
	track_t	*ds_trackp;		/* Pointer to track structure	*/
	__u32 ds_diskid;		/* Disk identification		*/
	__u16 ds_cdrflags;		/* Recording flags from cdrecord*/
	__u16 ds_flags;		/* Disk_status flags		*/
	unsigned char	 ds_wrmode;		/* Selected write mode		*/
	unsigned char	 ds_type;		/* Abstract disk type		*/

	unsigned char	 ds_disktype;		/* Disk type (from TOC/PMA)	*/
	unsigned char	 ds_diskstat;		/* Disk status (MMC)		*/
	unsigned char	 ds_sessstat;		/* Status of last sesion (MMC)	*/
	unsigned char	 ds_trfirst;		/* first track #		*/
	unsigned char	 ds_trlast;		/* last track #			*/
	unsigned char	 ds_trfirst_ls;		/* first track # in last session*/
	unsigned char	 ds_barcode[8];		/* Disk bar code		*/

	int		 ds_first_leadin;	/* Start of first lead in (ATIP)*/
	int		 ds_last_leadout;	/* Start of last lead out (ATIP)*/
	int		 ds_curr_leadin;	/* Start of next lead in	*/
	int		 ds_curr_leadout;	/* Start of next lead out	*/

	int		 ds_maxblocks;		/* # of official blocks on disk	*/
	int		 ds_maxrblocks;		/* # real blocks on disk	*/
	int		 ds_fwa;		/* first writable addr		*/

	int		 ds_startsec;		/* Actual start sector		*/
	int		 ds_endsec;		/* Actual end sector		*/
	int		 ds_buflow;		/* # of times drive buffer empty*/

	unsigned short ds_minbuf;		/* Minimum drive bufer fill rt.	*/

	unsigned short ds_at_min_speed;	/* The minimal ATIP write speed	*/
	unsigned short ds_at_max_speed;	/* The maximal ATIP write speed	*/
	unsigned short ds_dr_cur_rspeed;	/* The drive's cur read speed	*/
	unsigned short ds_dr_max_rspeed;	/* The drive's max read speed	*/
	unsigned short ds_dr_cur_wspeed;	/* The drive's cur write speed	*/
	unsigned short ds_dr_max_wspeed;	/* The drive's max write speed	*/
	unsigned short ds_wspeed;		/* The selected/drive wr. speed */

	int		 ds_layer_break;	/* Start of layer break		*/
};

struct	scsi_inquiry {
	unsigned char	type		: 5;	/*  0 */
	unsigned char	qualifier	: 3;	/*  0 */

	unsigned char	type_modifier	: 7;	/*  1 */
	unsigned char	removable	: 1;	/*  1 */

	unsigned char	ansi_version	: 3;	/*  2 */
	unsigned char	ecma_version	: 3;	/*  2 */
	unsigned char	iso_version	: 2;	/*  2 */

	unsigned char	data_format	: 4;	/*  3 */
	unsigned char	res3_54		: 2;	/*  3 */
	unsigned char	termiop		: 1;	/*  3 */
	unsigned char	aenc		: 1;	/*  3 */

	unsigned char	add_len		: 8;	/*  4 */
	unsigned char	sense_len	: 8;	/*  5 */ /* only Emulex ??? */
	unsigned char	res2		: 8;	/*  6 */

	unsigned char	softreset	: 1;	/*  7 */
	unsigned char	cmdque		: 1;
	unsigned char	res7_2		: 1;
	unsigned char	linked		: 1;
	unsigned char	sync		: 1;
	unsigned char	wbus16		: 1;
	unsigned char	wbus32		: 1;
	unsigned char	reladr		: 1;	/*  7 */

	char	vendor_info[8];		/*  8 */
	char	prod_ident[16];		/* 16 */
	char	prod_revision[4];	/* 32 */
};					/* 96 */

struct scsi_mode_header {
	unsigned char	sense_data_len	: 8;
	unsigned char	medium_type;
	unsigned char	res2		: 4;
	unsigned char	cache		: 1;
	unsigned char	res		: 2;
	unsigned char	write_prot	: 1;
	unsigned char	blockdesc_len;
};

struct scsi_modesel_header {
	unsigned char	sense_data_len	: 8;
	unsigned char	medium_type;
	unsigned char	res2		: 8;
	unsigned char	blockdesc_len;
};

struct scsi_mode_blockdesc {
	unsigned char	density;
	unsigned char	nlblock[3];
	unsigned char	res		: 8;
	unsigned char	lblen[3];
};

struct acb_mode_data {
	unsigned char	listformat;
	unsigned char	ncyl[2];
	unsigned char	nhead;
	unsigned char	start_red_wcurrent[2];
	unsigned char	start_precomp[2];
	unsigned char	landing_zone;
	unsigned char	step_rate;
	unsigned char			: 2;
	unsigned char	hard_sec	: 1;
	unsigned char	fixed_media	: 1;
	unsigned char			: 4;
	unsigned char	sect_per_trk;
};

struct scsi_mode_page_header {
	unsigned char	p_code		: 6;
	unsigned char	res		: 1;
	unsigned char	parsave		: 1;
	unsigned char	p_len;
};

struct scsi_mode_page_01 {		/* Error recovery Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0A = 12 Bytes */
	unsigned char	disa_correction	: 1;	/* Byte 2 */
	unsigned char	term_on_rec_err	: 1;
	unsigned char	report_rec_err	: 1;
	unsigned char	en_early_corr	: 1;
	unsigned char	read_continuous	: 1;
	unsigned char	tranfer_block	: 1;
	unsigned char	en_auto_reall_r	: 1;
	unsigned char	en_auto_reall_w	: 1;	/* Byte 2 */
	unsigned char	rd_retry_count;		/* Byte 3 */
	unsigned char	correction_span;
	char	head_offset_count;
	char	data_strobe_offset;
	unsigned char	res;
	unsigned char	wr_retry_count;
	unsigned char	res_tape[2];
	unsigned char	recov_timelim[2];
};

struct scsi_mode_page_02 {		/* Device dis/re connect Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0E = 16 Bytes */
	unsigned char	buf_full_ratio;
	unsigned char	buf_empt_ratio;
	unsigned char	bus_inact_limit[2];
	unsigned char	disc_time_limit[2];
	unsigned char	conn_time_limit[2];
	unsigned char	max_burst_size[2];	/* Start SCSI-2 */
	unsigned char	data_tr_dis_ctl	: 2;
	unsigned char			: 6;
	unsigned char	res[3];
};

struct scsi_mode_page_03 {		/* Direct access format Paramters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x16 = 24 Bytes */
	unsigned char	trk_per_zone[2];
	unsigned char	alt_sec_per_zone[2];
	unsigned char	alt_trk_per_zone[2];
	unsigned char	alt_trk_per_vol[2];
	unsigned char	sect_per_trk[2];
	unsigned char	bytes_per_phys_sect[2];
	unsigned char	interleave[2];
	unsigned char	trk_skew[2];
	unsigned char	cyl_skew[2];
	unsigned char			: 3;
	unsigned char	inhibit_save	: 1;
	unsigned char	fmt_by_surface	: 1;
	unsigned char	removable	: 1;
	unsigned char	hard_sec	: 1;
	unsigned char	soft_sec	: 1;
	unsigned char	res[3];
};

struct scsi_mode_page_04 {		/* Rigid disk Geometry Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x16 = 24 Bytes */
	unsigned char	ncyl[3];
	unsigned char	nhead;
	unsigned char	start_precomp[3];
	unsigned char	start_red_wcurrent[3];
	unsigned char	step_rate[2];
	unsigned char	landing_zone[3];
	unsigned char	rot_pos_locking	: 2;	/* Start SCSI-2 */
	unsigned char			: 6;	/* Start SCSI-2 */
	unsigned char	rotational_off;
	unsigned char	res1;
	unsigned char	rotation_rate[2];
	unsigned char	res2[2];
};

struct scsi_mode_page_05 {		/* Flexible disk Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x1E = 32 Bytes */
	unsigned char	transfer_rate[2];
	unsigned char	nhead;
	unsigned char	sect_per_trk;
	unsigned char	bytes_per_phys_sect[2];
	unsigned char	ncyl[2];
	unsigned char	start_precomp[2];
	unsigned char	start_red_wcurrent[2];
	unsigned char	step_rate[2];
	unsigned char	step_pulse_width;
	unsigned char	head_settle_delay[2];
	unsigned char	motor_on_delay;
	unsigned char	motor_off_delay;
	unsigned char	spc		: 4;
	unsigned char			: 4;
	unsigned char			: 5;
	unsigned char	mo		: 1;
	unsigned char	ssn		: 1;
	unsigned char	trdy		: 1;
	unsigned char	write_compensation;
	unsigned char	head_load_delay;
	unsigned char	head_unload_delay;
	unsigned char	pin_2_use	: 4;
	unsigned char	pin_34_use	: 4;
	unsigned char	pin_1_use	: 4;
	unsigned char	pin_4_use	: 4;
	unsigned char	rotation_rate[2];
	unsigned char	res[2];
};

struct scsi_mode_page_07 {		/* Verify Error recovery */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0A = 12 Bytes */
	unsigned char	disa_correction	: 1;	/* Byte 2 */
	unsigned char	term_on_rec_err	: 1;
	unsigned char	report_rec_err	: 1;
	unsigned char	en_early_corr	: 1;
	unsigned char	res		: 4;	/* Byte 2 */
	unsigned char	ve_retry_count;		/* Byte 3 */
	unsigned char	ve_correction_span;
	char	res2[5];		/* Byte 5 */
	unsigned char	ve_recov_timelim[2];	/* Byte 10 */
};

struct scsi_mode_page_08 {		/* Caching Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0A = 12 Bytes */
	unsigned char	disa_rd_cache	: 1;	/* Byte 2 */
	unsigned char	muliple_fact	: 1;
	unsigned char	en_wt_cache	: 1;
	unsigned char	res		: 5;	/* Byte 2 */
	unsigned char	wt_ret_pri	: 4;	/* Byte 3 */
	unsigned char	demand_rd_ret_pri: 4;	/* Byte 3 */
	unsigned char	disa_pref_tr_len[2];	/* Byte 4 */
	unsigned char	min_pref[2];		/* Byte 6 */
	unsigned char	max_pref[2];		/* Byte 8 */
	unsigned char	max_pref_ceiling[2];	/* Byte 10 */
};

struct scsi_mode_page_09 {		/* Peripheral device Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* >= 0x06 = 8 Bytes */
	unsigned char	interface_id[2];	/* Byte 2 */
	unsigned char	res[4];			/* Byte 4 */
	unsigned char	vendor_specific[1];	/* Byte 8 */
};

struct scsi_mode_page_0A {		/* Common device Control Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x06 = 8 Bytes */
	unsigned char	rep_log_exeption: 1;	/* Byte 2 */
	unsigned char	res		: 7;	/* Byte 2 */
	unsigned char	dis_queuing	: 1;	/* Byte 3 */
	unsigned char	queuing_err_man	: 1;
	unsigned char	res2		: 2;
	unsigned char	queue_alg_mod	: 4;	/* Byte 3 */
	unsigned char	EAENP		: 1;	/* Byte 4 */
	unsigned char	UAENP		: 1;
	unsigned char	RAENP		: 1;
	unsigned char	res3		: 4;
	unsigned char	en_ext_cont_all	: 1;	/* Byte 4 */
	unsigned char	res4		: 8;
	unsigned char	ready_aen_hold_per[2];	/* Byte 6 */
};

struct scsi_mode_page_0B {		/* Medium Types Supported Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x06 = 8 Bytes */
	unsigned char	res[2];			/* Byte 2 */
	unsigned char	medium_one_supp;	/* Byte 4 */
	unsigned char	medium_two_supp;	/* Byte 5 */
	unsigned char	medium_three_supp;	/* Byte 6 */
	unsigned char	medium_four_supp;	/* Byte 7 */
};

struct scsi_mode_page_0C {		/* Notch & Partition Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x16 = 24 Bytes */
	unsigned char	res		: 6;	/* Byte 2 */
	unsigned char	logical_notch	: 1;
	unsigned char	notched_drive	: 1;	/* Byte 2 */
	unsigned char	res2;			/* Byte 3 */
	unsigned char	max_notches[2];		/* Byte 4  */
	unsigned char	active_notch[2];	/* Byte 6  */
	unsigned char	starting_boundary[4];	/* Byte 8  */
	unsigned char	ending_boundary[4];	/* Byte 12 */
	unsigned char	pages_notched[8];	/* Byte 16 */
};

struct scsi_mode_page_0D {		/* CD-ROM Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x06 = 8 Bytes */
	unsigned char	res;			/* Byte 2 */
	unsigned char	inact_timer_mult: 4;	/* Byte 3 */
	unsigned char	res2		: 4;	/* Byte 3 */
	unsigned char	s_un_per_m_un[2];	/* Byte 4  */
	unsigned char	f_un_per_s_un[2];	/* Byte 6  */
};

struct sony_mode_page_20 {		/* Sony Format Mode Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0A = 12 Bytes */
	unsigned char	format_mode;
	unsigned char	format_type;
#define	num_bands	user_band_size	/* Gilt bei Type 1 */
	unsigned char	user_band_size[4];	/* Gilt bei Type 0 */
	unsigned char	spare_band_size[2];
	unsigned char	res[2];
};

struct toshiba_mode_page_20 {		/* Toshiba Speed Control Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x01 = 3 Bytes */
	unsigned char	speed		: 1;
	unsigned char	res		: 7;
};

struct ccs_mode_page_38 {		/* CCS Caching Parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x0E = 14 Bytes */

	unsigned char	cache_table_size: 4;	/* Byte 3 */
	unsigned char	cache_en	: 1;
	unsigned char	res2		: 1;
	unsigned char	wr_index_en	: 1;
	unsigned char	res		: 1;	/* Byte 3 */
	unsigned char	threshold;		/* Byte 4 Prefetch threshold */
	unsigned char	max_prefetch;		/* Byte 5 Max. prefetch */
	unsigned char	max_multiplier;		/* Byte 6 Max. prefetch multiplier */
	unsigned char	min_prefetch;		/* Byte 7 Min. prefetch */
	unsigned char	min_multiplier;		/* Byte 8 Min. prefetch multiplier */
	unsigned char	res3[8];		/* Byte 9 */
};

struct cd_mode_page_05 {		/* write parameters */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x32 = 50 Bytes */
	unsigned char	write_type	: 4;	/* Session write type (PACKET/TAO...)*/
	unsigned char	test_write	: 1;	/* Do not actually write data	    */
	unsigned char	LS_V		: 1;	/* Link size valid		    */
	unsigned char	BUFE		: 1;	/* Enable Bufunderrun free rec.	    */
	unsigned char	res_2_7		: 1;
	unsigned char	track_mode	: 4;	/* Track mode (Q-sub control nibble) */
	unsigned char	copy		: 1;	/* 1st higher gen of copy prot track ~*/
	unsigned char	fp		: 1;	/* Fixed packed (if in packet mode) */
	unsigned char	multi_session	: 2;	/* Multi session write type	    */
	unsigned char	dbtype		: 4;	/* Data block type		    */
	unsigned char	res_4		: 4;	/* Reserved			    */
	unsigned char	link_size;		/* Link Size (default is 7)	    */
	unsigned char	res_6;			/* Reserved			    */
	unsigned char	host_appl_code	: 6;	/* Host application code of disk    */
	unsigned char	res_7		: 2;	/* Reserved			    */
	unsigned char	session_format;		/* Session format (DA/CDI/XA)	    */
	unsigned char	res_9;			/* Reserved			    */
	unsigned char	packet_size[4];		/* # of user datablocks/fixed packet */
	unsigned char	audio_pause_len[2];	/* # of blocks where index is zero  */
	unsigned char	media_cat_number[16];	/* Media catalog Number (MCN)	    */
	unsigned char	ISRC[14];		/* ISRC for this track		    */
	unsigned char	sub_header[4];
	unsigned char	vendor_uniq[4];
};

struct cd_wr_speed_performance {
	unsigned char	res0;			/* Reserved			    */
	unsigned char	rot_ctl_sel	: 2;	/* Rotational control selected	    */
	unsigned char	res_1_27	: 6;	/* Reserved			    */
	unsigned char	wr_speed_supp[2];	/* Supported write speed	    */
};

struct cd_mode_page_2A {		/* CD Cap / mech status */
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0x14 = 20 Bytes (MMC) */
	/* 0x18 = 24 Bytes (MMC-2) */
	/* 0x1C >= 28 Bytes (MMC-3) */
	unsigned char	cd_r_read	: 1;	/* Reads CD-R  media		    */
	unsigned char	cd_rw_read	: 1;	/* Reads CD-RW media		    */
	unsigned char	method2		: 1;	/* Reads fixed packet method2 media */
	unsigned char	dvd_rom_read	: 1;	/* Reads DVD ROM media		    */
	unsigned char	dvd_r_read	: 1;	/* Reads DVD-R media		    */
	unsigned char	dvd_ram_read	: 1;	/* Reads DVD-RAM media		    */
	unsigned char	res_2_67	: 2;	/* Reserved			    */
	unsigned char	cd_r_write	: 1;	/* Supports writing CD-R  media	    */
	unsigned char	cd_rw_write	: 1;	/* Supports writing CD-RW media	    */
	unsigned char	test_write	: 1;	/* Supports emulation write	    */
	unsigned char	res_3_3		: 1;	/* Reserved			    */
	unsigned char	dvd_r_write	: 1;	/* Supports writing DVD-R media	    */
	unsigned char	dvd_ram_write	: 1;	/* Supports writing DVD-RAM media   */
	unsigned char	res_3_67	: 2;	/* Reserved			    */
	unsigned char	audio_play	: 1;	/* Supports Audio play operation    */
	unsigned char	composite	: 1;	/* Deliveres composite A/V stream   */
	unsigned char	digital_port_2	: 1;	/* Supports digital output on port 2 */
	unsigned char	digital_port_1	: 1;	/* Supports digital output on port 1 */
	unsigned char	mode_2_form_1	: 1;	/* Reads Mode-2 form 1 media (XA)   */
	unsigned char	mode_2_form_2	: 1;	/* Reads Mode-2 form 2 media	    */
	unsigned char	multi_session	: 1;	/* Reads multi-session media	    */
	unsigned char	BUF		: 1;	/* Supports Buffer under. free rec. */
	unsigned char	cd_da_supported	: 1;	/* Reads audio data with READ CD cmd */
	unsigned char	cd_da_accurate	: 1;	/* READ CD data stream is accurate   */
	unsigned char	rw_supported	: 1;	/* Reads R-W sub channel information */
	unsigned char	rw_deint_corr	: 1;	/* Reads de-interleved R-W sub chan  */
	unsigned char	c2_pointers	: 1;	/* Supports C2 error pointers	    */
	unsigned char	ISRC		: 1;	/* Reads ISRC information	    */
	unsigned char	UPC		: 1;	/* Reads media catalog number (UPC) */
	unsigned char	read_bar_code	: 1;	/* Supports reading bar codes	    */
	unsigned char	lock		: 1;	/* PREVENT/ALLOW may lock media	    */
	unsigned char	lock_state	: 1;	/* Lock state 0=unlocked 1=locked   */
	unsigned char	prevent_jumper	: 1;	/* State of prev/allow jumper 0=pres */
	unsigned char	eject		: 1;	/* Ejects disc/cartr with STOP LoEj  */
	unsigned char	res_6_4		: 1;	/* Reserved			    */
	unsigned char	loading_type	: 3;	/* Loading mechanism type	    */
	unsigned char	sep_chan_vol	: 1;	/* Vol controls each channel separat */
	unsigned char	sep_chan_mute	: 1;	/* Mute controls each channel separat*/
	unsigned char	disk_present_rep: 1;	/* Changer supports disk present rep */
	unsigned char	sw_slot_sel	: 1;	/* Load empty slot in changer	    */
	unsigned char	side_change	: 1;	/* Side change capable		    */
	unsigned char	pw_in_lead_in	: 1;	/* Reads raw P-W sucode from lead in */
	unsigned char	res_7		: 2;	/* Reserved			    */
	unsigned char	max_read_speed[2];	/* Max. read speed in KB/s	    */
	unsigned char	num_vol_levels[2];	/* # of supported volume levels	    */
	unsigned char	buffer_size[2];		/* Buffer size for the data in KB   */
	unsigned char	cur_read_speed[2];	/* Current read speed in KB/s	    */
	unsigned char	res_16;			/* Reserved			    */
	unsigned char	res_17_0	: 1;	/* Reserved			    */
	unsigned char	BCK		: 1;	/* Data valid on falling edge of BCK */
	unsigned char	RCK		: 1;	/* Set: HIGH high LRCK=left channel  */
	unsigned char	LSBF		: 1;	/* Set: LSB first Clear: MSB first  */
	unsigned char	length		: 2;	/* 0=32BCKs 1=16BCKs 2=24BCKs 3=24I2c*/
	unsigned char	res_17		: 2;	/* Reserved			    */
	unsigned char	max_write_speed[2];	/* Max. write speed supported in KB/s*/
	unsigned char	cur_write_speed[2];	/* Current write speed in KB/s	    */

	/* Byte 22 ... Only in MMC-2	    */
	unsigned char	copy_man_rev[2];	/* Copy management revision supported*/
	unsigned char	res_24;			/* Reserved			    */
	unsigned char	res_25;			/* Reserved			    */

	/* Byte 26 ... Only in MMC-3	    */
	unsigned char	res_26;			/* Reserved			    */
	unsigned char	res_27_27	: 6;	/* Reserved			    */
	unsigned char	rot_ctl_sel	: 2;	/* Rotational control selected	    */
	unsigned char	v3_cur_write_speed[2];	/* Current write speed in KB/s	    */
	unsigned char	num_wr_speed_des[2];	/* # of wr speed perf descr. tables */
	struct cd_wr_speed_performance
		wr_speed_des[1];	/* wr speed performance descriptor  */
	/* Actually more (num_wr_speed_des) */
};

struct scsi_mode_data {
	struct scsi_mode_header		header;
	struct scsi_mode_blockdesc	blockdesc;
	union	pagex	{
		struct acb_mode_data		acb;
		struct scsi_mode_page_01	page1;
		struct scsi_mode_page_02	page2;
		struct scsi_mode_page_03	page3;
		struct scsi_mode_page_04	page4;
		struct scsi_mode_page_05	page5;
		struct scsi_mode_page_07	page7;
		struct scsi_mode_page_08	page8;
		struct scsi_mode_page_09	page9;
		struct scsi_mode_page_0A	pageA;
		struct scsi_mode_page_0B	pageB;
		struct scsi_mode_page_0C	pageC;
		struct scsi_mode_page_0D	pageD;
		struct sony_mode_page_20	sony20;
		struct toshiba_mode_page_20	toshiba20;
		struct ccs_mode_page_38		ccs38;
	} pagex;
};

struct scsi_capacity {
	__u32	c_baddr;		/* must convert byteorder!! */
	__u32	c_bsize;		/* must convert byteorder!! */
};

struct scsi_def_header {
	unsigned char		: 8;
	unsigned char	format	: 3;
	unsigned char	gdl	: 1;
	unsigned char	mdl	: 1;
	unsigned char		: 3;
	unsigned char	length[2];
};

struct scsi_format_header {
	unsigned char	res		: 8;	/* Adaptec 5500: 1 --> format track */
	unsigned char	vu		: 1;	/* Vendor Unique		    */
	unsigned char	immed		: 1;	/* Return Immediately from Format   */
	unsigned char	tryout		: 1;	/* Check if format parameters OK    */
	unsigned char	ipattern	: 1;	/* Init patter descriptor present   */
	unsigned char	serr		: 1;	/* Stop on error		    */
	unsigned char	dcert		: 1;	/* Disable certification	    */
	unsigned char	dmdl		: 1;	/* Disable manufacturer defect list */
	unsigned char	enable		: 1;	/* Enable to use the next 3 bits    */
	unsigned char	length[2];		/* Length of following list in bytes*/
};

struct	scsi_def_bfi {
	unsigned char	cyl[3];
	unsigned char	head;
	unsigned char	bfi[4];
};

struct	scsi_def_phys {
	unsigned char	cyl[3];
	unsigned char	head;
	unsigned char	sec[4];
};

struct	scsi_def_list {
	struct	scsi_def_header	hd;
	union {
		unsigned char		list_block[1][4];
		struct	scsi_def_bfi	list_bfi[1];
		struct	scsi_def_phys	list_phys[1];
	} def_list;
};

struct	scsi_format_data {
	struct scsi_format_header hd;
	union {
		unsigned char		list_block[1][4];
		struct	scsi_def_bfi	list_bfi[1];
		struct	scsi_def_phys	list_phys[1];
	} def_list;
};

struct scsi_format_cap_header {
	unsigned char	res[3];			/* Reserved			*/
	unsigned char	len;			/* Len (a multiple of 8)	*/
};

struct scsi_format_cap_desc {
	unsigned char	nblock[4];		/* Number of blocks		*/
	unsigned char	desc_type	: 2;	/* Descriptor type		*/
	unsigned char	fmt_type	: 6;	/* Format Type			*/
	unsigned char	blen[3];		/* Logical block length		*/
};

struct	scsi_cap_data {
	struct scsi_format_cap_header	hd;
	struct scsi_format_cap_desc	list[1];
};

struct	scsi_send_diag_cmd {
	unsigned char	cmd;
	unsigned char	addr[4];
	unsigned char		: 8;
};

struct	scsi_sector_header {
	unsigned char	cyl[2];
	unsigned char	head;
	unsigned char	sec;
	unsigned char		: 5;
	unsigned char	rp	: 1;
	unsigned char	sp	: 1;
	unsigned char	dt	: 1;
};
////////////////////////
struct ricoh_mode_page_30 {
	MP_P_CODE;		/* parsave & pagecode */
	unsigned char	p_len;			/* 0xE = 14 Bytes */
	unsigned char	BUEFS		:1;	/* Burn-Free supported	*/
	unsigned char	TWBFS		:1;	/* Test Burn-Free sup.	*/
	unsigned char	res_2_23	:2;
	unsigned char	ARSCS		:1;	/* Auto read speed control supp. */
	unsigned char	AWSCS		:1;	/* Auto write speed control supp. */
	unsigned char	res_2_67	:2;
	unsigned char	BUEFE		:1;	/* Burn-Free enabled	*/
	unsigned char	res_2_13	:3;
	unsigned char	ARSCE		:1;	/* Auto read speed control enabled */
	unsigned char	AWSCD		:1;	/* Auto write speed control disabled */
	unsigned char	res_3_67	:2;
	unsigned char	link_counter[2];	/* Burn-Free link counter */
	unsigned char	res[10];		/* Padding up to 16 bytes */
};

static unsigned char yellowbook_scrambler[2340] = {
	1,128,0,96,0,40,0,30,128,8,96,6,168,2,254,129,128,96,96,40,40,30,158,
	136,104,102,174,170,252,127,1,224,0,72,0,54,128,22,224,14,200,4,86,131,126,225,
	224,72,72,54,182,150,246,238,198,204,82,213,253,159,1,168,0,126,128,32,96,24,40,
	10,158,135,40,98,158,169,168,126,254,160,64,120,48,34,148,25,175,74,252,55,1,214,
	128,94,224,56,72,18,182,141,182,229,182,203,54,215,86,222,190,216,112,90,164,59,59,
	83,83,125,253,225,129,136,96,102,168,42,254,159,0,104,0,46,128,28,96,9,232,6,
	206,130,212,97,159,104,104,46,174,156,124,105,225,238,200,76,86,181,254,247,0,70,128,
	50,224,21,136,15,38,132,26,227,75,9,247,70,198,178,210,245,157,135,41,162,158,249,
	168,66,254,177,128,116,96,39,104,26,174,139,60,103,81,234,188,79,49,244,20,71,79,
	114,180,37,183,91,54,187,86,243,126,197,224,83,8,61,198,145,146,236,109,141,237,165,
	141,187,37,179,91,53,251,87,3,126,129,224,96,72,40,54,158,150,232,110,206,172,84,
	125,255,97,128,40,96,30,168,8,126,134,160,98,248,41,130,158,225,168,72,126,182,160,
	118,248,38,194,154,209,171,28,127,73,224,54,200,22,214,142,222,228,88,75,122,183,99,
	54,169,214,254,222,192,88,80,58,188,19,49,205,212,85,159,127,40,32,30,152,8,106,
	134,175,34,252,25,129,202,224,87,8,62,134,144,98,236,41,141,222,229,152,75,42,183,
	95,54,184,22,242,142,197,164,83,59,125,211,97,157,232,105,142,174,228,124,75,97,247,
	104,70,174,178,252,117,129,231,32,74,152,55,42,150,159,46,232,28,78,137,244,102,199,
	106,210,175,29,188,9,177,198,244,82,199,125,146,161,173,184,125,178,161,181,184,119,50,
	166,149,186,239,51,12,21,197,207,19,20,13,207,69,148,51,47,85,220,63,25,208,10,
	220,7,25,194,138,209,167,28,122,137,227,38,201,218,214,219,30,219,72,91,118,187,102,
	243,106,197,239,19,12,13,197,197,147,19,45,205,221,149,153,175,42,252,31,1,200,0,
	86,128,62,224,16,72,12,54,133,214,227,30,201,200,86,214,190,222,240,88,68,58,179,
	83,53,253,215,1,158,128,104,96,46,168,28,126,137,224,102,200,42,214,159,30,232,8,
	78,134,180,98,247,105,134,174,226,252,73,129,246,224,70,200,50,214,149,158,239,40,76,
	30,181,200,119,22,166,142,250,228,67,11,113,199,100,82,171,125,191,97,176,40,116,30,
	167,72,122,182,163,54,249,214,194,222,209,152,92,106,185,239,50,204,21,149,207,47,20,
	28,15,73,196,54,211,86,221,254,217,128,90,224,59,8,19,70,141,242,229,133,139,35,
	39,89,218,186,219,51,27,85,203,127,23,96,14,168,4,126,131,96,97,232,40,78,158,
	180,104,119,110,166,172,122,253,227,1,137,192,102,208,42,220,31,25,200,10,214,135,30,
	226,136,73,166,182,250,246,195,6,209,194,220,81,153,252,106,193,239,16,76,12,53,197,
	215,19,30,141,200,101,150,171,46,255,92,64,57,240,18,196,13,147,69,173,243,61,133,
	209,163,28,121,201,226,214,201,158,214,232,94,206,184,84,114,191,101,176,43,52,31,87,
	72,62,182,144,118,236,38,205,218,213,155,31,43,72,31,118,136,38,230,154,202,235,23,
	15,78,132,52,99,87,105,254,174,192,124,80,33,252,24,65,202,176,87,52,62,151,80,
	110,188,44,113,221,228,89,139,122,231,99,10,169,199,62,210,144,93,172,57,189,210,241,
	157,132,105,163,110,249,236,66,205,241,149,132,111,35,108,25,237,202,205,151,21,174,143,
	60,100,17,235,76,79,117,244,39,7,90,130,187,33,179,88,117,250,167,3,58,129,211,
	32,93,216,57,154,146,235,45,143,93,164,57,187,82,243,125,133,225,163,8,121,198,162,
	210,249,157,130,233,161,142,248,100,66,171,113,191,100,112,43,100,31,107,72,47,118,156,
	38,233,218,206,219,20,91,79,123,116,35,103,89,234,186,207,51,20,21,207,79,20,52,
	15,87,68,62,179,80,117,252,39,1,218,128,91,32,59,88,19,122,141,227,37,137,219,
	38,219,90,219,123,27,99,75,105,247,110,198,172,82,253,253,129,129,160,96,120,40,34,
	158,153,168,106,254,175,0,124,0,33,192,24,80,10,188,7,49,194,148,81,175,124,124,
	33,225,216,72,90,182,187,54,243,86,197,254,211,0,93,192,57,144,18,236,13,141,197,
	165,147,59,45,211,93,157,249,169,130,254,225,128,72,96,54,168,22,254,142,192,100,80,
	43,124,31,97,200,40,86,158,190,232,112,78,164,52,123,87,99,126,169,224,126,200,32,
	86,152,62,234,144,79,44,52,29,215,73,158,182,232,118,206,166,212,122,223,99,24,41,
	202,158,215,40,94,158,184,104,114,174,165,188,123,49,227,84,73,255,118,192,38,208,26,
	220,11,25,199,74,210,183,29,182,137,182,230,246,202,198,215,18,222,141,152,101,170,171,
	63,63,80,16,60,12,17,197,204,83,21,253,207,1,148,0,111,64,44,48,29,212,9,
	159,70,232,50,206,149,148,111,47,108,28,45,201,221,150,217,174,218,252,91,1,251,64,
	67,112,49,228,20,75,79,119,116,38,167,90,250,187,3,51,65,213,240,95,4,56,3,
	82,129,253,160,65,184,48,114,148,37,175,91,60,59,81,211,124,93,225,249,136,66,230,
	177,138,244,103,7,106,130,175,33,188,24,113,202,164,87,59,126,147,96,109,232,45,142,
	157,164,105,187,110,243,108,69,237,243,13,133,197,163,19,57,205,210,213,157,159,41,168,
	30,254,136,64,102,176,42,244,31,7,72,2,182,129,182,224,118,200,38,214,154,222,235,
	24,79,74,180,55,55,86,150,190,238,240,76,68,53,243,87,5,254,131,0,97,192,40,
	80,30,188,8,113,198,164,82,251,125,131,97,161,232,120,78,162,180,121,183,98,246,169,
	134,254,226,192,73,144,54,236,22,205,206,213,148,95,47,120,28,34,137,217,166,218,250,
	219,3,27,65,203,112,87,100,62,171,80,127,124,32,33,216,24,90,138,187,39,51,90,
	149,251,47,3,92,1,249,192,66,208,49,156,20,105,207,110,212,44,95,93,248,57,130,
	146,225,173,136,125,166,161,186,248,115,2,165,193,187,16,115,76,37,245,219,7,27,66,
	139,113,167,100,122,171,99,63,105,208,46,220,28,89,201,250,214,195,30,209,200,92,86,
	185,254,242,192,69,144,51,44,21,221,207,25,148,10,239,71,12,50,133,213,163,31,57,
	200,18,214,141,158,229,168,75,62,183,80,118,188,38,241,218,196,91,19,123,77,227,117,
	137,231,38,202,154,215,43,30,159,72,104,54,174,150,252,110,193,236,80,77,252,53,129,
	215,32,94,152,56,106,146,175,45,188,29,177,201,180,86,247,126,198,160,82,248,61,130,
	145,161,172,120,125,226,161,137,184,102,242,170,197,191,19,48,13,212,5,159,67,40,49,
	222,148,88,111,122,172,35,61,217,209,154,220,107,25,239,74,204,55,21,214,143,30,228,
	8,75,70,183,114,246,165,134,251,34,195,89,145,250,236,67,13,241,197,132,83,35,125,
	217,225,154,200,107,22,175,78,252,52,65,215,112,94,164,56,123,82,163,125,185,225,178,
	200,117,150,167,46,250,156,67,41,241,222,196,88,83,122,189,227,49,137,212,102,223,106,
	216,47,26,156,11,41,199,94,210,184,93,178,185,181,178,247,53,134,151,34,238,153,140,
	106,229,239,11,12,7,69,194,179,17,181,204,119,21,230,143,10,228,7,11,66,135,113,
	162,164,121,187,98,243,105,133,238,227,12,73,197,246,211,6,221,194,217,145,154,236,107,
	13,239,69,140,51,37,213,219,31,27,72,11,118,135,102,226,170,201,191,22,240,14,196,
	4,83,67,125,241,225,132,72,99,118,169,230,254,202,192,87,16,62,140,16,101,204,43,
	21,223,79,24,52,10,151,71,46,178,156,117,169,231,62,202,144,87,44,62,157,208,105,
	156,46,233,220,78,217,244,90,199,123,18,163,77,185,245,178,199,53,146,151,45,174,157,
	188,105,177,238,244,76,71,117,242,167,5,186,131,51,33,213,216,95,26,184,11,50,135,
	85,162,191,57,176,18,244,13,135,69,162,179,57,181,210,247,29,134,137,162,230,249,138,
	194,231,17,138,140,103,37,234,155,15,43,68,31,115,72,37,246,155,6,235,66,207,113,
	148,36,111,91,108,59,109,211,109,157,237,169,141,190,229,176,75,52,55,87,86,190,190,
	240,112,68,36,51,91,85,251,127,3,96,1,232,0,78,128,52,96,23,104,14,174,132,
	124,99,97,233,232,78,206,180,84,119,127,102,160,42,248,31,2,136,1,166,128,122,224,
	35,8,25,198,138,210,231,29,138,137,167,38,250,154,195,43,17,223,76,88,53,250,151,
	3,46,129,220,96,89,232,58,206,147,20,109,207,109,148,45,175,93,188,57,177,210,244,
	93,135,121,162,162,249,185,130,242,225,133,136,99,38,169,218,254,219,0,91,64,59,112,
	19,100,13,235,69,143,115,36,37,219,91,27,123,75,99,119,105,230,174,202,252,87,1,
	254,128,64,96,48,40,20,30,143,72,100,54,171,86,255,126,192,32,80,24,60,10,145,
	199,44,82,157,253,169,129,190,224,112,72,36,54,155,86,235,126,207,96,84,40,63,94,
	144,56,108,18,173,205,189,149,177,175,52,124,23,97,206,168,84,126,191,96,112,40,36,
	30,155,72,107,118,175,102,252,42,193,223,16,88,12,58,133,211,35,29,217,201,154,214,
	235,30,207,72,84,54,191,86,240,62,196,16,83,76,61,245,209,135,28,98,137,233,166,
	206,250,212,67,31,113,200,36,86,155,126,235,96,79,104,52,46,151,92,110,185,236,114,
	205,229,149,139,47,39,92,26,185,203,50,215,85,158,191,40,112,30,164,8,123,70,163,
	114,249,229,130,203,33,151,88,110,186,172,115,61,229,209,139,28,103,73,234,182,207,54,
	212,22,223,78,216,52,90,151,123,46,163,92,121,249,226,194,201,145,150,236,110,205,236,
	85,141,255,37,128,27,32,11,88,7,122,130,163,33,185,216,114,218,165,155,59,43,83,
	95,125,248,33,130,152,97,170,168,127,62,160,16,120,12,34,133,217,163,26,249,203,2,
	215,65,158,176,104,116,46,167,92,122,185,227,50,201,213,150,223,46,216,28,90,137,251,
	38,195,90,209,251,28,67,73,241,246,196,70,211,114,221,229,153,
};

/* little endian */
static __u32 yellowbook_scrambler_DWORD[2340/4] = {
	0x60008001UL,0x1E002800UL,0x06600880UL,0x81FE02A8UL,0x28606080UL,
	0x889E1E28UL,0xAAAE6668UL,0xE0017FFCUL,0x36004800UL,0x0EE01680UL,
	0x835604C8UL,0x48E0E17EUL,0x96B63648UL,0xCCC6EEF6UL,0x9FFDD552UL,
	0x7E00A801UL,0x18602080UL,0x879E0A28UL,0xA99E6228UL,0xA0FE7EA8UL,
	0x22307840UL,0x4AAF1994UL,0xD60137FCUL,0x38E05E80UL,0x8DB61248UL,
	0xCBB6E5B6UL,0xDE56D736UL,0x5A70D8BEUL,0x533B3BA4UL,0xE1FD7D53UL,
	0x66608881UL,0x9FFE2AA8UL,0x2E006800UL,0x09601C80UL,0x82CE06E8UL,
	0x689F61D4UL,0x9CAE2E68UL,0xEEE1697CUL,0xB5564CC8UL,0x4600F7FEUL,
	0x15E03280UL,0x84260F88UL,0x094BE31AUL,0xB2C646F7UL,0x879DF5D2UL,
	0xF99EA229UL,0xB1FE42A8UL,0x27607480UL,0x8BAE1A68UL,0xEA51673CUL,
	0xF4314FBCUL,0x724F4714UL,0x5BB725B4UL,0xF356BB36UL,0x53E0C57EUL,
	0x91C63D08UL,0x8D6DEC92UL,0xBB8DA5EDUL,0x355BB325UL,0x7E0357FBUL,
	0x4860E081UL,0x969E3628UL,0xACCE6EE8UL,0x61FF7D54UL,0x1E602880UL,
	0x867E08A8UL,0x29F862A0UL,0xA8E19E82UL,0xA0B67E48UL,0xC226F876UL,
	0x1CABD19AUL,0x36E0497FUL,0x8ED616C8UL,0x4B58E4DEUL,0x3663B77AUL,
	0xDEFED6A9UL,0x3A5058C0UL,0xCD3113BCUL,0x7F9F55D4UL,0x981E2028UL,
	0xAF866A08UL,0x8119FC22UL,0x0857E0CAUL,0x6290863EUL,0xDE8D29ECUL,
	0x2A4B98E5UL,0xB8365FB7UL,0xC58EF216UL,0x7D3B53A4UL,0xE89D61D3UL,
	0xE4AE8E69UL,0xF7614B7CUL,0xB2AE4668UL,0xE78175FCUL,0x37984A20UL,
	0x2E9F962AUL,0x894E1CE8UL,0x6AC766F4UL,0xBC1DAFD2UL,0xF4C6B109UL,
	0x927DC752UL,0x7DB8ADA1UL,0xB8B5A1B2UL,0x95A63277UL,0x0C33EFBAUL,
	0x13CFC515UL,0x45CF0D14UL,0x552F3394UL,0xD0193FDCUL,0x1907DC0AUL,
	0xA7D18AC2UL,0xE3897A1CUL,0xD6DAC926UL,0x48DB1EDBUL,0x66BB765BUL,
	0xEFC56AF3UL,0xC50D0C13UL,0x2D1393C5UL,0x9995DDCDUL,0x1FFC2AAFUL,
	0x5600C801UL,0x10E03E80UL,0x85360C48UL,0xC91EE3D6UL,0xBED656C8UL,
	0x4458F0DEUL,0x3553B33AUL,0x9E01D7FDUL,0x2E606880UL,0x897E1CA8UL,
	0x2AC866E0UL,0xE81E9FD6UL,0xB4864E08UL,0x8669F762UL,0x49FCE2AEUL,
	0x46E0F681UL,0x95D632C8UL,0x4C28EF9EUL,0x77C8B51EUL,0xFA8EA616UL,
	0x710B43E4UL,0xAB5264C7UL,0xB061BF7DUL,0xA71E7428UL,0xA3B67A48UL,
	0xC2D6F936UL,0x5C98D1DEUL,0x32EFB96AUL,0xCF9515CCUL,0x0F1C142FUL,
	0xD336C449UL,0xD9FEDD56UL,0x3BE05A80UL,0x8D461308UL,0x8B85E5F2UL,
	0xDA592723UL,0x1B33DBBAUL,0x177FCB55UL,0x04A80E60UL,0x6160837EUL,
	0x9E4E28E8UL,0x6E7768B4UL,0xFD7AACA6UL,0xC08901E3UL,0xDC2AD066UL,
	0x0AC8191FUL,0xE21E87D6UL,0xB6A64988UL,0x06C3F6FAUL,0x51DCC2D1UL,
	0xC16AFC99UL,0x0C4C10EFUL,0x13D7C535UL,0x65C88D1EUL,0xFF2EAB96UL,
	0xF039405CUL,0x930DC412UL,0x3DF3AD45UL,0x1CA3D185UL,0xD6E2C979UL,
	0xE8D69EC9UL,0x54B8CE5EUL,0xB065BF72UL,0x571F342BUL,0x90B63E48UL,
	0xCD26EC76UL,0x1F9BD5DAUL,0x761F482BUL,0x9AE62688UL,0x0F17EBCAUL,
	0x6334844EUL,0xAEFE6957UL,0x21507CC0UL,0xCA4118FCUL,0x3E3457B0UL,
	0xBC6E5097UL,0xE4DD712CUL,0xE77A8B59UL,0xC7A90A63UL,0x5D90D23EUL,
	0xD2BD39ACUL,0x69849DF1UL,0xECF96EA3UL,0x95F1CD42UL,0x6C236F84UL,
	0xCDCAED19UL,0x8FAE1597UL,0xEB11643CUL,0xF4754F4CUL,0x825A0727UL,
	0x58B321BBUL,0x03A7FA75UL,0x20D3813AUL,0x9A39D85DUL,0x8F2DEB92UL,
	0xBB39A45DUL,0x857DF352UL,0x7908A3E1UL,0xF9D2A2C6UL,0xA1E9829DUL,
	0x4264F88EUL,0x64BF71ABUL,0x1F642B70UL,0x762F486BUL,0xDAE9269CUL,
	0x5B14DBCEUL,0x23747B4FUL,0xBAEA5967UL,0x151433CFUL,0x34144FCFUL,
	0x3E44570FUL,0xFC7550B3UL,0x80DA0127UL,0x583B205BUL,0xE38D7A13UL,
	0x26DB8925UL,0x7BDB5ADBUL,0x694B631BUL,0xACC66EF7UL,0x81FDFD52UL,
	0x7860A081UL,0x999E2228UL,0xAFFE6AA8UL,0x21007C00UL,0x0A5018C0UL,
	0xC23107BCUL,0x7CAF5194UL,0xD8E1217CUL,0xBBB65A48UL,0xC556F336UL,
	0x5D00D3FEUL,0x129039C0UL,0xC58D0DECUL,0x2D3B93A5UL,0xF99D5DD3UL,
	0xE1FE82A9UL,0x36604880UL,0x8EFE16A8UL,0x2B5064C0UL,0xC8611F7CUL,
	0xBE9E5628UL,0xA44E70E8UL,0x63577B34UL,0x7EE0A97EUL,0x985620C8UL,
	0x4F90EA3EUL,0xD71D342CUL,0xE8B69E49UL,0xD4A6CE76UL,0x1863DF7AUL,
	0xD79ECA29UL,0xB89E5E28UL,0xA5AE7268UL,0xE3317BBCUL,0x76FF4954UL,
	0x1AD026C0UL,0xC7190BDCUL,0x1DB7D24AUL,0xE6B689B6UL,0xD7C6CAF6UL,
	0x988DDE12UL,0x3FABAA65UL,0x3C10503FUL,0xCCC5110CUL,0xCFFD1553UL,
	0x6F009401UL,0x1D302C40UL,0x469F09D4UL,0x95CE32E8UL,0x6C2F6F94UL,
	0xDDC92D1CUL,0xDAAED996UL,0xFB015BFCUL,0x31704340UL,0x4F4B14E4UL,
	0xA7267477UL,0x03BBFA5AUL,0xF0D54133UL,0x0338045FUL,0xA0FD8152UL,
	0x7230B841UL,0x5BAF2594UL,0xD3513B3CUL,0xF9E15D7CUL,0xB1E64288UL,
	0x0767F48AUL,0x21AF826AUL,0xCA7118BCUL,0x7E3B57A4UL,0xE86D6093UL,
	0xA49D8E2DUL,0xF36EBB69UL,0xF3ED456CUL,0xA3C5850DUL,0xD2CD3913UL,
	0x299F9DD5UL,0x88FE1EA8UL,0x2AB06640UL,0x48071FF4UL,0xB681B602UL,
	0x26C876E0UL,0xEBDE9AD6UL,0xB44A4F18UL,0x96563737UL,0x4CF0EEBEUL,
	0x57F33544UL,0x0083FE05UL,0x5028C061UL,0x7108BC1EUL,0xFB52A4C6UL,
	0xA161837DUL,0xA24E78E8UL,0x62B779B4UL,0xFE86A9F6UL,0x9049C0E2UL,
	0xCD16EC36UL,0x5F94D5CEUL,0x221C782FUL,0xDAA6D989UL,0x1B03DBFAUL,
	0x5770CB41UL,0x50AB3E64UL,0x21207C7FUL,0x8A5A18D8UL,0x5A3327BBUL,
	0x032FFB95UL,0xC0F9015CUL,0x9C31D042UL,0x6ECF6914UL,0x5D5F2CD4UL,
	0x928239F8UL,0x7D88ADE1UL,0xF8BAA1A6UL,0xC1A50273UL,0x4C7310BBUL,
	0x07DBF525UL,0x718B421BUL,0xAB7A64A7UL,0xD0693F63UL,0x591CDC2EUL,
	0xC3D6FAC9UL,0x5CC8D11EUL,0xF2FEB956UL,0x339045C0UL,0xCFDD152CUL,
	0xEF0A9419UL,0x85320C47UL,0x391FA3D5UL,0x8DD612C8UL,0x4BA8E59EUL,
	0x7650B73EUL,0xDAF126BCUL,0x7B135BC4UL,0x8975E34DUL,0x9ACA26E7UL,
	0x9F1E2BD7UL,0xAE366848UL,0xC16EFC96UL,0xFC4D50ECUL,0x20D78135UL,
	0x6A38985EUL,0xBC2DAF92UL,0xB4C9B11DUL,0xC67EF756UL,0x3DF852A0UL,
	0xACA19182UL,0xA1E27D78UL,0xF266B889UL,0x13BFC5AAUL,0x05D40D30UL,
	0x3128439FUL,0x6F5894DEUL,0x3D23AC7AUL,0xDC9AD1D9UL,0x4AEF196BUL,
	0xD61537CCUL,0x08E41E8FUL,0x72B7464BUL,0xFB86A5F6UL,0x9159C322UL,
	0x0D43ECFAUL,0x5384C5F1UL,0xE1D97D23UL,0x166BC89AUL,0x34FC4EAFUL,
	0x5E70D741UL,0x527B38A4UL,0xE1B97DA3UL,0x9675C8B2UL,0x9CFA2EA7UL,
	0xDEF12943UL,0x7A5358C4UL,0x8931E3BDUL,0x6ADF66D4UL,0x9C1A2FD8UL,
	0x5EC7290BUL,0xB25DB8D2UL,0xF7B2B5B9UL,0x22978635UL,0x6A8C99EEUL,
	0x0C0BEFE5UL,0xB3C24507UL,0x77CCB511UL,0x0A8FE615UL,0x420B07E4UL,
	0xA4A27187UL,0xF362BB79UL,0xE3EE8569UL,0xF6C5490CUL,0xC2DD06D3UL,
	0xEC9A91D9UL,0x45EF0D6BUL,0xD525338CUL,0x481B1FDBUL,0x6687760BUL,
	0xBFC9AAE2UL,0xC40EF016UL,0x7D435304UL,0x4884E1F1UL,0xE6A97663UL,
	0x57C0CAFEUL,0x108C3E10UL,0x152BCC65UL,0x34184FDFUL,0x2E47970AUL,
	0xA9759CB2UL,0x90CA3EE7UL,0x9D3E2C57UL,0x2E9C69D0UL,0xD94EDCE9UL,
	0x7BC75AF4UL,0xB94DA312UL,0x35C7B2F5UL,0xAE2D9792UL,0xB169BC9DUL,
	0x474CF4EEUL,0x05A7F275UL,0x213383BAUL,0x1A5FD8D5UL,0x87320BB8UL,
	0x39BFA255UL,0x0DF412B0UL,0xB3A24587UL,0xF7D2B539UL,0xA289861DUL,
	0xC28AF9E6UL,0x8C8A11E7UL,0x9BEA2567UL,0x1F442B0FUL,0xF6254873UL,
	0x42EB069BUL,0x249471CFUL,0x3B6C5B6FUL,0x9D6DD36DUL,0xBE8DA9EDUL,
	0x344BB0E5UL,0xBE565737UL,0x4470F0BEUL,0x555B3324UL,0x60037FFBUL,
	0x4E00E801UL,0x17603480UL,0x84AE0E68UL,0xE961637CUL,0xB4CE4EE8UL,
	0x667F7754UL,0x1FF82AA0UL,0xA6018802UL,0x23E07A80UL,0x8AC61908UL,
	0x8A1DE7D2UL,0xFA26A789UL,0x112BC39AUL,0x35584CDFUL,0x2E0397FAUL,
	0x5960DC81UL,0x93CE3AE8UL,0x6DCF6D14UL,0x5DAF2D94UL,0xD2B139BCUL,
	0x79875DF4UL,0xB9F9A2A2UL,0x85E1F282UL,0xA9266388UL,0x00DBFEDAUL,
	0x703B405BUL,0xEB0D6413UL,0x24738F45UL,0x1B5BDB25UL,0x77634B7BUL,
	0xCAAEE669UL,0xFE0157FCUL,0x30604080UL,0x8F1E1428UL,0xAB366448UL,
	0xC07EFF56UL,0x3C185020UL,0x2CC7910AUL,0xA9FD9D52UL,0x70E0BE81UL,
	0x9B362448UL,0xCF7EEB56UL,0x3F285460UL,0x6C38905EUL,0xBDCDAD12UL,
	0x34AFB195UL,0xCE61177CUL,0xBF7E54A8UL,0x24287060UL,0x6B489B1EUL,
	0xFC66AF76UL,0x10DFC12AUL,0x853A0C58UL,0xD91D23D3UL,0xEBD69AC9UL,
	0x5448CF1EUL,0xF056BF36UL,0x5310C43EUL,0xD1F53D4CUL,0x89621C87UL,
	0xFACEA6E9UL,0x711F43D4UL,0x9B5624C8UL,0x4F60EB7EUL,0x972E3468UL,
	0xECB96E5CUL,0x95E5CD72UL,0x5C272F8BUL,0x32CBB91AUL,0xBF9E55D7UL,
	0xA41E7028UL,0xA3467B08UL,0x82E5F972UL,0x589721CBUL,0x73ACBA6EUL,
	0x8BD1E53DUL,0xEA49671CUL,0xD436CFB6UL,0xD84EDF16UL,0x7B975A34UL,
	0x795CA32EUL,0xC9C2E2F9UL,0x6EEC9691UL,0x8D55ECCDUL,0x1B8025FFUL,
	0x07580B20UL,0x21A3827AUL,0xDA72D8B9UL,0x2B3B9BA5UL,0xF87D5F53UL,
	0x61988221UL,0x3E7FA8AAUL,0x0C7810A0UL,0xA3D98522UL,0x02CBF91AUL,
	0xB09E41D7UL,0xA72E7468UL,0xE3B97A5CUL,0x96D5C932UL,0x1CD82EDFUL,
	0x26FB895AUL,0xFBD15AC3UL,0xF149431CUL,0xD346C4F6UL,0x99E5DD72UL,

};

/*
 * Useful definitions for audio tracks
 */
#define	msample		(44100 * 2)		/* one 16bit audio sample */
#define	ssample		(msample * 2)		/* one stereo sample	*/
#define	samples(v)	((v) / ssample)		/* # of stereo samples	*/
#define	hsamples(v)	((v) / (ssample/100))	/* 100* # of stereo samples/s*/
#define	fsamples(v)	((v) / (ssample/75))	/* 75* # of stereo samples/s */

#define	minutes(v)	((int)(samples(v) / 60))
#define	seconds(v)	((int)(samples(v) % 60))
#define	hseconds(v)	((int)(hsamples(v) % 100))
#define	frames(v)	((int)(fsamples(v) % 75))

#define	MAX_SCG		(512-MAX_ATA)	//Max # of SCSI controllers
#define	MAX_ATA		13		//Max # of ATA devices
#define	MAX_TGT		16
#define	MAX_LUN		8

#define O_BINARY 		0

//scsidefs
/*
 * Tapes
 */
#define	DEV_MT02		100
#define	DEV_SC4000		101
/*
 * Printer
 */
#define	DEV_PRT			200

/*
 * Processors
 */
#define	DEV_PROC		300

/*
 * Worm
 */
#define	DEV_WORM		400
#define	DEV_RXT800S		401
/*
 * CD-ROM
 */
#define	DEV_CDROM		500
#define	DEV_MMC_CDROM		501
#define	DEV_MMC_CDR		502
#define	DEV_MMC_CDRW		503
#define	DEV_MMC_DVD		504
#define	DEV_MMC_DVD_WR		505

#define	DEV_CDD_521_OLD		510
#define	DEV_CDD_521		511
#define	DEV_CDD_522		512
#define	DEV_PCD_600		513
#define	DEV_CDD_2000		514
#define	DEV_CDD_2600		515
#define	DEV_TYUDEN_EW50		516
#define	DEV_YAMAHA_CDR_100	520
#define	DEV_YAMAHA_CDR_400	521
#define	DEV_PLASMON_RF_4100	530
#define	DEV_SONY_CDU_924	540
#define	DEV_RICOH_RO_1420C	550
#define	DEV_RICOH_RO_1060C	551
#define	DEV_TEAC_CD_R50S	560
#define	DEV_MATSUSHITA_7501	570
#define	DEV_MATSUSHITA_7502	571
#define	DEV_PIONEER_DW_S114X	580
#define	DEV_PIONEER_DVDR_S101	581

/*
 * Scanners
 */
#define	DEV_HRSCAN		600
#define	DEV_MS300A		601

/*
 * Optical memory
 */
#define	DEV_SONY_SMO		700

/*
 * SCSI Operation codes.
 */
#define	SC_TEST_UNIT_READY	0x00
#define	SC_REZERO_UNIT		0x01
#define	SC_REQUEST_SENSE	0x03
#define	SC_FORMAT		0x04
#define	SC_FORMAT_TRACK		0x06
#define	SC_REASSIGN_BLOCK	0x07		/* CCS only */
#define	SC_SEEK			0x0b
#define	SC_TRANSLATE		0x0f		/* ACB4000 only */
#define	SC_INQUIRY		0x12		/* CCS only */
#define	SC_MODE_SELECT		0x15
#define	SC_RESERVE		0x16
#define	SC_RELEASE		0x17
#define	SC_MODE_SENSE		0x1a
#define	SC_START		0x1b
#define	SC_READ_DEFECT_LIST	0x37		/* CCS only, group 1 */
#define	SC_READ_BUFFER		0x3c		/* CCS only, group 1 */

/*
	 * Note, these two commands use identical command blocks for all
	 * controllers except the Adaptec ACB 4000 which sets bit 1 of byte 1.
	 */
#define	SC_READ			0x08
#define	SC_WRITE		0x0a
#define	SC_EREAD		0x28		/* 10 byte read */
#define	SC_EWRITE		0x2a		/* 10 byte write */
#define	SC_WRITE_VERIFY		0x2e		/* 10 byte write+verify */
#define	SC_WRITE_FILE_MARK	0x10
#define	SC_UNKNOWN		0xff		/* cmd list terminator */

#define	SCG_RECV_DATA	0x0001		/* DMA direction to Sun */
#define	SCG_DISRE_ENA	0x0002		/* enable disconnect/reconnect */
#define	SCG_SILENT	0x0004		/* be silent on errors */
#define	SCG_CMD_RETRY	0x0008		/* enable retries */
#define	SCG_NOPARITY	0x0010		/* disable parity for this command */

#define	SC_G0_CDBLEN	6	/* Len of Group 0 commands */
#define	SC_G1_CDBLEN	10	/* Len of Group 1 commands */
#define	SC_G5_CDBLEN	12	/* Len of Group 5 commands */

#define	SCG_MAX_CMD	24	/* 24 bytes max. size is supported */
#define	SCG_MAX_STATUS	3	/* XXX (sollte 4 allign.) Mamimum Status Len */
#define	SCG_MAX_SENSE	32	/* Mamimum Sense Len for auto Req. Sense */

#define	DEF_SENSE_LEN	16	/* Default Sense Len */
#define	CCS_SENSE_LEN	18	/* Sense Len for CCS compatible devices */

#define	SCG_VERSION		0	/* libscg or transport version */
#define	SCG_AUTHOR		1	/* Author of above */
#define	SCG_SCCS_ID		2	/* SCCS id of above */
#define	SCG_RVERSION		10	/* Remote transport version */
#define	SCG_RAUTHOR		11	/* Remote transport author */
#define	SCG_RSCCS_ID		12	/* Remote transport SCCS ID */
#define	SCG_KVERSION		20	/* Kernel transport version */

#define	SCG_NO_ERROR	0		//cdb transported without error
//SCG_NO_ERROR incudes all commands
//where the SCSI status is valid

#define	SCG_RETRYABLE	1		//any other case e.g. SCSI bus busy
//SCSI cdb could not be send,
//includes DMA errors other than
//DMA underrun

#define	SCG_FATAL	2			//could not select target
#define	SCG_TIMEOUT	3		//driver timed out

/*
 * Helpers for the error buffer in SCSI*
 */
#define	scg_errsize(scgp)	((scgp)->errptr - (scgp)->errstr)
#define	scg_errrsize(scgp)	(SCSI_ERRSTR_SIZE - scg_errsize(scgp))

#define	SCGF_PERM_EXIT	0x01	/* Exit on permission problems */

#define	SCG_RESET_NOP		0	/* Test if reset is supported */
#define	SCG_RESET_TGT		1	/* Reset Target only */
#define	SCG_RESET_BUS		2	/* Reset complete SCSI Bus */

/*
 * Drive specific flags for struct SCSI:
 */
#define	DRF_MODE_DMA_OVR	0x0001		/* Drive gives DMA overrun */
						/* on mode sense	   */

#define	SCSI_ERRSTR_SIZE	4096

#define	ST_VU_00	0x01	/* Vendor unique		*/
#define	ST_CHK_COND	0x02	/* Check condition		*/
#define	ST_COND_MET	0x04	/* Condition met		*/
#define	ST_BUSY		0x08	/* Busy				*/
#define	ST_IS_SEND	0x10	/* Intermediate status send	*/
#define	ST_VU_05	0x20	/* Vendor unique		*/
#define	ST_VU_06	0x40	/* Vendor unique		*/
#define	ST_RSVD_07	0x80	/* Reserved			*/

//Sense key values for extended sense.
#define	SC_NO_SENSE		0x00
#define	SC_RECOVERABLE_ERROR	0x01
#define	SC_NOT_READY		0x02
#define	SC_MEDIUM_ERROR		0x03
#define	SC_HARDWARE_ERROR	0x04
#define	SC_ILLEGAL_REQUEST	0x05
#define	SC_UNIT_ATTENTION	0x06
#define	SC_WRITE_PROTECT	0x07
#define	SC_BLANK_CHECK		0x08
#define	SC_VENDOR_UNIQUE	0x09
#define	SC_COPY_ABORTED		0x0A
#define	SC_ABORTED_COMMAND	0x0B
#define	SC_EQUAL		0x0C
#define	SC_VOLUME_OVERFLOW	0x0D
#define	SC_MISCOMPARE		0x0E
#define	SC_RESERVED		0x0F

//Messages that SCSI can send.
#define	SC_COMMAND_COMPLETE	0x00
#define	SC_SYNCHRONOUS		0x01
#define	SC_SAVE_DATA_PTR	0x02
#define	SC_RESTORE_PTRS		0x03
#define	SC_DISCONNECT		0x04
#define	SC_ABORT		0x06
#define	SC_MSG_REJECT		0x07
#define	SC_NO_OP		0x08
#define	SC_PARITY		0x09
#define	SC_IDENTIFY		0x80
#define	SC_DR_IDENTIFY		0xc0
#define	SC_DEVICE_RESET		0x0c

#define	scg_scsibus(scgp)	(scgp)->m_Addr.scsibus
#define	scg_target(scgp)	(scgp)->m_Addr.target
#define	scg_lun(scgp)		(scgp)->m_Addr.lun

typedef	struct disk_status	dstat_t;
typedef	off_t	tsize_t;

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional

#endif