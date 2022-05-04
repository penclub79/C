/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevDvdPlusBase.h>
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddLib.h>

#ifdef LINUX_APP
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#endif

//====================================================================
//local const
#define	CHANGE_SETTING	1
#define	DVD_PLUS_R	2
#define	DC_ERASE	4

#define	FMTDAT	0x10
#define	CMPLST	0x08

#define	E_GrOddDevDvdPlusBaseMaxTrack			99	/* Red Book track limit */

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
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
Cls_GrOddDevDvdPlusBase::Cls_GrOddDevDvdPlusBase():
Cls_GrOddDevBase()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevDvdPlusBase::~Cls_GrOddDevDvdPlusBase()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevDvdPlusBase::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	struct	cd_mode_page_2A *mp;
	int	profile;
	struct scsi_inquiry*	ip;

	ip	=	&scgp->inq;

	if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		return NULL;

	allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */

	mp = mmc_cap(scgp, mode); /* Get MMC capabilities */
	if (mp == NULL)
		return (NULL);	/* Pre St_GrScsiIf-3/mmc drive		*/

	if (ip->data_format < 2)
		ip->data_format = 2;

	profile = get_curprofile(scgp);

	if (profile == 0x001A)
		dp = V_PtrDevDvdPlusRW;
	if ((profile == 0x001B) || (profile == 0x002B))
		dp = V_PtrDevDvdPlusR;

	return (dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::attach(void* A_PtrScg)
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
		m_Dstat->ds_dr_max_wspeed = xspeed/1385;
		xspeed = 0;
		scsi_get_perf_curspeed(scgp, NULL, &xspeed, NULL);
		if (xspeed > 0)
			m_Dstat->ds_dr_cur_wspeed = xspeed / 1385;
	}

	if (m_SpeedMax > m_Dstat->ds_dr_max_wspeed)
		m_SpeedMax = m_Dstat->ds_dr_max_wspeed;

	if (m_SpeedDef  > m_SpeedMax)
		m_SpeedDef = m_SpeedMax;

	rp = get_justlink_ricoh(scgp, mode);

	if (mp->p_len >= 28)
		m_Flags |= CDR_MMC3;
	if (get_curprofile(scgp) >= 0)
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
	/*
	if (driveropts != NULL) 
	{
		char	*p;

		p = hasdrvopt(driveropts, "burnfree");
		if (p == NULL)
			p = hasdrvopt(driveropts, "burnproof");
		if (p != NULL && (m_Flags & CDR_BURNFREE) != 0) {
			if (*p == '1') {
				m_Dstat->ds_cdrflags |= RF_BURNFREE;
			} else if (*p == '0') {
				m_Dstat->ds_cdrflags &= ~RF_BURNFREE;
			}
		}

		p = hasdrvopt(driveropts, "forcespeed");
		if (p != NULL && *p == '1' && (m_Flags & CDR_FORCESPEED) != 0) {
			m_Dstat->ds_cdrflags |= RF_FORCESPEED;
		}

		p = hasdrvoptx(driveropts, "layerbreak", 0);
		if (p != NULL) {
			char	*ep;
			QWORD	ll;
			DWORD	lb;

			ep = astoll(p, &ll);
			lb = ll;
			if ((*ep != '\0' && *ep != ',') || *p == '\0' ||
			    ll <= 0 || ll != lb) {
				return (-1);
			}
			m_Dstat->ds_layer_break = lb;
		}
		if (m_Dstat->ds_layer_break >= 0 &&
		    get_curprofile(scgp) != 0x2B) {
			return (-1);
		}
		if (m_Dstat->ds_layer_break != -1 &&
		    m_Dstat->ds_layer_break !=
		    roundup(m_Dstat->ds_layer_break, 16)) {

			return (-1);
		}
	}
											*/

	if (scgp->m_TimeOut < 100)
		scgp->TimeOut(100);	/* 1:40			*/


	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::init(void* A_PtrScg)
{
		return (select_speed(A_PtrScg, NULL));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::getdisktype(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	//extern	char	*buf;
	dstat_t	*dsp = m_Dstat;
	struct disk_info *dip;
	unsigned char	mode[0x100];
	struct rzone_info *rp;
	struct dvd_structure_00 *sp;
	int	len;
	BOOL	did_dummy = FALSE;
	BOOL	did_reload = FALSE;
	int	profile;
	__u32	end_lba;

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

			profile = get_curprofile(scgp);
			if (profile == 0x001A) {	/* This is a DVD+RW */
				if (m_CmdFlags & F_FIX)
					return (0);
			}

			if (!did_dummy) {
				int	xspeed = 0xFFFF;
				int	oflags = m_CmdFlags;

				m_CmdFlags &= ~F_DUMMY;
				select_speed(scgp, &xspeed);
				m_CmdFlags = oflags;
				rezero_unit(scgp);
				did_dummy = TRUE;
				goto again;
			}
			if (!did_reload && profile == 0x001A) {	/* This is a DVD+RW */
				len = read_scsi(scgp, (caddr_t)scgp->m_PtrBuf, 0, 1);
				if (len < 0) {

					reload_media(scgp, this);
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
			if ((m_Dstat->ds_cdrflags & RF_PRATIP) != 0)
				print_format_capacities(scgp);
			return (0);
		}
	} else if (profile == 0x001B) {
		dsp->ds_flags |= DSF_DVD_PLUS_R;	/* This is a DVD+R */
	}

	memset((caddr_t)mode, '\0', sizeof (mode));
	rp = (struct rzone_info *)mode;
	read_rzone_info(scgp, (caddr_t)rp, sizeof (struct rzone_info));

	if ((m_Dstat->ds_cdrflags & RF_PRATIP) != 0) {
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
		return (drive_getdisktype(scgp, this));

	memset((caddr_t)mode, '\0', sizeof (mode));
	if (read_dvd_structure(scgp, (caddr_t)mode, 2, 0, 0, 0, 0) < 0) {

		return (drive_getdisktype(scgp, this));
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
		return (drive_getdisktype(scgp, this));

	if ((profile != 0x001A) &&
		(m_CmdFlags & F_MSINFO) == 0 &&
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

	return (drive_getdisktype(scgp, this));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::prdiskstatus(void* A_PtrScg)
{
		return (prdiskstatus_Lib(A_PtrScg, this, FALSE));
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::select_speed(void* A_PtrScg, int *speedp)
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


	if (dummy != 0) {
		return (-1);
	}
	if (get_curprofile(scgp) == 0x001A) {	/* This is a DVD+RW */
		if (m_CmdFlags & F_FIX)
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
	if (m_Flags & CDR_MMC3) {
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
int Cls_GrOddDevDvdPlusBase::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
	return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::open_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	long	sectors;
	struct	cd_mode_page_05 *mp;
	int	profile;

	if (trackp->track > 1)	/* XXX Hack to make one 'track' from several */
		return (0);	/* XXX files in Disk at once mode only.	    */

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

	profile = get_curprofile(scgp);
	if (profile == 0x2B) {
		if (set_p_layerbreak(scgp, sectors,
			m_Dstat->ds_layer_break) < 0) {
				return (-1);
		}
	}

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::close_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	long	sectors = 0L;
	__u32	amount;
	long	secsize = trackp->secsize;

	amount = roundup(trackp->tracksize, secsize);
	amount += (__u32)trackp->padsecs * secsize;
	sectors += amount/secsize;

	m_NextAddr += sectors;

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::stats(void* A_PtrScg)
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
int Cls_GrOddDevDvdPlusBase::blank(void* A_PtrScg, long addr, int blanktype)
{
		//extern char *blank_types[];
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	cdrr	 = FALSE;	/* Read CD-R	*/
	BOOL	cdwr	 = FALSE;	/* Write CD-R	*/
	BOOL	cdrrw	 = FALSE;	/* Read CD-RW	*/
	BOOL	cdwrw	 = FALSE;	/* Write CD-RW	*/
	BOOL	dvdwr	 = FALSE;	/* DVD writer	*/
	int	profile;

	int	flags = DC_ERASE;
	int	blanksize = 0x30000;
	int	ret;

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);

	profile = get_curprofile(scgp);

	if (!dvdplus_ricohbased(scgp)) {

		if (profile == 0x1A || profile == 0x2A) {
			ret = blank_simul(scgp, this, addr, blanktype);
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
		char	*p;

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
int Cls_GrOddDevDvdPlusBase::format(void* A_PtrScg, int fmtflags)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	char	cap_buf[4096];
	char	fmt_buf[128];
	int	len;
	int	i;
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
		if ((m_CmdFlags & F_FORCE) == 0) {
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
int Cls_GrOddDevDvdPlusBase::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::ricoh_dvdsetting(void* A_PtrScg, int erase_size, int flags, int immed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "set_dummy_dvdr_setttings";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusBase::set_p_layerbreak(void* A_PtrScg, long tsize, __u32 lbreak)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct dvd_structure_20	lb;
	int			ret;
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
BOOL Cls_GrOddDevDvdPlusBase::dvdplus_ricohbased(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
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
int Cls_GrOddDevDvdPlusBase::format_unit(void* A_PtrScg, void *fmt, int length, int list_format,
	int dgdl, int interlv, int pattern, int timeout)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "format unit";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
void	Cls_GrOddDevDvdPlusBase::di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp)
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
long	Cls_GrOddDevDvdPlusBase::rzone_size(track_t *trackp)
{
	int	i;
	BOOL	vtracks = FALSE;
	long	sectors = 0L;
	__u32	ttrsize = 0L;
	__u32	tamount = 0L;
	__u32	amount;
	long	secsize = trackp->secsize;

	for (i = 0; i < E_GrOddDevDvdPlusBaseMaxTrack; i++) {
		if (is_last(&trackp[i]))
			break;
	}
	if (i >= 1)
		vtracks = TRUE;

	for (i = 0; i < E_GrOddDevDvdPlusBaseMaxTrack; i++) {
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
			printf("Virtual track %d is not a multiple of secsize.\n", (int)trackp[i].track);
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
