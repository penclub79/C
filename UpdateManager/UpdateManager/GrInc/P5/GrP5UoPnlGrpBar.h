/*
Platform 5 graph bar

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoPnlGrpBar
#define	_EPC_GrP5UoPnlGrpBar

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoPntGrpBarItemNameMaxLen	7

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoPnlGrpBarItem
{
	__u32	Val;
}	*Ptr_GrP5UoPnlGrpBarItem;

typedef struct St_GrP5UoPnlGrpBarPara
{
	__u8	GridCnt;
	__u8	BoarderWidth;
	__u8	GridLineWidth;
	__u8	FontGrid;

	__u8	FontItem;
	__u8	FontVal;
	BOOL8	IsBarValView;
	__u8	_rsvd2;

	__u32	BarWidth;
	__u32	GridHeight;
	__u32	ItemNameHeight;
	__u32	GridValWidth;
	__u32	GridValHeight;
	__u32	ClrGrid;
	__u32	ClrBoarder;
	__u32	BarValHeight;

	St_GrP5FontDrawStyle	TxtStyleGrid;
	St_GrP5FontDrawStyle	TxtStyleItem;
	St_GrP5FontDrawStyle	TxtStyleVal;

}	*Ptr_GrP5UoPnlGrpBarPara;

//====================================================================
//global class

class Cls_GrP5UoPnlGrpBar	: public Cls_GrP5UoBase
{
	protected:

		St_GrP5UoPnlGrpBarPara	m_ParaUoPnlGrpBar;
		St_GrP5SkinCtlPnlGrpBar	m_SkinCtlPnlGrpBar;

		St_GrP5UoPnlGrpBarItem	m_ItemTbl[E_GrP5SkinPnlGrpBarItemMaxCnt];


		// runtime
		__u32		m_ValStep;				// value step

		// paint

		void	LcGridValBuild(void);

		// event system
		virtual	void	EvtScptDecDone(void);

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

	public:
		Cls_GrP5UoPnlGrpBar(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoPnlGrpBar(void);

		// basic information

		// control
		__u32	ItemCountGet(void);
		void	ItemCountSet(__u32 A_Cnt);

		__u32	GridStepGet(void);
		void	GridStepSet(__u32 A_Step);

		void	ItemNameSet(__u32 A_ItmIdx, WCHAR* A_Str);
		void	ItemDataSet(__u32 A_ItmIdx,__u32 A_Color, __u32 A_Value );
		void	ItemColorSet(__u32 A_ItmIdx, __u32 A_Color);
		void	ItemValueSet(__u32 A_ItmIdx, __u32 A_Value);

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

