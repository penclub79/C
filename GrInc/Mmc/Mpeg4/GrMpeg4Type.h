/*
	Mpeg4 strema type
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMpeg4Type
#define	_EPC_GrMpeg4Type

//====================================================================
// uses
#include <GrTypeBase.h>

//====================================================================
//constance

#define E_GrMpg4VoStart			0x00010000
#define E_GrMpg4VoIdMsk			0x1F000000			//void base

#define E_GrMpg4VolStart		0x20010000		
#define E_GrMpg4VolIdMsk		0x0F000000			//volid base

#define E_GrMpg4VopStart		0xB6010000		

#define E_GrMpg4VopShftType	6								//byte base
#define E_GrMpg4VopMskType	0x03						//byte base - later shift

#define E_GrMpg4VopTypeI		0
#define E_GrMpg4VopTypeP		1
#define E_GrMpg4VopTypeB		2								
#define E_GrMpg4VopTypeS		3								//sprite


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif