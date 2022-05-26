/*
Platform 5 graph line

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlGrpLine
#define	_EPC_GrP5UoPnlGrpLine

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance


#define	E_GrP5UoPntGrpBarColNameMaxLen		7

//====================================================================
//global macro

//====================================================================
//global type


typedef struct St_GrP5UoPnlGrpLineGrpCol
{
	__u32	Val;
}	*Ptr_GrP5UoPnlGrpLineGrpCol;

typedef struct St_GrP5UoPnlGrpLineGraph
{
	St_GrP5UoPnlGrpLineGrpCol	Cols[E_GrP5SkinPnlGrpLineColMaxCnt];
}	*Ptr_GrP5UoPnlGrpLineGraph;

typedef struct St_GrP5UoPnlGrpLinePara
{
	__u8	GridCnt;
	__u8	BoarderWidth;
	__u8	GridLineWidth;
	__u8	FontGrid;
	__u8	FontCol;
	__u8	GrpWidth;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	GridHeight;
	__u32	ColNameHeight;
	__u32	GridValWidth;
	__u32	GridValHeight;
	__u32	ClrGrid;
	__u32	ClrBoarder;

	St_GrP5FontDrawStyle	TxtStyleGrid;
	St_GrP5FontDrawStyle	TxtStyleCol;

	__u32	ClrGraph[E_GrP5SkinPnlGrpLineGrpMaxCnt];

}	*Ptr_GrP5UoPnlGrpLinePara;

//====================================================================
//global class

class Cls_GrP5UoPnlGrpLine	: public Cls_GrP5UoBase
{
	protected:
		St_GrP5UoPnlGrpLinePara	m_ParaUoPnlGrpLine;
		St_GrP5SkinCtlPnlGrpLine	m_SkinCtlPnlGrpLine;

		St_GrP5UoPnlGrpLineGraph	m_GrpTbl[E_GrP5SkinPnlGrpLineGrpMaxCnt];

		// runtime
		__u32		m_ValStep;				// value step

		// paint

		void	LcGridValBuild(void);
		void	LcGrpValBuild(void);

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlGrpLine(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlGrpLine(void);

		// basic information

		// control
		__u32	GraphCountGet(void);
		void	GraphCountSet(__u32 A_Cnt);

		__u32	ColumCountGet(void);
		void	ColumCountSet(__u32 A_Cnt);
		void	ColumNameSet(__u32 A_ColIdx, WCHAR* A_Str);

		__u32	GridStepGet(void);
		void	GridStepSet(__u32 A_Step);

		void	GraphVisibleSet(__u32 A_GrpIdx, BOOL8 A_IsVisible);
		void	GraphValueSet(__u32 A_GrpIdx, __u32 A_ColIdx, __u32 A_Value);

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
		virtual	void	DvlPreDelete(void);		// prepare delete
#endif

};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

