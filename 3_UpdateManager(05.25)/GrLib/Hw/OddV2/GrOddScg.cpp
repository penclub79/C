/*
	odd scsi class
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddDevBase.h>

#include <GrDebug.h>
#include <errno.h>
#include <GrDumyTool.h>

#ifdef LINUX_APP
	#include <stdio.h>
	#include <sys/time.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <sys/stat.h>
	#include <scsi/sg.h>
	#include <scsi/scsi.h>
	#include <signal.h>
	#include <stdlib.h>
	#include <sys/resource.h>
	#include <sched.h>
	#include <sys/sysmacros.h>
#endif

//====================================================================
//local const
#define	dma_read	flags		/* 1 if DMA to Sun, 0 otherwise */

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

#define	Mac_GrOddMin(a, b)	((a) < (b) ? (a):(b))
#define	Mac_GrOddMax(a, b)	((a) < (b) ? (b):(a))

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function
int		_select_target(Cls_GrOddScg	*scgp, FILE	*f, int	type, int	idx);
int		_read_buffer(Cls_GrOddScg	*scgp, int size);

//====================================================================
//var
extern struct timeval	starttime;
extern struct timeval	wstarttime;
extern struct timeval	stoptime;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddScg::Cls_GrOddScg(int A_Verbose)
{
		//init
		GrDumyZeroMem(&m_Scmd, sizeof(struct scg_cmd));
		GrDumyZeroMem(&m_Local, sizeof(St_GrOddScgLocal));
		GrDumyZeroMem(&m_CmdStart, sizeof(struct timeval));
		GrDumyZeroMem(&m_CmdStop, sizeof(struct timeval));
		GrDumyZeroMem(m_ErrStr, SCSI_ERRSTR_SIZE);
		GrDumyZeroMem(&inq, sizeof(struct scsi_inquiry));
		GrDumyZeroMem(&cap, sizeof(struct scsi_capacity));
		m_PtrBufBase		= NULL;
		m_PtrBuf				= NULL;
		m_PtrErrBegin		= NULL;
		m_PtrErr				= NULL;
		m_Dev						= 0;
		m_MaxDma				= 0;
		m_MaxBuf				= 0;

		m_Fd			= -1;
		m_TimeOut	= 20;
		m_IsRun		= FALSE;
		m_PtrErrBegin = m_ErrStr;
		m_PtrErr			= m_ErrStr;
		m_Addr.scsibus = -2;
		m_Addr.target	 = -2;
		m_Addr.lun		 = -2;
		m_Overbose		= A_Verbose;
}
//--------------------------------------------------------------------
Cls_GrOddScg::~Cls_GrOddScg()
{
		LcScg0_Close();
		LcScg0_FreeBuf();
}
//--------------------------------------------------------------------
int	Cls_GrOddScg::SetTarget(int A_Bus, int A_Target, int A_Lun)
{
	// local -------------------
		int			Tv_Result;
	// code --------------------
		Tv_Result		= -1;

		Tv_Result		= LcScg0_Fileno(A_Bus, A_Target, A_Lun);
		m_Fd				= Tv_Result;
		m_Addr.scsibus = A_Bus;
		m_Addr.target	 = A_Target;
		m_Addr.lun		 = A_Lun;

		return Tv_Result;
}
//--------------------------------------------------------------------
BOOL Cls_GrOddScg::Init(char* A_PtrDev)
{
	// local -------------------
		BOOL			Tv_Result;
	// code --------------------
		Tv_Result		= FALSE;

		Tv_Result = LcScgOpen(A_PtrDev);
		if(0 > Tv_Result)
		{
			return Tv_Result;
		}
		SetTarget(m_Addr.scsibus, m_Addr.target, m_Addr.lun);

		return Tv_Result;
}
//--------------------------------------------------------------------
int	Cls_GrOddScg::LcScgOpen(char *device)
{
	// var --------------------
		int	busno;
		int	tgt;
		int	tlun;
		int	f;
		int	i;
		int	b;
		int	t;
		int	l;
		int	nopen = 0;
		int	xopen = 0;
		char		devname[64];
		BOOL	use_ata = FALSE;
	// code -------------------
		busno		= scg_scsibus(this);
		tgt			= scg_target(this);
		tlun		= scg_lun(this);

		if ((busno >= MAX_SCG && busno < 1000) || busno >= (1000+MAX_ATA) || tgt >= MAX_TGT || tlun >= MAX_LUN)
		{
				errno = EINVAL;
				return (-1);
		}
		if (busno >= 1000)
		{
			busno = busno - 1000 + MAX_SCG;
		}

		m_Local.scgfile	= -1;
		m_Local.pgbus = -2;
		m_Local.SCSIbuf = (char *)-1;
		m_Local.pack_id = 5;
		m_Local.drvers = -1;
		m_Local.isold = -1;
		m_Local.xbufsize = 0L;
		m_Local.xbuf = NULL;

		for (b = 0; b < MAX_SCG+MAX_ATA; b++)
		{
			m_Local.buscookies[b] = (short)-1;
			for (t = 0; t < MAX_TGT; t++)
			{
				for (l = 0; l < MAX_LUN; l++)
					m_Local.scgfiles[b][t][l] = (short)-1;
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
				int	iparm;
	
				if (ioctl(f, SG_GET_TIMEOUT, &iparm) < 0)
				{
#ifdef LINUX_APP
					close(f);
#endif
					continue;
				}
				sg_clearnblock(f);

				if (sg_setup(f, busno, tgt, tlun, i))
				{
					return (++nopen);
				}
				if (busno < 0 && tgt < 0 && tlun < 0)
					nopen++;
			}
		}
		if (nopen > 0 && m_ErrStr)
			m_ErrStr[0] = '\0';
	
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

					if (sg_setup(f, busno, tgt, tlun, -1))
						return (++nopen);
					if (busno < 0 && tgt < 0 && tlun < 0)
						nopen++;
				}
			}
		}
		if (nopen > 0 && m_ErrStr)
			m_ErrStr[0] = '\0';
	
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

				if (sg_setup(f, busno, tgt, tlun, -1))
					return (++nopen);
				if (busno < 0 && tgt < 0 && tlun < 0)
					nopen++;
			}
		}
		if (nopen > 0 && m_ErrStr)
			m_ErrStr[0] = '\0';
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
		if (!sg_mapdev(f, &busno, &tgt, &tlun, 0, 0, b)) 
		{
#ifdef LINUX_APP
			close(f);
#endif
			goto openpg;
		}

		SetTarget(busno, tgt, tlun);

		if (sg_setup(f, busno, tgt, tlun, b))
		{
			return (++nopen);
		}
	}
openpg:
	/*nopen += pg_open(scgp, device);

	if (scgp->debug > 0) for (b = 0; b < MAX_SCG+MAX_ATA; b++) {
		for (t = 0; t < MAX_TGT; t++) {
			for (l = 0; l < MAX_LUN; l++) {
				if (scglocal(scgp).scgfiles[b][t][l] != (short)-1) {
					printf("error file %d, %d, %d\n", b, t, l);
				}
			}
		}
	}*/
	return (nopen);
}
//--------------------------------------------------------------------
void	Cls_GrOddScg::sg_clearnblock(int f)
{
	int	n;

	n = fcntl(f, F_GETFL);
	n &= ~O_NONBLOCK;
	fcntl(f, F_SETFL, n);
}
//--------------------------------------------------------------------
BOOL 	Cls_GrOddScg::sg_setup(int f, int	busno, int tgt, int	tlun, int	ataidx)
{
	// var --------------------
		int	n;
		int	Chan;
		int	Ino;
		int	Bus;
		int	Target;
		int	Lun;
		BOOL	onetarget = FALSE;
	// code -------------------
		if (m_Local.drvers < 0)
		{
			m_Local.drvers = 0;
			if (ioctl(f, SG_GET_VERSION_NUM, &n) >= 0)
			{
				m_Local.drvers = n;
			}
		}

		if (m_Addr.scsibus >= 0 && m_Addr.target >= 0 && m_Addr.lun >= 0)
			onetarget = TRUE;

		sg_mapdev(f, &Bus, &Target, &Lun, &Chan, &Ino, ataidx);

		Ino |= Chan << 8;
		n = sg_mapbus(Bus, Ino);
		if (Bus == -1)
		{
			Bus = n;
		}

		if (Bus < 0 || Bus >= (MAX_SCG+MAX_ATA) || Target < 0 ||
			Target >= MAX_TGT || Lun < 0 || Lun >= MAX_LUN)
		{
			return (FALSE);
		}

		if (m_Local.scgfiles[Bus][Target][Lun] == (short)-1)
		{
			m_Local.scgfiles[Bus][Target][Lun] = (short)f;
		}

		if (onetarget)
		{
			if (Bus == busno && Target == tgt && Lun == tlun)
			{
				sg_initdev(f);
				m_Local.scgfile = f;
				return (TRUE);
			}
			else
			{
				m_Local.scgfiles[Bus][Target][Lun] = (short)-1;
	#ifdef LINUX_APP
				close(f);
	#endif
			}
		}
		else
		{
			sg_initdev(f);
		}

		return (FALSE);
}
//--------------------------------------------------------------------
int Cls_GrOddScg::sg_mapbus(int	busno, int ino)
{
	// var --------------------
		int	i;
	// code -------------------
		if (busno >= 0 && busno < (MAX_SCG+MAX_ATA))
		{
			if (m_Local.buscookies[busno] == (short)-1)
			{
				m_Local.buscookies[busno] = ino;
				return (busno);
			}
			return (busno);
		}
		else
		{
			for (i = 0; i < (MAX_SCG+MAX_ATA); i++)
			{
				if (m_Local.buscookies[i] == (short)-1)
				{
					m_Local.buscookies[i] = ino;
					return (i);
				}

				if (m_Local.buscookies[i] == ino)
					return (i);
			}
		}

		return (0);
}
//--------------------------------------------------------------------
void	Cls_GrOddScg::sg_initdev(int f)
{
	// var --------------------
		struct sg_rep
		{
			struct sg_header	hd;
			unsigned char		rbuf[100];
		} sg_rep;
		int	n;
		int	i;
#ifdef LINUX_APP
		struct stat sb;
#endif
		int	ret;
	// code -------------------
		 sg_settimeout(f, m_TimeOut);
	
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
void Cls_GrOddScg::sg_settimeout(int f,	int	tmo)
{
	tmo *= 100;
	if (tmo)
		tmo += 100/2;

	if (ioctl(f, SG_SET_TIMEOUT, &tmo) < 0)
		printf("Cannot set SG_SET_TIMEOUT.\n");
}
//--------------------------------------------------------------------
BOOL	Cls_GrOddScg::sg_mapdev(int	f, int *busp, int	*tgtp, int *lunp, int	*chanp, int	*inop, int ataidx)
{
	// var --------------------
		struct	sg_id
		{
			long	l1; //target | lun << 8 | channel << 16 | low_ino << 24
			long	l2; //Unique id
		} sg_id;
		int	Chan;
		int	Ino;
		int	Bus;
		int	Target;
		int	Lun;	
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
void	Cls_GrOddScg::TimeOut(int A_TimeOut)
{
		m_TimeOut		= A_TimeOut;
}
//--------------------------------------------------------------------
long Cls_GrOddScg::BufSize(long amt)
{
	// var --------------------
		long	maxdma;
	// code -------------------
		maxdma = LcScg0_MaxDma(amt);

		if (amt <= 0 || amt > maxdma)
			amt = maxdma;
	
		m_MaxDma = maxdma;	/* Max possible  */
		m_MaxBuf = amt;	/* Current value */
	
		return (amt);
}
//--------------------------------------------------------------------
long	Cls_GrOddScg::scg_bufsize(long amt)
{
	// var --------------------
		long	maxdma;
	// code -------------------
		maxdma = LcScg0_MaxDma(amt);

		if (amt <= 0 || amt > maxdma)
			amt = maxdma;
	
		m_MaxDma		= maxdma;	/* Max possible  */
		m_MaxBuf		= amt;		/* Current value */
	
		return (amt);
}
//--------------------------------------------------------------------
void*	Cls_GrOddScg::scg_getbuf(long	amt)
{
	// var --------------------
		void	*buf;
	// code -------------------
		if (amt <= 0 || amt > scg_bufsize(amt))
			return ((void *)0);

		buf = LcScg0_GetBuf(amt);
		m_PtrBuf = buf;
		return (buf);
}
//--------------------------------------------------------------------
long	Cls_GrOddScg::LcScg0_MaxDma(long amt)
{
	long maxdma = MAX_DMA_LINUX;
	int 	test;

	if (m_Local.drvers >= 20134)
		maxdma = LcSg_RaiseDma(amt);

	if (maxdma < 0) {
		if (m_Local.pgbus == 0)
			return PG_MAX_DATA;
		if (m_Local.scgfile >= 0)
			maxdma = MAX_DMA_LINUX;
	}

	if (m_Addr.scsibus == m_Local.pgbus)
		return (PG_MAX_DATA);

	test = m_Addr.scsibus;
	//if ((scg_scsibus(scgp) < 0) && (PG_MAX_DATA < maxdma))
	if ((test < 0) && (PG_MAX_DATA < maxdma))
		return (PG_MAX_DATA);
	
	return (maxdma);
}
//--------------------------------------------------------------------
long	Cls_GrOddScg::LcSg_RaiseDma(long newmax)
{
	int	b;
	int	t;
	int	l;
	int	f;
	int	val;
	int	old;

	val = 126*1024;

	if (val > MAX_DMA_LINUX) {
		for (b = 0; b < (MAX_SCG+MAX_ATA); b++) {
			for (t = 0; t < MAX_TGT; t++) {
				for (l = 0; l < MAX_LUN; l++) {
					if ((f = LcScg0_Fileno(b, t, l)) < 0)
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
					if ((f = LcScg0_Fileno(b, t, l)) < 0)
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

				if ((f = LcScg0_Fileno(b, t, l)) < 0)
					continue;
				if (ioctl(f, SG_GET_RESERVED_SIZE, &val) < 0)
					continue;

				LcSg_CheckDma(f, &val);	/* This does not work */

				if (val < newmax)
					newmax = val;
			}
		}
	}

	return ((long)newmax);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::LcScg0_Fileno(int busno, int tgt, int tlun)
{
	if (busno >= 1000)
		busno = busno - 1000 + MAX_SCG;

	if (busno < 0 || busno >= (MAX_SCG+MAX_ATA) ||
		tgt < 0 || tgt >= MAX_TGT ||
		tlun < 0 || tlun >= MAX_LUN)
	{
		return (-1);
	}

	return ((int)m_Local.scgfiles[busno][tgt][tlun]);
}
//--------------------------------------------------------------------
void	Cls_GrOddScg::LcSg_CheckDma(int f, int* valp)
{
#ifdef LINUX_APP
	struct stat	sb;
#endif
	int		val;
	char		nbuf[80];
	int		fx;

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
void* Cls_GrOddScg::LcScg0_GetBuf(long amt)
{
	char	*ret;

	ret = (char*)valloc((size_t)(amt+getpagesize()));
	if (ret == NULL)
		return (ret);
	m_PtrBufBase = ret;
	ret += getpagesize();
	m_Local.SCSIbuf = ret;

	return ((void *)ret);
}
//--------------------------------------------------------------------
BOOL Cls_GrOddScg::Scg_HaveBus(int busno)
{
	int	t;
	int	l;

	if (busno >= 1000) {
		if (busno >= (1000+MAX_ATA))
			return (FALSE);
		busno = busno - 1000 + MAX_SCG;
	} else if (busno < 0 || busno >= MAX_SCG)
		return (FALSE);

	for (t = 0; t < MAX_TGT; t++) {
		for (l = 0; l < MAX_LUN; l++)
			if (m_Local.scgfiles[busno][t][l] >= 0)
				return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::Scg_Cmd()
{
		int		ret;
		struct	scg_cmd	*scmd = &m_Scmd;
		
#ifndef LINUX_APP
		return	0;
#endif
		ret = 0;
		m_PtrErr	= m_PtrErrBegin = m_ErrStr;
	
		if (scmd->timeout == 0 || scmd->timeout < m_TimeOut)
			scmd->timeout = m_TimeOut;
	
		scmd->u_sense.cmd_sense[0] = 0;		/* Paranioa */
		if (scmd->sense_len > SCG_MAX_SENSE)
			scmd->sense_len = SCG_MAX_SENSE;
		else if (scmd->sense_len < 0)
			scmd->sense_len = 0;
		
		if(m_IsRun)
		{
			if(m_CurCmdName)
			{
				DbgMsgPrint("Cls_GrOddScg::Scg_Cmd Currently running '%s' command.\n", m_CurCmdName);
			}
			raisecond("SCSI ALREADY RUNNING !!", 0L);
		}

		//scgp->cb_fun = NULL;
		gettimeofday( &m_CmdStart, NULL );
		m_CurCmdName = m_CmdName;
		m_IsRun = TRUE;
		ret = LcScg0_Send();

		m_IsRun = FALSE;
		LcScg_Times();

		/*if(m_Flags & SCGF_IGN_RESID)
		{
			scmd->resid	= 0;
		}*/
		if (ret < 0)
		{
			if (m_Addr.target < 8 || errno != EINVAL) 
			{
				int	err = errno;
	
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

		ret = LcScg_Vtail();

		return (ret);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::LcScg0_Send()
{
	struct scg_cmd	*sp = &m_Scmd;
	int		ret = 0;
	sg_io_hdr_t	sg_io;
	struct timeval	to;
	static	uid_t		cureuid = 0;

	if (m_Fd < 0) {
		sp->error = SCG_FATAL;
		sp->ux_errno = EIO;
		return (0);
	}
	if (m_Local.isold > 0) {
		return (LcSg_RwSend());
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

	ret = ioctl(m_Fd, SG_IO, &sg_io);

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
			m_Local.isold < 0) 
		{
			m_Local.isold = 1;

			return (LcSg_RwSend());
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
		LcScg_Times();

		if (sp->timeout > 1 && m_CmdStop.tv_sec == 0) {
			sp->u_scb.cmd_scb[0] = 0;
			sp->error = SCG_FATAL;	/* a selection timeout */
		} else {
			sp->error = SCG_TIMEOUT;
		}
		break;

	default:
		to.tv_sec = sp->timeout;
		to.tv_usec = 500000;
		LcScg_Times();

		if (m_CmdStop.tv_sec < to.tv_sec || (m_CmdStop.tv_sec == to.tv_sec &&
			m_CmdStop.tv_usec < to.tv_usec))
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
void		Cls_GrOddScg::LcScg_Times()
{
	struct timeval	*stp = &m_CmdStop;

	gettimeofday(stp, (struct timezone *)0);
	stp->tv_sec -= m_CmdStart.tv_sec;
	stp->tv_usec -= m_CmdStart.tv_usec;
	while (stp->tv_usec < 0) 
	{
		stp->tv_sec -= 1;
		stp->tv_usec += 1000000;
	}
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::LcSg_RwSend()
{
	int		f = m_Fd;
	struct scg_cmd	*sp = &m_Scmd;
	struct sg_rq	*sgp;
	struct sg_rq	*sgp2;
	int	i;
	int	pack_len;
	int	reply_len;
	int	amt = sp->cdb_len;
	struct sg_rq {
		struct sg_header	hd;
		unsigned char		buf[MAX_DMA_LINUX+SCG_MAX_CMD];
	} sg_rq;

	if (f < 0) {
		sp->error = SCG_FATAL;
		sp->ux_errno = EIO;
		return (0);
	}

	if (sp->timeout != m_TimeOut)
		sg_settimeout(f, sp->timeout);


	sgp2 = sgp = &sg_rq;
	if (sp->addr == m_Local.SCSIbuf) {
		sgp = (struct sg_rq *)
			(m_Local.SCSIbuf - (sizeof (struct sg_header) + amt));
		sgp2 = (struct sg_rq *)
			(m_Local.SCSIbuf - (sizeof (struct sg_header)));
	} else {
		if (sp->size > (int)(sizeof (sg_rq.buf) - SCG_MAX_CMD)) {

			if (m_Local.xbuf == NULL) {
				m_Local.xbufsize = m_MaxBuf;
				m_Local.xbuf = (char*)
					malloc(m_Local.xbufsize +
					SCG_MAX_CMD +
					sizeof (struct sg_header));
			}
			if (m_Local.xbuf == NULL || sp->size > m_Local.xbufsize)
			{
					errno = ENOMEM;
					return (-1);
			}
			sgp2 = sgp = (struct sg_rq *)m_Local.xbuf;
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
	sgp->hd.pack_id = m_Local.pack_id++;

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
		sg_settimeout(f, m_TimeOut);
		printf( "sg_rwsend - write error\n" );
		return (-1);
	} else if (amt != i) 
	{

	}

	if (sp->addr == m_Local.SCSIbuf) 
	{
		//movebytes(sgp, sgp2, sizeof (struct sg_header));
		GrDumyCopyMem( sgp2, sgp, sizeof (struct sg_header));
		sgp = sgp2;
	}
	sgp->hd.sense_buffer[0] = 0;
	if ((amt = read(f, sgp, reply_len)) < 0) 
	{		/* read */
		sg_settimeout(f, m_TimeOut);
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
			LcScg_Times();

			if (sp->timeout > 1 && m_CmdStop.tv_sec == 0) {
				sp->u_scb.cmd_scb[0] = 0;
				sp->ux_errno = EIO;
				sp->error = SCG_FATAL;	/* a selection timeout */
			} else if (m_CmdStop.tv_sec < to.tv_sec ||
				(m_CmdStop.tv_sec == to.tv_sec &&
				m_CmdStop.tv_usec < to.tv_usec)) {

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

	if (sp->timeout != m_TimeOut)
		sg_settimeout(f, m_TimeOut);
	return (0);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::LcScg_Vtail()
{
	int		ret;

	ret = LcScg_Cmd_Err() ? -1 : 0;

	return ret;
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::Scg_Sense_Key()
{
	struct scg_cmd *cp = &m_Scmd;
	int	key = -1;

	if (!LcScg_Cmd_Err())
		return (0);

	if (cp->sense.code >= 0x70)
		key = ((struct scsi_ext_sense *)&(cp->sense))->key;
	return (key);

}
//--------------------------------------------------------------------
int		Cls_GrOddScg::Scg_Cmd_Status()
{
	struct scg_cmd	*cp = &m_Scmd;
	int		cmdstatus = *(unsigned char*)&cp->scb;

	return (cmdstatus);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::Scg_Sense_Code()
{
	struct scg_cmd *cp = &m_Scmd;
	int	code = -1;

	if (!LcScg_Cmd_Err())
		return (0);

	if (cp->sense.code >= 0x70)
		code = ((struct scsi_ext_sense *)&(cp->sense))->sense_code;
	else
		code = cp->sense.code;
	return (code);
}
//--------------------------------------------------------------------
int		Cls_GrOddScg::Scg_Sense_Qual()
{
	struct scg_cmd *cp = &m_Scmd;

	if (!LcScg_Cmd_Err())
		return (0);

	if (cp->sense.code >= 0x70)
		return (((struct scsi_ext_sense *)&(cp->sense))->qual_code);
	else
		return (0);
}
//--------------------------------------------------------------------
int	Cls_GrOddScg::scg_getresid()
{
		return m_Scmd.resid;
}
//--------------------------------------------------------------------
int	Cls_GrOddScg::Scg_NumBus()
{
	return (1000+MAX_ATA);
	return (MAX_SCG+MAX_ATA);
}
//--------------------------------------------------------------------
int		_select_target(Cls_GrOddScg	*scgp, FILE	*f, int	type, int	idx)
{
	// var --------------------
	int	initiator;
	int	cscsibus;
	int	ctarget;
	int	clun;
	int	numbus;
	int	n;
	int	low	= -1;
	int	high	= -1;
	int	amt	= -1;
	int	bus;
	int	tgt;
	int	lun = 0;
	int	err;
	BOOL	have_tgt;
	struct scg_cmd* Tv_PtrScgCmd;
	// code -------------------
	cscsibus	= scg_scsibus(scgp);
	ctarget	  = scg_target(scgp);
	clun		 	= scg_lun(scgp);
	numbus 		= scgp->Scg_NumBus();
	Tv_PtrScgCmd = scgp->GetScgCmd();

	if (numbus < 0)
		numbus = 1024;

	for(bus = 0; bus < numbus; bus++)
	{
		scgp->SetTarget(bus, 0, 0);

		if(!scgp->Scg_HaveBus(bus))
			continue;

		//initiator = scgp->ops->scgo_initiator_id(scgp);
		initiator = -1;

		for (tgt = 0; tgt < 16; tgt++)
		{
			n = bus * 100 + tgt;

			scgp->SetTarget(bus, tgt, lun);

			have_tgt = unit_ready(scgp) || scgp->m_Scmd.error != SCG_FATAL;
			err = errno;
			if (err == EPERM || err == EACCES)
				return (-1);

			if (!have_tgt && tgt > 7)
			{
				if (Tv_PtrScgCmd->ux_errno == EINVAL)
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

	scgp->SetTarget(cscsibus, ctarget, clun);

	return (amt);
}
//--------------------------------------------------------------------
int		_read_buffer(Cls_GrOddScg	*scgp, int size)
{
	// local -------------------
		char*	Tv_PtrBuf;
	// code --------------------
		Tv_PtrBuf	=	(char*)scgp->m_PtrBuf;
		Tv_PtrBuf[size-2] = (char)0x55; 
		Tv_PtrBuf[size-1] = (char)0xFF;
		if (read_buffer(scgp, Tv_PtrBuf, size, 0) < 0 || scgp->scg_getresid() != 0)
		{
			return (-1);
		}

		if (Tv_PtrBuf[size-2] == (char)0x55 || Tv_PtrBuf[size-1] == (char)0xFF)
		{
			return (-1);
		}
		return (0);
}
//--------------------------------------------------------------------
int		select_target(Cls_GrOddScg *scgp, FILE *f)
{
		return (_select_target(scgp, f, -1, -1));
}
//--------------------------------------------------------------------
int		find_target(Cls_GrOddScg *scgp,	int	type,	int	idx)
{
		return (_select_target(scgp, (FILE *)NULL, type, idx));
}
//--------------------------------------------------------------------
void	reload_media(Cls_GrOddScg	*scgp, void* A_PtrDev)
{
	Cls_GrOddDevBase* dp;
	char	ans[2];
	int	f = -1;

	dp = (Cls_GrOddDevBase*) A_PtrDev;
	unload_media(scgp, dp, F_EJECT);

	if ((dp->GetFlags() & CDR_TRAYLOAD) != 0)
	{
		load_media(scgp, dp, FALSE);
	}

	if (((dp->GetFlags() & CDR_TRAYLOAD) == 0) || !wait_unit_ready(scgp, 5))
	{
		static FILE	*tty = NULL;

		if (tty == NULL) {
			tty = stdin;
			if ((dp->GetCmdFlags() & F_STDIN) != 0)
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
int		write_secs(Cls_GrOddScg	*scgp, void* A_PtrDev, char *bp, long startsec, int bytespt, int	secspt, BOOL islast)
{
	// local -------------------
		dstat_t*	Tv_PtrDstat;
		int				Tv_Result;
		Cls_GrOddDevBase* dp;
	// code --------------------
		dp	= (Cls_GrOddDevBase*)A_PtrDev;
		Tv_PtrDstat	= dp->GetDstat();
again:
		Tv_Result = dp->write_trackdata(scgp, bp, startsec, bytespt, secspt, islast);
		if (Tv_Result < 0)
		{
			if (scsi_in_progress(scgp))
			{
				if ((Tv_PtrDstat->ds_flags & DSF_NOCD) == 0)
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

		return Tv_Result;
}
//--------------------------------------------------------------------
void	load_media(Cls_GrOddScg	*scgp, void* A_PtrDev, BOOL	doexit)
{
	Cls_GrOddDevBase* dp;
	int	code;
	int	key;
	int	err;

	dp = (Cls_GrOddDevBase*) A_PtrDev;
	BOOL	immed = (dp->GetCmdFlags() & F_IMMED) != 0;

	test_unit_ready(scgp);		/* First eat up unit attention		*/
	if (dp->load(scgp) < 0) {	/* now try to load media and	*/
		if (!doexit)
			return;
	}
	scsi_start_stop_unit(scgp, 1, 0, immed); /* start unit in silent mode	*/

	if (!wait_unit_ready(scgp, 60)) {
		code = scgp->Scg_Sense_Code();
		key = scgp->Scg_Sense_Key();
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
void	unload_media(Cls_GrOddScg	*scgp, void* A_PtrDev,	__u32 flags)
{
	Cls_GrOddDevBase* dp;

	dp = (Cls_GrOddDevBase*) A_PtrDev;
	scsi_prevent_removal(scgp, 0);
	if ((flags & F_EJECT) != 0) {
		if (dp->cdr_unload(scgp) < 0)
			printf("Cannot eject media.\n");
	}
}
//--------------------------------------------------------------------
int		find_drive(Cls_GrOddScg	*scgp, char	*dev,	int	flags)
{
	int	ntarget;

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
int		pad_track(Cls_GrOddScg *scgp, void* A_PtrDev, track_t *trackp, long startsec, __u64 amt,
	BOOL dolast, __u64 *bytesp)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*) A_PtrDev;
	int	track = trackp->trackno;
	__u64	bytes	= 0;
	__u64 savbytes = 0;
	__u64	padsize = amt;
	int	secsize;
	int	secspt;
	int	bytespt;
	int	amount;
	BOOL	neednl	= FALSE;
	BOOL	islast	= FALSE;
	struct timeval tlast;
	float	secsps = 75.0;
	long bsize;
	long bfree;
	dstat_t* Tv_PtrDstat;

	Tv_PtrDstat		= dp->GetDstat();

	if (Tv_PtrDstat->ds_flags & DSF_DVD)
		secsps = (float)676.27;
	if (Tv_PtrDstat->ds_flags & DSF_BD)
		secsps = (float)2195.07;

	if (dp->buffer_cap(scgp, &bsize, &bfree) < 0)
		bsize = -1L;
	if (bsize == 0)		/* If we have no (known) buffer, we cannot */
		bsize = -1L;	/* retrieve the buffer fill ratio	   */

	secsize = trackp->secsize;
	secspt = trackp->secspt;
	bytespt = secsize * secspt;

	memset(scgp->m_PtrBuf, '\0', bytespt);

	/*
	if ((amt >> 20) > 0) {
		flush();
	}
	*/
	gettimeofday(&tlast, (struct timezone *)0);
	do {
		if (amt < bytespt) {
			bytespt = (int)roundup(amt, secsize);
			secspt = bytespt / secsize;
		}
		if (dolast && (amt - bytespt) <= 0)
			islast = TRUE;

		if (is_raw(trackp)) {
			encsectors(trackp, (unsigned char *)scgp->m_PtrBuf, startsec, secspt);
			fillsubch(trackp, (unsigned char *)scgp->m_PtrBuf, startsec, secspt);
		}

		amount = write_secs(scgp, dp, (char*)scgp->m_PtrBuf, startsec, bytespt, secspt, islast);
		if (amount < 0) {
			printf("%swrite track pad data: error after %lld bytes\n",
							neednl?"\n":"", bytes);
			if (bytesp)
				*bytesp = bytes;
			//(*dp->cdr_buffer_cap)(scgp, (long *)0, (long *)0);
			dp->buffer_cap(scgp, NULL, NULL);
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
void	catchsig(int	sig)
{
	//
}
//--------------------------------------------------------------------
int		get_dmaspeed(Cls_GrOddScg	*scgp, void* A_PtrDev, int A_BufSize )
{
	// local -------------------
		int	i;
		long	t;
		int	bs;
		int	tsize;
		int	maxdma;
		extern struct timeval	starttime;
		extern struct timeval	fixtime;
		char*	Tv_PtrBuf;
		Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	// code --------------------
		Tv_PtrBuf	=	(char*)scgp->m_PtrBuf;

		memset((caddr_t)Tv_PtrBuf, '\0', 4);
		tsize = 0;
		i = read_buffer(scgp, Tv_PtrBuf, 4, 0);
		if (i < 0 || scgp->scg_getresid() != 0)
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
int		read_buffer(Cls_GrOddScg *scgp, caddr_t bp, int cnt, int mode)
{
	struct	scg_cmd	*scmd = &scgp->m_Scmd;

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

	scgp->m_CmdName = "read buffer";

	return (scgp->Scg_Cmd());
}
//--------------------------------------------------------------------
int		write_buffer(Cls_GrOddScg *scgp, char *buffer, long length, int mode, int bufferid, long offset)
{
	struct	scg_cmd	*scmd = &scgp->m_Scmd;
	char		*cdb;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = buffer;
	scmd->size = length;
	scmd->flags = SCG_DISRE_ENA|SCG_CMD_RETRY;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;

	cdb = (char *)scmd->cdb.cmd_cdb;

	cdb[0] = 0x3B;
	cdb[1] = (char)(mode & 7);
	cdb[2] = (char)bufferid;
	cdb[3] = (char)(offset >> 16);
	cdb[4] = (char)((offset >> 8) & 0xff);
	cdb[5] = (char)(offset & 0xff);
	cdb[6] = (char)(length >> 16);
	cdb[7] = (char)((length >> 8) & 0xff);
	cdb[8] = (char)(length & 0xff);

	scgp->m_CmdName = "write_buffer";

	if (scgp->Scg_Cmd() >= 0)
		return (1);
	return (0);
}
//--------------------------------------------------------------------
void	opentracks(track_t *trackp, __u64 A_IsoSize)
{
	track_t	*tp;
	int	i;
	int	tracks = trackp[0].tracks;

	__u64	tracksize;
	int	secsize;

	printf("opentracks tracks = %d\n", tracks); 
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
				tp->padsecs =	(long)(300 - tracksize / secsize);
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
		trackp->tracksecs = (long)((trackp->itracksize + trackp->isecsize -1) / trackp->isecsize);
		trackp->tracksize = (long)((trackp->itracksize / trackp->isecsize) * trackp->secsize	+ trackp->itracksize % trackp->isecsize);
	}
	else
	{
		trackp->tracksecs = -1L;
	}
}
//--------------------------------------------------------------------
BOOL	check_wrmode(void* A_PtrDev, __u32 wmode, int tflags)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	int	cdflags = dp->GetFlags();

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
void	setleadinout(int tracks, track_t *trackp)
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
void	set_trsizes(void* A_PtrDev, int tracks, track_t *trackp, int A_BufSize )
{
	int	i;
	int	secsize;
	int	secspt;
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;

	trackp[1].flags		|= TI_FIRST;
	trackp[tracks].flags	|= TI_LAST;

	for (i = 0; i <= tracks+1; i++)
	{
		if ((dp->GetFlags() & CDR_SWABAUDIO) != 0 &&	is_audio(&trackp[i]))
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
void	setpregaps(int tracks, track_t	*trackp)
{
	int	i;
	int	sectype;
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
void	set_wrmode(void* A_PtrDev, __u32 wmode, int	tflags)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	dstat_t	*dsp = dp->GetDstat();

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
long	checktsize(int	tracks,	track_t	*trackp)
{
	int	i;
	__u64	curr;
	__u64	total = -150;	/* CD track #1 pregap compensation */
	track_t	*tp;

	if (trackp->flags & TI_NOCD)
		total = 0;
	for (i = 1; i <= tracks; i++) {
		tp = &trackp[i];
		if (!is_pregap(tp))
			total += tp->pregapsize;

		printf("track: %d start: %lld pregap: %ld\n",	i, total, tp->pregapsize);
		tp->trackstart = (long)total;
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
	tp->trackstart = (long)total;
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
	return (long)(total);
}
//--------------------------------------------------------------------
void	set_secsize(Cls_GrOddScg *scgp,	int	secsize)
{
	if (secsize > 0)
	{
		select_secsize(scgp, secsize);
	}
}
//--------------------------------------------------------------------
void	check_recovery(Cls_GrOddScg	*scgp, void* A_PtrDev, __u32 flags)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;

	if (dp->check_recovery(scgp))
	{
		printf("Recovery needed.\n");
		unload_media(scgp, dp, flags);
	}
}
//--------------------------------------------------------------------
BOOL	checkdsize(Cls_GrOddScg *scgp, void* A_PtrDev, long tsize, __u32 flags)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	long	startsec = 0L;
	long	endsec = 0L;
	dstat_t	*dsp = dp->GetDstat();

	dp->next_wr_address(scgp, (track_t*)0, &startsec);

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
BOOL	do_opc(Cls_GrOddScg	*scgp, void* A_PtrDev, __u32 flags)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;

	//if ((flags & F_DUMMY) == 0 && dp->opc() dp->cdr_opc)
	if ((flags & F_DUMMY) == 0)
	{
		printf("Performing OPC...\n");
		if (dp->opc(scgp, NULL, 0, TRUE) < 0)
		{
			printf("OPC failed.\n");
			if ((flags & F_FORCE) == 0)
				return (FALSE);
		}
	}
	return (TRUE);
}
//--------------------------------------------------------------------
void	cdrstats(void* A_PtrDev)
{
	Cls_GrOddDevBase* dp = (Cls_GrOddDevBase*)A_PtrDev;
	dstat_t* Tv_PtrDstat;
	float	secsps = 75.0;
	BOOL	nostop = FALSE;
	Tv_PtrDstat		= dp->GetDstat();

	if (starttime.tv_sec == 0)
		return;

	if (stoptime.tv_sec == 0) {
		gettimeofday(&stoptime, (struct timezone *)0);
		nostop = TRUE;
	}

	if ((Tv_PtrDstat->ds_cdrflags & RF_DID_STAT) != 0)
		return;
	Tv_PtrDstat->ds_cdrflags |= RF_DID_STAT;
}
//--------------------------------------------------------------------
int		fill_buf(int f,	track_t	*trackp, long	secno, char	*bp, int size)
{
	int	amount = 0;
	int	nsecs;
	int	rsize;
	int	rmod;
	int	readoffset = 0;

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
		subrecodesecs(trackp, (unsigned char *)bp, secno, nsecs);

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
		encsectors(trackp, (unsigned char *)bp, secno, nsecs);
		fillsubch(trackp, (unsigned char *)bp, secno, nsecs);
	}
	else
	{
		scrsectors(trackp, (unsigned char *)bp, secno, nsecs);

		if (trackp->isecsize == 2352)
			fillsubch(trackp, (unsigned char *)bp, secno, nsecs);
	}
	return (amount);
}
//--------------------------------------------------------------------
int		read_buf(int f,	char *bp,	int	size)
{
	char	*p = bp;
	int	amount = 0;
	int	n;

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
void	raisepri(int pri)
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
int		rt_raisepri(int pri)
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
int Cls_GrOddScg::LcScg0_Close()
{
	int	f;
	int	b;
	int	t;
	int	l;

	for (b = 0; b < MAX_SCG+MAX_ATA; b++) {
		if (b == m_Local.pgbus)
			continue;
		m_Local.buscookies[b] = (short)-1;
		for (t = 0; t < MAX_TGT; t++) {
			for (l = 0; l < MAX_LUN; l++) {
				f = m_Local.scgfiles[b][t][l];
#ifdef LINUX_APP
				if (f >= 0)
					close(f);
#endif
				m_Local.scgfiles[b][t][l] = (short)-1;
			}
		}
	}
	if (m_Local.xbuf != NULL) 
	{
		free(m_Local.xbuf);
		m_Local.xbufsize = 0L;
		m_Local.xbuf = NULL;
	}

	return (0);
}
//--------------------------------------------------------------------
void	Cls_GrOddScg::LcScg0_FreeBuf()
{
		if (m_PtrBufBase)
		{
			free(m_PtrBufBase);
		}
		m_PtrBufBase = NULL;
}
//--------------------------------------------------------------------
struct scg_cmd* Cls_GrOddScg::GetScgCmd()
{
		return &m_Scmd;
}
//--------------------------------------------------------------------
BOOL Cls_GrOddScg::LcScg_Cmd_Err()
{
	struct scg_cmd *cp = &m_Scmd;

	if (cp->error != SCG_NO_ERROR || cp->ux_errno != 0 ||	*(unsigned char *)&cp->scb != 0 ||
		cp->u_sense.cmd_sense[0] != 0)
	{
		return (TRUE);
	}

	return (FALSE);
}
//--------------------------------------------------------------------

