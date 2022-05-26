/*
	Platform 5 Developer reference header parser

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5DvlRefPrs
#define	_EPC_GrP5DvlRefPrs

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrError.h>

#include <P5/GrP5Mng.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrLayerFile.h>

//====================================================================
//constance

#define	E_GrP5DvlRefPrsTimerPeriod				1000

#define	E_GrP5DvlRefPrsSrcBufSize					0x80000

// line analysis
#define	E_GrP5DvlRefPrsLineAnalysisMax		64

// parsing local mode
#define	E_GrP5DvlRefPrsModeNormal					0
#define	E_GrP5DvlRefPrsModeRmkBlock				1

// pa


//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrP5DvlRefPrsLineAnal
{
	__u32	Pos;
	__u32	Cnt;
}	*Ptr_GrP5DvlRefPrsLineAnal;

//====================================================================
//global class

class Cls_GrP5DvlRefPrs : public Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TskSvr;

		__u32	m_MsgAloc;
		__u32	m_TmrMain;

		Def_WstrGrFnPath	m_StrFnHead;
		Def_StrTag				m_StrPrefix;
		__u32	m_LenPrefix;
		
		FILETIME	m_FtimeLast;

		BOOL8	m_IsDoPars;

		__u32	m_SrcSize;
		__u8	m_SrcBuf[E_GrP5DvlRefPrsSrcBufSize];

		// parsing
		__u32	m_ParsMode;
		BOOL8	m_IsRmkBlk;
		__u32	m_ParsPos;

		__u32	m_LineAnalCnt;
		St_GrP5DvlRefPrsLineAnal	m_LineAnalTbl[E_GrP5DvlRefPrsLineAnalysisMax];

	protected:

		void	TprcFileChk(void);
		BOOL8	BprcParse(void);
		void	ParsData(void);
		void	LineAnalysis(void);
		void	LinePars(void);
		void	ResetData(void);

		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrP5DvlRefPrs(Cls_GrTaskSvr* A_TaskSvr);
		virtual	~Cls_GrP5DvlRefPrs(void);

		// inherited


};


//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

