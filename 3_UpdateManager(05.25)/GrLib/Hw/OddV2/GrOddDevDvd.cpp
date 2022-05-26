/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevDvd.h>
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddLib.h>

#include <GrDumyTool.h>

//====================================================================
//local const
#define	E_GrOddDevDvdMaxTrack			99	/* Red Book track limit */

#define	LOG_TRESH	0		/* (Current) Treshold values	*/
#define	LOG_CUMUL	1		/* (Current) Cumulative values	*/
#define	LOG_DFL_TRESH	2		/* Default Treshold values	*/
#define	LOG_DFL_CUMUL	3		/* Default Cumulatice values	*/

//====================================================================
//local macro

//====================================================================
//local type
typedef struct scsi_log_header {	/* Log Page header		*/
	unsigned char	p_code	:6;		/* Page code			*/
	unsigned char	res	:2;		/* Reserved			*/
	unsigned char	res1;			/* Reserved			*/
	unsigned char	p_len[2];		/* Page len (n-3)		*/
} scsi_log_hdr;

typedef struct scsi_logp_header {	/* Log Parameter header		*/
	unsigned char	p_code[2];		/* Parameter code		*/
	unsigned char	lp	:1;		/* List Parameter		*/
	unsigned char	res2_1	:1;		/* Reserved			*/
	unsigned char	tmc	:2;		/* Treshold met Criteria	*/
	unsigned char	etc	:1;		/* Enable Treshold Comarison	*/
	unsigned char	tds	:1;		/* Target Save Disable		*/
	unsigned char	ds	:1;		/* Disble Save			*/
	unsigned char	du	:1;		/* Disable Update		*/
	unsigned char	p_len;			/* Parameter len (n-3)		*/
} scsi_logp_hdr;

struct scsi_logpage_0 {
	scsi_log_hdr	hdr;		/* Log Page header		*/
	unsigned char		p_code[1];	/* List of supported log pages	*/
};

struct pioneer_logpage_30_0 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned char		total_poh[4];	/* Total time powered on (hours)*/
};

struct pioneer_logpage_30_1 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned char		laser_poh[4];	/* Total time of laser turned on*/
};

struct pioneer_logpage_30_2 {
	scsi_logp_hdr	hdr;		/* Log Page header		*/
	unsigned char		record_poh[4];	/* Total time of recording	*/
};

//adip
typedef struct adip {
	unsigned char	cat_vers;		/*  0	*/
	unsigned char	disk_size;		/*  1	*/
	unsigned char	disk_struct;		/*  2	*/
	unsigned char	density;		/*  3	*/
	unsigned char	data_zone_alloc[12];	/*  4	*/
	unsigned char	mbz_16;			/* 16	*/
	unsigned char	res_17[2];		/* 17	*/
	unsigned char	man_id[8];		/* 19	*/
	unsigned char	media_id[3];		/* 27	*/
	unsigned char	prod_revision;		/* 30	*/
	unsigned char	adip_numbytes;		/* 31	*/
	unsigned char	ref_speed;		/* 32	*/
	unsigned char	max_speed;		/* 33	*/
	unsigned char	wavelength;		/* 34	*/
	unsigned char	norm_write_power;	/* 35	*/
	unsigned char	max_read_power_ref;	/* 36	*/
	unsigned char	pind_ref;		/* 37	*/
	unsigned char	beta_ref;		/* 38	*/
	unsigned char	max_read_power_max;	/* 39	*/
	unsigned char	pind_max;		/* 40	*/
	unsigned char	beta_max;		/* 41	*/
	unsigned char	pulse[14];		/* 42	*/
	unsigned char	res_56[192];		/* 56	*/
	unsigned char	res_controldat[8];	/* 248	*/
} adip_t;

//====================================================================
//local function

//====================================================================
//var
char	ill_booktype[] = "reserved book type";
char	*book_types[] = {
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

char	res_bvers[] = "reserved book version";
char	*R_vers[] = {
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

char	*RW_vers[] = {
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

char	ill_dsize[] = "illegal size";
char	*disc_sizes[] = {
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

char	ill_rate[] = "illegal rate";
char	*tr_rates[] = {
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

char	ill_layer[] = "illegal layer type";
char	*layer_types[] = {
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

char	ill_dens[] = "illegal density";
char	*ldensities[] = {
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

char	*tdensities[] = {
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
Cls_GrOddDevDvd::Cls_GrOddDevDvd():
Cls_GrOddDevBase()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_DVD|CDR_SWABAUDIO;
		m_Flags2	= CDR2_NOCD;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_SAO;
		m_SpeedDef	= 1000;
		m_SpeedMax	= 1000;
		m_DrName		= "mmc_dvd";
		m_DrText		= "generic SCSI-3/mmc-2 DVD-R/DVD-RW/DVD-RAM driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevDvd::~Cls_GrOddDevDvd()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevDvd::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	dvd	 = FALSE;	/* DVD writer	*/
	struct scsi_inquiry*	ip;
	unsigned char	mode[0x100];
	struct	cd_mode_page_2A *mp;
	int	profile;

	ip	=	&scgp->inq;

	if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		return NULL;

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

	return this;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::attach(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	struct	cd_mode_page_2A *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	__u32	xspeed;
	__u32	mp2Aspeed;

	allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */

	mp = mmc_cap(scgp, NULL); /* Get MMC capabilities in allocated mp */
	if (mp == NULL)
		return (-1);	/* Pre SCSI-3/mmc drive		*/

	m_CdCap = mp;	/* Store MMC cap pointer	*/

	m_Dstat->ds_dr_max_rspeed = a_to_u_2_byte(mp->max_read_speed)/1385;
	if (m_Dstat->ds_dr_max_rspeed == 0)
		m_Dstat->ds_dr_max_rspeed = 47;
	m_Dstat->ds_dr_cur_rspeed = a_to_u_2_byte(mp->cur_read_speed)/1385;
	if (m_Dstat->ds_dr_cur_rspeed == 0)
		m_Dstat->ds_dr_cur_rspeed = 47;

	m_Dstat->ds_dr_max_wspeed = a_to_u_2_byte(mp->max_write_speed)/1385;
	if (mp->p_len >= 28)
		m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->v3_cur_write_speed)/1385;
	else
		m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->cur_write_speed)/1385;

	xspeed = 0;
	scsi_get_perf_maxspeed(scgp, NULL, &xspeed, NULL);

	mp2Aspeed = a_to_u_2_byte(mp->max_write_speed);

	if ((is_cdspeed(mp2Aspeed) && !is_cdspeed(xspeed)) ||
	    (mp2Aspeed < 10000 && xspeed > 10000)) {
		m_Dstat->ds_dr_max_wspeed = (__u16)(xspeed/1385);
		xspeed = 0;
		scsi_get_perf_curspeed(scgp, NULL, &xspeed, NULL);
		if (xspeed > 0)
			m_Dstat->ds_dr_cur_wspeed = (__u16)(xspeed / 1385);
	}

	if (m_SpeedMax > m_Dstat->ds_dr_max_wspeed)
		m_SpeedMax = m_Dstat->ds_dr_max_wspeed;

	if (m_SpeedDef > m_SpeedMax)
		m_SpeedDef = m_SpeedMax;

	rp = get_justlink_ricoh(scgp, mode);

	if (mp->p_len >= 28)
		m_Flags |= CDR_MMC3;
	if (mp->p_len >= 24)
		m_Flags |= CDR_MMC2;
	m_Flags |= CDR_MMC;

	if (mp->loading_type == LT_TRAY)
		m_Flags |= CDR_TRAYLOAD;
	else if (mp->loading_type == LT_CADDY)
		m_Flags |= CDR_CADDYLOAD;

	if (mp->BUF != 0)
		m_Flags |= CDR_BURNFREE;

	check_writemodes_mmc(scgp, this);
	m_Flags &= ~(CDR_RAW|CDR_RAW16|CDR_RAW96P|CDR_RAW96R|CDR_SRAW96P|CDR_SRAW96R);
	m_Flags &= ~(CDR_TAO);

	if (strbeg("PIONEER", scgp->inq.vendor_info)) 
	{
		if (strbeg("DVD-RW  DVR-103", scgp->inq.prod_ident) ||
		    strbeg("DVD-R DVD-R7322", scgp->inq.prod_ident)) 
		{
			mp->BUF = 1;
		}
	}

	if (mp->BUF != 0) {
		m_Flags |= CDR_BURNFREE;
	} else if (rp) {
		if ((m_CmdFlags & F_DUMMY) && rp->TWBFS && rp->BUEFS)
			m_Flags |= CDR_BURNFREE;

		if (rp->BUEFS)
			m_Flags |= CDR_BURNFREE;
	}

	if (rp && rp->AWSCS)
		m_Flags |= CDR_FORCESPEED;


	if ((m_Flags & (CDR_SAO)) != (CDR_SAO)) {
		m_Flags |= CDR_SAO;
	}

	if (scgp->m_TimeOut < 100)
		scgp->TimeOut(100);	/* 1:40			*/

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::init(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int 	Tv_Result;

	Tv_Result 	=  select_speed(scgp, NULL);
	return Tv_Result;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::getdisktype(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	dstat_t	*dsp = m_Dstat;
	struct disk_info *dip;
	unsigned char	mode[0x100];
	struct rzone_info rz;
	struct rzone_info *rp;
	struct dvd_structure_00 *sp;
	int	profile;
	int	len;
	BOOL	did_dummy = FALSE;

	if (dsp->ds_type == DST_UNKNOWN) {
		profile = get_curprofile(scgp);
		if (profile >= 0)
			dsp->ds_type = profile;
	}

again:
	dip = (struct disk_info *)scgp->m_PtrBuf;
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);

	if (dip->disk_status == DS_COMPLETE &&
		(dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == RF_WRITE) {
			if (!did_dummy) {
				int	xspeed = 0xFFFF;
				int	oflags = m_CmdFlags;

				m_CmdFlags &= ~F_DUMMY;
				select_speed(scgp, &xspeed);
				m_CmdFlags = oflags;
				did_dummy = TRUE;
				goto again;
			}
			reload_media(scgp, this);
	}
	if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
		return (-1);
	di_to_dstat(dip, dsp);

	memset((caddr_t)mode, '\0', sizeof (mode));
	rp = (struct rzone_info *)mode;
	read_rzone_info(scgp, (caddr_t)rp, sizeof (struct rzone_info));

	if ((m_Dstat->ds_cdrflags & RF_PRATIP) != 0) 
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

	if ((m_CmdFlags & F_MSINFO) == 0 &&
		(a_to_u_3_byte(sp->phys_end) != 0) &&
		(dsp->ds_maxblocks !=
		(long)(a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1))) {

			if (dsp->ds_maxblocks == 0) {
				dsp->ds_maxblocks = a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1;
			}
	}

	return (drive_getdisktype(scgp, this));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::prdiskstatus(void* A_PtrScg)
{
		return (prdiskstatus_Lib(A_PtrScg, this, FALSE));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::select_speed(void* A_PtrScg, int *speedp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	unsigned char	moder[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	int	val;
	__u32	ul;
	BOOL	forcespeed = FALSE;
	BOOL	dummy = (m_CmdFlags & F_DUMMY) != 0;
	int	curspeed = 1;


	if (speedp)
		curspeed = *speedp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if (m_Dstat->ds_type == DST_DVD_RAM && dummy != 0) {

		return (-1);
	}

	mp->test_write = dummy != 0;

	mp->write_type = WT_SAO;
	if (m_Dstat->ds_layer_break >= 0)
		mp->write_type = WT_LAYER_JUMP;


	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);


	if (speedp == 0)
		return (0);


	rp = get_justlink_ricoh(scgp, moder);
	if ((m_Flags & CDR_FORCESPEED) != 0) {
		forcespeed = rp && rp->AWSCD != 0;
	}

	if (!forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) != 0) {
		forcespeed = TRUE;
	}
	if (forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) == 0) {
		forcespeed = FALSE;
	}
	if ((m_Flags & CDR_FORCESPEED) != 0) {

		if (rp) {
			rp->AWSCD = forcespeed?1:0;
			set_mode_params(scgp, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1);
			rp = get_justlink_ricoh(scgp, moder);
		}
	}

	val = curspeed*1390;
	if (val > 0x7FFFFFFF)
		val = 0x7FFFFFFF;
	if (m_Flags & CDR_MMC3)
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
int Cls_GrOddDevDvd::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct disk_info	di;
	struct rzone_info	rz;
	int			tracks;
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
		m_NextAddr = a_to_4_byte(rz.next_recordable_addr);
	}
	if (ap)
		*ap = m_NextAddr;
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::send_cue(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::open_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	long	sectors;
	struct	cd_mode_page_05 *mp;

	if (trackp->track > 1)	/* XXX Hack to make one 'track' from several */
		return (0);	/* XXX files in Disk at once mode only.	    */

	if (m_Dstat->ds_type == DST_DVD_RAM) {

		sectors = rzone_size(trackp);
		if (sectors < 0)
			return (-1);
		return (0);	/* No further setup needed */
	}

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
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
int Cls_GrOddDevDvd::close_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	long	sectors = 0L;
	__u64	amount;
	long	secsize = trackp->secsize;


	amount = roundup(trackp->tracksize, secsize);
	amount += (__u64)trackp->padsecs * secsize;
	sectors = sectors + (long)(amount/(__u64)secsize);

	m_NextAddr += sectors;

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::open_session(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	unsigned char	moder[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	BOOL	burnfree = FALSE;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
		return (-1);
	if (len == 0)
		return (-1);

	mp = (struct cd_mode_page_05 *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	mp->write_type = WT_SAO;
	if (m_Dstat->ds_layer_break >= 0)
		mp->write_type = WT_LAYER_JUMP;


	rp = get_justlink_ricoh(scgp, moder);

	if (m_CdCap->BUF != 0) {
		burnfree = mp->BUFE != 0;
	} else if ((m_Flags & CDR_BURNFREE) != 0) {
		burnfree = rp && rp->BUEFE != 0;
	}

	if (!burnfree && (m_Dstat->ds_cdrflags & RF_BURNFREE) != 0) {
		burnfree = TRUE;
	}
	if (burnfree && (m_Dstat->ds_cdrflags & RF_BURNFREE) == 0) {
		burnfree = FALSE;
	}
	if (m_CdCap->BUF != 0) {
		mp->BUFE = burnfree?1:0;
	} else if ((m_Flags & CDR_BURNFREE) != 0) {

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
int Cls_GrOddDevDvd::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::session_offset(void* A_PtrScg, long *soff)
{
	int 	Tv_Result;

	Tv_Result 	= sessstatus(A_PtrScg, FALSE, soff, (long *)NULL);
	return Tv_Result;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::fixate(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	oldtimeout = scgp->m_TimeOut;
	int	ret = 0;
	int	trackno;

	if (scgp->m_TimeOut < 1000)
		scgp->TimeOut(1000);

	if (scsi_flush_cache(scgp, FALSE) < 0)
	{
		scgp->TimeOut(oldtimeout);
		return (-1);
	}
	waitformat(scgp, 100);
	trackno = trackp->trackno;
	if (trackno <= 0)
		trackno = 1;

	scgp->TimeOut(oldtimeout);

	if (m_Dstat->ds_type == DST_DVD_RAM) {

		return (ret);
	}
	if (track_base(trackp)->tracktype & TOCF_MULTI)
		scsi_close_tr_session(scgp, CL_TYPE_SESSION, trackno, TRUE);

	return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::stats(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char mode[256];
	struct	ricoh_mode_page_30 *rp;
	__u32 count;

	if ((m_Dstat->ds_cdrflags & RF_BURNFREE) == 0)
		return (0);

	rp = get_justlink_ricoh(scgp, mode);
	if (rp) {
		count = a_to_u_2_byte(rp->link_counter);
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::blank(void* A_PtrScg, long addr, int blanktype)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	cdrr	 = FALSE;	/* Read CD-R	*/
	BOOL	cdwr	 = FALSE;	/* Write CD-R	*/
	BOOL	cdrrw	 = FALSE;	/* Read CD-RW	*/
	BOOL	cdwrw	 = FALSE;	/* Write CD-RW	*/
	BOOL	dvdwr	 = FALSE;	/* DVD writer	*/
	int	profile;
	extern char *blank_types[];

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);

	profile = get_curprofile(scgp);
	if (profile > 0)
		dvdwr = (profile == 0x13) || (profile == 0x14) ||
			(profile == 0x17);

	if (profile == 0x12)	/* DVD-RAM */
		return (blank_simul(scgp, this, addr, blanktype));

	if (!dvdwr)
		return (blank_dummy(scgp, this, addr, blanktype));

	/*
	if (lverbose) {
		flush();
	}
	*/

	return (scsi_blank(scgp, addr, blanktype, FALSE));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::log_sense(void* A_PtrScg, caddr_t bp, int cnt, int page, int pc, int pp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "log sense";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
BOOL Cls_GrOddDevDvd::has_log_page(void* A_PtrScg, int page, int pc)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	log[0x3F + sizeof (scsi_log_hdr)];
	int	len;
	int	i;
	struct scsi_logpage_0 *sp = (struct scsi_logpage_0 *)log;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), 0, pc, 0) < 0) {
		return (FALSE);
	}

	len = a_to_u_2_byte(sp->hdr.p_len);

	if (log_sense(scgp, (caddr_t)log, len + sizeof (scsi_log_hdr), 0, pc, 0) < 0) {
		return (FALSE);
	}

	len -= scgp->scg_getresid();

	for (i = 0; i < len; i++) {
		if (page == sp->p_code[i])
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvd::get_log(void* A_PtrScg, caddr_t bp, int *lenp, int page, int pc, int pp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char		log[sizeof (scsi_log_hdr)];
	scsi_log_hdr	*hp = (scsi_log_hdr *)log;
	int		maxlen = *lenp;
	int		len;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), page, pc, pp) < 0)
		return (-1);
	len = a_to_u_2_byte(hp->p_len);
	*lenp = len + sizeof (scsi_log_hdr);
	if ((len + (int)sizeof (scsi_log_hdr)) > maxlen)
		len = maxlen - sizeof (scsi_log_hdr);

	if (log_sense(scgp, bp, len + sizeof (scsi_log_hdr), page, pc, pp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
void Cls_GrOddDevDvd::print_logpages(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	log[0x3F + sizeof (scsi_log_hdr)];
	int	len;
	int	i;
	struct scsi_logpage_0 *sp = (struct scsi_logpage_0 *)log;

	if (log_sense(scgp, (caddr_t)log, sizeof (scsi_log_hdr), 0, LOG_CUMUL, 0) < 0) {
		return;
	}

	len = a_to_u_2_byte(sp->hdr.p_len);

	if (log_sense(scgp, (caddr_t)log, len + sizeof (scsi_log_hdr), 0, LOG_CUMUL,  0) < 0) {
		return;
	}

	len -= scgp->scg_getresid();

	printf("Supported log pages:");

	for (i = 0; i < len; i++) {
		printf(" %X", sp->p_code[i]);
	}
	printf("\n");
}
//--------------------------------------------------------------------
void Cls_GrOddDevDvd::di_to_dstat(struct disk_info *dip, dstat_t *dsp)
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
long	Cls_GrOddDevDvd::rzone_size(track_t *trackp)
{
	int	i;
	BOOL	vtracks = FALSE;
	long	sectors = 0L;
	__u64	ttrsize = 0L;
	__u64	tamount = 0L;
	__u64	amount;
	long	secsize = trackp->secsize;

	for (i = 0; i < E_GrOddDevDvdMaxTrack; i++) {
		if (is_last(&trackp[i]))
			break;
	}
	if (i >= 1)
		vtracks = TRUE;

	for (i = 0; i < E_GrOddDevDvdMaxTrack; i++)
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
			(int)trackp[i].track, (__u64)trackp[i].tracksize, amount, amount / (__u64)secsize);

		if (is_last(&trackp[i]))
			break;

	}

	return (sectors);
}
//--------------------------------------------------------------------

