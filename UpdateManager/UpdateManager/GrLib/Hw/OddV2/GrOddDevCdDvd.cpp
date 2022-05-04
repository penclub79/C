/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevCdDvd.h>
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddLib.h>
#include <GrDebug.h>

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
Cls_GrOddDevCdDvd::Cls_GrOddDevCdDvd():
Cls_GrOddDevMmc()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_SWABAUDIO;
		m_Flags2	= 0;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_NONE;
		m_SpeedDef	= 372;
		m_SpeedMax	= 372;
		m_DrName		= "mmc_cd_dvd";
		m_DrText		= "generic SCSI-3/mmc CD/DVD/BD driver (checks media)";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevCdDvd::~Cls_GrOddDevCdDvd()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevCdDvd::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevMmc::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::attach(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::init(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::getdisktype(void* A_PtrScg)
{
		return drive_getdisktype(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::prdiskstatus(void* A_PtrScg)
{
		return no_diskstatus(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::select_speed(void* A_PtrScg, int *speedp)
{
		// inherited
		return Cls_GrOddDevMmc::select_speed(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);;
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		DbgMsgPrint("Cls_GrOddDevCdDvd::write_trackdata\n");
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::gen_cue(track_t *trackp, void *cuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::open_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::open_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::close_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::close_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCdDvd::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
