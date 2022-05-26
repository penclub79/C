/*
	odd scsi class
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddScg
#define _EPC_GrOddScg

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include 	<stdio.h>
#ifdef	LINUX_APP
	#include	<sys/time.h>
	#include	<sys/types.h>
#endif
#include	<Hw/OddV2/GrOddDef.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct St_GrOddScgLocal
{
	int			scgfile;		/* Used for SG_GET_BUFSIZE ioctl()*/
	short		scgfiles[MAX_SCG + MAX_ATA][MAX_TGT][MAX_LUN];
	short		buscookies[MAX_SCG + MAX_ATA];
	int			pgbus;
	int			pack_id;		/* Should be a random number	*/
	int			drvers;
	short		isold;
	//short	flags;
	long		xbufsize;
	char*		xbuf;
	char*		SCSIbuf;
}	*Ptr_GrOddScgLocal;

typedef struct
{
	int	scsibus;	/* SCSI bus #    for next I/O		*/
	int	target;		/* SCSI target # for next I/O		*/
	int	lun;		/* SCSI lun #    for next I/O		*/
} scg_addr_t;

struct	scsi_status {
	unsigned char vu_00	: 1;	/* vendor unique */
	unsigned char chk	: 1;	/* check condition: sense data available */
	unsigned char cm	: 1;	/* condition met */
	unsigned char busy	: 1;	/* device busy or reserved */
	unsigned char is	: 1;	/* intermediate status sent */
	unsigned char vu_05	: 1;	/* vendor unique */
#define	st_scsi2	vu_05	/* SCSI-2 modifier bit */
	unsigned char vu_06	: 1;	/* vendor unique */
	unsigned char st_rsvd	: 1;	/* reserved */
#define	ext_st1	st_rsvd		/* extended status (next byte valid) */
	/* byte 1 */
	unsigned char ha_er	: 1;	/* host adapter detected error */
	unsigned char reserved: 6;	/* reserved */
	unsigned char ext_st2	: 1;	/* extended status (next byte valid) */
	/* byte 2 */
	unsigned char byte2;		/* third byte */
};

struct	scsi_sense {		/* scsi sense for error classes 0-6 */
	unsigned char	code	: 7;	/* error class/code */
	unsigned char	adr_val	: 1;	/* sense data is valid */
	unsigned char	high_addr;	/* high byte of block addr */
	unsigned char	mid_addr;	/* middle byte of block addr */
	unsigned char	low_addr;	/* low byte of block addr */
};

struct	scsi_g0cdb {		/* scsi group 0 command description block */
	unsigned char	cmd;		/* command code */
	unsigned char	high_addr : 5;	/* high part of block address */
	unsigned char	lun	  : 3;	/* logical unit number */
	unsigned char	mid_addr;	/* middle part of block address */
	unsigned char	low_addr;	/* low part of block address */
	unsigned char	count;		/* transfer length */
	unsigned char	link	  : 1;	/* link (another command follows) */
	unsigned char	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned char	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned char	rsvd	  : 3;	/* reserved */
	unsigned char	vu_56	  : 1;	/* vendor unique (byte 5 bit 6) */
	unsigned char	vu_57	  : 1;	/* vendor unique (byte 5 bit 7) */
};

struct	scsi_g1cdb {		/* scsi group 1 command description block */
	unsigned char	cmd;		/* command code */
	unsigned char	reladr	  : 1;	/* address is relative */
	unsigned char	res	  : 4;	/* reserved bits 1-4 of byte 1 */
	unsigned char	lun	  : 3;	/* logical unit number */
	unsigned char	addr[4];	/* logical block address */
	unsigned char	res6;		/* reserved byte 6 */
	unsigned char	count[2];	/* transfer length */
	unsigned char	link	  : 1;	/* link (another command follows) */
	unsigned char	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned char	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned char	rsvd	  : 3;	/* reserved */
	unsigned char	vu_96	  : 1;	/* vendor unique (byte 5 bit 6) */
	unsigned char	vu_97	  : 1;	/* vendor unique (byte 5 bit 7) */
};

struct	scsi_g5cdb {		/* scsi group 5 command description block */
	unsigned char	cmd;		/* command code */
	unsigned char	reladr	  : 1;	/* address is relative */
	unsigned char	res	  : 4;	/* reserved bits 1-4 of byte 1 */
	unsigned char	lun	  : 3;	/* logical unit number */
	unsigned char	addr[4];	/* logical block address */
	unsigned char	count[4];	/* transfer length */
	unsigned char	res10;		/* reserved byte 10 */
	unsigned char	link	  : 1;	/* link (another command follows) */
	unsigned char	fr	  : 1;	/* flag request (interrupt at completion) */
	unsigned char	naca	  : 1;	/* Normal ACA (Auto Contingent Allegiance) */
	unsigned char	rsvd	  : 3;	/* reserved */
	unsigned char	vu_B6	  : 1;	/* vendor unique (byte B bit 6) */
	unsigned char	vu_B7	  : 1;	/* vendor unique (byte B bit 7) */
};

struct	scg_cmd
{
	char*	addr;	
	//caddr_t	addr;		//Address of data in user space
	int	size;				//DMA count for data transfer
	int	flags;			//see below for definition
	int	cdb_len;		//Size of SCSI command in bytes
	int	sense_len;	//for intr() if -1 don't get sense
	int	timeout;		//timeout in seconds
	int	resid;			//Bytes not transfered
	int	error;			//Error code from scgintr()
	int	ux_errno;		//UNIX error code
	union
	{
		struct	scsi_status Scb; // Status returnd by command
		unsigned char cmd_scb[SCG_MAX_STATUS];
	} u_scb;
#define	scb	u_scb.Scb

	union {
		struct	scsi_sense Sense; /* Sense bytes from command */
		unsigned char cmd_sense[SCG_MAX_SENSE];
	} u_sense;
#define	sense	u_sense.Sense
	int	sense_count;		//Number of bytes valid in sense
	int	target;					//SCSI target id
	union
	{
		struct	scsi_g0cdb g0_cdb;
		struct	scsi_g1cdb g1_cdb;
		struct	scsi_g5cdb g5_cdb;
		unsigned char cmd_cdb[SCG_MAX_CMD];
	} cdb;				//24 bytes max. size is supported
};

struct	scsi_ext_sense
{
	/* byte 0 */
	unsigned char	type	: 7;	/* fixed at 0x70 */
	unsigned char	adr_val	: 1;	/* sense data is valid */
	/* byte 1 */
	unsigned char	seg_num;	/* segment number, applies to copy cmd only */
	/* byte 2 */
	unsigned char	key	: 4;	/* sense key, see below */
	unsigned char		: 1;	/* reserved */
	unsigned char	ili	: 1;	/* incorrect length indicator */
	unsigned char	eom	: 1;	/* end of media */
	unsigned char	fil_mk	: 1;	/* file mark on device */
	/* bytes 3 through 7 */
	unsigned char	info_1;		/* information byte 1 */
	unsigned char	info_2;		/* information byte 2 */
	unsigned char	info_3;		/* information byte 3 */
	unsigned char	info_4;		/* information byte 4 */
	unsigned char	add_len;	/* number of additional bytes */
	/* bytes 8 through 13, CCS additions */
	unsigned char	optional_8;	/* CCS search and copy only */
	unsigned char	optional_9;	/* CCS search and copy only */
	unsigned char	optional_10;	/* CCS search and copy only */
	unsigned char	optional_11;	/* CCS search and copy only */
	unsigned char 	sense_code;	/* sense code */
	unsigned char	qual_code;	/* sense code qualifier */
	unsigned char	fru_code;	/* Field replacable unit code */
	unsigned char	bptr	: 3;	/* bit pointer for failure (if bpv) */
	unsigned char	bpv	: 1;	/* bit pointer is valid */
	unsigned char		: 2;
	unsigned char	cd	: 1;	/* pointers refer to command not data */
	unsigned char	sksv	: 1;	/* sense key specific valid */
	unsigned char	field_ptr[2];	/* field pointer for failure */
	unsigned char	add_info[2];	/* round up to 20 bytes */
};

//====================================================================
//class
class Cls_GrOddScg
{
	public:
		int 		m_Fd; /* File descriptor for next I/O		*/
		scg_addr_t 	m_Addr; /* SCSI address for next I/O		*/
		int 		m_Flags; /* Drive specific flags (see below)	*/
		int 		m_Overbose; /* Be verbose in open() if value > 0	*/
		int 		m_TimeOut;
		int 		m_Dev;
		struct scg_cmd m_Scmd; //scmd
		char* 	m_CmdName;
		char* 	m_CurCmdName;
		BOOL		m_IsRun;
		__u32		m_MaxDma; /* Max DMA limit for this open instance	*/
		__u32		m_MaxBuf; /* Cur DMA buffer limit for this inst.	*/
		/* This is the size behind bufptr	*/
		struct timeval m_CmdStart;
		struct timeval m_CmdStop;

		St_GrOddScgLocal m_Local;

		void*		m_PtrBufBase;	/* needed for scsi_freebuf()		*/
		void*		m_PtrBuf;	/* DMA buffer pointer for appl. use	*/
		char		m_ErrStr[SCSI_ERRSTR_SIZE];		/* Error string for scsi_open/sendmcd	*/
		char*		m_PtrErrBegin;	/* Pointer to begin of not flushed data	*/
		char*		m_PtrErr;	/* Actual write pointer into errstr	*/

		struct scsi_inquiry inq;
		struct scsi_capacity cap;
	protected:
		//scgo
		int		LcScgOpen(char *device);

		//sg
		BOOL	LcMapDev(int A_Hndl, int* A_PtrBus, int* A_PtrTgt, int* A_PtrLun, int* A_PtrChan, int* A_PtrIno, int A_AtaIdx);
		void	sg_clearnblock(int f);
		long	LcScg0_MaxDma(long amt);
		long	LcSg_RaiseDma(long newmax);
		int		LcScg0_Fileno(int busno, int tgt, int tlun);
		void	LcSg_CheckDma(int f, int* valp);
		void* LcScg0_GetBuf(long amt);

		int		LcScg0_Send();
		void	LcScg_Times();
		int		LcSg_RwSend();
		int		LcScg_Vtail();
		BOOL	LcScg_Cmd_Err();

		int		LcScg0_Close();
		void	LcScg0_FreeBuf();

		void	sg_initdev(int f);
		int		sg_mapbus(int	busno, int ino);
		BOOL 	sg_setup(int f, int	busno, int tgt, int	tlun, int	ataidx);
		BOOL	sg_mapdev(int	f, int *busp, int	*tgtp, int *lunp, int	*chanp, int	*inop, int ataidx);
	public:
		Cls_GrOddScg(int A_Verbose);
		virtual	~Cls_GrOddScg();

		BOOL	Init(char* A_PtrDev);
		void	TimeOut(int A_TimeOut);
		long	BufSize(long amt);
		int		SetTarget(int A_Bus, int A_Target, int A_Lun);
		void*	scg_getbuf(long	amt);
		long	scg_bufsize(long amt);
		int		scg_getresid();

		int		Scg_Sense_Key();
		int		Scg_Cmd_Status();
		int		Scg_Sense_Code();
		int		Scg_Cmd();
		int		Scg_Sense_Qual();
		int		Scg_NumBus();
		BOOL	Scg_HaveBus(int busno);
		struct scg_cmd* GetScgCmd();
		void  sg_settimeout(int f,	int	tmo);
};
//====================================================================
//global function
int		select_target(Cls_GrOddScg *scgp, FILE *f);
int		find_target(Cls_GrOddScg *scgp,	int	type,	int	idx);
void	reload_media(Cls_GrOddScg	*scgp, void* A_PtrDev);
int		write_secs(Cls_GrOddScg	*scgp, void* A_PtrDev, char *bp, long startsec, int bytespt, int	secspt, BOOL islast);
void	load_media(Cls_GrOddScg	*scgp, void* A_PtrDev, BOOL	doexit);
void	unload_media(Cls_GrOddScg	*scgp, void* A_PtrDev, __u32 flags);
int		find_drive(Cls_GrOddScg	*scgp, char	*dev,	int	flags);
int		pad_track(Cls_GrOddScg *scgp, void* A_PtrDev, track_t *trackp, long startsec, __u64 amt,
	BOOL dolast, __u64 *bytesp);
void	catchsig(int	sig);
int		get_dmaspeed(Cls_GrOddScg	*scgp, void* A_PtrDev, int A_BufSize );

//buffer
int		read_buffer(Cls_GrOddScg *scgp, caddr_t bp, int cnt, int mode);
int		write_buffer(Cls_GrOddScg *scgp, char *buffer, long length, int mode, int bufferid, long offset);

void	opentracks(track_t *trackp, __u64 A_IsoSize);
void	checksize(track_t	*trackp, __u64 A_IsoSize);
BOOL	check_wrmode(void* A_PtrDev, __u32 wmode, int tflags);
void	setleadinout(int tracks, track_t *trackp);
void	set_trsizes(void* A_PtrDev, int tracks, track_t *trackp, int A_BufSize );
void	setpregaps(int tracks, track_t	*trackp);
void	set_wrmode(void* A_PtrDev, __u32 wmode, int	tflags);
long	checktsize(int	tracks,	track_t	*trackp);

void	set_secsize(Cls_GrOddScg *scgp,	int	secsize);
void	check_recovery(Cls_GrOddScg	*scgp, void* A_PtrDev, __u32 flags);
BOOL	checkdsize(Cls_GrOddScg *scgp, void* A_PtrDev, long tsize, __u32 flags);
//void	excdr(int	excode,	void *arg);
BOOL	do_opc(Cls_GrOddScg	*scgp, void* A_PtrDev, __u32 flags);

void	cdrstats(void* A_PtrDev);
int		fill_buf(int f,	track_t	*trackp, long	secno, char	*bp, int size);
int		read_buf(int f,	char *bp,	int	size);
void	raisepri(int pri);
int		rt_raisepri(int pri);

//====================================================================
//out Enviroment conditional

#endif
