/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevBdR.h>
#include <Hw/OddV2/GrOddLib.h>
#include <Hw/OddV2/GrOddScg.h>
#include <GrDumyTool.h>
#include <GrDebug.h>

#ifdef LINUX_APP
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#endif

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
extern Cls_GrOddDevBase* V_PtrDevBd;
extern Cls_GrOddDevBase* V_PtrDevBdRom;
extern Cls_GrOddDevBase* V_PtrDevBdR;
extern Cls_GrOddDevBase* V_PtrDevBdRE;

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevBdR::Cls_GrOddDevBdR():
Cls_GrOddDevBd()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= 0;
		m_Flags2	= CDR2_NOCD|CDR2_BD;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_SAO;
		m_SpeedDef	= 1000;
		m_SpeedMax	= 1000;
		m_DrName		= "mmc_bdr";
		m_DrText		= "generic SCSI-3/mmc-3 BD-R driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevBdR::~Cls_GrOddDevBdR()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevBdR::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		return Cls_GrOddDevBd::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::attach(void* A_PtrScg)
{
		return Cls_GrOddDevBd::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::init(void* A_PtrScg)
{
		return init_bd(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::getdisktype(void* A_PtrScg)
{
		return getdisktype_bd(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::prdiskstatus(void* A_PtrScg)
{
		return prdiskstatus_bd(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::select_speed(void* A_PtrScg, int *speedp)
{
		return speed_select_bd(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return next_wr_addr_bdr(A_PtrScg, trackp, ap);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::open_track(void* A_PtrScg, track_t *trackp)
{
		return open_track_bd(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::close_track(void* A_PtrScg, track_t *trackp)
{
		return close_track_bd(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::close_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::abort_session(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::fixate(void* A_PtrScg, track_t *trackp)
{
		return fixate_bdr(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::stats(void* A_PtrScg)
{
		return stats_bd(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_bd(A_PtrScg, addr, blanktype);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return NULL;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevBdR::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
