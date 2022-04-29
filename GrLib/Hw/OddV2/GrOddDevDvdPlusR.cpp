/*
	dev class type base 
*/

//====================================================================
// uses
#include <Hw/OddV2/GrOddDevDvdPlusR.h>
#include <Hw/OddV2/GrOddScg.h>
#include <Hw/OddV2/GrOddLib.h>

#ifdef LINUX_APP
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
Cls_GrOddDevDvdPlusR::Cls_GrOddDevDvdPlusR():
Cls_GrOddDevDvdPlusBase()
{
		m_Dev		= 0;
		m_CmdFlags		= 0;
		m_CmdFlags2		= 0;
		m_Flags		= CDR_DVD|CDR_SWABAUDIO;
		m_Flags2	= CDR2_NOCD;
		m_Cdrw_Support	= CDR_CDRW_ALL;
		m_WrModeDef			= WM_SAO;
		m_SpeedDef	= 1000;
		m_SpeedMax	= 1000;
		m_DrName		= "mmc_dvdplusr";
		m_DrText		= "generic SCSI-3/mmc-3 DVD+R driver";
		m_CdCap			= NULL;
		m_Dstat			= NULL;
}
//--------------------------------------------------------------------
Cls_GrOddDevDvdPlusR::~Cls_GrOddDevDvdPlusR()
{

}
//--------------------------------------------------------------------
Cls_GrOddDevBase* Cls_GrOddDevDvdPlusR::Identify(void* A_PtrScg, Cls_GrOddDevBase* dp)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::Identify(A_PtrScg, dp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::attach(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::attach(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::init(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::init(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::getdisktype(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::getdisktype(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::prdiskstatus(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::prdiskstatus(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::load(void* A_PtrScg)
{
		return scsi_load(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::cdr_unload(void* A_PtrScg)
{
		return scsi_unload(A_PtrScg, this);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::buffer_cap(void* A_PtrScg, long *sizep, long *freep)
{
		return read_buff_cap(A_PtrScg, sizep, freep);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::check_recovery(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::recover(void* A_PtrScg, int track)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::select_speed(void* A_PtrScg, int *speedp)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::select_speed(A_PtrScg, speedp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::set_secsize(void* A_PtrScg, int secsize)
{
		return select_secsize(A_PtrScg, secsize);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::next_wr_address(void* A_PtrScg, track_t *trackp, long *ap)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	struct disk_info	di;
	struct rzone_info	rz;
	int			tracks;
	long			next_addr = -1;

	if (trackp == 0) {
		memset((caddr_t)&di, '\0', sizeof (di));
		if (get_diskinfo(scgp, &di, sizeof (di)) < 0)
			return (-1);

		tracks = di.last_track_ls + di.last_track_ls_msb * 256;
		memset((caddr_t)&rz, '\0', sizeof (rz));
		if (get_trackinfo(scgp, (caddr_t)&rz, TI_TYPE_TRACK, tracks, sizeof (rz)) < 0)
			return (-1);
		if (!rz.nwa_v)
			return (-1);
		next_addr = a_to_4_byte(rz.next_recordable_addr);
		if (ap)
			*ap = next_addr;
		return (0);
	}
	if (trackp->track <= 1) {
		memset((caddr_t)&rz, '\0', sizeof (rz));
		read_rzone_info(scgp, (caddr_t)&rz, sizeof (struct rzone_info));
		m_NextAddr = a_to_4_byte(rz.next_recordable_addr);
	}
	if (ap)
		*ap = m_NextAddr;
	return (0);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::reserve_track(void* A_PtrScg, __u32 len)
{
		return cmd_ill(NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::write_trackdata(void* A_PtrScg, caddr_t buf, long daddr, long bytecnt, int seccnt, BOOL islast)
{
		return scsi_cdr_write(A_PtrScg, buf, daddr, bytecnt, seccnt, islast);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::gen_cue(track_t *trackp, void *cuep, BOOL needgap)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::send_cue(void* A_PtrScg, track_t *trackp)
{
		return no_sendcue(NULL, NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::write_leadin(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::open_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::open_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::close_track(void* A_PtrScg, track_t *trackp)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::close_track(A_PtrScg, trackp);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::open_session(void* A_PtrScg, track_t *trackp)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::close_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::abort_session(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::session_offset(void* A_PtrScg, long *soff)
{
		return read_session_offset(A_PtrScg, soff);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::fixate(void* A_PtrScg, track_t *trackp)
{
	Cls_GrOddScg* scgp = (Cls_GrOddScg*) A_PtrScg;
	int	oldtimeout = scgp->m_TimeOut;
	int	ret = 0;
	int	key = 0;
	int	code = 0;
	int	trackno;
	int	i;
#define	MAX_TRIES	15

	if (scgp->m_TimeOut < 1000)
		scgp->TimeOut(1000);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_flush_cache(scgp, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {

				scgp->TimeOut(oldtimeout);
				return (-1);
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scgp->Scg_Sense_Key();
	code = scgp->Scg_Sense_Code();
	waitformat(scgp, 600);

	trackno = trackp->trackno;
	if (trackno <= 0)
		trackno = 1;
	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_close_tr_session(scgp, CL_TYPE_TRACK, trackno, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {
				break;
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scgp->Scg_Sense_Key();
	code = scgp->Scg_Sense_Code();
	waitformat(scgp, 600);

	for (i = 0; i <= MAX_TRIES; i++) {
		if (scsi_close_tr_session(scgp, 0x06, 0, TRUE) < 0) {
			if (!scsi_in_progress(scgp) || i >= MAX_TRIES) {

				break;
			}
			sleep(1);
		} else {
			break;
		}
	}
	key = scgp->Scg_Sense_Key();
	code = scgp->Scg_Sense_Code();
	waitformat(scgp, 600);

	scgp->TimeOut(oldtimeout);
	return (ret);
#undef	MAX_TRIES
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::stats(void* A_PtrScg)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::stats(A_PtrScg);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::blank(void* A_PtrScg, long addr, int blanktype)
{
	// inherited
	return Cls_GrOddDevDvdPlusBase::blank(A_PtrScg, addr, blanktype);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::format(void* A_PtrScg, int fmtflags)
{
		// inherited
		return Cls_GrOddDevDvdPlusBase::format(A_PtrScg, fmtflags);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::opc(void* A_PtrScg, caddr_t bp, int cnt, int doopc)
{
		return 0;
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::opt1(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
int Cls_GrOddDevDvdPlusR::opt2(void* A_PtrScg)
{
		return cmd_dummy(NULL, NULL);
}
//--------------------------------------------------------------------
