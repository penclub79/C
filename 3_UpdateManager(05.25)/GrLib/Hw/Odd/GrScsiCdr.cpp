/*
	scsi cdr
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
#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include 	<string.h>
#include 	<stdlib.h>
#include 	<errno.h>

#include	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrSCSI.h>
#include 	<Hw/Odd/GrScsiMmc.h>
#include 	<Hw/Odd/GrModes.h>
#include 	<Hw/Odd/GrScsiOpr.h>

//====================================================================
//local const
#define	G0_MAXADDR	0x1FFFFFL

//====================================================================
//local type
struct trackdesc {
	unsigned __s8	res0;

	unsigned __s8	control		: 4;
	unsigned __s8	adr		: 4;

	unsigned __s8	track;
	unsigned __s8	res3;
	unsigned __s8	addr[4];
};

struct diskinfo {
	struct tocheader	hd;
	struct trackdesc	desc[1];
};

//====================================================================
//local function

//====================================================================
//var
BOOL8	is_atapi;

//====================================================================
//--------------------------------------------------------------------
BOOL8	unit_ready(St_GrScsiIf	*scgp)
{
		struct	scg_cmd	*scmd = &scgp->scmd;
	
		if (test_unit_ready(scgp) >= 0)		/* alles OK */
			return (TRUE);
		else if (scmd->error >= SCG_FATAL)	/* nicht selektierbar */
			return (FALSE);
	
		if (scg_sense_key(scgp) == SC_UNIT_ATTENTION) {
			if (test_unit_ready(scgp) >= 0)	/* alles OK */
				return (TRUE);
		}
		if ((scg_cmd_status(scgp) & ST_BUSY) != 0)
		{
	
			usleep(500000);
			if (test_unit_ready(scgp) >= 0)	/* alles OK */
				return (TRUE);
		}
		if (scg_sense_key(scgp) == -1) {	/* non extended Sense */
			if (scg_sense_code(scgp) == 4)	/* NOT_READY */
				return (FALSE);
			return (TRUE);
		}

		return (scg_sense_key(scgp) != SC_NOT_READY);
}
//--------------------------------------------------------------------
__s32 	test_unit_ready(St_GrScsiIf *scgp)
{
		struct	scg_cmd	*scmd = &scgp->scmd;
	
		memset((caddr_t)scmd, '\0', sizeof (*scmd));
		scmd->addr = (caddr_t)0;
		scmd->size = 0;
		scmd->flags = SCG_DISRE_ENA;
		scmd->cdb_len = SC_G0_CDBLEN;
		scmd->sense_len = CCS_SENSE_LEN;
		scmd->cdb.g0_cdb.cmd = SC_TEST_UNIT_READY;
		scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	
		scgp->cmdname = "test unit ready";
	
		return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
BOOL8	getdev(St_GrScsiIf	*scgp, BOOL8	print)
{
	BOOL8	got_inquiry = TRUE;
	__s8	vendor_info[8+1];
	__s8	prod_ident[16+1];
	__s8	prod_revision[4+1];
	__s32	inq_len = 0;
	struct	scg_cmd	*scmd = &scgp->scmd;
	struct scsi_inquiry *inq = &scgp->inq;

	memset((caddr_t)inq, '\0', sizeof (*inq));
	scgp->dev = DEV_UNKNOWN;
	(void) unit_ready(scgp);
	if (scmd->error >= SCG_FATAL &&
				!(scmd->scb.chk && scmd->sense_count > 0)) {
		return (FALSE);
	}


	if (inquiry(scgp, (caddr_t)inq, sizeof (*inq)) < 0) 
	{
		got_inquiry = FALSE;
	} 
	else 
	{
		inq_len = sizeof (*inq) - scgp->scmd.resid;
	}

	if (!got_inquiry)
	{
		if (scmd->scb.chk && scmd->sense_count == 4)
		{
			(void) qic02(scgp, 0x12);

			if (qic02(scgp, 1) < 0)
			{
				scgp->dev = DEV_ACB40X0;
			}
			else
			{
				scgp->dev = DEV_SC4000;
				inq->type = INQ_SEQD;
				inq->removable = 1;
			}
		}
	} 

	strncpy(vendor_info, inq->vendor_info, sizeof (inq->vendor_info));
	strncpy(prod_ident, inq->prod_ident, sizeof (inq->prod_ident));
	strncpy(prod_revision, inq->prod_revision, sizeof (inq->prod_revision));

	vendor_info[sizeof (inq->vendor_info)] = '\0';
	prod_ident[sizeof (inq->prod_ident)] = '\0';
	prod_revision[sizeof (inq->prod_revision)] = '\0';

	switch (inq->type)
	{
	case INQ_DASD:
		if (inq->add_len == 0 && inq->vendor_info[0] != '\0') {
			unsigned __s8	*p;

			if (inq_len >= 36)
				inq->add_len = 31;

			for (p = (unsigned __s8 *)&inq->vendor_info[0];
					p < (unsigned __s8 *)&inq->prod_revision[4];
									p++) {
				if (*p < 0x20 || *p > 0x7E) {
					inq->add_len = 0;
					break;
				}
			}
		}
		if (inq->add_len == 0) {
			if (scgp->dev == DEV_UNKNOWN && got_inquiry) {
				scgp->dev = DEV_ACB5500;
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-5500        FAKE");
			} else switch (scgp->dev) {
			case DEV_ACB40X0:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-40X0        FAKE");
				break;
			case DEV_ACB4000:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4000        FAKE");
				break;
			case DEV_ACB4010:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4010        FAKE");
				break;
			case DEV_ACB4070:
				strcpy(inq->vendor_info,
					"ADAPTEC ACB-4070        FAKE");
				break;
			}
		} else if (inq->add_len < 31) {
			scgp->dev = DEV_NON_CCS_DSK;

		} else if (strbeg("EMULEX", vendor_info)) {
			if (strbeg("MD21", prod_ident))
				scgp->dev = DEV_MD21;
			if (strbeg("MD23", prod_ident))
				scgp->dev = DEV_MD23;
			else
				scgp->dev = DEV_CCS_GENDISK;
		} else if (strbeg("ADAPTEC", vendor_info)) {
			if (strbeg("ACB-4520", prod_ident))
				scgp->dev = DEV_ACB4520A;
			if (strbeg("ACB-4525", prod_ident))
				scgp->dev = DEV_ACB4525;
			else
				scgp->dev = DEV_CCS_GENDISK;
		} else if (strbeg("SONY", vendor_info) &&
					strbeg("SMO-C501", prod_ident)) {
			scgp->dev = DEV_SONY_SMO;
		} else {
			scgp->dev = DEV_CCS_GENDISK;
		}
		break;

	case INQ_SEQD:
		if (scgp->dev == DEV_SC4000) {
			strcpy(inq->vendor_info,
				"SYSGEN  SC4000          FAKE");
		} else if (inq->add_len == 0 &&
					inq->removable &&
						inq->ansi_version == 1) {
			scgp->dev = DEV_MT02;
			strcpy(inq->vendor_info,
				"EMULEX  MT02            FAKE");
		}
		break;
	case INQ_ROMD:
	case INQ_WORM:
		if (strbeg("RXT-800S", prod_ident))
			scgp->dev = DEV_RXT800S;

		if (strbeg("ACER", vendor_info)) {
			if (strbeg("CR-4020C", prod_ident))
				scgp->dev = DEV_RICOH_RO_1420C;

		} else if (strbeg("CREATIVE", vendor_info)) {
			if (strbeg("CDR2000", prod_ident))
				scgp->dev = DEV_RICOH_RO_1060C;

		} else if (strbeg("GRUNDIG", vendor_info)) {
			if (strbeg("CDR100IPW", prod_ident))
				scgp->dev = DEV_CDD_2000;

		} else if (strbeg("JVC", vendor_info)) {
			if (strbeg("XR-W2001", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			else if (strbeg("XR-W2010", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			else if (strbeg("R2626", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("MITSBISH", vendor_info)) {
		} else if (strbeg("MITSUMI", vendor_info)) {
			scgp->dev = DEV_CDD_522;
		} else if (strbeg("OPTIMA", vendor_info)) {
			if (strbeg("CD-R 650", prod_ident))
				scgp->dev = DEV_SONY_CDU_924;

		} else if (strbeg("PHILIPS", vendor_info) ||
				strbeg("IMS", vendor_info) ||
				strbeg("KODAK", vendor_info) ||
				strbeg("HP", vendor_info)) {

			if (strbeg("CDD521/00", prod_ident))
				scgp->dev = DEV_CDD_521_OLD;
			else if (strbeg("CDD521/02", prod_ident))
				scgp->dev = DEV_CDD_521_OLD;		/* PCD 200R? */
			else if (strbeg("CDD521", prod_ident))
				scgp->dev = DEV_CDD_521;

			if (strbeg("CDD522", prod_ident))
				scgp->dev = DEV_CDD_522;
			if (strbeg("PCD225", prod_ident))
				scgp->dev = DEV_CDD_522;
			if (strbeg("KHSW/OB", prod_ident))	/* PCD600 */
				scgp->dev = DEV_PCD_600;
			if (strbeg("CDR-240", prod_ident))
				scgp->dev = DEV_CDD_2000;

			if (strbeg("CDD20", prod_ident))
				scgp->dev = DEV_CDD_2000;
			if (strbeg("CDD26", prod_ident))
				scgp->dev = DEV_CDD_2600;

			if (strbeg("C4324/C4325", prod_ident))
				scgp->dev = DEV_CDD_2000;
			if (strbeg("CD-Writer 6020", prod_ident))
				scgp->dev = DEV_CDD_2600;

		} else if (strbeg("PINNACLE", vendor_info)) {
			if (strbeg("RCD-1000", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD5020", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD5040", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			if (strbeg("RCD 4X4", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("PIONEER", vendor_info)) {
			if (strbeg("CD-WO DW-S114X", prod_ident))
				scgp->dev = DEV_PIONEER_DW_S114X;
			else if (strbeg("CD-WO DR-R504X", prod_ident))	/* Reoprt from philip@merge.com */
				scgp->dev = DEV_PIONEER_DW_S114X;
			else if (strbeg("DVD-R DVR-S101", prod_ident))
				scgp->dev = DEV_PIONEER_DVDR_S101;

		} else if (strbeg("PLASMON", vendor_info)) {
			if (strbeg("RF4100", prod_ident))
				scgp->dev = DEV_PLASMON_RF_4100;
			else if (strbeg("CDR4220", prod_ident))
				scgp->dev = DEV_CDD_2000;

		} else if (strbeg("PLEXTOR", vendor_info)) {
			if (strbeg("CD-R   PX-R24CS", prod_ident))
				scgp->dev = DEV_RICOH_RO_1420C;

		} else if (strbeg("RICOH", vendor_info)) {
			if (strbeg("RO-1420C", prod_ident))
				scgp->dev = DEV_RICOH_RO_1420C;
			if (strbeg("RO1060C", prod_ident))
				scgp->dev = DEV_RICOH_RO_1060C;

		} else if (strbeg("SAF", vendor_info)) {	/* Smart & Friendly */
			if (strbeg("CD-R2004", prod_ident) ||
			    strbeg("CD-R2006 ", prod_ident))
				scgp->dev = DEV_SONY_CDU_924;
			else if (strbeg("CD-R2006PLUS", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			else if (strbeg("CD-RW226", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;
			else if (strbeg("CD-R4012", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("SANYO", vendor_info)) {
			if (strbeg("CD-WO CRD-R24S", prod_ident))
				scgp->dev = DEV_CDD_521;

		} else if (strbeg("SONY", vendor_info)) {
			if (strbeg("CD-R   CDU92", prod_ident) ||
			    strbeg("CD-R   CDU94", prod_ident))
				scgp->dev = DEV_SONY_CDU_924;

		} else if (strbeg("TEAC", vendor_info)) {
			if (strbeg("CD-R50S", prod_ident) ||
			    strbeg("CD-R55S", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("TRAXDATA", vendor_info) ||
				strbeg("Traxdata", vendor_info)) {
			if (strbeg("CDR4120", prod_ident))
				scgp->dev = DEV_TEAC_CD_R50S;

		} else if (strbeg("T.YUDEN", vendor_info)) {
			if (strbeg("CD-WO EW-50", prod_ident))
				scgp->dev = DEV_TYUDEN_EW50;

		} else if (strbeg("WPI", vendor_info)) {	/* Wearnes */
			if (strbeg("CDR-632P", prod_ident))
				scgp->dev = DEV_CDD_2600;

		} else if (strbeg("YAMAHA", vendor_info)) {
			if (strbeg("CDR10", prod_ident))
				scgp->dev = DEV_YAMAHA_CDR_100;
			if (strbeg("CDR200", prod_ident))
				scgp->dev = DEV_YAMAHA_CDR_400;
			if (strbeg("CDR400", prod_ident))
				scgp->dev = DEV_YAMAHA_CDR_400;

		} else if (strbeg("MATSHITA", vendor_info)) {
			if (strbeg("CD-R   CW-7501", prod_ident))
				scgp->dev = DEV_MATSUSHITA_7501;
			if (strbeg("CD-R   CW-7502", prod_ident))
				scgp->dev = DEV_MATSUSHITA_7502;
		}
		if (scgp->dev == DEV_UNKNOWN) {
			if (strbeg("CDS615E", prod_ident))	/* Olympus */
				scgp->dev = DEV_SONY_CDU_924;
		}
		if (scgp->dev == DEV_UNKNOWN && inq->type == INQ_ROMD) {
			BOOL8	cdrr	 = FALSE;
			BOOL8	cdwr	 = FALSE;
			BOOL8	cdrrw	 = FALSE;
			BOOL8	cdwrw	 = FALSE;
			BOOL8	dvd	 = FALSE;
			BOOL8	dvdwr	 = FALSE;

			scgp->dev = DEV_CDROM;

			if (mmc_check(scgp, &cdrr, &cdwr, &cdrrw, &cdwrw,
								&dvd, &dvdwr))
				scgp->dev = DEV_MMC_CDROM;
			if (cdwr)
				scgp->dev = DEV_MMC_CDR;
			if (cdwrw)
				scgp->dev = DEV_MMC_CDRW;
			if (dvd)
				scgp->dev = DEV_MMC_DVD;
			if (dvdwr)
				scgp->dev = DEV_MMC_DVD_WR;
		}
		break;

	case INQ_PROCD:
		if (strbeg("BERTHOLD", vendor_info)) {
			if (strbeg("", prod_ident))
				scgp->dev = DEV_HRSCAN;
		}
		break;

	case INQ_SCAN:
		scgp->dev = DEV_MS300A;
		break;
	}

	if (!print)
		return (TRUE);

	if (scgp->dev == DEV_UNKNOWN && !got_inquiry) {
		return (FALSE);
	}

	return (TRUE);
}
//--------------------------------------------------------------------
__s32 	inquiry(St_GrScsiIf *scgp,	caddr_t	bp,	__s32	cnt)
{
		struct	scg_cmd	*scmd = &scgp->scmd;
	
		memset(bp, '\0', cnt);
		memset((caddr_t)scmd, '\0', sizeof (*scmd)); 
		scmd->addr = bp;
		scmd->size = cnt;
		scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
		scmd->cdb_len = SC_G0_CDBLEN;
		scmd->sense_len = CCS_SENSE_LEN;
		scmd->cdb.g0_cdb.cmd = SC_INQUIRY;
		scmd->cdb.g0_cdb.lun = scg_lun(scgp);
		scmd->cdb.g0_cdb.count = cnt;
	
		scgp->cmdname = "inquiry";
	
		if (scg_cmd(scgp) < 0)
			return (-1);

		return (0);
}
//--------------------------------------------------------------------
BOOL8	do_inquiry(St_GrScsiIf	*scgp, __s32 print)
{
		if (getdev(scgp, print))
		{
			return (TRUE);
		}
		else
		{
			return (FALSE);
		}
}
//--------------------------------------------------------------------
BOOL8	is_unknown_dev(St_GrScsiIf	*scgp)
{
	return (scgp->dev == DEV_UNKNOWN);
}
//--------------------------------------------------------------------
__s32 	scsi_load(St_GrScsiIf *scgp, cdr_t	*dp)
{
	__s32	key;
	__s32	code;

	if (dp && (dp->cdr_flags & CDR_CADDYLOAD) == 0) {
		if (scsi_start_stop_unit(scgp, 1, 1, dp->cdr_cmdflags&F_IMMED) >= 0)
			return (0);
	}

	if (wait_unit_ready(scgp, 60))
		return (0);

	key = scg_sense_key(scgp);
	code = scg_sense_code(scgp);

	return (-1);
}
//--------------------------------------------------------------------
__s32		scsi_unload(St_GrScsiIf *scgp, cdr_t *dp)
{
		return (scsi_start_stop_unit(scgp, 0, 1, dp && (dp->cdr_cmdflags&F_IMMED)));
}
//--------------------------------------------------------------------
__s32		read_buff_cap(St_GrScsiIf *scgp,	long *sp,	long *fp)
{
	__s8	resp[12];
	__u32	freespace;
	__u32	bufsize;
	__s32	per;
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)resp;
	scmd->size = sizeof (resp);
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5C;		/* Read buffer cap */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, sizeof (resp));

	scgp->cmdname = "read buffer cap";

	if (scg_cmd(scgp) < 0)
		return (-1);

	bufsize   = a_to_u_4_byte(&resp[4]);
	freespace = a_to_u_4_byte(&resp[8]);
	if (sp)
		*sp = bufsize;
	if (fp)
		*fp = freespace;

	if (bufsize == 0)
		return (0);
	per = (100 * (bufsize - freespace)) / bufsize;
	if (per < 0)
		return (0);
	if (per > 100)
		return (100);
	return (per);
}
//--------------------------------------------------------------------
__s32	select_secsize(St_GrScsiIf	*scgp, __s32	secsize)
{
	struct scsi_mode_data md;
	__s32	count = sizeof (struct scsi_mode_header) +
			sizeof (struct scsi_mode_blockdesc);

	(void) test_unit_ready(scgp);	/* clear any error situation */

	memset((caddr_t)&md, '\0', sizeof (md));
	md.header.blockdesc_len = 8;
	i_to_3_byte(md.blockdesc.lblen, secsize);

	return (mode_select(scgp, (unsigned __s8 *)&md, count, 0, scgp->inq.data_format >= 2));
}
//--------------------------------------------------------------------
__s32	scsi_cdr_write(St_GrScsiIf	*scgp, caddr_t bp,	long sectaddr,	long size,	__s32	blocks,	BOOL8 islast)
{
	return (write_xg1(scgp, bp, sectaddr, size, blocks));
}
//--------------------------------------------------------------------
__s32	write_xg0(St_GrScsiIf *scgp,	caddr_t	bp,	long addr, long	size, __s32	cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;

	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_WRITE;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	g0_cdbaddr(&scmd->cdb.g0_cdb, addr);
	scmd->cdb.g0_cdb.count = cnt;

	scgp->cmdname = "write_g0";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (size - scg_getresid(scgp));
}
//--------------------------------------------------------------------
__s32	write_xg1(St_GrScsiIf *scgp,	caddr_t	bp,	long addr, long	size, __s32	cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = SC_EWRITE;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "write_g1";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (size - scg_getresid(scgp));
}
//--------------------------------------------------------------------
__s32		read_session_offset(St_GrScsiIf	*scgp, long	*offp)
{
	struct	diskinfo *dp;
	__s8	xb[256];
	__s32	len;

	dp = (struct diskinfo *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	if (read_toc(scgp, (caddr_t)xb, 0, sizeof (struct tocheader), 0, FMT_SINFO) < 0)
		return (-1);

	len = a_to_u_2_byte(dp->hd.len) + sizeof (struct tocheader)-2;
	if (len > (__s32)sizeof (xb)) {
		return (-1);
	}
	if (read_toc(scgp, (caddr_t)xb, 0, len, 0, FMT_SINFO) < 0)
		return (-1);

	dp = (struct diskinfo *)xb;
	if (offp)
		*offp = a_to_u_4_byte(dp->desc[0].addr);
	return (0);
}
//--------------------------------------------------------------------
__s32		scsi_start_stop_unit(St_GrScsiIf	*scgp, __s32 flg,	__s32	loej,	BOOL8 immed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x1B;	/* Start Stop Unit */
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.count = (flg ? 1:0) | (loej ? 2:0);

	if (immed)
		scmd->cdb.cmd_cdb[1] |= 0x01;

	scgp->cmdname = "start/stop unit";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		get_trackinfo(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	type,	__s32	addr,	__s32	cnt)
{
	__s32	len;
	__s32	ret;

	memset(bp, '\0', cnt);

	if (read_track_info(scgp, bp, type, addr, 4) < 0)
		return (-1);

	len = a_to_u_2_byte(bp);
	len += 2;
	if (len > cnt)
		len = cnt;
	ret = read_track_info(scgp, bp, type, addr, len);

	return (ret);
}
//--------------------------------------------------------------------
__s32		read_disk_info(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
		
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes */
	scmd->cdb.g1_cdb.cmd = 0x51;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read disk info";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		mode_select(St_GrScsiIf *scgp,	unsigned __s8	*dp, __s32 cnt,	__s32	smp, __s32 pf)
{
	if (is_atapi)
		return (mode_select_sg0(scgp, dp, cnt, smp, pf));
	return (mode_select_g0(scgp, dp, cnt, smp, pf));
}
//--------------------------------------------------------------------
__s32		scsi_close_tr_session(St_GrScsiIf *scgp, __s32 type, __s32 track, BOOL8 immed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 8 * 60;		/* Needs up to 4 minutes */
	scmd->cdb.g1_cdb.cmd = 0x5B;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.addr[0] = type;
	scmd->cdb.g1_cdb.addr[3] = track;

	if (immed)
		scmd->cdb.g1_cdb.reladr = 1;

	scgp->cmdname = "close track/session";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		scsi_flush_cache(St_GrScsiIf *scgp, BOOL8 immed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 2 * 60;		/* Max: sizeof (CDR-cache)/150KB/s */
	scmd->cdb.g1_cdb.cmd = 0x35;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	if (immed)
		scmd->cdb.cmd_cdb[1] |= 0x02;

	scgp->cmdname = "flush cache";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32 	send_opc(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	doopc)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 60;
	scmd->cdb.g1_cdb.cmd = 0x54;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g1_cdb.reladr = doopc?1:0;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "send opc";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		scsi_set_speed(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed, __s32 rotctl)
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

	scmd->cdb.cmd_cdb[1] |= rotctl & 0x03;

	scgp->cmdname = "set cd speed";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		scsi_blank(St_GrScsiIf *scgp, long addr, __s32 blanktype, BOOL8 immed)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 160 * 60; /* full blank at 1x could take 80 minutes */
	scmd->cdb.g5_cdb.cmd = 0xA1;	/* Blank */
	scmd->cdb.g0_cdb.high_addr = blanktype;
	g1_cdbaddr(&scmd->cdb.g5_cdb, addr);

	if (immed)
		scmd->cdb.g5_cdb.res |= 8;

	scgp->cmdname = "blank unit";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
BOOL8	mmc_check(St_GrScsiIf *scgp, BOOL8 *cdrrp, BOOL8 *cdwrp, BOOL8 *cdrrwp, BOOL8 *cdwrwp,
				BOOL8 *dvdp, BOOL8 *dvdwp)
{
	unsigned __s8	mode[0x100];
	BOOL8	was_atapi;
	struct	cd_mode_page_2A *mp;

	if (scgp->inq.type != INQ_ROMD)
		return (FALSE);

	memset((caddr_t)mode, '\0', sizeof (mode));

	was_atapi = allow_atapi(scgp, TRUE);
	mp = mmc_cap(scgp, mode);
	allow_atapi(scgp, was_atapi);
	if (mp == NULL)
		return (FALSE);

	mmc_getval(mp, cdrrp, cdwrp, cdrrwp, cdwrwp, dvdp, dvdwp);

	return (TRUE);			/* Generic St_GrScsiIf-3/mmc CD	*/
}
//--------------------------------------------------------------------
void	mmc_getval(struct cd_mode_page_2A *mp, BOOL8 *cdrrp, BOOL8 *cdwrp, BOOL8 *cdrrwp, BOOL8 *cdwrwp,
				BOOL8 *dvdp, BOOL8 *dvdwp)
{
	BOOL8	isdvd;				/* Any DVD reader	*/
	BOOL8	isdvd_wr;			/* DVD writer (R / RAM)	*/
	BOOL8	iscd_wr;			/* CD  writer		*/

	iscd_wr = (mp->cd_r_write != 0) ||	/* SCSI-3/mmc CD-R	*/
		    (mp->cd_rw_write != 0);	/* SCSI-3/mmc CD-RW	*/

	if (cdrrp)
		*cdrrp = (mp->cd_r_read != 0);	/* SCSI-3/mmc CD	*/
	if (cdwrp)
		*cdwrp = (mp->cd_r_write != 0);	/* SCSI-3/mmc CD-R	*/

	if (cdrrwp)
		*cdrrwp = (mp->cd_rw_read != 0); /* SCSI-3/mmc CD	*/
	if (cdwrwp)
		*cdwrwp = (mp->cd_rw_write != 0); /* SCSI-3/mmc CD-RW	*/

	isdvd =					/* SCSI-3/mmc2 DVD 	*/
		(mp->dvd_ram_read + mp->dvd_r_read  +
		    mp->dvd_rom_read) != 0;

	isdvd_wr =				/* SCSI-3/mmc2 DVD writer*/
		(mp->dvd_ram_write + mp->dvd_r_write) != 0;

	if (dvdp)
		*dvdp = isdvd;
	if (dvdwp)
		*dvdwp = isdvd_wr;
}
//--------------------------------------------------------------------
BOOL8	allow_atapi(St_GrScsiIf *scgp, BOOL8 new1)
{
	BOOL8	old = is_atapi;
	unsigned __s8	mode[256];

	if (new1 == old)
		return (old);


	(void) unit_ready(scgp);
	if (new1 && mode_sense_g1(scgp, mode, 8, 0x3F, 0) < 0)
	{
		new1 = FALSE;
	}

	is_atapi = new1;
	return (old);
}
//--------------------------------------------------------------------
struct cd_mode_page_2A * mmc_cap(St_GrScsiIf	*scgp, unsigned __s8	*modep)
{
	__s32	len;
	__s32	val;
	unsigned __s8	mode[0x100];
	struct	cd_mode_page_2A *mp;
	struct	cd_mode_page_2A *mp2;

#ifdef LINUX_APP
#endif

retry:
	memset((caddr_t)mode, '\0', sizeof (mode));

	if (!get_mode_params(scgp, 0x2A, "CD capabilities",
			mode, (unsigned __s8 *)0, (unsigned __s8 *)0, (unsigned __s8 *)0, &len))
	{

		if (scg_sense_key(scgp) == SC_NOT_READY)
		{
			if (wait_unit_ready(scgp, 60))
				goto retry;
		}
		printf("mmc_cap - 1\n");
		return (NULL);		/* Pre SCSI-3/mmc drive		*/
	}

	if (len == 0)			/* Pre SCSI-3/mmc drive		*/
	{
		printf("mmc_cap - 2\n");
		return (NULL);
	}

	mp = (struct cd_mode_page_2A *)
		( mode + sizeof (struct scsi_mode_header) +	
		((struct scsi_mode_header *)mode)->blockdesc_len );

	if (mp->p_len < 0x10)
	{
		printf("mmc_cap - 3\n");
		return (NULL);
	}

	val = a_to_u_2_byte(mp->max_read_speed);
	if (val != 0 && val < 176)
	{
		printf("mmc_cap - 4\n");	
		return (NULL);
	}
	val = a_to_u_2_byte(mp->cur_read_speed);
	if (val != 0 && val < 176)
	{
		return (NULL);
	}

	len -= sizeof (struct scsi_mode_header) +
		((struct scsi_mode_header *)mode)->blockdesc_len;
	if (modep)
		mp2 = (struct cd_mode_page_2A *)modep;
	else
		mp2 = (struct cd_mode_page_2A *)malloc(len);
	if (mp2)
	{
		//movebytes(mp, mp2, len);
		GrDumyCopyMem( mp2, mp, len);
	}

	return (mp2);
}
//--------------------------------------------------------------------
BOOL8	scsi_in_progress(St_GrScsiIf *scgp)
{
	// local -------------------
		BOOL8	Tv_Result;
		__s32		Tv_SenseKey;
		__s32		Tv_SenseCode;
		__s32		Tv_SenseQual;
	// code --------------------
		Tv_Result	=	FALSE;

		//get info
		Tv_SenseKey		=	scg_sense_key(scgp);
		Tv_SenseCode	=	scg_sense_code(scgp);
		Tv_SenseQual	=	scg_sense_qual(scgp);

		if ( (SC_NOT_READY == Tv_SenseKey) && (0x04 == Tv_SenseCode) )
		{
			if ( (0x04 == Tv_SenseQual) || (0x07 == Tv_SenseQual) || (0x08 == Tv_SenseQual) )
			{
				/* long write in progress 
					qual : 0x04 = fromat in progress , 0x07 = operation in progress
				*/
				Tv_Result	=	TRUE;
			}
		}
		
		if ( !Tv_Result )
		{
			printf( "scsi_in_progress :: not in progress - sensekey=%X , sensecode=%X , sensequal=%X \n",
				Tv_SenseKey, Tv_SenseCode, Tv_SenseQual );
		}
		
		return Tv_Result;
}
//--------------------------------------------------------------------
__s32		send_cue_sheet(St_GrScsiIf	*scgp, caddr_t bp,long size)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = size;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x5D;	/* Send CUE sheet */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, size);

	scgp->cmdname = "send_cue_sheet";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (size - scmd->resid);
}
//--------------------------------------------------------------------
__s32		scsi_get_speed(St_GrScsiIf *scgp, __s32 *readspeedp, __s32 *writespeedp)
{
	struct	cd_mode_page_2A *mp;
	unsigned __s8	m[256];
	__s32	val;

	mp = mmc_cap(scgp, m); /* Get MMC capabilities in allocated mp */
	if (mp == NULL)
		return (-1);	/* Pre St_GrScsiIf-3/mmc drive		*/

	val = a_to_u_2_byte(mp->cur_read_speed);
	if (readspeedp)
		*readspeedp = val;

	if (mp->p_len >= 28)
		val = a_to_u_2_byte(mp->v3_cur_write_speed);
	else
		val = a_to_u_2_byte(mp->cur_write_speed);
	if (writespeedp)
		*writespeedp = val;

	return (0);
}
//--------------------------------------------------------------------
__s32	read_session_offset_philips(St_GrScsiIf	*scgp,	long	*offp)
{
	struct	sinfo *sp;
	__s8	xb[256];
	__s32	len;

	sp = (struct sinfo *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	if (read_toc_philips(scgp, (caddr_t)xb, 0, sizeof (struct siheader), 0, FMT_SINFO) < 0)
		return (-1);
	len = a_to_u_2_byte(sp->hd.len) + sizeof (struct siheader)-2;
	if (len > (__s32)sizeof (xb)) {
		return (-1);
	}
	if (read_toc_philips(scgp, (caddr_t)xb, 0, len, 0, FMT_SINFO) < 0)
		return (-1);

	sp = (struct sinfo *)xb;
	if (offp)
		*offp = a_to_u_4_byte(sp->desc[sp->hd.finished-1].addr);
	return (0);
}
//--------------------------------------------------------------------
BOOL8	is_mmc(St_GrScsiIf *scgp, BOOL8 *cdwp, BOOL8 *dvdwp)
{
	BOOL8	cdwr	= FALSE;
	BOOL8	cdwrw	= FALSE;

	if (cdwp)
		*cdwp = FALSE;
	if (dvdwp)
		*dvdwp = FALSE;

	if (!mmc_check(scgp, NULL, &cdwr, NULL, &cdwrw, NULL, dvdwp))
		return (FALSE);

	if (cdwp)
		*cdwp = cdwr | cdwrw;

	return (TRUE);
}
//--------------------------------------------------------------------
__s32		read_toc(St_GrScsiIf	*scgp, caddr_t bp, __s32 track,	__s32	cnt,	__s32	msf,	__s32	fmt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x43;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (msf)
		scmd->cdb.g1_cdb.res = 1;
	scmd->cdb.g1_cdb.addr[0] = fmt & 0x0F;
	scmd->cdb.g1_cdb.res6 = track;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read toc";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		read_track_info(St_GrScsiIf	*scgp, caddr_t bp, __s32 type, __s32 addr, __s32 cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes */
	scmd->cdb.g1_cdb.cmd = 0x52;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	scmd->cdb.cmd_cdb[1] = type;
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);	/* LBA/Track/Session */
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read track info";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		mode_select_sg0(St_GrScsiIf *scgp, unsigned __s8	*dp, __s32	cnt,	__s32	smp, __s32	pf)
{
	unsigned __s8	xmode[256+4];
	__s32	amt = cnt;

	if (amt < 1 || amt > 255) {

		return (-1);
	}

	if (amt < 4) {		/* Data length. medium type & VU */
		amt += 1;
	} else {
		amt += 4;
		//movebytes(&dp[4], &xmode[8], cnt-4);
		GrDumyCopyMem( &xmode[8], &dp[4], cnt-4);
	}
	xmode[0] = 0;
	xmode[1] = 0;
	xmode[2] = dp[1];
	xmode[3] = dp[2];
	xmode[4] = 0;
	xmode[5] = 0;
	i_to_2_byte(&xmode[6], (unsigned __s32)dp[3]);

	return (mode_select_g1(scgp, xmode, amt, smp, pf));
}
//--------------------------------------------------------------------
__s32		read_toc_philips(St_GrScsiIf	*scgp,	caddr_t	bp,	__s32	track,	__s32	cnt,	__s32	msf,	__s32	fmt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* May last  174s on a TEAC CD-R55S */
	scmd->cdb.g1_cdb.cmd = 0x43;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (msf)
		scmd->cdb.g1_cdb.res = 1;
	scmd->cdb.g1_cdb.res6 = track;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	if (fmt & 1)
		scmd->cdb.g1_cdb.vu_96 = 1;
	if (fmt & 2)
		scmd->cdb.g1_cdb.vu_97 = 1;

	scgp->cmdname = "read toc";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		mode_select_g1(St_GrScsiIf	*scgp, unsigned __s8 *dp, __s32	cnt, __s32 smp,	__s32	pf)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)dp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x55;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.high_addr = smp ? 1 : 0 | pf ? 0x10 : 0;
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "mode select g1";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		scsi_prevent_removal(St_GrScsiIf	*scgp, __s32 prevent)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x1E;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.count = prevent & 1;

	scgp->cmdname = "prevent/allow medium removal";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		read_dvd_structure(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 mt, __s32 addr, __s32 layer, __s32 fmt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes ??? */
	scmd->cdb.g5_cdb.cmd = 0xAD;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] |= (mt & 0x0F);	/* Media Type */
	g5_cdbaddr(&scmd->cdb.g5_cdb, addr);
	g5_cdblen(&scmd->cdb.g5_cdb, cnt);
	scmd->cdb.g5_cdb.count[0] = layer;
	scmd->cdb.g5_cdb.count[1] = fmt;

	scgp->cmdname = "read dvd structure";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32		rezero_unit(St_GrScsiIf *scgp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_REZERO_UNIT;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);

	scgp->cmdname = "rezero unit";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		read_rzone_info(St_GrScsiIf	*scgp,	caddr_t	bp,	__s32	cnt)
{
	return (get_trackinfo(scgp, bp, TI_TYPE_LBA, 0, cnt));
}
//--------------------------------------------------------------------
__s32		send_dvd_structure(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 fmt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->timeout = 4 * 60;		/* Needs up to 2 minutes ??? */
	scmd->cdb.g5_cdb.cmd = 0xBF;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	g5_cdblen(&scmd->cdb.g5_cdb, cnt);
	scmd->cdb.g5_cdb.count[0] = 0;
	scmd->cdb.g5_cdb.count[1] = fmt;

	scgp->cmdname = "send dvd structure";

	if (scg_cmd(scgp) < 0)
		return (-1);
	return (0);
}
//--------------------------------------------------------------------
__s32	reserve_tr_rzone(St_GrScsiIf	*scgp, long	size)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x53;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);

	i_to_4_byte(&scmd->cdb.cmd_cdb[5], size);

	scgp->cmdname = "reserve_track_rzone";

	if (scg_cmd(scgp) < 0)
	{
		return (-1);
	}
	return (0);
}
//--------------------------------------------------------------------
BOOL8	wait_unit_ready(St_GrScsiIf *scgp, __s32 secs)
{
	__s32	i;
	__s32	c;
	__s32	k;
	__s32	ret;
	__s32	err;

	errno = 0;
	ret = test_unit_ready(scgp);
	if (ret < 0) {
		err = errno;

		if (err == EPERM || err == EACCES) {
			return (FALSE);
		}
		ret = test_unit_ready(scgp);
	}

	if (ret >= 0)
		return (TRUE);

	for (i = 0; i < secs && (ret = test_unit_ready(scgp)) < 0; i++) {
		if (scgp->scmd.scb.busy != 0) {
			sleep(1);
			continue;
		}
		c = scg_sense_code(scgp);
		k = scg_sense_key(scgp);

		if ((k == SC_NOT_READY && (c == 0x3A || c == 0x30)) ||
		    (k == SC_MEDIUM_ERROR)) {
				scg_printerr(scgp);
			return (FALSE);
		}
		sleep(1);
	}
	if (ret < 0)
		return (FALSE);
	return (TRUE);
}
//--------------------------------------------------------------------
__s32	read_scsi(St_GrScsiIf	*scgp,	caddr_t	bp,	long	addr,	__s32	cnt)
{
	if (addr <= G0_MAXADDR && cnt < 256 && !is_atapi)
		return (read_g0(scgp, bp, addr, cnt));
	else
		return (read_g1(scgp, bp, addr, cnt));
}
//--------------------------------------------------------------------
__s32	read_g0(St_GrScsiIf	*scgp, caddr_t	bp,	long	addr,	__s32	cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	if (scgp->cap.c_bsize <= 0)
	{
		raisecond("capacity_not_set", 0L);
	}

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt*scgp->cap.c_bsize;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = SC_READ;
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	g0_cdbaddr(&scmd->cdb.g0_cdb, addr);
	scmd->cdb.g0_cdb.count = cnt;

	scgp->cmdname = "read_g0";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32	read_g1(St_GrScsiIf *scgp, caddr_t	bp,	long	addr,	__s32	cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	if (scgp->cap.c_bsize <= 0)
	{
		raisecond("capacity_not_set", 0L);
	}

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt*scgp->cap.c_bsize;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = SC_EREAD;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdbaddr(&scmd->cdb.g1_cdb, addr);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read_g1";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
BOOL8	is_cddrive(St_GrScsiIf	*scgp)
{
	if(scgp->inq.type == INQ_ROMD)
	{
		printf("scgp->inq->type == INQ_ROMD\n");
	}
	if(scgp->inq.type == INQ_WORM)
	{
		printf("scgp->inq->type == INQ_WORM\n");
	}
	return (scgp->inq.type == INQ_ROMD || scgp->inq.type == INQ_WORM);
}
//--------------------------------------------------------------------
__s32		qic02(St_GrScsiIf *scgp, __s32	cmd)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)0;
	scmd->size = 0;
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G0_CDBLEN;
	scmd->sense_len = DEF_SENSE_LEN;
	scmd->cdb.g0_cdb.cmd = 0x0D;	/* qic02 Sysgen SC4000 */
	scmd->cdb.g0_cdb.lun = scg_lun(scgp);
	scmd->cdb.g0_cdb.mid_addr = cmd;

	scgp->cmdname = "qic 02";
	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		sense_secsize(St_GrScsiIf *scgp,	__s32	current)
{
	unsigned __s8	mode[0x100];
	unsigned __s8	*p;
	unsigned __s8	*ep;
	__s32	len;
	__s32	secsize = -1;

	(void) unit_ready(scgp);


	memset(mode, '\0', sizeof (mode));

	len =	sizeof (struct scsi_mode_header) +
		sizeof (struct scsi_mode_blockdesc);

	if (mode_sense(scgp, mode, len, 0x3F, current?0:2) < 0) {
		memset(mode, '\0', sizeof (mode));
		if (mode_sense(scgp, mode, len, 0, current?0:2) < 0)	{ /* VU (block desc) */
			return (-1);
		}
	}
	if (mode[3] == 8)
	{
		secsize = a_to_u_3_byte(&mode[9]);
	}
	memset(mode, '\0', sizeof (mode));


	return (secsize);
}
//--------------------------------------------------------------------
__s32		read_capacity(St_GrScsiIf *scgp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t)&scgp->cap;
	scmd->size = sizeof (struct scsi_capacity);
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x25;	/* Read Capacity */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, 0); /* Full Media */

	scgp->cmdname = "read capacity";

	if (scg_cmd(scgp) < 0)
	{
		return (-1);
	}
	else
	{
		long	cbsize;
		long	cbaddr;

		cbsize = a_to_4_byte(&scgp->cap.c_bsize);
		cbaddr = a_to_4_byte(&scgp->cap.c_baddr);
		scgp->cap.c_bsize = cbsize;
		scgp->cap.c_baddr = cbaddr;
	}
	return (0);
}
//--------------------------------------------------------------------
__s32		read_tochdr(St_GrScsiIf *scgp,	cdr_t	*dp, __s32 *fp,	__s32	*lp)
{
	struct	tocheader *tp;
	__s8	xb[256];
	__s32	len;

	tp = (struct tocheader *)xb;

	memset((caddr_t)xb, '\0', sizeof (xb));
	if (read_toc(scgp, xb, 0, sizeof (struct tocheader), 0, FMT_TOC) < 0)
	{
			printf("Cannot read TOC header\n");
		return (-1);
	}
	len = a_to_u_2_byte(tp->len) + sizeof (struct tocheader)-2;
	if (len >= 4) {
		if (fp)
			*fp = tp->first;
		if (lp)
			*lp = tp->last;
		return (0);
	}
	return (-1);
}
//--------------------------------------------------------------------
BOOL8	cdr_underrun(St_GrScsiIf	*scgp)
{
	if ((scg_sense_key(scgp) != SC_ILLEGAL_REQUEST && scg_sense_key(scgp) != SC_MEDIUM_ERROR))
		return (FALSE);

	if ((scg_sense_code(scgp) == 0x21 && (scg_sense_qual(scgp) == 0x00 ||
	    scg_sense_qual(scgp) == 0x02)) ||	(scg_sense_code(scgp) == 0x0C &&
	    scg_sense_qual(scgp) == 0x09))
	{
		return (TRUE);
	}

	return (FALSE);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

