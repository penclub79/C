/*
	GAUSS DVR file system v5 DEMUX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFs5Dmux
#define	_EPC_GrFs5Dmux

//====================================================================
// parametter

#define	E_GrFsDmuxDbgMsg
//#define	E_GrFsDmuxFccChk

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/GrDvrBase.h>

#include <Dvr/Fs/V5/GrFs5IoRaid.h>
	
//====================================================================
//constance
#define	E_GrFs5DmuxFrmMskAll					0
#define	E_GrFs5DmuxFrmMskRef2x				1
#define	E_GrFs5DmuxFrmMskRef4x				2
#define	E_GrFs5DmuxFrmMskIonly				3

#define	E_GrFs5DmuxSeekTryCnt					E_GrDvrMaxChCnt

#define	E_GrFs5DmuxRsltOk							0	/* success */
#define	E_GrFs5DmuxRsltBusy						1	/* busy */
#define	E_GrFs5DmuxRsltDataEmpty			2	/* data empty */
#define	E_GrFs5DmuxRsltEndOfData			3	/* arrival end */
#define	E_GrFs5DmuxRsltBadFrame				4	/* bad frame type */
#define	E_GrFs5DmuxRsltUselessFrame		5	/* useless frame type */

#define	E_GrFs5DmuxDbgMsgView				/* view debug message */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrFs5DmuxChCtrl
{
	// mode
	__u8	MaskMode;			// frame mask mode
	
	BOOL8	IsVdoAble;		// channel video able
	BOOL8	IsAdoAble;
	//BOOL8	IsMtnAble;

	// status
	__u8	VdoFps;
	__u32	VdoFmt;

	// runtime
	//__u8	VdoChkSeqId;
	__s16	VdoOutSeqId;		// last sequence id
	__u8	VdoMaskPrgs;
	BOOL8	IsVdoGopSynced;	// video P frame able


}	*Ptr_GrFs5DmuxChCtrl;

typedef struct St_Fs5DmuxDivBuf
{
	void*	PtrBuf;
	__u32	FrmSize;		// 0 == not exist
	__s32	Prgs;				// frame progress position - head - last position, tail - head position
	//__u32	BlkSeq;		// block sequence id
	BOOL8	IsTail;
	BOOL8	IsOn;				// on divide buffer
}	*Ptr_Fs5DmuxDivBuf;

//====================================================================
//class

class	Cls_GrFs5Dmux	:	public	Cls_GrTaskCli
{
	protected:
		// property
		Cls_GrTaskSvr*	m_TaskSvr;

		__u8		m_DmuxRunDsub;				// running device sub

		BOOL8		m_IsDmuxDirBack;

		__u8		m_DmuxRunStat;				// runnning control

		__u8		m_DmuxIskipCnt;				// i only skip count

		__u32		m_DmuxFrmMaxSize;			// max frame size

		Def_GrTime	m_DmuxSeekTime;		// seek target time

		Cls_GrFs5IoRaid*	m_DmuxIo;
		
		St_GrFs5DmuxChCtrl	m_DmuxChTbl[E_GrFs5MaxChCnt];
		St_GrFs5SeekCtrl		m_DmuxSctl;

		// runtime
		Def_GrTime	m_DmuxTimeLast;				// now demuxing time

		Ptr_GrDvrFrmHdBase	m_PtrDmuxPars;			// demuxed parsing frame
		Ptr_GrDvrFrmHdBase	m_PtrDmuxOut;				// demuxed out able frame

		// divide
		St_Fs5DmuxDivBuf	m_DmuxDiv;						// divide

		// method
		void	LcDmuxParsFrmUpdt(void);

		void	LcDmuxOutFrmUpdtFw(void);
		void	LcDmuxOutFrmUpdtBw(void);

		// forward
		Def_GrErrCode	LcDmuxFwNextFrm(void);
		Def_GrErrCode	LcDmuxFwNextSctl(void);

		void	LcDmuxFwProcPlay(void);
		void	LcDumxFwProcSeek(void);

		Def_GrErrCode	LcDmuxFwGoTime(Def_GrTime A_Time);
		void	LcDmuxFwGoStart(void);
		Def_GrErrCode	LcDmuxFwGoHndl(__u64 A_Hndl);

		// backward
		Def_GrErrCode	LcDmuxBwNextFrm(void);
		Def_GrErrCode	LcDmuxBwNextSctl(void);

		void	LcDmuxBwProcPlay(void);
		void	LcDumxBwProcSeek(void);

		Def_GrErrCode	LcDmuxBwGoTime(Def_GrTime A_Time);

		virtual	void	EvtVdoFmtChg(__u8 A_Ch);	// video foramt change

		virtual	BOOL8	IsDataLoadAble(void);
		virtual	void	LoadBefore(void);
		virtual	void	LoadAfter(void);

		// api
		Def_GrErrCode	DmuxDevClose(void);

		void	DmuxVdoIfrmSyncAllCh(void);
		Def_GrErrCode	DmuxGoTime(Def_GrTime A_Time);
		Def_GrErrCode	DmuxGoHndl(__u64 A_Hndl);

		__u8	DmuxGoNext(void);

		void	DmuxDirSet(BOOL8 A_IsDirBack);
		void	DmuxVdoChAbleSet(__u32 A_Map);
		void	DmuxAdoChAbleSet(__u32 A_Map);
		void	DmuxVdoFrmMaskSet(__u8 A_Ch, __u8 A_Mask);

	public:
    Cls_GrFs5Dmux( Cls_GrTaskSvr* A_TaskSvr, __u32 A_MaxFrmSize );
    virtual		~Cls_GrFs5Dmux();


};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


