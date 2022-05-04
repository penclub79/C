/*
	scsi-mmc
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrModes
#define	_EPC_GrModes

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/Odd/GrSCSI.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function
BOOL8	get_mode_params(St_GrScsiIf *scgp, __s32 page, __s8 *pagename,	unsigned __s8 *modep,
					unsigned __s8 *cmodep, unsigned __s8 *dmodep, unsigned __s8 *smodep, __s32 *lenp);
BOOL8	set_mode_params(St_GrScsiIf *scgp, __s8 *pagename, unsigned __s8 *modep,	__s32 len, __s32 save, __s32 secsize);

__s32		mode_sense(St_GrScsiIf	*scgp, unsigned __s8 *dp, __s32	cnt, __s32 page, __s32 pcf);
__s32		mode_sense_sg0(St_GrScsiIf *scgp, unsigned __s8 *dp, __s32 cnt, __s32 page, __s32 pcf);
__s32		mode_select_g0(St_GrScsiIf *scgp, unsigned __s8 *, __s32, __s32, __s32);
__s32		mode_sense_g1(St_GrScsiIf *scgp,	unsigned __s8	*dp, __s32 cnt,	__s32	page,	__s32	pcf);
__s32		mode_sense_g0(St_GrScsiIf *scgp, unsigned __s8 *dp, __s32 cnt, __s32 page, __s32 pcf);

//====================================================================
//out Enviroment conditional
#endif

