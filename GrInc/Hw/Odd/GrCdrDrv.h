/*
	scsi cdr
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrCdrDrv
#define	_EPC_GrCdrDrv

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/Odd/GrScsiCdr.h>
#include 	<Hw/Odd/GrDrvMmc.h>
#include 	<Hw/Odd/GrDrvDvdPlus.h>

//====================================================================
//constance

//====================================================================
//global macro
__s32		cmd_dummy(St_GrScsiIf *scgp,	cdr_t	*dp);
__s32		cmd_ill(St_GrScsiIf	*scgp);
__s32		format_dummy(St_GrScsiIf *scgp, cdr_t *, __s32 fmtflags);
__s32		blank_dummy(St_GrScsiIf *scgp, cdr_t *, long addr, __s32 blanktype);
__s32		buf_dummy(St_GrScsiIf *scgp,	long	*sp,	long	*fp);
//BOOL8	set_cdrcmds(__s8 *name,	cdr_t	**dpp);
cdr_t*	get_cdrcmds(St_GrScsiIf *scgp);
__s32		drive_getdisktype(St_GrScsiIf *scgp, cdr_t *dp);
__s32		no_sendcue(St_GrScsiIf	*scgp, cdr_t	*dp, track_t	*trackp);
__s32		drive_attach(St_GrScsiIf *scgp, cdr_t *);
__s32		no_diskstatus(St_GrScsiIf	*scgp,	cdr_t	*dp);
__s32		blank_simul(St_GrScsiIf *scgp,	cdr_t	*dp, long	addr,	__s32	blanktype);

//====================================================================
//global type
/*
extern	cdr_t	cdr_oldcd;
extern	cdr_t	cdr_cd;
extern	cdr_t	cdr_mmc;
extern	cdr_t	cdr_mmc_sony;
extern	cdr_t	cdr_dvd;
extern	cdr_t	cdr_dvdplus;
extern	cdr_t	cdr_dvdplusr;
extern	cdr_t	cdr_dvdplusrw;
extern	cdr_t	cdr_bd;
extern	cdr_t	cdr_bdrom;
extern	cdr_t	cdr_bdr;
extern	cdr_t	cdr_bdre;
extern	cdr_t	cdr_cd_dvd;
extern	cdr_t	cdr_philips_cdd521O;
extern	cdr_t	cdr_philips_dumb;
extern	cdr_t	cdr_philips_cdd521;
extern	cdr_t	cdr_philips_cdd522;
extern	cdr_t	cdr_tyuden_ew50;
extern	cdr_t	cdr_kodak_pcd600;
extern	cdr_t	cdr_pioneer_dw_s114x;
extern	cdr_t	cdr_plasmon_rf4100;
extern	cdr_t	cdr_yamaha_cdr100;
extern	cdr_t	cdr_sony_cdu924;
extern	cdr_t	cdr_ricoh_ro1060;
extern	cdr_t	cdr_ricoh_ro1420;
extern	cdr_t	cdr_teac_cdr50;
extern	cdr_t	cdr_cw7501;
extern	cdr_t	cdr_cdr_simul;
extern	cdr_t	cdr_dvd_simul;
extern	cdr_t	cdr_bd_simul;
*/

extern	cdr_t	cdr_cd_dvd;
extern	cdr_t	cdr_dvdplus;
extern	cdr_t	cdr_dvdplusr;
extern	cdr_t	cdr_dvdplusrw;
extern	cdr_t	cdr_dvd;
extern	cdr_t	cdr_mmc;
extern	cdr_t	cdr_mmc_sony;
extern	cdr_t	cdr_cd;
extern	cdr_t	cdr_dvdplusrw;
extern	cdr_t	cdr_oldcd;

static cdr_t	*drivers[] = {
	&cdr_cd_dvd,
	&cdr_dvdplus,
	&cdr_dvdplusr,
	&cdr_dvdplusrw,
	&cdr_dvd,
	&cdr_mmc,
	&cdr_mmc_sony,
	&cdr_cd,
	&cdr_oldcd,
	(cdr_t *)NULL,
};

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

