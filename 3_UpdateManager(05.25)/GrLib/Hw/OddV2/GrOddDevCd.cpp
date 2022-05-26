/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevCd.h>
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
Cls_GrOddDevCd::Cls_GrOddDevCd():
Cls_GrOddDevMmc()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_ISREADER|CDR_SWABAUDIO;
		m_Flags2	= 0;
		m_Cdrw_Support	= CDR_CDRW_NONE;
		m_WrModeDef			= WM_NONE;
		m_SpeedDef	= 372;
		m_SpeedMax	= 372;
		m_DrName		= "mmc_cd";
		m_DrText		= "generic SCSI-3/mmc CD-ROM driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevCd::~Cls_GrOddDevCd()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevCd::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevMmc::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::attach(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::init(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::getdisktype(void* A_PtrScg)
{
		return drive_getdisktype(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::prdiskstatus(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::prdiskstatus(A_PtrScg);	
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(A_PtrScg, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::select_speed(void* A_PtrScg, int *speedp)
{
		// inherited
		return Cls_GrOddDevMmc::select_speed(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return cmd_ill(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		int Tv_Result;
		Tv_Result = scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
		return Tv_Result;
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::open_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::open_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::close_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::close_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::blank(void* A_PtrScg, long addr, int blanktype)
{
		return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevCd::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
