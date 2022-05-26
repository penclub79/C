/*
	scsi cdr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrvMmc
#define	_EPC_GrDrvMmc

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
cdr_t*	identify_mmc(St_GrScsiIf	*scgp, cdr_t *dp );
__s32			attach_mmc(St_GrScsiIf	*scgp, cdr_t *dp);
__s32			init_mmc(St_GrScsiIf	*scgp, cdr_t *dp);
__s32			getdisktype_mmc(St_GrScsiIf *scgp,	cdr_t	*dp);
__s32			prdiskstatus_mmc(St_GrScsiIf	*scgp, cdr_t *dp);
__s32			speed_select_mmc(St_GrScsiIf *scgp, cdr_t *dp, __s32 *speedp);
__s32			next_wr_addr_mmc(St_GrScsiIf	*scgp,	track_t	*trackp,	long	*ap);
__s32			gen_cue_mmc(track_t	*trackp, void	*vcuep,	BOOL8 needgap);
__s32			send_cue_mmc(St_GrScsiIf	*scgp,	cdr_t	*dp,	track_t	*trackp);
__s32			open_track_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t *trackp);
__s32			fixate_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t	*trackp);
__s32			stats_mmc(St_GrScsiIf *scgp,	cdr_t	*dp);
__s32			blank_mmc(St_GrScsiIf *scgp,	cdr_t	*dp, long	addr,	__s32	blanktype);
__s32			send_opc_mmc(St_GrScsiIf	*scgp, caddr_t bp, __s32 cnt,	__s32	doopc);
__s32			opt1_mmc(St_GrScsiIf	*scgp, cdr_t *dp);
__s32			opt2_mmc(St_GrScsiIf	*scgp, cdr_t *dp);
__s32			write_leadin_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t *trackp);
__s32			close_track_mmc(St_GrScsiIf *scgp,	cdr_t	*dp, track_t *trackp);
__s32			open_session_mmc(St_GrScsiIf	*scgp, cdr_t *dp,	track_t	*trackp);

__s8*		hasdrvopt(__s8 *optstr, __s8 *optname);
__s8*		hasdrvoptx(__s8 *optstr, __s8 *optname, __s32 flag);
struct ricoh_mode_page_30*	get_justlink_ricoh(St_GrScsiIf	*scgp, unsigned __s8 *mode);
__s32			check_writemodes_mmc(St_GrScsiIf *scgp, cdr_t *dp);

/*static cdr_t	cdr_mmc = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_SAO,
	372, 372,
	"mmc_cdr",
	"generic SCSI-3/mmc   CD-R/CD-RW driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	init_mmc,
	getdisktype_mmc,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*) (St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	next_wr_addr_mmc,
	(__s32(*) (St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	gen_cue_mmc,
	send_cue_mmc,
	write_leadin_mmc,
	open_track_mmc,
	close_track_mmc,
	open_session_mmc,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	fixate_mmc,
	stats_mmc,
	blank_mmc,
	format_dummy,
	send_opc_mmc,
	opt1_mmc,
	opt2_mmc,
};*/

/*
 * Sony MMC CD-writer
 */
/*cdr_t	cdr_mmc_sony = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_SAO,
	372, 372,
	"mmc_cdr_sony",
	"generic SCSI-3/mmc   CD-R/CD-RW driver (Sony 928 variant)",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	init_mmc,
	getdisktype_mmc,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	next_wr_addr_mmc,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	gen_cue_mmc,
	send_cue_mmc,
	write_leadin_mmc,
	open_track_mmc,
	close_track_mmc,
	open_session_mmc,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	fixate_mmc,
	cmd_dummy,
	blank_mmc,
	format_dummy,
	send_opc_mmc,
	opt1_mmc,
	opt2_mmc,
};*/

/*
 * SCSI-3/mmc conformant CD-ROM drive
 */
/*static cdr_t	cdr_cd = {
	0, 0, 0,
	CDR_ISREADER|CDR_SWABAUDIO,
	0,
	CDR_CDRW_NONE,
	WM_NONE,
	372, 372,
	"mmc_cd",
	"generic SCSI-3/mmc   CD-ROM driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	cmd_dummy,
	drive_getdisktype,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scgp, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};*/

/*
 * Old pre SCSI-3/mmc CD drive
 */
/*cdr_t	cdr_oldcd = {
	0, 0, 0,
	CDR_ISREADER,
	0,
	CDR_CDRW_NONE,
	WM_NONE,
	372, 372,
	"scsi2_cd",
	"generic SCSI-2       CD-ROM driver",
	0,
	(dstat_t *)0,
	identify_mmc,
	drive_attach,
	cmd_dummy,
	drive_getdisktype,
	prdiskstatus_mmc,
	scsi_load,
	scsi_unload,
	buf_dummy,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scg, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset_philips,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};*/

/*
 * SCSI-3/mmc conformant CD, DVD or BE writer
 * Checks the current medium and then returns either cdr_mmc or cdr_dvd
 */
/*static cdr_t	cdr_cd_dvd = {
	0, 0, 0,
	CDR_SWABAUDIO,
	0,
	CDR_CDRW_ALL,
	WM_NONE,
	372, 372,
	"mmc_cd_dvd",
	"generic SCSI-3/mmc   CD/DVD/BD driver (checks media)",
	0,
	(dstat_t *)0,
	identify_mmc,
	attach_mmc,
	cmd_dummy,
	drive_getdisktype,
	no_diskstatus,
	scsi_load,
	scsi_unload,
	read_buff_cap,
	cmd_dummy,
	(__s32(*)(St_GrScsiIf *, cdr_t *, __s32))cmd_dummy,
	speed_select_mmc,
	select_secsize,
	(__s32(*)(St_GrScsiIf *scgp, track_t *, long *))cmd_ill,
	(__s32(*)(St_GrScsiIf *, __u32))cmd_ill,
	scsi_cdr_write,
	(__s32(*)(track_t *, void *, BOOL8))cmd_dummy,
	no_sendcue,
	(__s32(*)(St_GrScsiIf *, cdr_t *, track_t *))cmd_dummy,
	open_track_mmc,
	close_track_mmc,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	cmd_dummy,
	read_session_offset,
	(__s32(*)(St_GrScsiIf *scgp, cdr_t *, track_t *))cmd_dummy,
	cmd_dummy,
	blank_dummy,
	format_dummy,
	(__s32(*)(St_GrScsiIf *, caddr_t, __s32, __s32))NULL,
	cmd_dummy,
	cmd_dummy,
};*/

static __s32	toc2sess[] = {
	SES_DA_ROM,	/* CD-DA		 */
	SES_DA_ROM,	/* CD-ROM		 */
	SES_XA,		/* CD-ROM XA mode 1	 */
	SES_XA,		/* CD-ROM XA MODE 2	 */
	SES_CDI,	/* CDI			 */
	SES_DA_ROM,	/* Invalid - use default */
	SES_DA_ROM,	/* Invalid - use default */
};

static __s32	st2mode[] = {
	0,		/* 0			*/
	TM_DATA,	/* 1 ST_ROM_MODE1	*/
	TM_DATA,	/* 2 ST_ROM_MODE2	*/
	0,		/* 3			*/
	0,		/* 4 ST_AUDIO_NOPRE	*/
	TM_PREEM,	/* 5 ST_AUDIO_PRE	*/
	0,		/* 6			*/
	0,		/* 7			*/
};

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

