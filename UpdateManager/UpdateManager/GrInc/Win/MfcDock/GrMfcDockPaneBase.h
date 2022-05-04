/*
	Pane base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMfcDockPaneBase
#define	_EPC_GrMfcDockPaneBase

//====================================================================
// uses
#include <afxDockablePane.h>

#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibObj.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global class
class Cls_GrMfcDockPaneBase	: public CDockablePane
{
	protected:
		__u32	m_PaneId;

	public:
		Cls_GrMfcDockPaneBase( void );
		virtual	~Cls_GrMfcDockPaneBase( void );

		__u32	PaneIdGet(void);

		virtual	void	RtlPaneMsgProc(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2);

};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

