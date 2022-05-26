/*
	saa 7128

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwSaa7128
#define	_EPC_GrHwSaa7128

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/Saa7128/RegSaa7128.h>
#include	<GrI2c.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwSaa7128
{
	private:
		__u8		m_I2cCh;
		__u8		m_Adr;			//i2c address
		BOOL8		m_IsSigPal;
	protected:
		void		WriteReg( __u8 A_RegAdr, __u8 A_RegVal );

	public:
		Cls_GrHwSaa7128( __u8 A_I2cCh, __u32 A_Adr );
		virtual	~Cls_GrHwSaa7128();

		//set
		void		SetSigType( __u32 A_IsPal );
		BOOL8		IsSigPal( void );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

