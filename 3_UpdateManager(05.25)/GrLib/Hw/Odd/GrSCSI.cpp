/*
	time Tools
*/
//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrSCSI.h>

#ifdef	LINUX_APP
	//#include	<unistd.h>
	#include	<fcntl.h>
	#include	<sys/ioctl.h>
	#include	<sys/time.h>
	#include <signal.h>
	#include <fcntl.h>
	#include <sys/resource.h>
	#include <sched.h>
	#include <unistd.h>
#endif
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<errno.h>

#include 	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrScsiOpr.h>
#include 	<Hw/Odd/GrScgo.h>

#include	<GrDumyTool.h>

//====================================================================
//local const
#define	Mac_GrOddMin(a, b)	((a) < (b) ? (a):(b))
#define	Mac_GrOddMax(a, b)	((a) < (b) ? (b):(a))

//====================================================================
//local type

//====================================================================
//local function
St_GrScsiIf* 	scg_smalloc(void);
void 		scg_sfree(St_GrScsiIf* scgp);
__s32 		scg__open(St_GrScsiIf	*scgp, __s8	*device);
void		exscsi(__s32 excode, void *arg);
__s32			scg__close(St_GrScsiIf *scgp);

//====================================================================
//var
extern struct timeval	starttime;
extern struct timeval	wstarttime;
extern struct timeval	stoptime;
//extern __s32		dminbuf;
//extern long		fs;

//====================================================================
//--------------------------------------------------------------------
St_GrScsiIf* scg_open(__s8 *scsidev, __s32 debug,	__s32 be_verbose)
{
	// var --------------------
		__s8	sdevname[256];
		St_GrScsiIf*		scgp;
		__s32	bus = 0;
		__s32	tgt = 0;
		__s32	lun = 0;
		__s32	x1;
		__s8	*devp = NULL;
		__s8	*sdev = NULL;
		__s32	n = 0;
	// code -------------------
		scgp 		= scg_smalloc();
		scgp->overbose = be_verbose;

		scg_settarget(scgp, -2, -2, -2);

		if (scg__open(scgp, scsidev ) <= 0)
		{
			scg_sfree(scgp);
			return ((St_GrScsiIf *)0);
		}
		return (scgp);
}
//--------------------------------------------------------------------
__s32 scg__open(St_GrScsiIf	*scgp, __s8	*device)
{
	// var --------------------
		__s32	ret;
		scg_ops_t *ops;
		extern	scg_ops_t scg_std_ops;
	// code -------------------
		scgp->ops = &scg_std_ops;

#ifdef LINUX_APP
		ret = SCGO_OPEN(scgp, device);
#else
		ret	=	1;
#endif

		if (ret < 0)
		{
			return (ret);
		}

		scg_settarget(scgp, scg_scsibus(scgp), scg_target(scgp), scg_lun(scgp));

		return (ret);
}
//--------------------------------------------------------------------
St_GrScsiIf* scg_smalloc(void)
{
	// var --------------------
		extern	scg_ops_t scg_dummy_ops;
		St_GrScsiIf*		Tv_Result;
	// code -------------------
		Tv_Result = (St_GrScsiIf*) malloc(sizeof(St_GrScsiIf));

		if ( NULL != Tv_Result )
		{
			GrDumyZeroMem( Tv_Result, sizeof(St_GrScsiIf) );
			Tv_Result->ops	=	&scg_dummy_ops;
			scg_settarget(Tv_Result, -1, -1, -1);
			Tv_Result->fd					= -1;
			Tv_Result->deftimeout = 20;
			Tv_Result->running		= FALSE;
			Tv_Result->errbeg			=	Tv_Result->errstr;
			Tv_Result->errptr			=	Tv_Result->errstr;
		}

		return Tv_Result;
}
//--------------------------------------------------------------------
void scg_sfree(St_GrScsiIf* scgp)
{
		//dma buffer free
		scgp->ops->scgo_freebuf(scgp);
		scgp->bufptr = NULL;

		free(scgp);
}
//--------------------------------------------------------------------
__s32		scg_settarget(St_GrScsiIf *scgp, __s32 busno, __s32 tgt, __s32 tlun)
{
	// var --------------------
		__s32 fd;
	// code -------------------
		fd 		= -1;
	
		if (scgp->ops != NULL)
		{
			fd = SCGO_FILENO(scgp, busno, tgt, tlun);
		}
			
		scgp->fd = fd;
		scg_scsibus(scgp) = busno;
		scg_target(scgp)  = tgt;
		scg_lun(scgp)	  = tlun;

		return (fd);
}
//--------------------------------------------------------------------
void 	scg_settimeout(St_GrScsiIf	*scgp, __s32 timeout)
{
		scgp->deftimeout = timeout;
}
//--------------------------------------------------------------------
__s32		select_target(St_GrScsiIf *scgp, FILE *f)
{
		return (_select_target(scgp, f, -1, -1));
}
//--------------------------------------------------------------------
__s32		find_target(St_GrScsiIf	*scgp, __s32 type, __s32 idx)
{
	return (_select_target(scgp, (FILE *)NULL, type, idx));
}
//--------------------------------------------------------------------
__s32 _select_target(St_GrScsiIf	*scgp, FILE	*f,	__s32	type,	__s32	idx)	
{
	// var --------------------
		__s32	initiator;
		__s32	cscsibus;
		__s32	ctarget;
		__s32	clun;
		__s32	numbus;
		__s32	n;
		__s32	low	= -1;
		__s32	high	= -1;
		__s32	amt	= -1;
		__s32	bus;
		__s32	tgt;
		__s32	lun = 0;
		__s32	err;
		BOOL8	have_tgt;
	// code -------------------
		cscsibus	= scg_scsibus(scgp);
		ctarget	  = scg_target(scgp);
		clun		 	= scg_lun(scgp);
		numbus 		= scgp->ops->scgo_numbus(scgp);

		if (numbus < 0)
			numbus = 1024;

		for(bus = 0; bus < numbus; bus++)
		{
			scg_settarget(scgp, bus, 0, 0);
	
			if(!scg_havebus(scgp, bus))
				continue;
	
			//initiator = scg_initiator_id(scgp);
			initiator = scgp->ops->scgo_initiator_id(scgp);
	
			for (tgt = 0; tgt < 16; tgt++)
			{
				n = bus * 100 + tgt;

				scg_settarget(scgp, bus, tgt, lun);

				have_tgt = unit_ready(scgp) || scgp->scmd.error != SCG_FATAL;
				err = errno;
				if (err == EPERM || err == EACCES)
					return (-1);
	
				if (!have_tgt && tgt > 7)
				{
					if (scgp->scmd.ux_errno == EINVAL)
						break;
					continue;
				}

				if (tgt == initiator) {
					if (f)
						fprintf(f, "HOST ADAPTOR\n");
					continue;
				}
				if (!have_tgt)
				{
					continue;
				}
				if (low < 0)
					low = n;
				high = n;

				getdev(scgp, FALSE);

				if (type >= 0 && scgp->inq.type == type)
				{
					amt++;
					if (amt == 0)	//amt starts at -1
						amt++;
					if (amt == idx) {
						return (amt);
					}
				} else if (type < 0) {
					amt++;
				}
				if (amt == 0)	//amt starts at -1
					amt++;
			}
		}
	
		if (low < 0)
		{
			return (0);
		}
		n = -1;

		scg_settarget(scgp, cscsibus, ctarget, clun);

		return (amt);
}
//--------------------------------------------------------------------
long	scg_bufsize(St_GrScsiIf *scgp, long amt)
{
	// var --------------------
		long	maxdma;
	// code -------------------
		maxdma = SCGO_MAXDMA(scgp, amt);

		if (amt <= 0 || amt > maxdma)
			amt = maxdma;
	
		scgp->maxdma = maxdma;	/* Max possible  */
		scgp->maxbuf = amt;	/* Current value */
	
		return (amt);
}
//--------------------------------------------------------------------
void*	scg_getbuf(St_GrScsiIf	*scgp, long	amt)
{
	// var --------------------
		void	*buf;
	// code -------------------
		if (amt <= 0 || amt > scg_bufsize(scgp, amt))
			return ((void *)0);
	
		buf = SCGO_GETBUF(scgp, amt);
		scgp->bufptr = buf;
		return (buf);
}
//--------------------------------------------------------------------
BOOL8 	scg_havebus(St_GrScsiIf *scgp,	__s32	busno)
{
		return scgp->ops->scgo_havebus(scgp, busno);
}
//--------------------------------------------------------------------
__s32		scg_cmd(St_GrScsiIf *scgp)
{
		__s32		ret;
		struct	scg_cmd	*scmd = &scgp->scmd;
		
#ifndef LINUX_APP
		return	0;
#endif

		scgp->errptr = scgp->errbeg = scgp->errstr;
	
		if (scmd->timeout == 0 || scmd->timeout < scgp->deftimeout)
			scmd->timeout = scgp->deftimeout;
		/*
		if (scgp->disre_disable)
			scmd->flags &= ~SCG_DISRE_ENA;
		if (scgp->noparity)
			scmd->flags |= SCG_NOPARITY;
		*/
	
		scmd->u_sense.cmd_sense[0] = 0;		/* Paranioa */
		if (scmd->sense_len > SCG_MAX_SENSE)
			scmd->sense_len = SCG_MAX_SENSE;
		else if (scmd->sense_len < 0)
			scmd->sense_len = 0;
	
		//scgp->cb_fun = NULL;
		gettimeofday( &scgp->cmdstart, NULL );
		scgp->curcmdname = scgp->cmdname;
		scgp->running = TRUE;
		ret = SCGO_SEND(scgp);
		scgp->running = FALSE;
		__scg_times(scgp);
		if (ret < 0)
		{
			printf( "scg_cmd - SCGO_SEND error\n" );
			if (scg_target(scgp) < 8 || errno != EINVAL) 
			{
				__s32	err = errno;
	
				/*
				if (scgp->flags & SCGF_PERM_EXIT)
					exit(err);
				*/
			}
			if (scmd->ux_errno == 0)
				scmd->ux_errno = errno;
			if (scmd->error == SCG_NO_ERROR)
				scmd->error = SCG_FATAL;
		}
	
		ret = scg_vtail(scgp);
		/*
		if ( 0 > ret )
		{
			printf( "scg_cmd - (vtail)error. error: %d , ux_errno: %d , scb : %d , u_sense.cmd_sense[0] : %d\n"	, 
				scgp->scmd.error, scgp->scmd.ux_errno , *(unsigned __s8 *)&scgp->scmd.scb ,	
				scgp->scmd.u_sense.cmd_sense[0] );
		}
		*/
		//scg_errflush(scgp);

		/*
		if (scgp->cb_fun != NULL)
			(*scgp->cb_fun)(scgp->cb_arg);
		*/
		return (ret);
}
//--------------------------------------------------------------------
__s32		scg_sense_key(St_GrScsiIf *scgp)
{
		struct scg_cmd *cp = &scgp->scmd;
		__s32	key = -1;
	
		if (!scg_cmd_err(scgp))
			return (0);
	
		if (cp->sense.code >= 0x70)
			key = ((struct scsi_ext_sense *)&(cp->sense))->key;
		return (key);
}
//--------------------------------------------------------------------
__s32		scg_cmd_status(St_GrScsiIf	*scgp)
{
		struct scg_cmd	*cp = &scgp->scmd;
		__s32		cmdstatus = *(unsigned __s8*)&cp->scb;
	
		return (cmdstatus);
}
//--------------------------------------------------------------------
__s32		scg_sense_code(St_GrScsiIf	*scgp)
{
		struct scg_cmd *cp = &scgp->scmd;
		__s32	code = -1;
	
		if (!scg_cmd_err(scgp))
			return (0);
	
		if (cp->sense.code >= 0x70)
			code = ((struct scsi_ext_sense *)&(cp->sense))->sense_code;
		else
			code = cp->sense.code;
		return (code);
}
//--------------------------------------------------------------------
BOOL8		scg_cmd_err(St_GrScsiIf	*scgp)
{
	struct scg_cmd *cp = &scgp->scmd;

	if (cp->error != SCG_NO_ERROR || cp->ux_errno != 0 ||	*(unsigned __s8 *)&cp->scb != 0 ||
				cp->u_sense.cmd_sense[0] != 0)
		return (TRUE);
	return (FALSE);
}
//--------------------------------------------------------------------
void	__scg_times(St_GrScsiIf *scgp)
{
		struct timeval	*stp = &scgp->cmdstop;
	
		gettimeofday(stp, (struct timezone *)0);
		stp->tv_sec -= scgp->cmdstart.tv_sec;
		stp->tv_usec -= scgp->cmdstart.tv_usec;
		while (stp->tv_usec < 0) 
		{
			stp->tv_sec -= 1;
			stp->tv_usec += 1000000;
		}
}
//--------------------------------------------------------------------
__s32		scg_sense_qual(St_GrScsiIf *scgp)
{
	struct scg_cmd *cp = &scgp->scmd;

	if (!scg_cmd_err(scgp))
		return (0);

	if (cp->sense.code >= 0x70)
		return (((struct scsi_ext_sense *)&(cp->sense))->qual_code);
	else
		return (0);
}
//--------------------------------------------------------------------
void	scg_printerr(St_GrScsiIf	*scgp)
{
		//not code
}
//--------------------------------------------------------------------
void	reload_media(St_GrScsiIf	*scgp,	cdr_t	*dp)
{
	__s8	ans[2];
	__s32	f = -1;

	unload_media(scgp, dp, F_EJECT);

	if ((dp->cdr_flags & CDR_TRAYLOAD) != 0) {
		load_media(scgp, dp, FALSE);
	}

	if (((dp->cdr_flags & CDR_TRAYLOAD) == 0) ||
				!wait_unit_ready(scgp, 5)) {
		static FILE	*tty = NULL;

		if (tty == NULL) {
			tty = stdin;
			if ((dp->cdr_cmdflags & F_STDIN) != 0)
				tty = fileluopen(STDERR_FILENO, "rw");
		}

		if (tty != NULL)
			f = fcntl(fileno(tty), F_GETFL, (long)0);
		if (f < 0 || (f & O_ACCMODE) == O_WRONLY) 
		{
			signal(SIGUSR1, catchsig);
			pause();
		} 
		else
		{
			fgetline(tty, ans, 1);
		}

	}

	load_media(scgp, dp, TRUE);
}
//--------------------------------------------------------------------
__s32		write_secs(St_GrScsiIf	*scgp, cdr_t *dp,	__s8 *bp,	long startsec, __s32 bytespt, __s32	secspt,	BOOL8 islast)
{
	__s32	amount;

again:
	amount = (*dp->cdr_write_trackdata)(scgp, bp, startsec, bytespt, secspt, islast);
	if (amount < 0)
	{
		if (scsi_in_progress(scgp))
		{
			if ((dp->cdr_dstat->ds_flags & DSF_NOCD) == 0)
			{
				usleep(60000);	/* CD case */
			}
			else
			{
				if (sysconf(_SC_CLK_TCK) < 100)
					usleep(20000);
				else
					usleep(10000);
			}
			goto again;
		}
		return (-1);
	}
	return (amount);
}
//--------------------------------------------------------------------
__s32		scg_getresid(St_GrScsiIf	*scgp)
{
	return (scgp->scmd.resid);
}
//--------------------------------------------------------------------
void	load_media(St_GrScsiIf	*scgp, cdr_t	*dp,	BOOL8	doexit)
{
	__s32	code;
	__s32	key;
	__s32	err;
	BOOL8	immed = (dp->cdr_cmdflags&F_IMMED) != 0;

	test_unit_ready(scgp);		/* First eat up unit attention		*/
	if ((*dp->cdr_load)(scgp, dp) < 0) {	/* now try to load media and	*/
		if (!doexit)
			return;
	}
	scsi_start_stop_unit(scgp, 1, 0, immed); /* start unit in silent mode	*/

	if (!wait_unit_ready(scgp, 60)) {
		code = scg_sense_code(scgp);
		key = scg_sense_key(scgp);
		scsi_prevent_removal(scgp, 0); /* In case someone locked it */

		if (!doexit)
			return;
	}

	scsi_prevent_removal(scgp, 1);
	scsi_start_stop_unit(scgp, 1, 0, immed);
	wait_unit_ready(scgp, 120);

	errno = 0;
	code = rezero_unit(scgp); /* Not supported by some drives */
	err = errno;
	if (code < 0 && (err == EPERM || err == EACCES)) {
		printf("linuxcheck();");
	}


	test_unit_ready(scgp);
	scsi_start_stop_unit(scgp, 1, 0, immed);
	wait_unit_ready(scgp, 120);
}
//--------------------------------------------------------------------
void	unload_media(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags)
{
	scsi_prevent_removal(scgp, 0);
	if ((flags & F_EJECT) != 0) {
		if ((*dp->cdr_unload)(scgp, dp) < 0)
			printf("Cannot eject media.\n");
	}
}
//--------------------------------------------------------------------
__s32		find_drive(St_GrScsiIf	*scgp, __s8	*dev,	__s32	flags)
{
	__s32	ntarget;
	
	ntarget = find_target(scgp, INQ_ROMD, -1);
	if (ntarget < 0)
	{
		return (ntarget);
	}
	if (ntarget == 1) {
		find_target(scgp, INQ_ROMD, 1);
	} else if (ntarget <= 0 && (ntarget = find_target(scgp, INQ_WORM, -1)) == 1) {
		find_target(scgp, INQ_WORM, 1);
	}

	return (ntarget);
}
//--------------------------------------------------------------------
__s32		pad_track(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp, long startsec, __u64 amt,
				BOOL8 dolast, __u64 *bytesp)
{
	__s32	track = trackp->trackno;
	__u64	bytes	= 0;
	__u64 savbytes = 0;
	__u64	padsize = amt;
	__s32	secsize;
	__s32	secspt;
	__s32	bytespt;
	__s32	amount;
	BOOL8	neednl	= FALSE;
	BOOL8	islast	= FALSE;
	struct timeval tlast;
	struct timeval tcur;
	float	secsps = 75.0;
	long bsize;
	long bfree;
	__s32 per;

	if (dp->cdr_dstat->ds_flags & DSF_DVD)
		secsps = 676.27;
	if (dp->cdr_dstat->ds_flags & DSF_BD)
		secsps = 2195.07;

	if ((*dp->cdr_buffer_cap)(scgp, &bsize, &bfree) < 0)
		bsize = -1L;
	if (bsize == 0)		/* If we have no (known) buffer, we cannot */
		bsize = -1L;	/* retrieve the buffer fill ratio	   */

	secsize = trackp->secsize;
	secspt = trackp->secspt;
	bytespt = secsize * secspt;

	memset(scgp->bufptr, '\0', bytespt);

	/*
	if ((amt >> 20) > 0) {
		flush();
	}
	*/
	gettimeofday(&tlast, (struct timezone *)0);
	do {
		if (amt < bytespt) {
			bytespt = roundup(amt, secsize);
			secspt = bytespt / secsize;
		}
		if (dolast && (amt - bytespt) <= 0)
			islast = TRUE;

		if (is_raw(trackp)) {
			encsectors(trackp, (unsigned __s8 *)scgp->bufptr, startsec, secspt);
			fillsubch(trackp, (unsigned __s8 *)scgp->bufptr, startsec, secspt);
		}

		amount = write_secs(scgp, dp, (__s8*)scgp->bufptr, startsec, bytespt, secspt, islast);
		if (amount < 0) {
			printf("%swrite track pad data: error after %lld bytes\n",
							neednl?"\n":"", bytes);
			if (bytesp)
				*bytesp = bytes;
			(*dp->cdr_buffer_cap)(scgp, (long *)0, (long *)0);
			return (-1);
		}
		amt -= amount;
		bytes += amount;
		startsec += amount / secsize;

	} while (amt > 0);

	if (bytesp)
		*bytesp = bytes;
	if (bytes == 0)
		return (0);
	return (bytes > 0 ? 1:-1);
}
//--------------------------------------------------------------------
void	catchsig(__s32	sig)
{
	
}
//--------------------------------------------------------------------
__s32		read_buffer(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 mode)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->dma_read = 1;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x3C;	/* Read Buffer */
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] |= (mode & 7);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read buffer";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		write_buffer(St_GrScsiIf *scgp, __s8 *buffer, long length, __s32 mode, __s32 bufferid, long offset)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	__s8		*cdb;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = buffer;
	scmd->size = length;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;

	cdb = (__s8 *)scmd->cdb.cmd_cdb;

	cdb[0] = 0x3B;
	cdb[1] = mode & 7;
	cdb[2] = bufferid;
	cdb[3] = offset >> 16;
	cdb[4] = (offset >> 8) & 0xff;
	cdb[5] = offset & 0xff;
	cdb[6] = length >> 16;
	cdb[7] = (length >> 8) & 0xff;
	cdb[8] = length & 0xff;

	scgp->cmdname = "write_buffer";

	if (scg_cmd(scgp) >= 0)
		return (1);
	return (0);
}
//--------------------------------------------------------------------
__s32		get_dmaspeed(St_GrScsiIf	*scgp, cdr_t *dp, __s32 A_BufSize )
{
	// local -------------------
		__s32	i;
		long	t;
		__s32	bs;
		__s32	tsize;
		__s32	maxdma;
		extern struct timeval	starttime;
		extern struct timeval	fixtime;
		__s8*	Tv_PtrBuf;
	// code --------------------
		Tv_PtrBuf	=	(__s8*)scgp->bufptr;

		memset((caddr_t)Tv_PtrBuf, '\0', 4);
		tsize = 0;
		i = read_buffer(scgp, Tv_PtrBuf, 4, 0);
		if (i < 0 || scg_getresid(scgp) != 0)
			return (-1);
		tsize = a_to_u_3_byte(&Tv_PtrBuf[1]);
		if (tsize <= 0)
			return (-1);

		bs = A_BufSize;
		if (tsize < bs)
			bs = tsize;
		if (bs > 0xFFFE)
			bs = 0xFFFE;

		memset((caddr_t)Tv_PtrBuf, '\0', bs);
		if (read_buffer(scgp, Tv_PtrBuf, bs, 0) < 0) {
			return (-1);
		}

		for (i = bs-1; i >= 0; i--)
		{
			if (Tv_PtrBuf[i] != '\0')
			{
				break;
			}
		}
		i++;
		maxdma = i;
		memset((caddr_t)Tv_PtrBuf, 0xFF, bs);
		if (read_buffer(scgp, Tv_PtrBuf, bs, 0) < 0) {
			return (-1);
		}

		for (i = bs-1; i >= 0; i--) {
			if ((Tv_PtrBuf[i] & 0xFF) != 0xFF) {
				break;
			}
		}
		i++;
		if (i > maxdma)
			maxdma = i;

		if (maxdma < bs)
		{
			return (-1);
		}
		if (maxdma < bs)
			bs = maxdma;

		if (_read_buffer(scgp, bs) < 0) {
			return (-1);
		}

		if (gettimeofday(&starttime, (struct timezone *)0) < 0)
			return (-1);

		for (i = 0; i < 100; i++) {
			if (_read_buffer(scgp, bs) < 0)
				return (-1);
		}

		if (gettimeofday(&fixtime, (struct timezone *)0) < 0) {
			return (-1);
		}
		timevaldiff(&starttime, &fixtime);
		tsize = bs * 100;
		t = fixtime.tv_sec * 1000 + fixtime.tv_usec / 1000;
		if (t <= 0)
			return (-1);

		return (tsize/t);
}
//--------------------------------------------------------------------
__s32		_read_buffer(St_GrScsiIf	*scgp, __s32 size)
{
	// local -------------------
		__s8*	Tv_PtrBuf;
	// code --------------------
		Tv_PtrBuf	=	(__s8*)scgp->bufptr;
		Tv_PtrBuf[size-2] = (__s8)0x55; 
		Tv_PtrBuf[size-1] = (__s8)0xFF;
		if (read_buffer(scgp, Tv_PtrBuf, size, 0) < 0 || scg_getresid(scgp) != 0)
		{
			return (-1);
		}

		if (Tv_PtrBuf[size-2] == (__s8)0x55 || Tv_PtrBuf[size-1] == (__s8)0xFF)
		{
			return (-1);
		}
		return (0);
}
//--------------------------------------------------------------------
void	opentracks(track_t *trackp, __u64 A_IsoSize)
{
	track_t	*tp;
	__s32	i;
	__s32	tracks = trackp[0].tracks;

	__u64	tracksize;
	__s32	secsize;

	printf("test tracks = %d\n", tracks); 
	for (i = 1; i <= tracks; i++)
	{
		tp = &trackp[i];

		checksize(tp, A_IsoSize);
		tracksize = tp->itracksize;
		secsize = tp->isecsize;

		if (!is_shorttrk(tp) && tracksize > 0 && (tracksize / secsize) < 300)
		{
			tracksize = roundup(tracksize, secsize);
			if ((tp->padsecs + (tracksize / secsize)) < 300)
			{
				tp->padsecs =
					300 - tracksize / secsize;
			}
		}

		if (tp->padsecs > 0)
		{
			tp->tracksecs += tp->padsecs;
		}
	}
}
//--------------------------------------------------------------------
void	checksize(track_t	*trackp, __u64 A_IsoSize)
{
	trackp->itracksize = A_IsoSize;

	if (trackp->itracksize >= 0)
	{
		trackp->tracksecs = (trackp->itracksize + trackp->isecsize -1) / trackp->isecsize;
		trackp->tracksize = (trackp->itracksize / trackp->isecsize) * trackp->secsize	+ trackp->itracksize % trackp->isecsize;
	}
	else
	{
		trackp->tracksecs = -1L;
	}
}
//--------------------------------------------------------------------
BOOL8	check_wrmode(cdr_t *dp,	__u32 wmode, __s32 tflags)
{
	__s32	cdflags = dp->cdr_flags;

	if ((tflags & TI_PACKET) != 0 && (cdflags & CDR_PACKET) == 0) {
		printf("Drive does not support PACKET recording.\n");
		return (FALSE);
	}
	if ((tflags & TI_TAO) != 0 && (cdflags & CDR_TAO) == 0) {
		printf("Drive does not support TAO recording.\n");
		return (FALSE);
	}
	if ((wmode & F_SAO) != 0) {
		if ((cdflags & CDR_SAO) == 0) {
			printf("Drive does not support SAO recording.\n");
			if ((cdflags & CDR_RAW) != 0)
				printf("Try -raw option.\n");
			return (FALSE);
		}

		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == TI_RAW && (cdflags & CDR_SRAW96P) == 0) {
			printf("Drive does not support SAO/RAW96P.\n");
			goto badsecs;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == (TI_RAW|TI_RAW96R) && (cdflags & CDR_SRAW96R) == 0) {
			printf("Drive does not support SAO/RAW96R.\n");
			goto badsecs;
		}
	}
	if ((wmode & F_RAW) != 0) {
		if ((cdflags & CDR_RAW) == 0) {
			printf("Drive does not support RAW recording.\n");
			return (FALSE);
		}
		if ((tflags & TI_RAW16) != 0 && (cdflags & CDR_RAW16) == 0) {
			printf("Drive does not support RAW/RAW16.\n");
			goto badsecs;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == TI_RAW && (cdflags & CDR_RAW96P) == 0) {
			printf("Drive does not support RAW/RAW96P.\n");
			goto badsecs;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == (TI_RAW|TI_RAW96R) && (cdflags & CDR_RAW96R) == 0) {
			printf("Drive does not support RAW/RAW96R.\n");
			goto badsecs;
		}
	}
	return (TRUE);

badsecs:
	if ((wmode & F_SAO) != 0)
		cdflags &= ~(CDR_RAW16|CDR_RAW96P|CDR_RAW96R);
	if ((wmode & F_RAW) != 0)
		cdflags &= ~(CDR_SRAW96P|CDR_SRAW96R);

	if ((cdflags & (CDR_SRAW96R|CDR_RAW96R)) != 0)
		printf("Try -raw96r option.\n");
	else if ((cdflags & (CDR_SRAW96P|CDR_RAW96P)) != 0)
		printf("Try -raw96p option.\n");
	else if ((cdflags & CDR_RAW16) != 0)
		printf("Try -raw16 option.\n");
	return (FALSE);
}
//--------------------------------------------------------------------
void	set_trsizes(cdr_t	*dp, __s32 tracks, track_t *trackp, __s32 A_BufSize )
{
	__s32	i;
	__s32	secsize;
	__s32	secspt;

	trackp[1].flags		|= TI_FIRST;
	trackp[tracks].flags	|= TI_LAST;

	for (i = 0; i <= tracks+1; i++)
	{
		if ((dp->cdr_flags & CDR_SWABAUDIO) != 0 &&	is_audio(&trackp[i]))
		{
			trackp[i].flags ^= TI_SWAB;
		}

		if (!is_audio(&trackp[i]))
		{
			trackp[i].flags &= ~TI_SWAB;
		}

		secsize = trackp[i].secsize;
		if (trackp[i].isecsize > secsize)
			secsize = trackp[i].isecsize;

		secspt = A_BufSize/secsize;
		secspt = Mac_GrOddMin(255, secspt);
		trackp[i].secspt = secspt;

		if (is_packet(&trackp[i]) && trackp[i].pktsize > 0)
		{
			if (trackp[i].secspt >= trackp[i].pktsize)
			{
				trackp[i].secspt = trackp[i].pktsize;
			}
			else
			{
				printf("Track %d packet size %d exceeds buffer limit of %d sectors",
					i, trackp[i].pktsize, trackp[i].secspt);
			}
		}
	}
}
//--------------------------------------------------------------------
void	setpregaps(__s32 tracks, track_t	*trackp)
{
	__s32	i;
	__s32	sectype;
	long	pregapsize;
	track_t	*tp;

	sectype = trackp[1].sectype;
	sectype &= ST_MASK;

	for (i = 1; i <= tracks; i++) {
		tp = &trackp[i];
		if (tp->pregapsize == -1L) {
			tp->pregapsize = 150;		/* Default CD Pre GAP*/
			if (trackp->flags & TI_NOCD) {
				tp->pregapsize = 0;
			} else if (sectype != (tp->sectype & ST_MASK)) {
				tp->pregapsize = 255;	/* Pre GAP is 255 */
				tp->flags &= ~TI_PREGAP;
			}
		}
		sectype = tp->sectype & ST_MASK;	/* Save old sectype */
	}
	trackp[tracks+1].pregapsize = 0;
	trackp[tracks+1].index0start = 0;

	for (i = 1; i <= tracks; i++) {
		trackp[i].index0start = trackp[i].tracksecs;

		pregapsize = trackp[i+1].pregapsize;
		if (is_pregap(&trackp[i+1]) && pregapsize > 0)
			trackp[i].index0start -= pregapsize;
	}
}
//--------------------------------------------------------------------
void	setleadinout(__s32 tracks, track_t	*trackp)
{
	if (!is_clone(&trackp[0])) {
		trackp[0].sectype = trackp[1].sectype;
		trackp[0].dbtype  = trackp[1].dbtype;
		trackp[0].dataoff = trackp[1].dataoff;

		if (is_audio(&trackp[1]))
			trackp[0].flags |= TI_AUDIO;
	}

	trackp[tracks+1].pregapsize = 0;
	trackp[tracks+1].isecsize   = trackp[tracks].isecsize;
	trackp[tracks+1].secsize    = trackp[tracks].secsize;

	if (!is_clone(&trackp[0])) {
		trackp[tracks+1].tracktype = trackp[tracks].tracktype;
		trackp[tracks+1].sectype   = trackp[tracks].sectype;
		trackp[tracks+1].dbtype    = trackp[tracks].dbtype;
		trackp[tracks+1].dataoff   = trackp[tracks].dataoff;
	}

	trackp[tracks+1].flags = trackp[tracks].flags;
}
//--------------------------------------------------------------------
void	set_wrmode(cdr_t	*dp, __u32 wmode,	__s32	tflags)
{
	dstat_t	*dsp = dp->cdr_dstat;

	if ((tflags & TI_PACKET) != 0) {
		dsp->ds_wrmode = WM_PACKET;
		return;
	}
	if ((tflags & TI_TAO) != 0) {
		dsp->ds_wrmode = WM_TAO;
		return;
	}
	if ((wmode & F_SAO) != 0) {
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == 0) {
			dsp->ds_wrmode = WM_SAO;
			return;
		}
		if ((tflags & TI_RAW16) != 0) {		/* Is this needed? */
			dsp->ds_wrmode = WM_SAO_RAW16;
			return;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == TI_RAW) {
			dsp->ds_wrmode = WM_SAO_RAW96P;
			return;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == (TI_RAW|TI_RAW96R)) {
			dsp->ds_wrmode = WM_SAO_RAW96R;
			return;
		}
	}
	if ((wmode & F_RAW) != 0) {
		if ((tflags & TI_RAW16) != 0) {
			dsp->ds_wrmode = WM_RAW_RAW16;
			return;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == TI_RAW) {
			dsp->ds_wrmode = WM_RAW_RAW96P;
			return;
		}
		if ((tflags & (TI_RAW|TI_RAW16|TI_RAW96R)) == (TI_RAW|TI_RAW96R)) {
			dsp->ds_wrmode = WM_RAW_RAW96R;
			return;
		}
	}
	dsp->ds_wrmode = WM_NONE;
}
//--------------------------------------------------------------------
long	checktsize(__s32	tracks,	track_t	*trackp)
{
	__s32	i;
	__u64	curr;
	__u64	total = -150;	/* CD track #1 pregap compensation */
	__u64	btotal;
	track_t	*tp;

	if (trackp->flags & TI_NOCD)
		total = 0;
	for (i = 1; i <= tracks; i++) {
		tp = &trackp[i];
		if (!is_pregap(tp))
			total += tp->pregapsize;

		printf("track: %d start: %lld pregap: %ld\n",	i, total, tp->pregapsize);
		tp->trackstart = total;
		if (tp->itracksize >= 0) {
			curr = (tp->itracksize + (tp->isecsize-1)) / tp->isecsize;
			curr += tp->padsecs;

			if (!is_shorttrk(tp) && curr < 300)
				curr = 300;
			if ((trackp->flags & TI_NOCD) == 0) {

				if (is_tao(tp) && !is_audio(tp)) {
					curr += 2;
				}
			}
			total += curr;
		} else if (is_sao(tp) || is_raw(tp)) {
			//
		}
	}
	tp = &trackp[i];
	tp->trackstart = total;
	tp->tracksecs = 6750;		/* Size of first session Lead-Out */
	/*
	if (!lverbose)
		return (total);

	if (trackp->flags & TI_NOCD)
		btotal = (__u64)total * 2048;
	else
		btotal = (__u64)total * 2352;

	if (tracks > 0) {
		if (trackp->flags & TI_NOCD) {
			printf("Total size:     %4llu MB = %lld sectors\n",
				btotal >> 20, total);
		} else {
			printf("Total size:     %4llu MB (%02d:%02d.%02d) = %lld sectors\n",
				btotal >> 20,
				minutes(btotal),
				seconds(btotal),
				hseconds(btotal), total);
			btotal += 150 * 2352;
			printf("Lout start:     %4llu MB (%02d:%02d/%02d) = %lld sectors\n",
				btotal >> 20,
				minutes(btotal),
				seconds(btotal),
				frames(btotal), total);
		}
	}
	*/
	return (total);
}
//--------------------------------------------------------------------
void	set_secsize(St_GrScsiIf *scgp,	__s32	secsize)
{
	if (secsize > 0)
	{
		select_secsize(scgp, secsize);
	}
}
//--------------------------------------------------------------------
void	check_recovery(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags)
{
	if ((*dp->cdr_check_recovery)(scgp, dp))
	{
		printf("Recovery needed.\n");
		unload_media(scgp, dp, flags);
	}
}
//--------------------------------------------------------------------
BOOL8	checkdsize(St_GrScsiIf *scgp, cdr_t *dp, long tsize, __u32 flags)
{
	long	startsec = 0L;
	long	endsec = 0L;
	dstat_t	*dsp = dp->cdr_dstat;

	(*dp->cdr_next_wr_address)(scgp, (track_t *)0, &startsec);

	if (startsec < 0)
		startsec = 0;

	endsec = startsec + tsize;
	dsp->ds_startsec = startsec;
	dsp->ds_endsec = endsec;

	if (dsp->ds_maxblocks > 0)
	{
		/*
		if (lverbose)
			printf("Blocks total: %ld Blocks current: %ld Blocks remaining: %ld\n",	(long)dsp->ds_maxblocks,
				(long)dsp->ds_maxblocks - startsec,	(long)dsp->ds_maxblocks - endsec);
		*/

		if (endsec > dsp->ds_maxblocks)
		{
			if (dsp->ds_flags & DSF_NOCD)
			{
				printf("Data does not fit on current disk.\n");
				goto toolarge;
			}
			printf("WARNING: Data may not fit on current disk.\n");
		}
		/*
		if (lverbose && dsp->ds_maxrblocks > 0)
			printf("RBlocks total: %ld RBlocks current: %ld RBlocks remaining: %ld\n",
					(long)dsp->ds_maxrblocks,	(long)dsp->ds_maxrblocks - startsec,
					(long)dsp->ds_maxrblocks - endsec);
		*/
		if (dsp->ds_maxrblocks > 0 && endsec > dsp->ds_maxrblocks)
		{
			printf("Data does not fit on current disk.\n");
			goto toolarge;
		}
		if ((endsec > dsp->ds_maxblocks && endsec > 404700) ||
				(dsp->ds_maxrblocks > 404700 && 449850 > dsp->ds_maxrblocks))
		{
			if (dsp->ds_maxrblocks > 404700)
			{
				printf("RedBook total: %ld RedBook current: %ld RedBook remaining: %ld\n",
					404700L, 404700L - startsec, 404700L - endsec);
			}
			if (endsec > dsp->ds_maxblocks && endsec > 404700)
			{
				if ((flags & (F_IGNSIZE|F_FORCE)) == 0)
				{
					printf("Notice: Most recorders cannot write CD's >= 90 minutes.\n");
					printf("Notice: Use -ignsize option to allow >= 90 minutes.\n");
				}
				goto toolarge;
			}
		}
	}
	else
	{
		printf("Disk capacity is unknown.\n");

		if (endsec >= (405000-300))
		{
			printf("Data will not fit on any CD.\n");
			goto toolarge;
		}
		else if (endsec >= (333000-150))
		{
			printf("WARNING: Data may not fit on standard 74min CD.\n");
		}
	}
	if (dsp->ds_maxblocks <= 0 || endsec <= dsp->ds_maxblocks)
		return (TRUE);

toolarge:
	if (dsp->ds_maxblocks > 0 && endsec > dsp->ds_maxblocks)
	{
		if ((flags & (F_OVERBURN|F_IGNSIZE|F_FORCE)) != 0)
		{
			if (dsp->ds_flags & DSF_NOCD)
			{
				printf("Notice: -overburn is not expected to work with DVD/BD media.\n");
			}
			printf("Notice: Overburning active. Trying to write more than the official disk capacity.\n");
			return (TRUE);
		}
		else
		{
			if ((dsp->ds_flags & DSF_NOCD) == 0)
			{
				printf("Notice: Use -overburn option to write more than the official disk capacity.\n");
				printf("Notice: Most CD-writers do overburning only on SAO or RAW mode.\n");
			}
			return (FALSE);
		}
	}
	if (dsp->ds_maxblocks < 449850)
	{
		if (dsp->ds_flags & DSF_NOCD)
		{
			if (endsec <= dsp->ds_maxblocks)
				return (TRUE);
			if (dsp->ds_maxblocks <= 0)
			{
				printf("DVD/BD capacity is unknown.\n");
				if ((flags & (F_IGNSIZE|F_FORCE)) != 0)
				{
					printf("Notice: -ignsize active.\n");
					return (TRUE);
				}
			}
			printf("Cannot write more than remaining DVD/BD capacity.\n");
			return (FALSE);
		}

		if (endsec > 449700)
		{
			printf("Cannot write CD's >= 100 minutes.\n");
			return (FALSE);
		}
	}
	if ((flags & (F_IGNSIZE|F_FORCE)) != 0)
		return (TRUE);
	return (FALSE);
}
//--------------------------------------------------------------------
/*
void	excdr(__s32	excode,	void *arg)
{
	struct exargs	*exp = (struct exargs *)arg;

	exscsi(excode, arg);

	cdrstats(exp->dp);
	if ((exp->dp->cdr_dstat->ds_cdrflags & RF_DID_CDRSTAT) == 0)
	{
		exp->dp->cdr_dstat->ds_cdrflags |= RF_DID_CDRSTAT;
		if (exp->scgp && exp->scgp->running == 0)
			(*exp->dp->cdr_stats)(exp->scgp, exp->dp);
	}

	if (exp->scgp)
		scg_close(exp->scgp);
	exp->scgp = NULL;

}
*/
//--------------------------------------------------------------------
void		exscsi(__s32 excode, void *arg)
{
	struct exargs	*exp = (struct exargs *)arg;

	if (exp != NULL && exp->exflags == 0)
	{
		if (exp->scgp == NULL)
		{
			return;
		}
		if (exp->scgp->running)
		{
			return;
		}

		scsi_flush_cache(exp->scgp, FALSE);
		(*exp->dp->cdr_abort_session)(exp->scgp, exp->dp);
		set_secsize(exp->scgp, exp->old_secsize);
		unload_media(exp->scgp, exp->dp, exp->flags);

		exp->exflags++;	/* Make sure that it only get called once */
	}
}
//--------------------------------------------------------------------
void		cdrstats(cdr_t *dp)
{
	float	secsps = 75.0;
	__s32	nsecs;
	float	fspeed;
	struct timeval	tcur;
	struct timeval	tlast;
	BOOL8	nostop = FALSE;

	if (starttime.tv_sec == 0)
		return;

	if (stoptime.tv_sec == 0) {
		gettimeofday(&stoptime, (struct timezone *)0);
		nostop = TRUE;
	}

	if ((dp->cdr_dstat->ds_cdrflags & RF_DID_STAT) != 0)
		return;
	dp->cdr_dstat->ds_cdrflags |= RF_DID_STAT;

}
//--------------------------------------------------------------------
__s32		scg_close(St_GrScsiIf * scgp)
{
	scg__close(scgp);
	scg_sfree(scgp);
	return (0);
}
//--------------------------------------------------------------------
__s32		scg__close(St_GrScsiIf *scgp)
{
	return (SCGO_CLOSE(scgp));
}
//--------------------------------------------------------------------
BOOL8	do_opc(St_GrScsiIf	*scgp, cdr_t *dp,	__u32 flags)
{
	if ((flags & F_DUMMY) == 0 && dp->cdr_opc)
	{
		printf("Performing OPC...\n");
		if (dp->cdr_opc(scgp, NULL, 0, TRUE) < 0)
		{
			printf("OPC failed.\n");
			if ((flags & F_FORCE) == 0)
				return (FALSE);
		}
	}
	return (TRUE);
}
//--------------------------------------------------------------------
__s32		fill_buf(__s32 f,	track_t	*trackp, long	secno, __s8	*bp, __s32 size)
{
	__s32	amount = 0;
	__s32	nsecs;
	__s32	rsize;
	__s32	rmod;
	__s32	readoffset = 0;

	nsecs = size / trackp->secsize;
	if (nsecs < trackp->secspt)
	{
		memset(bp, '\0', trackp->secspt * trackp->secsize);
	}

	if (!is_raw(trackp))
	{
		amount = read_buf(f, bp, size);
		if (amount != size)
		{
			if (amount < 0)
				return (amount);

			memset(&bp[amount], '\0', size-amount);
		}
		if (is_swab(trackp))
			swabbytes(bp, amount);
		return (amount);
	}

	rsize = nsecs * trackp->isecsize;
	rmod  = size % trackp->secsize;
	if (rmod > 0)
	{
		rsize += rmod;
		nsecs++;
	}

	readoffset = trackp->dataoff;
	amount = read_buf(f, bp + readoffset, rsize);
	if (is_swab(trackp))
		swabbytes(bp + readoffset, amount);

	if (trackp->isecsize == 2448 && trackp->secsize == 2368)
		subrecodesecs(trackp, (unsigned __s8 *)bp, secno, nsecs);

	scatter_secs(trackp, bp + readoffset, nsecs);

	if (amount != rsize)
	{
		if (amount < 0)
			return (amount);

		memset(&bp[amount], '\0', rsize-amount);
		nsecs = amount / trackp->isecsize;
		rmod  = amount % trackp->isecsize;
		amount = nsecs * trackp->secsize;
		if (rmod > 0)
		{
			nsecs++;
			amount += rmod;
		}
	}
	else
	{
		amount = size;
	}
	if ((trackp->sectype & ST_MODE_RAW) == 0)
	{
		encsectors(trackp, (unsigned __s8 *)bp, secno, nsecs);
		fillsubch(trackp, (unsigned __s8 *)bp, secno, nsecs);
	}
	else
	{
		scrsectors(trackp, (unsigned __s8 *)bp, secno, nsecs);

		if (trackp->isecsize == 2352)
			fillsubch(trackp, (unsigned __s8 *)bp, secno, nsecs);
	}
	return (amount);
}
//--------------------------------------------------------------------
__s32		read_buf(__s32 f,	__s8 *bp,	__s32	size)
{
	__s8	*p = bp;
	__s32	amount = 0;
	__s32	n;

	do {
		do {
			n = read(f, p, size-amount);
		} while (n < 0 && (errno == EAGAIN || errno == EINTR));
		if (n < 0)
			return (n);
		amount += n;
		p += n;

	} while (amount < size && n > 0);
	return (amount);
}
//--------------------------------------------------------------------
void	raisepri(__s32 pri)
{
	if (rt_raisepri(pri) >= 0)
		return;
#ifdef LINUX_APP
	if (setpriority(PRIO_PROCESS, getpid(), -20 + pri) < 0)
	{
		printf("WARNING: Cannot set priority using setpriority().\n");
		printf("WARNING: This causes a high risk for buffer underruns.\n");
	}
#endif
}
//--------------------------------------------------------------------
__s32		rt_raisepri(__s32 pri)
{
	struct sched_param scp;

	if (sysconf(_SC_PRIORITY_SCHEDULING) == -1)
	{
		printf("WARNING: RR-scheduler not available, disabling.\n");
		return (-1);
	}

	memset(&scp, '\0', sizeof (scp));
	scp.sched_priority = sched_get_priority_max(SCHED_RR) - pri;
	if (sched_setscheduler(0, SCHED_RR, &scp) < 0)
	{
		printf("WARNING: Cannot set RR-scheduler.\n");
		return (-1);
	}
	return (0);
}
//--------------------------------------------------------------------
__s32		scg_vtail(St_GrScsiIf *scgp)
{
		__s32	ret;
	
		//scgp->errptr += scg_svtail(scgp, &ret, scgp->errptr, scg_errrsize(scgp));
		scg_svtail(scgp, &ret, scgp->errptr, scg_errrsize(scgp));
		return (ret);
}
//--------------------------------------------------------------------
__s32		scg_svtail(St_GrScsiIf	*scgp, __s32 *retp,	__s8 *buf, __s32 maxcnt)
{
	__s8	*p = buf;
	__s32		amt;
	__s32		ret;

	ret = scg_cmd_err(scgp) ? -1 : 0;
	if (retp)
		*retp = ret;
	return 0;
}
//--------------------------------------------------------------------
/*
__s32		scg_errflush(St_GrScsiIf	*scgp)
{
	if (scgp->errfile == NULL)
		return (0);

	return (scg_errfflush(scgp, (FILE *)scgp->errfile));
}
*/
//--------------------------------------------------------------------
__s32		scg_errfflush(St_GrScsiIf *scgp, FILE *f)
{
	__s32	cnt;

	cnt = scgp->errptr - scgp->errbeg;
	if (cnt > 0) {
		//filewrite(f, scgp->errbeg, cnt);
		fflush(f);
		scgp->errbeg = scgp->errptr;
	}
	return (cnt);
}
//--------------------------------------------------------------------
__s32		scg_scandev(__s8 *devp,	__s32	*busp, __s32 *tgtp,	__s32	*lunp)
{
	__s32	x1, x2, x3;
	__s32	n = 0;
	__s8	*p = devp;

	x1 = x2 = x3 = 0;
	*busp = *tgtp = *lunp = 0;

	if (*p != '\0') {
		p = astoi(p, &x1);
		if (*p == ',') {
			p++;
			n++;
		} else {
			printf("Invalid bus or target specifier in '%s'", devp);
			return (-1);
		}
	}
	if (*p != '\0') {
		p = astoi(p, &x2);
		if (*p == ',' || *p == '\0') {
			if (*p != '\0')
				p++;
			n++;
		} else {
			printf("Invalid target or lun specifier in '%s'", devp);
			return (-1);
		}
	}
	if (*p != '\0') {
		p = astoi(p, &x3);
		if (*p == '\0') {
			n++;
		} else {
			printf("Invalid lun specifier in '%s'", devp);
			return (-1);
		}
	}
	if (n == 3) {
		*busp = x1;
		*tgtp = x2;
		*lunp = x3;
	}
	if (n == 2) {
		*tgtp = x1;
		*lunp = x2;
	}
	if (n == 1) {
		*tgtp = x1;
	}

	if (x1 < 0 || x2 < 0 || x3 < 0) {
		printf("Invalid value for bus, target or lun (%d,%d,%d)",	*busp, *tgtp, *lunp);
		return (-1);
	}
	return (n);
}
//--------------------------------------------------------------------

