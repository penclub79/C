/*
	emulation linux module 

*/

//====================================================================
//include

#include	<EmuLnx/GrElmDevBase.h>
#include	<GrDumyTool.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//global var

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrElmDevBase::Cls_GrElmDevBase( DWORD A_DevIdx )
{
		GrDumyZeroMem( &m_PciDev, sizeof(m_PciDev) );
		m_PciDev.DevIdx	=	A_DevIdx;

		m_UserData	=	NULL;
}
//--------------------------------------------------------------------
Cls_GrElmDevBase::~Cls_GrElmDevBase()
{
}
//--------------------------------------------------------------------
Ptr_pci_dev	Cls_GrElmDevBase::PciDevPtrGet( void )
{
		return	&m_PciDev;
}
//--------------------------------------------------------------------
DWORD	Cls_GrElmDevBase::PciVendorIdGet( void )
{
		return	m_PciVid;
}
//--------------------------------------------------------------------
DWORD	Cls_GrElmDevBase::PciProductIdGet( void )
{
		return	m_PciPid;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgWriteByte( DWORD A_Adr, BYTE A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgWriteWord( DWORD A_Adr, WORD A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgWriteDword( DWORD A_Adr, DWORD A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgReadByte( DWORD A_Adr, BYTE* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgReadWord( DWORD A_Adr, WORD* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::PciCfgReadDword( DWORD A_Adr, DWORD* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevBase::PciUserDataSet( void* A_Data )
{
		m_UserData	=	A_Data;
}
//--------------------------------------------------------------------
void*	Cls_GrElmDevBase::PciUserDataGet( void )
{
		return	m_UserData;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioReadByte( void* A_Ptr, BYTE* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioReadWord( void* A_Ptr, WORD* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioReadDword( void* A_Ptr, DWORD* A_PtrRtVal )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioWriteByte( void* A_Ptr, BYTE A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioWriteWord( void* A_Ptr, WORD A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevBase::MioWriteDword( void* A_Ptr, DWORD A_Val )
{
		return	FALSE;
}
//--------------------------------------------------------------------


