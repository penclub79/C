/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevOldCd.h>
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

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevOldCd::Cls_GrOddDevOldCd():
Cls_GrOddDevMmc()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_ISREADER;
		m_Flags2	= 0;
		m_Cdrw_Support	= CDR_CDRW_NONE;
		m_WrModeDef			= WM_NONE;
		m_SpeedDef	= 372;
		m_SpeedMax	= 372;
		m_DrName		= "scsi2_cd";
		m_DrText		= "generic SCSI-2 CD-ROM driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevOldCd::~Cls_GrOddDevOldCd()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevOldCd::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevMmc::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::attach(void* A_PtrScg)
{
		return drive_attach(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::init(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::getdisktype(void* A_PtrScg)
{
		return drive_getdisktype(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::prdiskstatus(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevMmc::prdiskstatus(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return buf_dummy(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::select_speed(void* A_PtrScg, int *speedp)
{
	// inherited
	return Cls_GrOddDevMmc::select_speed(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::gen_cue(track_t *trackp, void *vcuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::open_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::open_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::close_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevMmc::close_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset_philips(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::fixate(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::stats(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::blank(void* A_PtrScg, long addr, int blanktype)
{
	return blank_dummy(NULL, NULL, 0, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::format(void* A_PtrScg, int fmtflags)
{
		return format_dummy(NULL, NULL, 0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevOldCd::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
