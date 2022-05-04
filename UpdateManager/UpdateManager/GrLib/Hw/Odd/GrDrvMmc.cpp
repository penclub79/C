/*
	drv_mmc
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
	#include	<string.h>
#endif
#include 	<stdlib.h>

#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrSCSI.h>
#include 	<Hw/Odd/GrModes.h>
#include 	<Hw/Odd/GrScsiReg.h>
#include 	<Hw/Odd/GrScsiMmc.h>
#include 	<Hw/Odd/GrDrvMmc.h>
#include 	<Hw/Odd/GrScsiOpr.h>

//====================================================================
//local const
/*
 * #defines for drivemode_plextor()...
 */
#define	MODE_CODE_SH	0x01	/* Mode code for Single Session & Hide-CDR */
#define	MB1_SS		0x01	/* Single Session Mode			   */
#define	MB1_HIDE_CDR	0x02	/* Hide CDR Media			   */

#define	MODE_CODE_VREC	0x02	/* Mode code for Vari Rec		   */

#define	MODE_CODE_GREC	0x04	/* Mode code for Giga Rec		   */

#define	MODE_CODE_SPEED	0xbb	/* Mode code for Speed Read		   */
#define	MBbb_SPEAD_READ	0x01	/* Spead Read				   */

//====================================================================
//local type

__s8	clv_to_speed[16] = {
/*		0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
		0, 2, 4, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

__s8	hs_clv_to_speed[16] = {
/*		0  1  2  3  4  5  6  7   8  9 10 11 12 13 14 15 */
		0, 2, 4, 6, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

__s8	us_clv_to_speed[16] = {
/*		0  1  2  3  4  5   6  7  8   9   10  11 12 13 14 15 */
		0, 2, 4, 8, 0, 0, 16, 0, 24, 32, 40, 48, 0, 0, 0, 0
};

struct gr {
	unsigned __s8	val;
	__s8	vadd;
	__s8	*name;
} gr[] = {
	{ 0x00,	0,  "off", },
	{ 0x00,	0,  "1.0", },
	{ 0x01,	2,  "1.2", },
	{ 0x02,	3,  "1.3", },
	{ 0x03,	4,  "1.4", },
	{ 0x04,	1,  "1.1", },
	{ 0x81,	-2, "0.8", },
	{ 0x82,	-3, "0.7", },
	{ 0x83,	-4, "0.6", },
	{ 0x84,	-1, "0.9", },
	{ 0x00,	0,  NULL, },
};

unsigned __s8	db2df[] = {
	0x00,			/*  0 2352 bytes of raw data			*/
	0xFF,			/*  1 2368 bytes (raw data + P/Q Subchannel)	*/
	0xFF,			/*  2 2448 bytes (raw data + P-W Subchannel)	*/
	0xFF,			/*  3 2448 bytes (raw data + P-W raw Subchannel)*/
	0xFF,			/*  4 -    Reserved				*/
	0xFF,			/*  5 -    Reserved				*/
	0xFF,			/*  6 -    Reserved				*/
	0xFF,			/*  7 -    Vendor specific			*/
	0x10,			/*  8 2048 bytes Mode 1 (ISO/IEC 10149)		*/
	0x30,			/*  9 2336 bytes Mode 2 (ISO/IEC 10149)		*/
	0xFF,			/* 10 2048 bytes Mode 2! (CD-ROM XA form 1)	*/
	0xFF,			/* 11 2056 bytes Mode 2 (CD-ROM XA form 1)	*/
	0xFF,			/* 12 2324 bytes Mode 2 (CD-ROM XA form 2)	*/
	0xFF,			/* 13 2332 bytes Mode 2 (CD-ROM XA 1/2+subhdr)	*/
	0xFF,			/* 14 -    Reserved				*/
	0xFF,			/* 15 -    Vendor specific			*/
};

cdr_t	cdr_cd_dvd = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_NONE,
	372, 372,
	"mmc_cd_dvd",
	"generic SCSI-3/mmc   CD/DVD/BD driver (checks media)",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	cmd_dummy,
	drive_getdisktype,
	no_diskstatus,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scgp, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};

cdr_t	cdr_oldcd = {
	0, 0, 0,
	CDR_ISREADER,
	0,
	CDR_CDRW_NONE,
	WM_NONE,
	372, 372,
	"scsi2_cd",
	"generic SCSI-2       CD-ROM driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	drive_attach,
	cmd_dummy,
	drive_getdisktype,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	buf_dummy,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scg, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset_philips,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};

cdr_t	cdr_cd = {
	0, 0, 0,
	CDR_ISREADER|CDR_SWABAUDIO,
	0,
	CDR_CDRW_NONE,
	WM_NONE,
	372, 372,
	"mmc_cd",
	"generic SCSI-3/mmc   CD-ROM driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	cmd_dummy,
	drive_getdisktype,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scgp, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};

cdr_t	cdr_mmc_sony = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_SAO,
	372, 372,
	"mmc_cdr_sony",
	"generic SCSI-3/mmc   CD-R/CD-RW driver (Sony 928 variant)",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	init_mmc,
	getdisktype_mmc,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	next_wr_addr_mmc,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	gen_cue_mmc,
	send_cue_mmc,
	write_leadin_mmc,
	open_track_mmc,
	close_track_mmc,
	open_session_mmc,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	fixate_mmc,
	cmd_dummy,
	blank_mmc,
	format_dummy,
	send_opc_mmc,
	opt1_mmc,
	opt2_mmc,
};

cdr_t	cdr_mmc = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_SAO,
	372, 372,
	"mmc_cdr",
	"generic St_GrScsiIf-3/mmc   CD-R/CD-RW driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	init_mmc,
	getdisktype_mmc,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*) (St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	next_wr_addr_mmc,
	(__s32(*) (St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	gen_cue_mmc,
	send_cue_mmc,
	write_leadin_mmc,
	open_track_mmc,
	close_track_mmc,
	open_session_mmc,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	fixate_mmc,
	stats_mmc,
	blank_mmc,
	format_dummy,
	send_opc_mmc,
	opt1_mmc,
	opt2_mmc,
};

//====================================================================
//local function
BOOL8	mmc_isplextor(St_GrScsiIf *scgp);
void 	do_varirec_plextor(St_GrScsiIf	*scgp);
__s32		check_varirec_plextor(St_GrScsiIf *scgp);
__s32 	drivemode_plextor(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 modecode, void *modeval);
__s32		varirec_plextor(St_GrScsiIf *scgp,	BOOL8 on, __s32 val);
__s32		bpc_plextor(St_GrScsiIf *scgp, __s32 mode, __s32 *bpp);
__u32	gigarec_mult(__s32 code, __u32	val);
__s32		gigarec_plextor(St_GrScsiIf *scgp, __s32 val);
__s32		do_gigarec_plextor(St_GrScsiIf *scgp);
__s32		check_gigarec_plextor(St_GrScsiIf *scgp);
__s32		speed_rd_plextor(St_GrScsiIf *scgp, BOOL8 do_speedrd);
__s32		ss_hide_plextor(St_GrScsiIf *scgp, BOOL8 do_ss, BOOL8 do_hide);
__s32		set_audiomaster_yamaha(St_GrScsiIf *scgp, cdr_t *dp, BOOL8 keep_mode);
__s32		deflt_writemodes_mmc(St_GrScsiIf *scgp, BOOL8 reset_dummy);
__s32		waitfix_mmc(St_GrScsiIf *scgp, __s32 secs);
__s32		get_speeds_plextor(St_GrScsiIf	*scgp, __s32 *selp,	__s32	*maxp, __s32 *lastp);
void	fillcue(struct mmc_cue *cp,	__s32	ca, __s32	tno, __s32 idx,	__s32	dataform,	__s32	scms,	msf_t	*mp);
__s32		mmc_set_speed(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed, __s32 rotctl);
BOOL8	mmc_isyamaha(St_GrScsiIf *scgp);
__s32		force_speed_yamaha(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed);
__s32		check_powerrec_plextor(St_GrScsiIf *scgp);
__s32		powerrec_plextor(St_GrScsiIf	*scgp, BOOL8	do_powerrec);

__s32		drivemode2_plextor(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	cnt, __s32 modecode, void	*modeval);
__s32		do_tattoo_yamaha(St_GrScsiIf	*scgp, FILE	*f);
BOOL8	get_tattoo_yamaha(St_GrScsiIf *scgp, BOOL8 print, __u32 *irp, __u32 *orp);
__s32		yamaha_write_buffer(St_GrScsiIf *scgp, __s32 mode, __s32 bufferid, long offset,
							long parlen, void *buffer, long buflen);
__s32		plextor_enable(St_GrScsiIf *scgp);
__s32		plextor_disable(St_GrScsiIf *scgp);
__s32		plextor_getauth(St_GrScsiIf *scgp, void *dp, __s32 cnt);
__s32		plextor_setauth(St_GrScsiIf *scgp, void *dp, __s32 cnt);
__s32		check_ss_hide_plextor(St_GrScsiIf *scgp);
__s32		check_speed_rd_plextor(St_GrScsiIf *scgp);
__s32		getdisktype_mmc(St_GrScsiIf *scgp, cdr_t	*dp);

__s32		get_atip(St_GrScsiIf *scgp, struct atipinfo *atp);
static void	di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp);

__s32	init_mmc(St_GrScsiIf *scgp, cdr_t *dp);
__s32	prdiskstatus_mmc(St_GrScsiIf *scgp, cdr_t *dp);

//====================================================================
//var
__s32	curspeed = 1;
//extern long		bufsize;
extern __s8		*driveropts;
//extern __s32 		debug;

//====================================================================
//--------------------------------------------------------------------
cdr_t* identify_mmc(St_GrScsiIf *scgp,	cdr_t *dp )
{
		BOOL8	cdrr	 = FALSE;	/* Read CD-R	*/
		BOOL8	cdwr	 = FALSE;	/* Write CD-R	*/
		BOOL8	cdrrw	 = FALSE;	/* Read CD-RW	*/
		BOOL8	cdwrw	 = FALSE;	/* Write CD-RW	*/
		BOOL8	dvdwr	 = FALSE;	/* DVD writer	*/
		BOOL8	is_dvd	 = FALSE;	/* use DVD driver*/
		unsigned __s8	mode[0x100];
		struct	cd_mode_page_2A *mp;
		__s32	profile;
		struct scsi_inquiry* ip;

		ip	=	&scgp->inq;

		if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		{

			return ((cdr_t *)0);
		}
	
		allow_atapi(scgp, TRUE);
	
		mp = mmc_cap(scgp, mode);
		if (mp == NULL)
		{

			return (&cdr_oldcd);
		}
	
		if (ip->data_format < 2)
			ip->data_format = 2;
	
		if (strncmp(ip->vendor_info, "SONY", 4) == 0 &&
				strncmp(ip->prod_ident, "CD-R   CDU928E", 14) == 0)
		{
			return (&cdr_mmc_sony);
		}
	
		profile = get_curprofile(scgp);

		if (profile == 0) {

			load_media(scgp, dp, FALSE);
			profile = get_curprofile(scgp);
			scsi_prevent_removal(scgp, 0);
		}
		if (profile >= 0) {

			if (profile == 0 || profile > 0x19) {
				is_dvd = FALSE;
				dp = &cdr_cd;
	
				if (profile == 0)
				{
					BOOL8	is_cdr = FALSE;
	
					get_wproflist(scgp, &is_cdr, NULL,
								NULL, NULL);
					if (is_cdr)
					{
						return (&cdr_mmc);
					}
					return (dp);
				}
				if (profile <= 0x1F || profile == 0x2B)
				{
					extern	cdr_t	cdr_dvdplus;
	
					dp = &cdr_dvdplus;
					dp = dp->cdr_identify(scgp, dp );
					return (dp);
				} else if (profile >= 0x50 && profile <= 0x5F) {
					
					return ((cdr_t *)0);
				} else {
					
					return ((cdr_t *)0);
				}
			} else if (profile >= 0x10 && profile <= 0x19) {
				extern	cdr_t	cdr_dvd;
	
				dp = &cdr_dvd;
				dp = dp->cdr_identify(scgp, dp );
				return (dp);
			}
		}
	
		mmc_getval(mp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);
	
		if (!cdwr && !cdwrw)
		{
			dp = &cdr_cd;
		}
		else
		{
			dp = &cdr_mmc;
		}
	
		if (1)
		{
			__s8	xb[32];
	

			if (read_dvd_structure(scgp, (caddr_t)xb, 32, 0, 0, 0, 0) >= 0) {
				is_dvd = TRUE;
			} else {
				if (scg_sense_key(scgp) == SC_NOT_READY) {
					load_media(scgp, dp, FALSE);
					if (read_dvd_structure(scgp, (caddr_t)xb, 32, 0, 0, 0, 0) >= 0) {
						is_dvd = TRUE;
					}
					scsi_prevent_removal(scgp, 0);
				}
			}
		}
		if (is_dvd) {
			extern	cdr_t	cdr_dvd;
	
			dp = &cdr_dvd;
			dp = dp->cdr_identify(scgp, dp );
			return (dp);
		}
		return (dp);
}
//--------------------------------------------------------------------
__s32
attach_mmc(St_GrScsiIf	*scgp, cdr_t *dp)
{
		__s32	ret;
		unsigned __s8	mode[0x100];
		struct	cd_mode_page_2A *mp;
		struct	ricoh_mode_page_30 *rp = NULL;

		allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */
	
		mp = mmc_cap(scgp, NULL); /* Get MMC capabilities in allocated mp */
		if (mp == NULL)
			return (-1);	/* Pre St_GrScsiIf-3/mmc drive		*/
	
		dp->cdr_cdcap = mp;	/* Store MMC cap pointer	*/
	
		dp->cdr_dstat->ds_dr_max_rspeed = a_to_u_2_byte(mp->max_read_speed)/176;
		if (dp->cdr_dstat->ds_dr_max_rspeed == 0)
			dp->cdr_dstat->ds_dr_max_rspeed = 372;
		dp->cdr_dstat->ds_dr_cur_rspeed = a_to_u_2_byte(mp->cur_read_speed)/176;
		if (dp->cdr_dstat->ds_dr_cur_rspeed == 0)
			dp->cdr_dstat->ds_dr_cur_rspeed = 372;
	
		dp->cdr_dstat->ds_dr_max_wspeed = a_to_u_2_byte(mp->max_write_speed)/176;
		if (mp->p_len >= 28)
			dp->cdr_dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->v3_cur_write_speed)/176;
		else
			dp->cdr_dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->cur_write_speed)/176;
	
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
	
		if (mp->BUF != 0) {
			dp->cdr_flags |= CDR_BURNFREE;
		} else if (rp) {
			if ((dp->cdr_cmdflags & F_DUMMY) && rp->TWBFS && rp->BUEFS)
				dp->cdr_flags |= CDR_BURNFREE;
	
			if (rp->BUEFS)
				dp->cdr_flags |= CDR_BURNFREE;
		}
	
		if (mmc_isplextor(scgp)) {
			__s8	*p;
	
			/*
			p = hasdrvopt(driveropts, "plexdisable");
			if (p != NULL && *p == '1') 
			{
				plextor_disable(scgp);
			} else
			*/ 
			{
				p = NULL;
			}
	
			if (check_varirec_plextor(scgp) >= 0)
				dp->cdr_flags |= CDR_VARIREC;
	
			if (check_gigarec_plextor(scgp) < 0 && p == NULL)
				plextor_enable(scgp);
	
			if (check_gigarec_plextor(scgp) >= 0)
				dp->cdr_flags |= CDR_GIGAREC;
	
			if (check_ss_hide_plextor(scgp) >= 0)
				dp->cdr_flags |= CDR_SINGLESESS|CDR_HIDE_CDR;
	
			if (check_powerrec_plextor(scgp) >= 0)
				dp->cdr_flags |= CDR_FORCESPEED;
	
			if (check_speed_rd_plextor(scgp) >= 0)
				dp->cdr_flags |= CDR_SPEEDREAD;
	
			if (set_audiomaster_yamaha(scgp, dp, FALSE) >= 0)
				dp->cdr_flags |= CDR_AUDIOMASTER;
		}
		if (mmc_isyamaha(scgp)) {
			if (set_audiomaster_yamaha(scgp, dp, FALSE) >= 0)
				dp->cdr_flags |= CDR_AUDIOMASTER;
	
			if ((mp->p_len+2) >= (unsigned)28)
				dp->cdr_flags |= CDR_FORCESPEED;
	
			if (get_tattoo_yamaha(scgp, FALSE, 0, 0))
				dp->cdr_flags |= CDR_DISKTATTOO;
		}
	
		if (rp && rp->AWSCS)
			dp->cdr_flags |= CDR_FORCESPEED;
	
		check_writemodes_mmc(scgp, dp);
	
		if ((ret = get_supported_cdrw_media_types(scgp)) < 0) {
			dp->cdr_cdrw_support = CDR_CDRW_ALL;
			return (0);
		}
		dp->cdr_cdrw_support = ret;
	
		return (0);
}
//--------------------------------------------------------------------
__s32	speed_select_mmc(St_GrScsiIf	*scgp,	cdr_t 	*dp,	__s32	*speedp)
{
	unsigned __s8	mode[0x100];
	unsigned __s8	moder[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	__s32	val;
	BOOL8	forcespeed = FALSE;
	BOOL8	dummy = (dp->cdr_cmdflags & F_DUMMY) != 0;

	if (speedp)
		curspeed = *speedp;

	deflt_writemodes_mmc(scgp, FALSE);

	memset((caddr_t)mode, '\0', sizeof(mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->test_write = dummy != 0;

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);

	/*
	 * Neither set nor get speed.
	 */
	if (speedp == 0)
		return (0);


	rp = get_justlink_ricoh(scgp, moder);
	if (mmc_isyamaha(scgp)) {
		forcespeed = FALSE;
	} else if (mmc_isplextor(scgp) && (dp->cdr_flags & CDR_FORCESPEED) != 0) {
		__s32	pwr;

		pwr = check_powerrec_plextor(scgp);
		if (pwr >= 0)
			forcespeed = (pwr == 0);
	} else if ((dp->cdr_flags & CDR_FORCESPEED) != 0) {
		forcespeed = rp && rp->AWSCD != 0;
	}


	if (!forcespeed && (dp->cdr_dstat->ds_cdrflags & RF_FORCESPEED) != 0) {
		printf("Turning forcespeed on\n");
		forcespeed = TRUE;
	}
	if (forcespeed && (dp->cdr_dstat->ds_cdrflags & RF_FORCESPEED) == 0) {
		printf("Turning forcespeed off\n");
		forcespeed = FALSE;
	}
	if (mmc_isplextor(scgp) && (dp->cdr_flags & CDR_FORCESPEED) != 0) {
		powerrec_plextor(scgp, !forcespeed);
	}
	if (!mmc_isyamaha(scgp) && (dp->cdr_flags & CDR_FORCESPEED) != 0) {

		if (rp) {
			rp->AWSCD = forcespeed?1:0;
			set_mode_params(scgp, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1);
			rp = get_justlink_ricoh(scgp, moder);
		}
	}

	/*
	 * 44100 * 2 * 2 =  176400 bytes/s
	 *
	 * The right formula would be:
	 * tmp = (((long)curspeed) * 1764) / 10;
	 *
	 * But the standard is rounding the wrong way.
	 * Furtunately rounding down is guaranteed.
	 */
	val = curspeed*177;
	if (val > 0xFFFF)
		val = 0xFFFF;
	if (mmc_isyamaha(scgp) && forcespeed) {
		if (force_speed_yamaha(scgp, -1, val) < 0)
			return (-1);
	} else if (mmc_set_speed(scgp, -1, val, ROTCTL_CLV) < 0) {
		return (-1);
	}

	if (scsi_get_speed(scgp, 0, &val) >= 0) {
		if (val > 0) {
			curspeed = val / 176;
			*speedp = curspeed;
		}
	}
	return (0);
}
//--------------------------------------------------------------------
__s32	next_wr_addr_mmc(St_GrScsiIf	*scgp,	track_t	*trackp,	long	*ap)
{
	struct	track_info	track_info;
	long	next_addr;
	__s32	result = -1;
/*
	if(trackp != 0)
	{
		printf("test next_wr_addr_mmc - 1 trackp = %d, trackp->track = %d, flags = %d\n",
				(__s32)trackp, trackp->track, trackp->flags);
	}*/


	if (trackp != 0 && trackp->track > 0 && is_packet(trackp)) {
		result = get_trackinfo(scgp, (caddr_t)&track_info, TI_TYPE_TRACK,
							trackp->trackno,
							sizeof (track_info));
	}

	if (result < 0) {
		if (get_trackinfo(scgp, (caddr_t)&track_info, TI_TYPE_TRACK, 0xFF, sizeof (track_info)) < 0)
		{
			printf("Cannot get next writable address for 'invisible' track.\n");
			printf("This means that we are checking recorded media.\n");
			printf("This media cannot be written in streaming mode anymore.\n");
			printf("If you like to write to 'preformatted' RW media, try to blank the media first.\n");
			return (-1);
		}
	}
	next_addr = a_to_4_byte(track_info.next_writable_addr);
	if (ap)
		*ap = next_addr;
	return (0);
}
//--------------------------------------------------------------------
__s32 gen_cue_mmc(track_t	*trackp, void	*vcuep, BOOL8 needgap)
{
	__s32	tracks = trackp->tracks;
	__s32	i;
	struct mmc_cue	**cuep = (mmc_cue**)vcuep;
	struct mmc_cue	*cue;
	struct mmc_cue	*cp;
	__s32	ncue = 0;
	__s32	icue = 0;
	__s32	pgsize;
	msf_t	m;
	__s32	ctl;
	__s32	df;
	__s32	scms;

	cue = (mmc_cue*)malloc(1);

	for (i = 0; i <= tracks; i++) {
		ctl = (st2mode[trackp[i].sectype & ST_MASK]) << 4;
		if (is_copy(&trackp[i]))
		{
			ctl |= TM_ALLOW_COPY << 4;
		}
		/*if (is_quadro(&trackp[i]))
			ctl |= TM_QUADRO << 4;*/
		df = db2df[trackp[i].dbtype & 0x0F];
		if (trackp[i].tracktype == TOC_XA2 &&
		    trackp[i].sectype   == (SECT_MODE_2_MIX|ST_MODE_RAW)) {
			df = 0x21;
		}

		if (trackp[i].isrc) {	/* MCN or ISRC */
			ncue += 2;
			cue = (mmc_cue*)realloc(cue, ncue * sizeof (*cue));
			cp = &cue[icue++];
			if (i == 0) {
				cp->cs_ctladr = 0x02;
				//movebytes(&trackp[i].isrc[0], &cp->cs_tno, 7);
				GrDumyCopyMem( &cp->cs_tno, &trackp[i].isrc[0], 7 );
				cp = &cue[icue++];
				cp->cs_ctladr = 0x02;
				//movebytes(&trackp[i].isrc[7], &cp->cs_tno, 7);
				GrDumyCopyMem( &cp->cs_tno, &trackp[i].isrc[7], 7);
			} else {
				cp->cs_ctladr = 0x03;
				cp->cs_tno = i;
				//movebytes(&trackp[i].isrc[0], &cp->cs_index, 6);
				GrDumyCopyMem( &cp->cs_index, &trackp[i].isrc[0], 6);
				cp = &cue[icue++];
				cp->cs_ctladr = 0x03;
				cp->cs_tno = i;
				//movebytes(&trackp[i].isrc[6], &cp->cs_index, 6);
				GrDumyCopyMem(&cp->cs_index, &trackp[i].isrc[6], 6);
			}
		}
		if (i == 0) {	/* Lead in */
			df &= ~7;	/* Mask off data size & nonRAW subch */
			if (df < 0x10)
				df |= 1;
			else
				df |= 4;
			if (trackp[0].flags & TI_TEXT)	/* CD-Text in Lead-in*/
				df |= 0x40;
			lba_to_msf(-150, &m);
			cue = (mmc_cue*)realloc(cue, ++ncue * sizeof (*cue));
			cp = &cue[icue++];
			fillcue(cp, ctl|0x01, i, 0, df, 0, &m);
		} else {
			scms = 0;

			if (is_scms(&trackp[i]))
				scms = 0x80;
			pgsize = trackp[i].pregapsize;
			if (pgsize == 0 && needgap)
				pgsize++;
			lba_to_msf(trackp[i].trackstart-pgsize, &m);
			cue = (mmc_cue*)realloc(cue, ++ncue * sizeof (*cue));
			cp = &cue[icue++];
			fillcue(cp, ctl|0x01, i, 0, df, scms, &m);

			if (trackp[i].nindex == 1) {
				lba_to_msf(trackp[i].trackstart, &m);
				cue = (mmc_cue*)realloc(cue, ++ncue * sizeof (*cue));
				cp = &cue[icue++];
				fillcue(cp, ctl|0x01, i, 1, df, scms, &m);
			} else {
				__s32	idx;
				long	*idxlist;

				ncue += trackp[i].nindex;
				idxlist = trackp[i].tindex;
				cue = (mmc_cue*)realloc(cue, ncue * sizeof (*cue));

				for (idx = 1; idx <= trackp[i].nindex; idx++) {
					lba_to_msf(trackp[i].trackstart + idxlist[idx], &m);
					cp = &cue[icue++];
					fillcue(cp, ctl|0x01, i, idx, df, scms, &m);
				}
			}
		}
	}
	/* Lead out */
	ctl = (st2mode[trackp[tracks+1].sectype & ST_MASK]) << 4;
	if (is_copy(&trackp[i]))
		ctl |= TM_ALLOW_COPY << 4;
	/*if (is_quadro(&trackp[i]))
		ctl |= TM_QUADRO << 4;*/
	df = db2df[trackp[tracks+1].dbtype & 0x0F];
	if (trackp[i].tracktype == TOC_XA2 &&
	    trackp[i].sectype   == (SECT_MODE_2_MIX|ST_MODE_RAW))
	{
		df = 0x21;
	}
	df &= ~7;	/* Mask off data size & nonRAW subch */
	if (df < 0x10)
		df |= 1;
	else
		df |= 4;
	lba_to_msf(trackp[tracks+1].trackstart, &m);
	cue = (mmc_cue*)realloc(cue, ++ncue * sizeof (*cue));
	cp = &cue[icue++];
	fillcue(cp, ctl|0x01, 0xAA, 1, df, 0, &m);

	if (cuep)
		*cuep = cue;
	else
		free(cue);
	return (ncue);
}
//--------------------------------------------------------------------
__s32	send_cue_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t	*trackp)
{
	struct mmc_cue	*cp;
	__s32		ncue;
	__s32		ret;
	__u32		i;

	for (i = 1; i <= trackp->tracks; i++) {
		if (trackp[i].tracksize < (tsize_t)0) {
			return (-1);
		}
	}
	ncue = (*dp->cdr_gen_cue)(trackp, &cp, FALSE);

	ret = send_cue_sheet(scgp, (caddr_t)cp, ncue*8);
	free(cp);
	if (ret < 0) {
		ncue = (*dp->cdr_gen_cue)(trackp, &cp, TRUE);
		ret = send_cue_sheet(scgp, (caddr_t)cp, ncue*8);

		free(cp);
	}
	return (ret);
}
//--------------------------------------------------------------------
__s32 	open_track_mmc(St_GrScsiIf	*scgp,	cdr_t	*dp,	track_t *trackp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;

	if (!is_tao(trackp) && !is_packet(trackp)) {
		if (trackp->pregapsize > 0 && (trackp->flags & TI_PREGAP) == 0) {

			pad_track(scgp, dp, trackp, trackp->trackstart-trackp->pregapsize,
				(__u64)trackp->pregapsize*trackp->secsize, FALSE, 0);
		}
		return (0);
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

	mp->track_mode = st2mode[trackp->sectype & ST_MASK];

	mp->dbtype = trackp->dbtype;

	if (is_packet(trackp)) {
		mp->write_type = WT_PACKET;
		mp->track_mode |= TM_INCREMENTAL;
		mp->fp = (trackp->pktsize > 0) ? 1 : 0;
		i_to_4_byte(mp->packet_size, trackp->pktsize);
	} else if (is_tao(trackp)) {
		mp->write_type = WT_TAO;
		mp->fp = 0;
		i_to_4_byte(mp->packet_size, 0);
	} else {
		return (-1);
	}
	if (trackp->isrc) {
		mp->ISRC[0] = 0x80;	/* Set ISRC valid */
		strncpy((__s8 *)&mp->ISRC[1], trackp->isrc, 12);

	} else {
		memset(&mp->ISRC[0], '\0', sizeof (mp->ISRC));
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, trackp->secsize))
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32			fixate_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t	*trackp)
{
	__s32	ret = 0;
	__s32	key = 0;
	__s32	code = 0;
	struct timeval starttime;
	struct timeval stoptime;
	__s32	dummy = (track_base(trackp)->tracktype & TOCF_DUMMY) != 0;

	starttime.tv_sec = 0;
	starttime.tv_usec = 0;
	stoptime = starttime;
	gettimeofday(&starttime, (struct timezone *)0);

	if (is_tao(trackp) || is_packet(trackp)) {
		ret = scsi_close_tr_session(scgp, CL_TYPE_SESSION, 0,
				(dp->cdr_cmdflags&F_IMMED) != 0);
	} else {
		if (scsi_flush_cache(scgp, (dp->cdr_cmdflags&F_IMMED) != 0) < 0) {
			scsi_in_progress(scgp);

		}
	}
	key = scg_sense_key(scgp);
	code = scg_sense_code(scgp);

	unit_ready(scgp);

	if (ret >= 0) {
		wait_unit_ready(scgp, 420/curspeed);	/* XXX Wait for ATAPI */
		waitfix_mmc(scgp, 420/curspeed);	/* XXX Wait for ATAPI */
		return (ret);
	}

	if ((dummy != 0 && (key != SC_ILLEGAL_REQUEST)) ||
		((dummy == 0) &&
		(((key != SC_UNIT_ATTENTION) && (key != SC_NOT_READY)) ||
				((code != 0x2E) && (code != 0x04))))) {

		//scg_printerr(scgp);
		//scg_printresult(scgp);	/* XXX restore key/code in future */
	}

	unit_ready(scgp);

	wait_unit_ready(scgp, 420);	 /* XXX Wait for ATAPI */
	waitfix_mmc(scgp, 420/curspeed); /* XXX Wait for ATAPI */

	if (!dummy &&
		(ret >= 0 || (key == SC_UNIT_ATTENTION && code == 0x2E))) {

		gettimeofday(&stoptime, (struct timezone *)0);
		timevaldiff(&starttime, &stoptime);
		if (stoptime.tv_sec < (220 / curspeed)) {
			unsigned secs;


			secs = (280 / curspeed) - stoptime.tv_sec;

			sleep(secs);
		}
	}
	return (ret);
}
//--------------------------------------------------------------------
__s32			stats_mmc(St_GrScsiIf *scgp,	cdr_t	*dp)
{
	unsigned __s8 mode[256];
	struct	ricoh_mode_page_30 *rp;
	__u32 count;

	if (mmc_isplextor(scgp)) {
		__s32	sels;
		__s32	maxs;
		__s32	lasts;

		if (get_speeds_plextor(scgp, &sels, &maxs, &lasts) >= 0) {
			printf("Last selected write speed: %dx\n",	sels / 176);
			printf("Max media write speed:     %dx\n", 	maxs / 176);
			printf("Last actual write speed:   %dx\n", 	lasts / 176);
		}
	}

	if ((dp->cdr_dstat->ds_cdrflags & RF_BURNFREE) == 0)
		return (0);

	if (mmc_isplextor(scgp)) {
		__s32	i = 0;
		__s32	ret;

		ret = bpc_plextor(scgp, 2, &i);
		if (ret < 0)
			return (-1);
		count = i;

		bpc_plextor(scgp, 1, NULL);
	} else {
		rp = get_justlink_ricoh(scgp, mode);
		if (rp)
			count = a_to_u_2_byte(rp->link_counter);
		else
			return (-1);
	}

	return (0);
}
//--------------------------------------------------------------------
__s32			blank_mmc(St_GrScsiIf *scgp,	cdr_t	*dp, long	addr,	__s32	blanktype)
{
	BOOL8	cdrr	 = FALSE;
	BOOL8	cdwr	 = FALSE;
	BOOL8	cdrrw	 = FALSE;
	BOOL8	cdwrw	 = FALSE;
	__s32	ret;

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, NULL);
	if (!cdwrw)
		return (blank_dummy(scgp, dp, addr, blanktype));


	ret = scsi_blank(scgp, addr, blanktype, (dp->cdr_cmdflags&F_IMMED) != 0);
	if (ret < 0)
		return (ret);

	wait_unit_ready(scgp, 90*60/curspeed);
	waitfix_mmc(scgp, 90*60/curspeed);
	return (ret);
}
//--------------------------------------------------------------------
__s32			send_opc_mmc(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	doopc)
{
	__s32	ret;

	ret = send_opc(scgp, bp, cnt, doopc);

	if (ret >= 0)
		return (ret);

	if (scg_sense_key(scgp) == SC_UNIT_ATTENTION &&
	    scg_sense_code(scgp) == 0x5A &&
	    scg_sense_qual(scgp) == 0x03)
		return (0);

	if ((scg_sense_key(scgp) == SC_RECOVERABLE_ERROR ||
	    scg_sense_key(scgp) == SC_MEDIUM_ERROR) &&
	    scg_sense_code(scgp) == 0x73 &&
	    scg_sense_qual(scgp) == 0x01)
		return (0);

	if (scg_sense_key(scgp) != SC_ILLEGAL_REQUEST) {
		return (ret);
	}
	return (0);
}
//--------------------------------------------------------------------
__s32			opt1_mmc(St_GrScsiIf	*scgp, cdr_t *dp)
{
	__s32	oflags = dp->cdr_dstat->ds_cdrflags;

	if ((dp->cdr_dstat->ds_cdrflags & RF_AUDIOMASTER) != 0) {

		if (set_audiomaster_yamaha(scgp, dp, TRUE) < 0)
			return (-1);

		if (getdisktype_mmc(scgp, dp) < 0) {
			dp->cdr_dstat->ds_cdrflags = oflags;
			return (-1);
		}
		dp->cdr_dstat->ds_cdrflags = oflags;
		if (oflags & RF_PRATIP) {
			msf_t   msf;
			lba_to_msf(dp->cdr_dstat->ds_first_leadin, &msf);

			lba_to_msf(dp->cdr_dstat->ds_maxblocks, &msf);
		}
	}
	if (mmc_isplextor(scgp)) {
		__s32	gcode;

		if ((dp->cdr_flags & (CDR_SINGLESESS|CDR_HIDE_CDR)) != 0) {
			if (ss_hide_plextor(scgp,
			    (dp->cdr_dstat->ds_cdrflags & RF_SINGLESESS) != 0,
			    (dp->cdr_dstat->ds_cdrflags & RF_HIDE_CDR) != 0) < 0)
				return (-1);
		}

		if ((dp->cdr_flags & CDR_SPEEDREAD) != 0) {
			if (speed_rd_plextor(scgp,
			    (dp->cdr_dstat->ds_cdrflags & RF_SPEEDREAD) != 0) < 0)
				return (-1);
		}

		if ((dp->cdr_cmdflags & F_SETDROPTS) ||
		    (wm_base(dp->cdr_dstat->ds_wrmode) == WM_SAO) ||
		    (wm_base(dp->cdr_dstat->ds_wrmode) == WM_RAW))
			gcode = do_gigarec_plextor(scgp);
		else
			gcode = gigarec_plextor(scgp, 0);
		if (gcode != 0) {
			msf_t   msf;

			dp->cdr_dstat->ds_first_leadin =
					gigarec_mult(gcode, dp->cdr_dstat->ds_first_leadin);
			dp->cdr_dstat->ds_maxblocks =
					gigarec_mult(gcode, dp->cdr_dstat->ds_maxblocks);

			if (oflags & RF_PRATIP) {
				lba_to_msf(dp->cdr_dstat->ds_first_leadin, &msf);
				lba_to_msf(dp->cdr_dstat->ds_maxblocks, &msf);
			}
		}
	}
	return (0);
}
//--------------------------------------------------------------------
__s32			opt2_mmc(St_GrScsiIf	*scgp, cdr_t *dp)
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

		if (!set_mode_params(scgp, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1))
			return (-1);
		rp = get_justlink_ricoh(scgp, moder);
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);

	if (mmc_isplextor(scgp)) {
		bpc_plextor(scgp, 1, NULL);

		do_varirec_plextor(scgp);
	}

	return (0);
}
//--------------------------------------------------------------------
__s32 write_leadin_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t *trackp)
{
	__u32	i;
	long	startsec = 0L;

/*	if (flags & F_SAO) {*/
	if (wm_base(dp->cdr_dstat->ds_wrmode) == WM_SAO)
	{
		printf("Sending CUE sheet...\n");

		if ((*dp->cdr_send_cue)(scgp, dp, trackp) < 0)
		{
			printf("Cannot send CUE sheet.\n");
			return (-1);
		}

		(*dp->cdr_next_wr_address)(scgp, &trackp[0], &startsec);
		if (trackp[0].flags & TI_TEXT) {
			startsec = dp->cdr_dstat->ds_first_leadin;
		} else if (startsec <= 0 && startsec != -150) {
			startsec = -150;
		}

		if (trackp[0].flags & TI_TEXT) {
			if (startsec > 0) {
				return (-1);
			}
			if (write_cdtext(scgp, dp, startsec) < 0)
				return (-1);

			dp->cdr_dstat->ds_cdrflags |= RF_LEADIN;
		} else for (i = 1; i <= trackp->tracks; i++) {
			trackp[i].trackstart += startsec +150;
		}

	}

	if (wm_base(dp->cdr_dstat->ds_wrmode) == WM_RAW) {

		(*dp->cdr_next_wr_address)(scgp, &trackp[0], &startsec);
		if (startsec > -4500) {

			startsec = dp->cdr_dstat->ds_first_leadin;
		}
		if (startsec > -4500) {
			return (-1);
		}

		if (write_leadin(scgp, dp, trackp, startsec) < 0)
			return (-1);
		dp->cdr_dstat->ds_cdrflags |= RF_LEADIN;
	}
	return (0);
}
//--------------------------------------------------------------------
__s32			close_track_mmc(St_GrScsiIf *scgp,	cdr_t	*dp, track_t *trackp)
{
	__s32	ret;

	if (!is_tao(trackp) && !is_packet(trackp))
		return (0);

	if (scsi_flush_cache(scgp, (dp->cdr_cmdflags&F_IMMED) != 0) < 0) {
		return (-1);
	}
	wait_unit_ready(scgp, 300);
	if (is_packet(trackp) && !is_noclose(trackp)) {

		ret = scsi_close_tr_session(scgp, CL_TYPE_TRACK, 0xFF,
				(dp->cdr_cmdflags&F_IMMED) != 0);
		wait_unit_ready(scgp, 300);
		return (ret);
	}
	return (0);
}
//--------------------------------------------------------------------
__s32			open_session_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t	*trackp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->write_type = WT_TAO; /* fix to allow DAO later */
	mp->track_mode = st2mode[trackp[0].sectype & ST_MASK];
	mp->dbtype = trackp[0].dbtype;

	if (!is_tao(trackp) && !is_packet(trackp)) {
		mp->write_type = WT_SAO;
		if (dp->cdr_dstat->ds_cdrflags & RF_AUDIOMASTER)
			mp->write_type = 8;
		mp->track_mode = 0;
		mp->dbtype = DB_RAW;
	}
	if (is_raw(trackp)) {
		mp->write_type = WT_RAW;
		mp->track_mode = 0;

		if (is_raw16(trackp)) {
			mp->dbtype = DB_RAW_PQ;
		} else if (is_raw96r(trackp)) {
			mp->dbtype = DB_RAW_PW_R;
		} else {
			mp->dbtype = DB_RAW_PW;
		}
	}

	mp->multi_session = (track_base(trackp)->tracktype & TOCF_MULTI) ?
				MS_MULTI : MS_NONE;
	mp->session_format = toc2sess[track_base(trackp)->tracktype & TOC_MASK];

	if (trackp->isrc) {
		mp->media_cat_number[0] = 0x80;	/* Set MCN valid */
		strncpy((__s8 *)&mp->media_cat_number[1], trackp->isrc, 13);

	} else {
		memset(&mp->media_cat_number[0], '\0', sizeof (mp->media_cat_number));
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
BOOL8	mmc_isplextor(St_GrScsiIf *scgp)
{
	if (strncmp(scgp->inq.vendor_info, "PLEXTOR", 7) == 0) 
	{
		return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
void 	do_varirec_plextor(St_GrScsiIf	*scgp)
{
	//__s8	*p;
	/*__s32	voff;

	p = hasdrvopt(driveropts, "varirec");
	if ( p == NULL || curspeed != 4)
	*/ 
	{
		if (check_varirec_plextor(scgp) >= 0)
			varirec_plextor(scgp, FALSE, 0);
	}
	/* 
	else {
		__s8	*ep;
		ep = astoi(p, &voff);
		if (check_varirec_plextor(scgp) < 0)
		{
			//comerrno(EX_BAD, "Drive does not support VariRec.\n");
		}
		varirec_plextor(scgp, TRUE, voff);
	}
	*/
}
//--------------------------------------------------------------------
__s32		check_varirec_plextor(St_GrScsiIf *scgp)
{
	__s32	modecode = 2;
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (0);
}
//--------------------------------------------------------------------
__s32 	drivemode_plextor(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 modecode, void *modeval)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	if (modeval == NULL) {
		scmd->flags |= SCG_RECV_DATA;
		scmd->addr = bp;
		scmd->size = cnt;
	} else {
		scmd->cdb.g5_cdb.res = 0x08;
	}
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xE9;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.addr[0] = modecode;
	if (modeval)
	{
		//movebytes(modeval, &scmd->cdb.g1_cdb.addr[1], 6);
		GrDumyCopyMem( &scmd->cdb.g1_cdb.addr[1], modeval, 6);
	}
	else
		i_to_2_byte(&scmd->cdb.cmd_cdb[9], cnt);

	scgp->cmdname = "plextor drive mode";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		varirec_plextor(St_GrScsiIf *scgp,	BOOL8 on, __s32 val)
{
	__s32	modecode = 2;
	unsigned __s8	psetmode[8];
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	memset(psetmode, '\0', sizeof (psetmode));
	psetmode[0] = on?1:0;
	if (on) {

		if (val > 0) {
			psetmode[1] = val & 0x7F;
		} else {
			psetmode[1] = (-val) & 0x7F;
			psetmode[1] |= 0x80;
		}
	}

	if (drivemode_plextor(scgp, NULL, 0, modecode, psetmode) < 0)
		return (-1);

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s8*		hasdrvopt(__s8 *optstr, __s8 *optname)
{
	return (hasdrvoptx(optstr, optname, 1));
}
//--------------------------------------------------------------------
__s8*		hasdrvoptx(__s8 *optstr, __s8 *optname, __s32 flag)
{
	__s8	*ep;
	__s8	*np;
	__s8	*ret = NULL;
	__s32	optnamelen;
	__s32	optlen;
	BOOL8	not1 = FALSE;

	if (optstr == NULL)
		return (ret);

	optnamelen = strlen(optname);

	while (*optstr) {
		not1 = FALSE;			/* Reset before every token */
		if ((ep = strchr(optstr, ',')) != NULL) {
			optlen = ep - optstr;
			np = &ep[1];
		} else {
			optlen = strlen(optstr);
			np = &optstr[optlen];
		}
		if ((ep = strchr(optstr, '=')) != NULL) {
			if (ep < np)
				optlen = ep - optstr;
		}
		if (optstr[0] == '!') {
			optstr++;
			optlen--;
			not1 = TRUE;
		}
		if (strncmp(optstr, "no", 2) == 0) {
			optstr += 2;
			optlen -= 2;
			not1 = TRUE;
		}
		if (optlen == optnamelen &&
		    strncmp(optstr, optname, optlen) == 0) {
			ret = &optstr[optlen];
			break;
		}
		optstr = np;
	}
	if (ret != NULL) {
		if (*ret == ',' || *ret == '\0') {
			if (flag) {
				if (not1)
					return ("0");
				return ("1");
			}
			if (not1)
				return (NULL);
			return ("");
		}
		if (*ret == '=')
		{
			if (not1)
				return (NULL);
			return (++ret);
		}
	}
	return (ret);
}
//--------------------------------------------------------------------
__s32		bpc_plextor(St_GrScsiIf *scgp, __s32 mode, __s32 *bpp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	__s8	buf[4];
	__s32	i;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	memset((caddr_t)buf, '\0', sizeof (buf));
	scmd->flags = SCG_DISRE_ENA;
	scmd->flags |= SCG_RECV_DATA;
	scmd->addr = buf;
	scmd->size = sizeof (buf);
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xF5;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);

	scmd->cdb.g5_cdb.addr[1] = 0x08;
	scmd->cdb.g5_cdb.addr[2] = mode;

	i_to_2_byte(&scmd->cdb.cmd_cdb[8], sizeof (buf));

	scgp->cmdname = "plextor read bpc";

	if (scg_cmd(scgp) < 0)
		return (-1);

	if (scgp->scmd.resid > 2)

		return (0);

	i = a_to_u_2_byte(buf);
	if (bpp)
		*bpp = i;

	return (0);
}
//--------------------------------------------------------------------
struct ricoh_mode_page_30*	get_justlink_ricoh(St_GrScsiIf	*scgp, unsigned __s8 *mode)
{
	unsigned __s8	modec[0x100];
	__s32	len;
	struct	ricoh_mode_page_30 *mp;

	if (!get_mode_params(scgp, 0x30, "Ricoh Vendor Page", mode, modec, NULL, NULL, &len)) {
		return ((struct ricoh_mode_page_30 *)0);
	}

	if (len < 10)
		return ((struct ricoh_mode_page_30 *)0);

	mp = (struct ricoh_mode_page_30 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if ((len - ((unsigned __s8 *)mp - mode) -1) < 5)
		return ((struct ricoh_mode_page_30 *)0);

	return (mp);
}
//--------------------------------------------------------------------
__u32	gigarec_mult(__s32 code, __u32	val)
{
	__u32	add;
	struct gr *gp = gr;

	for (; gp->name != NULL; gp++) {
		if (code == gp->val)
			break;
	}
	if (gp->vadd == 0)
		return (val);

	add = val * gp->vadd / 10;
	return (val + add);
}
//--------------------------------------------------------------------
__s32		gigarec_plextor(St_GrScsiIf *scgp, __s32 val)
{
	__s32	modecode = 4;
	unsigned __s8	psetmode[8];
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	memset(psetmode, '\0', sizeof (psetmode));
	psetmode[1] = val;

	if (drivemode_plextor(scgp, NULL, 0, modecode, psetmode) < 0)
		return (-1);

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0)
		return (-1);

	{
		struct gr *gp = gr;

		for (; gp->name != NULL; gp++) {
			if (pgetmode[3] == gp->val)
				break;
		}
	}
	return (pgetmode[3]);
}
//--------------------------------------------------------------------
__s32		do_gigarec_plextor(St_GrScsiIf *scgp)
{
	__s8	*p;
	__s32	val = 0;	/* Make silly GCC happy */

	/*
	p = hasdrvopt(driveropts, "gigarec");
	if (p == NULL) 
	{
		if (check_gigarec_plextor(scgp) >= 0)
			gigarec_plextor(scgp, 0);
	} else
	*/ 
	{
		struct gr *gp = gr;

		if (strlen(p) >= 3) {
			for (; gp->name != NULL; gp++) {
				if (strncmp(p, gp->name, 3) == 0) {
					if (p[3] != '\0' && p[3] != ',')
						continue;
					val = gp->val;
					break;
				}
			}
		}

		if (check_gigarec_plextor(scgp) < 0)
		{
			//comerrno(EX_BAD, "Drive does not support GigaRec.\n");
		}
		return (gigarec_plextor(scgp, val));
	}
	return (0);
}
//--------------------------------------------------------------------
__s32		check_gigarec_plextor(St_GrScsiIf *scgp)
{
	__s32	modecode = 4;
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (0);
}
//--------------------------------------------------------------------
__s32		speed_rd_plextor(St_GrScsiIf *scgp, BOOL8 do_speedrd)
{
	__s32	modecode = 0xBB;
	unsigned __s8	psetmode[8];
	unsigned __s8	pgetmode[8];
	BOOL8	is_speedrd;

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	is_speedrd = (pgetmode[2] & MBbb_SPEAD_READ) != 0;


	memset(psetmode, '\0', sizeof (psetmode));
	psetmode[0] = pgetmode[2];		/* Copy over old values */
	if (do_speedrd >= 0) {
		if (do_speedrd)
			psetmode[0] |= MBbb_SPEAD_READ;
		else
			psetmode[0] &= ~MBbb_SPEAD_READ;
	}

	if (drivemode_plextor(scgp, NULL, 0, modecode, psetmode) < 0)
		return (-1);

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0)
		return (-1);

	if (do_speedrd >= 0 && do_speedrd != is_speedrd)
		scsi_set_speed(scgp, 0xFFFF, -1, ROTCTL_CAV);

	return (0);
}
//--------------------------------------------------------------------
__s32		ss_hide_plextor(St_GrScsiIf *scgp, BOOL8 do_ss, BOOL8 do_hide)
{
	__s32	modecode = 1;
	unsigned __s8	psetmode[8];
	unsigned __s8	pgetmode[8];
	BOOL8	is_ss;
	BOOL8	is_hide;

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}


	is_ss = (pgetmode[2] & MB1_SS) != 0;
	is_hide = (pgetmode[2] & MB1_HIDE_CDR) != 0;


	memset(psetmode, '\0', sizeof (psetmode));
	psetmode[0] = pgetmode[2];		/* Copy over old values */
	if (do_ss >= 0) {
		if (do_ss)
			psetmode[0] |= MB1_SS;
		else
			psetmode[0] &= ~MB1_SS;
	}
	if (do_hide >= 0) {
		if (do_hide)
			psetmode[0] |= MB1_HIDE_CDR;
		else
			psetmode[0] &= ~MB1_HIDE_CDR;
	}


	if (drivemode_plextor(scgp, NULL, 0, modecode, psetmode) < 0)
		return (-1);

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32		set_audiomaster_yamaha(St_GrScsiIf *scgp, cdr_t *dp, BOOL8 keep_mode)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	__s32	ret = 0;
	struct	cd_mode_page_05 *mp;

	deflt_writemodes_mmc(scgp, FALSE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len)) {
		return (-1);
	}
	if (len == 0) {
		return (-1);
	}

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->BUFE = 0;

	mp->write_type = 8;
	mp->track_mode = 0;
	mp->dbtype = DB_RAW;

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		ret = -1;

	if (!keep_mode || ret < 0)
		deflt_writemodes_mmc(scgp, FALSE);

	return (ret);
}
//--------------------------------------------------------------------
__s32		deflt_writemodes_mmc(St_GrScsiIf *scgp, BOOL8 reset_dummy)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len)) {
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
__s32		waitfix_mmc(St_GrScsiIf *scgp, __s32 secs)
{
	__s8	dibuf[16];
	__s32	i;
	__s32	key;
#define	W_SLEEP	2

	for (i = 0; i < secs/W_SLEEP; i++) {
		if (read_disk_info(scgp, dibuf, sizeof (dibuf)) >= 0) {
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
__s32		get_speeds_plextor(St_GrScsiIf	*scgp, __s32 *selp,	__s32	*maxp, __s32 *lastp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	__s8	buf[10];
	__s32	i;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	memset((caddr_t)buf, '\0', sizeof (buf));
	scmd->flags = SCG_DISRE_ENA;
	scmd->flags |= SCG_RECV_DATA;
	scmd->addr = buf;
	scmd->size = sizeof (buf);
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xEB;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);

	i_to_2_byte(&scmd->cdb.cmd_cdb[8], sizeof (buf));

	scgp->cmdname = "plextor get speedlist";

	if (scg_cmd(scgp) < 0)
		return (-1);

	i = a_to_u_2_byte(&buf[4]);
	if (selp)
		*selp = i;

	i = a_to_u_2_byte(&buf[6]);
	if (maxp)
		*maxp = i;

	i = a_to_u_2_byte(&buf[8]);
	if (lastp)
		*lastp = i;

	return (0);
}
//--------------------------------------------------------------------
void	fillcue(struct mmc_cue *cp,	__s32	ca, __s32	tno, __s32 idx,	__s32	dataform,	__s32	scms,	msf_t	*mp)
{
	cp->cs_ctladr = ca;		/* XXX wie lead in */
	cp->cs_tno = tno;
	cp->cs_index = idx;
	cp->cs_dataform = dataform;	/* XXX wie lead in */
	cp->cs_scms = scms;
	cp->cs_min = mp->msf_min;
	cp->cs_sec = mp->msf_sec;
	cp->cs_frame = mp->msf_frame;
}
//--------------------------------------------------------------------
__s32		mmc_set_speed(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed, __s32 rotctl)
{
	__s32	rs;
	__s32	ws;
	__s32	ret = -1;
	__s32	c;
	__s32	k;

	if (scsi_get_speed(scgp, &rs, &ws) >= 0) {
		if (readspeed < 0)
			readspeed = rs;
		if (writespeed < 0)
			writespeed = ws;
	}
	if (writespeed < 0 || writespeed > 0xFFFF)
		return (ret);

	while (writespeed <= 0xFFFF) {
		ret = scsi_set_speed(scgp, readspeed, writespeed, rotctl);
		if (ret >= 0)
			break;
		c = scg_sense_code(scgp);
		k = scg_sense_key(scgp);

		if ((k != SC_ILLEGAL_REQUEST) || (c != 0x24)) {

			return (-1);
		}
		writespeed += 177;
	}


	return (ret);
}
//--------------------------------------------------------------------
BOOL8	mmc_isyamaha(St_GrScsiIf *scgp)
{
	if ( strncmp(scgp->inq.vendor_info, "YAMAHA", 6) == 0) 
	{
		return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
__s32		force_speed_yamaha(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

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

	scmd->cdb.cmd_cdb[11] = 0x80;

	scgp->cmdname = "yamaha force cd speed";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		check_powerrec_plextor(St_GrScsiIf *scgp)
{
	__s32	modecode = 0;
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode2_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	if (pgetmode[2] & 1)
		return (1);

	return (0);
}
//--------------------------------------------------------------------
__s32	powerrec_plextor(St_GrScsiIf	*scgp, BOOL8	do_powerrec)
{
	__s32	modecode = 0;
	unsigned __s8	psetmode[8];
	unsigned __s8	pgetmode[8];
	BOOL8	is_powerrec;
	__s32	speed;

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode2_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	is_powerrec = (pgetmode[2] & 1) != 0;

	speed = a_to_u_2_byte(&pgetmode[4]);

	memset(psetmode, '\0', sizeof (psetmode));
	psetmode[0] = pgetmode[2];		/* Copy over old values */
	if (do_powerrec >= 0) {
		if (do_powerrec)
			psetmode[0] |= 1;
		else
			psetmode[0] &= ~1;
	}

	if (drivemode2_plextor(scgp, NULL, 0, modecode, psetmode) < 0)
		return (-1);

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode2_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32		drivemode2_plextor(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	cnt, __s32 modecode, void	*modeval)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	if (modeval == NULL) {
		scmd->flags |= SCG_RECV_DATA;
		scmd->addr = bp;
		scmd->size = cnt;
	} else {
		scmd->cdb.g5_cdb.res = 0x08;
	}
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xED;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.addr[0] = modecode;
	if (modeval)
		scmd->cdb.g5_cdb.reladr = *(__s8 *)modeval != 0 ? 1 : 0;
	else
		i_to_2_byte(&scmd->cdb.cmd_cdb[8], cnt);

	scgp->cmdname = "plextor drive mode2";

	if (scg_cmd(scgp) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32		do_tattoo_yamaha(St_GrScsiIf	*scgp, FILE	*f)
{
	__u32 ival = 0;
	__u32 oval = 0;
	__u32	lines;
	off_t	fsize;
	__s8	*buf = (__s8*)scgp->bufptr;
	long	bufsize = scgp->maxbuf;
	long	nsecs;
	long	amt;

	nsecs = bufsize / 2048;
	bufsize = nsecs * 2048;

	if (!get_tattoo_yamaha(scgp, FALSE, &ival, &oval)) {

		return (-1);
	}

	if (ival == 0 || oval == 0) {

		return (-1);
	}

	lines = oval - ival + 1;
	fsize = filesize(f);
	if ((fsize % 3744) != 0 || fsize < (lines*3744)) {

		return (-1);
	}
	if (fsize > (lines*3744))
		fsize = lines*3744;

	memset(buf, '\0', bufsize);
	if ((amt = fileread(f, buf, bufsize)) <= 0) {

		return (-1);
	}

	if (yamaha_write_buffer(scgp, 1, 0, ival, amt/2048, buf, amt) < 0) {

		return (-1);
	}
	amt = (amt+2047) / 2048 * 2048;
	fsize -= amt;

	while (fsize > 0) {
		memset(buf, '\0', bufsize);
		if ((amt = fileread(f, buf, bufsize)) <= 0) {

			return (-1);
		}
		amt = (amt+2047) / 2048 * 2048;
		fsize -= amt;
		if (yamaha_write_buffer(scgp, 1, 0, 0, amt/2048, buf, amt) < 0) {

			return (-1);
		}
	}

	if (yamaha_write_buffer(scgp, 1, 0, oval, 0, buf, 0) < 0) {

		return (-1);
	}

	wait_unit_ready(scgp, 1000);	/* Wait for DiskT@2 */
	waitfix_mmc(scgp, 1000);	/* Wait for DiskT@2 */

	return (0);
}
//--------------------------------------------------------------------
BOOL8	get_tattoo_yamaha(St_GrScsiIf *scgp, BOOL8 print, __u32 *irp, __u32 *orp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	__u32 ival;
	__u32 oval;
	unsigned __s8	*mp;

	if (!get_mode_params(scgp, 0x31, "Yamaha Tattoo Page", mode, NULL, NULL, NULL, &len)) {
		return (FALSE);
	}

	if (len < 20)
		return (FALSE);

	mp = (unsigned __s8 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if ((len - ((unsigned __s8 *)mp - mode) -1) < 10)
		return (FALSE);

	ival = a_to_u_3_byte(&mp[4]);
	oval = a_to_u_3_byte(&mp[7]);

	if (irp)
		*irp = ival;
	if (orp)
		*orp = oval;

	return (TRUE);
}
//--------------------------------------------------------------------
__s32		yamaha_write_buffer(St_GrScsiIf *scgp, __s32 mode, __s32 bufferid, long offset,
							long parlen, void *buffer, long buflen)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	unsigned __s8	*CDB;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (__s8*)buffer;
	scmd->size = buflen;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x3B;

	CDB    = (unsigned __s8 *)scmd->cdb.cmd_cdb;
	CDB[1] = mode & 7;
	CDB[2] = bufferid;
	i_to_3_byte(&CDB[3], offset);
	i_to_3_byte(&CDB[6], parlen);

	scgp->cmdname = "write_buffer";

	if (scg_cmd(scgp) >= 0)
		return (1);
	return (0);
}
//--------------------------------------------------------------------
__s32	plextor_enable(St_GrScsiIf *scgp)
{
	__s32		ret;
	__u32	key[4];

	ret = plextor_getauth(scgp, key, sizeof (key));
	if (ret < 0)
		return (ret);

	ret = plextor_setauth(scgp, key, sizeof (key));
	return (ret);
}
//--------------------------------------------------------------------
__s32	plextor_disable(St_GrScsiIf *scgp)
{
	return (plextor_setauth(scgp, NULL, 0));
}
//--------------------------------------------------------------------
__s32	plextor_getauth(St_GrScsiIf *scgp, void *dp, __s32 cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->flags |= SCG_RECV_DATA;
	scmd->addr = (__s8*)dp;
	scmd->size = cnt;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xD4;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[10] = cnt;

	scgp->cmdname = "plextor getauth";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32	plextor_setauth(St_GrScsiIf *scgp, void *dp, __s32 cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->addr = (__s8*)dp;
	scmd->size = cnt;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xD5;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] = 0x01;
	if (cnt != 0)				/* If cnt == 0 clearauth */
		scmd->cdb.cmd_cdb[2] = 0x01;
	scmd->cdb.cmd_cdb[10] = cnt;

	scgp->cmdname = "plextor setauth";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		check_ss_hide_plextor(St_GrScsiIf *scgp)
{
	__s32	modecode = 1;
	unsigned __s8	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (pgetmode[2] & 0x03);
}
//--------------------------------------------------------------------
__s32		check_speed_rd_plextor(St_GrScsiIf *scgp)
{
	__s32	modecode = 0xBB;
	unsigned __s8 pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (pgetmode[2] & 0x01);
}
//--------------------------------------------------------------------
__s32			check_writemodes_mmc(St_GrScsiIf *scgp, cdr_t *dp)
{
	unsigned __s8	mode[0x100];
	__s32	len;
	struct	cd_mode_page_05 *mp;


	deflt_writemodes_mmc(scgp, TRUE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len)) {
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

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_TAO;
	} else
		dp->cdr_flags &= ~CDR_TAO;

	mp->write_type = WT_PACKET;
	mp->track_mode |= TM_INCREMENTAL;

	mp->fp = 0;
	i_to_4_byte(mp->packet_size, 0);

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_PACKET;
	} else
		dp->cdr_flags &= ~CDR_PACKET;
	mp->fp = 0;
	i_to_4_byte(mp->packet_size, 0);
	mp->track_mode = TM_DATA;


	mp->write_type = WT_SAO;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_SAO;
	} else
		dp->cdr_flags &= ~CDR_SAO;

	if (dp->cdr_flags & CDR_SAO) {
		mp->dbtype = DB_RAW_PQ;


		mp->dbtype = DB_RAW_PW;

		if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
			dp->cdr_flags |= CDR_SRAW96P;
		}

		mp->dbtype = DB_RAW_PW_R;

		if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
			dp->cdr_flags |= CDR_SRAW96R;
		}
	}

	mp->write_type = WT_RAW;
	mp->dbtype = DB_RAW_PQ;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_RAW;
		dp->cdr_flags |= CDR_RAW16;

	}

	mp->dbtype = DB_RAW_PW;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_RAW;
		dp->cdr_flags |= CDR_RAW96P;

	}

	mp->dbtype = DB_RAW_PW_R;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1)) {
		dp->cdr_flags |= CDR_RAW;
		dp->cdr_flags |= CDR_RAW96R;

	}

	mp->track_mode = TM_DATA;
	mp->write_type = WT_LAYER_JUMP;

	if (set_mode_params(scgp, "CD write parameter", mode, len, 0, -1) &&
	    has_profile(scgp, 0x16) == 1) {
		dp->cdr_flags |= CDR_LAYER_JUMP;

	} else
		dp->cdr_flags &= ~CDR_LAYER_JUMP;


	deflt_writemodes_mmc(scgp, TRUE);

	return (0);
}
//--------------------------------------------------------------------
__s32		getdisktype_mmc(St_GrScsiIf *scgp, cdr_t	*dp)
{
	dstat_t	*dsp = dp->cdr_dstat;
	struct disk_info *dip;
	unsigned __s8	mode[0x100];
	msf_t	msf;
	BOOL8	did_atip = FALSE;
	BOOL8	did_dummy = FALSE;
	__s32	profile;

	msf.msf_min = msf.msf_sec = msf.msf_frame = 0;

	if (dsp->ds_type == DST_UNKNOWN) {
		profile = get_curprofile(scgp);
		if (profile >= 0)
			dsp->ds_type = profile;
	}

	if (get_atip(scgp, (struct atipinfo *)mode) >= 0) {
		struct atipinfo *atp = (struct atipinfo *)mode;

		msf.msf_min =		mode[8];
		msf.msf_sec =		mode[9];
		msf.msf_frame =		mode[10];
		if (atp->desc.erasable) {
			dsp->ds_flags |= DSF_ERA;
			if (atp->desc.sub_type == 1)
				dsp->ds_flags |= DSF_HIGHSP_ERA;
			else if (atp->desc.sub_type == 2)
				dsp->ds_flags |= DSF_ULTRASP_ERA;
			else if (atp->desc.sub_type == 3)
				dsp->ds_flags |= DSF_ULTRASP_ERA | DSF_ULTRASPP_ERA;
		}
		if (atp->desc.a1_v) {
			if (atp->desc.clv_low != 0)
				dsp->ds_at_min_speed = clv_to_speed[atp->desc.clv_low];
			if (atp->desc.clv_high != 0)
				dsp->ds_at_max_speed = clv_to_speed[atp->desc.clv_high];

			if (atp->desc.erasable && atp->desc.sub_type == 1) {
				if (atp->desc.clv_high != 0)
					dsp->ds_at_max_speed = hs_clv_to_speed[atp->desc.clv_high];
			}
		}
		if (atp->desc.a2_v && atp->desc.erasable && (atp->desc.sub_type == 2 || atp->desc.sub_type == 3)) {
			__u32	vlow;
			__u32	vhigh;

			vlow = (atp->desc.a2[0] >> 4) & 0x07;
			vhigh = atp->desc.a2[0] & 0x0F;
			if (vlow != 0)
				dsp->ds_at_min_speed = us_clv_to_speed[vlow];
			if (vhigh != 0)
				dsp->ds_at_max_speed = us_clv_to_speed[vhigh];
		}
		did_atip = TRUE;
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
			speed_select_mmc(scgp, dp, &xspeed);
			dp->cdr_cmdflags = oflags;
			did_dummy = TRUE;
			goto again;
		}

		reload_media(scgp, dp);
	}
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);
	di_to_dstat(dip, dsp);
	if (!did_atip && dsp->ds_first_leadin < 0)
		lba_to_msf(dsp->ds_first_leadin, &msf);

	if ((dp->cdr_dstat->ds_cdrflags & RF_PRATIP) != 0 && !did_atip) 
	{
		print_min_atip(dsp->ds_first_leadin, dsp->ds_last_leadout);
	}
	dsp->ds_maxrblocks = disk_rcap(&msf, dsp->ds_maxblocks, (BOOL8)dip->erasable, (BOOL8)dip->uru);

	return (drive_getdisktype(scgp, dp));
}
//--------------------------------------------------------------------
__s32		get_atip(St_GrScsiIf *scgp, struct atipinfo *atp)
{
	__s32	len;
	__s32	ret;

	memset((caddr_t)atp, '\0', sizeof (*atp));

	if (read_toc(scgp, (caddr_t)atp, 0, sizeof (struct tocheader), 0, FMT_ATIP) < 0)
		return (-1);
	len = a_to_u_2_byte(atp->hd.len);
	len += 2;
	ret = read_toc(scgp, (caddr_t)atp, 0, len, 0, FMT_ATIP);

	if (atp->desc.lead_in[1] == 0 &&
			atp->desc.lead_in[2] == 0 &&
			atp->desc.lead_in[3] == 0 &&
			atp->desc.lead_out[1] == 0 &&
			atp->desc.lead_out[2] == 0 &&
			atp->desc.lead_out[3] == 0)
		return (-1);

	if (atp->desc.lead_in[1] >= 0x90)
	{
		atp->desc.lead_in[1] = from_bcd(atp->desc.lead_in[1]);
		atp->desc.lead_in[2] = from_bcd(atp->desc.lead_in[2]);
		atp->desc.lead_in[3] = from_bcd(atp->desc.lead_in[3]);

		atp->desc.lead_out[1] = from_bcd(atp->desc.lead_out[1]);
		atp->desc.lead_out[2] = from_bcd(atp->desc.lead_out[2]);
		atp->desc.lead_out[3] = from_bcd(atp->desc.lead_out[3]);
	}

	return (ret);
}
//--------------------------------------------------------------------
void	di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp)
{
	dsp->ds_diskid = a_to_u_4_byte(dip->disk_id);
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

	dsp->ds_maxblocks = msf_to_lba(dip->last_lead_out[1],
					dip->last_lead_out[2],
					dip->last_lead_out[3], TRUE);
	/*
	 * Check for 0xFF:0xFF/0xFF which is an indicator for a complete disk
	 */
	if (dsp->ds_maxblocks == 1166730)
		dsp->ds_maxblocks = -1L;

	dsp->ds_first_leadin = msf_to_lba(dip->last_lead_in[1],
					dip->last_lead_in[2],
					dip->last_lead_in[3], FALSE);
	if (dsp->ds_first_leadin > 0)
		dsp->ds_first_leadin = 0;

	if (dsp->ds_last_leadout == 0 && dsp->ds_maxblocks >= 0)
		dsp->ds_last_leadout = dsp->ds_maxblocks;
}
//--------------------------------------------------------------------
__s32	prdiskstatus_mmc(St_GrScsiIf *scgp, cdr_t *dp)
{
	return (prdiskstatus(scgp, dp, TRUE));
}
//--------------------------------------------------------------------
__s32	init_mmc(St_GrScsiIf *scgp, cdr_t *dp)
{
	return (speed_select_mmc(scgp, dp, NULL));
}
//--------------------------------------------------------------------

