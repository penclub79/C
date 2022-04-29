/*
 dvr pantilt protocol base

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzPtBase.h>

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
Cls_GrDvrPtzPtBase::Cls_GrDvrPtzPtBase( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch )
{
		m_UartCh	=	A_UartCh;
		m_Id			=	A_Id;
		m_Ch			= A_Ch;
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtBase::~Cls_GrDvrPtzPtBase()
{

}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtBase::DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
								__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
		return	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtBase::DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
		return	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtBase::DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
		return	0;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzPtBase::DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoStop( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoAutoFocus( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuCall( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuExit( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoPresetSet( __u8 A_Id )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoPresetMove( __u8 A_Id, __u8 A_Speed )
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzPtBase::IsMenuAble( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuOk( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuCancel( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::ExIdView( BOOL8 A_IsOn )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::ExIdSet( __u8 A_IdNew )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::TourRun( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::TourStop( void )
{
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuLeft( void )
{
		DoPanTilt( TRUE, FALSE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuRight( void )
{
		DoPanTilt( FALSE, TRUE, FALSE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuUp( void )
{
		DoPanTilt( FALSE, FALSE, TRUE, FALSE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoMenuDown( void )
{
		DoPanTilt( FALSE, FALSE, FALSE, TRUE, 2, 2, FALSE );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzPtBase::DoTimer( void )
{
		//void
}
//--------------------------------------------------------------------

