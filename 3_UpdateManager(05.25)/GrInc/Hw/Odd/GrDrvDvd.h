/*
scsi opr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrvDvd
#define	_EPC_GrDrvDvd

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrOddRec.h>
#include 	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrCdrDrv.h>
#include 	<Hw/Odd/GrSCSI.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
cdr_t	*identify_dvd(St_GrScsiIf *scgp, cdr_t * );
__s32		attach_dvd(St_GrScsiIf *scgp, cdr_t *);
__s32		init_dvd(St_GrScsiIf *scgp, cdr_t *dp);
__s32		getdisktype_dvd(St_GrScsiIf *scgp, cdr_t *dp);
__s32		prdiskstatus_dvd(St_GrScsiIf *scgp, cdr_t *dp);
__s32		speed_select_dvd(St_GrScsiIf *scgp, cdr_t *dp, __s32 *speedp);
__s32 	session_offset_dvd(St_GrScsiIf *scgp, long *offp);
__s32		next_wr_addr_dvd(St_GrScsiIf *scgp, track_t *trackp, long *ap);
__s32		open_track_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
long	rzone_size(track_t *trackp);
__s32		close_track_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
__s32		open_session_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
__s32		waitformat(St_GrScsiIf *scgp, __s32 secs);
__s32		fixate_dvd(St_GrScsiIf *scgp, cdr_t *dp, track_t *trackp);
__s32		blank_dvd(St_GrScsiIf *scgp, cdr_t *dp, long addr, __s32 blanktype);
__s32		stats_dvd(St_GrScsiIf *scgp, cdr_t *dp);

__s32		set_layerbreak(St_GrScsiIf *scgp, long	tsize, __u32 lbreak);
void	print_dvd00(struct dvd_structure_00 *dp);
void	print_dvd01(struct dvd_structure_01 *dp);
void	print_dvd04(struct dvd_structure_04 *dp);
void	print_dvd05(struct dvd_structure_05 *dp);
void	print_dvd0D(struct dvd_structure_0D *dp);
void	print_dvd0E(struct dvd_structure_0E *dp);
void	print_dvd0F(struct dvd_structure_0F *dp);
void	print_dvd20(struct dvd_structure_20 *dp);
void	print_dvd22(struct dvd_structure_22 *dp);
void	print_dvd23(struct dvd_structure_23 *dp);
void	send_dvd0F(St_GrScsiIf *scgp);

void	print_dvd_info(St_GrScsiIf *scgp);
void	print_laserlog(St_GrScsiIf *scgp);

cdr_t	cdr_dvd = {
	0, 0, 0,
	CDR_DVD|CDR_SWABAUDIO,
	CDR2_NOCD,
	CDR_CDRW_ALL,
	WM_SAO,
	1000, 1000,
	"mmc_dvd",
	"generic SCSI-3/mmc-2 DVD-R/DVD-RW/DVD-RAM driver",
	0,
	(dstat_t *)0,
	identify_dvd,
	attach_dvd,
	init_dvd,
	getdisktype_dvd,
	prdiskstatus_dvd,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,					/* recovery_needed */
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,	/* recover	*/
	speed_select_dvd,
	select_secsize,
	next_wr_addr_dvd,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,		/* reserve_track */
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,	/* gen_cue */
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy, /* send_cue */
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy, /* leadin */
	open_track_dvd,
	close_track_dvd,
	open_session_dvd,
	cmd_dummy,
	cmd_dummy,					/* abort	*/
	session_offset_dvd,
	fixate_dvd,
	stats_dvd,
	blank_dvd,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,	/* no OPC	*/
	cmd_dummy,					/* opt1		*/
	cmd_dummy,					/* opt2		*/
};

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

