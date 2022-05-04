/*
	Platform 5 render

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Render
#define	_EPC_GrP5Render

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P5/GrP5Base.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <P5/GrP5Script.h>
#include <P5/GrP5Text.h>
#include <P5/GrP5Msg.h>

#include <FontV3/GrFontV3DrawBase.h>

#include <GrLayerData.h>

//====================================================================
// option

#define	E_GrP5RenderGatherTrans				// frame buffer transfer gathering mode

//====================================================================
//constance

#define	E_GrP5RenderTimerPeriod				100

#define	E_GrP5RendCmdRunModeSet				1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrP5RendCmdBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrP5RendCmdBase;

typedef struct St_GrP5RendCmdRunModeSet
{
	St_GrP5RendCmdBase	Hd;
	BOOL8	IsOn;
}	*Ptr_GrP5RendCmdRunModeSet;

//====================================================================
//global class

class Cls_GrP5Render	: public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*	m_TskSvr;
		__u32	m_MsgAloc;

		//__u32	m_TmrMain;

		// paint
		BOOL8	m_IsOn;			// rendering on

		__u8	m_ConIdx;
		__u8	m_SctIdx;		// running section index

		Cls_GrGdib*	m_GdibScr;

		St_GrRect	m_RcSct;		// section

		__u32	m_PntIp;

#ifdef E_GrP5RenderGatherTrans
		St_GrRect	m_RcGth;		// gather
#endif

		Cls_GrP5Font*	m_ObjFont;
		St_GrP5FontDrawStyle	m_FontPara;
		__s32	m_CusPos;
		__u32	m_ClrCus;

		// local function
		void	LcCmdSection(void);
		void	LcCmdClip(void);
		void	LcCmdFillRect(void);
		void	LcCmdLine(void);
		void	LcCmdPixel(void);
		void	LcCmdImage(void);
		void	LcCmdTxtStyle0(void);
		void	LcCmdTxtStyle1(void);
		void	LcCmdTxtCus(void);
		void	LcCmdTxtDraw(void);
		void	LcCmdDrawRect(void);

		BOOL8	LcTmrConPaint(void);

		BOOL8	LcBkpRender(void);
		BOOL8	LcConRender(__u8 A_ConIdx);


		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */
		virtual	void	RtlTimer(__u32 A_Hndl);

	public:
		Cls_GrP5Render( Cls_GrTaskSvr* A_TaskSvr );
		virtual	~Cls_GrP5Render(void);

		// immidately
		void	ImmRenderRunSet(BOOL8 A_IsRun);

		// request
		void	ReqRenderRunSet(BOOL8 A_IsRun);

};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

