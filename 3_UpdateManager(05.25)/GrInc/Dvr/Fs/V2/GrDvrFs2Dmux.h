/*
	GAUSE DVR file system v2 DEMUX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrFs2Dmux
#define	_EPC_GrDvrFs2Dmux

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>

#include <GrTaskSvr.h>
#include <GrTaskCli.h>

#include <Dvr/Fs/V2/GrDvrFs2Mng.h>
	
//====================================================================
//constance

#define E_GrDvrFs2DmuxStrmLimite		0x80000

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrFs2Dmux	:	public	Cls_GrTaskCli
{
	protected:
		// property
		Cls_GrTaskSvr*	m_TaskSvr;
		// __u32		m_MsgAloc;

		BOOL8		m_IsSafe;
		__u8		m_DevType;

		Cls_GrDvrFs2Mng*	m_FsMng;

		Ptr_GrDvrFs2SeekCtrl	m_PtrSeekCtrl;
		void*	m_PtrFrm;

		// method
		virtual	void	LcFrmPtrUpdt( void );		// frame pointer update

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
		void	SeekByMinFw( Def_GrTime A_Time );
		void	SeekByMinBw( Def_GrTime A_Time );

	public:
    Cls_GrDvrFs2Dmux( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs2Mng* A_FsMng, Ptr_GrDvrFs2SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe );
    virtual		~Cls_GrDvrFs2Dmux();

		__u8	SeekStatGet( void );

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


