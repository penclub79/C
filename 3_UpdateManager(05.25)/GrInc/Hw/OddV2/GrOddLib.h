/*
	odd library
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddLib
#define _EPC_GrOddLib

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/OddV2/GrOddDef.h>

#include 	<stdio.h>
#include 	<stdarg.h> 
#include 	<stdlib.h>

//====================================================================
//constance
#define	DEV_UNKNOWN		0
#define	DEV_ACB40X0		1
#define	DEV_ACB4000		2
#define	DEV_ACB4010		3
#define	DEV_ACB4070		4
#define	DEV_ACB5500		5
#define	DEV_ACB4520A		6
#define	DEV_ACB4525		7
#define	DEV_MD21		8
#define	DEV_MD23		9
#define	DEV_NON_CCS_DSK		10
#define	DEV_CCS_GENDISK		11

#define	TRM_ZERO	0
#define	TRM_USER_ECC	1	/* 2048 bytes user data + 288 Bytes ECC/EDC */
#define	TRM_USER	2	/* All user data (2336 bytes) */

#define	strbeg(s1, s2)	(strstr((s2), (s1)) == (s2))

//scsi mmc
#define	CGMS_PERMITTED		0	/* Unlimited copy permitted	*/
#define	CGMS_RES		1	/* Reserved			*/
#define	CGMS_ONE_COPY		2	/* One copy permitted		*/
#define	CGMS_NO_COPY		3	/* No copy permitted		*/

#define	WRC_DEF_RC	0		/* Media default rotational control */
#define	WRC_CAV		1		/* CAV				    */

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
	((((unsigned char*) a)[3]		& 0xFF) | \
	(((unsigned char*) a)[2] << 8	& 0xFF00) | \
	(((unsigned char*) a)[1] << 16	& 0xFF0000) | \
	(((unsigned char*) a)[0] << 24	& __TOP_4BYTE)))
#define	a_to_4_byte(a)		(long)(__u32)a_to_u_4_byte(a)

#define	i_to_2_byte(a, i)	(((unsigned char *)(a))[0] = ((i) >> 8) & 0xFF,\
	((unsigned char *)(a))[1] = (i) & 0xFF)

#define	i_to_3_byte(a, i)	(((unsigned char *)(a))[0] = ((i) >> 16)& 0xFF,\
	((unsigned char *)(a))[1] = ((i) >> 8) & 0xFF,\
	((unsigned char *)(a))[2] = (i) & 0xFF)

#define	i_to_4_byte(a, i)	(((unsigned char *)(a))[0] = ((i) >> 24)& 0xFF,\
	((unsigned char *)(a))[1] = ((i) >> 16)& 0xFF,\
	((unsigned char *)(a))[2] = ((i) >> 8) & 0xFF,\
	((unsigned char *)(a))[3] = (i) & 0xFF)



#define	a_to_byte(a)		(((__u8 *) a)[0])

#define	a_to_u_byte(a)		((__u8) \
	(((unsigned char *) a)[0]		& 0xFF))

#define	a_to_u_2_byte(a)	((unsigned short) \
	((((unsigned char *) a)[1]		& 0xFF) | \
	(((unsigned char *) a)[0] << 8	& 0xFF00)))

#define	a_to_2_byte(a)		(int)(unsigned short)a_to_u_2_byte(a)

#define	a_to_u_3_byte(a)	((__u32) \
	((((unsigned char *) a)[2]		& 0xFF) | \
	(((unsigned char *) a)[1] << 8	& 0xFF00) | \
	(((unsigned char *) a)[0] << 16	& 0xFF0000)))

#define	MP_P_CODE			\
	unsigned char	p_code		: 6;	\
	unsigned char	p_res		: 1;	\
	unsigned char	parsave		: 1

#define	g1_cdblen(cdb, len)	((cdb)->count[0]    = ((len) >> 8) & 0xFF,\
	(cdb)->count[1] = (len) & 0xFF)

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
#define	ALIGN_SHORT	2	/* alignment value for (short *)	*/
#define	ALIGN_SMASK	1	/* alignment mask  for (short *)	*/
#define	SIZE_SHORT	2	/* sizeof (short)			*/

#define	ALIGN_INT	4	/* alignment value for (int *)		*/
#define	ALIGN_IMASK	3	/* alignment mask  for (int *)		*/
#define	SIZE_INT	4	/* sizeof (int)				*/

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

#define	xalign(x, a, m)		( ((char *)(x)) + ( (a) - 1 - ((((UIntptr_t)(x))-1)&(m))) )

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
#define	GET_SHORT(a)	a_to_u_2_byte(&((unsigned char *) (a))[0])
#define	GET_BSHORT(a)	a_to_u_2_byte(&((unsigned char *) (a))[2])
#define	GET_INT(a)	a_to_4_byte(&((unsigned char *) (a))[0])
#define	GET_LINT(a)	la_to_4_byte(&((unsigned char *) (a))[0])
#define	GET_BINT(a)	a_to_4_byte(&((unsigned char *) (a))[4])

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
typedef struct msf {
	char	msf_min;
	char	msf_sec;
	char	msf_frame;
} msf_t;

struct siheader {
	unsigned char	len[2];
	unsigned char	finished;
	unsigned char	unfinished;
};

struct sidesc {
	unsigned char	sess_number;
	unsigned char	res1;
	unsigned char	track;
	unsigned char	res3;
	unsigned char	addr[4];
};

struct sinfo {
	struct siheader	hd;
	struct sidesc	desc[1];
};

typedef struct opc {
	unsigned char	opc_speed[2];
	unsigned char	opc_val[6];
} opc_t;

struct disk_info {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	disk_status	: 2;	/* Status of the disk		*/
	unsigned char	sess_status	: 2;	/* Status of last session	*/
	unsigned char	erasable	: 1;	/* Disk is erasable		*/
	unsigned char	dtype		: 3;	/* Disk information data type	*/
	unsigned char	first_track;		/* # of first track on disk	*/
	unsigned char	numsess;		/* # of sessions		*/
	unsigned char	first_track_ls;		/* First track in last session	*/
	unsigned char	last_track_ls;		/* Last track in last session	*/
	unsigned char	bg_format_stat	: 2;	/* Background format status	*/
	unsigned char	dbit		: 1;	/* Dirty Bit of defect table	*/
	unsigned char	res7_3		: 1;	/* Reserved			*/
	unsigned char	dac_v		: 1;	/* Disk application code valid	*/
	unsigned char	uru		: 1;	/* This is an unrestricted disk	*/
	unsigned char	dbc_v		: 1;	/* Disk bar code valid		*/
	unsigned char	did_v		: 1;	/* Disk id valid		*/
	unsigned char	disk_type;		/* Disk type			*/
	unsigned char	numsess_msb;		/* # of sessions (MSB)		*/
	unsigned char	first_track_ls_msb;	/* First tr. in last ses. (MSB)	*/
	unsigned char	last_track_ls_msb;	/* Last tr. in last ses. (MSB)	*/
	unsigned char	disk_id[4];		/* Disk identification		*/
	unsigned char	last_lead_in[4];	/* Last session lead in time	*/
	unsigned char	last_lead_out[4];	/* Last session lead out time	*/
	unsigned char	disk_barcode[8];	/* Disk bar code		*/
	unsigned char	disk_appl_code;		/* Disk application code	*/
	unsigned char	num_opc_entries;	/* # of OPC table entries	*/
	opc_t	opc_table[1];		/* OPC table 			*/
};

struct mmc_cue {
	unsigned char	cs_ctladr;		/* CTL/ADR for this track	*/
	unsigned char	cs_tno;			/* This track number		*/
	unsigned char	cs_index;		/* Index within this track	*/
	unsigned char	cs_dataform;		/* Data form 			*/
	unsigned char	cs_scms;		/* Serial copy management	*/
	unsigned char	cs_min;			/* Absolute time minutes	*/
	unsigned char	cs_sec;			/* Absolute time seconds	*/
	unsigned char	cs_frame;		/* Absolute time frames		*/
};

struct tocheader {
	unsigned char	len[2];
	unsigned char	first;
	unsigned char	last;
};

struct ftrackdesc {
	unsigned char	sess_number;

	unsigned char	control		: 4;
	unsigned char	adr		: 4;

	unsigned char	track;
	unsigned char	point;
	unsigned char	amin;
	unsigned char	asec;
	unsigned char	aframe;
	unsigned char	res7;
	unsigned char	pmin;
	unsigned char	psec;
	unsigned char	pframe;
};

struct fdiskinfo {
	struct tocheader	hd;
	struct ftrackdesc	desc[1];
};

struct atipdesc {
	unsigned char	ref_speed	: 3;	/* Reference speed		*/
	unsigned char	res4_3		: 1;	/* Reserved			*/
	unsigned char	ind_wr_power	: 3;	/* Indicative tgt writing power	*/
	unsigned char	res4_7		: 1;	/* Reserved (must be "1")	*/
	unsigned char	res5_05		: 6;	/* Reserved			*/
	unsigned char	uru		: 1;	/* Disk is for unrestricted use	*/
	unsigned char	res5_7		: 1;	/* Reserved (must be "0")	*/
	unsigned char	a3_v		: 1;	/* A 3 Values valid		*/
	unsigned char	a2_v		: 1;	/* A 2 Values valid		*/
	unsigned char	a1_v		: 1;	/* A 1 Values valid		*/
	unsigned char	sub_type	: 3;	/* Disc sub type		*/
	unsigned char	erasable	: 1;	/* Disk is erasable		*/
	unsigned char	res6_7		: 1;	/* Reserved (must be "1")	*/
	unsigned char	lead_in[4];		/* Lead in time			*/
	unsigned char	lead_out[4];		/* Lead out time		*/
	unsigned char	res15;			/* Reserved			*/
	unsigned char	clv_high	: 4;	/* Highes usable CLV recording speed */
	unsigned char	clv_low		: 3;	/* Lowest usable CLV recording speed */
	unsigned char	res16_7		: 1;	/* Reserved (must be "0")	*/
	unsigned char	res17_0		: 1;	/* Reserved			*/
	unsigned char	tgt_y_pow	: 3;	/* Tgt y val of the power mod fun */
	unsigned char	power_mult	: 3;	/* Power multiplication factor	*/
	unsigned char	res17_7		: 1;	/* Reserved (must be "0")	*/
	unsigned char	res_18_30	: 4;	/* Reserved			*/
	unsigned char	rerase_pwr_ratio: 3;	/* Recommended erase/write power*/
	unsigned char	res18_7		: 1;	/* Reserved (must be "1")	*/
	unsigned char	res19;			/* Reserved			*/
	unsigned char	a2[3];			/* A 2 Values			*/
	unsigned char	res23;			/* Reserved			*/
	unsigned char	a3[3];			/* A 3 Vaules			*/
	unsigned char	res27;			/* Reserved			*/
};

struct atipinfo {
	struct tocheader	hd;
	struct atipdesc		desc;
};

struct track_info {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	track_number;		/* Track number for this info	*/
	unsigned char	session_number;		/* Session number for this info	*/
	unsigned char	res4;			/* Reserved			*/
	unsigned char	track_mode	: 4;	/* Track mode (Q-sub control)	*/
	unsigned char	copy		: 1;	/* This track is a higher copy	*/
	unsigned char	damage		: 1;	/* if 1 & nwa_valid 0: inc track*/
	unsigned char	res5_67		: 2;	/* Reserved			*/
	unsigned char	data_mode	: 4;	/* Data mode of this track	*/
	unsigned char	fp		: 1;	/* This is a fixed packet track	*/
	unsigned char	packet		: 1;	/* This track is in packet mode	*/
	unsigned char	blank		: 1;	/* This is an invisible track	*/
	unsigned char	rt		: 1;	/* This is a reserved track	*/
	unsigned char	nwa_valid	: 1;	/* Next writable addr valid	*/
	unsigned char	res7_17		: 7;	/* Reserved			*/
	unsigned char	track_start[4];		/* Track start address		*/
	unsigned char	next_writable_addr[4];	/* Next writable address	*/
	unsigned char	free_blocks[4];		/* Free usr blocks in this track*/
	unsigned char	packet_size[4];		/* Packet size if in fixed mode	*/
	unsigned char	track_size[4];		/* # of user data blocks in trk	*/
};

struct rzone_info {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	rzone_num_lsb;		/* RZone number LSB		*/
	unsigned char	border_num_lsb;		/* Border number LSB		*/
	unsigned char	res_4;			/* Reserved			*/
	unsigned char	trackmode	: 4;	/* Track mode			*/
	unsigned char	copy		: 1;	/* Higher generation CD copy	*/
	unsigned char	damage		: 1;	/* Damaged RZone		*/
	unsigned char	ljrs		: 2;	/* Layer jump recording status	*/
	unsigned char	datamode	: 4;	/* Data mode			*/
	unsigned char	fp		: 1;	/* Fixed packet			*/
	unsigned char	incremental	: 1;	/* RZone is to be written incremental */
	unsigned char	blank		: 1;	/* RZone is blank		*/
	unsigned char	rt		: 1;	/* RZone is reserved		*/
	unsigned char	nwa_v		: 1;	/* Next WR address is valid	*/
	unsigned char	lra_v		: 1;	/* Last rec address is valid	*/
	unsigned char	res7_27		: 6;	/* Reserved			*/
	unsigned char	rzone_start[4];		/* RZone start address		*/
	unsigned char	next_recordable_addr[4]; /* Next recordable address	*/
	unsigned char	free_blocks[4];		/* Free blocks in RZone		*/
	unsigned char	block_factor[4];	/* # of sectors of disc acc unit */
	unsigned char	rzone_size[4];		/* RZone size			*/
	unsigned char	last_recorded_addr[4];	/* Last Recorded addr in RZone	*/
	unsigned char	rzone_num_msb;		/* RZone number MSB		*/
	unsigned char	border_num_msb;		/* Border number MSB		*/
	unsigned char	res_34_35[2];		/* Reserved			*/
	unsigned char	read_compat_lba[4];	/* Read Compatibilty LBA	*/
	unsigned char	next_layer_jump[4];	/* Next layer jump address	*/
	unsigned char	last_layer_jump[4];	/* Last layer jump address	*/
};

struct dvd_structure_00 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	book_version	: 4;	/* DVD Book version		*/
	unsigned char	book_type	: 4;	/* DVD Book type		*/
	unsigned char	maximum_rate	: 4;	/* Maximum data rate (coded)	*/
	unsigned char	disc_size	: 4;	/* Disc size (coded)		*/
	unsigned char	layer_type	: 4;	/* Layer type			*/
	unsigned char	track_path	: 1;	/* 0 = parallel, 1 = opposit dir*/
	unsigned char	numlayers	: 2;	/* Number of Layers (0 == 1)	*/
	unsigned char	res2_7		: 1;	/* Reserved			*/
	unsigned char	track_density	: 4;	/* Track density (coded)	*/
	unsigned char	linear_density	: 4;	/* Linear data density (coded)	*/
	unsigned char	res8;			/* Reserved			*/
	unsigned char	phys_start[3];		/* Starting Physical sector #	*/
	unsigned char	res12;			/* Reserved			*/
	unsigned char	phys_end[3];		/* End physical data sector #	*/
	unsigned char	res16;			/* Reserved			*/
	unsigned char	end_layer0[3];		/* End sector # in layer	*/
	unsigned char	res20		: 7;	/* Reserved			*/
	unsigned char	bca		: 1;	/* BCA flag bit			*/
};

struct dvd_structure_01 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	copyr_prot_type;	/* Copyright prot system type	*/
	unsigned char	region_mgt_info;	/* Region management info	*/
	unsigned char	res67[2];		/* Reserved			*/
};

struct dvd_structure_02 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	key_data[2048];		/* Disc Key data		*/
};

struct dvd_structure_03 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	bca_info[1];		/* BCA information (12-188 bytes)*/
};

struct dvd_structure_04 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	man_info[2048];		/* Disc manufacturing info	*/
};

struct dvd_structure_05 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res4_03		: 4;	/* Reserved			*/
	unsigned char	cgms		: 2;	/* CGMS (see below)		*/
	unsigned char	res4_6		: 1;	/* Reserved			*/
	unsigned char	cpm		: 1;	/* This is copyrighted material	*/
	unsigned char	res57[3];		/* Reserved			*/
};

struct dvd_structure_0D {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	last_rma_sector[2];	/* Last recorded RMA sector #	*/
	unsigned char	rmd_bytes[1];		/* Content of Record man area	*/
};

struct dvd_structure_0E {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	field_id;		/* Field ID (1)			*/
	unsigned char	application_code;	/* Disc Application code	*/
	unsigned char	phys_data;		/* Disc Phisical Data		*/
	unsigned char	last_recordable_addr[3]; /* Last addr of recordable area */
	unsigned char	res_a[2];		/* Reserved			*/
	unsigned char	field_id_2;		/* Field ID (2)			*/
	unsigned char	ind_wr_power;		/* Recommended writing power	*/
	unsigned char	ind_wavelength;		/* Wavelength for ind_wr_power	*/
	unsigned char	opt_wr_strategy[4];	/* Optimum write Strategy	*/
	unsigned char	res_b[1];		/* Reserved			*/
	unsigned char	field_id_3;		/* Field ID (3)			*/
	unsigned char	man_id[6];		/* Manufacturer ID		*/
	unsigned char	res_m1;			/* Reserved			*/
	unsigned char	field_id_4;		/* Field ID (4)			*/
	unsigned char	man_id2[6];		/* Manufacturer ID		*/
	unsigned char	res_m2;			/* Reserved			*/
};

struct dvd_structure_0F {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res45[2];		/* Reserved			*/
	unsigned char	random[2];		/* Random number		*/
	unsigned char	year[4];		/* Year (ascii)			*/
	unsigned char	month[2];		/* Month (ascii)		*/
	unsigned char	day[2];			/* Day (ascii)			*/
	unsigned char	hour[2];		/* Hour (ascii)			*/
	unsigned char	minute[2];		/* Minute (ascii)		*/
	unsigned char	second[2];		/* Second (ascii)		*/
};

struct dvd_structure_0F_w {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res45[2];		/* Reserved			*/
	unsigned char	year[4];		/* Year (ascii)			*/
	unsigned char	month[2];		/* Month (ascii)		*/
	unsigned char	day[2];			/* Day (ascii)			*/
	unsigned char	hour[2];		/* Hour (ascii)			*/
	unsigned char	minute[2];		/* Minute (ascii)		*/
	unsigned char	second[2];		/* Second (ascii)		*/
};

struct dvd_structure_20 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res47[4];		/* Reserved			*/
	unsigned char	l0_area_cap[4];		/* Layer 0 area capacity	*/
};

struct dvd_structure_22 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res47[4];		/* Reserved			*/
	unsigned char	jump_interval_size[4];	/* Jump interval size		*/
};

struct dvd_structure_23 {
	unsigned char	data_len[2];		/* Data len without this info	*/
	unsigned char	res23[2];		/* Reserved			*/
	unsigned char	res47[4];		/* Reserved			*/
	unsigned char	jump_lba[4];		/* Jump logical block address	*/
};

struct mmc_performance_header {
	unsigned char	p_datalen[4];		/* Performance Data length	*/
	unsigned char	p_exept		:1;	/* Nominal vs. Exept. conditions*/
	unsigned char	p_write		:1;	/* Write vs. Read performance	*/
	unsigned char	p_res_4		:6;	/* Reserved bits...		*/
	unsigned char	p_res[3];		/* Reserved bytes		*/
};

struct mmc_performance {		/* Type == 00 (nominal)		*/
	unsigned char	start_lba[4];		/* Starting LBA			*/
	unsigned char	start_perf[4];		/* Start Performance		*/
	unsigned char	end_lba[4];		/* Ending LBA			*/
	unsigned char	end_perf[4];		/* Ending Performance		*/
};

struct mmc_exceptions {			/* Type == 00 (exceptions)	*/
	unsigned char	lba[4];			/* LBA				*/
	unsigned char	time[2];		/* Time				*/
};

struct mmc_write_speed {		/* Type == 00 (write speed)	*/
	unsigned char	p_mrw		:1;	/* Suitable for mixed read/write*/
	unsigned char	p_exact		:1;	/* Speed count for whole media	*/
	unsigned char	p_rdd		:1;	/* Media rotational control	*/
	unsigned char	p_wrc		:2;	/* Write rotational control	*/
	unsigned char	p_res		:3;	/* Reserved bits...		*/
	unsigned char	res[3];			/* Reserved Bytes		*/
	unsigned char	end_lba[4];		/* Ending LBA			*/
	unsigned char	read_speed[4];		/* Read Speed			*/
	unsigned char	write_speed[4];		/* Write Speed			*/
};

struct mmc_streaming {			/* Performance for set streaming*/
	unsigned char	p_ra		:1;	/* Random Acess			*/
	unsigned char	p_exact		:1;	/* Set values exactly		*/
	unsigned char	p_rdd		:1;	/* Restore unit defaults	*/
	unsigned char	p_wrc		:2;	/* Write rotational control	*/
	unsigned char	p_res		:3;	/* Reserved bits...		*/
	unsigned char	res[3];			/* Reserved Bytes		*/
	unsigned char	start_lba[4];		/* Starting LBA			*/
	unsigned char	end_lba[4];		/* Ending LBA			*/
	unsigned char	read_size[4];		/* Read Size			*/
	unsigned char	read_time[4];		/* Read Time			*/
	unsigned char	write_size[4];		/* Write Size			*/
	unsigned char	write_time[4];		/* Write Time			*/
};

//flag
struct _io_flags {
	FILE	*fl_io;		/* file pointer */
	struct _io_flags	/* pointer to next struct */
		*fl_next;	/* if more file pointer to same fd */
	int	fl_flags;	/* my flags */
};

typedef	struct _io_flags _io_fl; 

static int	_io_glflag;		/* global default flag */
static int	_fl_inc = 10;		/* increment for expanding flag struct */
static int	_fl_max = FL_INIT;	/* max fd currently in _io_myfl */
static _io_fl	_io_smyfl[FL_INIT];	/* initial static space */
static _io_fl	*_io_myfl = _io_smyfl;	/* init to static space */

static char	_badfile[]	= "bad_file";
static char	_badmode[]	= "bad_file_mode";
static char	_badop[]	= "bad_file_op";


//cdtext
typedef struct St_GrOddTextcodes 
{
	char	*textcodes[16];
} textptr_t, *Ptr_GrOddTextcodes;

//sigblk
typedef struct sigblk {
	long		**sb_savfp;	/* Real saved framepointer	*/
	struct sigblk	*sb_signext;	/* Next sw signal blk for this func */
	short		sb_siglen;	/* Strlen for sb_signame	*/
	char	*sb_signame;	/* Name of software signal	*/

	/* sb_sigfun: function to call	*/
	int		(*sb_sigfun)	(const char *, long, long);

	long		sb_sigarg;	/* Second arg for sb_sigfun	*/
} SIGBLK;

static SIGBLK*	__roothandle;

static	char	_readerr[]	= "file_read_err";

//iso9660
struct	iso9660_voldesc {
	char	vd_type		[_delta(1, 1)];
	char	vd_id		[_delta(2, 6)];
	char	vd_version	[_delta(7, 7)];
	char	vd_fill		[_delta(8, 2048)];
};

struct	iso9660_boot_voldesc {
	char	vd_type		[_delta(1, 1)];
	char	vd_id		[_delta(2, 6)];
	char	vd_version	[_delta(7, 7)];
	char	vd_bootsys	[_delta(8, 39)];
	char	vd_bootid	[_delta(40, 71)];
	char	vd_bootcode	[_delta(72, 2048)];
};

struct	iso9660_pr_voldesc {
	char	vd_type			[_delta(1,	1)];
	char	vd_id			[_delta(2,	6)];
	char	vd_version		[_delta(7,	7)];
	char	vd_unused1		[_delta(8,	8)];
	char	vd_system_id		[_delta(9,	40)];
	char	vd_volume_id		[_delta(41,	72)];
	char	vd_unused2		[_delta(73,	80)];
	char	vd_volume_space_size	[_delta(81,	88)];
	char	vd_unused3		[_delta(89,	120)];
	char	vd_volume_set_size	[_delta(121,	124)];
	char	vd_volume_seq_number	[_delta(125,	128)];
	char	vd_lbsize		[_delta(129,	132)];
	char	vd_path_table_size	[_delta(133,	140)];
	char	vd_pos_path_table_l	[_delta(141,	144)];
	char	vd_opt_pos_path_table_l	[_delta(145,	148)];
	char	vd_pos_path_table_m	[_delta(149,	152)];
	char	vd_opt_pos_path_table_m	[_delta(153,	156)];
	char	vd_root_dir		[_delta(157,	190)];
	char	vd_volume_set_id	[_delta(191,	318)];
	char	vd_publisher_id		[_delta(319,	446)];
	char	vd_data_preparer_id	[_delta(447,	574)];
	char	vd_application_id	[_delta(575,	702)];
	char	vd_copyr_file_id	[_delta(703,	739)];
	char	vd_abstr_file_id	[_delta(740,	776)];
	char	vd_bibl_file_id		[_delta(777,	813)];
	char	vd_create_time		[_delta(814,	830)];
	char	vd_mod_time		[_delta(831,	847)];
	char	vd_expiry_time		[_delta(848,	864)];
	char	vd_effective_time	[_delta(865,	881)];
	char	vd_file_struct_vers	[_delta(882,	882)];
	char	vd_reserved1		[_delta(883,	883)];
	char	vd_application_use	[_delta(884,	1395)];
	char	vd_fill			[_delta(1396,	2048)];
};

struct	iso9660_dir {
	char	dr_len			[_delta(1,	1)];
	char	dr_eattr_len		[_delta(2,	2)];
	char	dr_eattr_pos		[_delta(3,	10)];
	char	dr_data_len		[_delta(11,	18)];
	char	dr_recording_time	[_delta(19,	25)];
	char	dr_file_flags		[_delta(26,	26)];
	char	dr_file_unit_size	[_delta(27,	27)];
	char	dr_interleave_gap	[_delta(28,	28)];
	char	dr_volume_seq_number	[_delta(29,	32)];
	char	dr_file_name_len	[_delta(33,	33)];
	char	dr_file_name		[_delta(34,	34)];
};

struct	iso9660_dtime {
	unsigned char	dt_year;
	unsigned char	dt_month;
	unsigned char	dt_day;
	unsigned char	dt_hour;
	unsigned char	dt_minute;
	unsigned char	dt_second;
	char	dt_gmtoff;
};

struct	iso9660_ltime {
	char	lt_year			[_delta(1,	4)];
	char	lt_month		[_delta(5,	6)];
	char	lt_day			[_delta(7,	8)];
	char	lt_hour			[_delta(9,	10)];
	char	lt_minute		[_delta(11,	12)];
	char	lt_second		[_delta(13,	14)];
	char	lt_hsecond		[_delta(15,	16)];
	char	lt_gmtoff		[_delta(17,	17)];
};

struct iso9660_path_table {
	char	pt_di_len		[_delta(1,	1)];
	char	pt_eattr_len		[_delta(2,	2)];
	char	pt_eattr_pos		[_delta(3,	6)];
	char	pt_di_parent		[_delta(7,	8)];
	char	pt_name			[_delta(9,	9)];
};

struct iso9660_eattr {
	char	ea_owner		[_delta(1,	4)];
	char	ea_group		[_delta(5,	8)];
	char	ea_perm			[_delta(9,	10)];
	char	ea_ctime		[_delta(11,	27)];
	char	ea_mtime		[_delta(28,	44)];
	char	ea_extime		[_delta(45,	61)];
	char	ea_eftime		[_delta(62,	78)];
	char	ea_record_format	[_delta(79,	79)];
	char	ea_record_attr		[_delta(80,	80)];
	char	ea_record_len		[_delta(81,	84)];
	char	ea_system_id		[_delta(85,	116)];
	char	ea_system_use		[_delta(117,	180)];
	char	ea_version		[_delta(181,	181)];
	char	ea_esc_seq_len		[_delta(182,	182)];
	char	ea_reserved1		[_delta(183,	246)];
	char	ea_appl_use_len		[_delta(247,	250)];
	char	ea_appl_use		[_delta(251,	251)];	/* actually more */
	/*	char	ea_esc_seq		[_delta(xxx,	xxx)];	*/

};

typedef enum faio_owner {
	owner_none,		/* Unused in real life			    */
	owner_writer,		/* owned by process that writes into FIFO   */
	owner_faio,		/* Intermediate state when buf still in use */
	owner_reader		/* owned by process that reads from FIFO    */
} fowner_t;

typedef struct faio {
	int	len;
	volatile fowner_t owner;
	volatile int users;
	short	fd;
	short	saved_errno;
	char	*bufp;
} faio_t;

static int	toc2sess[] = {
	SES_DA_ROM,	/* CD-DA		 */
	SES_DA_ROM,	/* CD-ROM		 */
	SES_XA,		/* CD-ROM XA mode 1	 */
	SES_XA,		/* CD-ROM XA MODE 2	 */
	SES_CDI,	/* CDI			 */
	SES_DA_ROM,	/* Invalid - use default */
	SES_DA_ROM,	/* Invalid - use default */
};

static int	st2mode[] = {
	0,		/* 0			*/
	TM_DATA,	/* 1 ST_ROM_MODE1	*/
	TM_DATA,	/* 2 ST_ROM_MODE2	*/
	0,		/* 3			*/
	0,		/* 4 ST_AUDIO_NOPRE	*/
	TM_PREEM,	/* 5 ST_AUDIO_PRE	*/
	0,		/* 6			*/
	0,		/* 7			*/
};

static struct exargs
{
	void*	scgp;		/* The open SCSI * pointer	    */
	void*	dp;		/* The pointer to the device driver */
	int		old_secsize;
	__u32	flags;
	int		exflags;
} exargs;

//====================================================================
//class

//====================================================================
//global function
BOOL	unit_ready(void* A_ObjScg);
BOOL	wait_unit_ready(void* A_PtrScg, int secs);
int		test_unit_ready(void* A_PtrScg);
int		scsi_start_stop_unit(void* A_PtrScg, int, int, BOOL immed);
int		scsi_close_tr_session(void* A_PtrScg, int type, int track, BOOL immed);
int		scsi_flush_cache(void* A_PtrScg, BOOL immed);
int		rezero_unit(void* A_PtrScg);
int		request_sense(void* A_PtrScg);
int		request_sense_b(void* A_PtrScg, caddr_t bp, int cnt);
BOOL	wait_unit_ready(void* A_PtrScg, int secs);

int		read_rzone_info(void* A_PtrScg,	caddr_t	bp,	int	cnt);
int		send_dvd_structure(void* A_PtrScg, caddr_t bp, int cnt, int fmt);

int		scsi_prevent_removal(void* A_PtrScg, int);
int		read_dvd_structure(void* A_PtrScg, caddr_t bp, int cnt, int mt, int addr, int layer, int fmt);

BOOL	getdev(void* A_PtrScg, BOOL);
int		inquiry(void* A_PtrScg, caddr_t	bp, int	cnt);
BOOL	do_inquiry(void* A_PtrScg, int	print);
BOOL	is_unknown_dev(void* A_PtrScg);
int		scsi_load(void* A_PtrScg,	void* A_PtrDev);
int		scsi_unload(void* A_PtrScg,	void* A_PtrDev);
int		read_buff_cap(void* A_PtrScg,	long *sp,	long *fp);
int		select_secsize(void* A_PtrScg, int	secsize);
int		scsi_cdr_write(void* A_PtrScg, caddr_t bp, long sectaddr, long size, int	blocks, BOOL islast);
int		write_xg0(void* A_PtrScg,	caddr_t	bp,	long addr, long	size, int	cnt);
int		write_xg1(void* A_PtrScg,	caddr_t	bp,	long addr, long	size, int	cnt);
int		read_session_offset(void* A_PtrScg, long	*offp);
int		get_trackinfo(void* A_PtrScg, caddr_t	bp,	int	type,	int	addr,	int	cnt);
int		read_disk_info(void* A_PtrScg, caddr_t	bp,	int	cnt);
int		mode_select(void* A_PtrScg,	unsigned char	*dp, int cnt,	int	smp, int pf);
int 	send_opc(void* A_PtrScg, caddr_t bp, int cnt,	int	doopc);
int		scsi_set_speed(void* A_PtrScg, int readspeed, int writespeed, int rotctl);
int		scsi_blank(void* A_PtrScg, long addr, int blanktype, BOOL immed);
BOOL	mmc_check(void* A_PtrScg, BOOL *cdrrp, BOOL *cdwrp, BOOL *cdrrwp, BOOL *cdwrwp, BOOL *dvdp, BOOL *dvdwp);
void	mmc_getval(struct cd_mode_page_2A *mp, BOOL *cdrrp, BOOL *cdwrp, BOOL *cdrrwp, BOOL *cdwrwp,
	BOOL *dvdp, BOOL *dvdwp);
BOOL	allow_atapi(void* A_PtrScg, BOOL new1);
struct cd_mode_page_2A * mmc_cap(void* A_PtrScg, unsigned char	*modep);
BOOL	scsi_in_progress(void* A_PtrScg);
int		send_cue_sheet(void* A_PtrScg, caddr_t bp,long size);
int		scsi_get_speed(void* A_PtrScg, int *readspeedp, int *writespeedp);
int		read_session_offset_philips(void* A_PtrScg, long *);

BOOL	is_mmc(void* A_PtrScg, BOOL *cdwp, BOOL *dvdwp);
int		read_toc(void* A_PtrScg, caddr_t bp, int track,	int	cnt,	int	msf,	int	fmt);
int		read_track_info(void* A_PtrScg, caddr_t bp, int type, int addr, int cnt);
int		mode_select_sg0(void* A_PtrScg, unsigned char	*dp, int	cnt,	int	smp, int	pf);

int		read_toc_philips(void* A_PtrScg, caddr_t	bp,	int	track,	int	cnt,	int	msf,	int	fmt);
int		mode_select_g1(void* A_PtrScg, unsigned char *dp, int	cnt, int smp,	int	pf);

int		reserve_tr_rzone(void* A_PtrScg, long	size);

int		read_scsi(void* A_PtrScg, caddr_t, long, int);
int		read_g0(void* A_PtrScg, caddr_t, long, int);
int		read_g1(void* A_PtrScg, caddr_t, long, int);

BOOL	is_cddrive(void* A_PtrScg);
int		qic02(void* A_PtrScg, int	cmd);
int		sense_secsize(void* A_PtrScg,	int	current);
int		read_capacity(void* A_PtrScg);
int		read_tochdr(void* A_PtrScg,	void* A_PtrDev, int *fp,	int	*lp);
BOOL	cdr_underrun(void* A_PtrScg);

//scsimmc
int		prdiskstatus_Lib(void* A_PtrScg, void* A_PtrDev, BOOL is_cd);
int		get_diskinfo(void* A_PtrScg, struct disk_info *dip, int cnt);
int		get_mediatype(void* A_PtrScg);
int		get_curprofile(void* A_PtrScg);
int		get_configuration(void* A_PtrScg, caddr_t	bp,	int	cnt, int st_feature, int rt);
BOOL	pname_known(__u32	code);
int		get_proflist(void* A_PtrScg, BOOL	*wp, BOOL	*cdp, BOOL *dvdp, BOOL *dvdplusp, BOOL *ddcdp);
int		get_profiles(void* A_PtrScg, caddr_t bp, int cnt);
int		get_conflen(void* A_PtrScg,	int	st_feature,	int	rt);
int		get_wproflist(void* A_PtrScg, BOOL *cdp, BOOL	*dvdp, BOOL	*dvdplusp, BOOL	*ddcdp);

int		get_supported_cdrw_media_types(void* A_PtrScg);
int		sessstatus(void* A_PtrScg, BOOL is_cd, long *offp, long *nwap);
int		scsi_get_perf_curspeed(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp);
void	print_format_capacities(void* A_PtrScg);
int		get_format_capacities(void* A_PtrScg, caddr_t bp, int cnt);
int		read_format_capacities(void* A_PtrScg, caddr_t bp, int cnt);
int		scsi_get_perf_maxspeed(void* A_PtrScg, __u32 *readp, __u32 *writep, __u32 *endp);
int		has_profile(void* A_PtrScg,	int	profile);

int		speed_select_mdvd(void* A_PtrScg, int readspeed, int writespeed);
int		get_blf(int mt);

BOOL	get_mode_params(void* A_PtrScg, int page, char *pagename, unsigned char *modep,
	unsigned char *cmodep, unsigned char *dmodep, unsigned char *smodep, int *lenp);
BOOL	set_mode_params(void* A_PtrScg, char *pagename, unsigned char *modep, int len, int save, int secsize);

int		mode_sense(void* A_PtrScg, unsigned char *dp, int	cnt, int page, int pcf);
int		mode_sense_sg0(void* A_PtrScg, unsigned char *dp, int cnt, int page, int pcf);
int		mode_select_g0(void* A_PtrScg, unsigned char *, int, int, int);
int		mode_sense_g1(void* A_PtrScg,	unsigned char	*dp, int cnt,	int	page,	int	pcf);
int		mode_sense_g0(void* A_PtrScg, unsigned char *dp, int cnt, int page, int pcf);

char*		astoi(const char *s, int *i);
char*		astol(const char *s, long *l);
char*		astolb(const char *s,	long *l, int base);
void		timevaldiff(struct timeval *start, struct timeval	*stop);

//subchan
int			do_leadin(track_t *trackp);
int			write_leadin_Lib(void* A_PtrScg, void* A_PtrDev, track_t *trackp, int leadinstart);
int			write_leadout(void* A_PtrScg, void* A_PtrDev, track_t *trackp);
void		fillsubch(track_t *trackp, unsigned char *sp, int secno, int nsecs);
void		filltpoint(unsigned char *sub, int ctrl_adr, int point, msf_t *mp);
void		fillttime(unsigned char *sub, msf_t *mp);
void		qpto96(unsigned char *sub, unsigned char *subq, int dop);
void		addrw(unsigned char *sub, unsigned char	*subrwptr);
void		qwto16(unsigned char *subq, unsigned char *subptr);
void		subrecodesecs(track_t *trackp, unsigned char *bp, int address, int nsecs);
void 		encsectors(track_t	*trackp, unsigned char	*bp,	int	address,	int	nsecs);

//cd_misc
int			from_bcd(int b);
int			to_bcd(int i);
long		msf_to_lba(int m, int s, int f, BOOL force_positive);
BOOL		lba_to_msf(long lba, msf_t *mp);
void		sec_to_msf(long sec, msf_t *mp);
void		print_min_atip(long li, long lo);
BOOL		is_cdspeed(int speed);

//crc16
__u16	calcCRC(unsigned char *buf, __u32 bsize);
__u16	fillcrc(unsigned char *buf, __u32 bsize);
__u16	flip_crc_error_corr(unsigned char *b, __u32 bsize, __u32 p_crc);

FILE*		fileopen(char	*name, char	*mode);

//cdtext
void	packtext(int tracks, track_t *trackp);
int		write_cdtext(void* A_PtrScg, void* A_PtrDev, long startsec);

//streql
int		streql(char	*a, char	*b);

//cmpnullbytes
int		cmpnullbytes(void	*fromp, int cnt);

//astoll
char*	astoll(char *s,	__u64 *l);
char*	astollb(char *s, __u64 *l, int base);

//filesize
off_t	filesize(FILE	*f);

//raisecond
void	raisecond(char *signame, long arg2);

//fileread
ssize_t	fileread(FILE	*f,	void *buf, size_t	len);

//flag
int	_io_get_my_flag(FILE	*fp);

//niread
ssize_t	_niread(int	f, void	*buf,	size_t count);

//fgetline
int	fgetline(FILE	*f, char *buf, int len);

//fileluopen
FILE*	fileluopen(int f, char *mode);

//diskid
__u32 disk_rcap(msf_t *mp, __u32 maxblock, BOOL rw, BOOL audio);

//sector
int		encspeed();
void	scrsectors(track_t *trackp,	unsigned char	*bp, int address,	int	nsecs);

//isosize
__u64	isosize(int	f, off_t* A_PtrOffSet);

//swapbytes
void	swabbytes(void *vp, int	cnt);

//movesect
void	scatter_secs(track_t *trackp,	char *bp,	int	nsecs);

//edcecc
int		scramble_L2(unsigned char *inout);

struct ricoh_mode_page_30*	get_justlink_ricoh(void* A_PtrScg, unsigned char *mode);

int		check_writemodes_mmc(void* A_PtrScg, void* A_PtrDev);
int		deflt_writemodes_mmc(void* A_PtrScg, BOOL reset_dummy);
int		cmd_dummy(void* A_PtrScg,	void* A_PtrDev);
int		cmd_ill(void* A_PtrScg);
int		format_dummy(void* A_PtrScg, void* A_PtrDev, int fmtflags);
int		blank_dummy(void* A_PtrScg, void* A_PtrDev, long addr, int blanktype);
int		buf_dummy(void* A_PtrScg,	long	*sp,	long	*fp);
//BOOL	set_cdrcmds(char *name,	cdr_t	**dpp);
void*	get_cdrcmds(void* A_PtrScg);
int		drive_getdisktype(void* A_PtrScg, void* A_PtrDev);
int		no_sendcue(void* A_PtrScg, void* A_PtrDev, track_t	*trackp);
int		drive_attach(void* A_PtrScg, void* A_PtrDev);
int		no_diskstatus(void* A_PtrScg,	void* A_PtrDev);
int		blank_simul(void* A_PtrScg,	void* A_PtrDev, long	addr,	int	blanktype);

int		pad_track(void* A_PtrScg, void* A_PtrDev, track_t *trackp, long startsec, __u64 amt,
	BOOL dolast, __u64 *bytesp);

int waitformat(void* A_PtrScg, int secs);

//====================================================================
//out Enviroment conditional
#endif
