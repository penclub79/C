/*
	scsi opr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrScsiOpr
#define	_EPC_GrScsiOpr

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrSCSI.h>
#include 	<Hw/Odd/GrOddRec.h>
#include 	<stdio.h>
#include 	<stdarg.h> 
#include 	<stdlib.h>

//====================================================================
//constance
#define	roundup(x, y)   ((((x)+((y)-1))/(y))*(y))

//cdtext
#define	tc_title	textcodes[0x00]
#define	tc_performer	textcodes[0x01]
#define	tc_songwriter	textcodes[0x02]
#define	tc_composer	textcodes[0x03]
#define	tc_arranger	textcodes[0x04]
#define	tc_message	textcodes[0x05]
#define	tc_diskid	textcodes[0x06]
#define	tc_genre	textcodes[0x07]
#define	tc_toc		textcodes[0x08]
#define	tc_toc2		textcodes[0x09]

#define	tc_closed_info	textcodes[0x0d]
#define	tc_isrc		textcodes[0x0e]

#define	GENRE_UNUSED		0	/* not used			    */
#define	GENRE_UNDEFINED		1	/* not defined			    */
#define	GENRE_ADULT_CONTEMP	2	/* Adult Contemporary		    */
#define	GENRE_ALT_ROCK		3	/* Alternative Rock		    */
#define	GENRE_CHILDRENS		4	/* Childrens Music		    */
#define	GENRE_CLASSIC		5	/* Classical			    */
#define	GENRE_CHRIST_CONTEMP	6	/* Contemporary Christian	    */
#define	GENRE_COUNTRY		7	/* Country			    */
#define	GENRE_DANCE		8	/* Dance			    */
#define	GENRE_EASY_LISTENING	9	/* Easy Listening		    */
#define	GENRE_EROTIC		10	/* Erotic			    */
#define	GENRE_FOLK		11	/* Folk				    */
#define	GENRE_GOSPEL		12	/* Gospel			    */
#define	GENRE_HIPHOP		13	/* Hip Hop			    */
#define	GENRE_JAZZ		14	/* Jazz				    */
#define	GENRE_LATIN		15	/* Latin			    */
#define	GENRE_MUSICAL		16	/* Musical			    */
#define	GENRE_NEWAGE		17	/* New Age			    */
#define	GENRE_OPERA		18	/* Opera			    */
#define	GENRE_OPERETTA		19	/* Operetta			    */
#define	GENRE_POP		20	/* Pop Music			    */
#define	GENRE_RAP		21	/* RAP				    */
#define	GENRE_REGGAE		22	/* Reggae			    */
#define	GENRE_ROCK		23	/* Rock Music			    */
#define	GENRE_RYTHMANDBLUES	24	/* Rhythm & Blues		    */
#define	GENRE_SOUNDEFFECTS	25	/* Sound Effects		    */
#define	GENRE_SPOKEN_WORD	26	/* Spoken Word			    */
#define	GENRE_WORLD_MUSIC	28	/* World Music			    */
#define	GENRE_RESERVED		29	/* Reserved is 29..32767	    */
#define	GENRE_RIAA		32768	/* Registration by RIAA 32768..65535 */
/*
 * Character codings used in CD-Text data.
 * Korean and Mandarin Chinese to be defined in sept 1996
 */
#define	CC_8859_1	0x00		/* ISO 8859-1			*/
#define	CC_ASCII	0x01		/* ISO 646, ASCII (7 bit)	*/
#define	CC_RESERVED_02	0x02		/* Reserved codes 0x02..0x7f	*/
#define	CC_KANJI	0x80		/* Music Shift-JIS Kanji	*/
#define	CC_KOREAN	0x81		/* Korean			*/
#define	CC_CHINESE	0x82		/* Mandarin Chinese		*/
#define	CC_RESERVED_83	0x83		/* Reserved codes 0x83..0xFF	*/

//schilyio
#define	down(f)
#define	down1(f, fl1)
#define	down2(f, fl1, fl2)


#define	DO_MYFLAG		/* use local flags */

#define	my_flag(p)	_io_get_my_flag(p)

#define	FI_NONE		0x0000	/* no flags defined */

#define	FI_READ		0x0001	/* open for reading */
#define	FI_WRITE	0x0002	/* open for writing */
#define	FI_BINARY	0x0004	/* open in binary mode */
#define	FI_APPEND	0x0008	/* append on each write */

#define	FI_CREATE	0x0010	/* create if nessecary */
#define	FI_TRUNC	0x0020	/* truncate file on open */
#define	FI_UNBUF	0x0080	/* dont't buffer io */
#define	FI_CLOSE	0x1000	/* close file on error */

#define	_JS_IONORAISE	01	/* do no raisecond() on errors */
#define	_JS_IOUNBUF	02	/* do unbuffered i/o */

#define	set_my_flag(p, v) _io_set_my_flag(p, v)
#define	add_my_flag(p, v) _io_add_my_flag(p, v)

//flag
#define	FL_INIT	10

//aligned
#define	ALIGN_SHORT	2	/* alignment value for (__s16 *)	*/
#define	ALIGN_SMASK	1	/* alignment mask  for (__s16 *)	*/
#define	SIZE_SHORT	2	/* sizeof (__s16)			*/

#define	ALIGN_INT	4	/* alignment value for (__s32 *)		*/
#define	ALIGN_IMASK	3	/* alignment mask  for (__s32 *)		*/
#define	SIZE_INT	4	/* sizeof (__s32)				*/

#define	ALIGN_LONG	4	/* alignment value for (long *)		*/
#define	ALIGN_LMASK	3	/* alignment mask  for (long *)		*/
#define	SIZE_LONG	4	/* sizeof (long)			*/

#define	ALIGN_LLONG	4	/* alignment value for (long long *)	*/
#define	ALIGN_LLMASK	3	/* alignment mask  for (long long *)	*/
#define	SIZE_LLONG	8	/* sizeof (long long)			*/

#define	ALIGN_FLOAT	4	/* alignment value for (float *)	*/
#define	ALIGN_FMASK	3	/* alignment mask  for (float *)	*/
#define	SIZE_FLOAT	4	/* sizeof (float)			*/

#define	ALIGN_DOUBLE	4	/* alignment value for (double *)	*/
#define	ALIGN_DMASK	3	/* alignment mask  for (double *)	*/
#define	SIZE_DOUBLE	8	/* sizeof (double)			*/

#define	ALIGN_PTR	4	/* alignment value for (pointer *)	*/
#define	ALIGN_PMASK	3	/* alignment mask  for (pointer *)	*/
#define	SIZE_PTR	4	/* sizeof (pointer)			*/

#define	xaligned(a, s)		((((__u32)(a)) & (s)) == 0 )
#define	x2aligned(a, b, s)	(((((__u32)(a)) | ((__u32)(b))) & (s)) == 0 )

#define	saligned(a)		xaligned(a, ALIGN_SMASK)
#define	s2aligned(a, b)		x2aligned(a, b, ALIGN_SMASK)

#define	ialigned(a)		xaligned(a, ALIGN_IMASK)
#define	i2aligned(a, b)		x2aligned(a, b, ALIGN_IMASK)

#define	laligned(a)		xaligned(a, ALIGN_LMASK)
#define	l2aligned(a, b)		x2aligned(a, b, ALIGN_LMASK)

#define	llaligned(a)		xaligned(a, ALIGN_LLMASK)
#define	ll2aligned(a, b)	x2aligned(a, b, ALIGN_LLMASK)

#define	faligned(a)		xaligned(a, ALIGN_FMASK)
#define	f2aligned(a, b)		x2aligned(a, b, ALIGN_FMASK)

#define	daligned(a)		xaligned(a, ALIGN_DMASK)
#define	d2aligned(a, b)		x2aligned(a, b, ALIGN_DMASK)

#define	paligned(a)		xaligned(a, ALIGN_PMASK)
#define	p2aligned(a, b)		x2aligned(a, b, ALIGN_PMASK)

#define	xalign(x, a, m)		( ((__s8 *)(x)) + ( (a) - 1 - ((((UIntptr_t)(x))-1)&(m))) )

#define	salign(x)		xalign((x), ALIGN_SHORT, ALIGN_SMASK)
#define	ialign(x)		xalign((x), ALIGN_INT, ALIGN_IMASK)
#define	lalign(x)		xalign((x), ALIGN_LONG, ALIGN_LMASK)
#define	llalign(x)		xalign((x), ALIGN_LLONG, ALIGN_LLMASK)
#define	falign(x)		xalign((x), ALIGN_FLOAT, ALIGN_FMASK)
#define	dalign(x)		xalign((x), ALIGN_DOUBLE, ALIGN_DMASK)
#define	palign(x)		xalign((x), ALIGN_PTR, ALIGN_PMASK)

#	define	mmap_sizeparm(s)	(s)

//xio
#define	X_NOREWIND	0x01	/* Do not rewind() the file on xclose() */
#define	X_UFLAGS	0xFF	/* Mask for flags allowed with xopen() */
#define	X_NOSEEK	0x1000	/* Cannot seek on this fd */
#define	xfileno(p)	(((xio_t *)(p))->x_file)

//iso9660
#define	_delta(from, to)	((to) - (from) + 1)

#define	VD_BOOT		0
#define	VD_PRIMARY	1
#define	VD_SUPPLEMENT	2
#define	VD_PARTITION	3
#define	VD_TERM		255

#define	VD_ID		"CD001"

#define	GET_UBYTE(a)	a_to_u_byte(a)
#define	GET_SBYTE(a)	a_to_byte(a)
#define	GET_SHORT(a)	a_to_u_2_byte(&((unsigned __s8 *) (a))[0])
#define	GET_BSHORT(a)	a_to_u_2_byte(&((unsigned __s8 *) (a))[2])
#define	GET_INT(a)	a_to_4_byte(&((unsigned __s8 *) (a))[0])
#define	GET_LINT(a)	la_to_4_byte(&((unsigned __s8 *) (a))[0])
#define	GET_BINT(a)	a_to_4_byte(&((unsigned __s8 *) (a))[4])

#define	PERM_MB_ONE	0xAAAA

#define	PERM_RSYS	0x0001
#define	PERM_XSYS	0x0004

#define	PERM_RUSR	0x0010
#define	PERM_XUSR	0x0040

#define	PERM_RGRP	0x0100
#define	PERM_XGRP	0x0400

#define	PERM_ROTH	0x1000
#define	PERM_XOTH	0x4000

//auheader
#define	AU_BAD		-1
#define	AU_BAD_HEADER	-2
#define	AU_BAD_CODING	-3

//ecc
#define L2_RAW (1024*2)
#define L2_Q   (26*2*2)
#define L2_P   (43*2*2)

//fifo
#define	__vfork_resume()

//====================================================================
//global macro

//====================================================================
//global type
/*unsigned __s8	_subq[110][12];
__s32	_nsubh;
unsigned __s8	*textsub;
__s32	textlen;*/

//flag
struct _io_flags {
	FILE	*fl_io;		/* file pointer */
	struct _io_flags	/* pointer to next struct */
		*fl_next;	/* if more file pointer to same fd */
	__s32	fl_flags;	/* my flags */
};

typedef	struct _io_flags _io_fl; 

static __s32	_io_glflag;		/* global default flag */
static __s32	_fl_inc = 10;		/* increment for expanding flag struct */
static __s32	_fl_max = FL_INIT;	/* max fd currently in _io_myfl */
static _io_fl	_io_smyfl[FL_INIT];	/* initial static space */
static _io_fl	*_io_myfl = _io_smyfl;	/* init to static space */

static __s8	_badfile[]	= "bad_file";
static __s8	_badmode[]	= "bad_file_mode";
static __s8	_badop[]	= "bad_file_op";


//cdtext
typedef struct St_GrOddTextcodes 
{
	__s8	*textcodes[16];
} textptr_t, *Ptr_GrOddTextcodes;

//sigblk
typedef struct sigblk {
	long		**sb_savfp;	/* Real saved framepointer	*/
	struct sigblk	*sb_signext;	/* Next sw signal blk for this func */
	__s16		sb_siglen;	/* Strlen for sb_signame	*/
	__s8	*sb_signame;	/* Name of software signal	*/

					/* sb_sigfun: function to call	*/
	__s32		(*sb_sigfun)	(const __s8 *, long, long);

	long		sb_sigarg;	/* Second arg for sb_sigfun	*/
} SIGBLK;

static SIGBLK*	__roothandle;

static	__s8	_readerr[]	= "file_read_err";

//iso9660
struct	iso9660_voldesc {
	__s8	vd_type		[_delta(1, 1)];
	__s8	vd_id		[_delta(2, 6)];
	__s8	vd_version	[_delta(7, 7)];
	__s8	vd_fill		[_delta(8, 2048)];
};

struct	iso9660_boot_voldesc {
	__s8	vd_type		[_delta(1, 1)];
	__s8	vd_id		[_delta(2, 6)];
	__s8	vd_version	[_delta(7, 7)];
	__s8	vd_bootsys	[_delta(8, 39)];
	__s8	vd_bootid	[_delta(40, 71)];
	__s8	vd_bootcode	[_delta(72, 2048)];
};

struct	iso9660_pr_voldesc {
	__s8	vd_type			[_delta(1,	1)];
	__s8	vd_id			[_delta(2,	6)];
	__s8	vd_version		[_delta(7,	7)];
	__s8	vd_unused1		[_delta(8,	8)];
	__s8	vd_system_id		[_delta(9,	40)];
	__s8	vd_volume_id		[_delta(41,	72)];
	__s8	vd_unused2		[_delta(73,	80)];
	__s8	vd_volume_space_size	[_delta(81,	88)];
	__s8	vd_unused3		[_delta(89,	120)];
	__s8	vd_volume_set_size	[_delta(121,	124)];
	__s8	vd_volume_seq_number	[_delta(125,	128)];
	__s8	vd_lbsize		[_delta(129,	132)];
	__s8	vd_path_table_size	[_delta(133,	140)];
	__s8	vd_pos_path_table_l	[_delta(141,	144)];
	__s8	vd_opt_pos_path_table_l	[_delta(145,	148)];
	__s8	vd_pos_path_table_m	[_delta(149,	152)];
	__s8	vd_opt_pos_path_table_m	[_delta(153,	156)];
	__s8	vd_root_dir		[_delta(157,	190)];
	__s8	vd_volume_set_id	[_delta(191,	318)];
	__s8	vd_publisher_id		[_delta(319,	446)];
	__s8	vd_data_preparer_id	[_delta(447,	574)];
	__s8	vd_application_id	[_delta(575,	702)];
	__s8	vd_copyr_file_id	[_delta(703,	739)];
	__s8	vd_abstr_file_id	[_delta(740,	776)];
	__s8	vd_bibl_file_id		[_delta(777,	813)];
	__s8	vd_create_time		[_delta(814,	830)];
	__s8	vd_mod_time		[_delta(831,	847)];
	__s8	vd_expiry_time		[_delta(848,	864)];
	__s8	vd_effective_time	[_delta(865,	881)];
	__s8	vd_file_struct_vers	[_delta(882,	882)];
	__s8	vd_reserved1		[_delta(883,	883)];
	__s8	vd_application_use	[_delta(884,	1395)];
	__s8	vd_fill			[_delta(1396,	2048)];
};

struct	iso9660_dir {
	__s8	dr_len			[_delta(1,	1)];
	__s8	dr_eattr_len		[_delta(2,	2)];
	__s8	dr_eattr_pos		[_delta(3,	10)];
	__s8	dr_data_len		[_delta(11,	18)];
	__s8	dr_recording_time	[_delta(19,	25)];
	__s8	dr_file_flags		[_delta(26,	26)];
	__s8	dr_file_unit_size	[_delta(27,	27)];
	__s8	dr_interleave_gap	[_delta(28,	28)];
	__s8	dr_volume_seq_number	[_delta(29,	32)];
	__s8	dr_file_name_len	[_delta(33,	33)];
	__s8	dr_file_name		[_delta(34,	34)];
};

struct	iso9660_dtime {
	unsigned __s8	dt_year;
	unsigned __s8	dt_month;
	unsigned __s8	dt_day;
	unsigned __s8	dt_hour;
	unsigned __s8	dt_minute;
	unsigned __s8	dt_second;
		__s8	dt_gmtoff;
};

struct	iso9660_ltime {
	__s8	lt_year			[_delta(1,	4)];
	__s8	lt_month		[_delta(5,	6)];
	__s8	lt_day			[_delta(7,	8)];
	__s8	lt_hour			[_delta(9,	10)];
	__s8	lt_minute		[_delta(11,	12)];
	__s8	lt_second		[_delta(13,	14)];
	__s8	lt_hsecond		[_delta(15,	16)];
	__s8	lt_gmtoff		[_delta(17,	17)];
};

struct iso9660_path_table {
	__s8	pt_di_len		[_delta(1,	1)];
	__s8	pt_eattr_len		[_delta(2,	2)];
	__s8	pt_eattr_pos		[_delta(3,	6)];
	__s8	pt_di_parent		[_delta(7,	8)];
	__s8	pt_name			[_delta(9,	9)];
};

struct iso9660_eattr {
	__s8	ea_owner		[_delta(1,	4)];
	__s8	ea_group		[_delta(5,	8)];
	__s8	ea_perm			[_delta(9,	10)];
	__s8	ea_ctime		[_delta(11,	27)];
	__s8	ea_mtime		[_delta(28,	44)];
	__s8	ea_extime		[_delta(45,	61)];
	__s8	ea_eftime		[_delta(62,	78)];
	__s8	ea_record_format	[_delta(79,	79)];
	__s8	ea_record_attr		[_delta(80,	80)];
	__s8	ea_record_len		[_delta(81,	84)];
	__s8	ea_system_id		[_delta(85,	116)];
	__s8	ea_system_use		[_delta(117,	180)];
	__s8	ea_version		[_delta(181,	181)];
	__s8	ea_esc_seq_len		[_delta(182,	182)];
	__s8	ea_reserved1		[_delta(183,	246)];
	__s8	ea_appl_use_len		[_delta(247,	250)];
	__s8	ea_appl_use		[_delta(251,	251)];	/* actually more */
/*	__s8	ea_esc_seq		[_delta(xxx,	xxx)];	*/

};

typedef enum faio_owner {
	owner_none,		/* Unused in real life			    */
	owner_writer,		/* owned by process that writes into FIFO   */
	owner_faio,		/* Intermediate state when buf still in use */
	owner_reader		/* owned by process that reads from FIFO    */
} fowner_t;

typedef struct faio {
	__s32	len;
	volatile fowner_t owner;
	volatile __s32 users;
	__s16	fd;
	__s16	saved_errno;
	__s8	*bufp;
} faio_t;


//====================================================================
//class

//====================================================================
//global function
__s8*		astoi(const __s8 *s, __s32 *i);
__s8*		astol(const __s8 *s, long *l);
__s8*		astolb(const __s8 *s,	long *l, __s32 base);
void		timevaldiff(struct timeval *start, struct timeval	*stop);

//subchan
__s32			do_leadin(track_t *trackp);
__s32			write_leadin(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp, __s32 leadinstart);
__s32			write_leadout(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
void		fillsubch(track_t *trackp, unsigned __s8 *sp, __s32 secno, __s32 nsecs);
void		filltpoint(unsigned __s8 *sub, __s32 ctrl_adr, __s32 point, msf_t *mp);
void		fillttime(unsigned __s8 *sub, msf_t *mp);
void		qpto96(unsigned __s8 *sub, unsigned __s8 *subq, __s32 dop);
void		addrw(unsigned __s8 *sub, unsigned __s8	*subrwptr);
void		qwto16(unsigned __s8 *subq, unsigned __s8 *subptr);
void		subrecodesecs(track_t *trackp, unsigned __s8 *bp, __s32 address, __s32 nsecs);
void 		encsectors(track_t	*trackp, unsigned __s8	*bp,	__s32	address,	__s32	nsecs);

//cd_misc
__s32			from_bcd(__s32 b);
__s32			to_bcd(__s32 i);
long		msf_to_lba(__s32 m, __s32 s, __s32 f, BOOL8 force_positive);
BOOL8		lba_to_msf(long lba, msf_t *mp);
void		sec_to_msf(long sec, msf_t *mp);
void		print_min_atip(long li, long lo);
BOOL8		is_cdspeed(__s32 speed);

//crc16
__u16	calcCRC(unsigned __s8 *buf, __u32 bsize);
__u16	fillcrc(unsigned __s8 *buf, __u32 bsize);
__u16	flip_crc_error_corr(unsigned __s8 *b, __u32 bsize, __u32 p_crc);

FILE*		fileopen(__s8	*name, __s8	*mode);

//cdtext
void	packtext(__s32 tracks, track_t *trackp);
__s32		write_cdtext(St_GrScsiIf *scgp, cdr_t *dp, long startsec);

//streql
__s32		streql(__s8	*a, __s8	*b);

//cmpnullbytes
__s32		cmpnullbytes(void	*fromp, __s32 cnt);

//astoll
__s8*	astoll(__s8 *s,	__u64 *l);
__s8*	astollb(__s8 *s, __u64 *l, __s32 base);

//filesize
off_t	filesize(FILE	*f);

//raisecond
void	raisecond(__s8 *signame, long arg2);

//fileread
ssize_t	fileread(FILE	*f,	void *buf, size_t	len);

//flag
__s32	_io_get_my_flag(FILE	*fp);

//niread
ssize_t	_niread(__s32	f, void	*buf,	size_t count);

//fgetline
__s32	fgetline(FILE	*f, __s8 *buf, __s32 len);

//fileluopen
FILE*	fileluopen(__s32 f, __s8 *mode);

//diskid
__u32 disk_rcap(msf_t *mp, __u32 maxblock, BOOL8 rw, BOOL8 audio);

//sector
__s32		encspeed();
void	scrsectors(track_t *trackp,	unsigned __s8	*bp, __s32 address,	__s32	nsecs);

//isosize
__u64	isosize(__s32	f, off_t* A_PtrOffSet);

//swapbytes
void	swabbytes(void *vp, __s32	cnt);

//movesect
void	scatter_secs(track_t *trackp,	__s8 *bp,	__s32	nsecs);

//edcecc
__s32		scramble_L2(unsigned __s8 *inout);

//====================================================================
//out Enviroment conditional
#endif

