/*
	cdr_drv
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
#include 	<stdlib.h>

#include	<GrTypeBase.h>
#include 	<Hw/Odd/GrSCSI.h>
#include 	<Hw/Odd/GrCdrDrv.h>
#include 	<Hw/Odd/GrScsiOpr.h>
#include 	<Hw/Odd/GrScsiMmc.h>


//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
__s32		cmd_dummy(St_GrScsiIf *scgp,	cdr_t	*dp)
{
	return (0);
}
//--------------------------------------------------------------------
__s32		cmd_ill(St_GrScsiIf	*scgp)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		format_dummy(St_GrScsiIf *scgp, cdr_t *, __s32 fmtflags)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		blank_dummy(St_GrScsiIf *scgp, cdr_t *, long addr, __s32 blanktype)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		buf_dummy(St_GrScsiIf *scgp,	long	*sp,	long	*fp)
{
	return (-1);
}
//--------------------------------------------------------------------
cdr_t*	get_cdrcmds(St_GrScsiIf *scgp)
{
	cdr_t	*dp = (cdr_t *)0;
	cdr_t	*odp = (cdr_t *)0;
	BOOL8	is_wr = FALSE;
	BOOL8	is_cd = FALSE;
	BOOL8	is_dvd = FALSE;
	BOOL8	is_dvdplus = FALSE;
	BOOL8	is_ddcd = FALSE;
	BOOL8	is_cdwr = FALSE;
	BOOL8	is_dvdwr = FALSE;
	BOOL8	is_dvdpluswr = FALSE;
	BOOL8	is_ddcdwr = FALSE;

	DbgMsgPrint("test get_cdrcmds\n");
	/*
	 * First check for St_GrScsiIf-3/mmc-3 drives.
	 */
	if (get_proflist(scgp, &is_wr, &is_cd, &is_dvd,
						&is_dvdplus, &is_ddcd) >= 0) {

		get_wproflist(scgp, &is_cdwr, &is_dvdwr,
						&is_dvdpluswr, &is_ddcdwr);
		if (!is_wr) {
			DbgMsgPrint("test cdr_cd\n");
			dp = &cdr_cd;
		} else {
			DbgMsgPrint("test cdr_cd_dvd\n");
			dp = &cdr_cd_dvd;
		}
	} else
	/*
	 * First check for SCSI-3/mmc drives.
	 */
	if (is_mmc(scgp, &is_cdwr, &is_dvdwr)) {
		DbgMsgPrint("Found MMC drive CDWR: %d DVDWR: %d.\n",	is_cdwr, is_dvdwr);

		if (is_cdwr && is_dvdwr)
		{
			DbgMsgPrint("test cdr_cd_dvd\n");
			dp = &cdr_cd_dvd;
		}
		else
		if (is_dvdwr)
		{
			printf("test cdr_dvd\n");
			dp = &cdr_dvd;
		}
		else
		{
			DbgMsgPrint("test cdr_mmc\n");
			dp = &cdr_mmc;
		}

	} else switch (scgp->dev) {

	case DEV_CDROM:
		DbgMsgPrint("test cdr_oldcd\n");
		dp = &cdr_oldcd;
		break;
	case DEV_MMC_CDROM:
		DbgMsgPrint("test cdr_cd\n");
		dp = &cdr_cd;
		break;
	case DEV_MMC_CDR:
		DbgMsgPrint("test cdr_mmc\n");
		dp = &cdr_mmc;
		break;
	case DEV_MMC_CDRW:
		DbgMsgPrint("test cdr_mmc\n");
		dp = &cdr_mmc;
		break;
	case DEV_MMC_DVD_WR:
		DbgMsgPrint("test cdr_cd_dvd\n");
		dp = &cdr_cd_dvd;
		break;
/*
	case DEV_CDD_521_OLD:	dp = &cdr_philips_cdd521O;	break;
	case DEV_CDD_521:	dp = &cdr_philips_cdd521;	break;
	case DEV_CDD_522:
	case DEV_CDD_2000:
	case DEV_CDD_2600:	dp = &cdr_philips_cdd522;	break;
	case DEV_TYUDEN_EW50:	dp = &cdr_tyuden_ew50;		break;
	case DEV_PCD_600:	dp = &cdr_kodak_pcd600;		break;
	case DEV_YAMAHA_CDR_100:dp = &cdr_yamaha_cdr100;	break;
	case DEV_MATSUSHITA_7501:dp = &cdr_cw7501;		break;*/
	case DEV_MATSUSHITA_7502:
	case DEV_YAMAHA_CDR_400:dp = &cdr_mmc;			break;
	/*case DEV_PLASMON_RF_4100:dp = &cdr_plasmon_rf4100;	break;
	case DEV_SONY_CDU_924:	dp = &cdr_sony_cdu924;		break;
	case DEV_RICOH_RO_1060C:dp = &cdr_ricoh_ro1060;		break;
	case DEV_RICOH_RO_1420C:dp = &cdr_ricoh_ro1420;		break;
	case DEV_TEAC_CD_R50S:	dp = &cdr_teac_cdr50;		break;

	case DEV_PIONEER_DW_S114X: dp = &cdr_pioneer_dw_s114x;	break;*/
	case DEV_PIONEER_DVDR_S101:
		DbgMsgPrint("test cdr_dvd\n");
		dp = &cdr_dvd;
		break;

	default:
		DbgMsgPrint("test cdr_mmc\n");
		dp = &cdr_mmc;
	}
	odp = dp;

	DbgMsgPrint("Using driver '%s' for identify.\n",	dp != NULL ? dp->cdr_drname :	"<no driver>");

	if ( NULL != dp )
	{
		dp = dp->cdr_identify( scgp, dp );
	}

	return (dp);
}
//--------------------------------------------------------------------
__s32	drive_getdisktype(St_GrScsiIf *scgp, cdr_t *dp)
{
	return (0);
}
//--------------------------------------------------------------------
__s32		no_sendcue(St_GrScsiIf	*scgp, cdr_t	*dp, track_t	*trackp)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		drive_attach(St_GrScsiIf *scgp, cdr_t *)
{
	return 0;
}
//--------------------------------------------------------------------
__s32		no_diskstatus(St_GrScsiIf	*scgp,	cdr_t	*dp)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		blank_simul(St_GrScsiIf *scgp,	cdr_t	*dp, long	addr,	__s32	blanktype)
{
	track_t	*trackp = dp->cdr_dstat->ds_trackp;
	__s32	secsize = trackp->secsize;
	__u64 padbytes = 0;			/* Make stupid GCC happy */
	__s32	ret = -1;

	switch (blanktype) {

	case BLANK_MINIMAL:
			padbytes = 1000 * secsize;
			break;
	case BLANK_DISC:
			if (dp->cdr_dstat->ds_maxblocks > 0)
				padbytes = dp->cdr_dstat->ds_maxblocks * (__u32)secsize;
			break;
	default:
			padbytes = 0;
	}
	if (padbytes > 0) {
		ret = pad_track(scgp, dp, trackp, 0, padbytes, TRUE, NULL);
	}
	if (0) {
		return (-1);
	}
	return (ret);

}
//--------------------------------------------------------------------

