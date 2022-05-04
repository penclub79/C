/*
	emulation Hi3531
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrElaIden1200
#define	_EPC_GrElaIden1200

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include <EmuLnx/GrElaBase.h>

#include	<EmuLnx/GrElmDevBase.h>

#include	<GrProcess.h>
#include	<GrGdibObj.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include	<Dvr/GrDvrBase.h>

extern	"C"
{

#include <register.h>

#include <iden_sys.h>

}

//====================================================================
//constance

#define	E_GrElaIden1200DevEmuCnt			2
#define	E_GrElaIden1200DevChCnt				8

#define	E_GrElaIden1200EncStrmCnt			32

//====================================================================
//global macro


//====================================================================
//global type

typedef struct St_GrElaIden1200HencItem
{
	VIDEO_FRAME_HEADER_S	Head;
	Def_GrTime	TmSec;
	__u32	TmMili;
	__u32	GopPrgs;
}	*Ptr_GrElaIden1200HencItem;

//====================================================================
//class

class	Cls_GrElaIden1200:	public Cls_GrTaskCli
{
	private:

	protected:
		void	LcEmuAvLoad( void );

	public:
		Cls_GrElaIden1200( void );
		virtual	~Cls_GrElaIden1200();
		
};


//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif
