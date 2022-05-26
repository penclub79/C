/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevBase.h>
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

//====================================================================
//--------------------------------------------------------------------
Cls_GrOddDevBase::Cls_GrOddDevBase()
{
		m_DrName	= "None";
}
//--------------------------------------------------------------------
Cls_GrOddDevBase::~Cls_GrOddDevBase()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevBase::Identify(void* A_PtrScg, Cls_GrOddDevBase* A_PtrDevObj)
{
		return NULL;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::attach(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::init(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::getdisktype(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::prdiskstatus(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::load(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::cdr_unload(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::check_recovery(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::recover(void* A_PtrScg, int track)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::select_speed(void* A_PtrScg, int *speedp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::set_secsize(void* A_PtrScg, int secsize)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::reserve_track(void* A_PtrScg, __u32 len)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::gen_cue(track_t *trackp, void *cuep, BOOL needgap)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::send_cue(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::open_track(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::close_track(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::open_session(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::close_session(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::abort_session(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::session_offset(void* A_PtrScg, long *soff)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::fixate(void* A_PtrScg, track_t *trackp)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::stats(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::blank(void* A_PtrScg, long addr, int blanktype)
{
	return -1;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::format(void* A_PtrScg, int fmtflags)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::opt1(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevBase::opt2(void* A_PtrScg)
{
		return 0;
}
//--------------------------------------------------------------------
__u32 Cls_GrOddDevBase::GetFlags()
{
		return m_Flags;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetFlags(int A_Flags)
{
		m_Flags		= A_Flags;
}
//--------------------------------------------------------------------
__u32 Cls_GrOddDevBase::GetFlags2()
{
		return m_Flags2;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetFlags2(int A_Flags)
{
		m_Flags2	= A_Flags;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetCmdFlags(int A_CmdFlags)
{
		m_CmdFlags	= A_CmdFlags;
}
//--------------------------------------------------------------------
__u32 Cls_GrOddDevBase::GetCmdFlags()
{
		return m_CmdFlags;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetCmdFlags2(int A_CmdFlags)
{
	m_CmdFlags2	= A_CmdFlags;
}
//--------------------------------------------------------------------
__u32 Cls_GrOddDevBase::GetCmdFlags2()
{
		return m_CmdFlags2;
}
//--------------------------------------------------------------------
dstat_t* Cls_GrOddDevBase::GetDstat()
{
		return m_Dstat;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetDstat(dstat_t* A_PtrDstat)
{
		m_Dstat = A_PtrDstat;
}
//--------------------------------------------------------------------
unsigned short Cls_GrOddDevBase::GetSpeedMax()
{
		return m_SpeedMax;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetSpeedMax(unsigned short A_SpeedMax)
{
		m_SpeedMax	= A_SpeedMax;
}
//--------------------------------------------------------------------
unsigned short Cls_GrOddDevBase::GetSpeedDef()
{
		return m_SpeedDef;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetSpeedDef(unsigned short A_SpeedDef)
{
		m_SpeedDef = A_SpeedDef;
}
//--------------------------------------------------------------------
unsigned char Cls_GrOddDevBase::GetCdrwSupport()
{
		return m_Cdrw_Support;
}
//--------------------------------------------------------------------
void Cls_GrOddDevBase::SetCdrwSupport(unsigned char A_CdrwSupport)
{
		m_Cdrw_Support = A_CdrwSupport;
}
//--------------------------------------------------------------------
char*	Cls_GrOddDevBase::GetDevName()
{
		return m_DrName;
}
//--------------------------------------------------------------------
