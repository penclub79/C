/*
	externel real time clock manager
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_R2HwDs3231
#define	_EPC_R2HwDs3231

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

#define E_GrHwDs3231DevAdr		0xD0
#define E_GrHwDs3231TimeLen		7

//register address
#define E_GrHwDs3231RaSec			0x00


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwDs3231
{
	private:
		__u8		m_ChipAdr;			//chip address
		__u8		m_I2cCh;
		__u8		m_I2cSlot;

		BOOL8		m_IsTimeReq;

	protected:
		BOOL8		LcReqRead( void );
		BOOL8		LcReqWrite( Def_GrTime A_Time );

		__u32		LcParseTime( void* A_PtrRecv );
	public:
		Cls_GrHwDs3231( __u8 A_I2cCh, __u32 A_Adr, __u8 A_Slot );
		virtual	~Cls_GrHwDs3231();

		void		ReqTimeRead( void );
		void		ReqTimeWrite( Def_GrTime A_Time );
		__u32		TryTimeGet( void );

};

//====================================================================
//global function

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif
