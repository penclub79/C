/*
	emulation LINUX application API
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrElaBase
#define	_EPC_GrElaBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrDebug.h>
#include	<GrProcess.h>

#include	<EmuLnx/GrEmuLnxBase.h>

//====================================================================
//constance

#define	UIO_MAXIOV			1024

#define EBADFD					81
#define ESTRPIPE				92

#define O_ACCMODE       00000003
#define O_RDONLY        00000000
#define O_WRONLY        00000001
#define O_RDWR          00000002
#define O_APPEND        00000010
#define O_BLKSEEK       00000100 /* HPUX only */
#define O_CREAT         00000400 /* not fcntl */
#define O_TRUNC         00001000 /* not fcntl */
#define O_EXCL          00002000 /* not fcntl */
#define O_LARGEFILE     00004000
#define O_SYNC          00100000
#define O_NONBLOCK      00200004 /* HPUX has separate NDELAY & NONBLOCK */
#define O_NDELAY        O_NONBLOCK
#define O_NOCTTY        00400000 /* not fcntl */
#define O_DSYNC         01000000 /* HPUX only */
#define O_RSYNC         02000000 /* HPUX only */
#define O_NOATIME       04000000

#define FASYNC          00020000 /* fcntl, for BSD compatibility */
#define O_DIRECT        00040000 /* direct disk access hint - currently ignored */
#define O_DIRECTORY     00010000 /* must be a directory */
#define O_NOFOLLOW      00000200 /* don't follow links */
#define O_INVISIBLE     04000000 /* invisible I/O, for DMAPI/XDSM */

#define F_DUPFD         0       /* dup */
#define F_GETFD         1       /* get f_flags */
#define F_SETFD         2       /* set f_flags */
#define F_GETFL         3       /* more flags (cloexec) */
#define F_SETFL         4
#define F_GETLK         5
#define F_SETLK         6
#define F_SETLKW        7
#define F_GETLK64       8
#define F_SETLK64       9
#define F_SETLKW64      10

#define PROT_READ				0
#define PROT_WRITE			0
#define PROT_EXEC				0
#define MAP_SHARED			0

#define PRIO_PROCESS						0
#define PRIO_PGRP								1
#define PRIO_USER								2

#define SCHED_NORMAL            0
#define SCHED_FIFO              1
#define SCHED_RR                2

#define	STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

#define _SC_PAGESIZE	0
#define MAP_ANONYMOUS	0

//check handle type
#define S_IFMT  00170000
#define S_IFSOCK 0140000
#define S_IFLNK  0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000
 
#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)     (((m) & S_IFMT) == S_IFSOCK)

//signal
#define SIGHUP           1
#define SIGINT           2
#define SIGQUIT          3
#define SIGILL           4
#define SIGTRAP          5
#define SIGABRT          6
#define SIGIOT           6
#define SIGEMT           7
#define SIGFPE           8
#define SIGKILL          9
#define SIGBUS          10
#define SIGSEGV         11
#define SIGSYS          12 /* Linux doesn't use this */
#define SIGPIPE         13
#define SIGALRM         14
#define SIGTERM         15
#define SIGUSR1         16
#define SIGUSR2         17
#define SIGCHLD         18
#define SIGPWR          19
#define SIGVTALRM       20
#define SIGPROF         21
#define SIGIO           22
#define SIGPOLL         SIGIO
#define SIGWINCH        23
#define SIGSTOP         24
#define SIGTSTP         25
#define SIGCONT         26
#define SIGTTIN         27
#define SIGTTOU         28
#define SIGURG          29
#define SIGLOST         30 /* Linux doesn't use this either */
#define SIGUNUSED       31
#define SIGRESERVE      SIGUNUSED

#define SIGXCPU         33
#define SIGXFSZ         34
#define SIGSTKFLT       36

/* These should not be considered constants from user land.  */
#define SIGRTMIN        37
#define SIGRTMAX        _NSIG /* it's 44 under HP/UX */

#define SG_DXFER_NONE					(-1)				/* e.g. a SCSI Test Unit Ready command */
#define SG_DXFER_TO_DEV				(-2)				/* e.g. a SCSI WRITE command */
#define SG_DXFER_FROM_DEV			(-3)				/* e.g. a SCSI READ command */
#define SG_DXFER_TO_FROM_DEV	(-4)				/*	treated like SG_DXFER_FROM_DEV with the
																						additional property than during indirect
																						IO the user buffer is copied into the
																						kernel buffers before the transfer */
#define SG_DXFER_UNKNOWN			(-5)				/* Unknown data direction */

#define SG_FLAG_DIRECT_IO						1     /* default is indirect IO */
#define SG_FLAG_UNUSED_LUN_INHIBIT	2			/* default is overwrite lun in SCSI */
																					/* command block (when <= SCSI_2) */
#define SG_FLAG_MMAP_IO							4			/* request memory mapped IO */
#define SG_FLAG_NO_DXFER						0x10000		/* no transfer of kernel buffers to/from */
																							/* user space (debug indirect IO) */

//ioctl
#define   _SC_ARG_MAX           1
#define   _SC_CHILD_MAX         2
#define   _SC_CLK_TCK           3
#define   _SC_NGROUPS_MAX       4
#define   _SC_OPEN_MAX          5
#define   _SC_JOB_CONTROL       6
#define   _SC_SAVED_IDS         7
#define   _SC_VERSION           8

#define		_SC_PRIORITY_SCHEDULING		0x100

//ioctl - SCSI para
#define SG_MAX_SENSE						16
#define SG_SCATTER_SZ						(8 * 4096)
#define SG_DEFAULT_RETRIES			0
#define SG_MAX_QUEUE						16
#define SG_DEF_RESERVED_SIZE		SG_SCATTER_SZ /* load time option */
#define SG_BIG_BUFF							SG_DEF_RESERVED_SIZE    /* for backward compatibility */

#define	SG_SCSI_RESET_NOTHING   0
#define SG_SCSI_RESET_DEVICE    1
#define SG_SCSI_RESET_BUS       2
#define SG_SCSI_RESET_HOST      3

//ioctl - SCSI para command

#define SG_SET_TIMEOUT				0x2201
#define SG_GET_TIMEOUT				0x2202
#define SG_GET_COMMAND_Q			0x2270
#define SG_SET_COMMAND_Q			0x2271

#define SG_EMULATED_HOST			0x2203
#define SG_SET_TRANSFORM			0x2204
#define SG_GET_TRANSFORM			0x2205

#define SG_SET_RESERVED_SIZE	0x2275
#define SG_GET_RESERVED_SIZE	0x2272
#define SG_GET_SCSI_ID				0x2276
#define SG_SET_FORCE_LOW_DMA	0x2279 
#define SG_GET_LOW_DMA				0x227A
#define SG_SET_FORCE_PACK_ID	0x227B
#define SG_GET_PACK_ID				0x227C
#define SG_GET_NUM_WAITING		0x227D

#define SG_GET_SG_TABLESIZE		0x227F
#define SG_GET_VERSION_NUM		0x2282

#define SG_SCSI_RESET					0x2284

#define SG_IO									0x2285   /* similar effect as write() followed by read() */
#define SG_GET_REQUEST_TABLE	0x2286   /* yields table of active requests */
#define SG_SET_KEEP_ORPHAN		0x2287 /* 1 -> hold for read(), 0 -> drop (def) */
#define SG_GET_KEEP_ORPHAN		0x2288
#define SG_GET_ACCESS_COUNT		0x2289

#define SCSI_IOCTL_GET_IDLUN  0x5382
#define SCSI_IOCTL_PROBE_HOST 0x5385
#define SCSI_IOCTL_GET_BUS_NUMBER   0x5386
#define SCSI_IOCTL_GET_PCI    0x5387

#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
#define I2C_M_STOP		0x8000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_16BIT_REG		0x0002	/* indicate reg bit-width is 16bit */
#define I2C_M_16BIT_DATA	0x0008	/* indicate data bit-width is 16bit */
#define I2C_M_DMA			0x0004  /* indicate use dma mode */

/* To determine what functionality is present */

#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004 /* I2C_M_NOSTART etc. */

#define I2C_RETRIES	0x0701	/* number of times a device address should
				   be polled when not acknowledging */
#define I2C_TIMEOUT	0x0702	/* set timeout in units of 10 ms */

/* NOTE: Slave address is 7 or 10 bits, but 10-bit addresses
 * are NOT supported! (due to code brokenness)
 */
#define I2C_SLAVE	0x0703	/* Use this slave address */
#define I2C_SLAVE_FORCE	0x0706	/* Use this slave address, even if it
				   is already in use by a driver! */
#define I2C_TENBIT	0x0704	/* 0 for 7 bit addrs, != 0 for 10 bit */

#define I2C_FUNCS	0x0705	/* Get the adapter functionality mask */

#define I2C_RDWR	0x0707	/* Combined R/W transfer (one STOP only) */

#define I2C_PEC		0x0708	/* != 0 to use PEC with SMBus */
#define I2C_SMBUS	0x0720	/* SMBus transfer */

#define  I2C_RDRW_IOCTL_MAX_MSGS	42

#define _IOWR(x,y,t)     (IOC_IN|(((long)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define RTC_AIE_ON	_IO('p', 0x01)	/* Alarm int. enable on		*/
#define RTC_AIE_OFF	_IO('p', 0x02)	/* ... off			*/
#define RTC_UIE_ON	_IO('p', 0x03)	/* Update int. enable on	*/
#define RTC_UIE_OFF	_IO('p', 0x04)	/* ... off			*/
#define RTC_PIE_ON	_IO('p', 0x05)	/* Periodic int. enable on	*/
#define RTC_PIE_OFF	_IO('p', 0x06)	/* ... off			*/
#define RTC_WIE_ON	_IO('p', 0x0f)  /* Watchdog int. enable on	*/
#define RTC_WIE_OFF	_IO('p', 0x10)  /* ... off			*/

#define RTC_ALM_SET	_IOW('p', 0x07, struct rtc_time) /* Set alarm time  */
#define RTC_ALM_READ	_IOR('p', 0x08, struct rtc_time) /* Read alarm time */
#define RTC_RD_TIME	_IOR('p', 0x09, struct rtc_time) /* Read RTC time   */
#define RTC_SET_TIME	_IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */
#define RTC_IRQP_READ	_IOR('p', 0x0b, unsigned long)	 /* Read IRQ rate   */
#define RTC_IRQP_SET	_IOW('p', 0x0c, unsigned long)	 /* Set IRQ rate    */
#define RTC_EPOCH_READ	_IOR('p', 0x0d, unsigned long)	 /* Read epoch      */
#define RTC_EPOCH_SET	_IOW('p', 0x0e, unsigned long)	 /* Set epoch       */

#define RTC_WKALM_SET	_IOW('p', 0x0f, struct rtc_wkalrm)/* Set wakeup alarm*/
#define RTC_WKALM_RD	_IOR('p', 0x10, struct rtc_wkalrm)/* Get wakeup alarm*/

#define RTC_PLL_GET	_IOR('p', 0x11, struct rtc_pll_info)  /* Get PLL correction */
#define RTC_PLL_SET	_IOW('p', 0x12, struct rtc_pll_info)  /* Set PLL correction */

/* interrupt flags */
#define RTC_IRQF 0x80 /* any of the following is active */
#define RTC_PF 0x40
#define RTC_AF 0x20
#define RTC_UF 0x10

// ALSA
#define SNDRV_MASK_MAX	256

enum sndrv_pcm_hw_param {
	SNDRV_PCM_HW_PARAM_ACCESS = 0,  /* Access type */
	SNDRV_PCM_HW_PARAM_FIRST_MASK = SNDRV_PCM_HW_PARAM_ACCESS,
	SNDRV_PCM_HW_PARAM_FORMAT,      /* Format */
	SNDRV_PCM_HW_PARAM_SUBFORMAT,   /* Sub format */
	SNDRV_PCM_HW_PARAM_LAST_MASK = SNDRV_PCM_HW_PARAM_SUBFORMAT,
	
	SNDRV_PCM_HW_PARAM_SAMPLE_BITS = 8, /* Bits per sample */
	SNDRV_PCM_HW_PARAM_FIRST_INTERVAL = SNDRV_PCM_HW_PARAM_SAMPLE_BITS,
	SNDRV_PCM_HW_PARAM_FRAME_BITS,  /* Bits per frame */
	SNDRV_PCM_HW_PARAM_CHANNELS,    /* Channels */
	SNDRV_PCM_HW_PARAM_RATE,        /* Approx rate */
	SNDRV_PCM_HW_PARAM_PERIOD_TIME, /* Approx distance between interrupts	in us */
	SNDRV_PCM_HW_PARAM_PERIOD_SIZE, /* Approx frames between interrupts */
	SNDRV_PCM_HW_PARAM_PERIOD_BYTES, /* Approx bytes between interrupts */
	SNDRV_PCM_HW_PARAM_PERIODS,     /* Approx interrupts per buffer */
	SNDRV_PCM_HW_PARAM_BUFFER_TIME, /* Approx duration of buffer in us */
	SNDRV_PCM_HW_PARAM_BUFFER_SIZE, /* Size of buffer in frames */
	SNDRV_PCM_HW_PARAM_BUFFER_BYTES, /* Size of buffer in bytes */
	SNDRV_PCM_HW_PARAM_TICK_TIME,   /* Approx tick duration in us */
	SNDRV_PCM_HW_PARAM_LAST_INTERVAL = SNDRV_PCM_HW_PARAM_TICK_TIME
};

enum sndrv_pcm_tstamp {
	SNDRV_PCM_TSTAMP_NONE = 0,
	SNDRV_PCM_TSTAMP_MMAP,
	SNDRV_PCM_TSTAMP_LAST = SNDRV_PCM_TSTAMP_MMAP,
	};

enum snd_pcm_stream_t  
{
	SND_PCM_STREAM_PLAYBACK	=	0,	//  Playback stream  
	SND_PCM_STREAM_CAPTURE				//	Capture stream  
};

#define SND_PCM_NONBLOCK		1

enum   snd_pcm_access_t { 
	SND_PCM_ACCESS_MMAP_INTERLEAVED = 0,	/* interleaved mmap */
	SND_PCM_ACCESS_MMAP_NONINTERLEAVED,		/* noninterleaved mmap */	
	SND_PCM_ACCESS_MMAP_COMPLEX,					/* complex mmap */
	SND_PCM_ACCESS_RW_INTERLEAVED,				/* readi/writei */
	SND_PCM_ACCESS_RW_NONINTERLEAVED,			/* readn/writen */
	SND_PCM_ACCESS_LAST = SND_PCM_ACCESS_RW_NONINTERLEAVED 
}; 

enum   snd_pcm_format_t { 
	SND_PCM_FORMAT_UNKNOWN = -1, SND_PCM_FORMAT_S8 = 0, SND_PCM_FORMAT_U8, SND_PCM_FORMAT_S16_LE, 
	SND_PCM_FORMAT_S16_BE, SND_PCM_FORMAT_U16_LE, SND_PCM_FORMAT_U16_BE, SND_PCM_FORMAT_S24_LE, 
	SND_PCM_FORMAT_S24_BE, SND_PCM_FORMAT_U24_LE, SND_PCM_FORMAT_U24_BE, SND_PCM_FORMAT_S32_LE, 
	SND_PCM_FORMAT_S32_BE, SND_PCM_FORMAT_U32_LE, SND_PCM_FORMAT_U32_BE, SND_PCM_FORMAT_FLOAT_LE, 
	SND_PCM_FORMAT_FLOAT_BE, SND_PCM_FORMAT_FLOAT64_LE, SND_PCM_FORMAT_FLOAT64_BE, SND_PCM_FORMAT_IEC958_SUBFRAME_LE, 
	SND_PCM_FORMAT_IEC958_SUBFRAME_BE, SND_PCM_FORMAT_MU_LAW, SND_PCM_FORMAT_A_LAW, SND_PCM_FORMAT_IMA_ADPCM, 
	SND_PCM_FORMAT_MPEG, SND_PCM_FORMAT_GSM, SND_PCM_FORMAT_SPECIAL = 31, SND_PCM_FORMAT_S24_3LE = 32, 
	SND_PCM_FORMAT_S24_3BE, SND_PCM_FORMAT_U24_3LE, SND_PCM_FORMAT_U24_3BE, SND_PCM_FORMAT_S20_3LE, 
	SND_PCM_FORMAT_S20_3BE, SND_PCM_FORMAT_U20_3LE, SND_PCM_FORMAT_U20_3BE, SND_PCM_FORMAT_S18_3LE, 
	SND_PCM_FORMAT_S18_3BE, SND_PCM_FORMAT_U18_3LE, SND_PCM_FORMAT_U18_3BE, SND_PCM_FORMAT_LAST = SND_PCM_FORMAT_U18_3BE, 
	SND_PCM_FORMAT_S16 = SND_PCM_FORMAT_S16_LE, SND_PCM_FORMAT_U16 = SND_PCM_FORMAT_U16_LE, SND_PCM_FORMAT_S24 = SND_PCM_FORMAT_S24_LE, SND_PCM_FORMAT_U24 = SND_PCM_FORMAT_U24_LE, 
	SND_PCM_FORMAT_S32 = SND_PCM_FORMAT_S32_LE, SND_PCM_FORMAT_U32 = SND_PCM_FORMAT_U32_LE, SND_PCM_FORMAT_FLOAT = SND_PCM_FORMAT_FLOAT_LE, SND_PCM_FORMAT_FLOAT64 = SND_PCM_FORMAT_FLOAT64_LE, 
	SND_PCM_FORMAT_IEC958_SUBFRAME = SND_PCM_FORMAT_IEC958_SUBFRAME_LE 
}; 

#define NCCS	19

#define B1200   0x09
#define B1800   0x0a
#define B2400   0x0b
#define B4800   0x0c
#define B9600   0x0d
#define B19200  0x0e
#define B38400  0x0f
#define B56000  0x10
#define B57600  0x11
#define B64000  0x12
#define B115200 0x13

#define   CS5   0000000
#define   CS6   0000020
#define   CS7   0000040
#define   CS8   0000060

#define CSTOPB  0000100
#define CREAD   0000200
#define PARENB  0000400
#define PARODD  0001000
#define HUPCL   0002000
#define CLOCAL  0004000

#define VINTR 0
#define VQUIT 1
#define VERASE 2
#define VKILL 3
#define VEOF 4
#define VTIME 5
#define VMIN 6
#define VSWTC 7
#define VSTART 8
#define VSTOP 9
#define VSUSP 10
#define VEOL 11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE 14
#define VLNEXT 15
#define VEOL2 16

#define TCIFLUSH        0
#define TCOFLUSH        1
#define TCIOFLUSH       2
/* tcsetattr uses these */
#define TCSANOW         0
#define TCSADRAIN       1
#define TCSAFLUSH       2

#define FBIOGET_VSCREENINFO     0x4600
#define FBIOPUT_VSCREENINFO     0x4601
#define FBIOGET_FSCREENINFO     0x4602
#define FBIOPAN_DISPLAY         0x4606

#define FB_ACTIVATE_NOW         0       /* set values immediately (or vbl)*/
#define FB_ACTIVATE_NXTOPEN     1       /* activate on next open        */
#define FB_ACTIVATE_TEST        2       /* don't set, round up impossible */
#define FB_ACTIVATE_MASK       15
                                        /* values                       */
#define FB_ACTIVATE_VBL        16       /* activate values on next vbl  */
#define FB_ACTIVATE_ALL        64       /* change all VCs on this fb    */
#define FB_ACTIVATE_FORCE     128       /* force apply even when no change*/
#define FB_ACTIVATE_INV_MODE  256       /* invalidate videomode */

#define IPC_CREAT							1000
#define IPC_EXCL							2000

enum EPOLL_EVENTS
{
	EPOLLIN = 0x001,
#define EPOLLIN EPOLLIN
	EPOLLPRI = 0x002,
#define EPOLLPRI EPOLLPRI
	EPOLLOUT = 0x004,
#define EPOLLOUT EPOLLOUT
	EPOLLRDNORM = 0x040,
#define EPOLLRDNORM EPOLLRDNORM
	EPOLLRDBAND = 0x080,
#define EPOLLRDBAND EPOLLRDBAND
	EPOLLWRNORM = 0x100,
#define EPOLLWRNORM EPOLLWRNORM
	EPOLLWRBAND = 0x200,
#define EPOLLWRBAND EPOLLWRBAND
	EPOLLMSG = 0x400,
#define EPOLLMSG EPOLLMSG
	EPOLLERR = 0x008,
#define EPOLLERR EPOLLERR
	EPOLLHUP = 0x010,
#define EPOLLHUP EPOLLHUP
	EPOLLET = (1 << 31),
#define EPOLLET EPOLLET
	EPOLLRDHUP = 0x2000
#define EPOLLRDHUP EPOLLRDHUP
};

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3


#define	AF_PACKET			20


//lcd300
#define LCD300_IOC_MAGIC  'g'
#define LCD300_IOC_GET_EDID        		_IOR(LCD300_IOC_MAGIC, 1, struct lcd300_edid)
#define LCD300_IOC_SET_EDID        		_IOW(LCD300_IOC_MAGIC, 2, unsigned int)
#define LCD300_IOC_GET_FBPADDR     		_IOR(LCD300_IOC_MAGIC, 3, unsigned int)   	/* get frame buffer physical address */
#define LCD300_IOC_GET_INDIM_OTYPE 		_IOR(LCD300_IOC_MAGIC, 4, struct lcd300_indim_otype)
#define LCD300_IOC_SET_INDIM_OTYPE 		_IOW(LCD300_IOC_MAGIC, 5, struct lcd300_indim_otype)
#define LCD300_IOC_SET_IOCURSOR				_IOW(LCD300_IOC_MAGIC, 7, unsigned int)
#define LCD300_IOC_RLE_FORCE_REFRESH  _IOW(LCD300_IOC_MAGIC, 8, unsigned int)   	/* refresh the gui immediately */
#define LCD300_IOC_RLE_SET_INTVAL     _IOW(LCD300_IOC_MAGIC, 9, unsigned int)   	/* set refresh interval */
#define LCD300_IOC_RLE_GET_INTVAL     _IOR(LCD300_IOC_MAGIC, 10, unsigned int)   	/* get refresh interval */
#define LCD300_IOC_SET_COLOR_KEY 			_IOW(LCD300_IOC_MAGIC, 12, struct lcd300_ckey)
#define LCD300_IOC_GET_INFMT					_IOR(LCD300_IOC_MAGIC, 14, unsigned int)	/* enum lcd300_infmt */
#define LCD300_IOC_SET_INFMT					_IOW(LCD300_IOC_MAGIC, 15, unsigned int)	/* enum lcd300_infmt */


//====================================================================
//global macro

#define readlink(A_StrFn,A_PtrBuf,A_BufSize) (int)GrFileCtrlRead( (A_StrFn), (A_PtrBuf), (A_BufSize), TRUE )

//====================================================================
//global type

typedef	char*	caddr_t;
typedef	int		pid_t;
typedef	int		ssize_t;
typedef	DWORD	mode_t;
//typedef	DWORD	tsize_t;
typedef	DWORD	uid_t;
typedef	DWORD	nlink_t;
typedef	DWORD	gid_t;
typedef	DWORD	blksize_t;
typedef	DWORD	blkcnt_t;
typedef	WORD	ino_t;
typedef	__int64	loff_t;
typedef int		key_t;

#ifndef dev_t
#define dev_t		int
#endif

typedef long snd_pcm_sframes_t;

typedef void (*sighandler_t)(int);

typedef	struct rlimit 
{
	DWORD rlim_cur;   /* Soft limit */
	DWORD rlim_max;   /* Hard limit (ceiling 
										 for rlim_cur) */
}	*Ptr_rlimit;

struct sched_param {
	int sched_priority;
};

typedef	struct flock 
{
	short l_type;				/* lock type */
	short l_whence;			/* area basis */
	off_t l_len;				/* area start position */
	off_t l_pid;				/* when F_GETLK is set, this function return process id */
}	*Ptr_flock;

typedef struct timezone
{    
	int tz_minuteswest;		/* GRINITCH west error term */
	int tz_dsttime;       /* DST correct value */
}	*Ptr_timezone;


#if 0
typedef	struct stat 
{
	dev_t         st_dev;      /* device */
	ino_t         st_ino;      /* inode */
	mode_t        st_mode;     /* protection */
	nlink_t       st_nlink;    /* number of hard links */
	uid_t         st_uid;      /* user ID of owner */
	gid_t         st_gid;      /* group ID of owner */
	dev_t         st_rdev;     /* device type (if inode device) */
	off_t         st_size;     /* total size, in bytes */
	blksize_t     st_blksize;  /* block size for file system I/O */
	blkcnt_t      st_blocks;   /* number of blocks allocated */
	time_t        st_atime;    /* time of last access */
	time_t        st_mtime;    /* time of last modification */
	time_t        st_ctime;    /* time of last change */
}	*Ptr_stat;
#endif

struct sg_header
{
     int pack_len;    /* [o] reply_len (ie useless), ignored as input */
     int reply_len;   /* [i] max length of expected reply (inc. sg_header) */
     int pack_id;     /* [io] id number of packet (use ints >= 0) */
     int result;      /* [o] 0==ok, else (+ve) Unix errno (best ignored) */
     unsigned int twelve_byte:1;
         /* [i] Force 12 byte command length for group 6 & 7 commands  */
     unsigned int target_status:5;   /* [o] scsi status from target */
     unsigned int host_status:8;     /* [o] host status (see "DID" codes) */
     unsigned int driver_status:8;   /* [o] driver status+suggestion */
     unsigned int other_flags:10;    /* unused */
     unsigned char sense_buffer[SG_MAX_SENSE]; /* [o] Output in 3 cases:
            when target_status is CHECK_CONDITION or
            when target_status is COMMAND_TERMINATED or
            when (driver_status & DRIVER_SENSE) is true. */
};      /* This structure is 36 bytes long on i386 */

typedef struct sg_io_hdr
{
     int interface_id;           /* [i] 'S' for SCSI generic (required) */
     int dxfer_direction;        /* [i] data transfer direction  */
     unsigned char cmd_len;      /* [i] SCSI command length ( <= 16 bytes) */
     unsigned char mx_sb_len;    /* [i] max length to write to sbp */
     unsigned short iovec_count; /* [i] 0 implies no scatter gather */
     unsigned int dxfer_len;     /* [i] byte count of data transfer */
     void *dxferp;        /* [i], [*io] points to data transfer memory
                                               or scatter gather list */
     unsigned char  *cmdp; /* [i], [*i] points to command to perform */
     void  *sbp;           /* [i], [*o] points to sense_buffer memory */
     unsigned int timeout;       /* [i] MAX_UINT->no timeout (unit: millisec) */
     unsigned int flags;         /* [i] 0 -> default, see SG_FLAG... */
     int pack_id;                /* [i->o] unused internally (normally) */
     void  * usr_ptr;      /* [i->o] unused internally */
     unsigned char status;       /* [o] SCSI status */
     unsigned char masked_status;/* [o] shifted, masked SCSI status */
     unsigned char msg_status;   /* [o] messaging level data (optional) */
     unsigned char sb_len_wr;    /* [o] byte count actually written to sbp */
     unsigned short host_status; /* [o] errors from host adapter */
     unsigned short driver_status;/* [o] errors from software driver */
     int resid;                  /* [o] dxfer_len - actual_transferred */
     unsigned int duration;      /* [o] time taken by cmd (unit: millisecond) */
     unsigned int info;          /* [o] auxiliary information */
} sg_io_hdr_t;  /* 64 bytes long (on i386) */

struct i2c_msg {
	WORD addr;	/* slave address			*/
	WORD flags;
	WORD len;		/* msg length				*/
	unsigned char *buf;		/* pointer to msg data			*/
};

/* This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/* pointers to i2c_msgs */
	DWORD nmsgs;			/* number of i2c_msgs */
};

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

struct mtd_info_user {
	BYTE type;
	DWORD flags;
	DWORD size;     /* Total size of the MTD */
	DWORD erasesize;
	DWORD writesize;
	DWORD oobsize;  /* Amount of OOB data per block (e.g. 16) */
	__u64 padding;  /* Old obsolete field; do not use */
};

struct mtd_oob_buf {
	DWORD	start;
	DWORD	length;
	BYTE*	ptr;
};

struct nand_oobinfo {
	DWORD	useecc;
	DWORD	eccbytes;
	DWORD	oobfree[8][2];
	DWORD	eccpos[32];
};

struct erase_info_t {
	DWORD	start;
	DWORD	length;
};

struct snd_pcm_str_t {
	int stream;                             /* stream (direction) */
	void *pcm;
	/* -- sub streams -- */
	unsigned int substream_count;
	unsigned int substream_opened;
	void*	substream;

	/*
	#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
		snd_pcm_oss_stream_t oss;
	#endif
	*/
	void*	files;
	void*	reg;
	void*	proc_root;
	void*	proc_info_entry;

	/*
	#ifdef CONFIG_SND_DEBUG
		unsigned int xrun_debug;        // 0 = disabled, 1 = verbose, 2 = stacktrace
		snd_info_entry_t *proc_xrun_debug_entry;
	#endif
	*/
};


struct snd_pcm_t {
	void*	card;
	unsigned int device;    /* device number */
	unsigned int info_flags;
	unsigned short dev_class;
	unsigned short dev_subclass;
	char id[64];
	char name[80];
	snd_pcm_str_t streams[2];
	int open_mutex;
	int open_wait;
	void *private_data;
	void (*private_free) (snd_pcm_t *pcm);
/*
#if defined(CONFIG_SND_PCM_OSS) || defined(CONFIG_SND_PCM_OSS_MODULE)
	snd_pcm_oss_t oss;
#endif
	*/
	};

struct sndrv_mask {
	unsigned int bits[(SNDRV_MASK_MAX+31)/32];
};

struct sndrv_interval {
	unsigned int min, max;
	unsigned int openmin:1,	openmax:1, integer:1,	empty:1;
};

typedef unsigned long sndrv_pcm_uframes_t;

struct snd_pcm_hw_params_t {
	unsigned int flags;
	struct sndrv_mask masks[SNDRV_PCM_HW_PARAM_LAST_MASK - SNDRV_PCM_HW_PARAM_FIRST_MASK + 1];
	struct sndrv_mask mres[5];      /* reserved masks */
	struct sndrv_interval intervals[SNDRV_PCM_HW_PARAM_LAST_INTERVAL - SNDRV_PCM_HW_PARAM_FIRST_INTERVAL + 1];
	struct sndrv_interval ires[9];  /* reserved intervals */
	unsigned int rmask;             /* W: requested masks */
	unsigned int cmask;             /* R: changed masks */
	unsigned int info;              /* R: Info flags for returned setup */
	unsigned int msbits;            /* R: used most significant bits */
	unsigned int rate_num;          /* R: rate numerator */
	unsigned int rate_den;          /* R: rate denominator */
	sndrv_pcm_uframes_t fifo_size;  /* R: chip FIFO size in frames */
	unsigned char reserved[64];     /* reserved for future */
};


struct snd_pcm_sw_params_t {
	enum sndrv_pcm_tstamp tstamp_mode;      /* timestamp mode */
	unsigned int period_step;
	unsigned int sleep_min;                 /* min ticks to sleep */
	sndrv_pcm_uframes_t avail_min;          /* min avail frames for wakeup */
	sndrv_pcm_uframes_t xfer_align;         /* xfer size need to be a multiple */
	sndrv_pcm_uframes_t start_threshold;    /* min hw_avail frames for automatic start */
	sndrv_pcm_uframes_t stop_threshold;     /* min avail frames for automatic stop */
	sndrv_pcm_uframes_t silence_threshold;  /* min distance from noise for silence filling */
	sndrv_pcm_uframes_t silence_size;       /* silence block size */
	sndrv_pcm_uframes_t boundary;           /* pointers wrap point */
	unsigned char reserved[64];             /* reserved for future */
};

typedef unsigned long snd_pcm_uframes_t;

typedef unsigned char cc_t;
typedef unsigned int tcflag_t;

struct termios {
	tcflag_t c_iflag;
	tcflag_t c_oflag;
	tcflag_t c_cflag;
	tcflag_t c_lflag;
	cc_t c_cc[NCCS];
};

typedef struct fb_fix_screeninfo 
{
         char id[16];                    /* identification string eg "TT Builtin" */
         unsigned long smem_start;       /* Start of frame buffer mem */
                                         /* (physical address) */
         DWORD smem_len;                 /* Length of frame buffer mem */
         DWORD type;                     /* see FB_TYPE_*                */
         DWORD type_aux;                 /* Interleave for interleaved Planes */
         DWORD visual;                   /* see FB_VISUAL_*              */ 
         WORD		xpanstep;                 /* zero if no hardware panning  */
         WORD		ypanstep;                 /* zero if no hardware panning  */
         WORD		ywrapstep;                /* zero if no hardware ywrap    */
         DWORD	line_length;              /* length of a line in bytes    */
         unsigned long mmio_start;       /* Start of Memory Mapped I/O   */
                                         /* (physical address) */
         DWORD	mmio_len;                 /* Length of Memory Mapped I/O  */
         DWORD	accel;                    /* Indicate to driver which     */
                                         /*  specific chip/card we have  */
         WORD		reserved[3];              /* Reserved for future compatibility */
}	*Ptr_fb_fix_screeninfo;

typedef struct fb_bitfield 
{
        DWORD	offset;                   /* beginning of bitfield        */
        DWORD	length;                   /* length of bitfield           */
        DWORD	msb_right;                /* != 0 : Most significant bit is */ 
                                        /* right */ 
}	*Ptr_fb_bitfield;


typedef struct fb_var_screeninfo 
{
         DWORD	xres;                     /* visible resolution           */
         DWORD	yres;
         DWORD	xres_virtual;             /* virtual resolution           */
         DWORD	yres_virtual;
         DWORD	xoffset;                  /* offset from virtual to visible */
         DWORD	yoffset;                  /* resolution                   */
 
         DWORD	bits_per_pixel;           /* guess what                   */
         DWORD	grayscale;                /* != 0 Graylevels instead of colors */
 
         struct fb_bitfield red;         /* bitfield in fb mem if true color, */
         struct fb_bitfield green;       /* else only length is significant */
         struct fb_bitfield blue;
         struct fb_bitfield transp;      /* transparency                 */      
 
         DWORD	nonstd;                   /* != 0 Non standard pixel format */
 
         DWORD	activate;                 /* see FB_ACTIVATE_*            */
 
         DWORD	height;                   /* height of picture in mm    */
         DWORD	width;                    /* width of picture in mm     */
 
         DWORD	accel_flags;              /* (OBSOLETE) see fb_info.flags */
 
         /* Timing: All values in pixclocks, except pixclock (of course) */
         DWORD	pixclock;                 /* pixel clock in ps (pico seconds) */
        DWORD	left_margin;              /* time from sync to picture    */
        DWORD	right_margin;             /* time from picture to sync    */
        DWORD	upper_margin;             /* time from sync to picture    */
        DWORD	lower_margin;
        DWORD	hsync_len;                /* length of horizontal sync    */
        DWORD	vsync_len;                /* length of vertical sync      */
        DWORD	sync;                     /* see FB_SYNC_*                */
        DWORD	vmode;                    /* see FB_VMODE_*               */
        DWORD	rotate;                   /* angle we rotate counter clockwise */
        DWORD	reserved[5];              /* Reserved for future compatibility */
}	*Ptr_fb_var_screeninfo;

typedef struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
}	*Ptr_iovec;

typedef union epoll_data {
	void    *ptr;
	int      fd;
	__u32 u32;
	__u64 u64;
} epoll_data_t;

struct epoll_event {
	__u32     events;    /* Epoll events */
	epoll_data_t data;      /* User data variable */
};


struct sysinfo{
	long uptime;             /* Seconds since boot */
	unsigned long loads[3];  /* 1, 5, and 15 minute load averages */
	unsigned long totalram;  /* Total usable main memory size */
	unsigned long freeram;   /* Available memory size */
	unsigned long sharedram; /* Amount of shared memory */
	unsigned long bufferram; /* Memory used by buffers */
	unsigned long totalswap; /* Total swap space size */
	unsigned long freeswap;  /* swap space still available */
	unsigned short procs;    /* Number of current processes */
	char _f[22];             /* Pads structure to 64 bytes */
};

#define FB_CUR_SETIMAGE 0x01
#define FB_CUR_SETPOS   0x02
#define FB_CUR_SETHOT   0x04
#define FB_CUR_SETCMAP  0x08
#define FB_CUR_SETSHAPE 0x10
#define FB_CUR_SETSIZE  0x20
#define FB_CUR_SETALL   0xFF

struct fbcurpos {
  __u16 x, y;
};

struct fb_cmap {
  __u32 start;            /* First entry  */
  __u32 len;          /* Number of entries */
  __u16 *red;         /* Red values   */
  __u16 *green;
  __u16 *blue;
  __u16 *transp;          /* transparency, can be NULL */
};

struct fb_image {
	__u32 dx;       /* Where to place image */
  __u32 dy;
  __u32 width;        /* Size of image */
  __u32 height;
  __u32 fg_color;     /* Only used when a mono bitmap */
  __u32 bg_color;
  __u8  depth;        /* Depth of the image */
  const char *data;   /* Pointer to image data */
  struct fb_cmap cmap;    /* color map info */
};

typedef struct fb_cursor
{
	__u16		set;
	__u16		enable;
	__u16		rop;
	char*		mask;
	struct fbcurpos hot;
	struct fb_image image;
};

//====================================================================
//class

//====================================================================
//global function

int open( char* A_StrFn, int flags, int A_Atb = 0 );
int ioctl(int A_Hndl, int A_Cmd, void* A_PtrPara = NULL );
void*	mmap( void* A_PtrStart, size_t A_Size, int A_Prot, int A_Flag, int A_Hndl, off_t A_Offset );
int	munmap(void* A_PtrMap, size_t A_Size );
int close( int A_Hndl );
int	read( int A_Hndl, void* A_PtrBuf, int A_Size );
ssize_t write(int fd, const void *buf, size_t count);

//share memory
int shmget(key_t A_Key, int A_Size, int A_Shmflg);
void* shmat(int shmid, const void *shmaddr, int shmflg);

int	mount( char* A_StrDev, char* A_StrMnt, char* A_StrType, DWORD A_Flag, void* A_PtrPara );
int	umount( char* A_StrMnt );

int fstat(int filedes, struct stat *buf);

int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd, struct flock *lock);

int stat(const char *file_name, struct stat *buf);

long lseek(int handle, long offset, int fromwhere);
long sysconf(int name);
int isatty( int handle );

int gettimeofday( struct timeval* A_PtrTv, struct timezone* A_PtrTz );

pid_t fork(void);
int kill(pid_t pid, int sig);
pid_t wait(int *status);
void usleep(unsigned long used);
unsigned int sleep(unsigned int seconds);
int seteuid( uid_t euid );
int setreuid( uid_t ruid, uid_t euid );

uid_t getuid(void);
uid_t geteuid(void);
//pid_t getpid(void);
sighandler_t signal(int signum, sighandler_t handler);
int pause(void);
int getpriority(int which, int who);
int setpriority(int which, int who, int prio);

int sched_get_priority_max(int policy);
int sched_get_priority_min(int policy);

int sched_setscheduler(pid_t pid, int policy,
											 const struct sched_param *param);
int sched_getscheduler(pid_t pid);

int		getpagesize(void);
void*	valloc( int A_Size );

int		major( int A_val );
int		minor( int A_Val );

// ALSA
int  snd_pcm_open (snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode);
int snd_pcm_hw_params_malloc  ( snd_pcm_hw_params_t **  ptr );
int snd_pcm_sw_params_malloc  ( snd_pcm_sw_params_t **  ptr );
int snd_pcm_hw_params_any  ( snd_pcm_t *  pcm, snd_pcm_hw_params_t* params );
int snd_pcm_hw_params_set_access  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_access_t access );
int snd_pcm_hw_params_set_format  ( snd_pcm_t * pcm, snd_pcm_hw_params_t* params,	snd_pcm_format_t format );
int snd_pcm_hw_params_set_channels  ( snd_pcm_t * pcm, snd_pcm_hw_params_t * params, unsigned int  val );
int snd_pcm_hw_params_set_rate_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, unsigned int* val, int* dir	);
int snd_pcm_hw_params_get_buffer_time_max  ( const snd_pcm_hw_params_t* params, unsigned int* val, int* dir	);
int snd_pcm_hw_params_set_period_time_near  ( snd_pcm_t* pcm,	snd_pcm_hw_params_t* params, unsigned int* val,	int* dir );
int snd_pcm_hw_params_set_period_size_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val, int* dir );
int snd_pcm_hw_params_get_buffer_size  (const snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val );
int snd_pcm_hw_params_set_buffer_time_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params,	unsigned int* val, int* dir ); 
int snd_pcm_hw_params_set_buffer_size_near  ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val ); 
int snd_pcm_hw_params ( snd_pcm_t* pcm, snd_pcm_hw_params_t* params );
int snd_pcm_hw_params_get_period_size  ( const snd_pcm_hw_params_t* params, snd_pcm_uframes_t* val,	int* dir );
int snd_pcm_sw_params_current  ( snd_pcm_t* pcm, snd_pcm_sw_params_t *  params );
int snd_pcm_sw_params_set_avail_min  ( snd_pcm_t *  pcm,	snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val	); 
int snd_pcm_sw_params_set_start_threshold  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val );
int snd_pcm_sw_params_set_stop_threshold  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params, snd_pcm_uframes_t  val );
int snd_pcm_sw_params  ( snd_pcm_t *  pcm, snd_pcm_sw_params_t *  params  );
int snd_pcm_format_physical_width (	snd_pcm_format_t format );
void snd_pcm_hw_params_free  ( snd_pcm_hw_params_t *  obj );
void snd_pcm_sw_params_free  ( snd_pcm_sw_params_t *  obj );
int snd_pcm_drain  ( snd_pcm_t *  pcm   );
int snd_pcm_close  ( snd_pcm_t *  pcm   );
snd_pcm_sframes_t snd_pcm_readi  ( snd_pcm_t *  pcm, void *  buffer, snd_pcm_uframes_t  size 	);
int snd_pcm_recover  ( snd_pcm_t *  pcm, int  err, int  silent );
int snd_pcm_prepare  ( snd_pcm_t *  pcm   );
int snd_pcm_nonblock  ( snd_pcm_t *  pcm,	int  nonblock );
int snd_pcm_pause ( snd_pcm_t *  pcm, int  enable );
snd_pcm_sframes_t snd_pcm_writei  ( snd_pcm_t *  pcm, const void *  buffer,	snd_pcm_uframes_t  size );

int tcflush(int fd, int queue_selector);
int tcsetattr(int fd, int optional_actions,	const struct termios *termios_p);
int tcdrain(int fd);

int epoll_create(int size);
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

int sysinfo(struct sysinfo *info);

/* emulation API */
int		GrElaDrvAdd( char* A_StrName, Ptr_file_operations A_PtrFop );

//====================================================================
//out Environment conditional
#endif
