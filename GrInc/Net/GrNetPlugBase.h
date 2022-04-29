/*
	GAUSE Network service plug-in

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrNetPlugBase
#define	_EPC_GrNetPlugBase

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskBase.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <GrSockV2.h>
	
//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type



//====================================================================
//class
class	Cls_GrNetPlugBase : public Cls_GrTaskCli
{
	protected:
		Cls_GrTaskSvr*	m_TaskSvr;

		Cls_GrTaskCli*	m_NetSvs;
		__u8		m_DevIdx;
		__u8		m_PlugIdx;		// plugin slot index

		// local


	public:
		Cls_GrNetPlugBase(Cls_GrTaskSvr* A_TaskSvr, Cls_GrTaskCli* A_NetSvs, __u8 A_DevIdx, __u8 A_PlugIdx);
		virtual		~Cls_GrNetPlugBase();

};

	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


