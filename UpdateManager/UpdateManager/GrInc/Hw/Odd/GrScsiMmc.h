/*
	scsi-mmc
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrScsiMmc
#define	_EPC_GrScsiMmc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrOddRec.h>

//====================================================================
//constance
#define	CGMS_PERMITTED		0	/* Unlimited copy permitted	*/
#define	CGMS_RES		1	/* Reserved			*/
#define	CGMS_ONE_COPY		2	/* One copy permitted		*/
#define	CGMS_NO_COPY		3	/* No copy permitted		*/

#define	WRC_DEF_RC	0		/* Media default rotational control */
#define	WRC_CAV		1		/* CAV				    */

//====================================================================
//global macro

//====================================================================
//global type
typedef struct opc {
	unsigned __s8	opc_speed[2];
	unsigned __s8	opc_val[6];
} opc_t;

struct disk_info {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	disk_status	: 2;	/* Status of the disk		*/
	unsigned __s8	sess_status	: 2;	/* Status of last session	*/
	unsigned __s8	erasable	: 1;	/* Disk is erasable		*/
	unsigned __s8	dtype		: 3;	/* Disk information data type	*/
	unsigned __s8	first_track;		/* # of first track on disk	*/
	unsigned __s8	numsess;		/* # of sessions		*/
	unsigned __s8	first_track_ls;		/* First track in last session	*/
	unsigned __s8	last_track_ls;		/* Last track in last session	*/
	unsigned __s8	bg_format_stat	: 2;	/* Background format status	*/
	unsigned __s8	dbit		: 1;	/* Dirty Bit of defect table	*/
	unsigned __s8	res7_3		: 1;	/* Reserved			*/
	unsigned __s8	dac_v		: 1;	/* Disk application code valid	*/
	unsigned __s8	uru		: 1;	/* This is an unrestricted disk	*/
	unsigned __s8	dbc_v		: 1;	/* Disk bar code valid		*/
	unsigned __s8	did_v		: 1;	/* Disk id valid		*/
	unsigned __s8	disk_type;		/* Disk type			*/
	unsigned __s8	numsess_msb;		/* # of sessions (MSB)		*/
	unsigned __s8	first_track_ls_msb;	/* First tr. in last ses. (MSB)	*/
	unsigned __s8	last_track_ls_msb;	/* Last tr. in last ses. (MSB)	*/
	unsigned __s8	disk_id[4];		/* Disk identification		*/
	unsigned __s8	last_lead_in[4];	/* Last session lead in time	*/
	unsigned __s8	last_lead_out[4];	/* Last session lead out time	*/
	unsigned __s8	disk_barcode[8];	/* Disk bar code		*/
	unsigned __s8	disk_appl_code;		/* Disk application code	*/
	unsigned __s8	num_opc_entries;	/* # of OPC table entries	*/
	opc_t	opc_table[1];		/* OPC table 			*/
};

struct mmc_cue {
	unsigned __s8	cs_ctladr;		/* CTL/ADR for this track	*/
	unsigned __s8	cs_tno;			/* This track number		*/
	unsigned __s8	cs_index;		/* Index within this track	*/
	unsigned __s8	cs_dataform;		/* Data form 			*/
	unsigned __s8	cs_scms;		/* Serial copy management	*/
	unsigned __s8	cs_min;			/* Absolute time minutes	*/
	unsigned __s8	cs_sec;			/* Absolute time seconds	*/
	unsigned __s8	cs_frame;		/* Absolute time frames		*/
};

struct tocheader {
	unsigned __s8	len[2];
	unsigned __s8	first;
	unsigned __s8	last;
};

struct ftrackdesc {
	unsigned __s8	sess_number;

	unsigned __s8	control		: 4;
	unsigned __s8	adr		: 4;

	unsigned __s8	track;
	unsigned __s8	point;
	unsigned __s8	amin;
	unsigned __s8	asec;
	unsigned __s8	aframe;
	unsigned __s8	res7;
	unsigned __s8	pmin;
	unsigned __s8	psec;
	unsigned __s8	pframe;
};

struct fdiskinfo {
	struct tocheader	hd;
	struct ftrackdesc	desc[1];
};

struct atipdesc {
	unsigned __s8	ref_speed	: 3;	/* Reference speed		*/
	unsigned __s8	res4_3		: 1;	/* Reserved			*/
	unsigned __s8	ind_wr_power	: 3;	/* Indicative tgt writing power	*/
	unsigned __s8	res4_7		: 1;	/* Reserved (must be "1")	*/
	unsigned __s8	res5_05		: 6;	/* Reserved			*/
	unsigned __s8	uru		: 1;	/* Disk is for unrestricted use	*/
	unsigned __s8	res5_7		: 1;	/* Reserved (must be "0")	*/
	unsigned __s8	a3_v		: 1;	/* A 3 Values valid		*/
	unsigned __s8	a2_v		: 1;	/* A 2 Values valid		*/
	unsigned __s8	a1_v		: 1;	/* A 1 Values valid		*/
	unsigned __s8	sub_type	: 3;	/* Disc sub type		*/
	unsigned __s8	erasable	: 1;	/* Disk is erasable		*/
	unsigned __s8	res6_7		: 1;	/* Reserved (must be "1")	*/
	unsigned __s8	lead_in[4];		/* Lead in time			*/
	unsigned __s8	lead_out[4];		/* Lead out time		*/
	unsigned __s8	res15;			/* Reserved			*/
	unsigned __s8	clv_high	: 4;	/* Highes usable CLV recording speed */
	unsigned __s8	clv_low		: 3;	/* Lowest usable CLV recording speed */
	unsigned __s8	res16_7		: 1;	/* Reserved (must be "0")	*/
	unsigned __s8	res17_0		: 1;	/* Reserved			*/
	unsigned __s8	tgt_y_pow	: 3;	/* Tgt y val of the power mod fun */
	unsigned __s8	power_mult	: 3;	/* Power multiplication factor	*/
	unsigned __s8	res17_7		: 1;	/* Reserved (must be "0")	*/
	unsigned __s8	res_18_30	: 4;	/* Reserved			*/
	unsigned __s8	rerase_pwr_ratio: 3;	/* Recommended erase/write power*/
	unsigned __s8	res18_7		: 1;	/* Reserved (must be "1")	*/
	unsigned __s8	res19;			/* Reserved			*/
	unsigned __s8	a2[3];			/* A 2 Values			*/
	unsigned __s8	res23;			/* Reserved			*/
	unsigned __s8	a3[3];			/* A 3 Vaules			*/
	unsigned __s8	res27;			/* Reserved			*/
};

struct atipinfo {
	struct tocheader	hd;
	struct atipdesc		desc;
};

struct track_info {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	track_number;		/* Track number for this info	*/
	unsigned __s8	session_number;		/* Session number for this info	*/
	unsigned __s8	res4;			/* Reserved			*/
	unsigned __s8	track_mode	: 4;	/* Track mode (Q-sub control)	*/
	unsigned __s8	copy		: 1;	/* This track is a higher copy	*/
	unsigned __s8	damage		: 1;	/* if 1 & nwa_valid 0: inc track*/
	unsigned __s8	res5_67		: 2;	/* Reserved			*/
	unsigned __s8	data_mode	: 4;	/* Data mode of this track	*/
	unsigned __s8	fp		: 1;	/* This is a fixed packet track	*/
	unsigned __s8	packet		: 1;	/* This track is in packet mode	*/
	unsigned __s8	blank		: 1;	/* This is an invisible track	*/
	unsigned __s8	rt		: 1;	/* This is a reserved track	*/
	unsigned __s8	nwa_valid	: 1;	/* Next writable addr valid	*/
	unsigned __s8	res7_17		: 7;	/* Reserved			*/
	unsigned __s8	track_start[4];		/* Track start address		*/
	unsigned __s8	next_writable_addr[4];	/* Next writable address	*/
	unsigned __s8	free_blocks[4];		/* Free usr blocks in this track*/
	unsigned __s8	packet_size[4];		/* Packet size if in fixed mode	*/
	unsigned __s8	track_size[4];		/* # of user data blocks in trk	*/
};

struct rzone_info {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	rzone_num_lsb;		/* RZone number LSB		*/
	unsigned __s8	border_num_lsb;		/* Border number LSB		*/
	unsigned __s8	res_4;			/* Reserved			*/
	unsigned __s8	trackmode	: 4;	/* Track mode			*/
	unsigned __s8	copy		: 1;	/* Higher generation CD copy	*/
	unsigned __s8	damage		: 1;	/* Damaged RZone		*/
	unsigned __s8	ljrs		: 2;	/* Layer jump recording status	*/
	unsigned __s8	datamode	: 4;	/* Data mode			*/
	unsigned __s8	fp		: 1;	/* Fixed packet			*/
	unsigned __s8	incremental	: 1;	/* RZone is to be written incremental */
	unsigned __s8	blank		: 1;	/* RZone is blank		*/
	unsigned __s8	rt		: 1;	/* RZone is reserved		*/
	unsigned __s8	nwa_v		: 1;	/* Next WR address is valid	*/
	unsigned __s8	lra_v		: 1;	/* Last rec address is valid	*/
	unsigned __s8	res7_27		: 6;	/* Reserved			*/
	unsigned __s8	rzone_start[4];		/* RZone start address		*/
	unsigned __s8	next_recordable_addr[4]; /* Next recordable address	*/
	unsigned __s8	free_blocks[4];		/* Free blocks in RZone		*/
	unsigned __s8	block_factor[4];	/* # of sectors of disc acc unit */
	unsigned __s8	rzone_size[4];		/* RZone size			*/
	unsigned __s8	last_recorded_addr[4];	/* Last Recorded addr in RZone	*/
	unsigned __s8	rzone_num_msb;		/* RZone number MSB		*/
	unsigned __s8	border_num_msb;		/* Border number MSB		*/
	unsigned __s8	res_34_35[2];		/* Reserved			*/
	unsigned __s8	read_compat_lba[4];	/* Read Compatibilty LBA	*/
	unsigned __s8	next_layer_jump[4];	/* Next layer jump address	*/
	unsigned __s8	last_layer_jump[4];	/* Last layer jump address	*/
};

struct dvd_structure_00 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	book_version	: 4;	/* DVD Book version		*/
	unsigned __s8	book_type	: 4;	/* DVD Book type		*/
	unsigned __s8	maximum_rate	: 4;	/* Maximum data rate (coded)	*/
	unsigned __s8	disc_size	: 4;	/* Disc size (coded)		*/
	unsigned __s8	layer_type	: 4;	/* Layer type			*/
	unsigned __s8	track_path	: 1;	/* 0 = parallel, 1 = opposit dir*/
	unsigned __s8	numlayers	: 2;	/* Number of Layers (0 == 1)	*/
	unsigned __s8	res2_7		: 1;	/* Reserved			*/
	unsigned __s8	track_density	: 4;	/* Track density (coded)	*/
	unsigned __s8	linear_density	: 4;	/* Linear data density (coded)	*/
	unsigned __s8	res8;			/* Reserved			*/
	unsigned __s8	phys_start[3];		/* Starting Physical sector #	*/
	unsigned __s8	res12;			/* Reserved			*/
	unsigned __s8	phys_end[3];		/* End physical data sector #	*/
	unsigned __s8	res16;			/* Reserved			*/
	unsigned __s8	end_layer0[3];		/* End sector # in layer	*/
	unsigned __s8	res20		: 7;	/* Reserved			*/
	unsigned __s8	bca		: 1;	/* BCA flag bit			*/
};

struct dvd_structure_01 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	copyr_prot_type;	/* Copyright prot system type	*/
	unsigned __s8	region_mgt_info;	/* Region management info	*/
	unsigned __s8	res67[2];		/* Reserved			*/
};

struct dvd_structure_02 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	key_data[2048];		/* Disc Key data		*/
};

struct dvd_structure_03 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	bca_info[1];		/* BCA information (12-188 bytes)*/
};

struct dvd_structure_04 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	man_info[2048];		/* Disc manufacturing info	*/
};

struct dvd_structure_05 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res4_03		: 4;	/* Reserved			*/
	unsigned __s8	cgms		: 2;	/* CGMS (see below)		*/
	unsigned __s8	res4_6		: 1;	/* Reserved			*/
	unsigned __s8	cpm		: 1;	/* This is copyrighted material	*/
	unsigned __s8	res57[3];		/* Reserved			*/
};

struct dvd_structure_0D {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	last_rma_sector[2];	/* Last recorded RMA sector #	*/
	unsigned __s8	rmd_bytes[1];		/* Content of Record man area	*/
};

struct dvd_structure_0E {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	field_id;		/* Field ID (1)			*/
	unsigned __s8	application_code;	/* Disc Application code	*/
	unsigned __s8	phys_data;		/* Disc Phisical Data		*/
	unsigned __s8	last_recordable_addr[3]; /* Last addr of recordable area */
	unsigned __s8	res_a[2];		/* Reserved			*/
	unsigned __s8	field_id_2;		/* Field ID (2)			*/
	unsigned __s8	ind_wr_power;		/* Recommended writing power	*/
	unsigned __s8	ind_wavelength;		/* Wavelength for ind_wr_power	*/
	unsigned __s8	opt_wr_strategy[4];	/* Optimum write Strategy	*/
	unsigned __s8	res_b[1];		/* Reserved			*/
	unsigned __s8	field_id_3;		/* Field ID (3)			*/
	unsigned __s8	man_id[6];		/* Manufacturer ID		*/
	unsigned __s8	res_m1;			/* Reserved			*/
	unsigned __s8	field_id_4;		/* Field ID (4)			*/
	unsigned __s8	man_id2[6];		/* Manufacturer ID		*/
	unsigned __s8	res_m2;			/* Reserved			*/
};

struct dvd_structure_0F {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res45[2];		/* Reserved			*/
	unsigned __s8	random[2];		/* Random number		*/
	unsigned __s8	year[4];		/* Year (ascii)			*/
	unsigned __s8	month[2];		/* Month (ascii)		*/
	unsigned __s8	day[2];			/* Day (ascii)			*/
	unsigned __s8	hour[2];		/* Hour (ascii)			*/
	unsigned __s8	minute[2];		/* Minute (ascii)		*/
	unsigned __s8	second[2];		/* Second (ascii)		*/
};

struct dvd_structure_0F_w {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res45[2];		/* Reserved			*/
	unsigned __s8	year[4];		/* Year (ascii)			*/
	unsigned __s8	month[2];		/* Month (ascii)		*/
	unsigned __s8	day[2];			/* Day (ascii)			*/
	unsigned __s8	hour[2];		/* Hour (ascii)			*/
	unsigned __s8	minute[2];		/* Minute (ascii)		*/
	unsigned __s8	second[2];		/* Second (ascii)		*/
};

struct dvd_structure_20 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res47[4];		/* Reserved			*/
	unsigned __s8	l0_area_cap[4];		/* Layer 0 area capacity	*/
};

struct dvd_structure_22 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res47[4];		/* Reserved			*/
	unsigned __s8	jump_interval_size[4];	/* Jump interval size		*/
};

struct dvd_structure_23 {
	unsigned __s8	data_len[2];		/* Data len without this info	*/
	unsigned __s8	res23[2];		/* Reserved			*/
	unsigned __s8	res47[4];		/* Reserved			*/
	unsigned __s8	jump_lba[4];		/* Jump logical block address	*/
};

struct mmc_performance_header {
	unsigned __s8	p_datalen[4];		/* Performance Data length	*/
	unsigned __s8	p_exept		:1;	/* Nominal vs. Exept. conditions*/
	unsigned __s8	p_write		:1;	/* Write vs. Read performance	*/
	unsigned __s8	p_res_4		:6;	/* Reserved bits...		*/
	unsigned __s8	p_res[3];		/* Reserved bytes		*/
};

struct mmc_performance {		/* Type == 00 (nominal)		*/
	unsigned __s8	start_lba[4];		/* Starting LBA			*/
	unsigned __s8	start_perf[4];		/* Start Performance		*/
	unsigned __s8	end_lba[4];		/* Ending LBA			*/
	unsigned __s8	end_perf[4];		/* Ending Performance		*/
};

struct mmc_exceptions {			/* Type == 00 (exceptions)	*/
	unsigned __s8	lba[4];			/* LBA				*/
	unsigned __s8	time[2];		/* Time				*/
};

struct mmc_write_speed {		/* Type == 00 (write speed)	*/
	unsigned __s8	p_mrw		:1;	/* Suitable for mixed read/write*/
	unsigned __s8	p_exact		:1;	/* Speed count for whole media	*/
	unsigned __s8	p_rdd		:1;	/* Media rotational control	*/
	unsigned __s8	p_wrc		:2;	/* Write rotational control	*/
	unsigned __s8	p_res		:3;	/* Reserved bits...		*/
	unsigned __s8	res[3];			/* Reserved Bytes		*/
	unsigned __s8	end_lba[4];		/* Ending LBA			*/
	unsigned __s8	read_speed[4];		/* Read Speed			*/
	unsigned __s8	write_speed[4];		/* Write Speed			*/
};

struct mmc_streaming {			/* Performance for set streaming*/
	unsigned __s8	p_ra		:1;	/* Random Acess			*/
	unsigned __s8	p_exact		:1;	/* Set values exactly		*/
	unsigned __s8	p_rdd		:1;	/* Restore unit defaults	*/
	unsigned __s8	p_wrc		:2;	/* Write rotational control	*/
	unsigned __s8	p_res		:3;	/* Reserved bits...		*/
	unsigned __s8	res[3];			/* Reserved Bytes		*/
	unsigned __s8	start_lba[4];		/* Starting LBA			*/
	unsigned __s8	end_lba[4];		/* Ending LBA			*/
	unsigned __s8	read_size[4];		/* Read Size			*/
	unsigned __s8	read_time[4];		/* Read Time			*/
	unsigned __s8	write_size[4];		/* Write Size			*/
	unsigned __s8	write_time[4];		/* Write Time			*/
};

//====================================================================
//class

//====================================================================
//global function
__s32		prdiskstatus(St_GrScsiIf	*scgp, cdr_t *dp,	BOOL8 is_cd);
__s32		get_diskinfo(St_GrScsiIf *scgp, struct disk_info *dip, __s32 cnt);
__s32		get_mediatype(St_GrScsiIf *scgp);
__s32		get_curprofile(St_GrScsiIf *scgp);
__s32		get_configuration(St_GrScsiIf *scgp, caddr_t	bp,	__s32	cnt, __s32 st_feature, __s32 rt);
BOOL8	pname_known(__u32	code);
__s32		get_proflist(St_GrScsiIf	*scgp, BOOL8	*wp, BOOL8	*cdp,	BOOL8 *dvdp,	BOOL8 *dvdplusp,	BOOL8 *ddcdp);
__s32		get_profiles(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt);
__s32		get_conflen(St_GrScsiIf *scgp,	__s32	st_feature,	__s32	rt);
__s32		get_wproflist(St_GrScsiIf *scgp,	BOOL8 *cdp, BOOL8	*dvdp, BOOL8	*dvdplusp, BOOL8	*ddcdp);

__s32		get_supported_cdrw_media_types(St_GrScsiIf *scgp);
__s32		sessstatus(St_GrScsiIf *scgp, BOOL8 is_cd, long *offp, long *nwap);
__s32		scsi_get_perf_curspeed(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp);
void	print_format_capacities(St_GrScsiIf	*scgp);
__s32		get_format_capacities(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt);
__s32		read_format_capacities(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt);
__s32		scsi_get_perf_maxspeed(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp);
__s32		has_profile(St_GrScsiIf *scgp,	__s32	profile);

__s32		speed_select_mdvd(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed);
__s32		get_blf(__s32 mt);

//====================================================================
//out Enviroment conditional
#endif

