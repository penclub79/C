/*
	emulation linux module device base
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevBase
#define	_EPC_GrElmDevBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<EmuLnx/GrElmBase.h>

//====================================================================
//constance

//====================================================================
//global macro


//====================================================================
//global type

//====================================================================
//class

class Cls_GrElmDevBase
{
	protected:
		pci_dev		m_PciDev;
		DWORD			m_PciVid;			//veodor id
		DWORD			m_PciPid;			//product id
		void*			m_UserData;		//pci user data

	public:
		Cls_GrElmDevBase( DWORD A_DevIdx );
		virtual	~Cls_GrElmDevBase();

		virtual	Ptr_pci_dev	PciDevPtrGet( void );
		virtual	DWORD	PciVendorIdGet( void );
		virtual	DWORD	PciProductIdGet( void );
	
		//configuration
		virtual	BOOL	PciCfgWriteByte( DWORD A_Adr, BYTE A_Val );
		virtual	BOOL	PciCfgWriteWord( DWORD A_Adr, WORD A_Val );
		virtual	BOOL	PciCfgWriteDword( DWORD A_Adr, DWORD A_Val );
		virtual	BOOL	PciCfgReadByte( DWORD A_Adr, BYTE* A_PtrRtVal );
		virtual	BOOL	PciCfgReadWord( DWORD A_Adr, WORD* A_PtrRtVal );
		virtual	BOOL	PciCfgReadDword( DWORD A_Adr, DWORD* A_PtrRtVal );

		//user data
		virtual	void	PciUserDataSet( void* A_Data );
		virtual	void*	PciUserDataGet( void );

		//io
		virtual	BOOL	MioReadByte( void* A_Ptr, BYTE* A_PtrRtVal );
		virtual	BOOL	MioReadWord( void* A_Ptr, WORD* A_PtrRtVal );
		virtual	BOOL	MioReadDword( void* A_Ptr, DWORD* A_PtrRtVal );
		virtual	BOOL	MioWriteByte( void* A_Ptr, BYTE A_Val );
		virtual	BOOL	MioWriteWord( void* A_Ptr, WORD A_Val );
		virtual	BOOL	MioWriteDword( void* A_Ptr, DWORD A_Val );

};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
