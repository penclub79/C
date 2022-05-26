/*
	dev class type base 
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddDevMmc
#define _EPC_GrOddDevMmc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/OddV2/GrOddDef.h>
#include	<Hw/OddV2/GrOddDevBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrOddDevMmc : public Cls_GrOddDevBase
{
	private:
		int		m_CurSpeed;
	protected:
		BOOL	mmc_isplextor(void* A_PtrScg);
		void 	do_varirec_plextor(void* A_PtrScg);
		int		check_varirec_plextor(void* A_PtrScg);
		int 	drivemode_plextor(void* A_PtrScg, caddr_t bp, int cnt, int modecode, void *modeval);
		int		varirec_plextor(void* A_PtrScg,	BOOL on, int val);
		int		bpc_plextor(void* A_PtrScg, int mode, int *bpp);
		__u32	gigarec_mult(int code, __u32	val);
		int		gigarec_plextor(void* A_PtrScg, int val);
		int		do_gigarec_plextor(void* A_PtrScg);
		int		check_gigarec_plextor(void* A_PtrScg);
		int		speed_rd_plextor(void* A_PtrScg, BOOL do_speedrd);
		int		ss_hide_plextor(void* A_PtrScg, BOOL do_ss, BOOL do_hide);
		int		set_audiomaster_yamaha(void* A_PtrScg, Cls_GrOddDevBase *dp, BOOL keep_mode);
		int		waitfix_mmc(void* A_PtrScg, int secs);
		int		get_speeds_plextor(void* A_PtrScg, int *selp,	int	*maxp, int *lastp);
		void	fillcue(struct mmc_cue *cp,	int	ca, int	tno, int idx,	int	dataform,	int	scms,	msf_t	*mp);
		int		mmc_set_speed(void* A_PtrScg, int readspeed, int writespeed, int rotctl);
		BOOL	mmc_isyamaha(void* A_PtrScg);
		int		force_speed_yamaha(void* A_PtrScg, int readspeed, int writespeed);
		int		check_powerrec_plextor(void* A_PtrScg);
		int		powerrec_plextor(void* A_PtrScg, BOOL	do_powerrec);

		int		drivemode2_plextor(void* A_PtrScg, caddr_t	bp,	int	cnt, int modecode, void	*modeval);
		int		do_tattoo_yamaha(void* A_PtrScg, FILE	*f);
		BOOL	get_tattoo_yamaha(void* A_PtrScg, BOOL print, __u32 *irp, __u32 *orp);
		int		yamaha_write_buffer(void* A_PtrScg, int mode, int bufferid, long offset,
			long parlen, void *buffer, long buflen);
		int		plextor_enable(void* A_PtrScg);
		int		plextor_disable(void* A_PtrScg);
		int		plextor_getauth(void* A_PtrScg, void *dp, int cnt);
		int		plextor_setauth(void* A_PtrScg, void *dp, int cnt);
		int		check_ss_hide_plextor(void* A_PtrScg);
		int		check_speed_rd_plextor(void* A_PtrScg);

		int		get_atip(void* A_PtrScg, struct atipinfo *atp);
		void	di_to_dstat(struct disk_info	*dip,	dstat_t	*dsp);
	public:
		Cls_GrOddDevMmc();
		virtual	~Cls_GrOddDevMmc();

		virtual Cls_GrOddDevBase* Identify(void* A_PtrScg, Cls_GrOddDevBase* dp);	/* identify drive */

		virtual int	attach(void* A_PtrScg);			/* init error decoding etc*/
		virtual int	init(void* A_PtrScg);			/* init drive to useful deflts */
		virtual int	getdisktype(void* A_PtrScg);			/* get disk type */
		virtual int	prdiskstatus(void* A_PtrScg);			/* print disk status */
		virtual int	load(void* A_PtrScg);			/* load disk */
		virtual int	cdr_unload(void* A_PtrScg);			/* unload disk */
		virtual int	buffer_cap(void* A_PtrScg, long *sizep, long *freep);	/* read buffer capacity */
		virtual int	check_recovery(void* A_PtrScg);			/* check if recover is needed */
		virtual int	recover(void* A_PtrScg, int track);		/* do recover */
		virtual int	select_speed(void* A_PtrScg, int *speedp);	/* set recording speed & dummy write */
		virtual int	set_secsize(void* A_PtrScg, int secsize);		/* set sector size */
		virtual int	next_wr_address(void* A_PtrScg, track_t *trackp, long *ap);	/* get next writable addr. */
		virtual int	reserve_track(void* A_PtrScg, __u32 len);			/* reserve track for future use */
		virtual int	write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast);
		virtual int	gen_cue(track_t *trackp, void *vcuep, BOOL needgap);		/* generate cue sheet */
		virtual int	send_cue(void* A_PtrScg, track_t *trackp);	/* send cue sheet */
		virtual int	write_leadin(void* A_PtrScg, track_t *trackp);	/* write leadin */
		virtual int	open_track(void* A_PtrScg, track_t *trackp);	/* open new track */
		virtual int	close_track(void* A_PtrScg, track_t *trackp);	/* close written track */
		virtual int	open_session(void* A_PtrScg, track_t *trackp);	/* open new session */
		virtual int	close_session(void* A_PtrScg);			/* really needed ??? */
		virtual int	abort_session(void* A_PtrScg);			/* abort current write */
		virtual int	session_offset(void* A_PtrScg, long *soff);			/* read session offset*/
		virtual int	fixate(void* A_PtrScg, track_t *trackp);	/* write toc on disk */
		virtual int	stats(void* A_PtrScg);			/* final statistics printing*/
		virtual int	blank(void* A_PtrScg, long addr, int blanktype);	/* blank something */
		virtual int	format(void* A_PtrScg, int fmtflags);	/* format media */
		virtual int	opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc);	/* Do OPC */
		virtual int	opt1(void* A_PtrScg);			/* do early option processing*/
		virtual int	opt2(void* A_PtrScg);			/* do late option processing */
};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional

#endif