/*
	GAUSE DVR file system v3 DEMUX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs3Dmux
#define	_EPC_GrDvrFs3Dmux

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V3/GrDvrFs3Mng.h>
	
//====================================================================
//constance

#define E_GrDvrFs3DmuxStrmLimite		0x80000

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrFs3Dmux	:	public	Cls_GrTaskCli
{
	protected:
		// property
		Cls_GrTaskSvr*	m_TaskSvr;
		// __u32		m_MsgAloc;

		BOOL8		m_IsSafe;
		__u8		m_DevType;

		Cls_GrDvrFs3Mng*	m_FsMng;

		Ptr_GrDvrFs3SeekCtrl	m_PtrSeekCtrl;
		void*	m_PtrFrm;

		// method
		void	LcFrmPtrUpdt( void );		// frame pointer update

		virtual	BOOL8	PrepareLoadFw( void );	// prepare load forward
		virtual	void	AfterLoadFw( void );		// after work load forward
		virtual	BOOL8	PrepareLoadBw( void );	// prepare load backward
		virtual	void	AfterLoadBw( void );		// after work load backward

		void	FrmNext( void );
		void	FrmPrev( void );

		void	SeekBegin( void );
		void	SeekEnd( void );
		void	SeekByHndlFw( __u64 A_Hndl );
		void	SeekByHndlBw( __u64 A_Hndl );
		BOOL8	SeekByMinFw( Def_GrTime A_Time );
		BOOL8	SeekByMinBw( Def_GrTime A_Time );

	public:
    Cls_GrDvrFs3Dmux( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs3Mng* A_FsMng, Ptr_GrDvrFs3SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe );
    virtual		~Cls_GrDvrFs3Dmux();

		__u8	SeekStatGet( void );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


