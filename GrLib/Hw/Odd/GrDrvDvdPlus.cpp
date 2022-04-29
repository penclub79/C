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

#include	<Hw/Odd/GrScsiOpr.h>
#include	<Hw/Odd/GrDrvDvdPlus.h>
#include	<Hw/Odd/GrScsiMmc.h>
#include 	<Hw/Odd/GrDrvMmc.h>
#include 	<Hw/Odd/GrModes.h>
#include 	<Hw/Odd/GrScsiCdr.h>

//====================================================================
//local const
#define	CHANGE_SETTING	1
#define	DVD_PLUS_R	2
#define	DC_ERASE	4

#define	FMTDAT	0x10
#define	CMPLST	0x08

//====================================================================
//local type

//====================================================================
//local function
static cdr_t	*identify_dvdplus(St_GrScsiIf *scgp, cdr_t * );
static __s32	attach_dvdplus(St_GrScsiIf *scgp, cdr_t *);
static void	di_to_dstat(struct disk_info *dip, dstat_t *dsp);
static __s32	init_dvdplus(St_GrScsiIf *scgp, cdr_t *dp);
static __s32	getdisktype_dvdplus(St_GrScsiIf *scgp, cdr_t *dp);
static __s32	prdiskstatus_dvdplus(St_GrScsiIf *scgp, cdr_t *dp);
static __s32	speed_select_dvdplus(St_GrScsiIf *scgp, cdr_t *dp, __s32 *speedp);
static __s32	next_wr_addr_dvdplusrw(St_GrScsiIf *scgp, track_t *trackp, long *ap);
static __s32	next_wr_addr_dvdplusr(St_GrScsiIf *scgp, track_t *trackp, long *ap);
static __s32	open_track_dvdplus(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
static long	rzone_size(track_t *trackp);
static __s32	close_track_dvdplus(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
static __s32	fixate_dvdplusrw(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
static __s32	fixate_dvdplusr(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
static BOOL8	dvdplus_ricohbased(St_GrScsiIf *scgp);
static __s32	blank_dvdplus(St_GrScsiIf *scgp, cdr_t *dp, long addr, __s32 blanktype);
static __s32	format_dvdplus(St_GrScsiIf *scgp, cdr_t *dp, __s32 fmtflags);
static __s32	waitformat(St_GrScsiIf *scgp, __s32 secs);
static __s32	stats_dvdplus(St_GrScsiIf *scgp, cdr_t *dp);

static __s32	set_p_layerbreak(St_GrScsiIf *scgp, long tsize, __u32 lbreak);

static __s32	ricoh_dvdsetting(St_GrScsiIf *scgp, __s32 erase_size, __s32 flags, __s32 immed);

//====================================================================
//var
cdr_t	cdr_dvdplus = {
	0, 0, 0,
	CDR_DVD|CDR_SWABAUDIO,
	CDR2_NOCD,
	CDR_CDRW_ALL,
	WM_NONE,
	1000, 1000,
	"mmc_dvdplus",
	"generic SCSI-3/mmc-3 DVD+R/DVD+RW driver (checks media)",
	0,
	(dstat_t *)0,
	identify_dvdplus,
	attach_dvdplus,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *))cmd_ill,	/* init */
	drive_getdisktype,
	no_diskstatus,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,					/* recovery_needed */
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,	/* recover	*/
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, __s32 *))cmd_dummy,		/* Speed */
	select_secsize,
	(__s32(*)(St_GrScsiIf *scgp, track_t *, long *))cmd_ill,	/* next_wr_addr	 */
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,			/* reserve_track */
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,	/* gen_cue */
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,		/* leadin */
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* open track */
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* close track */
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* open session */
	cmd_dummy,					/* close session */
	cmd_dummy,					/* abort */
	read_session_offset,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* fixation */
	cmd_dummy,					/* stats	*/
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,	/* no OPC	*/
	cmd_dummy,					/* opt1		*/
	cmd_dummy,					/* opt2		*/
};

/*
 * SCSI-3/mmc-3 conformant DVD+R writer
 */
cdr_t	cdr_dvdplusr = {
	0, 0, 0,
	CDR_DVD|CDR_SWABAUDIO,
	CDR2_NOCD,
	CDR_CDRW_ALL,
	WM_SAO,
	1000, 1000,
	"mmc_dvdplusr",
	"generic SCSI-3/mmc-3 DVD+R driver",
	0,
	(dstat_t *)0,
	identify_dvdplus,
	attach_dvdplus,
	init_dvdplus,
	getdisktype_dvdplus,
	prdiskstatus_dvdplus,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,					/* recovery_needed */
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,	/* recover	*/
	speed_select_dvdplus,
	select_secsize,
	next_wr_addr_dvdplusr,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,		/* reserve_track */
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,	/* gen_cue */
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy, /* leadin */
	open_track_dvdplus,
	close_track_dvdplus,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* open session */
	cmd_dummy,					/* close session */
	cmd_dummy,					/* abort	*/
	read_session_offset,
	fixate_dvdplusr,
	stats_dvdplus,
	blank_dvdplus,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,	/* no OPC	*/
	cmd_dummy,					/* opt1		*/
	cmd_dummy,					/* opt2		*/
};

/*
 * SCSI-3/mmc-3 conformant DVD+RW writer
 */
cdr_t	cdr_dvdplusrw = {
	0, 0, 0,
	CDR_DVD|CDR_SWABAUDIO,
	CDR2_NOCD,
	CDR_CDRW_ALL,
	WM_SAO,
	1000, 1000,
	"mmc_dvdplusrw",
	"generic SCSI-3/mmc-3 DVD+RW driver",
	0,
	(dstat_t *)0,
	identify_dvdplus,
	attach_dvdplus,
	init_dvdplus,
	getdisktype_dvdplus,
	prdiskstatus_dvdplus,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,					/* recovery_needed */
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,	/* recover	*/
	speed_select_dvdplus,
	select_secsize,
	next_wr_addr_dvdplusrw,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,		/* reserve_track */
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,	/* gen_cue */
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy, /* leadin */
	open_track_dvdplus,
	close_track_dvdplus,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,	/* open session */
	cmd_dummy,					/* close session */
	cmd_dummy,					/* abort	*/
	read_session_offset,
	fixate_dvdplusrw,
	stats_dvdplus,
	blank_dvdplus,
	format_dvdplus,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,	/* no OPC	*/
	cmd_dummy,					/* opt1		*/
	cmd_dummy,					/* opt2		*/
};

static long	dvd_next_addr;

//====================================================================
//--------------------------------------------------------------------
cdr_t *	identify_dvdplus(St_GrScsiIf *scgp, cdr_t *dp )
{
	unsigned __s8	mode[0x100];
	struct	cd_mode_page_2A *mp;
	__s32	profile;
	struct scsi_inquiry*	ip;

	ip	=	&scgp->inq;

	if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		return ((cdr_t *)0);

	allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */

	mp = mmc_cap(scgp, mode); /* Get MMC capabilities */
	if (mp == NULL)
		return (NULL);	/* Pre St_GrScsiIf-3/mmc drive		*/

	if (ip->data_format < 2)
		ip->data_format = 2;

	profile = get_curprofile(scgp);

	if (profile == 0x001A)
		dp = &cdr_dvdplusrw;
	if ((profile == 0x001B) || (profile == 0x002B))
		dp = &cdr_dvdplusr;

	return (dp);
}
//--------------------------------------------------------------------
__s32	attach_dvdplus(St_GrScsiIf	*scgp, cdr_t *dp)
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
		dp->cdr_dstat->ds_dr_max_wspeed = xspeed/1385;
		xspeed = 0;
		scsi_get_perf_curspeed(scgp, NULL, &xspeed, NULL);
		if (xspeed > 0)
			dp->cdr_dstat->ds_dr_cur_wspeed = xspeed / 1385;
	}

	if (dp->cdr_speedmax > dp->cdr_dstat->ds_dr_max_wspeed)
		dp->cdr_speedmax = dp->cdr_dstat->ds_dr_max_wspeed;

	if (dp->cdr_speeddef > dp->cdr_speedmax)
		dp->cdr_speeddef = dp->cdr_speedmax;

	rp = get_justlink_ricoh(scgp, mode);

	if (mp->p_len >= 28)
		dp->cdr_flags |= CDR_MMC3;
	if (get_curprofile(scgp) >= 0)
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
		if (p != NULL) {
			__s8	*ep;
			__u64	ll;
			__u32	lb;

			ep = astoll(p, &ll);
			lb = ll;
			if ((*ep != '\0' && *ep != ',') || *p == '\0' ||
			    ll <= 0 || ll != lb) {
				return (-1);
			}
			dp->cdr_dstat->ds_layer_break = lb;
		}
		if (dp->cdr_dstat->ds_layer_break >= 0 &&
		    get_curprofile(scgp) != 0x2B) {
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
void	di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp)
{
	dsp->ds_diskid = a_to_u_4_byte(dip->disk_id);

	dsp->ds_flags |= DSF_NOCD|DSF_DVD;	/* This is a DVD */

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
__s32	init_dvdplus(St_GrScsiIf	*scgp, cdr_t	*dp)
{
	return (speed_select_dvdplus(scgp, dp, NULL));
}
//--------------------------------------------------------------------
__s32	getdisktype_dvdplus(St_GrScsiIf *scgp,	cdr_t	*dp)
{
	//extern	__s8	*buf;
	dstat_t	*dsp = dp->cdr_dstat;
	struct disk_info *dip;
	unsigned __s8	mode[0x100];
	struct rzone_info *rp;
	struct dvd_structure_00 *sp;
	__s32	len;
	BOOL8	did_dummy = FALSE;
	BOOL8	did_reload = FALSE;
	__s32	profile;
	__u32	end_lba;

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

		profile = get_curprofile(scgp);
		if (profile == 0x001A) {	/* This is a DVD+RW */
			if (dp->cdr_cmdflags & F_FIX)
				return (0);
		}

		if (!did_dummy) {
			__s32	xspeed = 0xFFFF;
			__s32	oflags = dp->cdr_cmdflags;

			dp->cdr_cmdflags &= ~F_DUMMY;
			speed_select_dvdplus(scgp, dp, &xspeed);
			dp->cdr_cmdflags = oflags;
			rezero_unit(scgp);
			did_dummy = TRUE;
			goto again;
		}
		if (!did_reload && profile == 0x001A) {	/* This is a DVD+RW */
			len = read_scsi(scgp, (caddr_t)scgp->bufptr, 0, 1);
			if (len < 0) {

				reload_media(scgp, dp);
				did_reload = TRUE;
				goto again;
			}
		}

	}
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);
	di_to_dstat(dip, dsp);

	profile = get_curprofile(scgp);
	if (profile == 0x001A) {
		dsp->ds_flags |= DSF_DVD_PLUS_RW;	/* This is a DVD+RW */
		if (dip->disk_status == DS_EMPTY) {	/* Unformatted	    */

			dsp->ds_flags |= DSF_NEED_FORMAT;
			if ((dp->cdr_dstat->ds_cdrflags & RF_PRATIP) != 0)
				print_format_capacities(scgp);
			return (0);
		}
	} else if (profile == 0x001B) {
		dsp->ds_flags |= DSF_DVD_PLUS_R;	/* This is a DVD+R */
	}

	memset((caddr_t)mode, '\0', sizeof (mode));
	rp = (struct rzone_info *)mode;
	read_rzone_info(scgp, (caddr_t)rp, sizeof (struct rzone_info));

	if ((dp->cdr_dstat->ds_cdrflags & RF_PRATIP) != 0) {
		if (((dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == 0) ) 
		{
			print_format_capacities(scgp);
		}
	}

	dsp->ds_maxblocks = a_to_u_4_byte(rp->rzone_size);

	if (dsp->ds_maxblocks == 0)
		dsp->ds_maxblocks = a_to_u_4_byte(rp->free_blocks);

	if (rp->nwa_v)
		dsp->ds_maxblocks += a_to_u_4_byte(rp->next_recordable_addr);

	if (dip->disk_status == DS_COMPLETE)
		return (drive_getdisktype(scgp, dp));

	memset((caddr_t)mode, '\0', sizeof (mode));
	if (read_dvd_structure(scgp, (caddr_t)mode, 2, 0, 0, 0, 0) < 0) {

		return (drive_getdisktype(scgp, dp));
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
	if (read_dvd_structure(scgp, (caddr_t)sp, len, 0, 0, 0, 0) < 0)
		return (drive_getdisktype(scgp, dp));

	if ((profile != 0x001A) &&
	    (dp->cdr_cmdflags & F_MSINFO) == 0 &&
	    (a_to_u_3_byte(sp->phys_end) != 0) &&
			(dsp->ds_maxblocks !=
			(long)(a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1))) {

		if ((long)dsp->ds_maxblocks == 0) {
			printf("WARNING: Drive returns zero media size. Using media size from ADIP.\n");
			dsp->ds_maxblocks = a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1;
		}
	}

	end_lba = 0L;
	scsi_get_perf_maxspeed(scgp, (__u32 *)NULL, (__u32 *)NULL, &end_lba);

	if ((__u32)end_lba > dsp->ds_maxblocks)
		dsp->ds_maxblocks = end_lba;

	return (drive_getdisktype(scgp, dp));
}
//--------------------------------------------------------------------
__s32	prdiskstatus_dvdplus(St_GrScsiIf	*scgp, cdr_t	*dp)
{
	return (prdiskstatus(scgp, dp, FALSE));
}
//--------------------------------------------------------------------
__s32	speed_select_dvdplus(St_GrScsiIf	*scgp, cdr_t	*dp,	__s32	*speedp)
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


	if (dummy != 0) {
		return (-1);
	}
	if (get_curprofile(scgp) == 0x001A) {	/* This is a DVD+RW */
		if (dp->cdr_cmdflags & F_FIX)
			return (0);
	}
	mp->test_write = dummy != 0;

	mp->write_type = WT_SAO;

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		return (-1);
	}

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
	if (dp->cdr_flags & CDR_MMC3) {
		if (speed_select_mdvd(scgp, -1, val) < 0)
			printf("MMC-3 speed select did not work.\n");
	} else {
		if (val > 0xFFFF)
			val = 0xFFFF;
		if (scsi_set_speed(scgp, -1, val, ROTCTL_CLV) < 0) {

		}
	}

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
__s32	next_wr_addr_dvdplusrw(St_GrScsiIf	*scgp, track_t	*trackp,	long	*ap)
{
	if (trackp == 0 || trackp->track <= 1) {
		dvd_next_addr = 0;
	}
	if (ap)
		*ap = dvd_next_addr;
	return (0);
}

//--------------------------------------------------------------------
__s32	next_wr_addr_dvdplusr(St_GrScsiIf	*scgp, track_t	*trackp,	long	*ap)
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
		read_rzone_info(scgp, (caddr_t)&rz, sizeof (struct rzone_info));
		dvd_next_addr = a_to_4_byte(rz.next_recordable_addr);
	}
	if (ap)
		*ap = dvd_next_addr;
	return (0);
}
//--------------------------------------------------------------------
__s32	open_track_dvdplus(St_GrScsiIf	*scgp, cdr_t *dp, track_t *trackp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	long	sectors;
	struct	cd_mode_page_05 *mp;
	__s32	profile;

	if (trackp->track > 1)	/* XXX Hack to make one 'track' from several */
		return (0);	/* XXX files in Disk at once mode only.	    */

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

	profile = get_curprofile(scgp);
	if (profile == 0x2B) {
		if (set_p_layerbreak(scgp, sectors,
				dp->cdr_dstat->ds_layer_break) < 0) {
			return (-1);
		}
	}

	return (0);
}
//--------------------------------------------------------------------
long	rzone_size(track_t *trackp)
{
	__s32	i;
	BOOL8	vtracks = FALSE;
	long	sectors = 0L;
	__u32	ttrsize = 0L;
	__u32	tamount = 0L;
	__u32	amount;
	long	secsize = trackp->secsize;

	for (i = 0; i < E_GrOddRecMaxTrack; i++) {
		if (is_last(&trackp[i]))
			break;
	}
	if (i >= 1)
		vtracks = TRUE;

	for (i = 0; i < E_GrOddRecMaxTrack; i++) {
		if (trackp[i].tracksize < (tsize_t)0) {
			printf("VTrack %d has unknown length.\n", i);
			return (-1);
		}
		amount = roundup(trackp[i].tracksize, secsize);
		amount += (__u32)trackp[i].padsecs * secsize;
		sectors += amount/secsize;
		ttrsize += trackp[i].tracksize;
		tamount += amount;

		if (is_last(&trackp[i]))
			break;

		if (trackp[i].tracksize % secsize) {
			printf("Virtual track %d is not a multiple of secsize.\n", (__s32)trackp[i].track);
		}
	}

	/*
	if (vtracks && lverbose)
		printf("Vtracks: %d size: %lld bytes %lld rounded (%ld sectors) total\n",
			i+1, ttrsize, tamount, sectors);
	*/

	return (sectors);
}
//--------------------------------------------------------------------
__s32	close_track_dvdplus(St_GrScsiIf *scgp,	cdr_t	*dp, track_t *trackp)
{
	long	sectors = 0L;
	__u32	amount;
	long	secsize = trackp->secsize;

	amount = roundup(trackp->tracksize, secsize);
	amount += (__u32)trackp->padsecs * secsize;
	sectors += amount/secsize;

	dvd_next_addr += sectors;

	return (0);
}
//--------------------------------------------------------------------
__s32	fixate_dvdplusrw(St_GrScsiIf	*scgp, cdr_t *dp, track_t	*trackp)
{
	__s32	oldtimeout = scgp->deftimeout;
	__s32	ret = 0;
	__s32	i;
#define	MAX_TRIES	15

	if (scgp->deftimeout < 1000)
		scg_settimeout(scgp, 1000);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_flush_cache(scgp, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {

				scg_settimeout(scgp, oldtimeout);
				return (-1);
			}
			sleep(1);
		} else {
			break;
		}
	}
	waitformat(scgp, 300);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_close_tr_session(scgp, CL_TYPE_SESSION, 0, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {
				break;
			}
			sleep(1);
		} else {
			break;
		}
	}
	waitformat(scgp, 300);

	scg_settimeout(scgp, oldtimeout);
	return (ret);
#undef	MAX_TRIES
}
//--------------------------------------------------------------------
__s32	fixate_dvdplusr(St_GrScsiIf *scgp,	cdr_t	*dp, track_t *trackp)
{
	__s32	oldtimeout = scgp->deftimeout;
	__s32	ret = 0;
	__s32	key = 0;
	__s32	code = 0;
	__s32	trackno;
	__s32	i;
#define	MAX_TRIES	15

	if (scgp->deftimeout < 1000)
		scg_settimeout(scgp, 1000);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_flush_cache(scgp, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {

				scg_settimeout(scgp, oldtimeout);
				return (-1);
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scg_sense_key(scgp);
	code = scg_sense_code(scgp);
	waitformat(scgp, 600);

	trackno = trackp->trackno;
	if (trackno <= 0)
		trackno = 1;
	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_close_tr_session(scgp, CL_TYPE_TRACK, trackno, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {
				break;
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scg_sense_key(scgp);
	code = scg_sense_code(scgp);
	waitformat(scgp, 600);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_close_tr_session(scgp, 0x06, 0, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {

				break;
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scg_sense_key(scgp);
	code = scg_sense_code(scgp);
	waitformat(scgp, 600);

	scg_settimeout(scgp, oldtimeout);
	return (ret);
#undef	MAX_TRIES
}
//--------------------------------------------------------------------
BOOL8	dvdplus_ricohbased(St_GrScsiIf	*scgp)
{
	if (strncmp(scgp->inq.vendor_info, "RICOH", 5) == 0) {
		if (strbeg("DVD+RW MP5120", scgp->inq.prod_ident) ||
		    strbeg("DVD+RW MP5125", scgp->inq.prod_ident))
			return (TRUE);
	}
	if (strncmp(scgp->inq.vendor_info, "HP", 2) == 0) {
		if (strbeg("DVD Writer 100j", scgp->inq.prod_ident) ||
		    strbeg("DVD Writer 200j", scgp->inq.prod_ident))
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
__s32	blank_dvdplus(St_GrScsiIf *scgp,	cdr_t	*dp, long	addr,	__s32	blanktype)
{
	//extern __s8 *blank_types[];

	BOOL8	cdrr	 = FALSE;	/* Read CD-R	*/
	BOOL8	cdwr	 = FALSE;	/* Write CD-R	*/
	BOOL8	cdrrw	 = FALSE;	/* Read CD-RW	*/
	BOOL8	cdwrw	 = FALSE;	/* Write CD-RW	*/
	BOOL8	dvdwr	 = FALSE;	/* DVD writer	*/
	__s32	profile;

	__s32	flags = DC_ERASE;
	__s32	blanksize = 0x30000;
	__s32	ret;

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);

	profile = get_curprofile(scgp);

	if (!dvdplus_ricohbased(scgp)) {

		if (profile == 0x1A || profile == 0x2A) {
			ret = blank_simul(scgp, dp, addr, blanktype);
			waitformat(scgp, 600);
			scsi_flush_cache(scgp, TRUE);
			waitformat(scgp, 600);
			return (ret);
		}
		return (-1);
	}


	if (blanktype == BLANK_DISC) {
		blanksize = 0x26c090;
	} else {
		blanktype = BLANK_MINIMAL;
		blanksize = 0x30000;
	}
	/*
	if (lverbose) {

		flush();
	}
	*/
	/*
	if (driveropts != NULL) 
	{
		__s8	*p;

		p = hasdrvopt(driveropts, "erase");
		if (p == NULL)
			p = hasdrvopt(driveropts, "blank");
		if (p != NULL) {
			if (*p == '1')
				flags |= DC_ERASE;
			else
				flags &= ~DC_ERASE;
		}
		p = hasdrvopt(driveropts, "set");
		if (p != NULL) {
			if (*p == '1')
				flags |= CHANGE_SETTING;
			else
				flags &= ~CHANGE_SETTING;
		}
		p = hasdrvopt(driveropts, "dvdr");
		if (p != NULL) {
			if (*p == '1')
				flags |= DVD_PLUS_R;
			else
				flags &= ~DVD_PLUS_R;
		}
	}
	*/

	ret = ricoh_dvdsetting(scgp, blanksize, flags, TRUE);
	waitformat(scgp, 2000);
	return (ret);
}
//--------------------------------------------------------------------
__s32	format_dvdplus(St_GrScsiIf	*scgp, cdr_t *dp,	__s32	fmtflags)
{
	__s8	cap_buf[4096];
	__s8	fmt_buf[128];
	__s32	len;
	__s32	i;
	struct scsi_cap_data		*cp;
	struct scsi_format_cap_desc	*lp;
	struct scsi_format_header	*fhp;
	long	blocks = 0;
	struct timeval starttime;
	struct timeval stoptime;

	len = get_format_capacities(scgp, cap_buf, sizeof (cap_buf));
	if (len < 0)
		return (-1);

	cp = (struct scsi_cap_data *)cap_buf;
	lp = cp->list;
	len -= sizeof (struct scsi_format_cap_header);
	if (lp->desc_type == 2) {
		if ((dp->cdr_cmdflags & F_FORCE) == 0) {
			return (-1);
		}
	}

	for (i = len; i >= sizeof (struct scsi_format_cap_desc);
			i -= sizeof (struct scsi_format_cap_desc), lp++) {

		if (lp->fmt_type == FCAP_TYPE_DVDPLUS_FULL)
			blocks = a_to_u_4_byte(lp->nblock);
	}
	if (blocks == 0) {

		return (-1);
	}

	fhp = (struct scsi_format_header *)fmt_buf;
	lp = (struct scsi_format_cap_desc *)&fmt_buf[4];
	memset((caddr_t)fmt_buf, '\0', sizeof (fmt_buf));

	fhp->enable = 1;
	fhp->immed = 1;
	i_to_2_byte(fhp->length, 8);
	i_to_4_byte(lp->nblock, blocks);
	lp->fmt_type = FCAP_TYPE_DVDPLUS_FULL;
	i_to_3_byte(lp->blen, 0);

	/*
	if (lverbose) {
		flush();
	}
	*/
	starttime.tv_sec = 0;
	starttime.tv_usec = 0;
	stoptime = starttime;
	gettimeofday(&starttime, (struct timezone *)0);

	if (format_unit(scgp, fhp, /*fhp->length*/ 8 + sizeof (struct scsi_format_header),
				1, 0, 0, 0, 3800) < 0) {
		return (-1);
	}

	waitformat(scgp, 300);

	scsi_close_tr_session(scgp, CL_TYPE_STOP_DEICE, 0, TRUE);
	waitformat(scgp, 300);

	scsi_close_tr_session(scgp, CL_TYPE_SESSION, 0, TRUE);
	waitformat(scgp, 300);

	return (0);
}
//--------------------------------------------------------------------
__s32	waitformat(St_GrScsiIf	*scgp, __s32 secs)
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
__s32	stats_dvdplus(St_GrScsiIf	*scgp,	cdr_t	*dp)
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
__s32	format_unit(St_GrScsiIf *scgp,	void *fmt, __s32 length, __s32 list_format,	__s32	dgdl, __s32	interlv,
	__s32	pattern, __s32 timeout)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)fmt;
	scmd->size = length;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	if (timeout < 0)
		timeout = 24*3600;	/* Kein Timeout XXX kann haengen */
	scmd->timeout = timeout;
	scmd->cdb.g0_cdb.cmd = 0x04;	/* Format Unit */
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.high_addr = (fmt?FMTDAT:0)|(dgdl?CMPLST:0)|list_format;
	scmd->cdb.g0_cdb.mid_addr = pattern;
	scmd->cdb.g0_cdb.count = interlv;

	scgp->cmdname = "format unit";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32	set_p_layerbreak(St_GrScsiIf	*scgp, long	tsize, __u32 lbreak)
{
	struct dvd_structure_20	lb;
	__s32			ret;
	__u32		dsize;
	__u32		l0_cap;

	memset((caddr_t)&lb, '\0', sizeof (lb));
	ret = read_dvd_structure(scgp, (caddr_t)&lb, sizeof (lb), 0, 0, 0, 0x20);
	if (ret < 0)
		return (ret);

	if (lb.res47[0] & 0x80) {
		return (-1);
	}

	l0_cap = a_to_u_4_byte(lb.l0_area_cap);
	if (lbreak > 0 && lbreak > l0_cap) {
		return (-1);
	}
	dsize = roundup(tsize, 16);
	if (lbreak <= 0 && dsize <= l0_cap) {

		return (-1);
	}
	l0_cap = dsize / 2;
	l0_cap = roundup(l0_cap, 16);
	if (lbreak > 0 && lbreak < l0_cap) {
		return (-1);
	}
	if (lbreak > 0)
		l0_cap = lbreak;
	i_to_4_byte(lb.l0_area_cap, l0_cap);

	ret = send_dvd_structure(scgp, (caddr_t)&lb, sizeof (lb), 0x20);
	return (ret);
}
//--------------------------------------------------------------------
__s32	ricoh_dvdsetting(St_GrScsiIf	*scgp, __s32 erase_size, __s32 flags,	__s32	immed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 3600;
	scmd->cdb.g1_cdb.cmd = 0xEA;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] = immed ? 1 : 0;
	scmd->cdb.cmd_cdb[2] = flags;
	i_to_3_byte(&scmd->cdb.cmd_cdb[3], erase_size);

	scgp->cmdname = "set_dummy_dvdr_setttings";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------

