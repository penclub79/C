/*
Platform 5 grid base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5UoGridSmd
#define	_EPC_GrP5UoGridSmd

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5UoBase.h>
#include <P5/GrP5UoGridBase.h>
#include <GrTimeTool.h>

#include <GrGdibObj.h>

//====================================================================
//constance

#define	E_GrP5UoGridSmdDrawToggle		0
#define	E_GrP5UoGridSmdDrawSet			1
#define	E_GrP5UoGridSmdDrawReset		2

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5UoGridSmdPara
{
	__u16	StpAdr;
	__u16	ChSize;
}	*Ptr_GrP5UoGridSmdPara;

//====================================================================
//global class

class Cls_GrP5UoGridSmd	: public Cls_GrP5UoGridBase
{
	protected:

		St_GrP5UoGridSmdPara	m_ParaUoGridSmd;

		__u8	m_Ch;
		__u8	m_DrawMode;

		void	CellValueSet(__u32 A_X, __u32 A_Y, __u8 A_Mode);

		// inherited

#ifdef GR_P5_EDITOR
		// object implement
		virtual	void	DvlParaInfoBuild(void);		// this class parametter info build
#endif

		// event system
		virtual	void	DoEvtGrpItemOneSel(void);
		virtual	void	DoEvtGrpItemAreaSel(void);

		virtual	BOOL8	EvtKeyUp(__u8 A_Key);	/* not need inerite */

		virtual	void	EvtScptDecDone(void);

	public:
		Cls_GrP5UoGridSmd(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect);		// for window
		virtual	~Cls_GrP5UoGridSmd(void);

		// basic information

		// control
		BOOL8	CellXyCntSet(__u32 A_CntX, __u32 A_CntY);
		virtual	__u32	CellValueGet(__u32 A_IdxX, __u32 A_IdxY);


		void	ChannelSet(__u8 A_Ch);
		__u8	ChannelGet(void);

		void	DrawModeSet(__u8 A_Mode);
		__u8	DrawModeGet(void);
		void	GridClear(void);
		void	GridFill(void);

		// runtime

#ifdef GR_P5_EDITOR
		virtual	void	DvlParaInfoAction(__u32 A_Action);
#endif
};



//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

