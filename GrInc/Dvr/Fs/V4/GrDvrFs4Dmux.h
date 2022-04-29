/*
	GAUSS DVR file system v4 DEMUX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs4Dmux
#define	_EPC_GrDvrFs4Dmux

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V4/GrDvrFs4Mng.h>
	
//====================================================================
//constance

#define E_GrDvrFs4DmuxStrmLimite			0x80000

#define	E_GrDvrFs4DmuxTryDflt					32

#define	E_GrDvrFs4DmuxBufCnt					2

#define	E_GrDvrFs4DmuxModeAll					0
#define	E_GrDvrFs4DmuxModeInterlace		1
#define	E_GrDvrFs4DmuxModeRef2x				2
#define	E_GrDvrFs4DmuxModeRef4x				3
#define	E_GrDvrFs4DmuxModeCntSkip			4	/* frame count skip */
#define	E_GrDvrFs4DmuxModeTimeJmp			5
#define	E_GrDvrFs4DmuxModeByteJmp			6	/* realization later */

#define	E_GrDvrFs4DmuxRsltOk						0	/* success */
#define	E_GrDvrFs4DmuxRsltBusy					1	/* busy */
#define	E_GrDvrFs4DmuxRsltDataEmpty			2	/* data empty */
#define	E_GrDvrFs4DmuxRsltEndOfData			3	/* arrival end */
#define	E_GrDvrFs4DmuxRsltBadFrame			4	/* bad frame type */
#define	E_GrDvrFs4DmuxRsltUselessFrame	5	/* useless frame type */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs4DmuxChCtrl
{
	BOOL8	IsVdoAble;		// channel video able
	BOOL8	IsAdoAble;
	BOOL8	IsMtnAble;

	__u32	VdoFmt;
	__u8	VdoSigFps;
	__u8	VdoSkipCnt;
	__u8	VdoRefType;

	BOOL8	IsVdoCanP;
	__u32	VdoSkipPrgs;	// frame skip progress
	__u8	VdoSeqId;

}	*Ptr_GrDvrFs4DmuxChCtrl;

typedef struct St_GrDvrFs4DmuxBuf
{
	void*	Ptr;
	__u32	Size;
	__u32	Readed;
	__u32	Prgs;
}	*Ptr_GrDvrFs4DmuxBuf;


//====================================================================
//class

class	Cls_GrDvrFs4Dmux	:	public	Cls_GrTaskCli
{
	protected:
		// property
		Cls_GrTaskSvr*	m_TaskSvr;
		// __u32		m_MsgAloc;

		BOOL8		m_IsDmuxSafe;
		__u8		m_DmuxDevType;

		Cls_GrDvrFs4Mng*	m_FsMng;

		__u8		m_DmuxMode;

		St_GrDvrFs4DmuxChCtrl	m_DmuxChTbl[E_GrDvrMaxChCnt];
		__u8		m_DmuxVdoAbleChCnt;	// able channel count
		__u8		m_DmuxJmpPrgs;		// jump mode progress
		__u32		m_DmuxJmpStep;		// jump mode step (time = sec, byte = byte)
		Def_GrTime	m_DmuxJmpTmLast;	// last jump time
		__u32		m_DmuxSkipCnt;		// dmux frame skip count

		Def_GrTime	m_DmuxSeekTmLast;	// last seek time

		St_GrDvrFs4SeekCtrl	m_DmuxSeekCtrl;

		__u32		m_DmuxTryCnt;				// dmux try count

		Ptr_GrDvrFrmBase	m_PtrDmuxFrm;	// demuxed frame


		// method
		__u8	LcDmuxGetNowFrm(Ptr_GrDvrFrmBase* A_PtrRtFrmPtr);
		__u8	LcDmuxMoveOneFrmFw(void);
		__u8	LcDmuxMoveOneFrmBw(void);

		__u8	LcDmuxLoadFrmUnitFw(void);
		__u8	LcDmuxLoadFrmUnitBw(void);

		__u8	LcDmuxSeekFrmUnitFw(void);
		__u8	LcDmuxSeekFrmUnitBw(void);
		__u8	LcDmuxSeekTimeUnitFw(void);
		__u8	LcDmuxSeekTimeUnitBw(void);

		
		// event
		virtual	BOOL8	EvtBeforeLoadFw(void);	// prepare load forward
		virtual	void	EvtAfterLoadFw(void);		// after work load forward
		virtual	BOOL8	EvtBeforeLoadBw(void);	// prepare load backward
		virtual	void	EvtAfterLoadBw(void);		// after work load backward

		virtual	void	EvtVdoFmtChg(__u8 A_Ch);	// video foramt change

		// api
		void	DmuxModeSet(__u8 A_Mode, __u32 A_JmpStep, __u32 A_FrmSkipCnt);
		__u8	DmuxGoFw(void);
		__u8	DmuxGoBw(void);

		__u8	DmuxGoTimeFw(Def_GrTime A_Time);
		__u8	DmuxGoTimeBw(Def_GrTime A_Time);

		__u8	DmuxGoBegin(void);
		__u8	DmuxGoRecent(void);

		void	DmuxDevSet(__u8 A_DevType);
		void	DmuxSyncResetAllCh(void);
		void	DmuxSyncResetCh(__u8 A_Ch);

		void	DmuxVdoAbleMapSet(__u32 A_Map);
		void	DmuxAdoAbleMapSet(__u32 A_Map);
		void	DmuxMtnAbleMapSet(__u32 A_Map);

	public:
    Cls_GrDvrFs4Dmux( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs4Mng* A_FsMng, BOOL8 A_IsSafe );
    virtual		~Cls_GrDvrFs4Dmux();

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


