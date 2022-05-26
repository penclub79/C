/*
	GAUSS DVR file system v5 DEMUX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs6Dmux
#define	_EPC_GrDvrFs6Dmux

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/GrDvrBase.h>

#include <Dvr/Fs/V6/GrDvrFs6Mng.h>
	
//====================================================================
//constance

#define	E_GrDvrFs6DmuxFrmMskAll					0
#define	E_GrDvrFs6DmuxFrmMskRef2x				1
#define	E_GrDvrFs6DmuxFrmMskRef4x				2
#define	E_GrDvrFs6DmuxFrmMskIonly				3

#define	E_GrDvrFs6DmuxSeekTryCnt				E_GrDvrMaxChCnt

#define	E_GrDvrFs6DmuxRsltOk						0	/* success */
#define	E_GrDvrFs6DmuxRsltBusy					1	/* busy */
#define	E_GrDvrFs6DmuxRsltDataEmpty			2	/* data empty */
#define	E_GrDvrFs6DmuxRsltEndOfData			3	/* arrival end */
#define	E_GrDvrFs6DmuxRsltBadFrame			4	/* bad frame type */
#define	E_GrDvrFs6DmuxRsltUselessFrame	5	/* useless frame type */

#define	E_GrDvrFs6DmuxDbgMsgView				/* view debug message */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrFs6DmuxChCtrl
{
	// mode
	__u8	MaskMode;			// frame mask mode
	
	BOOL8	IsAble;				// channel able

	// status
	//__u8	Status;

	// runtime
	BOOL8	IsOut;				// now out
	St_GrDvrFs6ReadCtrl	ReadCtl;		// video read control

}	*Ptr_GrDvrFs6DmuxChCtrl;

//====================================================================
//class

class	Cls_GrDvrFs6Dmux
{
	protected:
		// property
		__u8		m_DmuxGrpType;				// group type
		BOOL8		m_IsDmuxDirBack;
		__u8		m_DmuxRunStat;				// runnning control

		__u8		m_DmuxMaxChCnt;				// max channel count

		Cls_GrDvrFs6Mng*	m_FsMng;
		
		St_GrDvrFs6DmuxChCtrl	m_DmuxChTbl[E_GrDvrFs6MaxChCnt];

		// runtime
		Def_GrTime	m_DmuxLastOutTime;		// last out time
		__u8		m_DmuxVdoChCnt;
		__u8		m_DmuxAdoChCnt;

		// method
		// common
		void	LcDmuxStatusUpdate(void);
		void	LcDmuxChFrameStatUpdate(__u8 A_Ch);

		__u64	LcDmuxReadyChMapGet(void);

		// forward
		void	LcDmuxFwChGoByTime(__u8 A_Ch, Def_GrTime A_Time);
		void	LcDmuxFwChRunPlay(__u8 A_Ch);

		void	LcDmuxFwGoByTime(Def_GrTime A_Time);
		void	LcDmuxFwRunPlay(void);

		// backward
		void	LcDmuxBwChGoByTime(__u8 A_Ch, Def_GrTime A_Time);
		void	LcDmuxBwChRunPlay(__u8 A_Ch);

		void	LcDmuxBwGoByTime(Def_GrTime A_Time);
		void	LcDmuxBwRunPlay(void);

	public:
    Cls_GrDvrFs6Dmux( __u8 A_MaxChCnt );
    virtual		~Cls_GrDvrFs6Dmux();

		// api
		void	DmuxRecGroupSet(__u8 A_RecGrpType);
		Ptr_GrDvrFs6ReadCtrl	DmuxReadCtrlPtrGet(__u8 A_Ch);

		__u64	DmuxDo(void);
		__u64	DmuxReadyFrameRead(Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl,__u32 A_BufCnt);
		void	DmuxReadyFrameDone(__u64 A_Map);

		void	DmuxGoTime(Def_GrTime A_Time);
		void	DmuxDirSet(BOOL8 A_IsDirBack);
		void	DmuxVdoChAbleSet(__u32 A_Map);
		void	DmuxAdoChAbleSet(__u32 A_Map);
		void	DmuxVdoFrmMaskSet(__u8 A_Ch, __u8 A_Mask);

		__u8	DmuxRunStatGet(void);

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


