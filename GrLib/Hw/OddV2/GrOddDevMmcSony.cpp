/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevMmcSony.h>
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddLib.h>

//====================================================================
//local const

//====================================================================
//local macro

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
extern Cls_GrOddDevBase* V_PtrDevMmc;
extern Cls_GrOddDevBase* V_PtrDevMmcSony;
extern Cls_GrOddDevBase* V_PtrDevCd;
extern Cls_GrOddDevBase* V_PtrDevOldCd;
extern Cls_GrOddDevBase* V_PtrDevDvd;
extern Cls_GrOddDevBase* V_PtrDevDvdPlus;
extern Cls_GrOddDevBase* V_PtrDevDvdPlusR;
extern Cls_GrOddDevBase* V_PtrDevDvdPlusRW;
extern Cls_GrOddDevBase* V_PtrCdDvd;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevMmcSony::Cls_GrOddDevMmcSony():
Cls_GrOddDevMmc()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_SWABAUDIO;
		m_Flags2	= 0;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_SAO;
		m_SpeedDef	= 372;
		m_SpeedMax	= 372;
		m_DrName		= "mmc_cdr_sony";
		m_DrText		= "generic St_GrScsiIf-3/mmc CD-R/CD-RW driver (Sony 928 variant)";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevMmcSony::~Cls_GrOddDevMmcSony()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevMmcSony::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevMmc::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::attach(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::init(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::init(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::getdisktype(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::getdisktype(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::prdiskstatus(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::prdiskstatus(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::select_speed(void* A_PtrScg, int *speedp)
{
		// inherited
		return Cls_GrOddDevMmc::select_speed(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		// inherited
		return Cls_GrOddDevMmc::next_wr_address(A_PtrScg, trackp, ap);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		// inherited
		return Cls_GrOddDevMmc::gen_cue(trackp, vcuep, needgap);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::send_cue(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::send_cue(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::write_leadin(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::write_leadin(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::open_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::open_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::close_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::close_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::open_session(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::open_session(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::fixate(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::fixate(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		// inherited
		return Cls_GrOddDevMmc::opc(A_PtrScg, bp, cnt, doopc);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::opt1(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::opt1(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevMmcSony::opt2(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::opt2(A_PtrScg);
}
//--------------------------------------------------------------------
