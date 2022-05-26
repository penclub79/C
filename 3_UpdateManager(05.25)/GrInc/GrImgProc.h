//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrImgProc
#define	_EPC_GrImgProc

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct St_GrImpSmp8x8 
{
	__u32		Avr;						//average
	__u8		Smp[64];				//sample data
}	*Ptr_GrImpSmp8x8;

//====================================================================
//global function
__u32		GrImpVecter8x8(void * A_PtrBlk1, void* A_PtrBlk2);

__u32		GrImpSad8x8( Ptr_GrImpSmp8x8 A_PtrSmp1, Ptr_GrImpSmp8x8 A_PtrSmp2 );

//sampling one block
void		GrImpSmpOneY8x8Uyvy( void* A_PtrDib, __u32 A_X, __u32 A_Y, __u32 A_Pitch, void* A_PtrRtBuf );

//sampling image
void		GrImpSmpImgY8x8Uyvy( void* A_PtrDib, __u32 A_CntX, __u32 A_CntY, __u32 A_Pitch, void* A_PtrRtBuf );

//info
__u32		GrImpCalc8x8SmpImgSize( __u32 A_BlkCnt );

//====================================================================
//out Enviroment conditional
#endif

