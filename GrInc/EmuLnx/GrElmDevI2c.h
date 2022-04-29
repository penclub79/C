/*
	emulation linux module I2c
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElmDevI2c
#define	_EPC_GrElmDevI2c

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<GrProcess.h>
#include	<Dvr/GrDvrBase.h>


//====================================================================
//constance

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class



//====================================================================
//global function

void	GrElmDevI2cInit( BYTE A_DevIdx );
void	GrElmDevI2cFinish( BYTE A_DevIdx );

//====================================================================
//out Enviroment conditional
#endif
