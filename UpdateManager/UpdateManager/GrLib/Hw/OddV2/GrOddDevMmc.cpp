/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevMmc.h>
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
//local macro

//====================================================================
//local type
char	clv_to_speed[16] = {
	/*		0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 */
	0, 2, 4, 6, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char	hs_clv_to_speed[16] = {
	/*		0  1  2  3  4  5  6  7   8  9 10 11 12 13 14 15 */
	0, 2, 4, 6, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char	us_clv_to_speed[16] = {
	/*		0  1  2  3  4  5   6  7  8   9   10  11 12 13 14 15 */
	0, 2, 4, 8, 0, 0, 16, 0, 24, 32, 40, 48, 0, 0, 0, 0
};

struct gr {
	unsigned char	val;
	signed char	vadd;
	char	*name;
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

unsigned char	db2df[] = {
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
extern Cls_GrOddDevBase* V_PtrDevBd;
extern Cls_GrOddDevBase* V_PtrDevBdRom;
extern Cls_GrOddDevBase* V_PtrDevBdR;
extern Cls_GrOddDevBase* V_PtrDevBdRE;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevMmc::Cls_GrOddDevMmc():
Cls_GrOddDevBase()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_SWABAUDIO;
		m_Flags2	= 0;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_SAO;
		m_SpeedDef	= 372;
		m_SpeedMax	= 372;
		m_DrName		= "mmc_cdr";
		m_DrText		= "generic St_GrScsiIf-3/mmc CD-R/CD-RW driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevMmc::~Cls_GrOddDevMmc()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevMmc::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		BOOL	cdrr	 = FALSE;	/* Read CD-R	*/
		BOOL	cdwr	 = FALSE;	/* Write CD-R	*/
		BOOL	cdrrw	 = FALSE;	/* Read CD-RW	*/
		BOOL	cdwrw	 = FALSE;	/* Write CD-RW	*/
		BOOL	dvdwr	 = FALSE;	/* DVD writer	*/
		BOOL	is_dvd	 = FALSE;	/* use DVD driver*/
		unsigned char	mode[0x100];
		struct	cd_mode_page_2A *mp;
		int	profile;
		struct scsi_inquiry* ip;

		ip	=	&scgp->inq;

		if (ip->type != INQ_WORM && ip->type != INQ_ROMD)
		{
			return NULL;
		}
	
		allow_atapi(scgp, TRUE);
	
		mp = mmc_cap(scgp, mode);
		if (mp == NULL)
		{
			return V_PtrDevOldCd;
		}
	
		if (ip->data_format < 2)
			ip->data_format = 2;
	
		if (strncmp(ip->vendor_info, "SONY", 4) == 0 &&
				strncmp(ip->prod_ident, "CD-R   CDU928E", 14) == 0)
		{
			return V_PtrDevMmcSony;
		}
	
		profile = get_curprofile(scgp);

		if (profile == 0) {

			load_media(scgp, dp, FALSE);
			profile = get_curprofile(scgp);
			scsi_prevent_removal(scgp, 0);
		}
		if (profile >= 0)
		{
			if (profile == 0 || profile > 0x19) {
				is_dvd = FALSE;

				dp = V_PtrDevCd;;
	
				if (profile == 0)
				{
					//no medium
					BOOL	is_cdr = FALSE;
	
					get_wproflist(scgp, &is_cdr, NULL,
								NULL, NULL);
					if (is_cdr)
					{
						return V_PtrDevMmc;
					}
					return (dp);
				}

				if (profile <= 0x1F || /* DVD+RW DVD+R */
					profile == 0x2B) /* DVD+R/DL */
				{
					dp = V_PtrDevDvdPlus;
					dp = dp->Identify(scgp, dp);
					return (dp);
				}
				else if(profile >= 0x40 && profile <= 0x4F)
				{
					DbgMsgPrint("Cls_GrOddDevMmc::Identify() - Bd\n");
					dp = V_PtrDevBd;
					dp = dp->Identify(scgp, dp);

					return (dp);
				}
				else if (profile >= 0x50 && profile <= 0x5F)
				{
					DbgMsgPrint("Cls_GrOddDevMmc::Identify() - Found unsupported HD-DVD 0x%X profile.\n");
					return NULL;
				}
				else
				{
					DbgMsgPrint("Cls_GrOddDevMmc::Identify() - Found unsupported 0x%X profile.\n", profile);
					return NULL;
				}
			}
			else if (profile >= 0x10 && profile <= 0x19)
			{
				dp = V_PtrDevDvd;
				dp = dp->Identify(scgp, dp);
				return (dp);
			}
		}
	
		mmc_getval(mp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, &dvdwr);
	
		if (!cdwr && !cdwrw)
		{
			dp = V_PtrDevCd;
		}
		else
		{
			dp = V_PtrDevMmc;
		}
	
		if (profile < 0 && ((cdwr || cdwrw) && dvdwr)) 
		{
			char	xb[32];
	

			if (read_dvd_structure(scgp, (caddr_t)xb, 32, 0, 0, 0, 0) >= 0)
			{
				is_dvd = TRUE;
			}
			else
			{
				if (scgp->Scg_Sense_Key() == SC_NOT_READY)
				{
					load_media(scgp, dp, FALSE);
					if (read_dvd_structure(scgp, (caddr_t)xb, 32, 0, 0, 0, 0) >= 0)
					{
						is_dvd = TRUE;
					}
					scsi_prevent_removal(scgp, 0);
				}
			}
		}

		if (is_dvd)
		{
			dp = V_PtrDevDvd;
			dp = dp->Identify(scgp, dp);

			return (dp);
		}

		return (dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::attach(void* A_PtrScg)
{
		Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
		int	ret;
		unsigned char	mode[0x100];
		struct	cd_mode_page_2A *mp;
		struct	ricoh_mode_page_30 *rp = NULL;

		allow_atapi(scgp, TRUE); /* Try to switch to 10 byte mode cmds */
	
		mp = mmc_cap(scgp, NULL); /* Get MMC capabilities in allocated mp */
		if (mp == NULL)
			return (-1);	/* Pre St_GrScsiIf-3/mmc drive		*/
	
		m_CdCap = mp;	/* Store MMC cap pointer	*/
	
		m_Dstat->ds_dr_max_rspeed = a_to_u_2_byte(mp->max_read_speed)/176;
		if (m_Dstat->ds_dr_max_rspeed == 0)
			m_Dstat->ds_dr_max_rspeed = 372;
		m_Dstat->ds_dr_cur_rspeed = a_to_u_2_byte(mp->cur_read_speed)/176;
		if (m_Dstat->ds_dr_cur_rspeed == 0)
			m_Dstat->ds_dr_cur_rspeed = 372;
	
		m_Dstat->ds_dr_max_wspeed = a_to_u_2_byte(mp->max_write_speed)/176;
		if (mp->p_len >= 28)
			m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->v3_cur_write_speed)/176;
		else
			m_Dstat->ds_dr_cur_wspeed = a_to_u_2_byte(mp->cur_write_speed)/176;
	
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
	
		if (mp->BUF != 0) {
			m_Flags |= CDR_BURNFREE;
		} else if (rp) {
			if ((m_CmdFlags & F_DUMMY) && rp->TWBFS && rp->BUEFS)
				m_Flags |= CDR_BURNFREE;
	
			if (rp->BUEFS)
				m_Flags |= CDR_BURNFREE;
		}
	
		if (mmc_isplextor(scgp)) {
			char	*p;
	
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
				m_Flags |= CDR_VARIREC;
	
			if (check_gigarec_plextor(scgp) < 0 && p == NULL)
				plextor_enable(scgp);
	
			if (check_gigarec_plextor(scgp) >= 0)
				m_Flags |= CDR_GIGAREC;
	
			if (check_ss_hide_plextor(scgp) >= 0)
				m_Flags |= CDR_SINGLESESS|CDR_HIDE_CDR;
	
			if (check_powerrec_plextor(scgp) >= 0)
				m_Flags |= CDR_FORCESPEED;
	
			if (check_speed_rd_plextor(scgp) >= 0)
				m_Flags |= CDR_SPEEDREAD;
	
			if (set_audiomaster_yamaha(scgp, this, FALSE) >= 0)
				m_Flags |= CDR_AUDIOMASTER;
		}
		if (mmc_isyamaha(scgp)) {
			if (set_audiomaster_yamaha(scgp, this, FALSE) >= 0)
				m_Flags |= CDR_AUDIOMASTER;
	
			if ((mp->p_len+2) >= (unsigned)28)
				m_Flags |= CDR_FORCESPEED;
	
			if (get_tattoo_yamaha(scgp, FALSE, 0, 0))
				m_Flags |= CDR_DISKTATTOO;
		}
	
		if (rp && rp->AWSCS)
			m_Flags |= CDR_FORCESPEED;
	
		check_writemodes_mmc(scgp, this);
	
		if ((ret = get_supported_cdrw_media_types(scgp)) < 0) {
			m_Cdrw_Support = CDR_CDRW_ALL;
			return (0);
		}
		m_Cdrw_Support = ret;
	
		return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::init(void* A_PtrScg)
{
		m_CurSpeed = 1;
		return (select_speed(A_PtrScg, NULL));
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::getdisktype(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	dstat_t	*dsp = m_Dstat;
	struct disk_info *dip;
	unsigned char	mode[0x100];
	msf_t	msf;
	BOOL	did_atip = FALSE;
	BOOL	did_dummy = FALSE;
	int	profile;

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
	if (!did_atip && dsp->ds_first_leadin < 0)
		lba_to_msf(dsp->ds_first_leadin, &msf);

	if ((m_Dstat->ds_cdrflags & RF_PRATIP) != 0 && !did_atip) 
	{
		print_min_atip(dsp->ds_first_leadin, dsp->ds_last_leadout);
	}
	dsp->ds_maxrblocks = disk_rcap(&msf, dsp->ds_maxblocks, (BOOL)dip->erasable, (BOOL)dip->uru);

	return (drive_getdisktype(scgp, this));
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::prdiskstatus(void* A_PtrScg)
{
		return (prdiskstatus_Lib(A_PtrScg, this, TRUE));
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::select_speed(void* A_PtrScg, int *speedp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	unsigned char	moder[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;
	struct	ricoh_mode_page_30 *rp = NULL;
	int	val;
	BOOL	forcespeed = FALSE;
	BOOL	dummy = (m_CmdFlags & F_DUMMY) != 0;

	if (speedp)
		m_CurSpeed = *speedp;

	deflt_writemodes_mmc(scgp, FALSE);

	memset((caddr_t)mode, '\0', sizeof(mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
			mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
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
	} else if (mmc_isplextor(scgp) && (m_Flags & CDR_FORCESPEED) != 0) {
		int	pwr;

		pwr = check_powerrec_plextor(scgp);
		if (pwr >= 0)
			forcespeed = (pwr == 0);
	} else if ((m_Flags & CDR_FORCESPEED) != 0) {
		forcespeed = rp && rp->AWSCD != 0;
	}


	if (!forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) != 0) {
		printf("Turning forcespeed on\n");
		forcespeed = TRUE;
	}
	if (forcespeed && (m_Dstat->ds_cdrflags & RF_FORCESPEED) == 0) {
		printf("Turning forcespeed off\n");
		forcespeed = FALSE;
	}
	if (mmc_isplextor(scgp) && (m_Flags & CDR_FORCESPEED) != 0) {
		powerrec_plextor(scgp, !forcespeed);
	}
	if (!mmc_isyamaha(scgp) && (m_Flags & CDR_FORCESPEED) != 0) {

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
	val = m_CurSpeed*177;
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
			m_CurSpeed = val / 176;
			*speedp = m_CurSpeed;
		}
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	track_info	track_info;
	long	next_addr;
	int	result = -1;
/*
	if(trackp != 0)
	{
		printf("test next_wr_addr_mmc - 1 trackp = %d, trackp->track = %d, flags = %d\n",
				(int)trackp, trackp->track, trackp->flags);
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
int Cls_GrOddDevMmc::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
	int	tracks = trackp->tracks;
	int	i;
	struct mmc_cue	**cuep = (mmc_cue**)vcuep;
	struct mmc_cue	*cue;
	struct mmc_cue	*cp;
	int	ncue = 0;
	int	icue = 0;
	int	pgsize;
	msf_t	m;
	int	ctl;
	int	df;
	int	scms;

	cue = (mmc_cue*)malloc(1);

	for (i = 0; i <= tracks; i++) {
		ctl = (st2mode[trackp[i].sectype & ST_MASK]) << 4;
		if (is_copy(&trackp[i]))
		{
			ctl |= TM_ALLOW_COPY << 4;
		}
		/*if(is_quadro(&trackp[i]))
		{
			ctl |= TM_QUADRO << 4;
		}*/
		//test start
		if(is_quadro(&trackp[i]))
		{
			ctl |= TM_QUADRO << 4;
		}

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
				int	idx;
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
int Cls_GrOddDevMmc::send_cue(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct mmc_cue	*cp;
	int		ncue;
	int		ret;
	__u32		i;

	for (i = 1; i <= trackp->tracks; i++) {
		if (trackp[i].tracksize < (tsize_t)0) {
			return (-1);
		}
	}
	ncue = gen_cue(trackp, &cp, FALSE);

	ret = send_cue_sheet(scgp, (caddr_t)cp, ncue*8);
	free(cp);
	if (ret < 0) {
		ncue = gen_cue(trackp, &cp, TRUE);
		ret = send_cue_sheet(scgp, (caddr_t)cp, ncue*8);

		free(cp);
	}
	return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::write_leadin(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	__u32	i;
	long	startsec = 0L;

	/*	if (flags & F_SAO) {*/
	if (wm_base(m_Dstat->ds_wrmode) == WM_SAO)
	{
		printf("Sending CUE sheet...\n");

		if (send_cue(scgp, trackp) < 0)
		{
			printf("Cannot send CUE sheet.\n");
			return (-1);
		}

		next_wr_address(scgp, &trackp[0], &startsec);
		if (trackp[0].flags & TI_TEXT) {
			startsec = m_Dstat->ds_first_leadin;
		} else if (startsec <= 0 && startsec != -150) {
			startsec = -150;
		}

		if (trackp[0].flags & TI_TEXT) {
			if (startsec > 0) {
				return (-1);
			}
			if (write_cdtext(scgp, this, startsec) < 0)
				return (-1);

			m_Dstat->ds_cdrflags |= RF_LEADIN;
		} else for (i = 1; i <= trackp->tracks; i++) {
			trackp[i].trackstart += startsec +150;
		}

	}

	if (wm_base(m_Dstat->ds_wrmode) == WM_RAW) {

		next_wr_address(scgp, &trackp[0], &startsec);
		if (startsec > -4500) {

			startsec = m_Dstat->ds_first_leadin;
		}
		if (startsec > -4500) {
			return (-1);
		}

		if (write_leadin_Lib(scgp, this, trackp, startsec) < 0)
			return (-1);
		m_Dstat->ds_cdrflags |= RF_LEADIN;
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::open_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;

	if (!is_tao(trackp) && !is_packet(trackp)) {
		if (trackp->pregapsize > 0 && (trackp->flags & TI_PREGAP) == 0) {

			pad_track(scgp, this, trackp, trackp->trackstart-trackp->pregapsize,
				(__u64)trackp->pregapsize*trackp->secsize, FALSE, 0);
		}
		return (0);
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
		strncpy((char *)&mp->ISRC[1], trackp->isrc, 12);

	} else {
		memset(&mp->ISRC[0], '\0', sizeof (mp->ISRC));
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, trackp->secsize))
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::close_track(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	ret;

	if (!is_tao(trackp) && !is_packet(trackp))
		return (0);

	if (scsi_flush_cache(scgp, (m_CmdFlags&F_IMMED) != 0) < 0) {
		return (-1);
	}
	wait_unit_ready(scgp, 300);
	if (is_packet(trackp) && !is_noclose(trackp)) {

		ret = scsi_close_tr_session(scgp, CL_TYPE_TRACK, 0xFF,
			(m_CmdFlags&F_IMMED) != 0);
		wait_unit_ready(scgp, 300);
		return (ret);
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::open_session(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	struct	cd_mode_page_05 *mp;

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len))
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
		if (m_Dstat->ds_cdrflags & RF_AUDIOMASTER)
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

	mp->multi_session = (track_base(trackp)->tracktype & TOCF_MULTI) ? MS_MULTI : MS_NONE;
	mp->session_format = toc2sess[track_base(trackp)->tracktype & TOC_MASK];

	if (trackp->isrc) {
		mp->media_cat_number[0] = 0x80;	/* Set MCN valid */
		strncpy((char *)&mp->media_cat_number[1], trackp->isrc, 13);

	} else {
		memset(&mp->media_cat_number[0], '\0', sizeof (mp->media_cat_number));
	}

	if (!set_mode_params(scgp, "CD write parameter", mode, len, 0, -1))
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::close_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::abort_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::fixate(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	ret = 0;
	int	key = 0;
	int	code = 0;
	struct timeval starttime;
	struct timeval stoptime;
	int	dummy = (track_base(trackp)->tracktype & TOCF_DUMMY) != 0;

	starttime.tv_sec = 0;
	starttime.tv_usec = 0;
	stoptime = starttime;
	gettimeofday(&starttime, (struct timezone *)0);

	if (is_tao(trackp) || is_packet(trackp)) {
		ret = scsi_close_tr_session(scgp, CL_TYPE_SESSION, 0,
			(m_CmdFlags&F_IMMED) != 0);
	} else {
		if (scsi_flush_cache(scgp, (m_CmdFlags&F_IMMED) != 0) < 0) {
			scsi_in_progress(scgp);

		}
	}
	key = scgp->Scg_Sense_Key();
	code = scgp->Scg_Sense_Code();

	unit_ready(scgp);

	if (ret >= 0) {
		wait_unit_ready(scgp, 420 / m_CurSpeed);	/* XXX Wait for ATAPI */
		waitfix_mmc(scgp, 420 / m_CurSpeed);	/* XXX Wait for ATAPI */
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
	waitfix_mmc(scgp, 420 / m_CurSpeed); /* XXX Wait for ATAPI */

	if (!dummy &&
		(ret >= 0 || (key == SC_UNIT_ATTENTION && code == 0x2E))) {

			gettimeofday(&stoptime, (struct timezone *)0);
			timevaldiff(&starttime, &stoptime);
			if (stoptime.tv_sec < (220 / m_CurSpeed)) {
				unsigned secs;


				secs = (280 / m_CurSpeed) - stoptime.tv_sec;

				sleep(secs);
			}
	}
	return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::stats(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char mode[256];
	struct	ricoh_mode_page_30 *rp;
	__u32 count;

	if (mmc_isplextor(scgp)) {
		int	sels;
		int	maxs;
		int	lasts;

		if (get_speeds_plextor(scgp, &sels, &maxs, &lasts) >= 0) {
			printf("Last selected write speed: %dx\n",	sels / 176);
			printf("Max media write speed:     %dx\n", 	maxs / 176);
			printf("Last actual write speed:   %dx\n", 	lasts / 176);
		}
	}

	if ((m_Dstat->ds_cdrflags & RF_BURNFREE) == 0)
		return (0);

	if (mmc_isplextor(scgp)) {
		int	i = 0;
		int	ret;

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
int Cls_GrOddDevMmc::blank(void* A_PtrScg, long addr, int blanktype)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	BOOL	cdrr	 = FALSE;
	BOOL	cdwr	 = FALSE;
	BOOL	cdrrw	 = FALSE;
	BOOL	cdwrw	 = FALSE;
	int	ret;

	mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw, NULL, NULL);
	if (!cdwrw)
		return (blank_dummy(scgp, this, addr, blanktype));


	ret = scsi_blank(scgp, addr, blanktype, (m_CmdFlags&F_IMMED) != 0);
	if (ret < 0)
		return (ret);

	wait_unit_ready(scgp, (90 * 60 / m_CurSpeed));
	waitfix_mmc(scgp, (90 * 60 / m_CurSpeed));
	return (ret);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::format(void* A_PtrScg, int fmtflags)
{
		return -1;
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	ret;

	ret = send_opc(scgp, bp, cnt, doopc);

	if (ret >= 0)
		return (ret);

	if (scgp->Scg_Sense_Key() == SC_UNIT_ATTENTION &&
		scgp->Scg_Sense_Code() == 0x5A &&
		scgp->Scg_Sense_Qual() == 0x03)
		return (0);

	if ((scgp->Scg_Sense_Key() == SC_RECOVERABLE_ERROR ||
		scgp->Scg_Sense_Key() == SC_MEDIUM_ERROR) &&
		scgp->Scg_Sense_Code() == 0x73 &&
		scgp->Scg_Sense_Qual() == 0x01)
		return (0);

	if (scgp->Scg_Sense_Key() != SC_ILLEGAL_REQUEST) {
		return (ret);
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::opt1(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	oflags = m_Dstat->ds_cdrflags;

	if ((m_Dstat->ds_cdrflags & RF_AUDIOMASTER) != 0) {

		if (set_audiomaster_yamaha(scgp, this, TRUE) < 0)
			return (-1);

		if (getdisktype(scgp) < 0) {
			m_Dstat->ds_cdrflags = oflags;
			return (-1);
		}
		m_Dstat->ds_cdrflags = oflags;
		if (oflags & RF_PRATIP) {
			msf_t   msf;
			lba_to_msf(m_Dstat->ds_first_leadin, &msf);

			lba_to_msf(m_Dstat->ds_maxblocks, &msf);
		}
	}
	if (mmc_isplextor(scgp)) {
		int	gcode;

		if ((m_Flags & (CDR_SINGLESESS|CDR_HIDE_CDR)) != 0) {
			if (ss_hide_plextor(scgp,
				(m_Dstat->ds_cdrflags & RF_SINGLESESS) != 0,
				(m_Dstat->ds_cdrflags & RF_HIDE_CDR) != 0) < 0)
				return (-1);
		}

		if ((m_Flags & CDR_SPEEDREAD) != 0) {
			if (speed_rd_plextor(scgp,
				(m_Dstat->ds_cdrflags & RF_SPEEDREAD) != 0) < 0)
				return (-1);
		}

		if ((m_CmdFlags & F_SETDROPTS) ||
			(wm_base(m_Dstat->ds_wrmode) == WM_SAO) ||
			(wm_base(m_Dstat->ds_wrmode) == WM_RAW))
			gcode = do_gigarec_plextor(scgp);
		else
			gcode = gigarec_plextor(scgp, 0);
		if (gcode != 0) {
			msf_t   msf;

			m_Dstat->ds_first_leadin =
				gigarec_mult(gcode, m_Dstat->ds_first_leadin);
			m_Dstat->ds_maxblocks =
				gigarec_mult(gcode, m_Dstat->ds_maxblocks);

			if (oflags & RF_PRATIP) {
				lba_to_msf(m_Dstat->ds_first_leadin, &msf);
				lba_to_msf(m_Dstat->ds_maxblocks, &msf);
			}
		}
	}
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmc::opt2(void* A_PtrScg)
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
BOOL	Cls_GrOddDevMmc::mmc_isplextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if (strncmp(scgp->inq.vendor_info, "PLEXTOR", 7) == 0) 
	{
		return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
void 	Cls_GrOddDevMmc::do_varirec_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if (check_varirec_plextor(scgp) >= 0)
		varirec_plextor(scgp, FALSE, 0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::check_varirec_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 2;
	unsigned char	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (0);
}
//--------------------------------------------------------------------
int 	Cls_GrOddDevMmc::drivemode_plextor(void* A_PtrScg, caddr_t bp, int cnt, int modecode, void *modeval)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "plextor drive mode";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::varirec_plextor(void* A_PtrScg,	BOOL on, int val)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 2;
	unsigned char	psetmode[8];
	unsigned char	pgetmode[8];

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
int		Cls_GrOddDevMmc::bpc_plextor(void* A_PtrScg, int mode, int *bpp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	char	buf[4];
	int	i;

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

	scgp->m_CmdName = "plextor read bpc";

	if (scgp->Scg_Cmd() < 0)
		return (-1);

	if (scgp->m_Scmd.resid > 2)

		return (0);

	i = a_to_u_2_byte(buf);
	if (bpp)
		*bpp = i;

	return (0);
}
//--------------------------------------------------------------------
__u32	Cls_GrOddDevMmc::gigarec_mult(int code, __u32	val)
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
int		Cls_GrOddDevMmc::gigarec_plextor(void* A_PtrScg, int val)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 4;
	unsigned char	psetmode[8];
	unsigned char	pgetmode[8];

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
int		Cls_GrOddDevMmc::do_gigarec_plextor(void* A_PtrScg)
{
	/*
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	char	*p;
	int	val = 0;	
	*/
	/* Make silly GCC happy */

	/*
	p = hasdrvopt(driveropts, "gigarec");
	if (p == NULL) 
	{
		if (check_gigarec_plextor(scgp) >= 0)
			gigarec_plextor(scgp, 0);
	} else
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
	*/ 

	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::check_gigarec_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 4;
	unsigned char	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::speed_rd_plextor(void* A_PtrScg, BOOL do_speedrd)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 0xBB;
	unsigned char	psetmode[8];
	unsigned char	pgetmode[8];
	BOOL	is_speedrd;

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
int		Cls_GrOddDevMmc::ss_hide_plextor(void* A_PtrScg, BOOL do_ss, BOOL do_hide)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 1;
	unsigned char	psetmode[8];
	unsigned char	pgetmode[8];
	BOOL	is_ss;
	BOOL	is_hide;

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
int		Cls_GrOddDevMmc::set_audiomaster_yamaha(void* A_PtrScg, Cls_GrOddDevBase *dp, BOOL keep_mode)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	int	ret = 0;
	struct	cd_mode_page_05 *mp;

	deflt_writemodes_mmc(scgp, FALSE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x05, "CD write parameter",
		mode, (unsigned char *)0, (unsigned char *)0, (unsigned char *)0, &len)) {
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
int		Cls_GrOddDevMmc::waitfix_mmc(void* A_PtrScg, int secs)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	char	dibuf[16];
	int	i;
	int	key;
#define	W_SLEEP	2

	for (i = 0; i < secs/W_SLEEP; i++) {
		if (read_disk_info(scgp, dibuf, sizeof (dibuf)) >= 0) {
			return (0);
		}
		key = scgp->Scg_Sense_Key();
		if (key != SC_UNIT_ATTENTION && key != SC_NOT_READY)
			break;
		sleep(W_SLEEP);
	}
	return (-1);
#undef	W_SLEEP
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::get_speeds_plextor(void* A_PtrScg, int *selp,	int	*maxp, int *lastp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	char	buf[10];
	int	i;

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

	scgp->m_CmdName = "plextor get speedlist";

	if (scgp->Scg_Cmd() < 0)
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
void	Cls_GrOddDevMmc::fillcue(struct mmc_cue *cp,	int	ca, int	tno, int idx,	int	dataform,	int	scms,	msf_t	*mp)
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
int		Cls_GrOddDevMmc::mmc_set_speed(void* A_PtrScg, int readspeed, int writespeed, int rotctl)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	rs;
	int	ws;
	int	ret = -1;
	int	c;
	int	k;

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
		c = scgp->Scg_Sense_Code();
		k = scgp->Scg_Sense_Key();

		if ((k != SC_ILLEGAL_REQUEST) || (c != 0x24)) {

			return (-1);
		}
		writespeed += 177;
	}


	return (ret);
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddDevMmc::mmc_isyamaha(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	if ( strncmp(scgp->inq.vendor_info, "YAMAHA", 6) == 0) 
	{
		return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::force_speed_yamaha(void* A_PtrScg, int readspeed, int writespeed)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "yamaha force cd speed";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::check_powerrec_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 0;
	unsigned char	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode2_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	if (pgetmode[2] & 1)
		return (1);

	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::powerrec_plextor(void* A_PtrScg, BOOL	do_powerrec)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 0;
	unsigned char	psetmode[8];
	unsigned char	pgetmode[8];
	BOOL	is_powerrec;
	int	speed;

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
int		Cls_GrOddDevMmc::drivemode2_plextor(void* A_PtrScg, caddr_t	bp,	int	cnt, int modecode, void	*modeval)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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
		scmd->cdb.g5_cdb.reladr = *(char *)modeval != 0 ? 1 : 0;
	else
		i_to_2_byte(&scmd->cdb.cmd_cdb[8], cnt);

	scgp->m_CmdName = "plextor drive mode2";

	if (scgp->Scg_Cmd() < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::do_tattoo_yamaha(void* A_PtrScg, FILE	*f)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	__u32 ival = 0;
	__u32 oval = 0;
	__u32	lines;
	off_t	fsize;
	char	*buf = (char*)scgp->m_PtrBuf;
	long	bufsize = scgp->m_MaxBuf;
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
BOOL	Cls_GrOddDevMmc::get_tattoo_yamaha(void* A_PtrScg, BOOL print, __u32 *irp, __u32 *orp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	unsigned char	mode[0x100];
	int	len;
	__u32 ival;
	__u32 oval;
	unsigned char	*mp;

	if (!get_mode_params(scgp, 0x31, "Yamaha Tattoo Page", mode, NULL, NULL, NULL, &len)) {
		return (FALSE);
	}

	if (len < 20)
		return (FALSE);

	mp = (unsigned char *)
		(mode + sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len);

	if ((len - ((unsigned char *)mp - mode) -1) < 10)
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
int		Cls_GrOddDevMmc::yamaha_write_buffer(void* A_PtrScg, int mode, int bufferid, long offset,
	long parlen, void *buffer, long buflen)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	unsigned char	*CDB;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (char*)buffer;
	scmd->size = buflen;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x3B;

	CDB    = (unsigned char *)scmd->cdb.cmd_cdb;
	CDB[1] = mode & 7;
	CDB[2] = bufferid;
	i_to_3_byte(&CDB[3], offset);
	i_to_3_byte(&CDB[6], parlen);

	scgp->m_CmdName = "write_buffer";

	if (scgp->Scg_Cmd() >= 0)
		return (1);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::plextor_enable(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int		ret;
	__u32	key[4];

	ret = plextor_getauth(scgp, key, sizeof (key));
	if (ret < 0)
		return (ret);

	ret = plextor_setauth(scgp, key, sizeof (key));
	return (ret);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::plextor_disable(void* A_PtrScg)
{
	return (plextor_setauth(A_PtrScg, NULL, 0));
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::plextor_getauth(void* A_PtrScg, void *dp, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->flags |= SCG_RECV_DATA;
	scmd->addr = (char*)dp;
	scmd->size = cnt;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xD4;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[10] = cnt;

	scgp->m_CmdName = "plextor getauth";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::plextor_setauth(void* A_PtrScg, void *dp, int cnt)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->addr = (char*)dp;
	scmd->size = cnt;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xD5;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] = 0x01;
	if (cnt != 0)				/* If cnt == 0 clearauth */
		scmd->cdb.cmd_cdb[2] = 0x01;
	scmd->cdb.cmd_cdb[10] = cnt;

	scgp->m_CmdName = "plextor setauth";

	if (scgp->Scg_Cmd() < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::check_ss_hide_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 1;
	unsigned char	pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (pgetmode[2] & 0x03);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::check_speed_rd_plextor(void* A_PtrScg)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	modecode = 0xBB;
	unsigned char pgetmode[8];

	memset(pgetmode, '\0', sizeof (pgetmode));
	if (drivemode_plextor(scgp, (caddr_t)pgetmode, sizeof (pgetmode), modecode, NULL) < 0) {
		return (-1);
	}

	return (pgetmode[2] & 0x01);
}
//--------------------------------------------------------------------
int		Cls_GrOddDevMmc::get_atip(void* A_PtrScg, struct atipinfo *atp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	len;
	int	ret;

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
void	Cls_GrOddDevMmc::di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp)
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
