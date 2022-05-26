/*
	dev class type base 
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef _EPC_GrOddDevBase
#define _EPC_GrOddDevBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/OddV2/GrOddDef.h>
#include	<Hw/OddV2/GrOddLib.h>
//#include	<Hw/OddV2/GrOddScg.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrOddDevBase
{
	protected:
		int		m_Dev;
		__u32		m_CmdFlags;
		__u32		m_CmdFlags2;
		__u32		m_Flags;
		__u32		m_Flags2;
		unsigned char m_Cdrw_Support;
		unsigned char m_WrModeDef;
		unsigned short m_SpeedDef;
		unsigned short m_SpeedMax;

		char* m_DrName;
		char* m_DrText;
		struct cd_mode_page_2A* m_CdCap;

		dstat_t	*m_Dstat;
	public:
		Cls_GrOddDevBase();
		virtual	~Cls_GrOddDevBase();

		virtual void SetFlags(int A_Flags);
		virtual __u32 GetFlags();
		virtual void SetFlags2(int A_Flags);
		virtual __u32 GetFlags2();
		virtual void SetCmdFlags(int A_CmdFlags);
		virtual __u32 GetCmdFlags();
		virtual void SetCmdFlags2(int A_CmdFlags);
		virtual __u32 GetCmdFlags2();
		virtual dstat_t* GetDstat();
		virtual void SetDstat(dstat_t* A_PtrDstat);
		virtual unsigned short GetSpeedMax();
		virtual void SetSpeedMax(unsigned short A_SpeedMax);
		virtual unsigned short GetSpeedDef();
		virtual void SetSpeedDef(unsigned short A_SpeedDef);
		virtual unsigned char GetCdrwSupport();
		virtual void SetCdrwSupport(unsigned char A_CdrwSupport);
		char*	GetDevName();

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