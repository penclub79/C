/*
	scsi-reg
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrScsiReg
#define	_EPC_GrScsiReg

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance
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
	unsigned __s8	p_code		: 6;	\
	unsigned __s8	p_res		: 1;	\
	unsigned __s8	parsave		: 1

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
struct	scsi_inquiry {
	unsigned __s8	type		: 5;	/*  0 */
	unsigned __s8	qualifier	: 3;	/*  0 */

	unsigned __s8	type_modifier	: 7;	/*  1 */
	unsigned __s8	removable	: 1;	/*  1 */

	unsigned __s8	ansi_version	: 3;	/*  2 */
	unsigned __s8	ecma_version	: 3;	/*  2 */
	unsigned __s8	iso_version	: 2;	/*  2 */

	unsigned __s8	data_format	: 4;	/*  3 */
	unsigned __s8	res3_54		: 2;	/*  3 */
	unsigned __s8	termiop		: 1;	/*  3 */
	unsigned __s8	aenc		: 1;	/*  3 */

	unsigned __s8	add_len		: 8;	/*  4 */
	unsigned __s8	sense_len	: 8;	/*  5 */ /* only Emulex ??? */
	unsigned __s8	res2		: 8;	/*  6 */

	unsigned __s8	softreset	: 1;	/*  7 */
	unsigned __s8	cmdque		: 1;
	unsigned __s8	res7_2		: 1;
	unsigned __s8	linked		: 1;
	unsigned __s8	sync		: 1;
	unsigned __s8	wbus16		: 1;
	unsigned __s8	wbus32		: 1;
	unsigned __s8	reladr		: 1;	/*  7 */

	__s8	vendor_info[8];		/*  8 */
	__s8	prod_ident[16];		/* 16 */
	__s8	prod_revision[4];	/* 32 */
};					/* 96 */

struct scsi_mode_header {
	unsigned __s8	sense_data_len	: 8;
	unsigned __s8	medium_type;
	unsigned __s8	res2		: 4;
	unsigned __s8	cache		: 1;
	unsigned __s8	res		: 2;
	unsigned __s8	write_prot	: 1;
	unsigned __s8	blockdesc_len;
};

struct scsi_modesel_header {
	unsigned __s8	sense_data_len	: 8;
	unsigned __s8	medium_type;
	unsigned __s8	res2		: 8;
	unsigned __s8	blockdesc_len;
};

struct scsi_mode_blockdesc {
	unsigned __s8	density;
	unsigned __s8	nlblock[3];
	unsigned __s8	res		: 8;
	unsigned __s8	lblen[3];
};

struct acb_mode_data {
	unsigned __s8	listformat;
	unsigned __s8	ncyl[2];
	unsigned __s8	nhead;
	unsigned __s8	start_red_wcurrent[2];
	unsigned __s8	start_precomp[2];
	unsigned __s8	landing_zone;
	unsigned __s8	step_rate;
	unsigned __s8			: 2;
	unsigned __s8	hard_sec	: 1;
	unsigned __s8	fixed_media	: 1;
	unsigned __s8			: 4;
	unsigned __s8	sect_per_trk;
};

struct scsi_mode_page_header {
	unsigned __s8	p_code		: 6;
	unsigned __s8	res		: 1;
	unsigned __s8	parsave		: 1;
	unsigned __s8	p_len;
};

struct scsi_mode_page_01 {		/* Error recovery Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0A = 12 Bytes */
	unsigned __s8	disa_correction	: 1;	/* Byte 2 */
	unsigned __s8	term_on_rec_err	: 1;
	unsigned __s8	report_rec_err	: 1;
	unsigned __s8	en_early_corr	: 1;
	unsigned __s8	read_continuous	: 1;
	unsigned __s8	tranfer_block	: 1;
	unsigned __s8	en_auto_reall_r	: 1;
	unsigned __s8	en_auto_reall_w	: 1;	/* Byte 2 */
	unsigned __s8	rd_retry_count;		/* Byte 3 */
	unsigned __s8	correction_span;
	__s8	head_offset_count;
	__s8	data_strobe_offset;
	unsigned __s8	res;
	unsigned __s8	wr_retry_count;
	unsigned __s8	res_tape[2];
	unsigned __s8	recov_timelim[2];
};

struct scsi_mode_page_02 {		/* Device dis/re connect Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0E = 16 Bytes */
	unsigned __s8	buf_full_ratio;
	unsigned __s8	buf_empt_ratio;
	unsigned __s8	bus_inact_limit[2];
	unsigned __s8	disc_time_limit[2];
	unsigned __s8	conn_time_limit[2];
	unsigned __s8	max_burst_size[2];	/* Start SCSI-2 */
	unsigned __s8	data_tr_dis_ctl	: 2;
	unsigned __s8			: 6;
	unsigned __s8	res[3];
};

struct scsi_mode_page_03 {		/* Direct access format Paramters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x16 = 24 Bytes */
	unsigned __s8	trk_per_zone[2];
	unsigned __s8	alt_sec_per_zone[2];
	unsigned __s8	alt_trk_per_zone[2];
	unsigned __s8	alt_trk_per_vol[2];
	unsigned __s8	sect_per_trk[2];
	unsigned __s8	bytes_per_phys_sect[2];
	unsigned __s8	interleave[2];
	unsigned __s8	trk_skew[2];
	unsigned __s8	cyl_skew[2];
	unsigned __s8			: 3;
	unsigned __s8	inhibit_save	: 1;
	unsigned __s8	fmt_by_surface	: 1;
	unsigned __s8	removable	: 1;
	unsigned __s8	hard_sec	: 1;
	unsigned __s8	soft_sec	: 1;
	unsigned __s8	res[3];
};

struct scsi_mode_page_04 {		/* Rigid disk Geometry Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x16 = 24 Bytes */
	unsigned __s8	ncyl[3];
	unsigned __s8	nhead;
	unsigned __s8	start_precomp[3];
	unsigned __s8	start_red_wcurrent[3];
	unsigned __s8	step_rate[2];
	unsigned __s8	landing_zone[3];
	unsigned __s8	rot_pos_locking	: 2;	/* Start SCSI-2 */
	unsigned __s8			: 6;	/* Start SCSI-2 */
	unsigned __s8	rotational_off;
	unsigned __s8	res1;
	unsigned __s8	rotation_rate[2];
	unsigned __s8	res2[2];
};

struct scsi_mode_page_05 {		/* Flexible disk Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x1E = 32 Bytes */
	unsigned __s8	transfer_rate[2];
	unsigned __s8	nhead;
	unsigned __s8	sect_per_trk;
	unsigned __s8	bytes_per_phys_sect[2];
	unsigned __s8	ncyl[2];
	unsigned __s8	start_precomp[2];
	unsigned __s8	start_red_wcurrent[2];
	unsigned __s8	step_rate[2];
	unsigned __s8	step_pulse_width;
	unsigned __s8	head_settle_delay[2];
	unsigned __s8	motor_on_delay;
	unsigned __s8	motor_off_delay;
	unsigned __s8	spc		: 4;
	unsigned __s8			: 4;
	unsigned __s8			: 5;
	unsigned __s8	mo		: 1;
	unsigned __s8	ssn		: 1;
	unsigned __s8	trdy		: 1;
	unsigned __s8	write_compensation;
	unsigned __s8	head_load_delay;
	unsigned __s8	head_unload_delay;
	unsigned __s8	pin_2_use	: 4;
	unsigned __s8	pin_34_use	: 4;
	unsigned __s8	pin_1_use	: 4;
	unsigned __s8	pin_4_use	: 4;
	unsigned __s8	rotation_rate[2];
	unsigned __s8	res[2];
};

struct scsi_mode_page_07 {		/* Verify Error recovery */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0A = 12 Bytes */
	unsigned __s8	disa_correction	: 1;	/* Byte 2 */
	unsigned __s8	term_on_rec_err	: 1;
	unsigned __s8	report_rec_err	: 1;
	unsigned __s8	en_early_corr	: 1;
	unsigned __s8	res		: 4;	/* Byte 2 */
	unsigned __s8	ve_retry_count;		/* Byte 3 */
	unsigned __s8	ve_correction_span;
	__s8	res2[5];		/* Byte 5 */
	unsigned __s8	ve_recov_timelim[2];	/* Byte 10 */
};

struct scsi_mode_page_08 {		/* Caching Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0A = 12 Bytes */
	unsigned __s8	disa_rd_cache	: 1;	/* Byte 2 */
	unsigned __s8	muliple_fact	: 1;
	unsigned __s8	en_wt_cache	: 1;
	unsigned __s8	res		: 5;	/* Byte 2 */
	unsigned __s8	wt_ret_pri	: 4;	/* Byte 3 */
	unsigned __s8	demand_rd_ret_pri: 4;	/* Byte 3 */
	unsigned __s8	disa_pref_tr_len[2];	/* Byte 4 */
	unsigned __s8	min_pref[2];		/* Byte 6 */
	unsigned __s8	max_pref[2];		/* Byte 8 */
	unsigned __s8	max_pref_ceiling[2];	/* Byte 10 */
};

struct scsi_mode_page_09 {		/* Peripheral device Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* >= 0x06 = 8 Bytes */
	unsigned __s8	interface_id[2];	/* Byte 2 */
	unsigned __s8	res[4];			/* Byte 4 */
	unsigned __s8	vendor_specific[1];	/* Byte 8 */
};

struct scsi_mode_page_0A {		/* Common device Control Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x06 = 8 Bytes */
	unsigned __s8	rep_log_exeption: 1;	/* Byte 2 */
	unsigned __s8	res		: 7;	/* Byte 2 */
	unsigned __s8	dis_queuing	: 1;	/* Byte 3 */
	unsigned __s8	queuing_err_man	: 1;
	unsigned __s8	res2		: 2;
	unsigned __s8	queue_alg_mod	: 4;	/* Byte 3 */
	unsigned __s8	EAENP		: 1;	/* Byte 4 */
	unsigned __s8	UAENP		: 1;
	unsigned __s8	RAENP		: 1;
	unsigned __s8	res3		: 4;
	unsigned __s8	en_ext_cont_all	: 1;	/* Byte 4 */
	unsigned __s8	res4		: 8;
	unsigned __s8	ready_aen_hold_per[2];	/* Byte 6 */
};

struct scsi_mode_page_0B {		/* Medium Types Supported Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x06 = 8 Bytes */
	unsigned __s8	res[2];			/* Byte 2 */
	unsigned __s8	medium_one_supp;	/* Byte 4 */
	unsigned __s8	medium_two_supp;	/* Byte 5 */
	unsigned __s8	medium_three_supp;	/* Byte 6 */
	unsigned __s8	medium_four_supp;	/* Byte 7 */
};

struct scsi_mode_page_0C {		/* Notch & Partition Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x16 = 24 Bytes */
	unsigned __s8	res		: 6;	/* Byte 2 */
	unsigned __s8	logical_notch	: 1;
	unsigned __s8	notched_drive	: 1;	/* Byte 2 */
	unsigned __s8	res2;			/* Byte 3 */
	unsigned __s8	max_notches[2];		/* Byte 4  */
	unsigned __s8	active_notch[2];	/* Byte 6  */
	unsigned __s8	starting_boundary[4];	/* Byte 8  */
	unsigned __s8	ending_boundary[4];	/* Byte 12 */
	unsigned __s8	pages_notched[8];	/* Byte 16 */
};

struct scsi_mode_page_0D {		/* CD-ROM Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x06 = 8 Bytes */
	unsigned __s8	res;			/* Byte 2 */
	unsigned __s8	inact_timer_mult: 4;	/* Byte 3 */
	unsigned __s8	res2		: 4;	/* Byte 3 */
	unsigned __s8	s_un_per_m_un[2];	/* Byte 4  */
	unsigned __s8	f_un_per_s_un[2];	/* Byte 6  */
};

struct sony_mode_page_20 {		/* Sony Format Mode Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0A = 12 Bytes */
	unsigned __s8	format_mode;
	unsigned __s8	format_type;
#define	num_bands	user_band_size	/* Gilt bei Type 1 */
	unsigned __s8	user_band_size[4];	/* Gilt bei Type 0 */
	unsigned __s8	spare_band_size[2];
	unsigned __s8	res[2];
};

struct toshiba_mode_page_20 {		/* Toshiba Speed Control Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x01 = 3 Bytes */
	unsigned __s8	speed		: 1;
	unsigned __s8	res		: 7;
};

struct ccs_mode_page_38 {		/* CCS Caching Parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x0E = 14 Bytes */

	unsigned __s8	cache_table_size: 4;	/* Byte 3 */
	unsigned __s8	cache_en	: 1;
	unsigned __s8	res2		: 1;
	unsigned __s8	wr_index_en	: 1;
	unsigned __s8	res		: 1;	/* Byte 3 */
	unsigned __s8	threshold;		/* Byte 4 Prefetch threshold */
	unsigned __s8	max_prefetch;		/* Byte 5 Max. prefetch */
	unsigned __s8	max_multiplier;		/* Byte 6 Max. prefetch multiplier */
	unsigned __s8	min_prefetch;		/* Byte 7 Min. prefetch */
	unsigned __s8	min_multiplier;		/* Byte 8 Min. prefetch multiplier */
	unsigned __s8	res3[8];		/* Byte 9 */
};

struct cd_mode_page_05 {		/* write parameters */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x32 = 50 Bytes */
	unsigned __s8	write_type	: 4;	/* Session write type (PACKET/TAO...)*/
	unsigned __s8	test_write	: 1;	/* Do not actually write data	    */
	unsigned __s8	LS_V		: 1;	/* Link size valid		    */
	unsigned __s8	BUFE		: 1;	/* Enable Bufunderrun free rec.	    */
	unsigned __s8	res_2_7		: 1;
	unsigned __s8	track_mode	: 4;	/* Track mode (Q-sub control nibble) */
	unsigned __s8	copy		: 1;	/* 1st higher gen of copy prot track ~*/
	unsigned __s8	fp		: 1;	/* Fixed packed (if in packet mode) */
	unsigned __s8	multi_session	: 2;	/* Multi session write type	    */
	unsigned __s8	dbtype		: 4;	/* Data block type		    */
	unsigned __s8	res_4		: 4;	/* Reserved			    */
	unsigned __s8	link_size;		/* Link Size (default is 7)	    */
	unsigned __s8	res_6;			/* Reserved			    */
	unsigned __s8	host_appl_code	: 6;	/* Host application code of disk    */
	unsigned __s8	res_7		: 2;	/* Reserved			    */
	unsigned __s8	session_format;		/* Session format (DA/CDI/XA)	    */
	unsigned __s8	res_9;			/* Reserved			    */
	unsigned __s8	packet_size[4];		/* # of user datablocks/fixed packet */
	unsigned __s8	audio_pause_len[2];	/* # of blocks where index is zero  */
	unsigned __s8	media_cat_number[16];	/* Media catalog Number (MCN)	    */
	unsigned __s8	ISRC[14];		/* ISRC for this track		    */
	unsigned __s8	sub_header[4];
	unsigned __s8	vendor_uniq[4];
};

struct cd_wr_speed_performance {
	unsigned __s8	res0;			/* Reserved			    */
	unsigned __s8	rot_ctl_sel	: 2;	/* Rotational control selected	    */
	unsigned __s8	res_1_27	: 6;	/* Reserved			    */
	unsigned __s8	wr_speed_supp[2];	/* Supported write speed	    */
};

struct cd_mode_page_2A {		/* CD Cap / mech status */
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0x14 = 20 Bytes (MMC) */
					/* 0x18 = 24 Bytes (MMC-2) */
					/* 0x1C >= 28 Bytes (MMC-3) */
	unsigned __s8	cd_r_read	: 1;	/* Reads CD-R  media		    */
	unsigned __s8	cd_rw_read	: 1;	/* Reads CD-RW media		    */
	unsigned __s8	method2		: 1;	/* Reads fixed packet method2 media */
	unsigned __s8	dvd_rom_read	: 1;	/* Reads DVD ROM media		    */
	unsigned __s8	dvd_r_read	: 1;	/* Reads DVD-R media		    */
	unsigned __s8	dvd_ram_read	: 1;	/* Reads DVD-RAM media		    */
	unsigned __s8	res_2_67	: 2;	/* Reserved			    */
	unsigned __s8	cd_r_write	: 1;	/* Supports writing CD-R  media	    */
	unsigned __s8	cd_rw_write	: 1;	/* Supports writing CD-RW media	    */
	unsigned __s8	test_write	: 1;	/* Supports emulation write	    */
	unsigned __s8	res_3_3		: 1;	/* Reserved			    */
	unsigned __s8	dvd_r_write	: 1;	/* Supports writing DVD-R media	    */
	unsigned __s8	dvd_ram_write	: 1;	/* Supports writing DVD-RAM media   */
	unsigned __s8	res_3_67	: 2;	/* Reserved			    */
	unsigned __s8	audio_play	: 1;	/* Supports Audio play operation    */
	unsigned __s8	composite	: 1;	/* Deliveres composite A/V stream   */
	unsigned __s8	digital_port_2	: 1;	/* Supports digital output on port 2 */
	unsigned __s8	digital_port_1	: 1;	/* Supports digital output on port 1 */
	unsigned __s8	mode_2_form_1	: 1;	/* Reads Mode-2 form 1 media (XA)   */
	unsigned __s8	mode_2_form_2	: 1;	/* Reads Mode-2 form 2 media	    */
	unsigned __s8	multi_session	: 1;	/* Reads multi-session media	    */
	unsigned __s8	BUF		: 1;	/* Supports Buffer under. free rec. */
	unsigned __s8	cd_da_supported	: 1;	/* Reads audio data with READ CD cmd */
	unsigned __s8	cd_da_accurate	: 1;	/* READ CD data stream is accurate   */
	unsigned __s8	rw_supported	: 1;	/* Reads R-W sub channel information */
	unsigned __s8	rw_deint_corr	: 1;	/* Reads de-interleved R-W sub chan  */
	unsigned __s8	c2_pointers	: 1;	/* Supports C2 error pointers	    */
	unsigned __s8	ISRC		: 1;	/* Reads ISRC information	    */
	unsigned __s8	UPC		: 1;	/* Reads media catalog number (UPC) */
	unsigned __s8	read_bar_code	: 1;	/* Supports reading bar codes	    */
	unsigned __s8	lock		: 1;	/* PREVENT/ALLOW may lock media	    */
	unsigned __s8	lock_state	: 1;	/* Lock state 0=unlocked 1=locked   */
	unsigned __s8	prevent_jumper	: 1;	/* State of prev/allow jumper 0=pres */
	unsigned __s8	eject		: 1;	/* Ejects disc/cartr with STOP LoEj  */
	unsigned __s8	res_6_4		: 1;	/* Reserved			    */
	unsigned __s8	loading_type	: 3;	/* Loading mechanism type	    */
	unsigned __s8	sep_chan_vol	: 1;	/* Vol controls each channel separat */
	unsigned __s8	sep_chan_mute	: 1;	/* Mute controls each channel separat*/
	unsigned __s8	disk_present_rep: 1;	/* Changer supports disk present rep */
	unsigned __s8	sw_slot_sel	: 1;	/* Load empty slot in changer	    */
	unsigned __s8	side_change	: 1;	/* Side change capable		    */
	unsigned __s8	pw_in_lead_in	: 1;	/* Reads raw P-W sucode from lead in */
	unsigned __s8	res_7		: 2;	/* Reserved			    */
	unsigned __s8	max_read_speed[2];	/* Max. read speed in KB/s	    */
	unsigned __s8	num_vol_levels[2];	/* # of supported volume levels	    */
	unsigned __s8	buffer_size[2];		/* Buffer size for the data in KB   */
	unsigned __s8	cur_read_speed[2];	/* Current read speed in KB/s	    */
	unsigned __s8	res_16;			/* Reserved			    */
	unsigned __s8	res_17_0	: 1;	/* Reserved			    */
	unsigned __s8	BCK		: 1;	/* Data valid on falling edge of BCK */
	unsigned __s8	RCK		: 1;	/* Set: HIGH high LRCK=left channel  */
	unsigned __s8	LSBF		: 1;	/* Set: LSB first Clear: MSB first  */
	unsigned __s8	length		: 2;	/* 0=32BCKs 1=16BCKs 2=24BCKs 3=24I2c*/
	unsigned __s8	res_17		: 2;	/* Reserved			    */
	unsigned __s8	max_write_speed[2];	/* Max. write speed supported in KB/s*/
	unsigned __s8	cur_write_speed[2];	/* Current write speed in KB/s	    */

					/* Byte 22 ... Only in MMC-2	    */
	unsigned __s8	copy_man_rev[2];	/* Copy management revision supported*/
	unsigned __s8	res_24;			/* Reserved			    */
	unsigned __s8	res_25;			/* Reserved			    */

					/* Byte 26 ... Only in MMC-3	    */
	unsigned __s8	res_26;			/* Reserved			    */
	unsigned __s8	res_27_27	: 6;	/* Reserved			    */
	unsigned __s8	rot_ctl_sel	: 2;	/* Rotational control selected	    */
	unsigned __s8	v3_cur_write_speed[2];	/* Current write speed in KB/s	    */
	unsigned __s8	num_wr_speed_des[2];	/* # of wr speed perf descr. tables */
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
	unsigned __s8		: 8;
	unsigned __s8	format	: 3;
	unsigned __s8	gdl	: 1;
	unsigned __s8	mdl	: 1;
	unsigned __s8		: 3;
	unsigned __s8	length[2];
};

struct scsi_format_header {
	unsigned __s8	res		: 8;	/* Adaptec 5500: 1 --> format track */
	unsigned __s8	vu		: 1;	/* Vendor Unique		    */
	unsigned __s8	immed		: 1;	/* Return Immediately from Format   */
	unsigned __s8	tryout		: 1;	/* Check if format parameters OK    */
	unsigned __s8	ipattern	: 1;	/* Init patter descriptor present   */
	unsigned __s8	serr		: 1;	/* Stop on error		    */
	unsigned __s8	dcert		: 1;	/* Disable certification	    */
	unsigned __s8	dmdl		: 1;	/* Disable manufacturer defect list */
	unsigned __s8	enable		: 1;	/* Enable to use the next 3 bits    */
	unsigned __s8	length[2];		/* Length of following list in bytes*/
};

struct	scsi_def_bfi {
	unsigned __s8	cyl[3];
	unsigned __s8	head;
	unsigned __s8	bfi[4];
};

struct	scsi_def_phys {
	unsigned __s8	cyl[3];
	unsigned __s8	head;
	unsigned __s8	sec[4];
};

struct	scsi_def_list {
	struct	scsi_def_header	hd;
	union {
			unsigned __s8		list_block[1][4];
		struct	scsi_def_bfi	list_bfi[1];
		struct	scsi_def_phys	list_phys[1];
	} def_list;
};

struct	scsi_format_data {
	struct scsi_format_header hd;
	union {
			unsigned __s8		list_block[1][4];
		struct	scsi_def_bfi	list_bfi[1];
		struct	scsi_def_phys	list_phys[1];
	} def_list;
};

struct scsi_format_cap_header {
	unsigned __s8	res[3];			/* Reserved			*/
	unsigned __s8	len;			/* Len (a multiple of 8)	*/
};

struct scsi_format_cap_desc {
	unsigned __s8	nblock[4];		/* Number of blocks		*/
	unsigned __s8	desc_type	: 2;	/* Descriptor type		*/
	unsigned __s8	fmt_type	: 6;	/* Format Type			*/
	unsigned __s8	blen[3];		/* Logical block length		*/
};

struct	scsi_cap_data {
	struct scsi_format_cap_header	hd;
	struct scsi_format_cap_desc	list[1];
};

struct	scsi_send_diag_cmd {
	unsigned __s8	cmd;
	unsigned __s8	addr[4];
	unsigned __s8		: 8;
};

struct	scsi_sector_header {
	unsigned __s8	cyl[2];
	unsigned __s8	head;
	unsigned __s8	sec;
	unsigned __s8		: 5;
	unsigned __s8	rp	: 1;
	unsigned __s8	sp	: 1;
	unsigned __s8	dt	: 1;
};
////////////////////////
struct ricoh_mode_page_30 {
		MP_P_CODE;		/* parsave & pagecode */
	unsigned __s8	p_len;			/* 0xE = 14 Bytes */
	unsigned __s8	BUEFS		:1;	/* Burn-Free supported	*/
	unsigned __s8	TWBFS		:1;	/* Test Burn-Free sup.	*/
	unsigned __s8	res_2_23	:2;
	unsigned __s8	ARSCS		:1;	/* Auto read speed control supp. */
	unsigned __s8	AWSCS		:1;	/* Auto write speed control supp. */
	unsigned __s8	res_2_67	:2;
	unsigned __s8	BUEFE		:1;	/* Burn-Free enabled	*/
	unsigned __s8	res_2_13	:3;
	unsigned __s8	ARSCE		:1;	/* Auto read speed control enabled */
	unsigned __s8	AWSCD		:1;	/* Auto write speed control disabled */
	unsigned __s8	res_3_67	:2;
	unsigned __s8	link_counter[2];	/* Burn-Free link counter */
	unsigned __s8	res[10];		/* Padding up to 16 bytes */
};

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

