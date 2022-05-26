/*
	time Tools
*/
//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<stdio.h>
#ifdef	LINUX_APP
#include	<sys/time.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<time.h>
#include	<linux/rtc.h>
#include	<sys/times.h>
#include	<scsi/sg.h>
#include 	<scsi/scsi.h>
#include 	<string.h>
#endif

#include	<GrTypeBase.h>

#include	<GrDumyTool.h>

#include	<Hw/Odd/GrScgo.h>
#include	<stdlib.h>
#include 	<errno.h>
#include 	<Hw/Odd/GrScsiOpr.h>

//====================================================================
//local const
#define	DID_OK		0x00 /* NO error				*/
#define	DID_NO_CONNECT	0x01 /* Couldn't connect before timeout period	*/
#define	DID_BUS_BUSY	0x02 /* BUS stayed busy through time out period	*/
#define	DID_TIME_OUT	0x03 /* TIMED OUT for other reason		*/
#define	DID_BAD_TARGET	0x04 /* BAD target.				*/
#define	DID_ABORT	0x05 /* Told to abort for some other reason	*/
#define	DID_PARITY	0x06 /* Parity error				*/
#define	DID_ERROR	0x07 /* Internal error				*/
#define	DID_RESET	0x08 /* Reset by somebody.			*/
#define	DID_BAD_INTR	0x09 /* Got an interrupt we weren't expecting.	*/

#define	DRIVER_BUSY	0x01
#define	DRIVER_SOFT	0x02
#define	DRIVER_MEDIA	0x03
#define	DRIVER_ERROR	0x04

#define	DRIVER_INVALID	0x05
#define	DRIVER_TIMEOUT	0x06
#define	DRIVER_HARD	0x07
#define	DRIVER_SENSE	0x08

#define	MAX_DMA_LINUX	SG_BIG_BUFF	/* Defined in include/scsi/sg.h	*/

#define	GETINT(a)	(a)

//pg
#define PG_MAX_DATA	32768

//====================================================================
//local type

//====================================================================
//local function
void	sg_checkdma(__s32	f, __s32 *valp);

//====================================================================
//var
__s8	_scg_trans_version[] = "scsi-linux-sg.c-1.94";
__s8	_scg_auth_schily[]	= "schily";
static	__s8 __sccsid[] =	"@(#)scsi-linux-sg.c	1.94 09/09/07 Copyright 1997-2009 J. Schilling";
__s8	_scg_trans_dversion[] = "scsihack.c-1.55";	/* The version for this transport*/
static	__s8 _sccsid[] =	"@(#)scsihack.c	1.55 09/07/13 Copyright 1997,2000-2009 J. Schilling";

scg_ops_t scg_std_ops = {
	scgo_send,
	scgo_version,
	scgo_help,
	scgo_open,
	scgo_close,
	scgo_maxdma,
	scgo_getbuf,
	scgo_freebuf,
	scgo_numbus,
	scgo_havebus,
	scgo_fileno,
	scgo_initiator_id,
	//scgo_isatapi,
	scgo_reset,
};

scg_ops_t scg_remote_ops = {
	scgo_dsend,
	scgo_dversion,
	scgo_nohelp,
	scgo_ropen,
	scgo_dclose,
	scgo_dmaxdma,
	scgo_dgetbuf,
	scgo_dfreebuf,
	scgo_dnumbus,
	scgo_dhavebus,
	scgo_dfileno,
	scgo_dinitiator_id,
	//scgo_disatapi,
	scgo_dreset,
};

scg_ops_t scg_dummy_ops = {
	scgo_dsend,
	scgo_dversion,
	scgo_dhelp,
	scgo_dopen,
	scgo_dclose,
	scgo_dmaxdma,
	scgo_dgetbuf,
	scgo_dfreebuf,
	scgo_dnumbus,
	scgo_dhavebus,
	scgo_dfileno,
	scgo_dinitiator_id,
	//scgo_disatapi,
	scgo_dreset,
};

//====================================================================
//--------------------------------------------------------------------
__s32 scgo_open(St_GrScsiIf *scgp,	__s8 *device)
{
	// var --------------------
		__s32	busno	= scg_scsibus(scgp);
		__s32	tgt	= scg_target(scgp);
		__s32	tlun	= scg_lun(scgp);
		__s32	f;
		__s32	i;
		__s32	b;
		__s32	t;
		__s32	l;
		__s32	nopen = 0;
		__s32	xopen = 0;
		__s8		devname[64];
		BOOL8	use_ata = FALSE;
	// code -------------------
		busno		= scg_scsibus(scgp);
		tgt			= scg_target(scgp);
		tlun		= scg_lun(scgp);

		if ((busno >= MAX_SCG && busno < 1000) || busno >= (1000+MAX_ATA) || tgt >= MAX_TGT || tlun >= MAX_LUN)
		{
				errno = EINVAL;
				return (-1);
		}
		if (busno >= 1000)
		{
			busno = busno - 1000 + MAX_SCG;
		}

		scglocal(scgp).scgfile = -1;
		scglocal(scgp).pgbus = -2;
		scglocal(scgp).SCSIbuf = (__s8 *)-1;
		scglocal(scgp).pack_id = 5;
		scglocal(scgp).drvers = -1;
		scglocal(scgp).isold = -1;
		scglocal(scgp).xbufsize = 0L;
		scglocal(scgp).xbuf = NULL;

		for (b = 0; b < MAX_SCG+MAX_ATA; b++)
		{
			scglocal(scgp).buscookies[b] = (__s16)-1;
			for (t = 0; t < MAX_TGT; t++)
			{
				for (l = 0; l < MAX_LUN; l++)
					scglocal(scgp).scgfiles[b][t][l] = (__s16)-1;
			}
		}

		if (use_ata)
		{
			goto scanopen;
		}
	
		if ((device != NULL && *device != '\0') || (busno == -2 && tgt == -2))
		{
			goto openbydev;
		}

scanopen:
		for (i = 0; i <= 25; i++)
		{
			sprintf(devname, "%s%c", "/dev/hd", i+'a');
			f = open(devname, O_RDWR | O_NONBLOCK);
			if (f < 0)
			{
				if (errno == EROFS)
					continue;

				if (errno != ENOENT && errno != ENXIO && errno != ENODEV)
				{
					return (0);
				}
			}
			else
			{
				__s32	iparm;
	
				if (ioctl(f, SG_GET_TIMEOUT, &iparm) < 0)
				{
#ifdef LINUX_APP
					close(f);
#endif
					continue;
				}
				sg_clearnblock(f);
				if (sg_setup(scgp, f, busno, tgt, tlun, i))
					return (++nopen);
				if (busno < 0 && tgt < 0 && tlun < 0)
					nopen++;
			}
		}
		if (nopen > 0 && scgp->errstr)
			scgp->errstr[0] = '\0';
	
		xopen = nopen;
		if (!use_ata)
		{
			for (i = 0; i < 32; i++)
			{
				sprintf(devname, "%s%d", "/dev/sg", i);
				f = open(devname, O_RDWR | O_NONBLOCK);
				if (f < 0)
				{
		
					if (errno != ENOENT && errno != ENXIO && errno != ENODEV)
					{
						return (0);
					}
				}
				else
				{
					sg_clearnblock(f);
					if (sg_setup(scgp, f, busno, tgt, tlun, -1))
						return (++nopen);
					if (busno < 0 && tgt < 0 && tlun < 0)
						nopen++;
				}
			}
		}
		if (nopen > 0 && scgp->errstr)
			scgp->errstr[0] = '\0';
	
		if (!use_ata && nopen == xopen) for (i = 0; i <= 25; i++)
		{
			sprintf(devname, "%s%c", "/dev/sg", i+'a');
			f = open(devname, O_RDWR | O_NONBLOCK);
			if (f < 0)
			{
				if (errno != ENOENT && errno != ENXIO && errno != ENODEV)
				{
					return (0);
				}
			}
			else
			{
				sg_clearnblock(f);
				if (sg_setup(scgp, f, busno, tgt, tlun, -1))
					return (++nopen);
				if (busno < 0 && tgt < 0 && tlun < 0)
					nopen++;
			}
		}
		if (nopen > 0 && scgp->errstr)
			scgp->errstr[0] = '\0';
openbydev:
		if (device != NULL && *device != '\0') {
		b = -1;
		if (strlen(device) == 8 && strncmp(device, "/dev/hd", 7) == 0) {
			b = device[7] - 'a';
			if (b < 0 || b > 25)
				b = -1;
		}

#ifdef LINUX_APP
		f = open(device, O_RDWR | O_NONBLOCK);
#else
		f	=	0x1234;
#endif

		if (f < 0) {
			return (0);
		}

		sg_clearnblock(f);
		if (!sg_mapdev(scgp, f, &busno, &tgt, &tlun, 0, 0, b)) 
		{
#ifdef LINUX_APP
			close(f);
#endif
			goto openpg;
		}

		scg_settarget(scgp, busno, tgt, tlun);
		if (sg_setup(scgp, f, busno, tgt, tlun, b))
			return (++nopen);
	}
openpg:
	/*nopen += pg_open(scgp, device);

	if (scgp->debug > 0) for (b = 0; b < MAX_SCG+MAX_ATA; b++) {
		for (t = 0; t < MAX_TGT; t++) {
			for (l = 0; l < MAX_LUN; l++) {
				if (scglocal(scgp).scgfiles[b][t][l] != (__s16)-1) {
					printf("error file %d, %d, %d\n", b, t, l);
				}
			}
		}
	}*/
	return (nopen);
}
//--------------------------------------------------------------------
void sg_clearnblock(__s32 f)
{
		__s32	n;
	
		n = fcntl(f, F_GETFL);
		n &= ~O_NONBLOCK;
		fcntl(f, F_SETFL, n);
}
//--------------------------------------------------------------------
BOOL8 	sg_setup(St_GrScsiIf	*scgp, __s32 f,	__s32	busno, __s32 tgt, __s32	tlun,	__s32	ataidx)
{
	// var --------------------
		__s32	n;
		__s32	Chan;
		__s32	Ino;
		__s32	Bus;
		__s32	Target;
		__s32	Lun;
		BOOL8	onetarget = FALSE;
	// code -------------------
		if (scglocal(scgp).drvers < 0)
		{
			scglocal(scgp).drvers = 0;
			if (ioctl(f, SG_GET_VERSION_NUM, &n) >= 0)
			{
				scglocal(scgp).drvers = n;
			}
		}

		if (scg_scsibus(scgp) >= 0 && scg_target(scgp) >= 0 && scg_lun(scgp) >= 0)
			onetarget = TRUE;
	
		sg_mapdev(scgp, f, &Bus, &Target, &Lun, &Chan, &Ino, ataidx);
	
		Ino |= Chan << 8;
		n = sg_mapbus(scgp, Bus, Ino);
		if (Bus == -1)
		{
			Bus = n;
		}
	
		if (Bus < 0 || Bus >= (MAX_SCG+MAX_ATA) || Target < 0 ||
				Target >= MAX_TGT || Lun < 0 || Lun >= MAX_LUN)
		{
			return (FALSE);
		}
	
		if (scglocal(scgp).scgfiles[Bus][Target][Lun] == (__s16)-1)
			scglocal(scgp).scgfiles[Bus][Target][Lun] = (__s16)f;
	
		if (onetarget)
		{
			if (Bus == busno && Target == tgt && Lun == tlun)
			{
				sg_initdev(scgp, f);
				scglocal(scgp).scgfile = f;
				return (TRUE);
			}
			else
			{
				scglocal(scgp).scgfiles[Bus][Target][Lun] = (__s16)-1;
#ifdef LINUX_APP
				close(f);
#endif
			}
		}
		else
		{
			sg_initdev(scgp, f);
		}
		return (FALSE);
}
//--------------------------------------------------------------------
BOOL8 sg_mapdev(St_GrScsiIf *scgp, __s32	f, __s32 *busp,	__s32	*tgtp, __s32 *lunp,	__s32	*chanp,	__s32	*inop, __s32 ataidx)
{
	// var --------------------
		struct	sg_id
		{
			long	l1; //target | lun << 8 | channel << 16 | low_ino << 24
			long	l2; //Unique id
		} sg_id;
		__s32	Chan;
		__s32	Ino;
		__s32	Bus;
		__s32	Target;
		__s32	Lun;	
	// code -------------------
		if (ataidx >= 0)
		{
			*busp = (ataidx / 2) + MAX_SCG;
			*tgtp = ataidx % 2;
			*lunp = 0;
			if (chanp)
				*chanp = 0;
			if (inop)
				*inop = 0;
			return (TRUE);
		}

		if (ioctl(f, SCSI_IOCTL_GET_IDLUN, &sg_id))
			return (FALSE);

		if (ioctl(f, SCSI_IOCTL_GET_BUS_NUMBER, &Bus) < 0)
		{
			Bus = -1;
		}
	
		Target	= sg_id.l1 & 0xFF;
		Lun	= (sg_id.l1 >> 8) & 0xFF;
		Chan	= (sg_id.l1 >> 16) & 0xFF;
		Ino	= (sg_id.l1 >> 24) & 0xFF;

		*busp = Bus;
		*tgtp = Target;
		*lunp = Lun;
		if (chanp)
			*chanp = Chan;
		if (inop)
			*inop = Ino;

		return (TRUE);
}
//--------------------------------------------------------------------
__s32 sg_mapbus(St_GrScsiIf *scgp,	__s32	busno, __s32 ino)
{
	// var --------------------
		__s32	i;
	// code -------------------
		if (busno >= 0 && busno < (MAX_SCG+MAX_ATA))
		{
			if (scglocal(scgp).buscookies[busno] == (__s16)-1)
			{
				scglocal(scgp).buscookies[busno] = ino;
				return (busno);
			}
			return (busno);
		}
		else
		{
			for (i = 0; i < (MAX_SCG+MAX_ATA); i++)
			{
				if (scglocal(scgp).buscookies[i] == (__s16)-1)
				{
					scglocal(scgp).buscookies[i] = ino;
					return (i);
				}

				if (scglocal(scgp).buscookies[i] == ino)
					return (i);
			}
		}

		return (0);
}
//--------------------------------------------------------------------
void	sg_initdev(St_GrScsiIf *scgp, __s32 f)
{
	// var --------------------
		struct sg_rep
		{
			struct sg_header	hd;
			unsigned __s8		rbuf[100];
		} sg_rep;
		__s32	n;
		__s32	i;
#ifdef LINUX_APP
		struct stat sb;
#endif
		__s32	ret;
	// code -------------------
		sg_settimeout(f, scgp->deftimeout);
	
#ifdef LINUX_APP
		if (fstat(f, &sb) >= 0 && S_ISBLK(sb.st_mode))
			return;
#endif
	
		n = fcntl(f, F_GETFL);
		fcntl(f, F_SETFL, n|O_NONBLOCK);

		//fillbytes((caddr_t)&sg_rep, sizeof (struct sg_header), '\0');
		memset((caddr_t)&sg_rep, '\0', sizeof (struct sg_header));
		sg_rep.hd.reply_len = sizeof (struct sg_header);

		for (i = 0; i < 1000; i++)
		{
			ret = read(f, &sg_rep, sizeof (sg_rep));
			if (ret > 0)
				continue;
			if (ret == 0 || errno == EAGAIN || errno == EIO)
				break;
			if (ret < 0 && i > 10)	/* Stop on repeated unknown error */
				break;
		}
		fcntl(f, F_SETFL, n);
}
//--------------------------------------------------------------------
void sg_settimeout(__s32 f,	__s32	tmo)
{
	tmo *= 100;
	if (tmo)
		tmo += 100/2;

	if (ioctl(f, SG_SET_TIMEOUT, &tmo) < 0)
		printf("Cannot set SG_SET_TIMEOUT.\n");
}
//--------------------------------------------------------------------
__s32		scgo_fileno(St_GrScsiIf *scgp,	__s32	busno, __s32 tgt,	__s32	tlun)
{
	if (busno >= 1000)
		busno = busno - 1000 + MAX_SCG;

	if (busno < 0 || busno >= (MAX_SCG+MAX_ATA) ||
	    tgt < 0 || tgt >= MAX_TGT ||
	    tlun < 0 || tlun >= MAX_LUN)
		return (-1);

	return ((__s32)scglocal(scgp).scgfiles[busno][tgt][tlun]);
}
//--------------------------------------------------------------------
__s32		scgo_send(St_GrScsiIf *scgp)
{
	struct scg_cmd	*sp = &scgp->scmd;
	__s32		ret;
	sg_io_hdr_t	sg_io;
	struct timeval	to;
	static	uid_t		cureuid = 0;

	//printf("scgo_send %d\n", scgp->fd);
	if (scgp->fd < 0) {
		sp->error = SCG_FATAL;
		sp->ux_errno = EIO;
		return (0);
	}
	if (scglocal(scgp).isold > 0) {
		return (sg_rwsend(scgp));
	}
	memset((caddr_t)&sg_io, '\0', sizeof (sg_io));

	sg_io.interface_id = 'S';

	if (sp->flags & SCG_RECV_DATA) {
		sg_io.dxfer_direction = SG_DXFER_FROM_DEV;
	} else if (sp->size > 0) {
		sg_io.dxfer_direction = SG_DXFER_TO_DEV;
	} else {
		sg_io.dxfer_direction = SG_DXFER_NONE;
	}
	sg_io.cmd_len = sp->cdb_len;
	if (sp->sense_len > SG_MAX_SENSE)
	{
		sg_io.mx_sb_len = SG_MAX_SENSE;
	}
	else
		sg_io.mx_sb_len = sp->sense_len;

	sg_io.dxfer_len = sp->size;
	sg_io.dxferp = sp->addr;
	sg_io.cmdp = sp->cdb.cmd_cdb;
	sg_io.sbp = sp->u_sense.cmd_sense;
	sg_io.timeout = sp->timeout*1000;
	sg_io.flags |= SG_FLAG_DIRECT_IO;

	if (cureuid != 0)
		seteuid(0);
again:
	errno = 0;

	ret = ioctl(scgp->fd, SG_IO, &sg_io);
	if (ret < 0 && errno == EPERM) 
	{	/* XXX Hack until we have uid management */
		cureuid = geteuid();
		if (seteuid(0) >= 0)
			goto again;
	}
	if (cureuid != 0)
		seteuid(cureuid);

	if (ret < 0) 
	{
		sp->ux_errno = errno;
		if ((sp->ux_errno == ENOTTY || sp->ux_errno == EINVAL) &&
		    scglocal(scgp).isold < 0) 
		{
			scglocal(scgp).isold = 1;
			return (sg_rwsend(scgp));
		}
		if (sp->ux_errno == ENXIO || sp->ux_errno == EPERM ||
		    sp->ux_errno == EINVAL || sp->ux_errno == EACCES) 
		{
			printf( "scgo_send - permisition error\n" );
			return (-1);
		}
	}

	sp->u_scb.cmd_scb[0] = sg_io.status;
	sp->sense_count = sg_io.sb_len_wr;

	switch (sg_io.host_status) 
	{

	case DID_OK:
			if ((sg_io.driver_status & DRIVER_SENSE) != 0) 
			{
				if (sp->ux_errno == 0)
					sp->ux_errno = EIO;

				if (sp->u_sense.cmd_sense[0] != 0 &&
				    sp->u_scb.cmd_scb[0] == 0) 
				{
					sp->u_scb.cmd_scb[0] = ST_CHK_COND;
					if (sp->sense_count == 0)
						sp->sense_count = SG_MAX_SENSE;

					if ((sp->u_sense.cmd_sense[2] == 0) &&
					    (sp->u_sense.cmd_sense[12] == 0) &&
					    (sp->u_sense.cmd_sense[13] == 0)) 
					{
						sp->u_scb.cmd_scb[0] = 0;
						sp->u_sense.cmd_sense[0] = 0;
						sp->sense_count = 0;
					}
				}
			}
			break;

	case DID_NO_CONNECT:	/* Arbitration won, retry NO_CONNECT? */
			sp->error = SCG_RETRYABLE;
			break;
	case DID_BAD_TARGET:
			sp->error = SCG_FATAL;
			break;

	case DID_TIME_OUT:
		__scg_times(scgp);

		if (sp->timeout > 1 && scgp->cmdstop.tv_sec == 0) {
			sp->u_scb.cmd_scb[0] = 0;
			sp->error = SCG_FATAL;	/* a selection timeout */
		} else {
			sp->error = SCG_TIMEOUT;
		}
		break;

	default:
		to.tv_sec = sp->timeout;
		to.tv_usec = 500000;
		__scg_times(scgp);

		if (scgp->cmdstop.tv_sec < to.tv_sec || (scgp->cmdstop.tv_sec == to.tv_sec &&
				scgp->cmdstop.tv_usec < to.tv_usec))
		{
			sp->ux_errno = 0;
			sp->error = SCG_TIMEOUT;	/* a timeout */
		} else {
			sp->error = SCG_RETRYABLE;
		}
		break;
	}
	if (sp->error && sp->ux_errno == 0)
		sp->ux_errno = EIO;

	sp->resid = sg_io.resid;
	return (0);
}
//--------------------------------------------------------------------
__s8*	scgo_version(St_GrScsiIf	*scgp, __s32 what)
{
	if (scgp != (St_GrScsiIf *)0) {
		switch (what) {

		case SCG_VERSION:
			return (_scg_trans_version);

		case SCG_AUTHOR:
			return (_scg_auth_schily);
		case SCG_SCCS_ID:
			return (__sccsid);
		case SCG_KVERSION:
			{
				static	__s8 kv[16];
				__s32	n;

				if (scglocal(scgp).drvers >= 0) {
					n = scglocal(scgp).drvers;
					return (kv);
				}
			}
		}
	}
	return ((__s8 *)0);
}
//--------------------------------------------------------------------
__s32		scgo_help(St_GrScsiIf *scgp,	FILE	*f)
{
	return (0);
}
//--------------------------------------------------------------------
long	scgo_maxdma(St_GrScsiIf *scgp,	long amt)
{
	long maxdma = MAX_DMA_LINUX;
	__s32 	test;

	if (scglocal(scgp).drvers >= 20134)
		maxdma = sg_raisedma(scgp, amt);

	if (maxdma < 0) {
		if (scglocal(scgp).pgbus == 0)
			return PG_MAX_DATA;
		if (scglocal(scgp).scgfile >= 0)
			maxdma = MAX_DMA_LINUX;
	}

	if (scg_scsibus(scgp) == scglocal(scgp).pgbus)
		return (PG_MAX_DATA);

	test = scg_scsibus(scgp);
	//if ((scg_scsibus(scgp) < 0) && (PG_MAX_DATA < maxdma))
	if ((test < 0) && (PG_MAX_DATA < maxdma))
		return (PG_MAX_DATA);
	
	return (maxdma);
}
//--------------------------------------------------------------------
__s32		scgo_close(St_GrScsiIf	*scgp)
{
	__s32	f;
	__s32	b;
	__s32	t;
	__s32	l;

	for (b = 0; b < MAX_SCG+MAX_ATA; b++) {
		if (b == scglocal(scgp).pgbus)
			continue;
		scglocal(scgp).buscookies[b] = (__s16)-1;
		for (t = 0; t < MAX_TGT; t++) {
			for (l = 0; l < MAX_LUN; l++) {
				f = scglocal(scgp).scgfiles[b][t][l];
#ifdef LINUX_APP
				if (f >= 0)
					close(f);
#endif
				scglocal(scgp).scgfiles[b][t][l] = (__s16)-1;
			}
		}
	}
	if (scglocal(scgp).xbuf != NULL) 
	{
		free(scglocal(scgp).xbuf);
		scglocal(scgp).xbufsize = 0L;
		scglocal(scgp).xbuf = NULL;
	}

	return (0);
}
//--------------------------------------------------------------------
void*	scgo_getbuf(St_GrScsiIf *scgp, long amt)
{
	__s8	*ret;

	ret = (__s8*)valloc((size_t)(amt+getpagesize()));
	if (ret == NULL)
		return (ret);
	scgp->bufbase = ret;
	ret += getpagesize();
	scglocal(scgp).SCSIbuf = ret;
	return ((void *)ret);
}
//--------------------------------------------------------------------
void	scgo_freebuf(St_GrScsiIf	*scgp)
{
	if (scgp->bufbase)
	{
		free(scgp->bufbase);
	}
	scgp->bufbase = NULL;
}
//--------------------------------------------------------------------
__s32		scgo_numbus(St_GrScsiIf *scgp)
{
	return (1000+MAX_ATA);
	return (MAX_SCG+MAX_ATA);
}
//--------------------------------------------------------------------
BOOL8	scgo_havebus(St_GrScsiIf	*scgp, __s32 busno)
{
	__s32	t;
	__s32	l;

	if (busno >= 1000) {
		if (busno >= (1000+MAX_ATA))
			return (FALSE);
		busno = busno - 1000 + MAX_SCG;
	} else if (busno < 0 || busno >= MAX_SCG)
		return (FALSE);

	for (t = 0; t < MAX_TGT; t++) {
		for (l = 0; l < MAX_LUN; l++)
			if (scglocal(scgp).scgfiles[busno][t][l] >= 0)
				return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
__s32		scgo_initiator_id(St_GrScsiIf *scgp)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32		scgo_reset(St_GrScsiIf	*scgp, __s32 what)
{
	__s32	f = scgp->fd;
	__s32	func = -1;


	func = SG_SCSI_RESET_NOTHING;
	if (ioctl(f, SG_SCSI_RESET, &func) >= 0) {
		if (what == SCG_RESET_NOP)
			return (0);

		if (what == SCG_RESET_TGT) {
			func = SG_SCSI_RESET_DEVICE;
			if (ioctl(f, SG_SCSI_RESET, &func) >= 0)
				return (0);
		}

		if (what == SCG_RESET_BUS) {
			func = SG_SCSI_RESET_BUS;
			if (ioctl(f, SG_SCSI_RESET, &func) >= 0)
				return (0);
		}

	}
	return (-1);
}
//--------------------------------------------------------------------
__s32	scgo_dsend(St_GrScsiIf *scgp)
{
	errno = EINVAL;
	return (-1);
}
//--------------------------------------------------------------------
__s8 *	scgo_dversion(St_GrScsiIf *scgp, __s32 what)
{
	if (scgp != (St_GrScsiIf *)0) {
		switch (what) {

		case SCG_VERSION:
			return (_scg_trans_dversion);
		case SCG_AUTHOR:
			return (_scg_auth_schily);
		case SCG_SCCS_ID:
			return (_sccsid);
		}
	}
	return ((__s8 *)0);
}
//--------------------------------------------------------------------
__s32	scgo_dhelp(St_GrScsiIf *scgp, FILE *f)
{
	return (0);
}
//--------------------------------------------------------------------
__s32	scgo_nohelp(St_GrScsiIf *scgp, FILE *f)
{
	return (0);
}
//--------------------------------------------------------------------
__s32	scgo_ropen(St_GrScsiIf *scgp, __s8 *device)
{
	return (-1);	/* Keep lint happy */
}
//--------------------------------------------------------------------
__s32	scgo_dopen(St_GrScsiIf *scgp, __s8 *device)
{
	return (-1);	/* Keep lint happy */
}
//--------------------------------------------------------------------
__s32	scgo_dclose(St_GrScsiIf *scgp)
{
	errno = EINVAL;
	return (-1);
}
//--------------------------------------------------------------------
long	scgo_dmaxdma(St_GrScsiIf *scgp, long amt)
{
	errno = EINVAL;
	return	(0L);
}
//--------------------------------------------------------------------
void *	scgo_dgetbuf(St_GrScsiIf *scgp, long amt)
{
	errno = EINVAL;
	return ((void *)0);
}
//--------------------------------------------------------------------
void	scgo_dfreebuf(St_GrScsiIf *scgp)
{
}
//--------------------------------------------------------------------
__s32	scgo_dnumbus(St_GrScsiIf *scgp)
{
	return (0);
}
//--------------------------------------------------------------------
BOOL8	scgo_dhavebus(St_GrScsiIf *scgp, __s32 busno)
{
	return (FALSE);
}
//--------------------------------------------------------------------
__s32	scgo_dfileno(St_GrScsiIf *scgp, __s32 busno, __s32 tgt, __s32 tlun)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32	scgo_dinitiator_id(St_GrScsiIf *scgp)
{
	return (-1);
}
//--------------------------------------------------------------------
__s32	scgo_disatapi(St_GrScsiIf *scgp)
{
	return (FALSE);
}
//--------------------------------------------------------------------
__s32	scgo_dreset(St_GrScsiIf *scgp, __s32 what)
{
	errno = EINVAL;
	return (-1);
}
//--------------------------------------------------------------------
__s32	sg_rwsend(St_GrScsiIf *scgp)
{
	__s32		f = scgp->fd;
	struct scg_cmd	*sp = &scgp->scmd;
	struct sg_rq	*sgp;
	struct sg_rq	*sgp2;
	__s32	i;
	__s32	pack_len;
	__s32	reply_len;
	__s32	amt = sp->cdb_len;
	struct sg_rq {
		struct sg_header	hd;
		unsigned __s8		buf[MAX_DMA_LINUX+SCG_MAX_CMD];
	} sg_rq;
	__s8	driver_byte;
	__s8	host_byte;
	__s8	msg_byte;
	__s8	status_byte;


	if (f < 0) {
		sp->error = SCG_FATAL;
		sp->ux_errno = EIO;
		return (0);
	}

	if (sp->timeout != scgp->deftimeout)
		sg_settimeout(f, sp->timeout);


	sgp2 = sgp = &sg_rq;
	if (sp->addr == scglocal(scgp).SCSIbuf) {
		sgp = (struct sg_rq *)
			(scglocal(scgp).SCSIbuf - (sizeof (struct sg_header) + amt));
		sgp2 = (struct sg_rq *)
			(scglocal(scgp).SCSIbuf - (sizeof (struct sg_header)));
	} else {
		if (sp->size > (__s32)(sizeof (sg_rq.buf) - SCG_MAX_CMD)) {

			if (scglocal(scgp).xbuf == NULL) {
				scglocal(scgp).xbufsize = scgp->maxbuf;
				scglocal(scgp).xbuf = (__s8*)
					malloc(scglocal(scgp).xbufsize +
						SCG_MAX_CMD +
						sizeof (struct sg_header));
			}
			if (scglocal(scgp).xbuf == NULL ||
				sp->size > scglocal(scgp).xbufsize) {
				errno = ENOMEM;
				return (-1);
			}
			sgp2 = sgp = (struct sg_rq *)scglocal(scgp).xbuf;
		}
	}

	pack_len = sizeof (struct sg_header) + amt;
	reply_len = sizeof (struct sg_header);
	if (sp->flags & SCG_RECV_DATA) {
		reply_len += sp->size;
	} else {
		pack_len += sp->size;
	}

	memset((caddr_t)sgp, '\0', sizeof (struct sg_header));

	sgp->hd.pack_len = pack_len;
	sgp->hd.reply_len = reply_len;
	sgp->hd.pack_id = scglocal(scgp).pack_id++;

	if (amt == 12)
		sgp->hd.twelve_byte = 1;


	for (i = 0; i < amt; i++) {
		sgp->buf[i] = sp->cdb.cmd_cdb[i];
	}
	if (!(sp->flags & SCG_RECV_DATA)) {
		if ((void *)sp->addr != (void *)&sgp->buf[amt])
		{
			//movebytes(sp->addr, &sgp->buf[amt], sp->size);
			GrDumyCopyMem( &sgp->buf[amt], sp->addr, sp->size);
		}
		amt += sp->size;
	}

	i = sizeof (struct sg_header) + amt;
	if ((amt = write(f, sgp, i)) < 0) 
	{			/* write */
		sg_settimeout(f, scgp->deftimeout);
		printf( "sg_rwsend - write error\n" );
		return (-1);
	} else if (amt != i) 
	{

	}

	if (sp->addr == scglocal(scgp).SCSIbuf) 
	{
		//movebytes(sgp, sgp2, sizeof (struct sg_header));
		GrDumyCopyMem( sgp2, sgp, sizeof (struct sg_header));
		sgp = sgp2;
	}
	sgp->hd.sense_buffer[0] = 0;
	if ((amt = read(f, sgp, reply_len)) < 0) 
	{		/* read */
		sg_settimeout(f, scgp->deftimeout);
		printf( "sg_rwsend - read error\n" );
		return (-1);
	}

	if (sp->flags & SCG_RECV_DATA && ((void *)sgp->buf != (void *)sp->addr)) 
	{
		//movebytes(sgp->buf, sp->addr, sp->size);
		GrDumyCopyMem( sp->addr, sgp->buf, sp->size);
	}
	sp->ux_errno = GETINT(sgp->hd.result);		/* Unaligned read */
	sp->error = SCG_NO_ERROR;

	{
		if (GETINT(sgp->hd.result) == EBUSY) {	/* Unaligned read */
			struct timeval to;

			to.tv_sec = sp->timeout;
			to.tv_usec = 500000;
			__scg_times(scgp);

			if (sp->timeout > 1 && scgp->cmdstop.tv_sec == 0) {
				sp->u_scb.cmd_scb[0] = 0;
				sp->ux_errno = EIO;
				sp->error = SCG_FATAL;	/* a selection timeout */
			} else if (scgp->cmdstop.tv_sec < to.tv_sec ||
			    (scgp->cmdstop.tv_sec == to.tv_sec &&
				scgp->cmdstop.tv_usec < to.tv_usec)) {

				sp->ux_errno = EIO;
				sp->error = SCG_TIMEOUT;	/* a timeout */
			} else {
				sp->error = SCG_RETRYABLE;	/* may be BUS_BUSY */
			}
		}

		if (sp->flags & SCG_RECV_DATA)
			sp->resid = (sp->size + sizeof (struct sg_header)) - amt;
		else
			sp->resid = 0;	/* sg version1 cannot return DMA resid count */

		if (sgp->hd.sense_buffer[0] != 0) {
			sp->scb.chk = 1;
			sp->sense_count = SG_MAX_SENSE;
			//movebytes(sgp->hd.sense_buffer, sp->u_sense.cmd_sense, sp->sense_count);
			GrDumyCopyMem( sp->u_sense.cmd_sense, sgp->hd.sense_buffer, sp->sense_count);
			if (sp->ux_errno == 0)
				sp->ux_errno = EIO;
		}
	}

	if (sp->timeout != scgp->deftimeout)
		sg_settimeout(f, scgp->deftimeout);
	return (0);
}
//--------------------------------------------------------------------
long	sg_raisedma(St_GrScsiIf *scgp, long newmax)
{
	__s32	b;
	__s32	t;
	__s32	l;
	__s32	f;
	__s32	val;
	__s32	old;

	val = 126*1024;

	if (val > MAX_DMA_LINUX) {
		for (b = 0; b < (MAX_SCG+MAX_ATA); b++) {
			for (t = 0; t < MAX_TGT; t++) {
				for (l = 0; l < MAX_LUN; l++) {
					if ((f = SCGO_FILENO(scgp, b, t, l)) < 0)
						continue;
					old = 0;
					if (ioctl(f, SG_GET_RESERVED_SIZE, &old) < 0)
						continue;
					if (val > old)
						ioctl(f, SG_SET_RESERVED_SIZE, &val);
				}
			}
		}
	}

	if (newmax > val) {
		val = newmax;
		for (b = 0; b < (MAX_SCG+MAX_ATA); b++) {
			for (t = 0; t < MAX_TGT; t++) {
				for (l = 0; l < MAX_LUN; l++) {
					if ((f = SCGO_FILENO(scgp, b, t, l)) < 0)
						continue;
					old = 0;
					if (ioctl(f, SG_GET_RESERVED_SIZE, &old) < 0)
						continue;
					if (val > old)
						ioctl(f, SG_SET_RESERVED_SIZE, &val);
				}
			}
		}
	}

	for (b = 0; b < (MAX_SCG+MAX_ATA); b++) {
		for (t = 0; t < MAX_TGT; t++) {
			for (l = 0; l < MAX_LUN; l++) {

				if ((f = SCGO_FILENO(scgp, b, t, l)) < 0)
					continue;
				if (ioctl(f, SG_GET_RESERVED_SIZE, &val) < 0)
					continue;

				sg_checkdma(f, &val);	/* This does not work */

				if (val < newmax)
					newmax = val;
			}
		}
	}

	return ((long)newmax);
}
//--------------------------------------------------------------------
void	sg_checkdma(__s32	f, __s32 *valp)
{
#ifdef LINUX_APP
	struct stat	sb;
#endif
	__s32		val;
	__s8		nbuf[80];
	__s32		fx;

	if (ioctl(f, SG_GET_PACK_ID, &val) < 0)	/* Only works with /dev/sg* */
		return;
	val = -1;
#ifdef LINUX_APP
	if (fstat(f, &sb) >= 0)			/* Try to get instance # */
		val = minor(sb.st_rdev);
#endif
	if (val < 0)
		return;


	/*js_snprintf(nbuf, sizeof (nbuf),
	    "/sys/class/scsi_generic/sg%d/device/block/queue/max_hw_sectors_kb",
	    val);*/

	fx = open("/sys/class/scsi_generic/sg0/device/block/queue/max_hw_sectors_kb", O_RDONLY | O_NDELAY);
	if (fx >= 0)
	{
		if (read(fx, nbuf, sizeof (nbuf)) > 0)
		{
			val = -1;
			astoi(nbuf, &val);
			if (val > 0)
			{
				val *= 1024;
				if (*valp > val)
					*valp = val;
			}
		}
#ifdef LINUX_APP
		close(fx);
#endif
	}
}
//--------------------------------------------------------------------

