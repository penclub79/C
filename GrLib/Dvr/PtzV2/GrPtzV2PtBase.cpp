/*
 DVR pan tilt protocol base V2

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
#include <GrStrTool.h>

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
Cls_GrPtzV2PtBase::Cls_GrPtzV2PtBase( void )
{
		GrStrClear( m_StrModel );
}
//--------------------------------------------------------------------
Cls_GrPtzV2PtBase::~Cls_GrPtzV2PtBase()
{

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::Init( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrPtzV2PtBase::ModelNameGet( void )
{
		return	m_StrModel;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoPanTilt( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
	__u8 A_SpdPan, __u8 A_SpdTilt )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoZoom( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsWide, __u8 A_Speed )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoFocus( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsFar, __u8 A_Speed )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoIris( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsOpen, __u8 A_Speed )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoAutoFocus( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoPresetSet( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoPresetMove( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id, __u8 A_Speed )
{
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2PtBase::IsMenuAble( void )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::ExIdView( __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, 
	__u8 A_Parity, __u8 A_StopBit, BOOL8 A_IsOn )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::ExIdSet( __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, 
	__u8 A_Parity, __u8 A_StopBit, __u8 A_IdNew )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::TourSet( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::TourRun( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::TourStop( Ptr_GrPtzV2ChMng A_PtrMng )
{
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuLeft( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoPanTilt( A_PtrMng, TRUE, FALSE, FALSE, FALSE, 2, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuRight( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoPanTilt( A_PtrMng, FALSE, TRUE, FALSE, FALSE, 2, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuUp( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoPanTilt( A_PtrMng, FALSE, FALSE, TRUE, FALSE, 2, 2 );
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2PtBase::DoMenuDown( Ptr_GrPtzV2ChMng A_PtrMng )
{
		DoPanTilt( A_PtrMng, FALSE, FALSE, FALSE, TRUE, 2, 2 );
}
//--------------------------------------------------------------------

