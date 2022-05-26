/*
	scsi-mmc
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
#include	<string.h>
#endif

#include	<GrTypeBase.h>

#include	<Hw/Odd/GrScsiCdr.h>
#include	<Hw/Odd/GrSCSI.h>
#include	<Hw/Odd/GrOddRec.h>
#include	<Hw/Odd/GrScsiMmc.h>

//====================================================================
//local const

//====================================================================
//local type
struct profiles {
	unsigned __s16	code;
	__s8		*name;
} pl[] = {
	{ 0x0000,	"Reserved", },
	{ 0x0001,	"Non -removable Disk", },
	{ 0x0002,	"Removable Disk", },
	{ 0x0003,	"MO Erasable", },
	{ 0x0004,	"MO Write Once", },
	{ 0x0005,	"AS-MO", },

	/* 0x06..0x07 is reserved */

	{ 0x0008,	"CD-ROM", },
	{ 0x0009,	"CD-R", },
	{ 0x000A,	"CD-RW", },

	/* 0x0B..0x0F is reserved */

	{ 0x0010,	"DVD-ROM", },
	{ 0x0011,	"DVD-R sequential recording", },
	{ 0x0012,	"DVD-RAM", },
	{ 0x0013,	"DVD-RW restricted overwrite", },
	{ 0x0014,	"DVD-RW sequential recording", },
	{ 0x0015,	"DVD-R/DL sequential recording", },
	{ 0x0016,	"DVD-R/DL layer jump recording", },
	{ 0x0017,	"DVD-RW/DL", },

	/* 0x18..0x19 is reserved */

	{ 0x001A,	"DVD+RW", },
	{ 0x001B,	"DVD+R", },

	{ 0x0020,	"DDCD-ROM", },
	{ 0x0021,	"DDCD-R", },
	{ 0x0022,	"DDCD-RW", },

	{ 0x002A,	"DVD+RW/DL", },
	{ 0x002B,	"DVD+R/DL", },

	{ 0x0040,	"BD-ROM", },
	{ 0x0041,	"BD-R sequential recording", },
	{ 0x0042,	"BD-R random recording", },
	{ 0x0043,	"BD-RE", },

	/* 0x44..0x4F is reserved */

	{ 0x0050,	"HD DVD-ROM", },
	{ 0x0051,	"HD DVD-R", },
	{ 0x0052,	"HD DVD-RAM", },
	{ 0x0053,	"HD DVD-RW", },

	/* 0x54..0x57 is reserved */

	{ 0x0058,	"HD DVD-R/DL", },

	{ 0x005A,	"HD DVD-RW/DL", },

	{ 0xFFFF,	"No standard Profile", },
};

//====================================================================
//local function
__s32	scsi_get_performance(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	ndesc, __s32 type, __s32 datatype);
__s32	scsi_set_streaming(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp);

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
__s32
prdiskstatus(St_GrScsiIf	*scgp,	cdr_t	*dp, BOOL8	is_cd)
{
	struct disk_info	di;
	struct rzone_info	rz;
	__s32			sessions;
	__s32			track;
	__s32			tracks;
	__s32			t;
	__s32			s;
	long			raddr;
	long			lastaddr = -1;
	long			lastsess = -1;
	long			leadout = -1;
	long			lo_sess = 0;
	long			nwa = -1;
	long			rsize = -1;
	long			border_size = -1;
	__s32			profile;

	profile = get_curprofile(scgp);
	if (profile > 0) {
		__s32 mt = get_mediatype(scgp);

		pname_known(profile);
	}
	get_diskinfo(scgp, &di, sizeof (di));

	sessions = di.numsess + di.numsess_msb * 256;
	tracks = di.last_track_ls + di.last_track_ls_msb * 256;

	memset((caddr_t)&rz, '\0', sizeof (rz));
	for (t = di.first_track; t <= tracks; t++) {
		memset((caddr_t)&rz, '\0', sizeof (rz));
		get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, t, sizeof (rz));
		track = rz.rzone_num_lsb + rz.rzone_num_msb * 256;
		s = rz.border_num_lsb + rz.border_num_msb * 256;
		raddr = a_to_4_byte(rz.rzone_start);
		if (rsize >= 0)
			border_size = raddr - (lastaddr+rsize);
		if (!rz.blank && s > lastsess) { /* First track in last sess ? */
			lastaddr = raddr;
			lastsess = s;
		}
		nwa = a_to_4_byte(rz.next_recordable_addr);
		rsize = a_to_4_byte(rz.rzone_size);
		if (!rz.blank) {
			leadout = raddr + rsize;
			lo_sess = s;
		}
		printf("%5d %5d %-6s %-10ld %-10ld %ld", track, s, rz.blank ? "Blank" :
				rz.trackmode & 4 ? "Data" : "Audio",
			raddr, raddr + rsize -1, rsize);
		printf("\n");
	}
	printf("\n");
	if (lastaddr >= 0)
		printf("Last session start address:         %ld\n", lastaddr);
	if (leadout >= 0)
		printf("Last session leadout start address: %ld\n", leadout);
	if (rz.nwa_v) {
		printf("Next writable address:              %ld\n", nwa);
		printf("Remaining writable size:            %ld\n", rsize);
	}

	return (0);
}
//--------------------------------------------------------------------
__s32		get_diskinfo(St_GrScsiIf *scgp, struct disk_info *dip, __s32 cnt)
{
	__s32	len;
	__s32	ret;

	memset((caddr_t)dip, '\0', cnt);

	if (read_disk_info(scgp, (caddr_t)dip, 4) < 0)
		return (-1);
	len = a_to_u_2_byte(dip->data_len);
	len += 2;
	if (len > cnt)
		len = cnt;
	ret = read_disk_info(scgp, (caddr_t)dip, len);

	return (ret);
}
//--------------------------------------------------------------------
__s32		get_mediatype(St_GrScsiIf *scgp)
{
	__s32	profile = get_curprofile(scgp);

	if (profile < 0x08)
		return (MT_NONE);
	if (profile >= 0x08 && profile < 0x10)
		return (MT_CD);
	if (profile >= 0x10 && profile < 0x40)
		return (MT_DVD);
	if (profile >= 0x40 && profile < 0x50)
		return (MT_BD);
	if (profile >= 0x50 && profile < 0x60)
		return (MT_HDDVD);

	return (MT_NONE);
}
//--------------------------------------------------------------------
__s32		get_curprofile(St_GrScsiIf *scgp)
{
	unsigned __s8	cbuf[8];
	__s32	amt;
	__s32	flen;
	__s32	profile;
	__s32	i;

	memset(cbuf, '\0', sizeof (cbuf));
	i = get_configuration(scgp, (__s8 *)cbuf, sizeof (cbuf), 0, 0);
	if (i < 0)
		return (-1);

	amt = sizeof (cbuf) - scgp->scmd.resid;
	if (amt < 8)
		return (-1);
	flen = a_to_u_4_byte(cbuf);
	if (flen < 4)
		return (-1);

	profile = a_to_u_2_byte(&cbuf[6]);

	return (profile);
}
//--------------------------------------------------------------------
__s32		get_configuration(St_GrScsiIf *scgp, caddr_t	bp,	__s32	cnt, __s32 st_feature, __s32 rt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x46;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	if (rt & 1)
		scmd->cdb.g1_cdb.reladr  = 1;
	if (rt & 2)
		scmd->cdb.g1_cdb.res  = 1;

	i_to_2_byte(scmd->cdb.g1_cdb.addr, st_feature);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "get_configuration";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
BOOL8	pname_known(__u32	code)
{
	unsigned __s16	i;

	for (i = 0; i < sizeof (pl) / sizeof (pl[0]); i++) {
		if (code == pl[i].code)
			return (TRUE);
	}
	return (FALSE);
}
//--------------------------------------------------------------------
__s32 get_proflist(St_GrScsiIf	*scgp, BOOL8	*wp, BOOL8	*cdp,	BOOL8 *dvdp,	BOOL8 *dvdplusp,	BOOL8 *ddcdp)
{
#ifdef LINUX_APP
		unsigned __s8	*p;
		__s32	n;
		__s32	i;
		__s32	profile;
		unsigned __s8	cbuf[1024];
		__s32	flen;
#endif
		__s32	curprofile;
		BOOL8	wr	= FALSE;
		BOOL8	cd	= FALSE;
		BOOL8	dvd	= FALSE;
		BOOL8	dvdplus	= FALSE;
		BOOL8	ddcd	= FALSE;
	
#ifdef LINUX_APP
		flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
		if (flen < 0)
			return (-1);

		p = cbuf;

		curprofile = a_to_u_2_byte(&p[6]);

		p += 8;		/* Skip feature header	*/
		n = p[3];	/* Additional length	*/
		n /= 4;
		p += 4;
	
		for (i = 0; i < n; i++)
		{
			profile = a_to_u_2_byte(p);
			p += 4;
			if (profile >= 0x0008 && profile < 0x0010)
				cd = TRUE;
			if (profile > 0x0008 && profile < 0x0010)
				wr = TRUE;
	
			if (profile >= 0x0010 && profile < 0x0018)
				dvd = TRUE;
			if (profile > 0x0010 && profile < 0x0018)
				wr = TRUE;
	
			if (profile >= 0x0018 && profile < 0x0020)
				dvdplus = TRUE;
			if (profile > 0x0018 && profile < 0x0020)
				wr = TRUE;
	
			if (profile >= 0x0020 && profile < 0x0028)
				ddcd = TRUE;
			if (profile > 0x0020 && profile < 0x0028)
				wr = TRUE;
		}
#else
		wr	=	TRUE;
		dvd	=	TRUE;
		curprofile	=	0;
		scgp->inq.type	=	INQ_ROMD;
#endif

		if (wp)
			*wp	= wr;
		if (cdp)
			*cdp	= cd;
		if (dvdp)
			*dvdp	= dvd;
		if (dvdplusp)
			*dvdplusp = dvdplus;
		if (ddcdp)
			*ddcdp	= ddcd;
	
		return (curprofile);
}
//--------------------------------------------------------------------
__s32 get_profiles(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt)
{
	__s32	amt;
	__s32	flen;
	__s32	i;

	flen = get_conflen(scgp, 0, 0);
	if (flen < 0)
		return (-1);
	if (cnt < flen)
		flen = cnt;

	memset(bp, '\0', cnt);
	i = get_configuration(scgp, (__s8 *)bp, flen, 0, 0);
	if (i < 0)
		return (-1);
	amt = flen - scgp->scmd.resid;

	flen = a_to_u_4_byte(bp);
	if ((flen+4) < amt)
		amt = flen+4;

	return (amt);
}
//--------------------------------------------------------------------
__s32 get_conflen(St_GrScsiIf *scgp,	__s32	st_feature,	__s32	rt)
{
		unsigned __s8	cbuf[8];
		__s32	flen;
		__s32	i;
	
		memset(cbuf, '\0', sizeof (cbuf));
		i = get_configuration(scgp, (__s8 *)cbuf, sizeof (cbuf), st_feature, rt);
		if (i < 0)
			return (-1);
		i = sizeof (cbuf) - scgp->scmd.resid;
		if (i < 4)
			return (-1);
	
		flen = a_to_u_4_byte(cbuf);
		if (flen < 4)
			return (-1);
		return (flen);
}
//--------------------------------------------------------------------
__s32 get_wproflist(St_GrScsiIf *scgp,	BOOL8 *cdp, BOOL8	*dvdp, BOOL8	*dvdplusp, BOOL8	*ddcdp)
{
		unsigned __s8	cbuf[1024];
		unsigned __s8	*p;
		__s32	flen;
		__s32	curprofile;
		__s32	profile;
		__s32	i;
		__s32	n;
		BOOL8	cd	= FALSE;
		BOOL8	dvd	= FALSE;
		BOOL8	dvdplus	= FALSE;
		BOOL8	ddcd	= FALSE;
	
		flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
		if (flen < 0)
			return (-1);
		p = cbuf;
		curprofile = a_to_u_2_byte(&p[6]);
	
		p += 8;		/* Skip feature header	*/
		n = p[3];	/* Additional length	*/
		n /= 4;
		p += 4;
	
		for (i = 0; i < n; i++) {
			profile = a_to_u_2_byte(p);
			p += 4;
			if (profile > 0x0008 && profile < 0x0010)
				cd = TRUE;
			if (profile > 0x0010 && profile < 0x0018)
				dvd = TRUE;
			if (profile > 0x0018 && profile < 0x0020)
				dvdplus = TRUE;
			if (profile > 0x0020 && profile < 0x0028)
				ddcd = TRUE;
		}
		if (cdp)
			*cdp	= cd;
		if (dvdp)
			*dvdp	= dvd;
		if (dvdplusp)
			*dvdplusp = dvdplus;
		if (ddcdp)
			*ddcdp	= ddcd;
	
		return (curprofile);
}
//--------------------------------------------------------------------
__s32		get_supported_cdrw_media_types(St_GrScsiIf *scgp)
{
	unsigned __s8  cbuf[16];
	__s32	ret;

	memset(cbuf, '\0', sizeof (cbuf));

	ret = get_configuration(scgp, (__s8 *)cbuf, sizeof (cbuf), 0x37, 2);

	if (ret < 0)
		return (-1);

	if (cbuf[3] < 12)	/* Couldn't retrieve feature 0x37	*/
		return (-1);

	return (__s32)(cbuf[13]);
}
//--------------------------------------------------------------------
__s32		sessstatus(St_GrScsiIf *scgp, BOOL8 is_cd, long *offp, long *nwap)
{
	struct disk_info	di;
	struct rzone_info	rz;
	__s32			sessions;
	__s32			track;
	__s32			tracks;
	__s32			t;
	__s32			s;
	long			raddr;
	long			lastaddr = -1;
	long			lastsess = -1;
	long			leadout = -1;
	long			lo_sess = 0;
	long			nwa = -1;
	long			rsize = -1;
	long			border_size = -1;


	if (get_diskinfo(scgp, &di, sizeof (di)) < 0)
		return (-1);

	sessions = di.numsess + di.numsess_msb * 256;
	tracks = di.last_track_ls + di.last_track_ls_msb * 256;

	memset((caddr_t)&rz, '\0', sizeof (rz));
	for (t = di.first_track; t <= tracks; t++) {
		memset((caddr_t)&rz, '\0', sizeof (rz));
		if (get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, t, sizeof (rz)) < 0)
			return (-1);
		track = rz.rzone_num_lsb + rz.rzone_num_msb * 256;
		s = rz.border_num_lsb + rz.border_num_msb * 256;
		raddr = a_to_4_byte(rz.rzone_start);
		if (rsize >= 0)
			border_size = raddr - (lastaddr+rsize);
		if (!rz.blank && s > lastsess) { /* First track in last sess ? */
			lastaddr = raddr;
			lastsess = s;
		}
		nwa = a_to_4_byte(rz.next_recordable_addr);
		rsize = a_to_4_byte(rz.rzone_size);
		if (!rz.blank) {
			leadout = raddr + rsize;
			lo_sess = s;
		}
	}
	if (lastaddr >= 0 && offp != NULL)
		*offp = lastaddr;

	if (rz.nwa_v && nwap != NULL)
		*nwap = nwa;

	return (0);
}
//--------------------------------------------------------------------
__s32		scsi_get_perf_curspeed(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp)
{
	register struct	scg_cmd	*scmd = &scgp->scmd;
	struct mmc_performance_header	*ph;
	struct mmc_performance		*perfp;
#define	MAX_AMT	100
	__s8 buffer[8 + MAX_AMT*16];
	__u32	ul;
	__u32	end;
	__u32	speed;
	__s32	amt;
	__s32	i;
	__s32	mt = 0;
	__s32	ssp = 1;

	if (endp || writep) {
		memset((caddr_t) buffer, '\0', sizeof (buffer));
		if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x00, 0x04) < 0) {
			goto doread;
		}

		ph = (struct mmc_performance_header *)buffer;
		amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_performance);
		if (amt < 1)
			amt = 1;
		if (amt > MAX_AMT)
			amt = MAX_AMT;

		if (scsi_get_performance(scgp, buffer, 8+16*amt, amt, 0x00, 0x04) < 0)
			return (-1);

		ph = (struct mmc_performance_header *)buffer;
		perfp = (struct mmc_performance *)(((__s8 *)ph) +
				sizeof (struct mmc_performance_header));

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > amt)
			i = amt;
		end = 0;
		speed = 0;
		for (; --i >= 0; perfp++) {
			ul = a_to_u_4_byte(perfp->end_lba);
			if (ul > end) {
				end = ul;
				ul = a_to_u_4_byte(perfp->end_perf);
				speed = ul;
			}
		}

		if (endp)
			*endp = end;

		if (writep)
			*writep = speed;

		perfp = (struct mmc_performance *)(((__s8 *)ph) +
				sizeof (struct mmc_performance_header));
		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > scmd->cdb.cmd_cdb[9])
			i = scmd->cdb.cmd_cdb[9];
	}
doread:
	if (readp) {
		memset((caddr_t) buffer, '\0', sizeof (buffer));
		if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x00, 0x00) < 0) {
			return (-1);
		}

		ph = (struct mmc_performance_header *)buffer;
		amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_performance);
		if (amt < 1)
			amt = 1;
		if (amt > MAX_AMT)
			amt = MAX_AMT;

		if (scsi_get_performance(scgp, buffer, 8+16*amt, amt, 0x00, 0x00) < 0)
			return (-1);

		ph = (struct mmc_performance_header *)buffer;
		perfp = (struct mmc_performance *)(((__s8 *)ph) +
				sizeof (struct mmc_performance_header));

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > amt)
			i = amt;
		end = 0;
		speed = 0;
		for (; --i >= 0; perfp++) {
			ul = a_to_u_4_byte(perfp->end_lba);
			if (ul > end) {
				end = ul;
				ul = a_to_u_4_byte(perfp->end_perf);
				speed = ul;
			}
		}

		if (readp)
			*readp = speed;

		i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_performance);
		if (i > scmd->cdb.cmd_cdb[9])
			i = scmd->cdb.cmd_cdb[9];
	}

	return (0);
}
//--------------------------------------------------------------------
void	print_format_capacities(St_GrScsiIf	*scgp)
{
	/*unsigned __s8	b[1024];
	__s32	i;
	unsigned __s8	*p;

	memset(b, '\0', sizeof (b));
	scgp->silent++;
	i = read_format_capacities(scgp, (__s8 *)b, sizeof (b));
	scgp->silent--;
	if (i < 0)
		return;

	i = b[3] + 4;
	memset(b, '\0', sizeof (b));
	if (read_format_capacities(scgp, (__s8 *)b, i) < 0)
		return;

	i = b[3];
	if (i > 0) {
		__s32	cnt;
		__u32 n1;
		__u32 n2;
		printf("\n    Capacity  Blklen/Sparesz.  Format-type  Type\n");
		for (p = &b[4]; i > 0; i -= 8, p += 8) {
			cnt = 0;
			n1 = a_to_u_4_byte(p);
			n2 = a_to_u_3_byte(&p[5]);
			printf("%12lu %16lu         0x%2.2X  %s\n",
				(__u32)n1, (__u32)n2,
				(p[4] >> 2) & 0x3F,
				fdt[p[4] & 0x03]);
		}
	}*/
}
//--------------------------------------------------------------------
__s32		get_format_capacities(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt)
{
	__s32			len = sizeof (struct scsi_format_cap_header);
	struct scsi_format_cap_header	*hp;

	memset(bp, '\0', cnt);
	if (cnt < len)
		return (-1);
	if (read_format_capacities(scgp, bp, len) < 0) {
		return (-1);
	}

	if (scg_getresid(scgp) > 0)
		return (-1);

	hp = (struct scsi_format_cap_header *)bp;
	len = hp->len;
	len += sizeof (struct scsi_format_cap_header);
	while (len > cnt)
		len -= sizeof (struct scsi_format_cap_desc);

	if (read_format_capacities(scgp, bp, len) < 0)
		return (-1);

	len -= scg_getresid(scgp);
	return (len);
}
//--------------------------------------------------------------------
__s32		read_format_capacities(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t)scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA|SCG_DISRE_ENA;
	scmd->cdb_len = SC_G1_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0x23;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	g1_cdblen(&scmd->cdb.g1_cdb, cnt);

	scgp->cmdname = "read_format_capacities";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		scsi_get_perf_maxspeed(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	struct mmc_performance_header	*ph;
	struct mmc_write_speed		*wsp;
#define	MAX_AMT	100
	__s8 buffer[8 + MAX_AMT*16];
	__u32	ul;
	__s32	amt;
	__s32	i;
	__s32	mt = 0;
	__s32	ssp = 1;
	__s8	*mname = NULL;

	memset((caddr_t) buffer, '\0', sizeof (buffer));
	ph = (struct mmc_performance_header *)buffer;
	if (scsi_get_performance(scgp, buffer, 8+16, 1, 0x03, 0) < 0)
		return (-1);

	amt = (a_to_4_byte(ph->p_datalen) -4)/sizeof (struct mmc_write_speed);
	if (amt < 1)
		amt = 1;
	if (amt > MAX_AMT)
		amt = MAX_AMT;
	if (scsi_get_performance(scgp, buffer, 8+amt*16, amt, 0x03, 0) < 0)
		return (-1);

	ph = (struct mmc_performance_header *)buffer;
	wsp = (struct mmc_write_speed *)(((__s8 *)ph) +
				sizeof (struct mmc_performance_header));

	ul = a_to_u_4_byte(wsp->end_lba);
	if (endp)
		*endp = ul;

	ul = a_to_u_4_byte(wsp->read_speed);
	if (readp)
		*readp = ul;

	ul = a_to_u_4_byte(wsp->write_speed);
	if (writep)
		*writep = ul;

	wsp = (struct mmc_write_speed *)(((__s8 *)ph) +
				sizeof (struct mmc_performance_header));

	i = (a_to_4_byte(buffer) -4)/sizeof (struct mmc_write_speed);
	if (i > scmd->cdb.cmd_cdb[9])
		i = scmd->cdb.cmd_cdb[9];

	return (0);
#undef	MAX_AMT
}
//--------------------------------------------------------------------
__s32	scsi_get_performance(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	ndesc, __s32 type, __s32 datatype)
{
	struct	scg_cmd	*scmd = &scgp->scmd;

	memset((caddr_t) scmd, '\0', sizeof (*scmd));
	scmd->addr = bp;
	scmd->size = cnt;
	scmd->flags = SCG_RECV_DATA | SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g1_cdb.cmd = 0xAC;
	scmd->cdb.g1_cdb.lun = scg_lun(scgp);
	scmd->cdb.cmd_cdb[1] |= datatype & 0x1F;
	scmd->cdb.cmd_cdb[9] = ndesc;
	scmd->cdb.cmd_cdb[10] = type;

	scgp->cmdname = "get performance";

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		has_profile(St_GrScsiIf *scgp,	__s32	profile)
{
	unsigned __s8	cbuf[1024];
	unsigned __s8	*p;
	__s32	flen;
	__s32	prf;
	__s32	i;
	__s32	n;

	flen = get_profiles(scgp, (caddr_t)cbuf, sizeof (cbuf));
	if (flen < 0)
		return (-1);

	p = cbuf;
	p += 8;		/* Skip feature header	*/
	n = p[3];	/* Additional length	*/
	n /= 4;
	p += 4;

	for (i = 0; i < n; i++) {
		prf = a_to_u_2_byte(p);

		if (profile == prf)
			return (1);
		p += 4;
	}
	return (0);
}
//--------------------------------------------------------------------
__s32	speed_select_mdvd(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed)
{
	__u32	end_lba = 0x7FFFFFFF;
	__u32	wspeed = writespeed;

	if (scsi_get_perf_maxspeed(scgp, (__u32 *)NULL, (__u32 *)NULL, &end_lba) < 0)
		return (-1);

	if (scsi_set_streaming(scgp, (__u32 *)NULL, &wspeed, &end_lba) < 0)
		return (-1);

	return (0);
}
//--------------------------------------------------------------------
__s32	scsi_set_streaming(St_GrScsiIf *scgp, __u32 *readp, __u32 *writep, __u32 *endp)
{
	struct	scg_cmd	*scmd = &scgp->scmd;
	struct mmc_streaming	str;
	struct mmc_streaming	*sp = &str;

	memset((caddr_t) scmd, '\0', sizeof (*scmd));
	scmd->addr = (caddr_t) sp;
	scmd->size = sizeof (*sp);
	scmd->flags = SCG_DISRE_ENA;
	scmd->cdb_len = SC_G5_CDBLEN;
	scmd->sense_len = CCS_SENSE_LEN;
	scmd->cdb.g5_cdb.cmd = 0xB6;
	scmd->cdb.g5_cdb.lun = scg_lun(scgp);
	i_to_2_byte(&scmd->cdb.cmd_cdb[9], sizeof (*sp)); /* Sz not G5 alike */

	scgp->cmdname = "set streaming";

	memset(sp, '\0', sizeof (*sp));
	if (endp)
		i_to_4_byte(sp->end_lba, *endp);
	else
		i_to_4_byte(sp->end_lba, 0x7FFFFFFF);

	if (readp)
		i_to_4_byte(sp->read_size, *readp);
	else
		i_to_4_byte(sp->read_size, 0x7FFFFFFF);

	if (writep)
		i_to_4_byte(sp->write_size, *writep);
	else
		i_to_4_byte(sp->write_size, 0x7FFFFFFF);

	i_to_4_byte(sp->read_time, 1000);
	i_to_4_byte(sp->write_time, 1000);

	return (scg_cmd(scgp));
}
//--------------------------------------------------------------------
__s32		get_blf(__s32 mt)
{
	switch (mt) {

	case MT_DVD:
		return (16);

	case MT_BD:
		return (32);

	case MT_HDDVD:
		return (32);	/* XXX ??? */

	default:
		return (1);
	}
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
