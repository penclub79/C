/*
	dev class type base 
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddDevBd
#define _EPC_GrOddDevBd

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
class Cls_GrOddDevBd : public Cls_GrOddDevBase
{
	private:
		long m_DvdNextAddr;
	protected:
		void	di_to_dstat(disk_info* dip, dstat_t* dsp);
		int		init_bd(void* A_PtrScg);
		int		getdisktype_bd(void* A_PtrScg);
		int		prdiskstatus_bd(void* A_PtrScg);
		int		next_wr_addr_bdre(void* A_PtrScg, track_t* trackp, long* ap);
		int		next_wr_addr_bdr(void* A_PtrScg, track_t* trackp, long* ap);
		int		open_track_bd(void* A_PtrScg, track_t* trackp);
		unsigned int rzone_size(track_t *trackp);
		int		close_track_bd(void* A_PtrScg, track_t *trackp);
		int		fixate_bdre(void* A_PtrScg, track_t *trackp);
		int		fixate_bdr(void* A_PtrScg, track_t *trackp);
		int		blank_bd(void* A_PtrScg, long addr, int blanktype);
		int		format_bd(void* A_PtrScg, int	fmtflags);
		int		waitformat_bd(void* A_PtrScg, int secs);
		int		stats_bd(void* A_PtrScg);
		int		format_unit(void* A_PtrScg,	void	*fmt,	int	length,	int	list_format, int dgdl, int interlv, int	pattern, int	timeout);
		int		speed_select_bd(void* A_PtrScg,	int* speedp);
	public:
		Cls_GrOddDevBd();
		virtual	~Cls_GrOddDevBd();

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