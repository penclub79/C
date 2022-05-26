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
#include 	<string.h>
#endif

#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include	<Hw/Odd/GrModes.h>
#include 	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrScsiOpr.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function
BOOL8	has_mode_page(St_GrScsiIf *scgp, __s32 page, __s8 *pagename, __s32 *lenp);

//====================================================================
//var
extern BOOL8	is_atapi;
__s32	scsi_compliant;

//====================================================================
//--------------------------------------------------------------------
//--------------------------------------------------------------------
BOOL8	get_mode_params(St_GrScsiIf *scgp, __s32 page, __s8 *pagename,	unsigned __s8 *modep,
					unsigned __s8 *cmodep, unsigned __s8 *dmodep, unsigned __s8 *smodep, __s32 *lenp)
{
	// local -------------------
		__s32	len;
		BOOL8	ret = TRUE;
	// code --------------------

#ifdef LINUX_APP
		if (lenp)
			*lenp = 0;
		if (!has_mode_page(scgp, page, pagename, &len))
		{
			return (FALSE);
		}
		if (lenp)
			*lenp = len;

		if (modep) {
			memset(modep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, modep, len, page, 0) < 0) {
				ret = FALSE;
			}
		}

		if (cmodep) {
			memset(cmodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, cmodep, len, page, 1) < 0) { /* Page x change */
				ret = FALSE;
			}
		}

		if (dmodep) {
			memset(dmodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, dmodep, len, page, 2) < 0) { /* Page x default */
				ret = FALSE;
			}
		}

		if (smodep) {
			memset(smodep, '\0', 0x100);
			(void) unit_ready(scgp);
			if (mode_sense(scgp, smodep, len, page, 3) < 0) { /* Page x saved */
				ret = FALSE;
			}
		}
#else
		if ( NULL != lenp )
		{
			*lenp	=	92;
		}
#endif

	return (ret);
}
//--------------------------------------------------------------------
BOOL8	set_mode_params(St_GrScsiIf *scgp, __s8 *pagename, unsigned __s8 *modep,	__s32 len, __s32 save, __s32 secsize)
{
	__s32	i;

	((struct scsi_modesel_header *)modep)->sense_data_len	= 0;
	((struct scsi_modesel_header *)modep)->res2		= 0;

	i = ((struct scsi_mode_header *)modep)->blockdesc_len;
	if (i > 0) {
		i_to_3_byte(
			((struct scsi_mode_data *)modep)->blockdesc.nlblock,
								0);
		if (secsize >= 0)
		i_to_3_byte(((struct scsi_mode_data *)modep)->blockdesc.lblen,
							secsize);
	}

	(void) unit_ready(scgp);
	if (save == 0 || mode_select(scgp, modep, len, save, scgp->inq.data_format >= 2) < 0)
	{
		(void) unit_ready(scgp);
		if (mode_select(scgp, modep, len, 0, scgp->inq.data_format >= 2) < 0)
		{
			return (FALSE);
		}
	}
	return (TRUE);
}
//--------------------------------------------------------------------
__s32		mode_sense(St_GrScsiIf	*scgp, unsigned __s8 *dp,	__s32	cnt, __s32 page, __s32 pcf)
{
		if (is_atapi)
		{
			return (mode_sense_sg0(scgp, dp, cnt, page, pcf));
		}
		return (mode_sense_g0(scgp, dp, cnt, page, pcf));
}
//--------------------------------------------------------------------
__s32		mode_sense_sg0(St_GrScsiIf	*scgp, unsigned __s8 *dp,	__s32	cnt, __s32 page, __s32 pcf)
{
	unsigned __s8	xmode[256+4];
	__s32	amt = cnt;
	__s32	len;

	if (amt < 1 || amt > 255) {
		return (-1);
	}

	memset((caddr_t)xmode, '\0', sizeof (xmode));
	if (amt < 4) {		/* Data length. medium type & VU */
		amt += 1;
	} else {
		amt += 4;
	}
	if (mode_sense_g1(scgp, xmode, amt, page, pcf) < 0)
		return (-1);

	amt = cnt - scgp->scmd.resid;
/*
 * For tests: Solaris 8 & LG CD-ROM always returns resid == amt
 */
/*	amt = cnt;*/
	if (amt > 4)
	{
		//movebytes(&xmode[8], &dp[4], amt-4);
		GrDumyCopyMem( &dp[4], &xmode[8], amt-4);
	}
	len = a_to_u_2_byte(xmode);
	if (len == 0) {
		dp[0] = 0;
	} else if (len < 6) {
		if (len > 2)
			len = 2;
		dp[0] = len;
	} else {
		dp[0] = len - 3;
	}
	dp[1] = xmode[2];
	dp[2] = xmode[3];
	len = a_to_u_2_byte(&xmode[6]);
	dp[3] = len;


	return (0);
}
//--------------------------------------------------------------------
__s32		mode_select_g0(St_GrScsiIf	*scgp, unsigned __s8 *dp,	__s32	cnt, __s32 smp,	__s32	pf)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_MODE_SELECT;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.high_addr = smp ? 1 : 0 | pf ? 0x10 : 0;
	scmd->cdb.g0_cdb.count = cnt;

	scgp->cmdname = "mode select g0";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		mode_sense_g1(St_GrScsiIf *scgp,	unsigned __s8	*dp, __s32 cnt,	__s32	page,	__s32	pcf)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5A;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	scmd->cdb.g1_cdb.addr[0] = (page&0x3F) | ((pcf<<6)&0xC0);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "mode sense g1";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		mode_sense_g0(St_GrScsiIf *scgp, unsigned __s8 *dp, __s32 cnt, __s32 page, __s32 pcf)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = 0xFF;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_MODE_SENSE;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.mid_addr = (page&0x3F) | ((pcf<<6)&0xC0);
	scmd->cdb.g0_cdb.count = page ? 0xFF : 24;
	scmd->cdb.g0_cdb.count = cnt;

	scgp->cmdname = "mode sense g0";

	if (scg_cmd(scgp) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
BOOL8	has_mode_page(St_GrScsiIf *scgp, __s32 page, __s8 *pagename, __s32 *lenp)
{
	unsigned __s8	mode[0x100];
	__s32	hdlen;
	__s32	len = 1;
	__s32	try1 = 0;
	struct	scsi_mode_page_header *mp;

	if ((scgp->dflags & DRF_MODE_DMA_OVR) != 0)
		len = sizeof (struct scsi_mode_header);
again:
	memset((caddr_t)mode, '\0', sizeof (mode));
	if (lenp)
		*lenp = 0;

	(void) unit_ready(scgp);

	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		if (len < (__s32)sizeof (struct scsi_mode_header) && try1 == 0) {
			len = sizeof (struct scsi_mode_header);
			goto again;
		}
		return (FALSE);
	}
	else 
	{
		if (len > 1 && try1 == 0) {
			if ((scgp->dflags & DRF_MODE_DMA_OVR) == 0)
			{

				scgp->dflags |= DRF_MODE_DMA_OVR;
			}
		}
		len = ((struct scsi_mode_header *)mode)->sense_data_len + 1;
	}

	(void) unit_ready(scgp);
	if (mode_sense(scgp, mode, len, page, 0) < 0) {	/* Page n current */
		return (FALSE);
	}

	hdlen = sizeof (struct scsi_mode_header) +
			((struct scsi_mode_header *)mode)->blockdesc_len;
	mp = (struct scsi_mode_page_header *)(mode + hdlen);

	if (mp->p_len == 0)
	{
		if (!scsi_compliant && try1 == 0)
		{
			len = hdlen;

			len += 2;
			try1++;
			goto again;
		}
	}
	if (!scsi_compliant &&
	    (len < (__s32)(mp->p_len + hdlen + 2))) {
		len = mp->p_len + hdlen + 2;
	}
	if (mp->p_code != page) {
		return (FALSE);
	}

	if (lenp)
		*lenp = len;
	return (mp->p_len > 0);
}
//--------------------------------------------------------------------

