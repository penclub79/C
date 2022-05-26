/*
	odd library
*/

//====================================================================
// uses
#include	<GrLnxApp.h>

#ifdef	LINUX_APP
#include	<sys/time.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<time.h>
#include	<linux/rtc.h>
#include	<sys/times.h>
#endif
#include	<GrTypeBase.h>
#include <GrDebug.h>

#include	<GrDumyTool.h>

#include 	<string.h>
#include 	<stdlib.h>
#include 	<errno.h>

#include	<Hw/OddV2/GrOddLib.h>
#include  <Hw/OddV2/GrOddDef.h>
#include	<Hw/OddV2/GrOddScg.h>
#include	<Hw/OddV2/GrOddDevBase.h>

//====================================================================
//local const
#define	G0_MAXADDR	0x1FFFFFL

#define	DO4(a)	a;a;a;a;
#define	DO8(a)	a; a; a; a; a; a; a; a;
#define	DO13(a)	a;a;a;a;a;a;a;a;a;a;a;a;a;

#define	is_space(c)	 ((c) == ' ' || (c) == '\t')
#define	is_digit(c)	 ((c) >= '0' && (c) <= '9')
#define	is_hex(c)	(\
	((c) >= 'a' && (c) <= 'f') || \
	((c) >= 'A' && (c) <= 'F'))

#define	to_lower(c)	(((c) >= 'A' && (c) <= 'Z') ? (c) - 'A'+'a' : (c))

#define	BPW	16
#define	BPB	8

//cdtext
#define	PTI_TITLE	0x80	/* Album name and Track titles */
#define	PTI_PERFORMER	0x81	/* Singer/player/conductor/orchestra */
#define	PTI_SONGWRITER	0x82	/* Name of the songwriter */
#define	PTI_COMPOSER	0x83	/* Name of the composer */
#define	PTI_ARRANGER	0x84	/* Name of the arranger */
#define	PTI_MESSAGE	0x85	/* Message from content provider or artist */
#define	PTI_DISK_ID	0x86	/* Disk identification information */
#define	PTI_GENRE	0x87	/* Genre identification / information */
#define	PTI_TOC		0x88	/* TOC information */
#define	PTI_TOC2	0x89	/* Second TOC */
#define	PTI_RES_8A	0x8A	/* Reserved 8A */
#define	PTI_RES_8B	0x8B	/* Reserved 8B */
#define	PTI_RES_8C	0x8C	/* Reserved 8C */
#define	PTI_CLOSED_INFO	0x8D	/* For internal use by content provider */
#define	PTI_ISRC	0x8E	/* UPC/EAN code of album and ISRC for tracks */
#define	PTI_SIZE	0x8F	/* Size information of the block */

#define	EXT_DATA 0x80		/* Extended data indicator in track_no */
#define	DBCC	 0x80		/* Double byte char indicator in block */

//astoll
#define	is_space(c)	 ((c) == ' ' || (c) == '\t')
#define	is_digit(c)	 ((c) >= '0' && (c) <= '9')
#define	is_hex(c)	(\
	((c) >= 'a' && (c) <= 'f') || \
	((c) >= 'A' && (c) <= 'F'))

#define	to_lower(c)	(((c) >= 'A' && (c) <= 'Z') ? (c) - 'A'+'a' : (c))

//raisecond
#define	next_frame(vp)	vp = (((SIGBLK *)(vp))->sb_savfp);

//cvmod
#define	O_BINARY	0

//default
#define	MAXLINE	512

//audiosize
#define	WAV_RIFF_MAGIC		"RIFF"		/* Magic for file format    */
#define	WAV_WAVE_MAGIC		"WAVE"		/* Magic for Waveform Audio */
#define	WAV_FMT_MAGIC		"fmt "		/* Start of Waveform format */
#define	WAV_DATA_MAGIC		"data"		/* Start of data chunk	    */
#define	WAV_FORMAT_PCM		0x0001		/* Linear PCM format	    */
#define	WAV_FORMAT_ULAW		0x0101		/* American ISDN Telephonie */
#define	WAV_FORMAT_ALAW		0x0102		/* International ISDN Tel.  */
#define	WAV_FORMAT_ADPCM	0x0103		/* ADPCM format		    */

#define	le_a_to_u_short(a)	((unsigned short) \
	((((unsigned char *)a)[0]	& 0xFF) | \
	(((unsigned char *)a)[1] << 8	& 0xFF00)))

#define	le_a_to_u_long(a)	((unsigned long) \
	((((unsigned char *)a)[0]	& 0xFF) | \
	(((unsigned  char *)a)[1] << 8	& 0xFF00) | \
	(((unsigned  char *)a)[2] << 16	& 0xFF0000) | \
	(((unsigned  char *)a)[3] << 24	& 0xFF000000UL)))

#define	SUN_AU_MAGIC		".snd"
#define	SUN_AU_UNKNOWN_LEN	((Uint)~0)
#define	SUN_AU_ULAW8		1		/* American ISDN Telephonie */
#define	SUN_AU_LINEAR8		2		/* Linear PCM 8 bit/channel  */
#define	SUN_AU_LINEAR16		3		/* Linear PCM 16 bit/channel */
#define	SUN_AU_LINEAR24		4		/* Linear PCM 24 bit/channel */
#define	SUN_AU_LINEAR32		5		/* Linear PCM 32 bit/channel */
#define	SUN_AU_FLOAT		6		/* 32 bit IEEE floatingpoint */
#define	SUN_AU_DOUBLE		7		/* 64 bit IEEE floatingpoint */
#define	SUN_AU_G721		23		/* 4 bit CCITT G.721 ADPCM  */
#define	SUN_AU_G722		24		/* CCITT G.722 ADPCM	    */
#define	SUN_AU_G723_3		25		/* 3 bit CCITT G.723 ADPCM  */
#define	SUN_AU_G723_5		26		/* 5 bit CCITT G.723 ADPCM  */
#define	SUN_AU_ALAW8		27		/* International ISDN Tel.  */

//sector
#define	IS_SECHDR(p)	((p)[0] == 0 &&				\
	(p)[1] == 0xFF && (p)[2] == 0xFF &&	\
	(p)[3] == 0xFF && (p)[4] == 0xFF &&	\
	(p)[5] == 0xFF && (p)[6] == 0xFF &&	\
	(p)[7] == 0xFF && (p)[8] == 0xFF &&	\
	(p)[9] == 0xFF && (p)[10] == 0xFF &&	\
	(p)[11] == 0)

//fifo
#define	MIN_BUFFERS	3
#define	MSECS	1000
#define	SECS	(1000*MSECS)
#define	WRITER_DELAY	(20*MSECS)
#define	WRITER_MAXWAIT	(240*SECS)	/* 240 seconds max wait for data */
#define	READER_DELAY	(80*MSECS)
#define	READER_MAXWAIT	(240*SECS)	/* 240 seconds max wait for reader */

//====================================================================
//local macro

//====================================================================
//local type
struct trackdesc {
	unsigned char	res0;

	unsigned char	control		: 4;
	unsigned char	adr		: 4;

	unsigned char	track;
	unsigned char	res3;
	unsigned char	addr[4];
};

struct diskinfo {
	struct tocheader	hd;
	struct trackdesc	desc[1];
};

struct profiles {
	unsigned short	code;
	char		*name;
} pl[] = {
	{ 0x0000,	"Reserved", },
	{ 0x0001,	"Non -removable Disk", },
	{ 0x0002,	"Removable Disk", },
	{ 0x0003,	"MO Erasable", },
	{ 0x0004,	"MO Write Once", },
	{ 0x0005,	"AS-MO", },

	/* 0x06..0x07 is reserved */

	{ 0x0008,	"CD-ROM", },
	{ 0x0009,	"CD-R", },
	{ 0x000A,	"CD-RW", },

	/* 0x0B..0x0F is reserved */

	{ 0x0010,	"DVD-ROM", },
	{ 0x0011,	"DVD-R sequential recording", },
	{ 0x0012,	"DVD-RAM", },
	{ 0x0013,	"DVD-RW restricted overwrite", },
	{ 0x0014,	"DVD-RW sequential recording", },
	{ 0x0015,	"DVD-R/DL sequential recording", },
	{ 0x0016,	"DVD-R/DL layer jump recording", },
	{ 0x0017,	"DVD-RW/DL", },

	/* 0x18..0x19 is reserved */

	{ 0x001A,	"DVD+RW", },
	{ 0x001B,	"DVD+R", },

	{ 0x0020,	"DDCD-ROM", },
	{ 0x0021,	"DDCD-R", },
	{ 0x0022,	"DDCD-RW", },

	{ 0x002A,	"DVD+RW/DL", },
	{ 0x002B,	"DVD+R/DL", },

	{ 0x0040,	"BD-ROM", },
	{ 0x0041,	"BD-R sequential recording", },
	{ 0x0042,	"BD-R random recording", },
	{ 0x0043,	"BD-RE", },

	/* 0x44..0x4F is reserved */

	{ 0x0050,	"HD DVD-ROM", },
	{ 0x0051,	"HD DVD-R", },
	{ 0x0052,	"HD DVD-RAM", },
	{ 0x0053,	"HD DVD-RW", },

	/* 0x54..0x57 is reserved */

	{ 0x0058,	"HD DVD-R/DL", },

	{ 0x005A,	"HD DVD-RW/DL", },

	{ 0xFFFF,	"No standard Profile", },
};

__u16 crctab[1<<BPB] ={
    0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
    0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
    0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
    0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
    0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
    0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
    0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
    0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
    0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
    0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
    0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
    0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
    0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
    0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
    0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
    0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
    0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
    0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
    0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
    0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
    0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
    0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
    0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
    0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
    0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
    0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
    0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
    0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
    0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
    0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
    0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
    0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0,
};

__u8 fliptab[BPB] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80,
};

//fcons
char	*fmtab[] = {
			"",	/* 0	FI_NONE				*/
			"r",	/* 1	FI_READ				*/
			"w",	/* 2	FI_WRITE		**1)	*/
			"r+",	/* 3	FI_READ  | FI_WRITE		*/
			"b",	/* 4	FI_NONE  | FI_BINARY		*/
			"rb",	/* 5	FI_READ  | FI_BINARY		*/
			"wb",	/* 6	FI_WRITE | FI_BINARY	**1)	*/
			"r+b",	/* 7	FI_READ  | FI_WRITE | FI_BINARY	*/
/* + FI_APPEND	*/	"",	/* 0	FI_NONE				*/
/* ...		*/	"r",	/* 1	FI_READ				*/
			"a",	/* 2	FI_WRITE		**1)	*/
			"a+",	/* 3	FI_READ  | FI_WRITE		*/
			"b",	/* 4	FI_NONE  | FI_BINARY		*/
			"rb",	/* 5	FI_READ  | FI_BINARY		*/
			"ab",	/* 6	FI_WRITE | FI_BINARY	**1)	*/
			"a+b",	/* 7	FI_READ  | FI_WRITE | FI_BINARY	*/
		};

//cdtext
typedef struct textpack {
	unsigned char	pack_type;	/* Pack Type indicator	*/
	char	track_no;	/* Track Number (0..99)	*/
	char	seq_number;	/* Sequence Number	*/
	char	block_number;	/* Block # / Char pos	*/
	char	text[12];	/* CD-Text Data field	*/
	char	crc[2];		/* CRC 16		*/
} txtpack_t;

typedef struct textsizes {
	char	charcode;
	char	first_track;
	char	last_track;
	char	copyr_flags;
	char	pack_count[16];
	char	last_seqnum[8];
	char	language_codes[8];
} txtsize_t;

typedef struct textargs {
	txtpack_t	*tp;
	char		*p;
	txtsize_t	*tsize;
	int		seqno;
} txtarg_t;

struct disk_rcap {
	msf_t	ci_msf;				/* Lead in start time	    */
	long	ci_cap;				/* Lead out start time	    */
	long	ci_rcap;			/* Abs max lead out start   */
};

static	struct disk_rcap rcap[] = {
	{{97, 35, 44}, 359849, 449700 },	/*! Unknown 99 min (99:58/00) */
	{{97, 31, 00}, 359849, 368923 },	/*! Arita CD-R 80	    */
	{{97, 26, 50}, 359849, 369096 },	/*! Lead Data CD-R 80	    */
	{{97, 26, 12}, 359849, 368000 },	/*X POSTECH 80 Min	    */
	{{97, 25, 00}, 359849, 374002 },	/* TDK 80 Minuten	    */
	{{97, 20, 14}, 359700, 376386 },	/*! Albrechts DataFile Plus */
	{{97, 35, 44}, 359100, 368791 },	/*! NoName BC-1 700 Mb/80 Min */

	{{97, 26, 60}, 337350, 349030 },	/* Koch gr? CD-R74PRO	    */
	{{97, 26, 50}, 337050, 351205 },	/* Saba			    */
	{{97, 26, 00}, 337050, 351411 },	/*!DGN (FORNET)		    */
	{{97, 22, 40}, 336631, 349971 },	/* Targa gr? CD-R74	    */
	{{97, 26, 50}, 336631, 351727 },	/*! Sunstar (Lead Data)	    */
	{{97, 26, 55}, 336631, 350474 },	/*! NoName ZAP (Lead Data)  */

	{{97, 27, 28}, 336601, 346489 },	/*! BTC CD-R (Princo)	    */
	{{97, 27, 30}, 336601, 351646 },	/*! Pioneer blau CDM-W74S   */
	{{97, 27, 31}, 336601, 351379 },	/* Pioneer blau CDM-W74S    */
	{{97, 27, 33}, 336601, 347029 },	/*! Pioneer braun CDM-V74S  */
	{{97, 26, 40}, 336225, 346210 },	/* Fuji Silver Disk	    */
	{{97, 28, 10}, 336225, 348757 },	/*!GigaStorage Cursor CD-R  */
	{{97, 31, 00}, 336225, 345460 },	/* Arita gr?		    */
	{{97, 25, 28}, 336075, 352879 },	/* Maxell gold CD-R74G	    */
	{{97, 24, 01}, 336075, 346856 },	/*!Philips Premium Silver   */
	{{97, 24, 00}, 336075, 346741 },	/* Philips gr? CD-R74	    */

	{{97, 22, 41}, 335206, 349385 },	/* Octek gr?		    */
	{{97, 34, 20}, 335100, 342460 },	/* Verbatim DataLifePlus    */
	{{97, 33, 00}, 335100, 344634 },	/*!ITS Singapore (braun/gr?) */
	{{97, 32, 19}, 335100, 343921 },	/*!Prodisc silber/silber    */
	{{97, 25, 21}, 335100, 346013 },	/* Maxell gr? CD-R74XL	    */
	{{97, 27, 00}, 335100, 353448 },	/* TDK gr? CD-RXG74	    */
	{{97, 27, 31}, 335100, 351862 },	/*!Maxell CD-R74MU (Musik)  */
	{{97, 27, 33}, 335100, 351336 },	/* Pioneer RDD-74A	    */

	{{97, 26, 60}, 334259, 349036 },	/* BASF gr?		    */
	{{97, 28, 21}, 333976, 348217 },	/*! Noname-B (MMMM)	    */
	{{97, 28, 20}, 333976, 346485 },	/* Koch  gr?  CD-R74 PRO   */
	{{97, 32, 00}, 333975, 345736 },	/* Imation 3M		    */
	{{97, 32, 00}, 333975, 348835 },	/* TDK Reflex X	    CD-R74  */
	{{97, 30, 18}, 333899, 344857 },	/* HiSpace  gr?	    */
	{{97, 27, 66}, 333750, 352726 },	/*!Philips Megalife (Musik) */
	{{97, 28, 43}, 333750, 345344 },	/*!MMore CD-R		    */
	{{97, 27, 65}, 333750, 348343 },	/* Ricoh gold		    */

	{{97, 27, 00}, 333750, 336246 },	/* BestMedia gr?   CD-R74  */
	{{97, 27, 28}, 333491, 347473 },	/* Fuji gr? (alt)	    */
	{{97, 24, 48}, 333491, 343519 },	/* BASF (alt)		    */
	{{97, 27, 55}, 333235, 343270 },	/* Teac gold CD-R74	    */
	{{97, 27, 45}, 333226, 343358 },	/* Kodak gold		    */
	{{97, 28, 20}, 333226, 346483 },	/* SAST gr?		    */
	{{97, 27, 45}, 333226, 343357 },	/* Mitsumi gold		    */
	{{97, 28, 25}, 333226, 346481 },	/* Cedar Gr?		    */
	{{97, 23, 00}, 333226, 346206 },	/* Fuji gr? (alt)	    */
	{{97, 33, 00}, 333225, 349623 },	/* DataFile Albrechts	    */
	{{97, 24, 24}, 333198, 342536 },	/*!SUN CD Recordable	    */

	{{97, 27, 19}, 332850, 348442 },	/* Plasmon gold PCD-R74	    */
	{{97, 32, 00}, 96600,  106502 },	/* TDK 80mm (for music only) */

	/*
	 * List end marker
	 */
	{{00, 00, 00}, 0L, 0L },
};

//audiosize
typedef struct {
	unsigned char	magic[4];
	unsigned char	hdr_size[4];
	unsigned char	data_size[4];
	unsigned char	encoding[4];
	unsigned char	sample_rate[4];
	unsigned char	channels[4];
} sun_au_t;

typedef struct {
	unsigned char	ckid[4];
	unsigned char	cksize[4];
} chunk_t;

typedef struct {
	unsigned char	wave[4];
} riff_chunk;

typedef struct {
	unsigned char	fmt_tag[2];
	unsigned char	channels[2];
	unsigned char	sample_rate[4];
	unsigned char	av_byte_rate[4];
	unsigned char	block_size[2];
	unsigned char	bits_per_sample[2];
} fmt_chunk;

//fifo
struct faio_stats {
	long	puts;
	long	gets;
	long	empty;
	long	full;
	long	done;
	long	cont_low;
	int	users;
} *sp;

char	*onames[] = {
	"none",
	"writer",
	"faio",
	"reader",
};

//====================================================================
//local function
int	scsi_get_performance(void* A_PtrScg, caddr_t bp, int cnt,	int	ndesc, int type, int datatype);
int	scsi_set_streaming(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp);
BOOL	has_mode_page(void* A_PtrScg, int page, char *pagename, int *lenp);

__u32		updcrc(__u32 p_crc, __u8 *cp, __u32 cnt);

//cdtext
void	setuptextdata(unsigned char *bp, int len);
BOOL	cdtext_crc_ok(struct textpack *p);
BOOL	anytext(int pack_type, int tracks, track_t *trackp);
void	fillup_pack(txtarg_t *ap);
void	fillpacks(txtarg_t *ap, char *from, int len, int track_no, int pack_type);
void	eight2six(unsigned char *in, unsigned char *out);

//subchan
void	filldsubq(unsigned char *sub, int ca, int t, int i, msf_t *mrp, msf_t *mp);
void	fillmcn(unsigned char *sub, unsigned char *mcn);
void	fillisrc(unsigned char *sub, unsigned char *isrc);
int		ascii2q(int c);
void	qpto16(unsigned char *sub, unsigned char *subq, int dop);

//raisecond
void	raiseabort(char	*signame);
BOOL	framehandle(SIGBLK *sp, char	*handlename, char	*signame, long arg2);

//flag
int _more_flags(FILE *fp);
void	_io_set_my_flag(FILE *fp,	int	flag);

//fcons
FILE*	_fcons(FILE	*fd, int f,	int	flag);

//open fd
int _openfd(char *name,	int omode);

//cvmod
int	_cvmod(char	*mode, int *omode, int *flag);

//====================================================================
//var
BOOL	is_atapi;
int	scsi_compliant;
unsigned char	_subq[110][12];
int	_nsubh;
unsigned char	*textsub;
int	textlen;

extern Cls_GrOddDevBase* V_PtrDevMmc;
extern Cls_GrOddDevBase* V_PtrDevMmcSony;
extern Cls_GrOddDevBase* V_PtrDevCd;
extern Cls_GrOddDevBase* V_PtrDevOldCd;
extern Cls_GrOddDevBase* V_PtrDevDvd;
extern Cls_GrOddDevBase* V_PtrDevDvdPlus;
extern Cls_GrOddDevBase* V_PtrDevDvdPlusR;
extern Cls_GrOddDevBase* V_PtrDevDvdPlusRW;
extern Cls_GrOddDevBase* V_PtrCdDvd;

//====================================================================
//--------------------------------------------------------------------
BOOL	unit_ready(void* A_ObjScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_ObjScg;
		struct	scg_cmd	*scmd = &scgp->m_Scmd;

		if (test_unit_ready(scgp) >= 0)		/* alles OK */
		{
			return (TRUE);
		}
		else if (scmd->error >= SCG_FATAL)	/* nicht selektierbar */
		{
			return (FALSE);
		}
	
		if (scgp->Scg_Sense_Key() == SC_UNIT_ATTENTION)
		{
			if (test_unit_ready(scgp) >= 0)	/* alles OK */
			{
				return (TRUE);
			}
		}
		if ((scgp->Scg_Cmd_Status() & ST_BUSY) != 0)
		{
			usleep(500000);
			if (test_unit_ready(scgp) >= 0)	/* alles OK */
			{
				return (TRUE);
			}
		}
		if (scgp->Scg_Sense_Key() == -1)
		{	/* non extended Sense */
			if (scgp->Scg_Sense_Code() == 4)	/* NOT_READY */
			{
				return (FALSE);
			}
			return (TRUE);
		}

		return (scgp->Scg_Sense_Key() != SC_NOT_READY);
}
//--------------------------------------------------------------------
int 	test_unit_ready(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		struct	scg_cmd	*scmd = &scgp->m_Scmd;
	
		memset((caddr_t)scmd, '\0', sizeof (*scmd));
		scmd->addr = (caddr_t)0;
		scmd->size = 0;
		scmd->flags = SCG_DISRE_ENA | SCG_SILENT;
		scmd->cdb_len = SC_G0_CDBLEN;
		scmd->sense_len = CCS_SENSE_LEN;
		scmd->cdb.g0_cdb.cmd = SC_TEST_UNIT_READY;
		scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	
		scgp->m_CmdName = "test unit ready"; 

		return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
BOOL	getdev(void* A_PtrScg, BOOL	print)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	got_inquiry = TRUE;
	char	vendor_info[8+1];
	char	prod_ident[16+1];
	char	prod_revision[4+1];
	int	inq_len = 0;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	struct scsi_inquiry *inq = &scgp->inq;

	memset((caddr_t)inq, '\0', sizeof (*inq));
	scgp->m_Dev = DEV_UNKNOWN;
	(void) unit_ready(scgp);
	if (scmd->error >= SCG_FATAL && !(scmd->scb.chk && scmd->sense_count > 0))
	{
		return (FALSE);
	}

	if (inquiry(scgp, (caddr_t)inq, sizeof (*inq)) < 0) 
	{
		got_inquiry = FALSE;
	} 
	else 
	{
		inq_len = sizeof (*inq) - scgp->m_Scmd.resid;
	}

	if (!got_inquiry)
	{
		if (scmd->scb.chk && scmd->sense_count == 4)
		{
			(void) qic02(scgp, 0x12);

			if (qic02(scgp, 1) < 0)
			{
				scgp->m_Dev = DEV_ACB40X0;
			}
			else
			{
				scgp->m_Dev = DEV_SC4000;
				inq->type = INQ_SEQD;
				inq->removable = 1;
			}
		}
	} 

	strncpy(vendor_info, inq->vendor_info, sizeof (inq->vendor_info));
	strncpy(prod_ident, inq->prod_ident, sizeof (inq->prod_ident));
	strncpy(prod_revision, inq->prod_revision, sizeof (inq->prod_revision));

	vendor_info[sizeof (inq->vendor_info)] = '\0';
	prod_ident[sizeof (inq->prod_ident)] = '\0';
	prod_revision[sizeof (inq->prod_revision)] = '\0';

	switch (inq->type)
	{
	case INQ_DASD:
		if (inq->add_len == 0 && inq->vendor_info[0] != '\0') {
			unsigned char	*p;

			if (inq_len >= 36)
				inq->add_len = 31;

			for (p = (unsigned char *)&inq->vendor_info[0];
					p < (unsigned char *)&inq->prod_revision[4];
									p++) {
				if (*p < 0x20 || *p > 0x7E) {
					inq->add_len = 0;
					break;
				}
			}
		}
		if (inq->add_len == 0) {
			if (scgp->m_Dev == DEV_UNKNOWN && got_inquiry) {
				scgp->m_Dev = DEV_ACB5500;
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-5500        FAKE");
			} else switch (scgp->m_Dev) {
			case DEV_ACB40X0:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-40X0        FAKE");
				break;
			case DEV_ACB4000:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4000        FAKE");
				break;
			case DEV_ACB4010:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4010        FAKE");
				break;
			case DEV_ACB4070:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4070        FAKE");
				break;
			}
		} else if (inq->add_len < 31) {
			scgp->m_Dev = DEV_NON_CCS_DSK;

		} else if (strbeg("EMULEX", vendor_info)) {
			if (strbeg("MD21", prod_ident))
				scgp->m_Dev = DEV_MD21;
			if (strbeg("MD23", prod_ident))
				scgp->m_Dev = DEV_MD23;
			else
				scgp->m_Dev = DEV_CCS_GENDISK;
		} else if (strbeg("ADAPTEC", vendor_info)) {
			if (strbeg("ACB-4520", prod_ident))
				scgp->m_Dev = DEV_ACB4520A;
			if (strbeg("ACB-4525", prod_ident))
				scgp->m_Dev = DEV_ACB4525;
			else
				scgp->m_Dev = DEV_CCS_GENDISK;
		} else if (strbeg("SONY", vendor_info) &&
					strbeg("SMO-C501", prod_ident)) {
			scgp->m_Dev = DEV_SONY_SMO;
		} else {
			scgp->m_Dev = DEV_CCS_GENDISK;
		}
		break;

	case INQ_SEQD:
		if (scgp->m_Dev == DEV_SC4000) {
			strcpy(inq->vendor_info,
				"SYSGEN  SC4000          FAKE");
		} else if (inq->add_len == 0 &&
					inq->removable &&
						inq->ansi_version == 1) {
			scgp->m_Dev = DEV_MT02;
			strcpy(inq->vendor_info,
				"EMULEX  MT02            FAKE");
		}
		break;
	case INQ_ROMD:
	case INQ_WORM:
		if (strbeg("RXT-800S", prod_ident))
			scgp->m_Dev = DEV_RXT800S;

		if (strbeg("ACER", vendor_info)) {
			if (strbeg("CR-4020C", prod_ident))
				scgp->m_Dev = DEV_RICOH_RO_1420C;

		} else if (strbeg("CREATIVE", vendor_info)) {
			if (strbeg("CDR2000", prod_ident))
				scgp->m_Dev = DEV_RICOH_RO_1060C;

		} else if (strbeg("GRUNDIG", vendor_info)) {
			if (strbeg("CDR100IPW", prod_ident))
				scgp->m_Dev = DEV_CDD_2000;

		} else if (strbeg("JVC", vendor_info)) {
			if (strbeg("XR-W2001", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			else if (strbeg("XR-W2010", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			else if (strbeg("R2626", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("MITSBISH", vendor_info)) {
		} else if (strbeg("MITSUMI", vendor_info)) {
			scgp->m_Dev = DEV_CDD_522;
		} else if (strbeg("OPTIMA", vendor_info)) {
			if (strbeg("CD-R 650", prod_ident))
				scgp->m_Dev = DEV_SONY_CDU_924;

		} else if (strbeg("PHILIPS", vendor_info) ||
				strbeg("IMS", vendor_info) ||
				strbeg("KODAK", vendor_info) ||
				strbeg("HP", vendor_info)) {

			if (strbeg("CDD521/00", prod_ident))
				scgp->m_Dev = DEV_CDD_521_OLD;
			else if (strbeg("CDD521/02", prod_ident))
				scgp->m_Dev = DEV_CDD_521_OLD;		/* PCD 200R? */
			else if (strbeg("CDD521", prod_ident))
				scgp->m_Dev = DEV_CDD_521;

			if (strbeg("CDD522", prod_ident))
				scgp->m_Dev = DEV_CDD_522;
			if (strbeg("PCD225", prod_ident))
				scgp->m_Dev = DEV_CDD_522;
			if (strbeg("KHSW/OB", prod_ident))	/* PCD600 */
				scgp->m_Dev = DEV_PCD_600;
			if (strbeg("CDR-240", prod_ident))
				scgp->m_Dev = DEV_CDD_2000;

			if (strbeg("CDD20", prod_ident))
				scgp->m_Dev = DEV_CDD_2000;
			if (strbeg("CDD26", prod_ident))
				scgp->m_Dev = DEV_CDD_2600;

			if (strbeg("C4324/C4325", prod_ident))
				scgp->m_Dev = DEV_CDD_2000;
			if (strbeg("CD-Writer 6020", prod_ident))
				scgp->m_Dev = DEV_CDD_2600;

		} else if (strbeg("PINNACLE", vendor_info)) {
			if (strbeg("RCD-1000", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD5020", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD5040", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD 4X4", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("PIONEER", vendor_info)) {
			if (strbeg("CD-WO DW-S114X", prod_ident))
				scgp->m_Dev = DEV_PIONEER_DW_S114X;
			else if (strbeg("CD-WO DR-R504X", prod_ident))	/* Reoprt from philip@merge.com */
				scgp->m_Dev = DEV_PIONEER_DW_S114X;
			else if (strbeg("DVD-R DVR-S101", prod_ident))
				scgp->m_Dev = DEV_PIONEER_DVDR_S101;

		} else if (strbeg("PLASMON", vendor_info)) {
			if (strbeg("RF4100", prod_ident))
				scgp->m_Dev = DEV_PLASMON_RF_4100;
			else if (strbeg("CDR4220", prod_ident))
				scgp->m_Dev = DEV_CDD_2000;

		} else if (strbeg("PLEXTOR", vendor_info)) {
			if (strbeg("CD-R   PX-R24CS", prod_ident))
				scgp->m_Dev = DEV_RICOH_RO_1420C;

		} else if (strbeg("RICOH", vendor_info)) {
			if (strbeg("RO-1420C", prod_ident))
				scgp->m_Dev = DEV_RICOH_RO_1420C;
			if (strbeg("RO1060C", prod_ident))
				scgp->m_Dev = DEV_RICOH_RO_1060C;

		} else if (strbeg("SAF", vendor_info)) {	/* Smart & Friendly */
			if (strbeg("CD-R2004", prod_ident) ||
			    strbeg("CD-R2006 ", prod_ident))
				scgp->m_Dev = DEV_SONY_CDU_924;
			else if (strbeg("CD-R2006PLUS", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			else if (strbeg("CD-RW226", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;
			else if (strbeg("CD-R4012", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("SANYO", vendor_info)) {
			if (strbeg("CD-WO CRD-R24S", prod_ident))
				scgp->m_Dev = DEV_CDD_521;

		} else if (strbeg("SONY", vendor_info)) {
			if (strbeg("CD-R   CDU92", prod_ident) ||
			    strbeg("CD-R   CDU94", prod_ident))
				scgp->m_Dev = DEV_SONY_CDU_924;

		} else if (strbeg("TEAC", vendor_info)) {
			if (strbeg("CD-R50S", prod_ident) ||
			    strbeg("CD-R55S", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("TRAXDATA", vendor_info) ||
				strbeg("Traxdata", vendor_info)) {
			if (strbeg("CDR4120", prod_ident))
				scgp->m_Dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("T.YUDEN", vendor_info)) {
			if (strbeg("CD-WO EW-50", prod_ident))
				scgp->m_Dev = DEV_TYUDEN_EW50;

		} else if (strbeg("WPI", vendor_info)) {	/* Wearnes */
			if (strbeg("CDR-632P", prod_ident))
				scgp->m_Dev = DEV_CDD_2600;

		} else if (strbeg("YAMAHA", vendor_info)) {
			if (strbeg("CDR10", prod_ident))
				scgp->m_Dev = DEV_YAMAHA_CDR_100;
			if (strbeg("CDR200", prod_ident))
				scgp->m_Dev = DEV_YAMAHA_CDR_400;
			if (strbeg("CDR400", prod_ident))
				scgp->m_Dev = DEV_YAMAHA_CDR_400;

		} else if (strbeg("MATSHITA", vendor_info)) {
			if (strbeg("CD-R   CW-7501", prod_ident))
				scgp->m_Dev = DEV_MATSUSHITA_7501;
			if (strbeg("CD-R   CW-7502", prod_ident))
				scgp->m_Dev = DEV_MATSUSHITA_7502;
		}
		if (scgp->m_Dev == DEV_UNKNOWN) {
			if (strbeg("CDS615E", prod_ident))	/* Olympus */
				scgp->m_Dev = DEV_SONY_CDU_924;
		}
		if (scgp->m_Dev == DEV_UNKNOWN && inq->type == INQ_ROMD) {
			BOOL	cdrr	 = FALSE;
			BOOL	cdwr	 = FALSE;
			BOOL	cdrrw	 = FALSE;
			BOOL	cdwrw	 = FALSE;
			BOOL	dvd	 = FALSE;
			BOOL	dvdwr	 = FALSE;

			scgp->m_Dev = DEV_CDROM;

			if (mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw,
								&dvd, &dvdwr))
				scgp->m_Dev = DEV_MMC_CDROM;
			if (cdwr)
				scgp->m_Dev = DEV_MMC_CDR;
			if (cdwrw)
				scgp->m_Dev = DEV_MMC_CDRW;
			if (dvd)
				scgp->m_Dev = DEV_MMC_DVD;
			if (dvdwr)
				scgp->m_Dev = DEV_MMC_DVD_WR;
		}
		break;

	case INQ_PROCD:
		if (strbeg("BERTHOLD", vendor_info)) {
			if (strbeg("", prod_ident))
				scgp->m_Dev = DEV_HRSCAN;
		}
		break;

	case INQ_SCAN:
		scgp->m_Dev = DEV_MS300A;
		break;
	}

	if (!print)
		return (TRUE);

	if (scgp->m_Dev == DEV_UNKNOWN && !got_inquiry) {
		return (FALSE);
	}

	return (TRUE);
}
//--------------------------------------------------------------------
int 	inquiry(void* A_PtrScg,	caddr_t	bp,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		struct	scg_cmd	*scmd = &scgp->m_Scmd;
	
		memset(bp, '\0', cnt);
		memset((caddr_t)scmd, '\0', sizeof (*scmd)); 
		scmd->addr = bp;
		scmd->size = cnt;
		scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
		scmd->cdb_len = SC_G0_CDBLEN;
		scmd->sense_len = CCS_SENSE_LEN;
		scmd->cdb.g0_cdb.cmd = SC_INQUIRY;
		scmd->cdb.g0_cdb.lun = scg_lun(scgp);
		scmd->cdb.g0_cdb.count = cnt;
	
		scgp->m_CmdName = "inquiry";
	
		if (scgp->Scg_Cmd() < 0)
			return (-1);

		return (0);
}
//--------------------------------------------------------------------
BOOL	do_inquiry(void* A_PtrScg, int print)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		if (getdev(scgp, print))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
}
//--------------------------------------------------------------------
BOOL	is_unknown_dev(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	return (scgp->m_Dev == DEV_UNKNOWN);
}
//--------------------------------------------------------------------
int 	scsi_load(void* A_PtrScg,	void* A_PtrDev)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	int	key;
	int	code;

	if (dp && (dp->GetFlags() & CDR_CADDYLOAD) == 0) 
	{
		if (scsi_start_stop_unit(scgp, 1, 1, (dp->GetCmdFlags() & F_IMMED)) >= 0)
		{
			return (0);
		}
	}

	if (wait_unit_ready(scgp, 60))
		return (0);

	key = scgp->Scg_Sense_Key();
	code = scgp->Scg_Sense_Code();

	return (-1);
}
//--------------------------------------------------------------------
int		scsi_unload(void* A_PtrScg,	void* A_PtrDev)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;

		return (scsi_start_stop_unit(scgp, 0, 1, dp && (dp->GetCmdFlags() & F_IMMED)));
}
//--------------------------------------------------------------------
int		read_buff_cap(void* A_PtrScg,	long *sp,	long *fp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	char	resp[12];
	__u32	freespace;
	__u32	bufsize;
	int	per;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)resp;
	scmd->size = sizeof (resp);
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5C;		/* Read buffer cap */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, sizeof (resp));

	scgp->m_CmdName = "read buffer cap";

	if (scgp->Scg_Cmd() < 0)
		return (-1);

	bufsize   = a_to_u_4_byte(&resp[4]);
	freespace = a_to_u_4_byte(&resp[8]);
	if (sp)
		*sp = bufsize;
	if (fp)
		*fp = freespace;

	if (bufsize == 0)
		return (0);
	per = (100 * (bufsize - freespace)) / bufsize;
	if (per < 0)
		return (0);
	if (per > 100)
		return (100);
	return (per);
}
//--------------------------------------------------------------------
int	select_secsize(void* A_PtrScg, int	secsize)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct scsi_mode_data md;
	int	count = sizeof (struct scsi_mode_header) +
			sizeof (struct scsi_mode_blockdesc);

	(void) test_unit_ready(scgp);	/* clear any error situation */

	memset((caddr_t)&md, '\0', sizeof (md));
	md.header.blockdesc_len = 8;
	i_to_3_byte(md.blockdesc.lblen, secsize);

	return (mode_select(scgp, (unsigned char *)&md, count, 0, scgp->inq.data_format >= 2));
}
//--------------------------------------------------------------------
int	scsi_cdr_write(void* A_PtrScg, caddr_t bp, long sectaddr, long size, int	blocks, BOOL islast)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	return (write_xg1(scgp, bp, sectaddr, size, blocks));
}
//--------------------------------------------------------------------
int	write_xg0(void* A_PtrScg,	caddr_t	bp,	long addr, long	size, int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;

	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_WRITE;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	g0_cdbaddr(&scmd->cdb.g0_cdb, addr);
	scmd->cdb.g0_cdb.count = cnt;

	scgp->m_CmdName = "write_g0";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (size - scgp->scg_getresid());
}
//--------------------------------------------------------------------
int	write_xg1(void* A_PtrScg,	caddr_t	bp,	long addr, long	size, int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = SC_EWRITE;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "write_g1";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (size - scgp->scg_getresid());
}
//--------------------------------------------------------------------
int		read_session_offset(void* A_PtrScg, long	*offp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	diskinfo *dp;
	char	xb[256];
	int	len;

	dp = (struct diskinfo *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	if (read_toc(scgp, (caddr_t)xb, 0, sizeof (struct tocheader), 0, FMT_SINFO) < 0)
		return (-1);

	len = a_to_u_2_byte(dp->hd.len) + sizeof (struct tocheader)-2;
	if (len > (int)sizeof (xb)) {
		return (-1);
	}
	if (read_toc(scgp, (caddr_t)xb, 0, len, 0, FMT_SINFO) < 0)
		return (-1);

	dp = (struct diskinfo *)xb;
	if (offp)
		*offp = a_to_u_4_byte(dp->desc[0].addr);
	return (0);
}
//--------------------------------------------------------------------
int		scsi_start_stop_unit(void* A_PtrScg, int flg, int	loej, BOOL immed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x1B;	/* Start Stop Unit */
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.count = (flg ? 1:0) | (loej ? 2:0);

	if (immed)
		scmd->cdb.cmd_cdb[1] |= 0x01;

	scgp->m_CmdName = "start/stop unit";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		get_trackinfo(void* A_PtrScg, caddr_t	bp,	int	type,	int	addr,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	len;
	int	ret;

	memset(bp, '\0', cnt);

	if (read_track_info(scgp, bp, type, addr, 4) < 0)
		return (-1);

	len = a_to_u_2_byte(bp);
	len += 2;
	if (len > cnt)
		len = cnt;
	ret = read_track_info(scgp, bp, type, addr, len);

	return (ret);
}
//--------------------------------------------------------------------
int		read_disk_info(void* A_PtrScg, caddr_t	bp,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
		
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes */
	scmd->cdb.g1_cdb.cmd = 0x51;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "read disk info";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		mode_select(void* A_PtrScg,	unsigned char	*dp, int cnt,	int	smp, int pf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if (is_atapi)
		return (mode_select_sg0(scgp, dp, cnt, smp, pf));
	return (mode_select_g0(scgp, dp, cnt, smp, pf));
}
//--------------------------------------------------------------------
int		scsi_close_tr_session(void* A_PtrScg, int type, int track, BOOL immed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 8 * 60;		/* Needs up to 4 minutes */
	scmd->cdb.g1_cdb.cmd = 0x5B;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.addr[0] = type;
	scmd->cdb.g1_cdb.addr[3] = track;

	if (immed)
		scmd->cdb.g1_cdb.reladr = 1;

	scgp->m_CmdName = "close track/session";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		scsi_flush_cache(void* A_PtrScg, BOOL immed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 2 * 60;		/* Max: sizeof (CDR-cache)/150KB/s */
	scmd->cdb.g1_cdb.cmd = 0x35;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	if (immed)
		scmd->cdb.cmd_cdb[1] |= 0x02;

	scgp->m_CmdName = "flush cache";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int 	send_opc(void* A_PtrScg, caddr_t bp, int cnt,	int	doopc)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 60;
	scmd->cdb.g1_cdb.cmd = 0x54;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.reladr = doopc?1:0;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "send opc";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		scsi_set_speed(void* A_PtrScg, int readspeed, int writespeed, int rotctl)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xBB;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);

	if (readspeed < 0)
		i_to_2_byte(&scmd->cdb.g5_cdb.addr[0], 0xFFFF);
	else
		i_to_2_byte(&scmd->cdb.g5_cdb.addr[0], readspeed);
	if (writespeed < 0)
		i_to_2_byte(&scmd->cdb.g5_cdb.addr[2], 0xFFFF);
	else
		i_to_2_byte(&scmd->cdb.g5_cdb.addr[2], writespeed);

	scmd->cdb.cmd_cdb[1] |= rotctl & 0x03;

	scgp->m_CmdName = "set cd speed";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		scsi_blank(void* A_PtrScg, long addr, int blanktype, BOOL immed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 160 * 60; /* full blank at 1x could take 80 minutes */
	scmd->cdb.g5_cdb.cmd = 0xA1;	/* Blank */
	scmd->cdb.g0_cdb.high_addr = blanktype;
	g1_cdbaddr(&scmd->cdb.g5_cdb, addr);

	if (immed)
		scmd->cdb.g5_cdb.res |= 8;

	scgp->m_CmdName = "blank unit";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
BOOL	mmc_check(void* A_PtrScg, BOOL *cdrrp, BOOL *cdwrp, BOOL *cdrrwp, BOOL *cdwrwp,
	BOOL *dvdp, BOOL *dvdwp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	BOOL	was_atapi;
	struct	cd_mode_page_2A *mp;

	if (scgp->inq.type != INQ_ROMD)
		return (FALSE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	was_atapi = allow_atapi(scgp, TRUE);
	mp = mmc_cap(scgp, mode);
	allow_atapi(scgp, was_atapi);
	if (mp == NULL)
		return (FALSE);

	mmc_getval(mp, cdrrp, cdwrp, cdrrwp, cdwrwp, dvdp, dvdwp);

	return (TRUE);			/* Generic St_GrScsiIf-3/mmc CD	*/
}
//--------------------------------------------------------------------
void	mmc_getval(struct cd_mode_page_2A *mp, BOOL *cdrrp, BOOL *cdwrp, BOOL *cdrrwp, BOOL *cdwrwp,
	BOOL *dvdp, BOOL *dvdwp)
{
	BOOL	isdvd;				/* Any DVD reader	*/
	BOOL	isdvd_wr;			/* DVD writer (R / RAM)	*/
	BOOL	iscd_wr;			/* CD  writer		*/

	iscd_wr = (mp->cd_r_write != 0) ||	/* SCSI-3/mmc CD-R	*/
		    (mp->cd_rw_write != 0);	/* SCSI-3/mmc CD-RW	*/

	if (cdrrp)
		*cdrrp = (mp->cd_r_read != 0);	/* SCSI-3/mmc CD	*/
	if (cdwrp)
		*cdwrp = (mp->cd_r_write != 0);	/* SCSI-3/mmc CD-R	*/

	if (cdrrwp)
		*cdrrwp = (mp->cd_rw_read != 0); /* SCSI-3/mmc CD	*/
	if (cdwrwp)
		*cdwrwp = (mp->cd_rw_write != 0); /* SCSI-3/mmc CD-RW	*/

	isdvd =					/* SCSI-3/mmc2 DVD 	*/
		(mp->dvd_ram_read + mp->dvd_r_read  +
		    mp->dvd_rom_read) != 0;

	isdvd_wr =				/* SCSI-3/mmc2 DVD writer*/
		(mp->dvd_ram_write + mp->dvd_r_write) != 0;

	if (dvdp)
		*dvdp = isdvd;
	if (dvdwp)
		*dvdwp = isdvd_wr;
}
//--------------------------------------------------------------------
BOOL	allow_atapi(void* A_PtrScg, BOOL new1)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	old = is_atapi;
	unsigned char	mode[256];

	if (new1 == old)
		return (old);


	(void) unit_ready(scgp);
	if (new1 && mode_sense_g1(scgp, mode, 8, 0x3F, 0) < 0)
	{
		new1 = FALSE;
	}

	is_atapi = new1;
	return (old);
}
//--------------------------------------------------------------------
struct cd_mode_page_2A * mmc_cap(void* A_PtrScg, unsigned char	*modep)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	len;
	int	val;
	unsigned char	mode[0x100];
	struct	cd_mode_page_2A *mp;
	struct	cd_mode_page_2A *mp2;

#ifdef LINUX_APP
#endif

retry:
	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x2A, "CD capabilities",
			mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
	{

		if (scgp->Scg_Sense_Key() == SC_NOT_READY)
		{
			if (wait_unit_ready(scgp, 60))
				goto retry;
		}
		printf("mmc_cap - 1\n");
		return (NULL);		/* Pre SCSI-3/mmc drive		*/
	}

	if (len == 0)			/* Pre SCSI-3/mmc drive		*/
	{
		printf("mmc_cap - 2\n");
		return (NULL);
	}

	mp = (struct cd_mode_page_2A *)
		( mode + sizeof (struct scsi_mode_header) +	
		((struct scsi_mode_header *)mode)->blockdesc_len );

	if (mp->p_len < 0x10)
	{
		printf("mmc_cap - 3\n");
		return (NULL);
	}

	val = a_to_u_2_byte(mp->max_read_speed);
	if (val != 0 && val < 176)
	{
		printf("mmc_cap - 4\n");	
		return (NULL);
	}
	val = a_to_u_2_byte(mp->cur_read_speed);
	if (val != 0 && val < 176)
	{
		return (NULL);
	}

	len -= sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len;
	if (modep)
		mp2 = (struct cd_mode_page_2A *)modep;
	else
		mp2 = (struct cd_mode_page_2A *)malloc(len);
	if (mp2)
	{
		//movebytes(mp, mp2, len);
		GrDumyCopyMem( mp2, mp, len);
	}

	return (mp2);
}
//--------------------------------------------------------------------
BOOL	scsi_in_progress(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	// local -------------------
	BOOL	Tv_Result;
		int		Tv_SenseKey;
		int		Tv_SenseCode;
		int		Tv_SenseQual;
	// code --------------------
		Tv_Result	=	FALSE;

		//get info
		Tv_SenseKey		=	scgp->Scg_Sense_Key();
		Tv_SenseCode	=	scgp->Scg_Sense_Code();
		Tv_SenseQual	=	scgp->Scg_Sense_Qual();

		if ( (SC_NOT_READY == Tv_SenseKey) && (0x04 == Tv_SenseCode) )
		{
			if ( (0x04 == Tv_SenseQual) || (0x07 == Tv_SenseQual) || (0x08 == Tv_SenseQual) )
			{
				/* long write in progress 
					qual : 0x04 = fromat in progress , 0x07 = operation in progress
				*/
				Tv_Result	=	TRUE;
			}
		}
		
		if ( !Tv_Result )
		{
			printf( "scsi_in_progress :: not in progress - sensekey=%X , sensecode=%X , sensequal=%X \n",
				Tv_SenseKey, Tv_SenseCode, Tv_SenseQual );
		}
		
		return Tv_Result;
}
//--------------------------------------------------------------------
int		send_cue_sheet(void* A_PtrScg, caddr_t bp,long size)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5D;	/* Send CUE sheet */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, size);

	scgp->m_CmdName = "send_cue_sheet";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (size - scmd->resid);
}
//--------------------------------------------------------------------
int		scsi_get_speed(void* A_PtrScg, int *readspeedp, int *writespeedp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	cd_mode_page_2A *mp;
	unsigned char	m[256];
	int	val;

	mp = mmc_cap(scgp, m); /* Get MMC capabilities in allocated mp */
	if (mp == NULL)
		return (-1);	/* Pre St_GrScsiIf-3/mmc drive		*/

	val = a_to_u_2_byte(mp->cur_read_speed);
	if (readspeedp)
		*readspeedp = val;

	if (mp->p_len >= 28)
		val = a_to_u_2_byte(mp->v3_cur_write_speed);
	else
		val = a_to_u_2_byte(mp->cur_write_speed);
	if (writespeedp)
		*writespeedp = val;

	return (0);
}
//--------------------------------------------------------------------
int	read_session_offset_philips(void* A_PtrScg,	long	*offp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	sinfo *sp;
	char	xb[256];
	int	len;

	sp = (struct sinfo *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	if (read_toc_philips(scgp, (caddr_t)xb, 0, sizeof (struct siheader), 0, FMT_SINFO) < 0)
		return (-1);
	len = a_to_u_2_byte(sp->hd.len) + sizeof (struct siheader)-2;
	if (len > (int)sizeof (xb)) {
		return (-1);
	}
	if (read_toc_philips(scgp, (caddr_t)xb, 0, len, 0, FMT_SINFO) < 0)
		return (-1);

	sp = (struct sinfo *)xb;
	if (offp)
		*offp = a_to_u_4_byte(sp->desc[sp->hd.finished-1].addr);
	return (0);
}
//--------------------------------------------------------------------
BOOL	is_mmc(void* A_PtrScg, BOOL *cdwp, BOOL *dvdwp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	cdwr	= FALSE;
	BOOL	cdwrw	= FALSE;

	if (cdwp)
		*cdwp = FALSE;
	if (dvdwp)
		*dvdwp = FALSE;

	if (!mmc_check(scgp, NULL, &cdwr, NULL, &cdwrw, NULL, dvdwp))
		return (FALSE);

	if (cdwp)
		*cdwp = cdwr | cdwrw;

	return (TRUE);
}
//--------------------------------------------------------------------
int		read_toc(void* A_PtrScg, caddr_t bp, int track,	int	cnt,	int	msf,	int	fmt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x43;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (msf)
		scmd->cdb.g1_cdb.res = 1;
	scmd->cdb.g1_cdb.addr[0] = fmt & 0x0F;
	scmd->cdb.g1_cdb.res6 = track;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "read toc";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		read_track_info(void* A_PtrScg, caddr_t bp, int type, int addr, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes */
	scmd->cdb.g1_cdb.cmd = 0x52;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	scmd->cdb.cmd_cdb[1] = type;
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);	/* LBA/Track/Session */
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "read track info";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		mode_select_sg0(void* A_PtrScg, unsigned char	*dp, int	cnt,	int	smp, int	pf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	xmode[256+4];
	int	amt = cnt;

	if (amt < 1 || amt > 255) {

		return (-1);
	}

	if (amt < 4) {		/* Data length. medium type & VU */
		amt += 1;
	} else {
		amt += 4;
		//movebytes(&dp[4], &xmode[8], cnt-4);
		GrDumyCopyMem( &xmode[8], &dp[4], cnt-4);
	}
	xmode[0] = 0;
	xmode[1] = 0;
	xmode[2] = dp[1];
	xmode[3] = dp[2];
	xmode[4] = 0;
	xmode[5] = 0;
	i_to_2_byte(&xmode[6], (unsigned int)dp[3]);

	return (mode_select_g1(scgp, xmode, amt, smp, pf));
}
//--------------------------------------------------------------------
int		read_toc_philips(void* A_PtrScg,	caddr_t	bp,	int	track,	int	cnt,	int	msf,	int	fmt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* May last  174s on a TEAC CD-R55S */
	scmd->cdb.g1_cdb.cmd = 0x43;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (msf)
		scmd->cdb.g1_cdb.res = 1;
	scmd->cdb.g1_cdb.res6 = track;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	if (fmt & 1)
		scmd->cdb.g1_cdb.vu_96 = 1;
	if (fmt & 2)
		scmd->cdb.g1_cdb.vu_97 = 1;

	scgp->m_CmdName = "read toc";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		mode_select_g1(void* A_PtrScg, unsigned char *dp, int	cnt, int smp,	int	pf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x55;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.high_addr = smp ? 1 : 0 | pf ? 0x10 : 0;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "mode select g1";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		scsi_prevent_removal(void* A_PtrScg, int prevent)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x1E;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.count = prevent & 1;

	scgp->m_CmdName = "prevent/allow medium removal";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		read_dvd_structure(void* A_PtrScg, caddr_t bp, int cnt, int mt, int addr, int layer, int fmt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes ??? */
	scmd->cdb.g5_cdb.cmd = 0xAD;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] |= (mt & 0x0F);	/* Media Type */
	g5_cdbaddr(&scmd->cdb.g5_cdb, addr);
	g5_cdblen(&scmd->cdb.g5_cdb, cnt);
	scmd->cdb.g5_cdb.count[0] = layer;
	scmd->cdb.g5_cdb.count[1] = fmt;

	scgp->m_CmdName = "read dvd structure";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		rezero_unit(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_REZERO_UNIT;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);

	scgp->m_CmdName = "rezero unit";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		read_rzone_info(void* A_PtrScg,	caddr_t	bp,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	return (get_trackinfo(scgp, bp, TI_TYPE_LBA, 0, cnt));
}
//--------------------------------------------------------------------
int		send_dvd_structure(void* A_PtrScg, caddr_t bp, int cnt, int fmt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes ??? */
	scmd->cdb.g5_cdb.cmd = 0xBF;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	g5_cdblen(&scmd->cdb.g5_cdb, cnt);
	scmd->cdb.g5_cdb.count[0] = 0;
	scmd->cdb.g5_cdb.count[1] = fmt;

	scgp->m_CmdName = "send dvd structure";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int	reserve_tr_rzone(void* A_PtrScg, long	size)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x53;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	i_to_4_byte(&scmd->cdb.cmd_cdb[5], size);

	scgp->m_CmdName = "reserve_track_rzone";

	if (scgp->Scg_Cmd() < 0)
	{
		return (-1);
	}
	return (0);
}
//--------------------------------------------------------------------
BOOL	wait_unit_ready(void* A_PtrScg, int secs)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	i;
	int	c;
	int	k;
	int	ret;
	int	err;

	errno = 0;
	ret = test_unit_ready(scgp);
	if (ret < 0) {
		err = errno;

		if (err == EPERM || err == EACCES) {
			return (FALSE);
		}
		ret = test_unit_ready(scgp);
	}

	if (ret >= 0)
		return (TRUE);

	for (i = 0; i < secs && (ret = test_unit_ready(scgp)) < 0; i++) {
		if (scgp->m_Scmd.scb.busy != 0) {
			sleep(1);
			continue;
		}
		c = scgp->Scg_Sense_Code();
		k = scgp->Scg_Sense_Key();

		if ((k == SC_NOT_READY && (c == 0x3A || c == 0x30)) || (k == SC_MEDIUM_ERROR))
		{
			return (FALSE);
		}
		sleep(1);
	}
	if (ret < 0)
		return (FALSE);
	return (TRUE);
}
//--------------------------------------------------------------------
int	read_scsi(void* A_PtrScg,	caddr_t	bp,	long	addr,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if (addr <= G0_MAXADDR && cnt < 256 && !is_atapi)
		return (read_g0(scgp, bp, addr, cnt));
	else
		return (read_g1(scgp, bp, addr, cnt));
}
//--------------------------------------------------------------------
int	read_g0(void* A_PtrScg, caddr_t	bp,	long	addr,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	if (scgp->cap.c_bsize <= 0)
	{
		raisecond("capacity_not_set", 0L);
	}

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt*scgp->cap.c_bsize;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_READ;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	g0_cdbaddr(&scmd->cdb.g0_cdb, addr);
	scmd->cdb.g0_cdb.count = cnt;

	scgp->m_CmdName = "read_g0";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int	read_g1(void* A_PtrScg, caddr_t	bp,	long	addr,	int	cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	if (scgp->cap.c_bsize <= 0)
	{
		raisecond("capacity_not_set", 0L);
	}

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt*scgp->cap.c_bsize;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = SC_EREAD;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "read_g1";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
BOOL	is_cddrive(void* A_PtrScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		if(scgp->inq.type == INQ_ROMD)
		{
			printf("scgp->inq->type == INQ_ROMD\n");
		}
		if(scgp->inq.type == INQ_WORM)
		{
			printf("scgp->inq->type == INQ_WORM\n");
		}
		return (scgp->inq.type == INQ_ROMD || scgp->inq.type == INQ_WORM);
}
//--------------------------------------------------------------------
int		qic02(void* A_PtrScg, int	cmd)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = DEF_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x0D;	/* qic02 Sysgen SC4000 */
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.mid_addr = cmd;

	scgp->m_CmdName = "qic 02";
	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		sense_secsize(void* A_PtrScg,	int	current)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	int	secsize = -1;

	(void) unit_ready(scgp);


	memset(mode, '\0', sizeof (mode));

	len =	sizeof (struct scsi_mode_header) +
		sizeof (struct scsi_mode_blockdesc);

	if (mode_sense(scgp, mode, len, 0x3F, current?0:2) < 0) {
		memset(mode, '\0', sizeof (mode));
		if (mode_sense(scgp, mode, len, 0, current?0:2) < 0)	{ /* VU (block desc) */
			return (-1);
		}
	}
	if (mode[3] == 8)
	{
		secsize = a_to_u_3_byte(&mode[9]);
	}
	memset(mode, '\0', sizeof (mode));


	return (secsize);
}
//--------------------------------------------------------------------
int		read_capacity(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)&scgp->cap;
	scmd->size = sizeof (struct scsi_capacity);
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x25;	/* Read Capacity */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, 0); /* Full Media */

	scgp->m_CmdName = "read capacity";

	if (scgp->Scg_Cmd() < 0)
	{
		return (-1);
	}
	else
	{
		long	cbsize;
		long	cbaddr;

		cbsize = a_to_4_byte(&scgp->cap.c_bsize);
		cbaddr = a_to_4_byte(&scgp->cap.c_baddr);
		scgp->cap.c_bsize = cbsize;
		scgp->cap.c_baddr = cbaddr;
	}
	return (0);
}
//--------------------------------------------------------------------
int		read_tochdr(void* A_PtrScg,	void* A_PtrDev, int *fp,	int	*lp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	struct	tocheader *tp;
	char	xb[256];
	int	len;

	tp = (struct tocheader *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	DbgMsgPrint("read_tochdr\n");
	if (read_toc(scgp, xb, 0, sizeof (struct tocheader), 0, FMT_TOC) < 0)
	{
		printf("Cannot read TOC header\n");
		return (-1);
	}
	len = a_to_u_2_byte(tp->len) + sizeof (struct tocheader)-2;
	if (len >= 4) {
		if (fp)
			*fp = tp->first;
		if (lp)
			*lp = tp->last;
		return (0);
	}
	return (-1);
}
//--------------------------------------------------------------------
BOOL	cdr_underrun(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if ((scgp->Scg_Sense_Key() != SC_ILLEGAL_REQUEST && scgp->Scg_Sense_Key() != SC_MEDIUM_ERROR))
		return (FALSE);

	if ((scgp->Scg_Sense_Code() == 0x21 && (scgp->Scg_Sense_Qual() == 0x00 ||
	    scgp->Scg_Sense_Qual() == 0x02)) ||	(scgp->Scg_Sense_Code() == 0x0C &&
	    scgp->Scg_Sense_Qual() == 0x09))
	{
		return (TRUE);
	}

	return (FALSE);
}
//--------------------------------------------------------------------
int
prdiskstatus_Lib(void* A_PtrScg, void* A_PtrDev, BOOL	is_cd)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	struct disk_info	di;
	struct rzone_info	rz;
	int			sessions;
	int			track;
	int			tracks;
	int			t;
	int			s;
	long			raddr;
	long			lastaddr = -1;
	long			lastsess = -1;
	long			leadout = -1;
	long			lo_sess = 0;
	long			nwa = -1;
	long			rsize = -1;
	long			border_size = -1;
	int			profile;

	profile = get_curprofile(scgp);
	if (profile > 0) {
		int mt = get_mediatype(scgp);

		pname_known(profile);
	}
	get_diskinfo(scgp, &di, sizeof (di));

	sessions = di.numsess + di.numsess_msb * 256;
	tracks = di.last_track_ls + di.last_track_ls_msb * 256;

	memset((caddr_t)&rz, '\0', sizeof (rz));
	for (t = di.first_track; t <= tracks; t++) {
		memset((caddr_t)&rz, '\0', sizeof (rz));
		get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, t, sizeof (rz));
		track = rz.rzone_num_lsb + rz.rzone_num_msb * 256;
		s = rz.border_num_lsb + rz.border_num_msb * 256;
		raddr = a_to_4_byte(rz.rzone_start);
		if (rsize >= 0)
			border_size = raddr - (lastaddr+rsize);
		if (!rz.blank && s > lastsess) { /* First track in last sess ? */
			lastaddr = raddr;
			lastsess = s;
		}
		nwa = a_to_4_byte(rz.next_recordable_addr);
		rsize = a_to_4_byte(rz.rzone_size);
		if (!rz.blank) {
			leadout = raddr + rsize;
			lo_sess = s;
		}
		printf("%5d %5d %-6s %-10ld %-10ld %ld", track, s, rz.blank ? "Blank" :
				rz.trackmode & 4 ? "Data" : "Audio",
			raddr, raddr + rsize -1, rsize);
		printf("\n");
	}
	printf("\n");
	if (lastaddr >= 0)
		printf("Last session start address:         %ld\n", lastaddr);
	if (leadout >= 0)
		printf("Last session leadout start address: %ld\n", leadout);
	if (rz.nwa_v) {
		printf("Next writable address:              %ld\n", nwa);
		printf("Remaining writable size:            %ld\n", rsize);
	}

	return (0);
}
//--------------------------------------------------------------------
int		get_diskinfo(void* A_PtrScg, struct disk_info *dip, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	len;
	int	ret;

	memset((caddr_t)dip, '\0', cnt);

	if (read_disk_info(scgp, (caddr_t)dip, 4) < 0)
		return (-1);
	len = a_to_u_2_byte(dip->data_len);
	len += 2;
	if (len > cnt)
		len = cnt;
	ret = read_disk_info(scgp, (caddr_t)dip, len);

	return (ret);
}
//--------------------------------------------------------------------
int		get_mediatype(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	profile = get_curprofile(scgp);

	if (profile < 0x08)
		return (MT_NONE);
	if (profile >= 0x08 && profile < 0x10)
		return (MT_CD);
	if (profile >= 0x10 && profile < 0x40)
		return (MT_DVD);
	if (profile >= 0x40 && profile < 0x50)
		return (MT_BD);
	if (profile >= 0x50 && profile < 0x60)
		return (MT_HDDVD);

	return (MT_NONE);
}
//--------------------------------------------------------------------
int		get_curprofile(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	cbuf[8];
	int	amt;
	int	flen;
	int	profile;
	int	i;

	memset(cbuf, '\0', sizeof (cbuf));
	i = get_configuration(scgp, (char *)cbuf, sizeof (cbuf), 0, 0);
	if (i < 0)
		return (-1);

	amt = sizeof (cbuf) - scgp->m_Scmd.resid;
	if (amt < 8)
		return (-1);
	flen = a_to_u_4_byte(cbuf);
	if (flen < 4)
		return (-1);

	profile = a_to_u_2_byte(&cbuf[6]);

	return (profile);
}
//--------------------------------------------------------------------
int		get_configuration(void* A_PtrScg, caddr_t	bp,	int	cnt, int st_feature, int rt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x46;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (rt & 1)
		scmd->cdb.g1_cdb.reladr  = 1;
	if (rt & 2)
		scmd->cdb.g1_cdb.res  = 1;

	i_to_2_byte(scmd->cdb.g1_cdb.addr, st_feature);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "get_configuration";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
BOOL	pname_known(__u32	code)
{
	unsigned short	i;

	for (i = 0; i < sizeof (pl) / sizeof (pl[0]); i++) {
		if (code == pl[i].code)
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
int get_proflist(void* A_PtrScg, BOOL	*wp, BOOL	*cdp, BOOL *dvdp, BOOL *dvdplusp, BOOL *ddcdp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
#ifdef LINUX_APP
		unsigned char	*p;
		int	n;
		int	i;
		int	profile;
		unsigned char	cbuf[1024];
		int	flen;
#endif
		int	curprofile;
		BOOL	wr	= FALSE;
		BOOL	cd	= FALSE;
		BOOL	dvd	= FALSE;
		BOOL	dvdplus	= FALSE;
		BOOL	ddcd	= FALSE;
	
#ifdef LINUX_APP
		flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
		if (flen < 0)
			return (-1);

		p = cbuf;

		curprofile = a_to_u_2_byte(&p[6]);

		p += 8;		/* Skip feature header	*/
		n = p[3];	/* Additional length	*/
		n /= 4;
		p += 4;
	
		for (i = 0; i < n; i++)
		{
			profile = a_to_u_2_byte(p);
			p += 4;
			if (profile >= 0x0008 && profile < 0x0010)
				cd = TRUE;
			if (profile > 0x0008 && profile < 0x0010)
				wr = TRUE;
	
			if (profile >= 0x0010 && profile < 0x0018)
				dvd = TRUE;
			if (profile > 0x0010 && profile < 0x0018)
				wr = TRUE;
	
			if (profile >= 0x0018 && profile < 0x0020)
				dvdplus = TRUE;
			if (profile > 0x0018 && profile < 0x0020)
				wr = TRUE;
	
			if (profile >= 0x0020 && profile < 0x0028)
				ddcd = TRUE;
			if (profile > 0x0020 && profile < 0x0028)
				wr = TRUE;
		}
#else
		wr	=	TRUE;
		dvd	=	TRUE;
		curprofile	=	0;
		scgp->inq.type	=	INQ_ROMD;
#endif

		if (wp)
			*wp	= wr;
		if (cdp)
			*cdp	= cd;
		if (dvdp)
			*dvdp	= dvd;
		if (dvdplusp)
			*dvdplusp = dvdplus;
		if (ddcdp)
			*ddcdp	= ddcd;
	
		return (curprofile);
}
//--------------------------------------------------------------------
int get_profiles(void* A_PtrScg, caddr_t bp, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	amt;
	int	flen;
	int	i;

	flen = get_conflen(scgp, 0, 0);
	if (flen < 0)
		return (-1);
	if (cnt < flen)
		flen = cnt;

	memset(bp, '\0', cnt);
	i = get_configuration(scgp, (char *)bp, flen, 0, 0);
	if (i < 0)
		return (-1);
	amt = flen - scgp->m_Scmd.resid;

	flen = a_to_u_4_byte(bp);
	if ((flen+4) < amt)
		amt = flen+4;

	return (amt);
}
//--------------------------------------------------------------------
int get_conflen(void* A_PtrScg,	int	st_feature,	int	rt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		unsigned char	cbuf[8];
		int	flen;
		int	i;
	
		memset(cbuf, '\0', sizeof (cbuf));
		i = get_configuration(scgp, (char *)cbuf, sizeof (cbuf), st_feature, rt);
		if (i < 0)
			return (-1);
		i = sizeof (cbuf) - scgp->m_Scmd.resid;
		if (i < 4)
			return (-1);
	
		flen = a_to_u_4_byte(cbuf);
		if (flen < 4)
			return (-1);
		return (flen);
}
//--------------------------------------------------------------------
int get_wproflist(void* A_PtrScg, BOOL *cdp, BOOL	*dvdp, BOOL	*dvdplusp, BOOL	*ddcdp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		unsigned char	cbuf[1024];
		unsigned char	*p;
		int	flen;
		int	curprofile;
		int	profile;
		int	i;
		int	n;
		BOOL	cd	= FALSE;
		BOOL	dvd	= FALSE;
		BOOL	dvdplus	= FALSE;
		BOOL	ddcd	= FALSE;
	
		flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
		if (flen < 0)
			return (-1);
		p = cbuf;
		curprofile = a_to_u_2_byte(&p[6]);
	
		p += 8;		/* Skip feature header	*/
		n = p[3];	/* Additional length	*/
		n /= 4;
		p += 4;
	
		for (i = 0; i < n; i++) {
			profile = a_to_u_2_byte(p);
			p += 4;
			if (profile > 0x0008 && profile < 0x0010)
				cd = TRUE;
			if (profile > 0x0010 && profile < 0x0018)
				dvd = TRUE;
			if (profile > 0x0018 && profile < 0x0020)
				dvdplus = TRUE;
			if (profile > 0x0020 && profile < 0x0028)
				ddcd = TRUE;
		}
		if (cdp)
			*cdp	= cd;
		if (dvdp)
			*dvdp	= dvd;
		if (dvdplusp)
			*dvdplusp = dvdplus;
		if (ddcdp)
			*ddcdp	= ddcd;
	
		return (curprofile);
}
//--------------------------------------------------------------------
int		get_supported_cdrw_media_types(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char  cbuf[16];
	int	ret;

	memset(cbuf, '\0', sizeof (cbuf));

	ret = get_configuration(scgp, (char *)cbuf, sizeof (cbuf), 0x37, 2);

	if (ret < 0)
		return (-1);

	if (cbuf[3] < 12)	/* Couldn't retrieve feature 0x37	*/
		return (-1);

	return (int)(cbuf[13]);
}
//--------------------------------------------------------------------
int		sessstatus(void* A_PtrScg, BOOL is_cd, long *offp, long *nwap)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct disk_info	di;
	struct rzone_info	rz;
	int			sessions;
	int			track;
	int			tracks;
	int			t;
	int			s;
	long			raddr;
	long			lastaddr = -1;
	long			lastsess = -1;
	long			leadout = -1;
	long			lo_sess = 0;
	long			nwa = -1;
	long			rsize = -1;
	long			border_size = -1;


	if (get_diskinfo(scgp, &di, sizeof (di)) < 0)
		return (-1);

	sessions = di.numsess + di.numsess_msb * 256;
	tracks = di.last_track_ls + di.last_track_ls_msb * 256;

	memset((caddr_t)&rz, '\0', sizeof (rz));
	for (t = di.first_track; t <= tracks; t++) {
		memset((caddr_t)&rz, '\0', sizeof (rz));
		if (get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, t, sizeof (rz)) < 0)
			return (-1);
		track = rz.rzone_num_lsb + rz.rzone_num_msb * 256;
		s = rz.border_num_lsb + rz.border_num_msb * 256;
		raddr = a_to_4_byte(rz.rzone_start);
		if (rsize >= 0)
			border_size = raddr - (lastaddr+rsize);
		if (!rz.blank && s > lastsess) { /* First track in last sess ? */
			lastaddr = raddr;
			lastsess = s;
		}
		nwa = a_to_4_byte(rz.next_recordable_addr);
		rsize = a_to_4_byte(rz.rzone_size);
		if (!rz.blank) {
			leadout = raddr + rsize;
			lo_sess = s;
		}
	}
	if (lastaddr >= 0 && offp != NULL)
		*offp = lastaddr;

	if (rz.nwa_v && nwap != NULL)
		*nwap = nwa;

	return (0);
}
//--------------------------------------------------------------------
int		scsi_get_perf_curspeed(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	register struct	scg_cmd	*scmd = &scgp->m_Scmd;
	struct mmc_performance_header	*ph;
	struct mmc_performance		*perfp;
#define	MAX_AMT	100
	char buffer[8 + MAX_AMT*16];
	__u32	ul;
	__u32	end;
	__u32	speed;
	int	amt;
	int	i;
	int	mt = 0;
	int	ssp = 1;

	if (endp || writep) {
		memset((caddr_t) buffer, '\0', sizeof (buffer));
		if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x00, 0x04) < 0) {
			goto doread;
		}

		ph = (struct mmc_performance_header *)buffer;
		amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_performance);
		if (amt < 1)
			amt = 1;
		if (amt > MAX_AMT)
			amt = MAX_AMT;

		if (scsi_get_performance(scgp, buffer, 8+16*amt, amt, 0x00, 0x04) < 0)
			return (-1);

		ph = (struct mmc_performance_header *)buffer;
		perfp = (struct mmc_performance *)(((char *)ph) +
				sizeof (struct mmc_performance_header));

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > amt)
			i = amt;
		end = 0;
		speed = 0;
		for (; --i >= 0; perfp++) {
			ul = a_to_u_4_byte(perfp->end_lba);
			if (ul > end) {
				end = ul;
				ul = a_to_u_4_byte(perfp->end_perf);
				speed = ul;
			}
		}

		if (endp)
			*endp = end;

		if (writep)
			*writep = speed;

		perfp = (struct mmc_performance *)(((char *)ph) +
				sizeof (struct mmc_performance_header));
		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > scmd->cdb.cmd_cdb[9])
			i = scmd->cdb.cmd_cdb[9];
	}
doread:
	if (readp) {
		memset((caddr_t) buffer, '\0', sizeof (buffer));
		if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x00, 0x00) < 0) {
			return (-1);
		}

		ph = (struct mmc_performance_header *)buffer;
		amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_performance);
		if (amt < 1)
			amt = 1;
		if (amt > MAX_AMT)
			amt = MAX_AMT;

		if (scsi_get_performance(scgp, buffer, 8+16*amt, amt, 0x00, 0x00) < 0)
			return (-1);

		ph = (struct mmc_performance_header *)buffer;
		perfp = (struct mmc_performance *)(((char *)ph) +
				sizeof (struct mmc_performance_header));

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > amt)
			i = amt;
		end = 0;
		speed = 0;
		for (; --i >= 0; perfp++) {
			ul = a_to_u_4_byte(perfp->end_lba);
			if (ul > end) {
				end = ul;
				ul = a_to_u_4_byte(perfp->end_perf);
				speed = ul;
			}
		}

		if (readp)
			*readp = speed;

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > scmd->cdb.cmd_cdb[9])
			i = scmd->cdb.cmd_cdb[9];
	}

	return (0);
}
//--------------------------------------------------------------------
void	print_format_capacities(void* A_PtrScg)
{

}
//--------------------------------------------------------------------
int		get_format_capacities(void* A_PtrScg, caddr_t bp, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int			len = sizeof (struct scsi_format_cap_header);
	struct scsi_format_cap_header	*hp;

	memset(bp, '\0', cnt);
	if (cnt < len)
		return (-1);
	if (read_format_capacities(scgp, bp, len) < 0) {
		return (-1);
	}

	if (scgp->scg_getresid() > 0)
		return (-1);

	hp = (struct scsi_format_cap_header *)bp;
	len = hp->len;
	len += sizeof (struct scsi_format_cap_header);
	while (len > cnt)
		len -= sizeof (struct scsi_format_cap_desc);

	if (read_format_capacities(scgp, bp, len) < 0)
		return (-1);

	len -= scgp->scg_getresid();
	return (len);
}
//--------------------------------------------------------------------
int		read_format_capacities(void* A_PtrScg, caddr_t bp, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x23;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "read_format_capacities";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		scsi_get_perf_maxspeed(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	struct mmc_performance_header	*ph;
	struct mmc_write_speed		*wsp;
#define	MAX_AMT	100
	char buffer[8 + MAX_AMT*16];
	__u32	ul;
	int	amt;
	int	i;
	int	mt = 0;
	int	ssp = 1;
	char	*mname = NULL;

	memset((caddr_t) buffer, '\0', sizeof (buffer));
	ph = (struct mmc_performance_header *)buffer;
	if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x03, 0) < 0)
		return (-1);

	amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_write_speed);
	if (amt < 1)
		amt = 1;
	if (amt > MAX_AMT)
		amt = MAX_AMT;
	if (scsi_get_performance(scgp, buffer, 8+amt*16, amt, 0x03, 0) < 0)
		return (-1);

	ph = (struct mmc_performance_header *)buffer;
	wsp = (struct mmc_write_speed *)(((char *)ph) +
				sizeof (struct mmc_performance_header));

	ul = a_to_u_4_byte(wsp->end_lba);
	if (endp)
		*endp = ul;

	ul = a_to_u_4_byte(wsp->read_speed);
	if (readp)
		*readp = ul;

	ul = a_to_u_4_byte(wsp->write_speed);
	if (writep)
		*writep = ul;

	wsp = (struct mmc_write_speed *)(((char *)ph) +
				sizeof (struct mmc_performance_header));

	i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_write_speed);
	if (i > scmd->cdb.cmd_cdb[9])
		i = scmd->cdb.cmd_cdb[9];

	return (0);
#undef	MAX_AMT
}
//--------------------------------------------------------------------
int	scsi_get_performance(void* A_PtrScg, caddr_t bp, int cnt,	int	ndesc, int type, int datatype)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t) scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA | SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0xAC;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] |= datatype & 0x1F;
	scmd->cdb.cmd_cdb[9] = ndesc;
	scmd->cdb.cmd_cdb[10] = type;

	scgp->m_CmdName = "get performance";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		has_profile(void* A_PtrScg,	int	profile)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	cbuf[1024];
	unsigned char	*p;
	int	flen;
	int	prf;
	int	i;
	int	n;

	flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
	if (flen < 0)
		return (-1);

	p = cbuf;
	p += 8;		/* Skip feature header	*/
	n = p[3];	/* Additional length	*/
	n /= 4;
	p += 4;

	for (i = 0; i < n; i++) {
		prf = a_to_u_2_byte(p);

		if (profile == prf)
			return (1);
		p += 4;
	}
	return (0);
}
//--------------------------------------------------------------------
int	speed_select_mdvd(void* A_PtrScg, int readspeed, int writespeed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	__u32	end_lba = 0x7FFFFFFF;
	__u32	wspeed = writespeed;

	if (scsi_get_perf_maxspeed(scgp, (__u32 *)NULL, (__u32 *)NULL, &end_lba) < 0)
		return (-1);

	if (scsi_set_streaming(scgp, (__u32 *)NULL, &wspeed, &end_lba) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
int	scsi_set_streaming(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	struct mmc_streaming	str;
	struct mmc_streaming	*sp = &str;

	memset((caddr_t) scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t) sp;
	scmd->size = sizeof (*sp);
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xB6;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	i_to_2_byte(&scmd->cdb.cmd_cdb[9], sizeof (*sp)); /* Sz not G5 alike */

	scgp->m_CmdName = "set streaming";

	memset(sp, '\0', sizeof (*sp));
	if (endp)
		i_to_4_byte(sp->end_lba, *endp);
	else
		i_to_4_byte(sp->end_lba, 0x7FFFFFFF);

	if (readp)
		i_to_4_byte(sp->read_size, *readp);
	else
		i_to_4_byte(sp->read_size, 0x7FFFFFFF);

	if (writep)
		i_to_4_byte(sp->write_size, *writep);
	else
		i_to_4_byte(sp->write_size, 0x7FFFFFFF);

	i_to_4_byte(sp->read_time, 1000);
	i_to_4_byte(sp->write_time, 1000);

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		get_blf(int mt)
{
	switch (mt) {

	case MT_DVD:
		return (16);

	case MT_BD:
		return (32);

	case MT_HDDVD:
		return (32);	/* XXX ??? */

	default:
		return (1);
	}
}
//--------------------------------------------------------------------
BOOL	get_mode_params(void* A_PtrScg, int page, char *pagename, unsigned char *modep,
					unsigned char *cmodep, unsigned char *dmodep, unsigned char *smodep, int *lenp)
{
	// local -------------------
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		int	len;
		BOOL	ret = TRUE;
	// code --------------------

#ifdef LINUX_APP
		if (lenp)
			*lenp = 0;
		if (!has_mode_page(scgp, page, pagename, &len))
		{
			return (FALSE);
		}
		if (lenp)
			*lenp = len;

		if (modep) {
			memset(modep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, modep, len, page, 0) < 0) {
				ret = FALSE;
			}
		}

		if (cmodep) {
			memset(cmodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, cmodep, len, page, 1) < 0) { /* Page x change */
				ret = FALSE;
			}
		}

		if (dmodep) {
			memset(dmodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, dmodep, len, page, 2) < 0) { /* Page x default */
				ret = FALSE;
			}
		}

		if (smodep) {
			memset(smodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, smodep, len, page, 3) < 0) { /* Page x saved */
				ret = FALSE;
			}
		}
#else
		if ( NULL != lenp )
		{
			*lenp	=	92;
		}
#endif

	return (ret);
}
//--------------------------------------------------------------------
BOOL	set_mode_params(void* A_PtrScg, char *pagename, unsigned char *modep, int len, int save, int secsize)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	i;

	((struct scsi_modesel_header *)modep)->sense_data_len	= 0;
	((struct scsi_modesel_header *)modep)->res2		= 0;

	i = ((struct scsi_mode_header *)modep)->blockdesc_len;
	if (i > 0) {
		i_to_3_byte(
			((struct scsi_mode_data *)modep)->blockdesc.nlblock,
								0);
		if (secsize >= 0)
		i_to_3_byte(((struct scsi_mode_data *)modep)->blockdesc.lblen,
							secsize);
	}

	(void) unit_ready(scgp);

	if (save == 0 || mode_select(scgp, modep, len, save, scgp->inq.data_format >= 2) < 0)
	{
		(void) unit_ready(scgp);
		if (mode_select(scgp, modep, len, 0, scgp->inq.data_format >= 2) < 0)
		{
			DbgMsgPrint("set_mode_params() - TEST 4\n");
			return (FALSE);
		}
	}
	return (TRUE);
}
//--------------------------------------------------------------------
int		mode_sense(void* A_PtrScg, unsigned char *dp,	int	cnt, int page, int pcf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		if (is_atapi)
		{
			return (mode_sense_sg0(scgp, dp, cnt, page, pcf));
		}
		return (mode_sense_g0(scgp, dp, cnt, page, pcf));
}
//--------------------------------------------------------------------
int		mode_sense_sg0(void* A_PtrScg, unsigned char *dp,	int	cnt, int page, int pcf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	xmode[256+4];
	int	amt = cnt;
	int	len;

	if (amt < 1 || amt > 255) {
		return (-1);
	}

	memset((caddr_t)xmode, '\0', sizeof (xmode));
	if (amt < 4) {		/* Data length. medium type & VU */
		amt += 1;
	} else {
		amt += 4;
	}
	if (mode_sense_g1(scgp, xmode, amt, page, pcf) < 0)
		return (-1);

	amt = cnt - scgp->m_Scmd.resid;
/*
 * For tests: Solaris 8 & LG CD-ROM always returns resid == amt
 */
/*	amt = cnt;*/
	if (amt > 4)
	{
		//movebytes(&xmode[8], &dp[4], amt-4);
		GrDumyCopyMem( &dp[4], &xmode[8], amt-4);
	}
	len = a_to_u_2_byte(xmode);
	if (len == 0) {
		dp[0] = 0;
	} else if (len < 6) {
		if (len > 2)
			len = 2;
		dp[0] = len;
	} else {
		dp[0] = len - 3;
	}
	dp[1] = xmode[2];
	dp[2] = xmode[3];
	len = a_to_u_2_byte(&xmode[6]);
	dp[3] = len;


	return (0);
}
//--------------------------------------------------------------------
int		mode_select_g0(void* A_PtrScg, unsigned char *dp,	int	cnt, int smp,	int	pf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_MODE_SELECT;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.high_addr = smp ? 1 : 0 | pf ? 0x10 : 0;
	scmd->cdb.g0_cdb.count = cnt;

	scgp->m_CmdName = "mode select g0";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		mode_sense_g1(void* A_PtrScg,	unsigned char	*dp, int cnt,	int	page,	int	pcf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5A;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	scmd->cdb.g1_cdb.addr[0] = (page&0x3F) | ((pcf<<6)&0xC0);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->m_CmdName = "mode sense g1";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		mode_sense_g0(void* A_PtrScg, unsigned char *dp, int cnt, int page, int pcf)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = 0xFF;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_MODE_SENSE;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.mid_addr = (page&0x3F) | ((pcf<<6)&0xC0);
	scmd->cdb.g0_cdb.count = page ? 0xFF : 24;
	scmd->cdb.g0_cdb.count = cnt;

	scgp->m_CmdName = "mode sense g0";

	if (scgp->Scg_Cmd() < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
BOOL	has_mode_page(void* A_PtrScg, int page, char *pagename, int *lenp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	hdlen;
	int	len = 1;
	int	try1 = 0;
	struct	scsi_mode_page_header *mp;

	if ((scgp->m_Flags & DRF_MODE_DMA_OVR) != 0)
		len = sizeof (struct scsi_mode_header);
again:
	memset((caddr_t)mode, '\0', sizeof (mode));
	if (lenp)
		*lenp = 0;

	(void) unit_ready(scgp);

	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		if (len < (int)sizeof (struct scsi_mode_header) && try1 == 0) {
			len = sizeof (struct scsi_mode_header);
			goto again;
		}
		return (FALSE);
	}
	else 
	{
		if (len > 1 && try1 == 0) {
			if ((scgp->m_Flags & DRF_MODE_DMA_OVR) == 0)
			{

				scgp->m_Flags |= DRF_MODE_DMA_OVR;
			}
		}
		len = ((struct scsi_mode_header *)mode)->sense_data_len + 1;
	}

	(void) unit_ready(scgp);
	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		return (FALSE);
	}

	hdlen = sizeof (struct scsi_mode_header) +
			((struct scsi_mode_header *)mode)->blockdesc_len;
	mp = (struct scsi_mode_page_header *)(mode + hdlen);

	if (mp->p_len == 0)
	{
		if (!scsi_compliant && try1 == 0)
		{
			len = hdlen;

			len += 2;
			try1++;
			goto again;
		}
	}
	if (!scsi_compliant &&
	    (len < (int)(mp->p_len + hdlen + 2))) {
		len = mp->p_len + hdlen + 2;
	}
	if (mp->p_code != page) {
		return (FALSE);
	}

	if (lenp)
		*lenp = len;
	return (mp->p_len > 0);
}
//--------------------------------------------------------------------
char *	astoi(const char *s, int *i)
{
	long l;
	char *ret;

	ret = astol(s, &l);
	*i = l;
	return (ret);
}
//--------------------------------------------------------------------
char *	astol(const char *s, long *l)
{
	return (astolb(s, l, 0));
}
//--------------------------------------------------------------------
char *	astolb(const char *s,	long *l, int base)
{
	int neg = 0;
	long ret = 0L;
	int digit;
	char c;

	while (is_space(*s))
		s++;

	if (*s == '+') {
		s++;
	} else if (*s == '-') {
		s++;
		neg++;
	}

	if (base == 0) {
		if (*s == '0') {
			base = 8;
			s++;
			if (*s == 'x' || *s == 'X') {
				s++;
				base = 16;
			}
		} else {
			base = 10;
		}
	}
	for (; (c = *s) != 0; s++) {

		if (is_digit(c)) {
			digit = c - '0';
		} else if (is_hex(c)) {
			digit = to_lower(c) - 'a' + 10;
		} else {
			break;
		}

		if (digit < base) {
			ret *= base;
			ret += digit;
		} else {
			break;
		}
	}
	if (neg)
		ret = -ret;
	*l = ret;
	return ((char *)s);
}
//--------------------------------------------------------------------
void		timevaldiff(struct timeval *start, struct timeval	*stop)
{
	struct timeval tv;

	tv.tv_sec = stop->tv_sec - start->tv_sec;
	tv.tv_usec = stop->tv_usec - start->tv_usec;
	while (tv.tv_usec > 1000000) {
		tv.tv_usec -= 1000000;
		tv.tv_sec += 1;
	}
	while (tv.tv_usec < 0) {
		tv.tv_usec += 1000000;
		tv.tv_sec -= 1;
	}
	*stop = tv;
}
//--------------------------------------------------------------------
FILE*		fileopen(char	*name, char	*mode)
{
	int	ret;
	int	omode = 0;
	int	flag = 0;

	if (!_cvmod(mode, &omode, &flag))
		return ((FILE *) NULL);

	if ((ret = _openfd(name, omode)) < 0)
		return ((FILE *) NULL);

	return (_fcons((FILE *)0, ret, flag | FI_CLOSE));
}
//--------------------------------------------------------------------
int			from_bcd(int b)
{
	return ((b & 0x0F) + 10 * (((b)>> 4) & 0x0F));
}
//--------------------------------------------------------------------
int			to_bcd(int i)
{
	return (i % 10 | ((i / 10) % 10) << 4);
}
//--------------------------------------------------------------------
long		msf_to_lba(int m, int s, int f, BOOL force_positive)
{
	long	ret = m * 60 + s;

	ret *= 75;
	ret += f;
	if (m < 90 || force_positive)
		ret -= 150;
	else
		ret -= 450150;
	return (ret);
}
//--------------------------------------------------------------------
BOOL		lba_to_msf(long lba, msf_t *mp)
{
	int	m;
	int	s;
	int	f;

	if (lba >= -150) {

		m = (lba + 150) / 60 / 75;
		s = (lba + 150 - m*60*75)  / 75;
		f = (lba + 150 - m*60*75 - s*75);

	} else if (lba >= -45150 && lba <= -151) {
		m = (lba + 450150) / 60 / 75;
		s = (lba + 450150 - m*60*75)  / 75;
		f = (lba + 450150 - m*60*75 - s*75);

	} else {
		mp->msf_min   = -1;
		mp->msf_sec   = -1;
		mp->msf_frame = -1;

		return (FALSE);
	}
	mp->msf_min   = m;
	mp->msf_sec   = s;
	mp->msf_frame = f;

	if (lba > 404849)			/* 404850 -> 404999: lead out */
		return (FALSE);
	return (TRUE);
}
//--------------------------------------------------------------------
void		sec_to_msf(long sec, msf_t *mp)
{
	int	m;
	int	s;
	int	f;

	m = (sec) / 60 / 75;
	s = (sec - m*60*75)  / 75;
	f = (sec - m*60*75 - s*75);

	mp->msf_min   = m;
	mp->msf_sec   = s;
	mp->msf_frame = f;
}
//--------------------------------------------------------------------
void		print_min_atip(long li, long lo)
{
	msf_t	msf;

	if (li < 0) {
		lba_to_msf(li, &msf);

		printf("  ATIP start of lead in:  %ld (%02d:%02d/%02d)\n",
			li, msf.msf_min, msf.msf_sec, msf.msf_frame);
	}
	if (lo > 0) {
		lba_to_msf(lo, &msf);
		printf("  ATIP start of lead out: %ld (%02d:%02d/%02d)\n",
			lo, msf.msf_min, msf.msf_sec, msf.msf_frame);
	}
}
//--------------------------------------------------------------------
BOOL		is_cdspeed(int speed)
{
	int	cdspeed;
	long	cdrest;
	int	dvdspeed;
	long	dvdrest;

	cdspeed = speed / 176;
	cdrest = cdspeed * 1764;
	cdrest = speed * 10 - cdrest;
	if (cdrest < 0)
		cdrest *= -1;
	if (cdrest > 1764/2)
		cdrest = 1764 - cdrest;


	if (speed >= 2990 && speed <= 3656) {
		dvdrest = 33240;
		dvdrest = speed * 10 - dvdrest;
	} else {
		dvdspeed = speed / 1385;
		dvdrest = dvdspeed * 13850;
		dvdrest = speed * 10 - dvdrest;
	}
	if (dvdrest < 0)
		dvdrest *= -1;
	if (dvdrest > 13850/2)
		dvdrest = 13850 - dvdrest;

	return (dvdrest > (cdrest*8 - cdrest/6));
}
//--------------------------------------------------------------------
__u16	calcCRC(unsigned char *buf, __u32 bsize)
{
	return (__u16)(updcrc(0x0000, (__u8 *)buf, bsize));
}
//--------------------------------------------------------------------
__u16	fillcrc(unsigned char *buf, __u32 bsize)
{
	__u16	crc = calcCRC(buf, bsize-2);


	crc = crc ^ 0xFFFF;

	buf[bsize-2] = (crc >> 8) & 0xFF;
	buf[bsize-1] = crc & 0xFF;
	return (crc);
}
//--------------------------------------------------------------------
__u16	flip_crc_error_corr(unsigned char *b, __u32 bsize, __u32 p_crc)
{
	__u16		crc = (__u16)p_crc;
	__u32		btsize = bsize * BPB;

	if (crc != 0) {
		int	i;

		for (i = 0; (__u32)i < btsize; i++) {
			__u8	c;

			c = fliptab[i % BPB];
			b[i / BPB] ^= c;
			if ((crc = calcCRC(b, bsize)) == 0) {
				return (crc);
			}
			b[i / BPB] ^= c;
		}
	}
	return (crc & 0xffff);
}
//--------------------------------------------------------------------
__u32		updcrc(__u32 p_crc, __u8 *cp, __u32 cnt)
{
	__u16	crc = (__u16)p_crc;

	while (cnt-- > 0) {
		crc = (crc<<BPB) ^ crctab[(crc>>(BPW-BPB)) ^ (*cp++)];
	}
	return (crc);
}
//--------------------------------------------------------------------
int			do_leadin(track_t *trackp)
{
	int	tracks = trackp->tracks;
	msf_t	m;
	int	ctrl;
	int	i;
	int	toctype = trackp[0].tracktype & TOC_MASK;

	if (_nsubh) {
		return (0);
	}

	memset(_subq, '\0', sizeof (_subq));

	ctrl = (st2mode[trackp[0].sectype & ST_MASK]) << 4;
	if (is_copy(&trackp[0]))
		ctrl |= TM_ALLOW_COPY << 4;
	m.msf_min = trackp[1].trackno;

	m.msf_sec = toc2sess[toctype & TOC_MASK];
	m.msf_sec = from_bcd(m.msf_sec);		/* convert to BCD */
	m.msf_frame = 0;
	filltpoint(_subq[0], ctrl|0x01, 0xA0, &m);


	ctrl = (st2mode[trackp[tracks].sectype & ST_MASK]) << 4;
	if (is_copy(&trackp[tracks]))
		ctrl |= TM_ALLOW_COPY << 4;
	m.msf_min = trackp[tracks].trackno;
	m.msf_sec = 0;
	m.msf_frame = 0;
	filltpoint(_subq[1], ctrl|0x01, 0xA1, &m);

	lba_to_msf(trackp[tracks+1].trackstart, &m);
	ctrl = (st2mode[trackp[tracks].sectype & ST_MASK]) << 4;
	if (is_copy(&trackp[tracks]))
		ctrl |= TM_ALLOW_COPY << 4;
	filltpoint(_subq[2], ctrl|0x01, 0xA2, &m);


	for (i = 1; i <= tracks; i++) {
		lba_to_msf(trackp[i].trackstart, &m);
		ctrl = (st2mode[trackp[i].sectype & ST_MASK]) << 4;
		if (is_copy(&trackp[i]))
			ctrl |= TM_ALLOW_COPY << 4;
		filltpoint(_subq[i-1+3], ctrl|0x01, to_bcd(trackp[i].trackno), &m);	/* track n */
	}
	return (0);
}
//--------------------------------------------------------------------
int			write_leadin_Lib(void* A_PtrScg, void* A_PtrDev, track_t *trackp, int leadinstart)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	msf_t	m;
	int	i;
	__u32	j;
	unsigned char	*bp = (unsigned char*)scgp->m_PtrBuf;
	unsigned char	*subp;
	unsigned char	*sp;
	int	secsize;
	int	secspt;
	int	bytespt;
	long	amount;
	int	startsec;
	long	bytes = 0L;
	int	textoff = 0;
	msf_t	msf;

	secsize = trackp[0].secsize;
	secspt = trackp[0].secspt;
	bytespt = secspt * secsize;

	lba_to_msf(leadinstart, &msf);

	memset(bp, '\0', bytespt);

	startsec = leadinstart;
	sp = bp;
	subp = bp + 2352;
	for (i = leadinstart, j = 0; i < -150; i++, j++) {

		if (_nsubh) {
			if (j >= (3*_nsubh))
				j = 0;
		} else {
			if (j >= (3*3 + 3*trackp->tracks))
				j = 0;
		}
		lba_to_msf((long)i, &m);
		fillttime(_subq[j/3], &m);
		fillcrc(_subq[j/3], 12);

		if (is_raw16(&trackp[0])) {
			qpto16(subp, _subq[j/3], 0);
		} else {
			qpto96(subp, _subq[j/3], 0);
			if (textsub) {
				addrw(subp, &textsub[textoff]);
				textoff += 96;
				if (textoff >= textlen)
					textoff = 0;
			}
		}
		if ((startsec+secspt-1) == i || i == -151) {
			if ((i-startsec+1) < secspt) {
				secspt = i-startsec+1;
				bytespt = secspt * secsize;
			}
			encsectors(trackp, bp, startsec, secspt);

			amount = write_secs(scgp, dp,	(char *)bp, startsec, bytespt, secspt, FALSE);
			if (amount < 0) {
				return (-1);
			}
			bytes += amount;
			startsec = i+1;
			sp = bp;
			subp = bp + 2352;
			continue;
		}
		sp += secsize;
		subp += secsize;
	}
	return (0);
}
//--------------------------------------------------------------------
int			write_leadout(void* A_PtrScg, void* A_PtrDev, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	int	tracks = trackp->tracks;
	msf_t	m;
	msf_t	mr;
	int	ctrl;
	int	i;
	int	j;
	unsigned char	*bp = (unsigned char*)scgp->m_PtrBuf;
	unsigned char	*subp;
	unsigned char	*sp;
	int	secsize;
	int	secspt;
	int	bytespt;
	long	amount;
	long	startsec;
	long	endsec;
	long	bytes = 0L;
	int	leadoutstart;
	unsigned char	sub[12];
	BOOL	p;
	msf_t	msf;

	memset(sub, '\0', 12);

	secsize = trackp[tracks+1].secsize;
	secspt = trackp[tracks+1].secspt;
	bytespt = secspt * secsize;

	leadoutstart = trackp[tracks+1].trackstart;
	lba_to_msf(leadoutstart, &msf);

	memset(bp, '\0', bytespt);

	startsec = leadoutstart;
	endsec = startsec + trackp[tracks+1].tracksecs;
	sp = bp;
	subp = bp + 2352;
	ctrl = (st2mode[trackp->sectype & ST_MASK]) << 4;
	if (is_copy(trackp))
		ctrl |= TM_ALLOW_COPY << 4;

	for (i = leadoutstart, j = 0; i < endsec; i++, j++) {

		lba_to_msf((long)i, &m);
		sec_to_msf((long)j, &mr);
		filldsubq(sub, ctrl|0x01, 0xAA, 1, &mr, &m);
		sub[1] = 0xAA;
		fillcrc(sub, 12);
		p = (j % 150) < 75;
		if (j < 150)
			p = FALSE;

		if (is_raw16(&trackp[0])) {
			qpto16(subp, sub, p);
		} else {
			qpto96(subp, sub, p);
		}
		if ((startsec+secspt-1) == i || i == (endsec-1)) {
			if ((i-startsec+1) < secspt) {
				secspt = i-startsec+1;
				bytespt = secspt * secsize;
			}
			encsectors(trackp, bp, startsec, secspt);

			amount = write_secs(scgp, dp,	(char *)bp, startsec, bytespt, secspt, FALSE);
			if (amount < 0) {

				return (-1);
			}
			bytes += amount;
			startsec = i+1;
			sp = bp;
			subp = bp + 2352;
			continue;
		}
		sp += secsize;
		subp += secsize;
	}
	return (0);
}
//--------------------------------------------------------------------
void		fillsubch(track_t *trackp, unsigned char *sp, int secno, int nsecs)
{
	msf_t	m;
	msf_t	mr;
	int	ctrl;
	int	i;
	int	rsecno;
	int	end;
	int	secsize = trackp->secsize;
	int	trackno = trackp->trackno;
	int	nindex = trackp->nindex;
	int	curindex;
	long	*tindex = NULL;
	long	nextindex = 0L;
	unsigned char	sub[12];
	unsigned char *sup;
	char	*mcn;
	/*
	 * In theory this should make fillsubch() non-reenrtrant but it seems
	 * that the probability check at the beginning of the function is
	 * sufficient to make it work as expected.
	 */
static	long	nextmcn = -1000000L;
static	long	nextisrc = -1000000L;
static	unsigned char	lastindex = 255;

	memset(sub, '\0', 12);

	mcn = track_base(trackp)->isrc;
	rsecno = secno - trackp->trackstart;

	sup = sp + 2352;
	if (mcn && (nextmcn < secno || nextmcn > (secno+100))) {
		nextmcn = secno/100*100 + 99;
	}
	if (trackp->isrc && (nextisrc < secno || nextisrc > (secno+100))) {
		nextisrc = secno/100*100 + 49;
	}
	ctrl = (st2mode[trackp->sectype & ST_MASK]) << 4;
	if (is_copy(trackp))
		ctrl |= TM_ALLOW_COPY << 4;


	if (rsecno < 0) {

		curindex = 0;
		end = trackp->trackstart;

	} else if (rsecno > trackp->index0start) {

		trackno++;
		curindex = 0;
		end = trackp->trackstart + trackp->tracksecs;
	} else {

		end = 0;
		curindex = 1;
		if (nindex > 1) {
			tindex = trackp->tindex;
			nextindex = trackp->tracksecs;

			for (curindex = nindex; curindex >= 1; curindex--) {
				if (rsecno >= tindex[curindex]) {
					if (curindex < nindex)
						nextindex = tindex[curindex+1];
					break;
				}
			}
		}
	}

	for (i = 0; i < nsecs; i++) {

		if (tindex != NULL && rsecno >= nextindex) {

			if (curindex < nindex) {
				curindex++;
				nextindex = tindex[curindex+1];
			}
		}
		if (rsecno == trackp->index0start) {
			
			trackno++;
			curindex = 0;
			end = trackp->trackstart + trackp->tracksecs;
		}
		lba_to_msf((long)secno, &m);
		if (curindex == 0)
			sec_to_msf((long)end-1 - secno, &mr);
		else
			sec_to_msf((long)rsecno, &mr);
		if (is_scms(trackp)) {
			if ((secno % 8) <= 3) {
				ctrl &= ~(TM_ALLOW_COPY << 4);
			} else {
				ctrl |= TM_ALLOW_COPY << 4;
			}
		}
		filldsubq(sub, ctrl|0x01, trackno, curindex, &mr, &m);
		if (mcn && (secno == nextmcn)) {
			if (curindex == lastindex) {
				fillmcn(sub, (unsigned char *)mcn);
				nextmcn = (secno+1)/100*100 + 99;
			} else {
				nextmcn++;
			}
		}
		if (trackp->isrc && (secno == nextisrc)) {
			if (curindex == lastindex) {
				fillisrc(sub, (unsigned char *)trackp->isrc);
				nextisrc = (secno+1)/100*100 + 49;
			} else {
				nextisrc++;
			}
		}
		fillcrc(sub, 12);

		if (is_raw16(trackp)) {
			qpto16(sup, sub, curindex == 0);
		} else {
			qpto96(sup, sub, curindex == 0);
		}
		lastindex = curindex;
		secno++;
		rsecno++;
		sup += secsize;
	}
}
//--------------------------------------------------------------------
void		filltpoint(unsigned char *sub, int ctrl_adr, int point, msf_t *mp)
{
	sub[0] = ctrl_adr;
	sub[2] = point;
	sub[7] = to_bcd(mp->msf_min);
	sub[8] = to_bcd(mp->msf_sec);
	sub[9] = to_bcd(mp->msf_frame);
}
//--------------------------------------------------------------------
void		fillttime(unsigned char *sub, msf_t *mp)
{
	sub[3] = to_bcd(mp->msf_min);
	sub[4] = to_bcd(mp->msf_sec);
	sub[5] = to_bcd(mp->msf_frame);
}
//--------------------------------------------------------------------
void		qpto96(unsigned char *sub, unsigned char *subqptr, int dop)
{
	unsigned char	tmp[16];
	unsigned char	*p;
	int	c;
	int	i;

	if (subqptr == sub) 
	{
		//movebytes(subqptr, tmp, 12);
		GrDumyCopyMem( tmp, subqptr, 12);
		subqptr = tmp;
	}

	memset(sub, '\0', 96);

	/* BEGIN CSTYLED */
	if (dop) for (i = 0, p = sub; i < 96; i++) {
		*p++ |= 0x80;
	}
	for (i = 0, p = sub; i < 12; i++) {
		c = subqptr[i] & 0xFF;

		if (c & 0x80)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x40)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x20)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x10)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x08)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x04)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x02)
			*p++ |= 0x40;
		else
			p++;
		if (c & 0x01)
			*p++ |= 0x40;
		else
			p++;
	}
}
//--------------------------------------------------------------------
void		addrw(unsigned char *sub, unsigned char	*subrwptr)
{
	 int	i;

#define	DO8(a)	a; a; a; a; a; a; a; a;

	for (i = 0; i < 12; i++) {
		DO8(*sub++ |= *subrwptr++ & 0x3F);
	}
}
//--------------------------------------------------------------------
void		qwto16(unsigned char *subq, unsigned char *subptr)
{
	int	i;
	int	np = 0;
	unsigned char	*p;
	unsigned char	tmp[96];

	p = subptr;
	for (i = 0; i < 96; i++)
		if (*p++ & 0x80)
			np++;
	p = subptr;
	if (subptr == subq) 
	{
		//movebytes(subptr, tmp, 96);
		GrDumyCopyMem( tmp, subptr, 96);
		p = tmp;
	}

	for (i = 0; i < 12; i++) {
		subq[i] = 0;
		if (*p++ & 0x40)
			subq[i] |= 0x80;
		if (*p++ & 0x40)
			subq[i] |= 0x40;
		if (*p++ & 0x40)
			subq[i] |= 0x20;
		if (*p++ & 0x40)
			subq[i] |= 0x10;
		if (*p++ & 0x40)
			subq[i] |= 0x08;
		if (*p++ & 0x40)
			subq[i] |= 0x04;
		if (*p++ & 0x40)
			subq[i] |= 0x02;
		if (*p++ & 0x40)
			subq[i] |= 0x01;
	}
	subq[12] = 0;
	subq[13] = 0;
	subq[14] = 0;
	if (np > (96/2))
		subq[15] = 0x80;
}
//--------------------------------------------------------------------
void		subrecodesecs(track_t *trackp, unsigned char *bp, int address, int nsecs)
{
	bp += 2352;
	while (--nsecs >= 0) {
		qwto16(bp, bp);
		bp += trackp->isecsize;
	}
}
//--------------------------------------------------------------------
void 	encsectors(track_t	*trackp, unsigned char	*bp,	int	address,	int	nsecs)
{
	int	sectype = trackp->sectype;

	if ((sectype & ST_MODE_MASK) == ST_MODE_AUDIO)
		return;
}
//--------------------------------------------------------------------
void	packtext(int tracks, track_t *trackp)
{
	int	type;
	int	i;
	struct textpack *tp;
	struct textsizes tsize;
	txtarg_t targ;
	char	sbuf[256*18];

	memset(sbuf, 0, sizeof (sbuf));
	memset(&tsize, 0, sizeof (tsize));

	tsize.charcode		= CC_8859_1;		/* ISO-8859-1	    */
	tsize.first_track	= trackp[1].trackno;
	tsize.last_track	= trackp[1].trackno + tracks - 1;
	tsize.copyr_flags	= 0;			/* no Copyr. limitat. */

	tsize.pack_count[0x0F]	= 3;			/* 3 size packs	    */
	tsize.last_seqnum[0]	= 0;			/* Start value only */
	tsize.language_codes[0]	= 9;		/* English	    */

	tp = (struct textpack *)sbuf;

	targ.tp = tp;
	targ.p = NULL;
	targ.tsize = &tsize;
	targ.seqno = 0;

	for (type = 0; type <= 0x0E; type++) {
		int	maxtrk;
		char	*s;

		if (!anytext(type, tracks, trackp))
			continue;
		maxtrk = tsize.last_track;
		if (type == 6) {
			maxtrk = 0;
		}
		for (i = 0; i <= maxtrk; i++) {
			s = (char*)trackp[i].text;
			if (s)
				s = ((textptr_t *)s)->textcodes[type];
			if (s)
				fillpacks(&targ, s, strlen(s)+1, i, 0x80| type);
			else
				fillpacks(&targ, "", 1, i, 0x80| type);

		}
		fillup_pack(&targ);
	}

	tsize.last_seqnum[0] = targ.seqno + 2;

	for (i = 0; i < 3; i++) {
		fillpacks(&targ, &((char *)(&tsize))[i*12], 12, i, 0x8f);
	}

	setuptextdata((unsigned char *)sbuf, targ.seqno*18);
}
//--------------------------------------------------------------------
int		write_cdtext(void* A_PtrScg, void* A_PtrDev, long startsec)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	char	*bp = (char *)textsub;
	int	buflen = textlen;
	long	amount;
	long	bytes = 0;
	long	end = -150;
	int	secspt = textlen / 96;
	int	bytespt = textlen;
	long	maxdma = scgp->m_MaxBuf;
	int	idx;
	int	secs;
	int	nbytes;

	if (textlen <= 0)
		return (-1);

	if (maxdma >= (2*textlen)) {

		bp = (char *)scgp->m_PtrBuf;
		for (idx = 0; (idx + textlen) <= maxdma; idx += textlen)
		{
			//movebytes(textsub, &bp[idx], textlen);
			GrDumyCopyMem( &bp[idx], textsub, textlen);
		}
		buflen = idx;
		secspt = buflen / 96;
		bytespt = buflen;

	} else if (maxdma < buflen) {

		secspt = maxdma / 96;
		bytespt = secspt * 96;
	}
	while (startsec < end) {
		if ((end - startsec) < secspt) {
			secspt = end - startsec;
			bytespt = secspt * 96;
		}
		idx = 0;
		secs = secspt;
		nbytes = bytespt;
		do {			/* loop over CD-Text data buffer */

			if ((idx + nbytes) > buflen) {
				nbytes = buflen - idx;
				secs = nbytes / 96;
			}

			amount = write_secs(scgp, dp,	(char *)&bp[idx], startsec, nbytes, secs, FALSE);
			if (amount < 0) {
				printf("write CD-Text data: error after %ld bytes\n",
						bytes);
				return (-1);
			}
			bytes += amount;
			idx += amount;
			startsec += secs;
		} while (idx < buflen && startsec < end);
	}
	return (0);
}
//--------------------------------------------------------------------
void	setuptextdata(unsigned char *bp, int len)
{
	int	n;
	int	i;
	int	j;
	unsigned char	*p;

	if (len == 0) {
		return;
	}
	i = (len/sizeof (struct textpack)) % 4;
	if (i == 0) {
		n = len;
	} else if (i == 2) {
		n = 2 * len;
	} else {
		n = 4 * len;
	}
	n = (n * 4) / 3;
	p = (unsigned char*)malloc(n);
	if (p == NULL) {
		return;
	}
	for (i = 0, j = 0; j < n; ) {
		eight2six(&bp[i%len], &p[j]);
		i += 3;
		j += 4;
	}
	textsub = p;
	textlen = n;
}
//--------------------------------------------------------------------
BOOL	cdtext_crc_ok(struct textpack *p)
{
	int		crc;
	struct textpack	new1;

	//movebytes(p, &new1, sizeof (struct textpack));
	GrDumyCopyMem( &new1, p, sizeof (struct textpack));
	new1.crc[0] ^= 0xFF;
	new1.crc[1] ^= 0xFF;
	crc = calcCRC((unsigned char *)&new1, sizeof (struct textpack));
	crc = flip_crc_error_corr((unsigned char *)&new1, sizeof (struct textpack), crc);
	new1.crc[0] ^= 0xFF;
	new1.crc[1] ^= 0xFF;
	if (crc == 0)
	{
		//movebytes(&new1, p, 18);
		GrDumyCopyMem( p, &new1, 18);
	}
	return (crc == 0);
}
//--------------------------------------------------------------------
BOOL	anytext(int pack_type, int tracks, track_t *trackp)
{
	int	i;
	char	*p;

	for (i = 0; i <= tracks; i++) {
		if (trackp[i].text == NULL)
			continue;
		p = ((textptr_t *)(trackp[i].text))->textcodes[pack_type];
		if (p != NULL && *p != '\0')
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
void	fillup_pack(txtarg_t *ap)
{
	if (ap->p) {
		memset(ap->p, '\0', &ap->tp->text[12] - ap->p);
		fillcrc((unsigned char *)ap->tp, sizeof (*ap->tp));
		ap->p  = 0;
		ap->tp++;
	}
}
//--------------------------------------------------------------------
void	fillpacks(txtarg_t *ap, char *from, int len, int track_no, int pack_type)
{
	int		charpos;
	char		*p;
	txtpack_t	*tp;

	tp = ap->tp;
	p  = ap->p;
	charpos = 0;
	do {
		if (p == 0) {
			p = tp->text;
			tp->pack_type = pack_type;
			if (pack_type != 0x8f)
				ap->tsize->pack_count[pack_type & 0x0F]++;
			tp->track_no = track_no;
			tp->seq_number = ap->seqno++;
			if (charpos < 15)
				tp->block_number = charpos;
			else
				tp->block_number = 15;
		}
		for (; --len >= 0 && p < &tp->text[12]; charpos++) {
			*p++ = *from++;
		}
		len++;	/* Overshoot compensation */

		if (p >= &tp->text[12]) {
			fillcrc((unsigned char *)tp, sizeof (*tp));
			p = 0;
			tp++;
		}
	} while (len > 0);

	ap->tp = tp;
	ap->p = p;
}
//--------------------------------------------------------------------
void	eight2six(unsigned char *in, unsigned char *out)
{
	int	c;

	c = in[0];
	out[0]  = (c >> 2) & 0x3F;
	out[1]  = (c & 0x03) << 4;

	c = in[1];
	out[1] |= (c & 0xF0) >> 4;
	out[2]  = (c & 0x0F) << 2;

	c = in[2];
	out[2] |= (c & 0xC0) >> 6;
	out[3]  = c & 0x3F;
}
//--------------------------------------------------------------------
/*
int	flush()
{
	return (fflush(stdout));
}
*/
//--------------------------------------------------------------------
int		streql(char	*a, char	*b)
{
	char	*s1 = a;
	char	*s2 = b;

	if (s1 == NULL || s2 ==  NULL)
		return (FALSE);

	if (s1 == s2)
		return (TRUE);

	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return (TRUE);

	return (FALSE);
}
//--------------------------------------------------------------------
void	filldsubq(unsigned char *sub, int ca, int t, int i, msf_t *mrp, msf_t *mp)
{
	sub[0] = ca;
	sub[1] = to_bcd(t);
	sub[2] = to_bcd(i);
	sub[3] = to_bcd(mrp->msf_min);
	sub[4] = to_bcd(mrp->msf_sec);
	sub[5] = to_bcd(mrp->msf_frame);

	sub[7] = to_bcd(mp->msf_min);
	sub[8] = to_bcd(mp->msf_sec);
	sub[9] = to_bcd(mp->msf_frame);
}
//--------------------------------------------------------------------
void	fillmcn(unsigned char *sub, unsigned char *mcn)
{
	int	i;
	int	c;

	sub[0] = ADR_MCN;
	for (i = 1; i <= 8; i++) {
		c = *mcn++;
		if (c >= '0' && c <= '9')
			sub[i] = (c - '0') << 4;
		else
			sub[i] = 0;

		if (c != '\0')
			c = *mcn++;
		if (c >= '0' && c <= '9')
			sub[i] |= (c - '0');

		if (c == '\0') {
			i++;
			break;
		}
	}
	for (; i <= 8; i++)
		sub[i] = '\0';
}
//--------------------------------------------------------------------
void	fillisrc(unsigned char *sub, unsigned char *isrc)
{
	int	i;
	int	j;
	unsigned char		tmp[13];
	unsigned char		*sp;

	sub[0] = ADR_ISRC;
	sp = &sub[1];


	for (i = 0, j = 0; i < 12; i++) {
		if (isrc[i+j] == '-')
			j++;
		if (isrc[i+j] == '\0')
			break;
		tmp[i] = ascii2q(isrc[i+j]);
	}
	for (; i < 13; i++)
		tmp[i] = '\0';


	sp[0]  = tmp[0] << 2;
	sp[0] |= (tmp[1] >> 4) & 0x03;
	sp[1]  = (tmp[1] << 4) & 0xF0;
	sp[1] |= (tmp[2] >> 2) & 0x0F;
	sp[2]  = (tmp[2] << 6) & 0xC0;
	sp[2] |= tmp[3] & 0x3F;
	sp[3]  = tmp[4] << 2;


	for (i = 4, j = 5; i < 8; i++) {
		sp[i]  = tmp[j++] << 4;
		sp[i] |= tmp[j++];
	}
}
//--------------------------------------------------------------------
int		ascii2q(int c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= '@' && c <= 'o')
		return (0x10 + c - '@');
	return (0);
}
//--------------------------------------------------------------------
void	qpto16(unsigned char *sub, unsigned char	*subqptr,	int	dop)
{
	if (sub != subqptr)
	{
		//movebytes(subqptr, sub, 12);
		GrDumyCopyMem( sub, subqptr, 12);
	}
	sub[12] = '\0';
	sub[13] = '\0';
	sub[14] = '\0';
	sub[15] = '\0';
	if (dop)
		sub[15] |= 0x80;
}
//--------------------------------------------------------------------
int		cmpnullbytes(void	*fromp, int cnt)
{
	register const char	*from	= (char *)fromp;
	register int		n;

	if ((n = cnt) <= 0)
		return (cnt);

	while (--n >= 0 && !laligned(from)) {
		if (*from++ != 0)
			goto cdiff;
	}
	n++;

	if (n >= (int)(8 * sizeof (long))) {
		if (laligned(from)) {
			register const long *froml = (const long *)from;
			register int rem = n % (8 * sizeof (long));

			n /= (8 * sizeof (long));
			do {
				DO8(
					if (*froml++ != 0)
						break;
				);
			} while (--n > 0);

			if (n > 0) {
				--froml;
				from = (const char *)froml;
				goto ldiff;
			}
			from = (const char *)froml;
			n = rem;
		}

		if (n >= 8) {
			n -= 8;
			do {
				DO8(
					if (*from++ != 0)
						goto cdiff;
				);
			} while ((n -= 8) >= 0);
			n += 8;
		}
		if (n > 0) do {
			if (*from++ != 0)
				goto cdiff;
		} while (--n > 0);
		return (cnt);
	}
	if (n > 0) do {
		if (*from++ != 0)
			goto cdiff;
	} while (--n > 0);
	return (cnt);
ldiff:
	n = sizeof (long);
	do {
		if (*from++ != 0)
			goto cdiff;
	} while (--n > 0);
cdiff:
	return (--from - (char *)fromp);
}
//--------------------------------------------------------------------
char*	astoll(char *s,	__u64 *l)
{
	return (astollb(s, l, 0));
}
//--------------------------------------------------------------------
char*	astollb(char *s, __u64 *l, int base)
{
	int neg = 0;
	__u64 ret = (__u64)0;
	int digit;
	char c;

	while (is_space(*s))
		s++;

	if (*s == '+') {
		s++;
	} else if (*s == '-') {
		s++;
		neg++;
	}

	if (base == 0) {
		if (*s == '0') {
			base = 8;
			s++;
			if (*s == 'x' || *s == 'X') {
				s++;
				base = 16;
			}
		} else {
			base = 10;
		}
	}
	for (; (c = *s) != 0; s++) {

		if (is_digit(c)) {
			digit = c - '0';
		} else if (is_hex(c)) {
			digit = to_lower(c) - 'a' + 10;
		} else {
			break;
		}

		if (digit < base) {
			ret *= base;
			ret += digit;
		} else {
			break;
		}
	}
	if (neg)
	{
		ret = (__u64)(-((__s64)ret));
	}
	*l = ret;
	return ((char *)s);
}
//--------------------------------------------------------------------
off_t	filesize(FILE	*f)
{
#ifdef LINUX_APP
	struct stat buf;

	down(f);
	if (fstat(fileno(f), &buf) < 0) 
	{
		raisecond("filesize", 0L);
		return (-1);
	}
	return (buf.st_size);
#else
	return	0;
#endif
}
//--------------------------------------------------------------------
void	raisecond(char *signame, long arg2)
{
	void	*vp = NULL;
	vp = __roothandle;

	while (vp) 
	{
		if (framehandle((SIGBLK *)vp, signame, signame, arg2))
		{
			return;
		}
		else if (framehandle((SIGBLK *)vp, "any_other", signame, arg2))
		{
			return;
		}
		next_frame(vp);
	}
	raiseabort(signame);
}
//--------------------------------------------------------------------
void	raiseabort(char	*signame)
{
		DbgMsgPrint( "ODD - try abort\n" );
		//abort();
}
//--------------------------------------------------------------------
BOOL	framehandle(SIGBLK *sp, char	*handlename, char	*signame, long arg2)
{
	for (; sp; sp = sp->sb_signext) {
		if (sp->sb_signame != NULL &&
		    streql(sp->sb_signame, handlename)) {
			if (sp->sb_sigfun == NULL) {	/* deactivated */
				return (FALSE);
			} else {
				return (*sp->sb_sigfun)(signame,
							sp->sb_sigarg, arg2);
			}
		}
	}
	return (FALSE);
}
//--------------------------------------------------------------------
ssize_t	fileread(FILE	*f,	void *buf, size_t	len)
{
		ssize_t	cnt;
	
		down2(f, _IOREAD, _IORW);
	
		if (my_flag(f) & _JS_IOUNBUF)
			return (_niread(fileno(f), buf, len));
		cnt = fread(buf, 1, len, f);
	
		if (!ferror(f))
		{
			return (cnt);
		}
		if (!(my_flag(f) & _JS_IONORAISE))
		{
			raisecond(_readerr, 0L);
		}
		return (-1);
}
//--------------------------------------------------------------------
int	_io_get_my_flag(FILE	*fp)
{
	int	f = fileno(fp);
	_io_fl	*fl;

	if (f >= _fl_max)
		return (_more_flags(fp));

	fl = &_io_myfl[f];

	if (fl->fl_io == 0 || fl->fl_io == fp)
		return (fl->fl_flags);

	while (fl && fl->fl_io != fp)
		fl = fl->fl_next;

	if (fl == 0)
		return (0);

	return (fl->fl_flags);
}
//--------------------------------------------------------------------
int _more_flags(FILE *fp)
{
	int	f = fileno(fp);
	int	n = _fl_max;
	_io_fl	*np;

	while (n <= f)
		n += _fl_inc;

	if (_io_myfl == _io_smyfl) {
		np = (_io_fl *) malloc(n * sizeof (*np));
		memset(np, '\0', n * sizeof (*np));
		//movebytes(_io_smyfl, np, sizeof (_io_smyfl)/sizeof (*np));
		GrDumyCopyMem( np, _io_smyfl, sizeof (_io_smyfl)/sizeof (*np));
	} else {
		np = (_io_fl *) realloc(_io_myfl, n * sizeof (*np));
		if (np)
			memset(&np[_fl_max], '\0', (n-_fl_max)*sizeof (*np));
	}
	if (np) {
		_io_myfl = np;
		_fl_max = n;
		return (_io_get_my_flag(fp));
	} else {
		return (_JS_IONORAISE);
	}
}
//--------------------------------------------------------------------
ssize_t	_niread(int	f, void	*buf,	size_t count)
{
	ssize_t	ret;
	int	oerrno = errno;

	if ((ret = (ssize_t)count) < 0) {
		errno	= EINVAL;
		return ((ssize_t)-1);
	}
	while ((ret = read(f, buf, count)) < 0 && errno == EINTR) {
		errno = oerrno;
	}
	return (ret);
}
//--------------------------------------------------------------------
FILE*	_fcons(FILE	*fd, int f,	int	flag)
{
	int	my_gflag = _io_glflag;

	if (fd == (FILE *)NULL)
		fd = fdopen(f,
			fmtab[flag&(FI_READ|FI_WRITE|FI_BINARY | FI_APPEND)]);

	if (fd != (FILE *)NULL) {
		if (flag & FI_APPEND) {
			(void) fseek(fd, (off_t)0, SEEK_END);
		}
		if (flag & FI_UNBUF) {
			setbuf(fd, NULL);
			my_gflag |= _JS_IOUNBUF;
		}
		set_my_flag(fd, my_gflag); /* must clear it if fd is reused */
		return (fd);
	}
#ifdef LINUX_APP
	if (flag & FI_CLOSE)
		close(f);
#endif

	return ((FILE *) NULL);
}
//--------------------------------------------------------------------
void	_io_set_my_flag(FILE *fp,	int	flag)
{
	int	f = fileno(fp);
	_io_fl	*fl;
	_io_fl	*fl2;

	if (f >= _fl_max)
		(void) _more_flags(fp);

	fl = &_io_myfl[f];

	if (fl->fl_io != (FILE *)0) {
		fl2 = fl;

		while (fl && fl->fl_io != fp)
			fl = fl->fl_next;
		if (fl == 0) {
			if ((fl = (_io_fl *) malloc(sizeof (*fl))) == 0)
				return;
			fl->fl_next = fl2->fl_next;
			fl2->fl_next = fl;
		}
	}
	fl->fl_io = fp;
	fl->fl_flags = flag;
}
//--------------------------------------------------------------------
int _openfd(char *name,	int omode)
{
	return (open(name, omode, (mode_t)0666));
}
//--------------------------------------------------------------------
int	_cvmod(char	*mode, int *omode, int *flag)
{
	while (*mode) {
		switch (*mode) {

		case 'r':   *omode |= O_RDONLY;	*flag |= FI_READ;	break;
		case 'w':   *omode |= O_WRONLY;	*flag |= FI_WRITE;	break;
		case 'e':   *omode |= O_EXCL;				break;
		case 'c':   *omode |= O_CREAT;	*flag |= FI_CREATE;	break;
		case 't':   *omode |= O_TRUNC;	*flag |= FI_TRUNC;	break;
		case 'a':   *omode |= O_APPEND;	*flag |= FI_APPEND;	break;
		case 'u':			*flag |= FI_UNBUF;	break;
			/* dummy on UNIX */
		case 'b':   *omode |= O_BINARY; *flag |= FI_BINARY;	break;
			/*
			 * XXX do we need this ?
			 * XXX May this be a problem?
			 */
		case 'l':   *omode |= O_LARGEFILE;			break;
		default:    raisecond(_badmode, 0L);
			    return (0);
		}
		mode++;
	}
	if (*flag & FI_READ && *flag & FI_WRITE) {
		*omode &= ~(O_RDONLY|O_WRONLY);
		*omode |= O_RDWR;
	}
	return (1);
}
//--------------------------------------------------------------------
int	fgetline(FILE	*f, char *buf, int len)
{
	int	c	= '\0';
	char	*bp	= buf;
	int	nl	= '\n';

	down2(f, _IOREAD, _IORW);

	for (;;) {
		if ((c = getc(f)) < 0)
			break;
		if (c == nl)
			break;
		if (--len > 0) {
			*bp++ = (char)c;
		} else {
			/*
			 * Read up to end of line
			 */
			while ((c = getc(f)) >= 0 && c != nl)
				/* LINTED */
				;
			break;
		}
	}
	*bp = '\0';
	/*
	 * If buffer is empty and we hit EOF, return EOF
	 */
	if (c < 0 && bp == buf)
		return (c);

	return (bp - buf);
}
//--------------------------------------------------------------------
FILE*	fileluopen(int f, char *mode)
{
	int	omode = 0;
	int	flag = 0;

	if (!_cvmod(mode, &omode, &flag))
		return ((FILE *) NULL);

	if (omode & (O_NDELAY|O_CREAT|O_TRUNC|O_EXCL)) {
		raisecond(_badmode, 0L);
		return ((FILE *) NULL);
	}

	if (fcntl(f, F_GETFD, (long)0) < 0) 
	{
		raisecond(_badfile, 0L);
		return ((FILE *) NULL);
	}

	if (omode & O_APPEND)
		lseek(f, (off_t)0, SEEK_END);

	return (_fcons((FILE *)0, f, flag));
}
//--------------------------------------------------------------------
__u32 disk_rcap(msf_t	*mp, __u32	maxblock, BOOL rw, BOOL	audio)
{
	struct disk_rcap * dp;

	dp = rcap;
	while (dp->ci_msf.msf_min != 0) {
		if (mp->msf_min == dp->ci_msf.msf_min &&
				mp->msf_sec == dp->ci_msf.msf_sec &&
				mp->msf_frame == dp->ci_msf.msf_frame &&
				maxblock == dp->ci_cap)
			return (dp->ci_rcap);
		dp++;
	}
	return (0L);
}
//--------------------------------------------------------------------
int		encspeed()
{
	track_t	t[1];
	unsigned char	sect[2352];
	int	i;
	struct	timeval tv;
	struct	timeval tv2;

	t[0].sectype = ST_MODE_1;

	for (i = 0; i < 2352; )
	{
		sect[i++] = 'J';
		sect[i++] = 'S';
	}
	gettimeofday(&tv, (struct timezone *)0);
	for (i = 0; i < 75000; i++)
	{
		encsectors(t, sect, 12345, 1);
		gettimeofday(&tv2, (struct timezone *)0);
		if (tv2.tv_sec >= (tv.tv_sec+1) &&
		    tv2.tv_usec >= tv.tv_usec)
			break;
	}

	return ((i+74)/75);
}
//--------------------------------------------------------------------
__u64	isosize(int	f, off_t* A_PtrOffSet)
{
	struct iso9660_voldesc		vd;
	struct iso9660_pr_voldesc	*vp;
	__u64				isize;
#ifdef LINUX_APP
	struct stat			sb;
	mode_t				mode;
#endif

	if (isatty(f))
		return ((__u64)-1);
#ifdef LINUX_APP
	if (fstat(f, &sb) < 0)
		return ((__u64)-1);
	mode = sb.st_mode & S_IFMT;
	if (!S_ISREG(mode) && !S_ISBLK(mode) && !S_ISCHR(mode))
		return ((__u64)-1);
#endif

	//if (lseek(f, (off_t)(16L * 2048L), SEEK_SET) == -1)
	if (lseek(f, *A_PtrOffSet + (off_t)(16L * 2048L), SEEK_SET) == -1)
		return ((__u64)-1);

	vp = (struct iso9660_pr_voldesc *) &vd;

	do {
		read(f, &vd, sizeof (vd));
		if (GET_UBYTE(vd.vd_type) == VD_PRIMARY)
			break;

	} while (GET_UBYTE(vd.vd_type) != VD_TERM);

	//lseek(f, (off_t)0L, SEEK_SET);
	lseek(f, *A_PtrOffSet + (off_t)0L, SEEK_SET);

	if (GET_UBYTE(vd.vd_type) != VD_PRIMARY)
		return (-1L);

	isize = (__u64)GET_BINT(vp->vd_volume_space_size);
	isize *= GET_BSHORT(vp->vd_lbsize);
	return (isize);
}
//--------------------------------------------------------------------
void	swabbytes(void *vp, int	cnt)
{
	char	*bp = (char *)vp;
	char	c;

	cnt /= 2;	/* even count only */
	while ((cnt -= 8) >= 0) {
		DO8(c = *bp++; bp[-1] = *bp; *bp++ = c;);
	}
	cnt += 8;
	while (--cnt >= 0) {
		c = *bp++; bp[-1] = *bp; *bp++ = c;
	}
}
//--------------------------------------------------------------------
void	scatter_secs(track_t *trackp,	char *bp,	int	nsecs)
{
	char	*from;
	char	*to;
	int	isecsize = trackp->isecsize;
	int	secsize = trackp->secsize;
	int	i;

	if (secsize == isecsize)
		return;

	nsecs -= 1;	/* we do not move sector # 0 */

	if (secsize < isecsize) {
		from = bp + isecsize;
		to   = bp + secsize;

		for (i = nsecs; i > 0; i--) 
		{
			//movebytes(from, to, secsize);
			GrDumyCopyMem( to, from, secsize);
			from += isecsize;
			to   += secsize;
		}
	} else {
		from = bp + (nsecs * isecsize);
		to   = bp + (nsecs * secsize);

		for (i = nsecs; i > 0; i--) 
		{
			//movebytes(from, to, isecsize);
			GrDumyCopyMem( to, from, isecsize);
			from -= isecsize;
			to   -= secsize;
		}
	}
}
//--------------------------------------------------------------------
void	scrsectors(track_t *trackp,	unsigned char	*bp, int address,	int	nsecs)
{
	while (--nsecs >= 0)
	{
		if (IS_SECHDR(bp))
			scramble_L2(bp);
		bp += trackp->secsize;
	}
}
//--------------------------------------------------------------------
int		scramble_L2(unsigned char *inout)
{
	if (!xaligned(inout + 12, sizeof(__u32)-1)) {

		unsigned char		*r = inout + 12;
		unsigned char	*s = yellowbook_scrambler;
		int	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof(unsigned char)/4; --i >= 0;) {
			DO4(*r++ ^= *s++);
		}

	} else {
		__u32	*r = (__u32 *)(inout + 12);
		const __u32 *s = yellowbook_scrambler_DWORD;
		int	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof(__u32)/13; --i >= 0;) {
			DO13(*r++ ^= *s++);
		}
	}

	return (0);
}
//--------------------------------------------------------------------
struct ricoh_mode_page_30*	get_justlink_ricoh(void* A_PtrScg, unsigned char *mode)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	modec[0x100];
	int	len;
	struct	ricoh_mode_page_30 *mp;

	if (!get_mode_params(scgp, 0x30, "Ricoh Vendor Page", mode, modec, NULL, NULL, &len)) {
		return ((struct ricoh_mode_page_30 *)0);
	}

	if (len < 10)
		return ((struct ricoh_mode_page_30 *)0);

	mp = (struct ricoh_mode_page_30 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if ((len - ((unsigned char *)mp - mode) -1) < 5)
		return ((struct ricoh_mode_page_30 *)0);

	return (mp);
}
//--------------------------------------------------------------------
int		check_writemodes_mmc(void* A_PtrScg, void* A_PtrDev)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	unsigned char	mode[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;
	__u32		Tv_Val;


	deflt_writemodes_mmc(scgp, TRUE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len)) {
			return (-1);
	}
	if (len == 0) {
		return (-1);
	}

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->write_type = WT_TAO;
	mp->track_mode = TM_DATA;
	mp->dbtype = DB_ROM_MODE1;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_TAO;
		dp->SetFlags(Tv_Val);
	}
	else
	{
		Tv_Val	= dp->GetFlags() & ~CDR_TAO;
		dp->SetFlags(Tv_Val);
	}

	mp->write_type = WT_PACKET;
	mp->track_mode |= TM_INCREMENTAL;

	mp->fp = 0;
	i_to_4_byte(mp->packet_size, 0);

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_PACKET;
		dp->SetFlags(Tv_Val);		
	}
	else
	{
		Tv_Val	= dp->GetFlags() & ~CDR_PACKET;
		dp->SetFlags(Tv_Val);
	}
	mp->fp = 0;
	i_to_4_byte(mp->packet_size, 0);
	mp->track_mode = TM_DATA;


	mp->write_type = WT_SAO;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_SAO;
		dp->SetFlags(Tv_Val);		
	}
	else
	{
		Tv_Val	= dp->GetFlags() & ~CDR_SAO;
		dp->SetFlags(Tv_Val);
	}

	if (dp->GetFlags() & CDR_SAO) {
		mp->dbtype = DB_RAW_PQ;


		mp->dbtype = DB_RAW_PW;

		if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		{
			Tv_Val	= dp->GetFlags() | CDR_SRAW96P;
			dp->SetFlags(Tv_Val);		
		}

		mp->dbtype = DB_RAW_PW_R;

		if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		{
			Tv_Val	= dp->GetFlags() | CDR_SRAW96R;
			dp->SetFlags(Tv_Val);
		}
	}

	mp->write_type = WT_RAW;
	mp->dbtype = DB_RAW_PQ;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_RAW | CDR_RAW16;
		dp->SetFlags(Tv_Val);
	}

	mp->dbtype = DB_RAW_PW;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_RAW | CDR_RAW96P;
		dp->SetFlags(Tv_Val);
	}

	mp->dbtype = DB_RAW_PW_R;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
	{
		Tv_Val	= dp->GetFlags() | CDR_RAW | CDR_RAW96R;
		dp->SetFlags(Tv_Val);
	}

	mp->track_mode = TM_DATA;
	mp->write_type = WT_LAYER_JUMP;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1) && has_profile(scgp, 0x16) == 1)
	{
		Tv_Val	= dp->GetFlags() | CDR_LAYER_JUMP;
		dp->SetFlags(Tv_Val);
	} 
	else
	{
		Tv_Val	= dp->GetFlags() & ~CDR_LAYER_JUMP;
		dp->SetFlags(Tv_Val);
	}

	deflt_writemodes_mmc(scgp, TRUE);

	return (0);
}
//--------------------------------------------------------------------
int		deflt_writemodes_mmc(void* A_PtrScg, BOOL reset_dummy)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len)) {
			return (-1);
	}
	if (len == 0) {
		return (-1);
	}

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if (reset_dummy)
		mp->test_write = 0;

	mp->write_type = WT_TAO;
	mp->track_mode = TM_DATA;
	mp->dbtype = DB_ROM_MODE1;
	mp->session_format = SES_DA_ROM; /* Matsushita has illegal def. value */

	i_to_2_byte(mp->audio_pause_len, 150);	/* LG has illegal def. value */

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {

		mp->write_type	= WT_SAO;
		mp->LS_V	= 0;
		mp->copy	= 0;
		mp->fp		= 0;
		mp->multi_session  = MS_NONE;
		mp->host_appl_code = 0;

		if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
			return (-1);
		}
	}
	return (0);
}
//--------------------------------------------------------------------
int		cmd_dummy(void* A_PtrScg,	void* A_PtrDev)
{
	return (0);
}
//--------------------------------------------------------------------
int		cmd_ill(void* A_PtrScg)
{
	return (-1);
}
//--------------------------------------------------------------------
int		format_dummy(void* A_PtrScg, void* A_PtrDev, int fmtflags)
{
	return (-1);
}
//--------------------------------------------------------------------
int		blank_dummy(void* A_PtrScg, void* A_PtrDev, long addr, int blanktype)
{
	return (-1);
}
//--------------------------------------------------------------------
int		buf_dummy(void* A_PtrScg,	long	*sp,	long	*fp)
{
	return (-1);
}
//--------------------------------------------------------------------
void*	get_cdrcmds(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase	*dp = NULL;
	Cls_GrOddDevBase	*odp = NULL;
	BOOL	is_wr = FALSE;
	BOOL	is_cd = FALSE;
	BOOL	is_dvd = FALSE;
	BOOL	is_dvdplus = FALSE;
	BOOL	is_ddcd = FALSE;
	BOOL	is_cdwr = FALSE;
	BOOL	is_dvdwr = FALSE;
	BOOL	is_dvdpluswr = FALSE;
	BOOL	is_ddcdwr = FALSE;

	/*
	 * First check for St_GrScsiIf-3/mmc-3 drives.
	 */
	if (get_proflist(scgp, &is_wr, &is_cd, &is_dvd,
						&is_dvdplus, &is_ddcd) >= 0) {

		get_wproflist(scgp, &is_cdwr, &is_dvdwr,
						&is_dvdpluswr, &is_ddcdwr);
		if (!is_wr) {
			//DbgMsgPrint("scsi driver : cdr_cd\n");
			dp = V_PtrDevCd;
		} else {
			//DbgMsgPrint("scsi driver : cdr_cd_dvd\n");
			dp = V_PtrCdDvd;
		}
	} else
	/*
	 * First check for SCSI-3/mmc drives.
	 */
	if (is_mmc(scgp, &is_cdwr, &is_dvdwr)) {
		DbgMsgPrint("Found MMC drive CDWR: %d DVDWR: %d.\n",	is_cdwr, is_dvdwr);

		if (is_cdwr && is_dvdwr)
		{
			//DbgMsgPrint("scsi driver : cdr_cd_dvd\n");
			dp = V_PtrCdDvd;
		}
		else
		if (is_dvdwr)
		{
			//DbgMsgPrint("scsi driver : cdr_dvd\n");
			dp = V_PtrDevDvd;
		}
		else
		{
			//DbgMsgPrint("scsi driver : cdr_mmc\n");
			dp = V_PtrDevMmc;
		}

	}
	else 
	{
		switch (scgp->m_Dev)
		{
		case DEV_CDROM:
			//DbgMsgPrint("scsi driver : cdr_oldcd\n");
			dp = V_PtrDevOldCd;
			break;
		case DEV_MMC_CDROM:
			//DbgMsgPrint("scsi driver : cdr_cd\n");
			dp = V_PtrDevCd;
			break;
		case DEV_MMC_CDR:
			//DbgMsgPrint("scsi driver : cdr_mmc\n");
			dp = V_PtrDevMmc;
			break;
		case DEV_MMC_CDRW:
			//DbgMsgPrint("scsi driver : cdr_mmc\n");
			dp = V_PtrDevMmc;
			break;
		case DEV_MMC_DVD_WR:
			//DbgMsgPrint("scsi driver : cdr_cd_dvd\n");
			dp = V_PtrCdDvd;
			break;
	/*
		case DEV_CDD_521_OLD:	dp = &cdr_philips_cdd521O;	break;
		case DEV_CDD_521:	dp = &cdr_philips_cdd521;	break;
		case DEV_CDD_522:
		case DEV_CDD_2000:
		case DEV_CDD_2600:	dp = &cdr_philips_cdd522;	break;
		case DEV_TYUDEN_EW50:	dp = &cdr_tyuden_ew50;		break;
		case DEV_PCD_600:	dp = &cdr_kodak_pcd600;		break;
		case DEV_YAMAHA_CDR_100:dp = &cdr_yamaha_cdr100;	break;
		case DEV_MATSUSHITA_7501:dp = &cdr_cw7501;		break;*/
		case DEV_MATSUSHITA_7502:
		case DEV_YAMAHA_CDR_400:
			DbgMsgPrint("scsi driver : cdr_mmc\n");
			dp = V_PtrDevMmc;			
			break;
		/*case DEV_PLASMON_RF_4100:dp = &cdr_plasmon_rf4100;	break;
		case DEV_SONY_CDU_924:	dp = &cdr_sony_cdu924;		break;
		case DEV_RICOH_RO_1060C:dp = &cdr_ricoh_ro1060;		break;
		case DEV_RICOH_RO_1420C:dp = &cdr_ricoh_ro1420;		break;
		case DEV_TEAC_CD_R50S:	dp = &cdr_teac_cdr50;		break;

		case DEV_PIONEER_DW_S114X: dp = &cdr_pioneer_dw_s114x;	break;*/
		case DEV_PIONEER_DVDR_S101:
			//DbgMsgPrint("scsi driver : cdr_dvd\n");
			dp = V_PtrDevDvd;
			break;
		default:
			//DbgMsgPrint("scsi driver : default cdr_mmc\n");
			dp = V_PtrDevMmc;
		}
	}
	
	odp = dp;

	DbgMsgPrint("Using driver '%s' for identify.\n",	dp != NULL ? dp->GetDevName() :	"<no driver>");

	if ( NULL != dp )
	{
		dp = dp->Identify(scgp, dp);
	}

	return (dp);
}
//--------------------------------------------------------------------
int	drive_getdisktype(void* A_PtrScg, void* A_PtrDev)
{
	return (0);
}
//--------------------------------------------------------------------
int		no_sendcue(void* A_PtrScg, void* A_PtrDev, track_t	*trackp)
{
	return (-1);
}
//--------------------------------------------------------------------
int		drive_attach(void* A_PtrScg, void* A_PtrDev)
{
	return 0;
}
//--------------------------------------------------------------------
int		no_diskstatus(void* A_PtrScg,	void* A_PtrDev)
{
	return (-1);
}
//--------------------------------------------------------------------
int		blank_simul(void* A_PtrScg,	void* A_PtrDev, long	addr,	int	blanktype)
{
	// local -------------------
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
		track_t	*trackp;
		dstat_t* Tv_PtrDstat;
		int secsize;
		int ret ;
	// code --------------------

	 Tv_PtrDstat = dp->GetDstat();
	 trackp = Tv_PtrDstat->ds_trackp;
 
	 secsize = trackp->secsize;
	 __u64 padbytes = 0;   /* Make stupid GCC happy */
	 ret = -1;

	switch (blanktype) {

	case BLANK_MINIMAL:
			padbytes = 1000 * secsize;
			break;
	case BLANK_DISC:
			if (Tv_PtrDstat->ds_maxblocks > 0)
				padbytes = Tv_PtrDstat->ds_maxblocks * (__u32)secsize;
			break;
	default:
			padbytes = 0;
	}
	if (padbytes > 0) {
		ret = pad_track(scgp, dp, trackp, 0, padbytes, TRUE, NULL);
	}
	if (0) {
		return (-1);
	}
	return (ret);

}
//--------------------------------------------------------------------
int		pad_track(void* A_PtrScg, void* A_PtrDev, track_t *trackp, long startsec, __u64 amt,
	BOOL dolast, __u64 *bytesp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	int	track = trackp->trackno;
	dstat_t* Tv_PtrDstat;
	__u64	bytes	= 0;
	__u64 savbytes = 0;
	__u64	padsize = amt;
	int	secsize;
	int	secspt;
	int	bytespt;
	int	amount;
	BOOL	neednl	= FALSE;
	BOOL	islast	= FALSE;
	struct timeval tlast;
	float	secsps = 75.0;
	long bsize;
	long bfree;

	Tv_PtrDstat = dp->GetDstat();
	if (Tv_PtrDstat->ds_flags & DSF_DVD)
		secsps = 676.27;
	if (Tv_PtrDstat->ds_flags & DSF_BD)
		secsps = 2195.07;

	if (dp->buffer_cap(scgp, &bsize, &bfree) < 0)
		bsize = -1L;
	if (bsize == 0)		/* If we have no (known) buffer, we cannot */
		bsize = -1L;	/* retrieve the buffer fill ratio	   */

	secsize = trackp->secsize;
	secspt = trackp->secspt;
	bytespt = secsize * secspt;

	memset(scgp->m_PtrBuf, '\0', bytespt);

	/*
	if ((amt >> 20) > 0) {
		flush();
	}
	*/
	gettimeofday(&tlast, (struct timezone *)0);
	do {
		if (amt < bytespt) {
			bytespt = roundup(amt, secsize);
			secspt = bytespt / secsize;
		}
		if (dolast && (amt - bytespt) <= 0)
			islast = TRUE;

		if (is_raw(trackp)) {
			encsectors(trackp, (unsigned char *)scgp->m_PtrBuf, startsec, secspt);
			fillsubch(trackp, (unsigned char *)scgp->m_PtrBuf, startsec, secspt);
		}

		amount = write_secs(scgp, dp, (char*)scgp->m_PtrBuf, startsec, bytespt, secspt, islast);
		if (amount < 0) {
			printf("%swrite track pad data: error after %lld bytes\n",
							neednl?"\n":"", bytes);
			if (bytesp)
				*bytesp = bytes;
			dp->buffer_cap(scgp, (long *)0, (long *)0);
			return (-1);
		}
		amt -= amount;
		bytes += amount;
		startsec += amount / secsize;

	} while (amt > 0);

	if (bytesp)
		*bytesp = bytes;
	if (bytes == 0)
		return (0);
	return (bytes > 0 ? 1:-1);
}
//--------------------------------------------------------------------
int waitformat(void* A_PtrScg, int secs)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	i;
	int	key;
#define	W_SLEEP	2

	for (i = 0; i < secs/W_SLEEP; i++) {
		if (test_unit_ready(scgp) >= 0) {
			return (0);
		}
		key = scgp->Scg_Sense_Key();
		if (key != SC_UNIT_ATTENTION && key != SC_NOT_READY)
			break;

		sleep(W_SLEEP);
	}
	return (-1);
#undef	W_SLEEP
}
//--------------------------------------------------------------------
int		request_sense(void* A_PtrScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		char	sensebuf[CCS_SENSE_LEN];
		char	*cmdsave;

		cmdsave = scgp->m_CmdName;

		if (request_sense_b(scgp, sensebuf, sizeof (sensebuf)) < 0)
			return (-1);
		scgp->m_CmdName = cmdsave;
		 ((char *)sensebuf, CCS_SENSE_LEN - scgp->scg_getresid());

		return (0);
}
//--------------------------------------------------------------------
int		request_sense_b(void* A_PtrScg, caddr_t bp, int cnt)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		struct	scg_cmd	*scmd = &scgp->m_Scmd;

		memset((caddr_t)scmd, '\0', sizeof(*scmd));
		scmd->addr = bp;
		scmd->size = cnt;
		scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
		scmd->cdb_len = SC_G0_CDBLEN;
		scmd->sense_len = CCS_SENSE_LEN;
		scmd->cdb.g0_cdb.cmd = SC_REQUEST_SENSE;
		scmd->cdb.g0_cdb.lun = scg_lun(scgp);
		scmd->cdb.g0_cdb.count = cnt;

		scgp->m_CmdName = "request_sense";

		if (scgp->Scg_Cmd() < 0)
			return (-1);
		return (0);
}
//--------------------------------------------------------------------
