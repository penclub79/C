/*
	scsi opr
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
	#include <unistd.h>
	#include 	<signal.h>
	#include 	<wait.h>
	#include <sys/mman.h>
#endif
#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include 	<errno.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<string.h>

#include	<Hw/Odd/GrScsiOpr.h>
#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrDrvMmc.h>
#include 	<Hw/Odd/GrScrambleTable.h>

//====================================================================
//local const
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
#define	DBCC	 0x80		/* Double byte __s8 indicator in block */

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

#define	le_a_to_u_short(a)	((unsigned __s16) \
				((((unsigned __s8 *)a)[0]	& 0xFF) | \
				(((unsigned __s8 *)a)[1] << 8	& 0xFF00)))

#define	le_a_to_u_long(a)	((unsigned long) \
				((((unsigned __s8 *)a)[0]	& 0xFF) | \
				(((unsigned  __s8 *)a)[1] << 8	& 0xFF00) | \
				(((unsigned  __s8 *)a)[2] << 16	& 0xFF0000) | \
				(((unsigned  __s8 *)a)[3] << 24	& 0xFF000000UL)))

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
//local type
__u16 crctab[1<<BPB] = {
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
__s8	*fmtab[] = {
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
	unsigned __s8	pack_type;	/* Pack Type indicator	*/
	__s8	track_no;	/* Track Number (0..99)	*/
	__s8	seq_number;	/* Sequence Number	*/
	__s8	block_number;	/* Block # / Char pos	*/
	__s8	text[12];	/* CD-Text Data field	*/
	__s8	crc[2];		/* CRC 16		*/
} txtpack_t;

typedef struct textsizes {
	__s8	charcode;
	__s8	first_track;
	__s8	last_track;
	__s8	copyr_flags;
	__s8	pack_count[16];
	__s8	last_seqnum[8];
	__s8	language_codes[8];
} txtsize_t;

typedef struct textargs {
	txtpack_t	*tp;
	__s8		*p;
	txtsize_t	*tsize;
	__s32		seqno;
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
	unsigned __s8	magic[4];
	unsigned __s8	hdr_size[4];
	unsigned __s8	data_size[4];
	unsigned __s8	encoding[4];
	unsigned __s8	sample_rate[4];
	unsigned __s8	channels[4];
} sun_au_t;

typedef struct {
	unsigned __s8	ckid[4];
	unsigned __s8	cksize[4];
} chunk_t;

typedef struct {
	unsigned __s8	wave[4];
} riff_chunk;

typedef struct {
	unsigned __s8	fmt_tag[2];
	unsigned __s8	channels[2];
	unsigned __s8	sample_rate[4];
	unsigned __s8	av_byte_rate[4];
	unsigned __s8	block_size[2];
	unsigned __s8	bits_per_sample[2];
} fmt_chunk;

//fifo
struct faio_stats {
	long	puts;
	long	gets;
	long	empty;
	long	full;
	long	done;
	long	cont_low;
	__s32	users;
} *sp;

__s8	*onames[] = {
	"none",
	"writer",
	"faio",
	"reader",
};

//====================================================================
//local function
__u32		updcrc(__u32 p_crc, __u8 *cp, __u32 cnt);

//cdtext
void	setuptextdata(unsigned __s8 *bp, __s32 len);
BOOL8	cdtext_crc_ok(struct textpack *p);
BOOL8	anytext(__s32 pack_type, __s32 tracks, track_t *trackp);
void	fillup_pack(txtarg_t *ap);
void	fillpacks(txtarg_t *ap, __s8 *from, __s32 len, __s32 track_no, __s32 pack_type);
void	eight2six(unsigned __s8 *in, unsigned __s8 *out);

//subchan
void	filldsubq(unsigned __s8 *sub, __s32 ca, __s32 t, __s32 i, msf_t *mrp, msf_t *mp);
void	fillmcn(unsigned __s8 *sub, unsigned __s8 *mcn);
void	fillisrc(unsigned __s8 *sub, unsigned __s8 *isrc);
__s32		ascii2q(__s32 c);
void	qpto16(unsigned __s8 *sub, unsigned __s8 *subq, __s32 dop);

//raisecond
void	raiseabort(__s8	*signame);
BOOL8	framehandle(SIGBLK *sp, __s8	*handlename, __s8	*signame,	long arg2);

//flag
__s32 _more_flags(FILE *fp);
void	_io_set_my_flag(FILE *fp,	__s32	flag);

//fcons
FILE*	_fcons(FILE	*fd, __s32 f,	__s32	flag);

//open fd
__s32 _openfd(__s8 *name,	__s32 omode);

//cvmod
__s32	_cvmod(__s8	*mode, __s32 *omode, __s32 *flag);

//====================================================================
//var

extern __s32	_nsubh;
extern unsigned __s8	_subq[110][12];
extern unsigned __s8	*textsub;
extern __s32	textlen;

//====================================================================
//--------------------------------------------------------------------
__s8 *	astoi(const __s8 *s, __s32 *i)
{
	long l;
	__s8 *ret;

	ret = astol(s, &l);
	*i = l;
	return (ret);
}
//--------------------------------------------------------------------
__s8 *	astol(const __s8 *s, long *l)
{
	return (astolb(s, l, 0));
}
//--------------------------------------------------------------------
__s8 *	astolb(const __s8 *s,	long *l, __s32 base)
{
	__s32 neg = 0;
	long ret = 0L;
	__s32 digit;
	__s8 c;

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
	return ((__s8 *)s);
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
FILE*		fileopen(__s8	*name, __s8	*mode)
{
	__s32	ret;
	__s32	omode = 0;
	__s32	flag = 0;

	if (!_cvmod(mode, &omode, &flag))
		return ((FILE *) NULL);

	if ((ret = _openfd(name, omode)) < 0)
		return ((FILE *) NULL);

	return (_fcons((FILE *)0, ret, flag | FI_CLOSE));
}
//--------------------------------------------------------------------
__s32			from_bcd(__s32 b)
{
	return ((b & 0x0F) + 10 * (((b)>> 4) & 0x0F));
}
//--------------------------------------------------------------------
__s32			to_bcd(__s32 i)
{
	return (i % 10 | ((i / 10) % 10) << 4);
}
//--------------------------------------------------------------------
long		msf_to_lba(__s32 m, __s32 s, __s32 f, BOOL8 force_positive)
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
BOOL8		lba_to_msf(long lba, msf_t *mp)
{
	__s32	m;
	__s32	s;
	__s32	f;

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
	__s32	m;
	__s32	s;
	__s32	f;

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
BOOL8		is_cdspeed(__s32 speed)
{
	__s32	cdspeed;
	long	cdrest;
	__s32	dvdspeed;
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
__u16	calcCRC(unsigned __s8 *buf, __u32 bsize)
{
	return (__u16)(updcrc(0x0000, (__u8 *)buf, bsize));
}
//--------------------------------------------------------------------
__u16	fillcrc(unsigned __s8 *buf, __u32 bsize)
{
	__u16	crc = calcCRC(buf, bsize-2);


	crc = crc ^ 0xFFFF;

	buf[bsize-2] = (crc >> 8) & 0xFF;
	buf[bsize-1] = crc & 0xFF;
	return (crc);
}
//--------------------------------------------------------------------
__u16	flip_crc_error_corr(unsigned __s8 *b, __u32 bsize, __u32 p_crc)
{
	__u16		crc = (__u16)p_crc;
	__u32		btsize = bsize * BPB;

	if (crc != 0) {
		__s32	i;

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
	__u16	crc = p_crc;

	while (cnt-- > 0) {
		crc = (crc<<BPB) ^ crctab[(crc>>(BPW-BPB)) ^ (*cp++)];
	}
	return (crc);
}
//--------------------------------------------------------------------
__s32			do_leadin(track_t *trackp)
{
	__s32	tracks = trackp->tracks;
	msf_t	m;
	__s32	ctrl;
	__s32	i;
	__s32	toctype = trackp[0].tracktype & TOC_MASK;

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
__s32			write_leadin(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp, __s32 leadinstart)
{
	msf_t	m;
	__s32	i;
	__u32	j;
	unsigned __s8	*bp = (unsigned __s8*)scgp->bufptr;
	unsigned __s8	*subp;
	unsigned __s8	*sp;
	__s32	secsize;
	__s32	secspt;
	__s32	bytespt;
	long	amount;
	__s32	startsec;
	long	bytes = 0L;
	__s32	textoff = 0;
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

			amount = write_secs(scgp, dp,
					(__s8 *)bp, startsec, bytespt, secspt, FALSE);
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
__s32			write_leadout(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp)
{
	__s32	tracks = trackp->tracks;
	msf_t	m;
	msf_t	mr;
	__s32	ctrl;
	__s32	i;
	__s32	j;
	unsigned __s8	*bp = (unsigned __s8*)scgp->bufptr;
	unsigned __s8	*subp;
	unsigned __s8	*sp;
	__s32	secsize;
	__s32	secspt;
	__s32	bytespt;
	long	amount;
	long	startsec;
	long	endsec;
	long	bytes = 0L;
	__s32	leadoutstart;
	unsigned __s8	sub[12];
	BOOL8	p;
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

			amount = write_secs(scgp, dp,
					(__s8 *)bp, startsec, bytespt, secspt, FALSE);
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
void		fillsubch(track_t *trackp, unsigned __s8 *sp, __s32 secno, __s32 nsecs)
{
	msf_t	m;
	msf_t	mr;
	__s32	ctrl;
	__s32	i;
	__s32	rsecno;
	__s32	end;
	__s32	secsize = trackp->secsize;
	__s32	trackno = trackp->trackno;
	__s32	nindex = trackp->nindex;
	__s32	curindex;
	long	*tindex = NULL;
	long	nextindex = 0L;
	unsigned __s8	sub[12];
	unsigned __s8 *sup;
	__s8	*mcn;
	/*
	 * In theory this should make fillsubch() non-reenrtrant but it seems
	 * that the probability check at the beginning of the function is
	 * sufficient to make it work as expected.
	 */
static	long	nextmcn = -1000000L;
static	long	nextisrc = -1000000L;
static	unsigned __s8	lastindex = 255;

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
				fillmcn(sub, (unsigned __s8 *)mcn);
				nextmcn = (secno+1)/100*100 + 99;
			} else {
				nextmcn++;
			}
		}
		if (trackp->isrc && (secno == nextisrc)) {
			if (curindex == lastindex) {
				fillisrc(sub, (unsigned __s8 *)trackp->isrc);
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
void		filltpoint(unsigned __s8 *sub, __s32 ctrl_adr, __s32 point, msf_t *mp)
{
	sub[0] = ctrl_adr;
	sub[2] = point;
	sub[7] = to_bcd(mp->msf_min);
	sub[8] = to_bcd(mp->msf_sec);
	sub[9] = to_bcd(mp->msf_frame);
}
//--------------------------------------------------------------------
void		fillttime(unsigned __s8 *sub, msf_t *mp)
{
	sub[3] = to_bcd(mp->msf_min);
	sub[4] = to_bcd(mp->msf_sec);
	sub[5] = to_bcd(mp->msf_frame);
}
//--------------------------------------------------------------------
void		qpto96(unsigned __s8 *sub, unsigned __s8 *subqptr, __s32 dop)
{
	unsigned __s8	tmp[16];
	unsigned __s8	*p;
	__s32	c;
	__s32	i;

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
void		addrw(unsigned __s8 *sub, unsigned __s8	*subrwptr)
{
	 __s32	i;

#define	DO8(a)	a; a; a; a; a; a; a; a;

	for (i = 0; i < 12; i++) {
		DO8(*sub++ |= *subrwptr++ & 0x3F);
	}
}
//--------------------------------------------------------------------
void		qwto16(unsigned __s8 *subq, unsigned __s8 *subptr)
{
	__s32	i;
	__s32	np = 0;
	unsigned __s8	*p;
	unsigned __s8	tmp[96];

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
void		subrecodesecs(track_t *trackp, unsigned __s8 *bp, __s32 address, __s32 nsecs)
{
	bp += 2352;
	while (--nsecs >= 0) {
		qwto16(bp, bp);
		bp += trackp->isecsize;
	}
}
//--------------------------------------------------------------------
void 	encsectors(track_t	*trackp, unsigned __s8	*bp,	__s32	address,	__s32	nsecs)
{
	__s32	sectype = trackp->sectype;

	if ((sectype & ST_MODE_MASK) == ST_MODE_AUDIO)
		return;
}
//--------------------------------------------------------------------
void	packtext(__s32 tracks, track_t *trackp)
{
	__s32	type;
	__s32	i;
	struct textpack *tp;
	struct textsizes tsize;
	txtarg_t targ;
	__s8	sbuf[256*18];

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
		__s32	maxtrk;
		__s8	*s;

		if (!anytext(type, tracks, trackp))
			continue;
		maxtrk = tsize.last_track;
		if (type == 6) {
			maxtrk = 0;
		}
		for (i = 0; i <= maxtrk; i++) {
			s = (__s8*)trackp[i].text;
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
		fillpacks(&targ, &((__s8 *)(&tsize))[i*12], 12, i, 0x8f);
	}

	setuptextdata((unsigned __s8 *)sbuf, targ.seqno*18);
}
//--------------------------------------------------------------------
__s32		write_cdtext(St_GrScsiIf *scgp, cdr_t *dp, long startsec)
{
	__s8	*bp = (__s8 *)textsub;
	__s32	buflen = textlen;
	long	amount;
	long	bytes = 0;
	long	end = -150;
	__s32	secspt = textlen / 96;
	__s32	bytespt = textlen;
	long	maxdma = scgp->maxbuf;
	__s32	idx;
	__s32	secs;
	__s32	nbytes;

	if (textlen <= 0)
		return (-1);

	if (maxdma >= (2*textlen)) {

		bp = (__s8 *)scgp->bufptr;
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

			amount = write_secs(scgp, dp,
				(__s8 *)&bp[idx], startsec, nbytes, secs, FALSE);
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
void	setuptextdata(unsigned __s8 *bp, __s32 len)
{
	__s32	n;
	__s32	i;
	__s32	j;
	unsigned __s8	*p;

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
	p = (unsigned __s8*)malloc(n);
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
BOOL8	cdtext_crc_ok(struct textpack *p)
{
	__s32		crc;
	struct textpack	new1;

	//movebytes(p, &new1, sizeof (struct textpack));
	GrDumyCopyMem( &new1, p, sizeof (struct textpack));
	new1.crc[0] ^= 0xFF;
	new1.crc[1] ^= 0xFF;
	crc = calcCRC((unsigned __s8 *)&new1, sizeof (struct textpack));
	crc = flip_crc_error_corr((unsigned __s8 *)&new1, sizeof (struct textpack), crc);
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
BOOL8	anytext(__s32 pack_type, __s32 tracks, track_t *trackp)
{
	__s32	i;
	__s8	*p;

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
		fillcrc((unsigned __s8 *)ap->tp, sizeof (*ap->tp));
		ap->p  = 0;
		ap->tp++;
	}
}
//--------------------------------------------------------------------
void	fillpacks(txtarg_t *ap, __s8 *from, __s32 len, __s32 track_no, __s32 pack_type)
{
	__s32		charpos;
	__s8		*p;
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
			fillcrc((unsigned __s8 *)tp, sizeof (*tp));
			p = 0;
			tp++;
		}
	} while (len > 0);

	ap->tp = tp;
	ap->p = p;
}
//--------------------------------------------------------------------
void	eight2six(unsigned __s8 *in, unsigned __s8 *out)
{
	__s32	c;

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
__s32	flush()
{
	return (fflush(stdout));
}
*/
//--------------------------------------------------------------------
__s32		streql(__s8	*a, __s8	*b)
{
	__s8	*s1 = a;
	__s8	*s2 = b;

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
void	filldsubq(unsigned __s8 *sub, __s32 ca, __s32 t, __s32 i, msf_t *mrp, msf_t *mp)
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
void	fillmcn(unsigned __s8 *sub, unsigned __s8 *mcn)
{
	__s32	i;
	__s32	c;

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
void	fillisrc(unsigned __s8 *sub, unsigned __s8 *isrc)
{
	__s32	i;
	__s32	j;
	unsigned __s8		tmp[13];
	unsigned __s8		*sp;

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
__s32		ascii2q(__s32 c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= '@' && c <= 'o')
		return (0x10 + c - '@');
	return (0);
}
//--------------------------------------------------------------------
void	qpto16(unsigned __s8 *sub, unsigned __s8	*subqptr,	__s32	dop)
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
__s32		cmpnullbytes(void	*fromp, __s32 cnt)
{
	register const __s8	*from	= (__s8 *)fromp;
	register __s32		n;

	if ((n = cnt) <= 0)
		return (cnt);

	while (--n >= 0 && !laligned(from)) {
		if (*from++ != 0)
			goto cdiff;
	}
	n++;

	if (n >= (__s32)(8 * sizeof (long))) {
		if (laligned(from)) {
			register const long *froml = (const long *)from;
			register __s32 rem = n % (8 * sizeof (long));

			n /= (8 * sizeof (long));
			do {
				DO8(
					if (*froml++ != 0)
						break;
				);
			} while (--n > 0);

			if (n > 0) {
				--froml;
				from = (const __s8 *)froml;
				goto ldiff;
			}
			from = (const __s8 *)froml;
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
	return (--from - (__s8 *)fromp);
}
//--------------------------------------------------------------------
__s8*	astoll(__s8 *s,	__u64 *l)
{
	return (astollb(s, l, 0));
}
//--------------------------------------------------------------------
__s8*	astollb(__s8 *s, __u64 *l, __s32 base)
{
	__s32 neg = 0;
	__u64 ret = (__u64)0;
	__s32 digit;
	__s8 c;

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
	return ((__s8 *)s);
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
void	raisecond(__s8 *signame, long arg2)
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
void	raiseabort(__s8	*signame)
{
		DbgMsgPrint( "ODD - try abort\n" );
		//abort();
}
//--------------------------------------------------------------------
BOOL8	framehandle(SIGBLK *sp, __s8	*handlename, __s8	*signame,	long arg2)
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
__s32	_io_get_my_flag(FILE	*fp)
{
	__s32	f = fileno(fp);
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
__s32 _more_flags(FILE *fp)
{
	__s32	f = fileno(fp);
	__s32	n = _fl_max;
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
ssize_t	_niread(__s32	f, void	*buf,	size_t count)
{
	ssize_t	ret;
	__s32	oerrno = errno;

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
FILE*	_fcons(FILE	*fd, __s32 f,	__s32	flag)
{
	__s32	my_gflag = _io_glflag;

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
void	_io_set_my_flag(FILE *fp,	__s32	flag)
{
	__s32	f = fileno(fp);
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
__s32 _openfd(__s8 *name,	__s32 omode)
{
	return (open(name, omode, (mode_t)0666));
}
//--------------------------------------------------------------------
__s32	_cvmod(__s8	*mode, __s32 *omode, __s32 *flag)
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
__s32	fgetline(FILE	*f, __s8 *buf, __s32 len)
{
	__s32	c	= '\0';
	__s8	*bp	= buf;
	__s32	nl	= '\n';

	down2(f, _IOREAD, _IORW);

	for (;;) {
		if ((c = getc(f)) < 0)
			break;
		if (c == nl)
			break;
		if (--len > 0) {
			*bp++ = (__s8)c;
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
FILE*	fileluopen(__s32 f, __s8 *mode)
{
	__s32	omode = 0;
	__s32	flag = 0;

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
__u32 disk_rcap(msf_t	*mp, __u32	maxblock,	BOOL8 rw, BOOL8	audio)
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
__s32		encspeed()
{
	track_t	t[1];
	unsigned __s8	sect[2352];
	__s32	i;
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
__u64	isosize(__s32	f, off_t* A_PtrOffSet)
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
void	swabbytes(void *vp, __s32	cnt)
{
	__s8	*bp = (__s8 *)vp;
	__s8	c;

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
void	scatter_secs(track_t *trackp,	__s8 *bp,	__s32	nsecs)
{
	__s8	*from;
	__s8	*to;
	__s32	isecsize = trackp->isecsize;
	__s32	secsize = trackp->secsize;
	__s32	i;

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
void	scrsectors(track_t *trackp,	unsigned __s8	*bp, __s32 address,	__s32	nsecs)
{
	while (--nsecs >= 0)
	{
		if (IS_SECHDR(bp))
			scramble_L2(bp);
		bp += trackp->secsize;
	}
}
//--------------------------------------------------------------------
__s32		scramble_L2(unsigned __s8 *inout)
{
	if (!xaligned(inout + 12, sizeof(__u32)-1)) {

		unsigned __s8		*r = inout + 12;
		unsigned __s8	*s = yellowbook_scrambler;
		__s32	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof(unsigned __s8)/4; --i >= 0;) {
			DO4(*r++ ^= *s++);
		}

	} else {
		__u32	*r = (__u32 *) (inout + 12);
		const __u32 *s = yellowbook_scrambler_DWORD;
		__s32	i;

		for (i = (L2_RAW + L2_Q + L2_P +16)/sizeof(__u32)/13; --i >= 0;) {
			DO13(*r++ ^= *s++);
		}
	}

	return (0);
}
//--------------------------------------------------------------------
