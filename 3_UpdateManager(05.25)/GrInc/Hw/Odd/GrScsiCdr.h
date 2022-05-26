/*
	scsi cdr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrScsiCdr
#define	_EPC_GrScsiCdr

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include 	<Hw/Odd/GrSCSI.h>

//====================================================================
//constance
#define	DEV_UNKNOWN		0
#define	DEV_ACB40X0		1
#define	DEV_ACB4000		2
#define	DEV_ACB4010		3
#define	DEV_ACB4070		4
#define	DEV_ACB5500		5
#define	DEV_ACB4520A		6
#define	DEV_ACB4525		7
#define	DEV_MD21		8
#define	DEV_MD23		9
#define	DEV_NON_CCS_DSK		10
#define	DEV_CCS_GENDISK		11

#define	TRM_ZERO	0
#define	TRM_USER_ECC	1	/* 2048 bytes user data + 288 Bytes ECC/EDC */
#define	TRM_USER	2	/* All user data (2336 bytes) */

#define	strbeg(s1, s2)	(strstr((s2), (s1)) == (s2))

//====================================================================
//global macro

//====================================================================
//global type
struct siheader {
	unsigned __s8	len[2];
	unsigned __s8	finished;
	unsigned __s8	unfinished;
};

struct sidesc {
	unsigned __s8	sess_number;
	unsigned __s8	res1;
	unsigned __s8	track;
	unsigned __s8	res3;
	unsigned __s8	addr[4];
};

struct sinfo {
	struct siheader	hd;
	struct sidesc	desc[1];
};

//====================================================================
//class

//====================================================================
//global function
BOOL8	unit_ready(St_GrScsiIf *scgp);
BOOL8	wait_unit_ready(St_GrScsiIf *scgp, __s32 secs);
__s32		test_unit_ready(St_GrScsiIf *scgp);
__s32		scsi_start_stop_unit(St_GrScsiIf *scgp, __s32, __s32, BOOL8 immed);
__s32		scsi_close_tr_session(St_GrScsiIf *scgp, __s32 type, __s32 track, BOOL8 immed);
__s32		scsi_flush_cache(St_GrScsiIf *scgp, BOOL8 immed);
__s32		rezero_unit(St_GrScsiIf *scgp);
BOOL8	wait_unit_ready(St_GrScsiIf *scgp, __s32 secs);

__s32		read_rzone_info(St_GrScsiIf	*scgp,	caddr_t	bp,	__s32	cnt);
__s32		send_dvd_structure(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 fmt);

__s32		scsi_prevent_removal(St_GrScsiIf *scgp, __s32);
__s32		read_dvd_structure(St_GrScsiIf *scgp, caddr_t bp, __s32 cnt, __s32 mt, __s32 addr, __s32 layer, __s32 fmt);

BOOL8	getdev(St_GrScsiIf *scgp, BOOL8);
__s32		inquiry(St_GrScsiIf *scgp, caddr_t	bp, __s32	cnt);
BOOL8	do_inquiry(St_GrScsiIf	*scgp, __s32	print);
BOOL8	is_unknown_dev(St_GrScsiIf	*scgp);
__s32		scsi_load(St_GrScsiIf	*scgp,	cdr_t	*dp);
__s32		scsi_unload(St_GrScsiIf *scgp, cdr_t *dp);
__s32		read_buff_cap(St_GrScsiIf *scgp,	long *sp,	long *fp);
__s32		select_secsize(St_GrScsiIf	*scgp, __s32	secsize);
__s32		scsi_cdr_write(St_GrScsiIf	*scgp, caddr_t bp,	long sectaddr,	long size,	__s32	blocks,	BOOL8 islast);
__s32		write_xg0(St_GrScsiIf *scgp,	caddr_t	bp,	long addr, long	size, __s32	cnt);
__s32		write_xg1(St_GrScsiIf *scgp,	caddr_t	bp,	long addr, long	size, __s32	cnt);
__s32		read_session_offset(St_GrScsiIf	*scgp, long	*offp);
__s32		get_trackinfo(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	type,	__s32	addr,	__s32	cnt);
__s32		read_disk_info(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	cnt);
__s32		mode_select(St_GrScsiIf *scgp,	unsigned __s8	*dp, __s32 cnt,	__s32	smp, __s32 pf);
__s32 	send_opc(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	doopc);
__s32		scsi_set_speed(St_GrScsiIf *scgp, __s32 readspeed, __s32 writespeed, __s32 rotctl);
__s32		scsi_blank(St_GrScsiIf *scgp, long addr, __s32 blanktype, BOOL8 immed);
BOOL8	mmc_check(St_GrScsiIf *scgp, BOOL8 *cdrrp, BOOL8 *cdwrp, BOOL8 *cdrrwp, BOOL8 *cdwrwp, BOOL8 *dvdp, BOOL8 *dvdwp);
void	mmc_getval(struct cd_mode_page_2A *mp, BOOL8 *cdrrp, BOOL8 *cdwrp, BOOL8 *cdrrwp, BOOL8 *cdwrwp,
				BOOL8 *dvdp, BOOL8 *dvdwp);
BOOL8	allow_atapi(St_GrScsiIf *scgp, BOOL8 new1);
struct cd_mode_page_2A * mmc_cap(St_GrScsiIf	*scgp, unsigned __s8	*modep);
BOOL8	scsi_in_progress(St_GrScsiIf *scgp);
__s32		send_cue_sheet(St_GrScsiIf	*scgp, caddr_t bp,long size);
__s32		scsi_get_speed(St_GrScsiIf *scgp, __s32 *readspeedp, __s32 *writespeedp);
__s32		read_session_offset_philips(St_GrScsiIf *scgp, long *);

BOOL8	is_mmc(St_GrScsiIf *scgp, BOOL8 *cdwp, BOOL8 *dvdwp);
__s32		read_toc(St_GrScsiIf	*scgp, caddr_t bp, __s32 track,	__s32	cnt,	__s32	msf,	__s32	fmt);
__s32		read_track_info(St_GrScsiIf	*scgp, caddr_t bp, __s32 type, __s32 addr, __s32 cnt);
__s32		mode_select_sg0(St_GrScsiIf *scgp, unsigned __s8	*dp, __s32	cnt,	__s32	smp, __s32	pf);

__s32		read_toc_philips(St_GrScsiIf	*scgp, caddr_t	bp,	__s32	track,	__s32	cnt,	__s32	msf,	__s32	fmt);
__s32		mode_select_g1(St_GrScsiIf	*scgp, unsigned __s8 *dp, __s32	cnt, __s32 smp,	__s32	pf);

__s32		reserve_tr_rzone(St_GrScsiIf	*scgp, long	size);

__s32		read_scsi(St_GrScsiIf *scgp, caddr_t, long, __s32);
__s32		read_g0(St_GrScsiIf *scgp, caddr_t, long, __s32);
__s32		read_g1(St_GrScsiIf *scgp, caddr_t, long, __s32);

BOOL8	is_cddrive(St_GrScsiIf	*scgp);
__s32		qic02(St_GrScsiIf *scgp, __s32	cmd);
__s32		sense_secsize(St_GrScsiIf *scgp,	__s32	current);
__s32		read_capacity(St_GrScsiIf *scgp);
__s32		read_tochdr(St_GrScsiIf *scgp,	cdr_t	*dp, __s32 *fp,	__s32	*lp);
BOOL8	cdr_underrun(St_GrScsiIf	*scgp);

//====================================================================
//out Enviroment conditional
#endif

