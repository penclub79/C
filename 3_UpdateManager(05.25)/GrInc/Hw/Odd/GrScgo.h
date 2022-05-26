/*
	cdrecord class
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrScgo
#define	_EPC_GrScgo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include 	<Hw/Odd/GrSCSI.h>

//====================================================================
//constance

#define	LF_ATA		0x01		/* Using /dev/hd* ATA interface	*/

#define	scglocal(p)	(((p)->local))

#define	SCGO_SEND(scgp)				(*(scgp)->ops->scgo_send)(scgp)
#define	SCGO_VERSION(scgp, what)		(*(scgp)->ops->scgo_version)(scgp, what)
#define	SCGO_HELP(scgp, f)			(*(scgp)->ops->scgo_help)(scgp, f)
#define	SCGO_OPEN(scgp, device)			(*(scgp)->ops->scgo_open)(scgp, device)
#define	SCGO_CLOSE(scgp)			(*(scgp)->ops->scgo_close)(scgp)
#define	SCGO_MAXDMA(scgp, amt)			(*(scgp)->ops->scgo_maxdma)(scgp, amt)
#define	SCGO_GETBUF(scgp, amt)			(*(scgp)->ops->scgo_getbuf)(scgp, amt)
#define	SCGO_FREEBUF(scgp)			(*(scgp)->ops->scgo_freebuf)(scgp)
#define	SCGO_NUMBUS(scgp)			(*(scgp)->ops->scgo_numbus)(scgp)
#define	SCGO_HAVEBUS(scgp, busno)		(*(scgp)->ops->scgo_havebus)(scgp, busno)
#define	SCGO_FILENO(scgp, busno, tgt, tlun)	(*(scgp)->ops->scgo_fileno)(scgp, busno, tgt, tlun)
#define	SCGO_INITIATOR_ID(scgp)			(*(scgp)->ops->scgo_initiator_id)(scgp)
//#define	SCGO_ISATAPI(scgp)			(*(scgp)->ops->scgo_isatapi)(scgp)
#define	SCGO_RESET(scgp, what)			(*(scgp)->ops->scgo_reset)(scgp, what)


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function
__s32 	scgo_open(St_GrScsiIf *scgp,	__s8	*device);
void 	sg_clearnblock(__s32 f);
BOOL8 	sg_setup(St_GrScsiIf	*scgp, __s32 f,	__s32	busno, __s32 tgt, __s32	tlun,	__s32	ataidx);
BOOL8	sg_mapdev(St_GrScsiIf *scgp, __s32	f, __s32 *busp,	__s32	*tgtp, __s32 *lunp,	__s32	*chanp,	__s32	*inop, __s32 ataidx);
__s32		sg_mapbus(St_GrScsiIf *scgp, __s32 busno, __s32 ino);
void	sg_initdev(St_GrScsiIf *scgp, __s32 f);
void	sg_settimeout(__s32 f, __s32 timeout);

__s32		scgo_fileno(St_GrScsiIf *scgp,	__s32	busno, __s32 tgt,	__s32	tlun);

__s32		scgo_send(St_GrScsiIf *scgp);
__s8*	scgo_version(St_GrScsiIf	*scgp, __s32 what);
__s32		scgo_help(St_GrScsiIf *scgp,	FILE	*f);
long	scgo_maxdma(St_GrScsiIf *scgp,	long amt);
__s32		scgo_close(St_GrScsiIf	*scgp);
void*	scgo_getbuf(St_GrScsiIf *scgp, long amt);
void	scgo_freebuf(St_GrScsiIf	*scgp);
__s32		scgo_numbus(St_GrScsiIf *scgp);
BOOL8	scgo_havebus(St_GrScsiIf	*scgp, __s32 busno);
__s32		scgo_initiator_id(St_GrScsiIf *scgp);
//__s32		scgo_isatapi(St_GrScsiIf	*scgp);
__s32		scgo_reset(St_GrScsiIf	*scgp, __s32 what);

__s32	scgo_dsend(St_GrScsiIf *scgp);
__s8 *	scgo_dversion(St_GrScsiIf *scgp, __s32 what);
__s32	scgo_dhelp(St_GrScsiIf *scgp, FILE *f);
__s32	scgo_nohelp(St_GrScsiIf *scgp, FILE *f);
__s32	scgo_ropen(St_GrScsiIf *scgp, __s8 *device);
__s32	scgo_dopen(St_GrScsiIf *scgp, __s8 *device);
__s32	scgo_dclose(St_GrScsiIf *scgp);
long	scgo_dmaxdma(St_GrScsiIf *scgp, long amt);
void *	scgo_dgetbuf(St_GrScsiIf *scgp, long amt);
void	scgo_dfreebuf(St_GrScsiIf *scgp);
__s32	scgo_dnumbus(St_GrScsiIf *scgp);
BOOL8	scgo_dhavebus(St_GrScsiIf *scgp, __s32 busno);
__s32	scgo_dfileno(St_GrScsiIf *scgp, __s32 busno, __s32 tgt, __s32 tlun);
__s32	scgo_dinitiator_id(St_GrScsiIf *scgp);
__s32	scgo_disatapi(St_GrScsiIf *scgp);
__s32	scgo_dreset(St_GrScsiIf *scgp, __s32 what);

__s32	sg_rwsend(St_GrScsiIf *scgp);
long	sg_raisedma(St_GrScsiIf *scgp, long newmax);

//====================================================================
//out Enviroment conditional
#endif

