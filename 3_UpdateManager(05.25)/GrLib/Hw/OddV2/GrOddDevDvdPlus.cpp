/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevDvdPlus.h>
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
Cls_GrOddDevDvdPlus::Cls_GrOddDevDvdPlus():
Cls_GrOddDevDvdPlusBase()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_DVD|CDR_SWABAUDIO;
		m_Flags2	= CDR2_NOCD;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_NONE;
		m_SpeedDef	= 1000;
		m_SpeedMax	= 1000;
		m_DrName		= "mmc_dvdplus";
		m_DrText		= "generic SCSI-3/mmc-3 DVD+R/DVD+RW driver (checks media)";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevDvdPlus::~Cls_GrOddDevDvdPlus()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevDvdPlus::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::attach(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::init(void* A_PtrScg)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::getdisktype(void* A_PtrScg)
{
		return drive_getdisktype(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::prdiskstatus(void* A_PtrScg)
{
		return no_diskstatus(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::select_speed(void* A_PtrScg, int *speedp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::open_track(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::close_track(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlus::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
