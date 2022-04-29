/*
	saa 7121

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwSaa7121
#define	_EPC_GrHwSaa7121

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrI2c.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwSaa7121
{
	private:
		__u8		m_I2cCh;
		__u8		m_Adr;			//i2c address
		BOOL8		m_IsSigPal;
	protected:
		void		WriteReg( __u8 A_RegAdr, __u8 A_RegVal );

	public:
		Cls_GrHwSaa7121( __u8 A_I2cCh, __u32 A_Adr );
		virtual	~Cls_GrHwSaa7121();

		//set
		void		SetSigType( __u32 A_IsPal );
		BOOL8		IsSigPal( void );

};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

