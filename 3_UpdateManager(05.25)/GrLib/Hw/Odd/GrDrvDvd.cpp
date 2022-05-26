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
#endif
#include 	<string.h>

#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include	<Hw/Odd/GrScsiOpr.h>
#include	<Hw/Odd/GrDrvDvd.h>
#include	<Hw/Odd/GrScsiMmc.h>
#include 	<Hw/Odd/GrDrvMmc.h>
#include 	<Hw/Odd/GrModes.h>

//====================================================================
//local const
#define	LOG_TRESH	0		/* (Current) Treshold values	*/
#define	LOG_CUMUL	1		/* (Current) Cumulative values	*/
#define	LOG_DFL_TRESH	2		/* Default Treshold values	*/
#define	LOG_DFL_CUMUL	3		/* Default Cumulatice values	*/

//====================================================================
//local type
__s8	ill_booktype[] = "reserved book type";
__s8	*book_types[] = {
	"DVD-ROM",
	"DVD-RAM",
	"DVD-R",
	"DVD-RW",
	"HD DVD-ROM",
	"HD DVD-RAM",
	"HD DVD-R",
	ill_booktype,
	ill_booktype,
	"DVD+RW",
	"DVD+R",
	ill_booktype,
	ill_booktype,
	"DVD+RW/DL",
	"DVD+R/DL",
	ill_booktype,
};

__s8	res_bvers[] = "reserved book version";
__s8	*R_vers[] = {
	"0.9x",
	"1.0x",
	"1.1x",
	res_bvers,
	"1.9x",
	"2.0x",
	"> 2.0x",
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
};

__s8	*RW_vers[] = {
	"0.9x",
	"1.0x",
	"1.1x",
	"> 1.1x",
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
	res_bvers,
};

__s8	ill_dsize[] = "illegal size";
__s8	*disc_sizes[] = {
	"120mm",
	"80mm",
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
	ill_dsize,
};

__s8	ill_rate[] = "illegal rate";
__s8	*tr_rates[] = {
	"2.52 MB/s",
	"5.04 MB/s",
	"10.08 MB/s",
	"20.16 MB/s",
	"30.24 MB/s",
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	ill_rate,
	"Not specified",
};

__s8	ill_layer[] = "illegal layer type";
__s8	*layer_types[] = {
	"Embossed Data",
	"Recordable Area",
	"Rewritable Area",
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
	ill_layer,
};

__s8	ill_dens[] = "illegal density";
__s8	*ldensities[] = {
	"0.267 ??bit",
	"0.293 ??bit",
	"0.409-0.435 ??bit",
	"0.280-0.291 ??bit",
	"0.353 ??bit",
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
};

__s8	*tdensities[] = {
	"0.74 ??track",
	"0.80 ??track",
	"0.615 ??track",
	"0.40 ??track",
	"0.34 ??track",
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
	ill_dens,
};

typedef struct scsi_log_header {	/* Log Page header		*/
	unsigned __s8	p_code	:6;		/* Page code			*/
	unsigned __s8	res	:2;		/* Reserved			*/
	unsigned __s8	res1;			/* Reserved			*/
	unsigned __s8	p_len[2];		/* Page len (n-3)		*/
} scsi_log_hdr;

typedef struct scsi_logp_header {	/* Log Parameter header		*/
	unsigned __s8	p_code[2];		/* Parameter code		*/
	unsigned __s8	lp	:1;		/* List Parameter		*/
	unsigned __s8	res2_1	:1;		/* Reserved			*/
	unsigned __s8	tmc	:2;		/* Treshold met Criteria	*/
	unsigned __s8	etc	:1;		/* Enable Treshold Comarison	*/
	unsigned __s8	tds	:1;		/* Target Save Disable		*/
	unsigned __s8	ds	:1;		/* Disble Save			*/
	unsigned __s8	du	:1;		/* Disable Update		*/
	unsigned __s8	p_len;			/* Parameter len (n-3)		*/
} scsi_logp_hdr;

struct scsi_logpage_0 {
	scsi_log_hdr	hdr;		/* Log Page header		*/
	unsigned __s8		p_code[1];	/* List of supported log pages	*/
};

struct pioneer_logpage_30_0 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned __s8		total_poh[4];	/* Total time powered on (hours)*/
};

struct pioneer_logpage_30_1 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned __s8		laser_poh[4];	/* Total time of laser turned on*/
};

struct pioneer_logpage_30_2 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned __s8		record_poh[4];	/* Total time of recording	*/
};

//adip
typedef struct adip {
	unsigned __s8	cat_vers;		/*  0	*/
	unsigned __s8	disk_size;		/*  1	*/
	unsigned __s8	disk_struct;		/*  2	*/
	unsigned __s8	density;		/*  3	*/
	unsigned __s8	data_zone_alloc[12];	/*  4	*/
	unsigned __s8	mbz_16;			/* 16	*/
	unsigned __s8	res_17[2];		/* 17	*/
	unsigned __s8	man_id[8];		/* 19	*/
	unsigned __s8	media_id[3];		/* 27	*/
	unsigned __s8	prod_revision;		/* 30	*/
	unsigned __s8	adip_numbytes;		/* 31	*/
	unsigned __s8	ref_speed;		/* 32	*/
	unsigned __s8	max_speed;		/* 33	*/
	unsigned __s8	wavelength;		/* 34	*/
	unsigned __s8	norm_write_power;	/* 35	*/
	unsigned __s8	max_read_power_ref;	/* 36	*/
	unsigned __s8	pind_ref;		/* 37	*/
	unsigned __s8	beta_ref;		/* 38	*/
	unsigned __s8	max_read_power_max;	/* 39	*/
	unsigned __s8	pind_max;		/* 40	*/
	unsigned __s8	beta_max;		/* 41	*/
	unsigned __s8	pulse[14];		/* 42	*/
	unsigned __s8	res_56[192];		/* 56	*/
	unsigned __s8	res_controldat[8];	/* 248	*/
} adip_t;

//====================================================================
//local function
__s32		log_sense(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 page, __s32 pc, __s32 pp);
BOOL8	has_log_page(St_GrScsiIf *scgp, __s32 page, __s32 pc);
__s32		get_log(St_GrScsiIf *scgp, caddr_t bp, __s32 *lenp, __s32 page, __s32 pc, __s32 pp);
void	print_logpages(St_GrScsiIf *scgp);
void	di_to_dstat(struct disk_info *dip, dstat_t *dsp);

//====================================================================
//var
static long	dvd_next_addr;
extern __s8	*driveropts;
extern __s32 	lverbose;

//====================================================================
//--------------------------------------------------------------------
cdr_t	*identify_dvd(St_GrScsiIf *scgp,	cdr_t *dp )
{
	BOOL8	dvd	 = FALSE;	/* DVD writer	*/
	struct scsi_inquiry*	ip;
	unsigned __s8	mode[0x100];
	struct	cd_mode_page_2A *mp;
	__s32	profile;

	ip	=	&scgp->inq;

	if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		return ((cdr_t *)0);

	allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */

	mp = mmc_cap(scgp, mode); /* Get MMC capabilities */
	if (mp == NULL)
		return (NULL);	/* Pre SCSI-2/mmc drive		*/

	if (ip->data_format < 2)
		ip->data_format = 2;

	dvd = mp->dvd_r_write;		/* Mode page 0x2A DVD-R writer */

	profile = get_curprofile(scgp);
	if (profile >= 0x11 && profile <= 0x19)
		dvd = TRUE;

	if (!dvd)
		get_wproflist(scgp, NULL, &dvd, NULL, NULL);

	if (!dvd)			/* Any DVD- writer		*/
		return (NULL);

	return (dp);
}
//--------------------------------------------------------------------
__s32		attach_dvd(St_GrScsiIf *scgp, cdr_t	*dp)
{
	unsigned __s8	mode[0x100];
	struct	cd_mode_page_2A *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	__u32	xspeed;
	__u32	mp2Aspeed;

	allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */

	mp = mmc_cap(scgp, NULL); /* Get MMC capabilities in allocated mp */
	if (mp == NULL)
		return (-1);	/* Pre SCSI-3/mmc drive		*/

	dp->cdr_cdcap = mp;	/* Store MMC cap pointer	*/

	dp->cdr_dstat->ds_dr_max_rspeed = a_to_u_2_byte(mp->max_read_speed)/1385;
	if (dp->cdr_dstat->ds_dr_max_rspeed == 0)
		dp->cdr_dstat->ds_dr_max_rspeed = 47;
	dp->cdr_dstat->ds_dr_cur_rspeed = a_to_u_2_byte(mp->cur_read_speed)/1385;
	if (dp->cdr_dstat->ds_dr_cur_rspeed == 0)
		dp->cdr_dstat->ds_dr_cur_rspeed = 47;

	dp->cdr_dstat->ds_dr_max_wspeed = a_to_u_2_byte(mp->max_write_speed)/1385;
	if (mp->p_len >= 28)
		dp->cdr_dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->v3_cur_write_speed)/1385;
	else
		dp->cdr_dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->cur_write_speed)/1385;

	xspeed = 0;
	scsi_get_perf_maxspeed(scgp, NULL, &xspeed, NULL);

	mp2Aspeed = a_to_u_2_byte(mp->max_write_speed);

	if ((is_cdspeed(mp2Aspeed) && !is_cdspeed(xspeed)) ||
	    (mp2Aspeed < 10000 && xspeed > 10000)) {
		dp->cdr_dstat->ds_dr_max_wspeed = (__u16)(xspeed/1385);
		xspeed = 0;
		scsi_get_perf_curspeed(scgp, NULL, &xspeed, NULL);
		if (xspeed > 0)
			dp->cdr_dstat->ds_dr_cur_wspeed = (__u16)(xspeed / 1385);
	}

	if (dp->cdr_speedmax > dp->cdr_dstat->ds_dr_max_wspeed)
		dp->cdr_speedmax = dp->cdr_dstat->ds_dr_max_wspeed;

	if (dp->cdr_speeddef > dp->cdr_speedmax)
		dp->cdr_speeddef = dp->cdr_speedmax;

	rp = get_justlink_ricoh(scgp, mode);

	if (mp->p_len >= 28)
		dp->cdr_flags |= CDR_MMC3;
	if (mp->p_len >= 24)
		dp->cdr_flags |= CDR_MMC2;
	dp->cdr_flags |= CDR_MMC;

	if (mp->loading_type == LT_TRAY)
		dp->cdr_flags |= CDR_TRAYLOAD;
	else if (mp->loading_type == LT_CADDY)
		dp->cdr_flags |= CDR_CADDYLOAD;

	if (mp->BUF != 0)
		dp->cdr_flags |= CDR_BURNFREE;

	check_writemodes_mmc(scgp, dp);
	dp->cdr_flags &= ~(CDR_RAW|CDR_RAW16|CDR_RAW96P|CDR_RAW96R|CDR_SRAW96P|CDR_SRAW96R);
	dp->cdr_flags &= ~(CDR_TAO);

	if (strbeg("PIONEER", scgp->inq.vendor_info)) 
	{
		if (strbeg("DVD-RW  DVR-103", scgp->inq.prod_ident) ||
		    strbeg("DVD-R DVD-R7322", scgp->inq.prod_ident)) 
		{
			mp->BUF = 1;
		}
	}

	if (mp->BUF != 0) {
		dp->cdr_flags |= CDR_BURNFREE;
	} else if (rp) {
		if ((dp->cdr_cmdflags & F_DUMMY) && rp->TWBFS && rp->BUEFS)
			dp->cdr_flags |= CDR_BURNFREE;

		if (rp->BUEFS)
			dp->cdr_flags |= CDR_BURNFREE;
	}

	if (rp && rp->AWSCS)
		dp->cdr_flags |= CDR_FORCESPEED;


	if ((dp->cdr_flags & (CDR_SAO)) != (CDR_SAO)) {
		dp->cdr_flags |= CDR_SAO;
	}

	/*	
	if (driveropts != NULL) 
	{
		__s8	*p;

		p = hasdrvopt(driveropts, "burnfree");
		if (p == NULL)
			p = hasdrvopt(driveropts, "burnproof");
		if (p != NULL && (dp->cdr_flags & CDR_BURNFREE) != 0) {
			if (*p == '1') {
				dp->cdr_dstat->ds_cdrflags |= RF_BURNFREE;
			} else if (*p == '0') {
				dp->cdr_dstat->ds_cdrflags &= ~RF_BURNFREE;
			}
		}

		p = hasdrvopt(driveropts, "forcespeed");
		if (p != NULL && *p == '1' && (dp->cdr_flags & CDR_FORCESPEED) != 0) {
			dp->cdr_dstat->ds_cdrflags |= RF_FORCESPEED;
		}

		p = hasdrvoptx(driveropts, "layerbreak", 0);
		if (p != NULL && *p != '\0') {
			__s8	*ep;
			__u64	ll;
			__s32		lb;

			ep = astoll(p, &ll);
			lb = ll;
			if ((*ep != '\0' && *ep != ',') ||
			    ll <= 0 || ll != lb) {

				return (-1);
			}
			dp->cdr_dstat->ds_layer_break = lb;
		} else {
			p = hasdrvopt(driveropts, "layerbreak");
			if (p != NULL && *p == '1')
				dp->cdr_dstat->ds_layer_break = 0;
		}
		if (dp->cdr_dstat->ds_layer_break >= 0 &&
		    (dp->cdr_flags & CDR_LAYER_JUMP) == 0) {

			return (-1);
		}
		if (dp->cdr_dstat->ds_layer_break != -1 &&
		    dp->cdr_dstat->ds_layer_break !=
		    roundup(dp->cdr_dstat->ds_layer_break, 16)) {

			return (-1);
		}
	}
											*/

	if (scgp->deftimeout < 100)
		scg_settimeout(scgp, 100);	/* 1:40			*/

	return (0);
}
//--------------------------------------------------------------------
void	di_to_dstat(struct disk_info *dip, dstat_t *dsp)
{
	dsp->ds_diskid = a_to_u_4_byte(dip->disk_id);

	dsp->ds_flags |= DSF_NOCD | DSF_DVD;	/* This is a DVD */

	if (dip->did_v)
		dsp->ds_flags |= DSF_DID_V;
	dsp->ds_disktype = dip->disk_type;
	dsp->ds_diskstat = dip->disk_status;
	dsp->ds_sessstat = dip->sess_status;
	if (dip->erasable)
		dsp->ds_flags |= DSF_ERA;

	dsp->ds_trfirst	   = dip->first_track;
	dsp->ds_trlast	   = dip->last_track_ls;
	dsp->ds_trfirst_ls = dip->first_track_ls;

}
//--------------------------------------------------------------------
__s32		init_dvd(St_GrScsiIf *scgp, cdr_t *dp)
{
	__s32 	Tv_Result;
	Tv_Result 	= speed_select_dvd(scgp, dp, NULL);
	return Tv_Result;
}
//--------------------------------------------------------------------
__s32		getdisktype_dvd(St_GrScsiIf *scgp, cdr_t *dp)
{
	dstat_t	*dsp = dp->cdr_dstat;
	struct disk_info *dip;
	unsigned __s8	mode[0x100];
	struct rzone_info rz;
	struct rzone_info *rp;
	struct dvd_structure_00 *sp;
	__s32	profile;
	__s32	len;
	BOOL8	did_dummy = FALSE;

	if (dsp->ds_type == DST_UNKNOWN) {
		profile = get_curprofile(scgp);
		if (profile >= 0)
			dsp->ds_type = profile;
	}

again:
		dip = (struct disk_info *)scgp->bufptr;
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);

	if (dip->disk_status == DS_COMPLETE &&
			(dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == RF_WRITE) {
		if (!did_dummy) {
			__s32	xspeed = 0xFFFF;
			__s32	oflags = dp->cdr_cmdflags;

			dp->cdr_cmdflags &= ~F_DUMMY;
			speed_select_dvd(scgp, dp, &xspeed);
			dp->cdr_cmdflags = oflags;
			did_dummy = TRUE;
			goto again;
		}
		reload_media(scgp, dp);
	}
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);
	di_to_dstat(dip, dsp);

	memset((caddr_t)mode, '\0', sizeof (mode));
	rp = (struct rzone_info *)mode;
	read_rzone_info(scgp, (caddr_t)rp, sizeof (struct rzone_info));

	if ((dp->cdr_dstat->ds_cdrflags & RF_PRATIP) != 0) 
	{
		if (((dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == 0) ) 
		{
			print_format_capacities(scgp);
		}
	}
	//movebytes(mode, (caddr_t)&rz, sizeof (struct rzone_info));
	GrDumyCopyMem( (caddr_t)&rz, mode, sizeof (struct rzone_info));
	dsp->ds_maxblocks = a_to_u_4_byte(rp->free_blocks);
	if (rp->nwa_v)
		dsp->ds_maxblocks += a_to_u_4_byte(rp->next_recordable_addr);

	
	memset((caddr_t)mode, '\0', sizeof (mode));
	if (read_dvd_structure(scgp, (caddr_t)mode, 2, 0, 0, 0, 0) < 0) {

		return (-1);
	}
	len = a_to_u_2_byte(mode);
	len += 2;			/* Data len is not included */

	if (len > sizeof (struct dvd_structure_00)) {
		len = sizeof (struct dvd_structure_00);
		len += 1;
		len &= ~1;
	}
	
	memset((caddr_t)mode, '\0', sizeof (mode));
	sp = (struct dvd_structure_00 *)mode;
	read_dvd_structure(scgp, (caddr_t)sp, len, 0, 0, 0, 0);

	if (((struct dvd_structure_00 *)sp)->book_type == 1) {
		dsp->ds_type = DST_DVD_RAM;
	} else {
		dsp->ds_type = DST_UNKNOWN;
	}
	if (get_curprofile(scgp) == 0x12) {
		dsp->ds_type = DST_DVD_RAM;
	}
	if (dsp->ds_type == DST_DVD_RAM)
		dsp->ds_maxblocks = a_to_u_4_byte(rz.rzone_size);

	if ((dp->cdr_cmdflags & F_MSINFO) == 0 &&
	    (a_to_u_3_byte(sp->phys_end) != 0) &&
			(dsp->ds_maxblocks !=
			(long)(a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1))) {

		if (dsp->ds_maxblocks == 0) {
			dsp->ds_maxblocks = a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1;
		}
	}

	return (drive_getdisktype(scgp, dp));
}
//--------------------------------------------------------------------
__s32		prdiskstatus_dvd(St_GrScsiIf *scgp, cdr_t *dp)
{
	return (prdiskstatus(scgp, dp, FALSE));
}
//--------------------------------------------------------------------
__s32		speed_select_dvd(St_GrScsiIf *scgp, cdr_t *dp, __s32 *speedp)
{
	unsigned __s8	mode[0x100];
	unsigned __s8	moder[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	__s32	val;
	__u32	ul;
	BOOL8	forcespeed = FALSE;
	BOOL8	dummy = (dp->cdr_cmdflags & F_DUMMY) != 0;
	__s32	curspeed = 1;


	if (speedp)
		curspeed = *speedp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if (dp->cdr_dstat->ds_type == DST_DVD_RAM && dummy != 0) {

		return (-1);
	}

	mp->test_write = dummy != 0;

	mp->write_type = WT_SAO;
	if (dp->cdr_dstat->ds_layer_break >= 0)
		mp->write_type = WT_LAYER_JUMP;


	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);


	if (speedp == 0)
		return (0);


	rp = get_justlink_ricoh(scgp, moder);
	if ((dp->cdr_flags & CDR_FORCESPEED) != 0) {
		forcespeed = rp && rp->AWSCD != 0;
	}

	if (!forcespeed && (dp->cdr_dstat->ds_cdrflags & RF_FORCESPEED) != 0) {
		forcespeed = TRUE;
	}
	if (forcespeed && (dp->cdr_dstat->ds_cdrflags & RF_FORCESPEED) == 0) {
		forcespeed = FALSE;
	}
	if ((dp->cdr_flags & CDR_FORCESPEED) != 0) {

		if (rp) {
			rp->AWSCD = forcespeed?1:0;
			set_mode_params(scgp, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1);
			rp = get_justlink_ricoh(scgp, moder);
		}
	}

	val = curspeed*1390;
	if (val > 0x7FFFFFFF)
		val = 0x7FFFFFFF;
	if (dp->cdr_flags & CDR_MMC3)
	{
		if (speed_select_mdvd(scgp, -1, val) < 0)
		{
			printf("MMC-3 speed select did not work.\n");
		}
	} else {
		if (val > 0xFFFF)
			val = 0xFFFF;
		if (scsi_set_speed(scgp, -1, val, ROTCTL_CLV) < 0) {

		}
	}

	val = 0;
	if (scsi_get_speed(scgp, 0, &val) >= 0) {
		if (val > 0) {
			curspeed = val / 1385;
			*speedp = curspeed;
		}
	}

	ul = 0;
	if (scsi_get_perf_curspeed(scgp, NULL, &ul, NULL) >= 0) {
		if (is_cdspeed(val) && !is_cdspeed(ul)) {
			curspeed = ul / 1385;
			*speedp = curspeed;
		}
	}

	return (0);
}
//--------------------------------------------------------------------
__s32 	session_offset_dvd(St_GrScsiIf *scgp, long *offp)
{
	__s32 	Tv_Result;
	Tv_Result 	= sessstatus(scgp, FALSE, offp, (long *)NULL);
	return Tv_Result;
}
//--------------------------------------------------------------------
__s32		next_wr_addr_dvd(St_GrScsiIf *scgp, track_t *trackp, long *ap)
{
	struct disk_info	di;
	struct rzone_info	rz;
	__s32			tracks;
	long			next_addr = -1;


	if (trackp == 0) {
		memset((caddr_t)&di, '\0', sizeof (di));
		if (get_diskinfo(scgp, &di, sizeof (di)) < 0)
			return (-1);

		tracks = di.last_track_ls + di.last_track_ls_msb * 256;
		memset((caddr_t)&rz, '\0', sizeof (rz));
		if (get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, tracks, sizeof (rz)) < 0)
			return (-1);
		if (!rz.nwa_v)
			return (-1);
		next_addr = a_to_4_byte(rz.next_recordable_addr);
		if (ap)
			*ap = next_addr;
		return (0);
	}
	if (trackp->track <= 1) {

		memset((caddr_t)&rz, '\0', sizeof (rz));
		if (track_base(trackp)->tracktype & TOCF_MULTI)
			get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, trackp->trackno, sizeof (rz));
		else
			read_rzone_info(scgp, (caddr_t)&rz, sizeof (struct rzone_info));
		dvd_next_addr = a_to_4_byte(rz.next_recordable_addr);
	}
	if (ap)
		*ap = dvd_next_addr;
	return (0);
}
//--------------------------------------------------------------------
__s32		open_track_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	long	sectors;
	struct	cd_mode_page_05 *mp;

	if (trackp->track > 1)	/* XXX Hack to make one 'track' from several */
		return (0);	/* XXX files in Disk at once mode only.	    */

	if (dp->cdr_dstat->ds_type == DST_DVD_RAM) {

		sectors = rzone_size(trackp);
		if (sectors < 0)
			return (-1);
		return (0);	/* No further setup needed */
	}

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, trackp->secsize))
		return (-1);

	sectors = rzone_size(trackp);
	if (sectors < 0)
		return (-1);
	return (reserve_tr_rzone(scgp, sectors));
}
//--------------------------------------------------------------------
long	rzone_size(track_t *trackp)
{
	__s32	i;
	BOOL8	vtracks = FALSE;
	long	sectors = 0L;
	__u64	ttrsize = 0L;
	__u64	tamount = 0L;
	__u64	amount;
	long	secsize = trackp->secsize;

	for (i = 0; i < E_GrOddRecMaxTrack; i++) {
		if (is_last(&trackp[i]))
			break;
	}
	if (i >= 1)
		vtracks = TRUE;

	for (i = 0; i < E_GrOddRecMaxTrack; i++)
	{
		if (trackp[i].tracksize < (tsize_t)0)
		{
			return (-1);
		}
		amount = ((((trackp[i].tracksize)+((secsize)-1))/(secsize))*(secsize));
		//amount = roundup(trackp[i].tracksize, secsize);
		amount += (__u64)trackp[i].padsecs * secsize;
		sectors =	sectors + (long)(amount/(__u64)secsize);
		ttrsize += trackp[i].tracksize;
		tamount += amount;
		printf("Vtrack:  %d size: %lld bytes %lld rounded (%lld sectors)\n",
				(__s32)trackp[i].track, (__u64)trackp[i].tracksize, amount, amount / (__u64)secsize);

		if (is_last(&trackp[i]))
			break;

	}

	return (sectors);
}
//--------------------------------------------------------------------
__s32		close_track_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp)
{
	long	sectors = 0L;
	__u64	amount;
	long	secsize = trackp->secsize;


	amount = roundup(trackp->tracksize, secsize);
	amount += (__u64)trackp->padsecs * secsize;
	sectors = sectors + (long)(amount/(__u64)secsize);

	dvd_next_addr += sectors;

	return (0);
}
//--------------------------------------------------------------------
__s32		open_session_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp)
{
	unsigned __s8	mode[0x100];
	unsigned __s8	moder[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	BOOL8	burnfree = FALSE;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->write_type = WT_SAO;
	if (dp->cdr_dstat->ds_layer_break >= 0)
		mp->write_type = WT_LAYER_JUMP;


	rp = get_justlink_ricoh(scgp, moder);

	if (dp->cdr_cdcap->BUF != 0) {
		burnfree = mp->BUFE != 0;
	} else if ((dp->cdr_flags & CDR_BURNFREE) != 0) {
		burnfree = rp && rp->BUEFE != 0;
	}

	if (!burnfree && (dp->cdr_dstat->ds_cdrflags & RF_BURNFREE) != 0) {
		burnfree = TRUE;
	}
	if (burnfree && (dp->cdr_dstat->ds_cdrflags & RF_BURNFREE) == 0) {
		burnfree = FALSE;
	}
	if (dp->cdr_cdcap->BUF != 0) {
		mp->BUFE = burnfree?1:0;
	} else if ((dp->cdr_flags & CDR_BURNFREE) != 0) {

		if (rp)
			rp->BUEFE = burnfree?1:0;
	}
	if (rp) {
		i_to_2_byte(rp->link_counter, 0);

		set_mode_params(scgp, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1);
		rp = get_justlink_ricoh(scgp, moder);
	}


	mp->multi_session = (track_base(trackp)->tracktype & TOCF_MULTI) ?
			MS_MULTI : MS_NONE;

	if (track_base(trackp)->tracktype & TOCF_MULTI) {
		mp->write_type = WT_PACKET;
		mp->track_mode = TM_DATA;
		mp->track_mode |= TM_INCREMENTAL;
		mp->fp = 0;
		i_to_4_byte(mp->packet_size, 0);
		mp->link_size = 16;
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32		waitformat(St_GrScsiIf *scgp, __s32 secs)
{
	__s32	i;
	__s32	key;
#define	W_SLEEP	2

	for (i = 0; i < secs/W_SLEEP; i++) {
		if (test_unit_ready(scgp) >= 0) {
			return (0);
		}
		key = scg_sense_key(scgp);
		if (key != SC_UNIT_ATTENTION && key != SC_NOT_READY)
			break;

		sleep(W_SLEEP);
	}
	return (-1);
#undef	W_SLEEP
}
//--------------------------------------------------------------------
__s32		fixate_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp)
{
	__s32	oldtimeout = scgp->deftimeout;
	__s32	ret = 0;
	__s32	trackno;


	if (scgp->deftimeout < 1000)
		scg_settimeout(scgp, 1000);

	if (scsi_flush_cache(scgp, FALSE) < 0) {

		scg_settimeout(scgp, oldtimeout);
		return (-1);
	}
	waitformat(scgp, 100);
	trackno = trackp->trackno;
	if (trackno <= 0)
		trackno = 1;

	scg_settimeout(scgp, oldtimeout);

	if (dp->cdr_dstat->ds_type == DST_DVD_RAM) {

		return (ret);
	}
	if (track_base(trackp)->tracktype & TOCF_MULTI)
		scsi_close_tr_session(scgp, CL_TYPE_SESSION, trackno, TRUE);

	return (ret);
}
//--------------------------------------------------------------------
__s32		blank_dvd(St_GrScsiIf *scgp, cdr_t *dp, long addr, __s32 blanktype)
{
	BOOL8	cdrr	 = FALSE;	/* Read CD-R	*/
	BOOL8	cdwr	 = FALSE;	/* Write CD-R	*/
	BOOL8	cdrrw	 = FALSE;	/* Read CD-RW	*/
	BOOL8	cdwrw	 = FALSE;	/* Write CD-RW	*/
	BOOL8	dvdwr	 = FALSE;	/* DVD writer	*/
	__s32	profile;
	extern __s8 *blank_types[];

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);

	profile = get_curprofile(scgp);
	if (profile > 0)
		dvdwr = (profile == 0x13) || (profile == 0x14) ||
			(profile == 0x17);

	if (profile == 0x12)	/* DVD-RAM */
		return (blank_simul(scgp, dp, addr, blanktype));

	if (!dvdwr)
		return (blank_dummy(scgp, dp, addr, blanktype));

	/*
	if (lverbose) {
		flush();
	}
	*/

	return (scsi_blank(scgp, addr, blanktype, FALSE));
}
//--------------------------------------------------------------------
__s32		stats_dvd(St_GrScsiIf *scgp, cdr_t *dp)
{
	unsigned __s8 mode[256];
	struct	ricoh_mode_page_30 *rp;
	__u32 count;

	if ((dp->cdr_dstat->ds_cdrflags & RF_BURNFREE) == 0)
		return (0);

	rp = get_justlink_ricoh(scgp, mode);
	if (rp) {
		count = a_to_u_2_byte(rp->link_counter);
	}
	return (0);
}
//--------------------------------------------------------------------
__s32		set_layerbreak(St_GrScsiIf *scgp, long	tsize, __u32 lbreak)
{
	struct dvd_structure_23	lb;
	__s32			ret;
	__u32		dsize;
	__u32		jump_lba;

	memset((caddr_t)&lb, '\0', sizeof (lb));

	ret = read_dvd_structure(scgp, (caddr_t)&lb, sizeof (lb), 0, 0, 0, 0x23);
	if (ret < 0)
		return (ret);

	jump_lba = a_to_u_4_byte(lb.jump_lba);
	if (lbreak > 0 && lbreak > jump_lba) {
		return (-1);
	}
	dsize = roundup(tsize, 16);
	if (lbreak <= 0 && dsize <= (jump_lba+1)) {

		return (-1);
	}
	jump_lba = dsize / 2;
	jump_lba = roundup(jump_lba, 16);
	if (lbreak > 0 && lbreak < jump_lba) {
		return (-1);
	}
	if (lbreak > 0)
		jump_lba = lbreak;
	jump_lba -= 1;
	i_to_4_byte(lb.jump_lba, jump_lba);

	ret = send_dvd_structure(scgp, (caddr_t)&lb, sizeof (lb), 0x23);
	return (ret);
}
//--------------------------------------------------------------------
void	print_dvd00(struct dvd_structure_00 *dp)
{

}
//--------------------------------------------------------------------
void	print_dvd01(struct dvd_structure_01 *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd04(struct dvd_structure_04 *dp)
{
	if (cmpnullbytes(dp->man_info, sizeof (dp->man_info)) <
						sizeof (dp->man_info)) {
		printf("Manufacturing info: '%.2048s'\n", dp->man_info);
	}
}
//--------------------------------------------------------------------
void	print_dvd05(struct dvd_structure_05 *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd0D(struct dvd_structure_0D *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd0E(struct dvd_structure_0E *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd0F(struct dvd_structure_0F *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd20(struct dvd_structure_20 *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd22(struct dvd_structure_22 *dp)
{
}
//--------------------------------------------------------------------
void	print_dvd23(struct dvd_structure_23 *dp)
{
}
//--------------------------------------------------------------------
void	send_dvd0F(St_GrScsiIf *scgp)
{
	struct dvd_structure_0F_w d;

	strncpy((__s8 *)d.year,		"1998", 4);
	strncpy((__s8 *)d.month,	"05", 2);
	strncpy((__s8 *)d.day,		"12", 2);
	strncpy((__s8 *)d.hour,		"22", 2);
	strncpy((__s8 *)d.minute,	"59", 2);
	strncpy((__s8 *)d.second,	"00", 2);
/*	send_dvd_structure(scgp, (caddr_t)&d, sizeof (d));*/
}
//--------------------------------------------------------------------
void	print_dvd_info(St_GrScsiIf *scgp)
{
	unsigned __s8	mode[4096];
	__s32	ret;
	//__s32	i;

	/*
	if (lverbose > 1)
		mode_sense(scgp, mode, 250, 0x3F, 0);
	*/

	wait_unit_ready(scgp, 120);
	wait_unit_ready(scgp, 120);

	/*
	 * Physical Format information 0x00
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0);
	if (ret >= 0) {
		
		print_dvd00((struct dvd_structure_00 *)mode);
		ret = get_curprofile(scgp);
		if (ret == 0x001A || ret == 0x001B) {
			/*profile >= 0x0018 && profile < 0x0020*/
			printf("Manufacturer:    '%.8s'\n", &mode[23]);
			printf("Media type:      '%.3s'\n", &mode[23+8]);
		}
	}

	/*
	 * ADIP information 0x11
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0x11);
	if (ret >= 0) {
		adip_t	*adp;

/*		print_dvd0F((struct dvd_structure_0F *)mode);*/
		adp = (adip_t *)&mode[4];
/*		printf("size %d %d\n", sizeof (adip_t), offsetof(adip_t, res_controldat));*/
		printf("Category/Version	%02X\n", adp->cat_vers);
		printf("Disk size		%02X\n", adp->disk_size);
		printf("Disk structure		%02X\n", adp->disk_struct);
		printf("Recoding density	%02X\n", adp->density);

		printf("Manufacturer:		'%.8s'\n", adp->man_id);
		printf("Media type:		'%.3s'\n", adp->media_id);
		printf("Product revision	%u\n", adp->prod_revision);
		printf("ADIP numbytes		%u\n", adp->adip_numbytes);
		printf("Reference speed		%u\n", adp->ref_speed);
		printf("Max speed		%u\n", adp->max_speed);
	}

	/*
	 * Layer boundary information 0x20
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0x20);
	if (ret >= 0) {
		print_dvd20((struct dvd_structure_20 *)mode);
	}

	/*
	 * Jump interval size 0x22
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0x22);
	if (ret >= 0) {
		print_dvd22((struct dvd_structure_22 *)mode);
	}

	/*
	 * Jump logical block address 0x23
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0x23);
	if (ret >= 0) {
		print_dvd23((struct dvd_structure_23 *)mode);
	}

	/*
	 * Copyright information 0x01
	 */
	memset((caddr_t)mode, '\0',sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 1);
	if (ret >= 0) {
		print_dvd01((struct dvd_structure_01 *)mode);
	}


	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 4);
	if (ret >= 0) {
		print_dvd04((struct dvd_structure_04 *)mode);
	}

	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 5);
	if (ret >= 0) {
		print_dvd05((struct dvd_structure_05 *)mode);
	}

	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0xD);
	if (ret >= 0) {
		print_dvd0D((struct dvd_structure_0D *)mode);
	}

	/*
	 * Prerecorded information 0x0E
	 */
	memset((caddr_t)mode, '\0', sizeof (mode));
	ret = read_dvd_structure(scgp, (caddr_t)mode, sizeof (mode), 0, 0, 0, 0xE);
	if (ret >= 0) {

		print_dvd0E((struct dvd_structure_0E *)mode);
	}

}
//--------------------------------------------------------------------
void	print_laserlog(St_GrScsiIf *scgp)
{
	unsigned __s8 log[256];
	unsigned __s8 *p;
	__s32	len = sizeof (log);
	long	val;

	if (!has_log_page(scgp, 0x30, LOG_CUMUL))
		return;

	p = log + sizeof (scsi_log_hdr);

	memset((caddr_t)log, '\0', sizeof (log));
	if (get_log(scgp, (caddr_t)log, &len, 0x30, LOG_CUMUL, 0) < 0) {
		return;
	}

	val = a_to_u_4_byte(((struct pioneer_logpage_30_0 *)p)->total_poh);
	if (((struct scsi_logp_header *)log)->p_len > 0)
		printf("Total power on  hours: %ld\n", val);

	memset((caddr_t)log, '\0', sizeof (log));
	if (get_log(scgp, (caddr_t)log, &len, 0x30, LOG_CUMUL, 1) < 0) {
		return;
	}

	val = a_to_u_4_byte(((struct pioneer_logpage_30_1 *)p)->laser_poh);
	if (((struct scsi_logp_header *)log)->p_len > 0)
		printf("Total laser on  hours: %ld\n", val);

	memset((caddr_t)log, '\0', sizeof (log));
	if (get_log(scgp, (caddr_t)log, &len, 0x30, LOG_CUMUL, 2) < 0) {
		return;
	}

	val = a_to_u_4_byte(((struct pioneer_logpage_30_2 *)p)->record_poh);
	if (((struct scsi_logp_header *)log)->p_len > 0)
		printf("Total recording hours: %ld\n", val);
}
//--------------------------------------------------------------------
__s32		log_sense(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 page, __s32 pc, __s32 pp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset(bp, '\0', cnt);
	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x4D;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.addr[0] = (pc << 6) | (page & 0x3f);
	i_to_2_byte(&scmd->cdb.cmd_cdb[5], pp);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "log sense";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
BOOL8	has_log_page(St_GrScsiIf *scgp, __s32 page, __s32 pc)
{
	unsigned __s8	log[0x3F + sizeof (scsi_log_hdr)];
	__s32	len;
	__s32	i;
	struct scsi_logpage_0 *sp = (struct scsi_logpage_0 *)log;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), 0, pc, 0) < 0) {
		return (FALSE);
	}

	len = a_to_u_2_byte(sp->hdr.p_len);

	if (log_sense(scgp, (caddr_t)log, len + sizeof (scsi_log_hdr), 0, pc, 0) < 0) {
		return (FALSE);
	}

	len -= scg_getresid(scgp);

	for (i = 0; i < len; i++) {
		if (page == sp->p_code[i])
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
__s32		get_log(St_GrScsiIf *scgp, caddr_t bp, __s32 *lenp, __s32 page, __s32 pc, __s32 pp)
{
	unsigned __s8		log[sizeof (scsi_log_hdr)];
	scsi_log_hdr	*hp = (scsi_log_hdr *)log;
	__s32		maxlen = *lenp;
	__s32		len;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), page, pc, pp) < 0)
		return (-1);
	len = a_to_u_2_byte(hp->p_len);
	*lenp = len + sizeof (scsi_log_hdr);
	if ((len + (__s32)sizeof (scsi_log_hdr)) > maxlen)
		len = maxlen - sizeof (scsi_log_hdr);

	if (log_sense(scgp, bp, len + sizeof (scsi_log_hdr), page, pc, pp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
void	print_logpages(St_GrScsiIf *scgp)
{
	unsigned __s8	log[0x3F + sizeof (scsi_log_hdr)];
	__s32	len;
	__s32	i;
	struct scsi_logpage_0 *sp = (struct scsi_logpage_0 *)log;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), 0, LOG_CUMUL, 0) < 0) {
		return;
	}

	len = a_to_u_2_byte(sp->hdr.p_len);

	if (log_sense(scgp, (caddr_t)log, len + sizeof (scsi_log_hdr), 0, LOG_CUMUL,  0) < 0) {
		return;
	}

	len -= scg_getresid(scgp);

	printf("Supported log pages:");

	for (i = 0; i < len; i++) {
		printf(" %X", sp->p_code[i]);
	}
	printf("\n");
}
//--------------------------------------------------------------------

