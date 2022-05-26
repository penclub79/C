/*
	cdrecord class
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrSCSI
#define	_EPC_GrSCSI

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
#include 	<Hw/Odd/GrScsiReg.h>
//#include 	<linux/types.h>

//====================================================================
//constance
/*
 * Useful definitions for audio tracks
 */
#define	msample		(44100 * 2)		/* one 16bit audio sample */
#define	ssample		(msample * 2)		/* one stereo sample	*/
#define	samples(v)	((v) / ssample)		/* # of stereo samples	*/
#define	hsamples(v)	((v) / (ssample/100))	/* 100* # of stereo samples/s*/
#define	fsamples(v)	((v) / (ssample/75))	/* 75* # of stereo samples/s */

#define	minutes(v)	((__s32)(samples(v) / 60))
#define	seconds(v)	((__s32)(samples(v) % 60))
#define	hseconds(v)	((__s32)(hsamples(v) % 100))
#define	frames(v)	((__s32)(fsamples(v) % 75))

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

#define	scg_scsibus(scgp)	(scgp)->addr.scsibus
#define	scg_target(scgp)	(scgp)->addr.target
#define	scg_lun(scgp)		(scgp)->addr.lun

//scgcmd.h
#define	dma_read	flags		/* 1 if DMA to Sun, 0 otherwise */

#define	scsi_status	scg_status
#define	scsi_sense	scg_sense
#define	scsi_ext_sense	scg_ext_sense

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

//scsicdb.h


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

#	define	__TOP_4BYTE	0xFF000000UL

#define	a_to_u_4_byte(a)	((__u32) \
				((((unsigned __s8*) a)[3]		& 0xFF) | \
				    (((unsigned __s8*) a)[2] << 8	& 0xFF00) | \
				    (((unsigned __s8*) a)[1] << 16	& 0xFF0000) | \
				    (((unsigned __s8*) a)[0] << 24	& __TOP_4BYTE)))
#define	a_to_4_byte(a)		(long)(__u32)a_to_u_4_byte(a)

#define	i_to_2_byte(a, i)	(((unsigned __s8 *)(a))[0] = ((i) >> 8) & 0xFF,\
				    ((unsigned __s8 *)(a))[1] = (i) & 0xFF)

#define	i_to_3_byte(a, i)	(((unsigned __s8 *)(a))[0] = ((i) >> 16)& 0xFF,\
				    ((unsigned __s8 *)(a))[1] = ((i) >> 8) & 0xFF,\
				    ((unsigned __s8 *)(a))[2] = (i) & 0xFF)

#define	i_to_4_byte(a, i)	(((unsigned __s8 *)(a))[0] = ((i) >> 24)& 0xFF,\
				    ((unsigned __s8 *)(a))[1] = ((i) >> 16)& 0xFF,\
				    ((unsigned __s8 *)(a))[2] = ((i) >> 8) & 0xFF,\
				    ((unsigned __s8 *)(a))[3] = (i) & 0xFF)



#define	a_to_byte(a)		(((__u8 *) a)[0])

#define	a_to_u_byte(a)		((__u8) \
				(((unsigned __s8 *) a)[0]		& 0xFF))

#define	a_to_u_2_byte(a)	((unsigned __s16) \
				((((unsigned __s8 *) a)[1]		& 0xFF) | \
				    (((unsigned __s8 *) a)[0] << 8	& 0xFF00)))

#define	a_to_2_byte(a)		(__s32)(unsigned __s16)a_to_u_2_byte(a)

#define	a_to_u_3_byte(a)	((__u32) \
				((((unsigned __s8 *) a)[2]		& 0xFF) | \
				    (((unsigned __s8 *) a)[1] << 8	& 0xFF00) | \
				    (((unsigned __s8 *) a)[0] << 16	& 0xFF0000)))

#define	MP_P_CODE			\
	unsigned __s8	p_code		: 6;	\
	unsigned __s8	p_res		: 1;	\
	unsigned __s8	parsave		: 1

#define	g1_cdblen(cdb, len)	((cdb)->count[0]    = ((len) >> 8) & 0xFF,\
				    (cdb)->count[1] = (len) & 0xFF)

//====================================================================
//global macro
typedef	struct cdr_cmd	cdr_t;
typedef	struct disk_status	dstat_t;
typedef	off_t	tsize_t;

//====================================================================
//global type
typedef	struct St_GrScsiInterface	St_GrScsiIf;

typedef	__s32	(*scg_cb_t)	(void *);

typedef struct {
	__s32	scsibus;	/* SCSI bus #    for next I/O		*/
	__s32	target;		/* SCSI target # for next I/O		*/
	__s32	lun;		/* SCSI lun #    for next I/O		*/
} scg_addr_t;

struct	scsi_status {
	unsigned __s8 vu_00	: 1;	/* vendor unique */
	unsigned __s8 chk	: 1;	/* check condition: sense data available */
	unsigned __s8 cm	: 1;	/* condition met */
	unsigned __s8 busy	: 1;	/* device busy or reserved */
	unsigned __s8 is	: 1;	/* intermediate status sent */
	unsigned __s8 vu_05	: 1;	/* vendor unique */
	#define	st_scsi2	vu_05	/* SCSI-2 modifier bit */
	unsigned __s8 vu_06	: 1;	/* vendor unique */
	unsigned __s8 st_rsvd	: 1;	/* reserved */
	#define	ext_st1	st_rsvd		/* extended status (next byte valid) */
	/* byte 1 */
	unsigned __s8 ha_er	: 1;	/* host adapter detected error */
	unsigned __s8 reserved: 6;	/* reserved */
	unsigned __s8 ext_st2	: 1;	/* extended status (next byte valid) */
	/* byte 2 */
	unsigned __s8 byte2;		/* third byte */
};

struct	scsi_sense {		/* scsi sense for error classes 0-6 */
	unsigned __s8	code	: 7;	/* error class/code */
	unsigned __s8	adr_val	: 1;	/* sense data is valid */
	unsigned __s8	high_addr;	/* high byte of block addr */
	unsigned __s8	mid_addr;	/* middle byte of block addr */
	unsigned __s8	low_addr;	/* low byte of block addr */
};

//scsicdb.h
struct	scsi_g0cdb {		/* scsi group 0 command description block */
	unsigned __s8	cmd;		/* command code */
	unsigned __s8	high_addr : 5;	/* high part of block address */
	unsigned __s8	lun	  : 3;	/* logical unit number */
	unsigned __s8	mid_addr;	/* middle part of block address */
	unsigned __s8	low_addr;	/* low part of block address */
	unsigned __s8	count;		/* transfer length */
	unsigned __s8	link	  : 1;	/* link (another command follows) */
	unsigned __s8	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned __s8	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned __s8	rsvd	  : 3;	/* reserved */
	unsigned __s8	vu_56	  : 1;	/* vendor unique (byte 5 bit 6) */
	unsigned __s8	vu_57	  : 1;	/* vendor unique (byte 5 bit 7) */
};

struct	scsi_g1cdb {		/* scsi group 1 command description block */
	unsigned __s8	cmd;		/* command code */
	unsigned __s8	reladr	  : 1;	/* address is relative */
	unsigned __s8	res	  : 4;	/* reserved bits 1-4 of byte 1 */
	unsigned __s8	lun	  : 3;	/* logical unit number */
	unsigned __s8	addr[4];	/* logical block address */
	unsigned __s8	res6;		/* reserved byte 6 */
	unsigned __s8	count[2];	/* transfer length */
	unsigned __s8	link	  : 1;	/* link (another command follows) */
	unsigned __s8	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned __s8	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned __s8	rsvd	  : 3;	/* reserved */
	unsigned __s8	vu_96	  : 1;	/* vendor unique (byte 5 bit 6) */
	unsigned __s8	vu_97	  : 1;	/* vendor unique (byte 5 bit 7) */
};

struct	scsi_g5cdb {		/* scsi group 5 command description block */
	unsigned __s8	cmd;		/* command code */
	unsigned __s8	reladr	  : 1;	/* address is relative */
	unsigned __s8	res	  : 4;	/* reserved bits 1-4 of byte 1 */
	unsigned __s8	lun	  : 3;	/* logical unit number */
	unsigned __s8	addr[4];	/* logical block address */
	unsigned __s8	count[4];	/* transfer length */
	unsigned __s8	res10;		/* reserved byte 10 */
	unsigned __s8	link	  : 1;	/* link (another command follows) */
	unsigned __s8	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned __s8	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned __s8	rsvd	  : 3;	/* reserved */
	unsigned __s8	vu_B6	  : 1;	/* vendor unique (byte B bit 6) */
	unsigned __s8	vu_B7	  : 1;	/* vendor unique (byte B bit 7) */
};

//scsireg.h

typedef struct scg_ops
{
	__s32	(*scgo_send)(St_GrScsiIf *scgp);

	__s8 *(*scgo_version)		(St_GrScsiIf *scgp, __s32 what);
	__s32	(*scgo_help)		(St_GrScsiIf *scgp, FILE *f);
	__s32	(*scgo_open)		(St_GrScsiIf *scgp, __s8 *device);
	__s32	(*scgo_close)		(St_GrScsiIf *scgp);
	long	(*scgo_maxdma)		(St_GrScsiIf *scgp, long amt);
	void *(*scgo_getbuf)		(St_GrScsiIf *scgp, long amt);
	void	(*scgo_freebuf)		(St_GrScsiIf *scgp);


	__s32	(*scgo_numbus)		(St_GrScsiIf *scgp);
	BOOL8	(*scgo_havebus)		(St_GrScsiIf *scgp, __s32 busno);
	__s32	(*scgo_fileno)		(St_GrScsiIf *scgp, __s32 busno, __s32 tgt, __s32 tlun);
	__s32	(*scgo_initiator_id)	(St_GrScsiIf *scgp);
	//__s32	(*scgo_isatapi)		(St_GrScsiIf *scgp);
	__s32	(*scgo_reset)		(St_GrScsiIf *scgp, __s32 what);
} scg_ops_t;

struct	scg_cmd
{
	__s8*	addr;	
	//caddr_t	addr;		//Address of data in user space
	__s32	size;				//DMA count for data transfer
	__s32	flags;			//see below for definition
	__s32	cdb_len;		//Size of SCSI command in bytes
	__s32	sense_len;	//for intr() if -1 don't get sense
	__s32	timeout;		//timeout in seconds
	__s32	resid;			//Bytes not transfered
	__s32	error;			//Error code from scgintr()
	__s32	ux_errno;		//UNIX error code
	union
	{
		struct	scsi_status Scb; // Status returnd by command
		unsigned __s8 cmd_scb[SCG_MAX_STATUS];
	} u_scb;
#define	scb	u_scb.Scb

	union {
		struct	scsi_sense Sense; /* Sense bytes from command */
		unsigned __s8 cmd_sense[SCG_MAX_SENSE];
	} u_sense;
#define	sense	u_sense.Sense
	__s32	sense_count;		//Number of bytes valid in sense
	__s32	target;					//SCSI target id
	union
	{
		struct	scsi_g0cdb g0_cdb;
		struct	scsi_g1cdb g1_cdb;
		struct	scsi_g5cdb g5_cdb;
		unsigned __s8 cmd_cdb[SCG_MAX_CMD];
	} cdb;				//24 bytes max. size is supported
};

//scsisence.h
struct	scsi_ext_sense
{
	/* byte 0 */
	unsigned __s8	type	: 7;	/* fixed at 0x70 */
	unsigned __s8	adr_val	: 1;	/* sense data is valid */
	/* byte 1 */
	unsigned __s8	seg_num;	/* segment number, applies to copy cmd only */
	/* byte 2 */
	unsigned __s8	key	: 4;	/* sense key, see below */
	unsigned __s8		: 1;	/* reserved */
	unsigned __s8	ili	: 1;	/* incorrect length indicator */
	unsigned __s8	eom	: 1;	/* end of media */
	unsigned __s8	fil_mk	: 1;	/* file mark on device */
	/* bytes 3 through 7 */
	unsigned __s8	info_1;		/* information byte 1 */
	unsigned __s8	info_2;		/* information byte 2 */
	unsigned __s8	info_3;		/* information byte 3 */
	unsigned __s8	info_4;		/* information byte 4 */
	unsigned __s8	add_len;	/* number of additional bytes */
	/* bytes 8 through 13, CCS additions */
	unsigned __s8	optional_8;	/* CCS search and copy only */
	unsigned __s8	optional_9;	/* CCS search and copy only */
	unsigned __s8	optional_10;	/* CCS search and copy only */
	unsigned __s8	optional_11;	/* CCS search and copy only */
	unsigned __s8 	sense_code;	/* sense code */
	unsigned __s8	qual_code;	/* sense code qualifier */
	unsigned __s8	fru_code;	/* Field replacable unit code */
	unsigned __s8	bptr	: 3;	/* bit pointer for failure (if bpv) */
	unsigned __s8	bpv	: 1;	/* bit pointer is valid */
	unsigned __s8		: 2;
	unsigned __s8	cd	: 1;	/* pointers refer to command not data */
	unsigned __s8	sksv	: 1;	/* sense key specific valid */
	unsigned __s8	field_ptr[2];	/* field pointer for failure */
	unsigned __s8	add_info[2];	/* round up to 20 bytes */
};

typedef	struct St_GrScgoLocal
{
	__s32	scgfile;		/* Used for SG_GET_BUFSIZE ioctl()*/
	__s16	scgfiles[MAX_SCG + MAX_ATA][MAX_TGT][MAX_LUN];
	__s16	buscookies[MAX_SCG + MAX_ATA];
	__s32	pgbus;
	__s32	pack_id;		/* Should be a random number	*/
	__s32	drvers;
	__s16	isold;
	//__s16	flags;
	long	xbufsize;
	__s8	*xbuf;
	__s8	*SCSIbuf;
}	*Ptr_GrScgoLocal;

typedef	struct St_GrScsiInterface 
{
	scg_ops_t *ops;		/* Ptr to low level SCSI transport ops	*/
	__s32	fd;		/* File descriptor for next I/O		*/
	scg_addr_t	addr;	/* SCSI address for next I/O		*/
	__s32	dflags;		/* Drive specific flags (see below)	*/
	__s32	overbose;	/* Be verbose in open() if value > 0	*/
	__s32	deftimeout;
	__s32	dev;		/* from scsi_cdr.c			*/
	struct scg_cmd scmd;
	__s8	*cmdname;
	__s8	*curcmdname;
	BOOL8	running;

	long	maxdma;		/* Max DMA limit for this open instance	*/
	long	maxbuf;		/* Cur DMA buffer limit for this inst.	*/
				/* This is the size behind bufptr	*/
	struct timeval	cmdstart;
	struct timeval	cmdstop;
	St_GrScgoLocal	local;
	//void	*local;		/* Local data from the low level code	*/
	void	*bufbase;	/* needed for scsi_freebuf()		*/
	void	*bufptr;	/* DMA buffer pointer for appl. use	*/
	__s8	errstr[SCSI_ERRSTR_SIZE];		/* Error string for scsi_open/sendmcd	*/
	__s8	*errbeg;	/* Pointer to begin of not flushed data	*/
	__s8	*errptr;	/* Actual write pointer into errstr	*/

	struct scsi_inquiry inq;
	struct scsi_capacity cap;
}	*Ptr_GrScsiInterface;

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
	__s32	isecsize;	/* Read input sector size for this track*/
	__s32	secsize;	/* Sector size for this track (bytes)	*/
	__s32	secspt;		/* # of sectors to copy for one transfer*/
	__s32	pktsize;	/* # of blocks per write packet		*/
	unsigned __s8	dataoff;	/* offset of user data in raw sector	*/
	unsigned __s8	tracks;		/* Number of tracks on this disk	*/
	unsigned __s8	track;		/* Track # as offset in track[] array	*/
	unsigned __s8	trackno;	/* Track # on disk for this track	*/
	unsigned __s8	tracktype;	/* Track type (toc type)		*/
	unsigned __s8	dbtype;		/* Data block type for this track	*/
	__s32	sectype;	/* Sector type				*/
	__s32	flags;		/* Flags (see below)			*/
	__s32	nindex;		/* Number of indices for track		*/
	long	*tindex;	/* Track index descriptor		*/
	__s8	*isrc;		/* ISRC code for this track / disk MCN	*/
	void	*text;		/* Opaque CD-Text data (txtptr_t *)	*/
} track_t, *Ptr_GrOddTrack;

struct disk_status {
	track_t	*ds_trackp;		/* Pointer to track structure	*/
	__u32 ds_diskid;		/* Disk identification		*/
	__u16 ds_cdrflags;		/* Recording flags from cdrecord*/
	__u16 ds_flags;		/* Disk_status flags		*/
	unsigned __s8	 ds_wrmode;		/* Selected write mode		*/
	unsigned __s8	 ds_type;		/* Abstract disk type		*/

	unsigned __s8	 ds_disktype;		/* Disk type (from TOC/PMA)	*/
	unsigned __s8	 ds_diskstat;		/* Disk status (MMC)		*/
	unsigned __s8	 ds_sessstat;		/* Status of last sesion (MMC)	*/
	unsigned __s8	 ds_trfirst;		/* first track #		*/
	unsigned __s8	 ds_trlast;		/* last track #			*/
	unsigned __s8	 ds_trfirst_ls;		/* first track # in last session*/
	unsigned __s8	 ds_barcode[8];		/* Disk bar code		*/

	__s32		 ds_first_leadin;	/* Start of first lead in (ATIP)*/
	__s32		 ds_last_leadout;	/* Start of last lead out (ATIP)*/
	__s32		 ds_curr_leadin;	/* Start of next lead in	*/
	__s32		 ds_curr_leadout;	/* Start of next lead out	*/

	__s32		 ds_maxblocks;		/* # of official blocks on disk	*/
	__s32		 ds_maxrblocks;		/* # real blocks on disk	*/
	__s32		 ds_fwa;		/* first writable addr		*/

	__s32		 ds_startsec;		/* Actual start sector		*/
	__s32		 ds_endsec;		/* Actual end sector		*/
	__s32		 ds_buflow;		/* # of times drive buffer empty*/

	unsigned __s16 ds_minbuf;		/* Minimum drive bufer fill rt.	*/

	unsigned __s16 ds_at_min_speed;	/* The minimal ATIP write speed	*/
	unsigned __s16 ds_at_max_speed;	/* The maximal ATIP write speed	*/
	unsigned __s16 ds_dr_cur_rspeed;	/* The drive's cur read speed	*/
	unsigned __s16 ds_dr_max_rspeed;	/* The drive's max read speed	*/
	unsigned __s16 ds_dr_cur_wspeed;	/* The drive's cur write speed	*/
	unsigned __s16 ds_dr_max_wspeed;	/* The drive's max write speed	*/
	unsigned __s16 ds_wspeed;		/* The selected/drive wr. speed */

	__s32		 ds_layer_break;	/* Start of layer break		*/
};

struct cdr_cmd {
	__s32	cdr_dev;			/* Numerical device type */
	__u32 cdr_cmdflags;			/* Command line options */
	__u32 cdr_cmdflags2;			/* More cmdline options */
	__u32 cdr_flags;			/* Drive related flags	*/
	__u32 cdr_flags2;			/* More Drive flags	*/
	unsigned __s8	 cdr_cdrw_support;		/* CD-RW write media types */
	unsigned __s8	 cdr_wrmodedef;			/* Default write mode	*/
	unsigned __s16 cdr_speeddef;			/* Default write speed	*/
	unsigned __s16 cdr_speedmax;			/* Max. write speed	*/

	__s8	*cdr_drname;			/* Driver ID string	*/
	__s8	*cdr_drtext;			/* Driver ID text	*/
	struct cd_mode_page_2A *cdr_cdcap;
	dstat_t	*cdr_dstat;

	cdr_t	*(*cdr_identify)(St_GrScsiIf *scgp, cdr_t * );	/* identify drive */

	__s32	(*cdr_attach)		(St_GrScsiIf *scgp, cdr_t *);			/* init error decoding etc*/
	__s32	(*cdr_init)		(St_GrScsiIf *scgp, cdr_t *);			/* init drive to useful deflts */
	__s32	(*cdr_getdisktype)	(St_GrScsiIf *scgp, cdr_t *);			/* get disk type */
	__s32	(*cdr_prdiskstatus)	(St_GrScsiIf *scgp, cdr_t *);			/* print disk status */
	__s32	(*cdr_load)		(St_GrScsiIf *scgp, cdr_t *);			/* load disk */
	__s32	(*cdr_unload)		(St_GrScsiIf *scgp, cdr_t *);			/* unload disk */
	__s32	(*cdr_buffer_cap)	(St_GrScsiIf *scgp, long *sizep, long *freep);	/* read buffer capacity */
	__s32	(*cdr_check_recovery)	(St_GrScsiIf *scgp, cdr_t *);			/* check if recover is needed */
	__s32	(*cdr_recover)		(St_GrScsiIf *scgp, cdr_t *, __s32 track);		/* do recover */
	__s32	(*cdr_set_speed_dummy)	(St_GrScsiIf *scgp, cdr_t *, __s32 *speedp);	/* set recording speed & dummy write */
	__s32	(*cdr_set_secsize)	(St_GrScsiIf *scgp, __s32 secsize);		/* set sector size */
	__s32	(*cdr_next_wr_address)	(St_GrScsiIf *scgp, track_t *trackp, long *ap);	/* get next writable addr. */
	__s32	(*cdr_reserve_track)	(St_GrScsiIf *scgp, __u32 len);			/* reserve track for future use */
	__s32	(*cdr_write_trackdata)	(St_GrScsiIf *scgp, caddr_t buf, long daddr, long bytecnt, __s32 seccnt, BOOL8 islast);
	__s32	(*cdr_gen_cue)		(track_t *trackp, void *cuep, BOOL8 needgap);		/* generate cue sheet */
	__s32	(*cdr_send_cue)		(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* send cue sheet */
	__s32	(*cdr_write_leadin)	(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* write leadin */
	__s32	(*cdr_open_track)	(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* open new track */
	__s32	(*cdr_close_track)	(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* close written track */
	__s32	(*cdr_open_session)	(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* open new session */
	__s32	(*cdr_close_session)	(St_GrScsiIf *scgp, cdr_t *);			/* really needed ??? */
	__s32	(*cdr_abort_session)	(St_GrScsiIf *scgp, cdr_t *);			/* abort current write */
	__s32	(*cdr_session_offset)	(St_GrScsiIf *scgp, long *soff);			/* read session offset*/
	__s32	(*cdr_fixate)		(St_GrScsiIf *scgp, cdr_t *, track_t *trackp);	/* write toc on disk */
	__s32	(*cdr_stats)		(St_GrScsiIf *scgp, cdr_t *);			/* final statistics printing*/
	__s32	(*cdr_blank)		(St_GrScsiIf *scgp, cdr_t *, long addr, __s32 blanktype);	/* blank something */
	__s32	(*cdr_format)		(St_GrScsiIf *scgp, cdr_t *, __s32 fmtflags);	/* format media */
	__s32	(*cdr_opc)		(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 doopc);	/* Do OPC */
	__s32	(*cdr_opt1)		(St_GrScsiIf *scgp, cdr_t *);			/* do early option processing*/
	__s32	(*cdr_opt2)		(St_GrScsiIf *scgp, cdr_t *);			/* do late option processing */
};

static struct exargs {
	St_GrScsiIf		*scgp;		/* The open SCSI * pointer	    */
	cdr_t		*dp;		/* The pointer to the device driver */
	__s32		old_secsize;
	__u32	flags;
	__s32		exflags;
} exargs;

//====================================================================
//class

//====================================================================
//global function
St_GrScsiIf* scg_open(__s8 *scsidev, __s32 debug, __s32 be_verbose);
__s32		scg_close(St_GrScsiIf * scgp);

__s32		scg_settarget(St_GrScsiIf *scgp, __s32 busno, __s32 tgt, __s32 tlun);

void 	scg_settimeout(St_GrScsiIf	*scgp, __s32 timeout);

long	scg_bufsize(St_GrScsiIf *scgp, long amt);
void*	scg_getbuf(St_GrScsiIf	*scgp, long	amt);
BOOL8	scg_havebus(St_GrScsiIf *scgp,	__s32	busno);
__s32		scg_initiator_id(St_GrScsiIf *scgp);

__s32		scg_cmd(St_GrScsiIf *scgp);
__s32		scg_sense_key(St_GrScsiIf *scgp);
__s32		scg_cmd_status(St_GrScsiIf *scgp);
__s32		scg_sense_code(St_GrScsiIf *scgp);
BOOL8	scg_cmd_err(St_GrScsiIf *scgp);
void	__scg_times(St_GrScsiIf *scgp);
__s32		scg_sense_qual(St_GrScsiIf *scgp);

__s32		select_target(St_GrScsiIf *scgp, FILE *f);
__s32		find_target(St_GrScsiIf *scgp,	__s32	type,	__s32	idx);
__s32		_select_target(St_GrScsiIf	*scgp, FILE	*f, __s32	type, __s32	idx);

void	scg_printerr(St_GrScsiIf	*scgp);

void	reload_media(St_GrScsiIf	*scgp,	cdr_t	*dp);
__s32		write_secs(St_GrScsiIf	*scgp, cdr_t *dp,	__s8 *bp,	long startsec, __s32 bytespt, __s32	secspt,	BOOL8 islast);
__s32		scg_getresid(St_GrScsiIf	*scgp);
void	load_media(St_GrScsiIf *scgp, cdr_t *, BOOL8);
void	unload_media(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags);
__s32		find_drive(St_GrScsiIf	*scgp, __s8	*dev,	__s32	flags);
__s32		pad_track(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp, long startsec, __u64 amt,
				BOOL8 dolast, __u64 *bytesp);
void	catchsig(__s32	sig);
__s32		get_dmaspeed(St_GrScsiIf	*scgp, cdr_t *dp, __s32 A_BufSize );

//buffer
__s32		read_buffer(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 mode);
__s32		write_buffer(St_GrScsiIf *scgp, __s8 *buffer, long length, __s32 mode, __s32 bufferid, long offset);

__s32		_read_buffer(St_GrScsiIf	*scgp, __s32 size);
void	opentracks(track_t *trackp, __u64 A_IsoSize);
void	checksize(track_t	*trackp, __u64 A_IsoSize);
BOOL8	check_wrmode(cdr_t *dp,	__u32 wmode, __s32 tflags);
void	setleadinout(__s32 tracks, track_t *trackp);
void	set_trsizes(cdr_t	*dp, __s32 tracks, track_t *trackp, __s32 A_BufSize );
void	setpregaps(__s32 tracks, track_t	*trackp);
void	set_wrmode(cdr_t	*dp, __u32 wmode,	__s32	tflags);
long	checktsize(__s32	tracks,	track_t	*trackp);
void	checkfile(__s32	track, track_t *trackp);

void	set_secsize(St_GrScsiIf *scgp,	__s32	secsize);
void	check_recovery(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags);
BOOL8	checkdsize(St_GrScsiIf *scgp, cdr_t *dp, long tsize, __u32 flags);
//void	excdr(__s32	excode,	void *arg);
BOOL8	do_opc(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags);

void	cdrstats(cdr_t *dp);
__s32		fill_buf(__s32 f,	track_t	*trackp, long	secno, __s8	*bp, __s32 size);
__s32		read_buf(__s32 f,	__s8 *bp,	__s32	size);
void	raisepri(__s32 pri);
__s32		rt_raisepri(__s32 pri);

__s32		scg_vtail(St_GrScsiIf *scgp);
__s32		scg_svtail(St_GrScsiIf	*scgp, __s32 *retp,	__s8 *buf, __s32 maxcnt);
//__s32		scg_errflush(St_GrScsiIf	*scgp);
__s32		scg_errfflush(St_GrScsiIf *scgp, FILE *f);

__s32		scg_scandev(__s8 *devp,	__s32	*busp, __s32 *tgtp,	__s32	*lunp);

//====================================================================
//out Enviroment conditional
#endif

