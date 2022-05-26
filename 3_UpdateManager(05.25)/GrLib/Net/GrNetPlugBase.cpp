/*
 GAUSE Network plug-in

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Net/GrNetPlugBase.h>
#include <Net/GrNetSvs.h>

#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrNetPlugBase::Cls_GrNetPlugBase(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx)
{
	// local -------------------
	// code --------------------
		// init
		m_TaskSvr	=	A_TaskSvr;

		m_NetSvs	=	A_NetSvs;

		m_DevIdx	=	A_DevIdx;
		m_PlugIdx	=	A_PlugIdx;

		// regist plug in index
		((Cls_GrNetSvs*)m_NetSvs)->RtlPluginRegist(this, m_PlugIdx);

}
//--------------------------------------------------------------------
Cls_GrNetPlugBase::~Cls_GrNetPlugBase()
{

		// unregist plug in index
	
}
//--------------------------------------------------------------------
