/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevBd.h>
#include <Hw/OddV2/GrOddLib.h>
#include <Hw/OddV2/GrOddScg.h>
#include <GrDumyTool.h>
#include <GrDebug.h>

#ifdef LINUX_APP
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#endif

//====================================================================
//local const
#define	E_GrOddDevBdMaxTrack				99	/* Red Book track limit */

#define	MAX_TRIES				15
#define	FMTDAT					0x10
#define	CMPLST					0x08

#define	FCAP_TYPE_FULL							0x00	/* Full Format			*/
#define	FCAP_TYPE_EXPAND_SPARE			0x01	/* Spare area expansion		*/
#define	FCAP_TYPE_ZONE_REFORMAT			0x04	/* DVD-RAM Zone Reformat	*/
#define	FCAP_TYPE_ZONE_FORMAT				0x05	/* DVD-RAM Zone Format		*/
#define	FCAP_TYPE_CDRW_FULL					0x10	/* CD-RW/DVD-RW Full Format	*/
#define	FACP_TYPE_CDRW_GROW_SES			0x11	/* CD-RW/DVD-RW grow session	*/
#define	FACP_TYPE_CDRW_ADD_SES			0x12	/* CD-RW/DVD-RW add session	*/
#define	FACP_TYPE_DVDRW_QGROW				0x13	/* DVD-RW quick grow last border*/
#define	FACP_TYPE_DVDRW_QADD_SES		0x14	/* DVD-RW quick add session	*/
#define	FACP_TYPE_DVDRW_QUICK				0x15	/* DVD-RW quick interm. session	*/
#define	FCAP_TYPE_FULL_SPARE				0x20	/* Full Format with sparing	*/
#define	FCAP_TYPE_MRW_FULL					0x24	/* CD-RW/DVD+RW Full Format	*/
#define	FCAP_TYPE_DVDPLUS_BASIC			0x26	/* DVD+RW Basic Format		*/
#define	FCAP_TYPE_DVDPLUS_FULL			0x26	/* DVD+RW Full Format		*/
#define	FCAP_TYPE_BDRE_SPARE				0x30	/* BD-RE Full Format with spare	*/
#define	FCAP_TYPE_BDRE							0x31	/* BD-RE Full Format without spare*/
#define	FCAP_TYPE_BDR_SPARE					0x32	/* BD-R Full Format with spare	*/

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern Cls_GrOddDevBase* V_PtrDevBd;
extern Cls_GrOddDevBase* V_PtrDevBdRom;
extern Cls_GrOddDevBase* V_PtrDevBdR;
extern Cls_GrOddDevBase* V_PtrDevBdRE;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevBd::Cls_GrOddDevBd():
Cls_GrOddDevBase()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= 0;
		m_Flags2	= CDR2_NOCD|CDR2_BD;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_NONE;
		m_SpeedDef	= 1000;
		m_SpeedMax	= 1000;
		m_DrName		= "mmc_bd";
		m_DrText		= "generic SCSI-3/mmc-3 BD-R/BD-RE driver (checks media)";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevBd::~Cls_GrOddDevBd()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevBd::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		struct scsi_inquiry* ip;
		unsigned char	mode[0x100];
		struct	cd_mode_page_2A *mp;
		int	profile;
		DbgMsgPrint("Cls_GrOddDevBd::Identify() - 1\n");
		ip	=	&scgp->inq;

		if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		{
			return NULL;
		}
	
		allow_atapi(scgp, TRUE);
	
		mp = mmc_cap(scgp, mode);
		if (mp == NULL)
		{
			return NULL;
		}
	
		if (ip->data_format < 2)
			ip->data_format = 2;
	
		profile = get_curprofile(scgp);

		if(profile == 0x0043)
		{DbgMsgPrint("Cls_GrOddDevBd::Identify() - V_PtrDevBdRE\n");
			dp = V_PtrDevBdRE;
		}
		else if((profile == 0x0041) || (profile == 0x0042))
		{DbgMsgPrint("Cls_GrOddDevBd::Identify() - V_PtrDevBdR\n");
			dp = V_PtrDevBdR;
		}
		else if(profile == 0x0040)
		{DbgMsgPrint("Cls_GrOddDevBd::Identify() - V_PtrDevBdRom\n");
			dp = V_PtrDevBdRom;
		}
		else
		{
			DbgMsgPrint("Found unsupported 0x%X profile.\n", profile);
			return NULL;
		}

		return (dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::attach(void* A_PtrScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		int	ret;
		unsigned char	mode[0x100];
		struct	cd_mode_page_2A *mp;
		struct	ricoh_mode_page_30 *rp = NULL;
		__u32	xspeed;
		__u32	mp2Aspeed;
		DbgMsgPrint("Cls_GrOddDevBd::attach()\n");
		allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */
	
		mp = mmc_cap(scgp, NULL); /* Get MMC capabilities in allocated mp */
		if (mp == NULL)
			return (-1);	/* Pre St_GrScsiIf-3/mmc drive		*/
	
		m_CdCap = mp;	/* Store MMC cap pointer	*/
	
		m_Dstat->ds_dr_max_rspeed = a_to_u_2_byte(mp->max_read_speed)/4495;
		if (m_Dstat->ds_dr_max_rspeed == 0)
			m_Dstat->ds_dr_max_rspeed = 14;
		m_Dstat->ds_dr_cur_rspeed = a_to_u_2_byte(mp->cur_read_speed)/4495;
		if (m_Dstat->ds_dr_cur_rspeed == 0)
			m_Dstat->ds_dr_cur_rspeed = 14;
	
		m_Dstat->ds_dr_max_wspeed = a_to_u_2_byte(mp->max_write_speed)/4495;
		if (mp->p_len >= 28)
			m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->v3_cur_write_speed)/4495;
		else
			m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->cur_write_speed)/4495;

		xspeed = 0;
		scsi_get_perf_maxspeed(scgp, NULL, &xspeed, NULL);

		mp2Aspeed = a_to_u_2_byte(mp->max_write_speed);

		if ((is_cdspeed(mp2Aspeed) && !is_cdspeed(xspeed)) || (mp2Aspeed < 10000 && xspeed > 10000))
		{
			m_Dstat->ds_dr_max_wspeed = xspeed/4495;
			xspeed = 0;
			scsi_get_perf_curspeed(scgp, NULL, &xspeed, NULL);

			if(xspeed > 0)
			{
				m_Dstat->ds_dr_cur_wspeed = xspeed / 4495;
			}
		}

		if (m_SpeedMax > m_Dstat->ds_dr_max_wspeed)
			m_SpeedMax = m_Dstat->ds_dr_max_wspeed;

		if (m_SpeedDef > m_SpeedMax)
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
		{
			m_Flags |= CDR_BURNFREE;
		}

		check_writemodes_mmc(scgp, this);

		m_Flags &= ~(CDR_RAW|CDR_RAW16|CDR_RAW96P|CDR_RAW96R|CDR_SRAW96P|CDR_SRAW96R);
		m_Flags &= ~(CDR_TAO);

		if (mp->BUF != 0)
		{
			m_Flags |= CDR_BURNFREE;
		}
		else if (rp)
		{
			if ((m_Flags & F_DUMMY) && rp->TWBFS && rp->BUEFS)
				m_Flags |= CDR_BURNFREE;

			if (rp->BUEFS)
				m_Flags |= CDR_BURNFREE;
		}

		if (rp && rp->AWSCS)
			m_Flags |= CDR_FORCESPEED;

		if ((m_Flags & (CDR_SAO)) != (CDR_SAO))
		{
			m_Flags |= CDR_SAO;
		}

		/*if (driveropts != NULL) {
		char	*p;

		if (strcmp(driveropts, "help") == 0) {
			mmc_opthelp(scgp, dp, 0);
		}

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
	}*/

		if (scgp->m_TimeOut < 100)
		scgp->TimeOut(100);	/* 1:40			*/

		return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::init(void* A_PtrScg)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::getdisktype(void* A_PtrScg)
{
		return drive_getdisktype(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::prdiskstatus(void* A_PtrScg)
{
		return no_diskstatus(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::select_speed(void* A_PtrScg, int *speedp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::open_track(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::close_track(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::close_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::abort_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return NULL;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
void Cls_GrOddDevBd::di_to_dstat(disk_info* dip, dstat_t* dsp)
{
	dsp->ds_diskid = a_to_u_4_byte(dip->disk_id);

	dsp->ds_flags |= DSF_NOCD|DSF_BD;	/* This is a BD */

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

#ifdef	nono
	/*
	 * On BD systems, there is no lead out start time
	 * in the disk info because there is no time based data.
	 */
	dsp->ds_maxblocks = msf_to_lba(dip->last_lead_out[1],
					dip->last_lead_out[2],
					dip->last_lead_out[3], TRUE);
#endif
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::init_bd(void* A_PtrScg)
{
		return (speed_select_bd(A_PtrScg, NULL));
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::getdisktype_bd(void* A_PtrScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		dstat_t	*dsp = m_Dstat;
		struct disk_info *dip;
		unsigned char mode[0x100];
		struct rzone_info *rp;
		struct dvd_structure_00 *sp;
		int	len;
		BOOL	did_dummy = FALSE;
		BOOL	did_reload = FALSE;
		int	profile;
		int	maxblocks;
		unsigned int	end_lba;

	again:
		dip = (struct disk_info *)scgp->m_PtrBuf;
		if (get_diskinfo(scgp, dip, sizeof (*dip)) < 0)
			return (-1);

		/*
		 * Check for non writable disk first.
		*/
		if (dip->disk_status == DS_COMPLETE &&
				(dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == RF_WRITE)
		{
			profile = get_curprofile(scgp);
			if (profile == 0x0043) {	/* This is a BD-RE */
				if (m_Flags & F_FIX)
					return (0);
			}

			if (!did_dummy)
			{
				int	xspeed = 0xFFFF;
				int	oflags = m_Flags;

				m_Flags &= ~F_DUMMY;
				speed_select_bd(scgp, &xspeed);
				m_Flags = oflags;

				rezero_unit(scgp);

				did_dummy = TRUE;
				goto again;
			}
			if (!did_reload && profile == 0x0043)
			{	/* This is a BD-RE */
				len = read_scsi(scgp, (caddr_t)scgp->m_PtrBuf, 0, 1);
				if (len < 0)
				{
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
		if (profile == 0x0043)
		{
			dsp->ds_flags |= DSF_BD_RE;		/* This is a BD-RE  */
			if (dip->disk_status == DS_EMPTY)
			{	/* Unformatted	    */
				dsp->ds_flags |= DSF_NEED_FORMAT;
				return (0);
			}
		}
		else if (profile == 0x0041 || profile == 0x0042)
		{
			dsp->ds_flags |= DSF_BD;		/* This is a BD-R */
		}

		memset((caddr_t)mode, '\0', sizeof (mode));
		rp = (struct rzone_info *)mode;
		read_rzone_info(scgp, (caddr_t)rp, sizeof (struct rzone_info));

		if ((m_Dstat->ds_cdrflags & RF_PRATIP) != 0)
		{
			if (((dsp->ds_cdrflags & (RF_WRITE|RF_BLANK)) == 0))
			{
				//przone(rp);
				print_format_capacities(scgp);
			}
		}

		dsp->ds_maxblocks = a_to_u_4_byte(rp->rzone_size);

		if (dsp->ds_maxblocks == 0)
			dsp->ds_maxblocks = a_to_u_4_byte(rp->free_blocks);

		if (rp->nwa_v)
			dsp->ds_maxblocks += a_to_u_4_byte(rp->next_recordable_addr);
		maxblocks = dsp->ds_maxblocks;

		if (dip->disk_status == DS_COMPLETE && profile != 0x0043)
			return (drive_getdisktype(scgp, this));

		memset((caddr_t)mode, '\0', sizeof (mode));
		if (read_dvd_structure(scgp, (caddr_t)mode, 2, 1, 0, 0, 0) < 0)
		{
			DbgMsgPrint("Cls_GrOddDevBd::getdisktype_bd - Cannot read BD structure.\n" );
			return (drive_getdisktype(scgp, this));
		}
		len = a_to_u_2_byte(mode);
		len += 2;			/* Data len is not included */

		if (len > sizeof (struct dvd_structure_00))
		{
			len = sizeof (struct dvd_structure_00);

			len += 1;
			len &= ~1;
		}
		memset((caddr_t)mode, '\0', sizeof (mode));
		sp = (struct dvd_structure_00 *)mode;
		read_dvd_structure(scgp, (caddr_t)sp, len, 1, 0, 0, 0);

		if ((profile != 0x0043) && (a_to_u_3_byte(sp->phys_end) != 0) &&
				(dsp->ds_maxblocks != (long)(a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1)))
		{
			DbgMsgPrint("Cls_GrOddDevBd::getdisktype_bd - WARNING: Phys disk size %ld differs from rzone size %ld! Prerecorded disk?\n",
				(long)(a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1),
				(long)dsp->ds_maxblocks);
			DbgMsgPrint("Cls_GrOddDevBd::getdisktype_bd - WARNING: Phys start: %ld Phys end %ld\n",
				(long)a_to_u_3_byte(sp->phys_start), (long)a_to_u_3_byte(sp->phys_end));
		}

		if ((long)dsp->ds_maxblocks == 0)
		{
			dsp->ds_maxblocks = a_to_u_3_byte(sp->phys_end) - a_to_u_3_byte(sp->phys_start) + 1;
		}

		end_lba = 0L;
		scsi_get_perf_maxspeed(scgp, (unsigned int*)NULL, (unsigned int*)NULL, &end_lba);

		if ((unsigned int)end_lba > dsp->ds_maxblocks)
		{
			if (maxblocks == 0)
				DbgMsgPrint("Cls_GrOddDevBd::getdisktype_bd - WARNING: Drive returns zero media size, correcting.\n");
			dsp->ds_maxblocks = end_lba + 1;
		}

		return (drive_getdisktype(scgp, this));
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::prdiskstatus_bd(void* A_PtrScg)
{
		return (prdiskstatus_Lib(A_PtrScg, this, FALSE));
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::next_wr_addr_bdre(void* A_PtrScg, track_t* trackp, long* ap)
{
		if (trackp == 0 || trackp->track <= 1)
		{
			m_DvdNextAddr = 0;
		}
		if (ap)
			*ap = m_DvdNextAddr;

		return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::next_wr_addr_bdr(void* A_PtrScg, track_t* trackp, long* ap)
{
		struct disk_info	di;
		struct rzone_info	rz;
		int			tracks;
		long			next_addr = -1;

		if (trackp == 0)
		{
			memset(&di, '\0', sizeof(di));
			if (get_diskinfo(A_PtrScg, &di, sizeof (di)) < 0)
				return (-1);

			tracks = di.last_track_ls + di.last_track_ls_msb * 256;
			memset(&rz, '\0', sizeof(rz));

			if (get_trackinfo(A_PtrScg, (caddr_t)&rz, TI_TYPE_TRACK, tracks, sizeof (rz)) < 0)
				return (-1);
			if (!rz.nwa_v)
				return (-1);
			next_addr = a_to_4_byte(rz.next_recordable_addr);
			if (ap)
				*ap = next_addr;
			return (0);
		}
		if (trackp->track <= 1)
		{
			memset(&rz, '\0', sizeof(rz));
			read_rzone_info(A_PtrScg, (caddr_t)&rz, sizeof (struct rzone_info));
			m_DvdNextAddr = a_to_4_byte(rz.next_recordable_addr);
		}
		if (ap)
			*ap = m_DvdNextAddr;

		return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::open_track_bd(void* A_PtrScg, track_t* trackp)
{
		unsigned char	mode[0x100];
		int	len;
		long	sectors;
		struct	cd_mode_page_05 *mp;

		if (trackp->track > 1)	/* XXX Hack to make one 'track' from several */
			return (0);	/* XXX files in Disk at once mode only.	    */

		memset((caddr_t)mode, '\0', sizeof(mode));

		if (!get_mode_params(A_PtrScg, 0x05, "CD write parameter",
				mode, (unsigned char*)0, (unsigned char*)0, (unsigned char*)0, &len))
			return (-1);

		if (len == 0)
			return (-1);

		mp = (struct cd_mode_page_05 *)
			(mode + sizeof (struct scsi_mode_header) +
			((struct scsi_mode_header *)mode)->blockdesc_len);

		if (!set_mode_params(A_PtrScg, "CD write parameter", mode, len, 0, trackp->secsize))
			return (-1);

		sectors = rzone_size(trackp);

		return (0);
}
//--------------------------------------------------------------------
unsigned int Cls_GrOddDevBd::rzone_size(track_t *trackp)
{
		int	i;
		BOOL	vtracks = FALSE;
		long	sectors = 0L;
		__u64	ttrsize = 0L;
		__u64	tamount = 0L;
		__u64	amount;
		long	secsize = trackp->secsize;

		for (i = 0; i < E_GrOddDevBdMaxTrack; i++) {
			if (is_last(&trackp[i]))
				break;
		}
		if (i >= 1)
			vtracks = TRUE;
		if (vtracks)
			DbgMsgPrint("Cls_GrOddDevBd::rzone_size() - Compiling virtual track list ...\n");

		for (i = 0; i < E_GrOddDevBdMaxTrack; i++)
		{
			if (trackp[i].tracksize < (tsize_t)0)
			{
				DbgMsgPrint("Cls_GrOddDevBd::rzone_size() - VTrack %d has unknown length.\n", i);
				return (-1);
			}
			amount = roundup(trackp[i].tracksize, secsize);
			amount += (__u64)trackp[i].padsecs * secsize;
			sectors += amount/secsize;
			ttrsize += trackp[i].tracksize;
			tamount += amount;
			if (vtracks)
			{
				DbgMsgPrint("Cls_GrOddDevBd::rzone_size() - Vtrack:  %d size: %lld bytes %lld rounded (%lld sectors)\n", 
					(int)trackp[i].track, (__u64)trackp[i].tracksize, amount, amount / (__u64)secsize);
			}

			if (is_last(&trackp[i]))
				break;

			if (trackp[i].tracksize % secsize)
			{
				DbgMsgPrint("Cls_GrOddDevBd::rzone_size() - Virtual track %d is not a multiple of secsize.\n", (int)trackp[i].track);
			}
		}

		if (vtracks)
		{
			DbgMsgPrint("Cls_GrOddDevBd::rzone_size() - Vtracks: %d size: %lld bytes %lld rounded (%ld sectors) total\n",
				i+1, ttrsize, tamount, sectors);
		}

		return (sectors);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::close_track_bd(void* A_PtrScg, track_t *trackp)
{
		unsigned int sectors = 0L;
		__u64	amount;
		unsigned int secsize = trackp->secsize;

		amount = roundup(trackp->tracksize, secsize);
		amount += (__u64)trackp->padsecs * secsize;
		sectors += amount/secsize;

		m_DvdNextAddr += sectors;

		return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::fixate_bdre(void* A_PtrScg, track_t *trackp)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		int	oldtimeout = scgp->m_TimeOut;
		int	ret = 0;
		int	i;

		if (scgp->m_TimeOut < 1000)
			scgp->TimeOut(1000);

		for (i = 0; i <= MAX_TRIES; i++)
		{
			if (scsi_flush_cache(scgp, TRUE) < 0) {
				if (!scsi_in_progress(scgp) || i >= MAX_TRIES)
				{
					scgp->TimeOut(oldtimeout);
					return (-1);
				}
				sleep(1);
			} else {
				break;
			}
		}

		waitformat_bd(scgp, 300);

		waitformat_bd(scgp, 300);

		scgp->TimeOut(oldtimeout);
		return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::fixate_bdr(void* A_PtrScg, track_t *trackp)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		int	oldtimeout = scgp->m_TimeOut;
		int	ret = 0;
		int	key = 0;
		int	code = 0;
		int	trackno;
		int	i;

		if (scgp->m_TimeOut < 1000)
			scgp->TimeOut(1000);

		for (i = 0; i <= MAX_TRIES; i++)
		{
			if (scsi_flush_cache(scgp, TRUE) < 0)
			{
				if (!scsi_in_progress(scgp) || i >= MAX_TRIES)
				{
					scgp->TimeOut(oldtimeout);
					return (-1);
				}
				sleep(1);
			} else {
				break;
			}
		}

		key = scgp->Scg_Sense_Key();
		code = scgp->Scg_Sense_Code();
		waitformat_bd(scgp, 600);

		trackno = trackp->trackno;
		if (trackno <= 0)
			trackno = 1;

		for (i = 0; i <= MAX_TRIES; i++)
		{
			if (scsi_close_tr_session(scgp, CL_TYPE_TRACK, trackno, TRUE) < 0)
			{
				if (!scsi_in_progress(scgp) || i >= MAX_TRIES)
				{
					DbgMsgPrint("Cls_GrOddDevBd::fixate_bdr() - Trouble closing the track\n");
					break;
				}
				sleep(1);
			} else {
				break;
			}
		}

		key = scgp->Scg_Sense_Key();
		code = scgp->Scg_Sense_Code();
		waitformat_bd(scgp, 600);

		for (i = 0; i <= MAX_TRIES; i++)
		{
			if (scsi_close_tr_session(scgp, 0x06, 0, TRUE) < 0)
			{
				if (!scsi_in_progress(scgp) || i >= MAX_TRIES)
				{
					DbgMsgPrint("Cls_GrOddDevBd::fixate_bdr() - Trouble closing the last session\n");
					break;
				}
				sleep(1);
			} else {
				break;
			}
		}

		key = scgp->Scg_Sense_Key();
		code = scgp->Scg_Sense_Code();
		waitformat_bd(scgp, 600);

		scgp->TimeOut(oldtimeout);

		return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::blank_bd(void* A_PtrScg, long addr, int blanktype)
{
		BOOL	cdrr	 = FALSE;	/* Read CD-R	*/
		BOOL	cdwr	 = FALSE;	/* Write CD-R	*/
		BOOL	cdrrw	 = FALSE;	/* Read CD-RW	*/
		BOOL	cdwrw	 = FALSE;	/* Write CD-RW	*/
		BOOL	dvdwr	 = FALSE;	/* DVD writer	*/

		int	ret;

		mmc_check(A_PtrScg, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);

		ret = blank_simul(A_PtrScg, this, addr, blanktype);
		waitformat_bd(A_PtrScg, 600);
		scsi_flush_cache(A_PtrScg, TRUE);
		waitformat_bd(A_PtrScg, 600);

		return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBd::format_bd(void* A_PtrScg, int	fmtflags)
{
		char	cap_buf[4096];
		char	fmt_buf[128];
		int	len;
		int	i;
		struct scsi_cap_data		*cp;
		struct scsi_format_cap_desc	*lp;
		struct scsi_format_header	*fhp;
		long	blocks = 0;
		long	blen = 0;
		struct timeval starttime;
		struct timeval stoptime;

		len = get_format_capacities(A_PtrScg, cap_buf, sizeof (cap_buf));
		if (len < 0)
			return (-1);

		cp = (struct scsi_cap_data *)cap_buf;
		lp = cp->list;
		len -= sizeof (struct scsi_format_cap_header);
		if (lp->desc_type == 2)
		{
			if ((m_Flags & F_FORCE) == 0)
			{
				DbgMsgPrint("Cls_GrOddDevBd::fixate_bdr() - Medium is already formatted.\n");
				return (-1);
			}
		}

		for (i = len; i >= sizeof (struct scsi_format_cap_desc);i -= sizeof (struct scsi_format_cap_desc), lp++)
		{
			if (lp->fmt_type == FCAP_TYPE_BDRE_SPARE)
			{
				blocks = a_to_u_4_byte(lp->nblock);
				blen = (long)a_to_u_3_byte(lp->blen);
				break;
			}
		}
		if (blocks == 0)
		{
			DbgMsgPrint("Cls_GrOddDevBd::fixate_bdr() - BD-RE Full format with spares, capacity not found.\n");
			return (-1);
		}

		fhp = (struct scsi_format_header *)fmt_buf;
		lp = (struct scsi_format_cap_desc *)&fmt_buf[4];
		memset((caddr_t)fmt_buf, '\0', sizeof(fmt_buf));

		fhp->enable = 1;
		fhp->immed = 1;
		i_to_2_byte(fhp->length, 8);
		i_to_4_byte(lp->nblock, blocks);
		lp->fmt_type = FCAP_TYPE_BDRE_SPARE;
		lp->desc_type = 3;			/* Quick certification */
		i_to_3_byte(lp->blen, blen);

		/*
		if (lverbose)
		{
			flush();
		}*/
		starttime.tv_sec = 0;
		starttime.tv_usec = 0;
		stoptime = starttime;
		gettimeofday(&starttime, (struct timezone *)0);

		if (format_unit(A_PtrScg, fhp, /*fhp->length*/ 8 + sizeof (struct scsi_format_header),
					1, 0, 0, 0,
					blocks > 20000000 ? 11000:5300) < 0) {
			return (-1);
		}

		waitformat_bd(A_PtrScg, 300);

		return (0);
}
//--------------------------------------------------------------------
int	Cls_GrOddDevBd::waitformat_bd(void* A_PtrScg, int secs)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		unsigned char   sensebuf[CCS_SENSE_LEN];
		int	printed = 0;
		int	i;
		int	key;
		#define	W_SLEEP	2

		for (i = 0; i < secs/W_SLEEP; i++)
		{
			if (test_unit_ready(A_PtrScg) >= 0)
			{
				return (0);
			}
			key = scgp->Scg_Sense_Key();
			if (key != SC_UNIT_ATTENTION && key != SC_NOT_READY)
				break;
			request_sense_b(scgp, (caddr_t)sensebuf, sizeof (sensebuf));
	
			sleep(W_SLEEP);
		}

		return (-1);
	#undef	W_SLEEP
}
//--------------------------------------------------------------------
int	Cls_GrOddDevBd::stats_bd(void* A_PtrScg)
{
		unsigned char mode[256];
		struct	ricoh_mode_page_30 *rp;
		unsigned int count;

		if ((m_Dstat->ds_cdrflags & RF_BURNFREE) == 0)
			return (0);

		rp = get_justlink_ricoh(A_PtrScg, mode);
		if (rp)
		{
			count = a_to_u_2_byte(rp->link_counter);
		}
		return (0);
}
//--------------------------------------------------------------------
int	Cls_GrOddDevBd::format_unit(void* A_PtrScg,	void	*fmt,	int	length,	int	list_format, int dgdl, 
	int interlv, int	pattern, int	timeout)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		struct	scg_cmd	*scmd = &scgp->m_Scmd;

		memset((caddr_t)scmd, '\0', sizeof(*scmd));
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

		return scgp->Scg_Cmd();
}
//--------------------------------------------------------------------
int	Cls_GrOddDevBd::speed_select_bd(void* A_PtrScg,	int* speedp)
{
		unsigned char	mode[0x100];
		unsigned char	moder[0x100];
		int	len;
		struct	cd_mode_page_05 *mp;
		struct	ricoh_mode_page_30 *rp = NULL;
		int	val;
		unsigned int	ul;
		BOOL	forcespeed = FALSE;
		BOOL	dummy = (m_Flags & F_DUMMY) != 0;
		int	curspeed = 1;

		if (speedp)
			curspeed = *speedp;

		memset((caddr_t)mode, '\0', sizeof(*mode));

		if (!get_mode_params(A_PtrScg, 0x05, "CD write parameter",
				mode, (unsigned char*)0, (unsigned char*)0, (unsigned char*)0, &len))
			return (-1);
		if (len == 0)
			return (-1);

		mp = (struct cd_mode_page_05 *)
			(mode + sizeof (struct scsi_mode_header) +
			((struct scsi_mode_header *)mode)->blockdesc_len);

		if (get_curprofile(A_PtrScg) == 0x0043) {	/* This is a BD-RE */
			if (dummy != 0)
			{
				DbgMsgPrint("Cls_GrOddDevBd::speed_select_bd() - BD-RE has no -dummy mode.\n");
				return (-1);
			}
			if (m_CmdFlags & F_FIX)
				return (0);
		}
		mp->test_write = dummy != 0;
		mp->write_type = WT_SAO;

		if (!set_mode_params(A_PtrScg, "CD write parameter", mode, len, 0, -1))
		{
			return (-1);
		}

		if (speedp == 0)
			return (0);

		rp = get_justlink_ricoh(A_PtrScg, moder);
		if ((m_Flags & CDR_FORCESPEED) != 0)
		{
			forcespeed = rp && rp->AWSCD != 0;
		}

		if (!forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) != 0)
		{
			forcespeed = TRUE;
		}
		if (forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) == 0)
		{
			forcespeed = FALSE;
		}
		if ((m_Flags & CDR_FORCESPEED) != 0)
		{
			if (rp)
			{
				rp->AWSCD = forcespeed?1:0;
				set_mode_params(A_PtrScg, "Ricoh Vendor Page", moder, moder[0]+1, 0, -1);
				rp = get_justlink_ricoh(A_PtrScg, moder);
			}
		}

		val = curspeed*4496;
		if (val > 0x7FFFFFFF)
			val = 0x7FFFFFFF;
		if (m_Flags & CDR_MMC3)
		{
			if (speed_select_mdvd(A_PtrScg, -1, val) < 0)
				DbgMsgPrint("Cls_GrOddDevBd::speed_select_bd() - MMC-3 speed select did not work.\n");
		}
		else
		{
			if (val > 0xFFFF)
				val = 0xFFFF;

			if (scsi_set_speed(A_PtrScg, -1, val, ROTCTL_CLV) < 0)
			{
				/*
				 * Don't complain if it does not work,
				 * BD drives may not have speed setting.
				 */
			}
		}

		if (scsi_get_speed(A_PtrScg, 0, &val) >= 0)
		{
			if (val > 0)
			{
				curspeed = val / 4495;
				*speedp = curspeed;
			}
		}

		ul = 0;
		if (scsi_get_perf_curspeed(A_PtrScg, NULL, &ul, NULL) >= 0)
		{
			if (is_cdspeed(val) && !is_cdspeed(ul)) {
				curspeed = ul / 4495;
				*speedp = curspeed;
			}
		}

		return (0);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

