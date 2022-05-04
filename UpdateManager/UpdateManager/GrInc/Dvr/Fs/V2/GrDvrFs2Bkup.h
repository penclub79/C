/*
	GAUSE DVR file system v2 backup

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Bkup
#define	_EPC_GrDvrFs2Bkup

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V2/GrDvrFs2Dmux.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance
#define E_GrDvrFs2BkupCmdQueSize		0x1000

#define E_GrDvrFs2BkupDecOk					0		/* decode success */
#define E_GrDvrFs2BkupDecBusy				1		/* decoder busy */
#define E_GrDvrFs2BkupDecFail				2		/* decode failure */

#define E_GrDvrFs2BkupTimeGapLimite	5		/* 5 sec time gap limit */

#define E_GrDvrFs2BkupRunModeWait		0
#define E_GrDvrFs2BkupRunModeSeek		1
#define E_GrDvrFs2BkupRunModePlay		2
#define E_GrDvrFs2BkupRunModeNext		3		/* next frame */
#define E_GrDvrFs2BkupRunModePrev		4		/* prev frame */

#define E_GrDvrFs2BkupCmdDoPlay			1
#define E_GrDvrFs2BkupCmdJmpByTime	2		/* Dw = time, Bt = seek after play */
#define E_GrDvrFs2BkupCmdJmpByHndl	3		/* Hndl = handle, Bt = seek after play */
#define E_GrDvrFs2BkupCmdJmpByCnt		4		/* Dw = frame count, Bt = seek after play */
#define E_GrDvrFs2BkupCmdJmpBegin		5		/* Bt = seek after play */
#define E_GrDvrFs2BkupCmdJmpEnd			6		/* Bt = seek after play */
#define E_GrDvrFs2BkupCmdDirSet			7	
#define E_GrDvrFs2BkupCmdVdoChSet		8		/* Wd = channel map  */
#define E_GrDvrFs2BkupCmdAdoChSet		9		/* Wd = channel map  */
#define E_GrDvrFs2BkupCmdSpeedSet		10	/* Dw = speed  */
#define E_GrDvrFs2BkupCmdStepNext		11
#define E_GrDvrFs2BkupCmdStepPrev		12
#define E_GrDvrFs2BkupCmdDevSet			13	/* Bt = record device type */
#define E_GrDvrFs2BkupCmdDoStop			14	
#define E_GrDvrFs2BkupCmdIfrmOnly		15	/* Wd = channel map */
#define E_GrDvrFs2BkupCmdNoTimeSync	16	/* Bt = no time sync mode set */

#define E_GrDvrFs2BkupSpeedMjBase		4

#define E_GrDvrFs2BkupSpeedMax			6

//	#define DAMGI_DBG

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrFs2BkupChItem 
{
	BOOL8	IsVdoAble;
	BOOL8	IsAdoAble;

	// runtime
	BOOL8	IsPable;			// video P-frame able
	__u8	SeqId;

}	*Ptr_GrDvrFs2BkupChItem;


//====================================================================
//class
class	Cls_GrDvrFs2Bkup	:	public	Cls_GrDvrFs2Dmux
{
	protected:
		// property
		//__u32		m_HndlTimer;

		BOOL8		m_IsBackup;
		Def_GrTime	m_BkupTimeSt;
		Def_GrTime	m_BkupTimeEd;
		Def_GrTime	m_BkupTimePrgs;

		St_GrDvrFs2BkupChItem	m_ChTbl[E_GrDvrMaxChCnt];

		// method
		void	LcVdoChEnSet( __u16 A_Map );
		void	LcAdoChEnSet( __u16 A_Map );
		void	LcPableReset( void );

		// virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
		// virtual	void	RtlTimer( __u32 A_Hndl );
	public:
		Cls_GrDvrFs2Bkup( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs2Mng* A_FsMng, Ptr_GrDvrFs2SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe );
		virtual		~Cls_GrDvrFs2Bkup();

		// info
		__u8	BkupRecDevTypeGet( void );
		BOOL8	IsBackup( void );
		Def_GrTime	BkupTimeProgress( void );
		Def_GrTime	BkupTimeStart( void );
		Def_GrTime	BkupTimeEnd( void );

		// control
		BOOL8	BkupStart( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u8 A_DevType, __u16 A_VdoChMap, __u16 A_AdoChMap );
		Ptr_GrDvrAvHd	BkupFrameGet( void );
		BOOL8	BkupGoNext( void );

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


