/*
	DVR PTZ protocol PMA200

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrPtzV2PtPma200
#define	_EPC_GrPtzV2PtPma200

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
#include <Dvr/PtzV2/GrPtzV2PtPelcoD.h>
	
//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrPtzV2PtPma200	:	public Cls_GrPtzV2PtPelcoD
{
	public:
    Cls_GrPtzV2PtPma200( void );
    virtual		~Cls_GrPtzV2PtPma200();

		// information
		virtual	BOOL8	IsMenuAble( void );

		// command
		virtual	void	DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng );

		virtual	void	TourSet( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour set

};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


