/*
	solo motion plugin motion check
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrSoloMpMtn
#define	_EPC_GrSoloMpMtn

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<Hw/Solo/RegSolo.h>
#include	<Hw/Solo/GrDrvSolo.h>
#include	<Hw/Solo/GrDevSolo.h>

#include	<Hw/Solo/GrRecRcvSoloV0.h>

//====================================================================
//constance
#define E_GrSoloMpMtnDispBufSize		52
#define E_GrSoloMpMtnDispBufDwUnit	(E_GrSoloMpMtnDispBufSize >> 2)

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class

class Cls_GrSoloMpMtn	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;		//task server
		Cls_GrDevSolo*	m_DevSolo;

		BOOL8	m_Is6110;

		//motion map
		__u32	m_ChUseMap;
		void*	m_MtnMapTbl[E_SoloMaxChCnt];
		__u8	m_DispBufTbl[E_SoloMaxChCnt][E_GrSoloMpMtnDispBufSize];
		__u32	m_MtnOcuMap;
		__u8	m_TmpMap[E_GrSoloMpMtnDispBufSize];			//scaling templation map
	protected:
		//local
		void	LcMtnCheck( Ptr_GrRecRcvMtnCbMsg A_PtrMsg );

		//inherited
		virtual	BOOL8	RtlCallback( Ptr_GrTaskMsgHd A_PtrMsg );	
		
	public:
		Cls_GrSoloMpMtn( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_ObjDev );
		virtual	~Cls_GrSoloMpMtn();

		void	MotionMapSet( __u8 A_Ch, void* A_PtrMap );
		void*	DisplayMapGet( __u8 A_Ch );
		__u32	MotionOucMapGet( void );
		void	ChannelUseMapSet( __u32 A_Map );
		
};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
