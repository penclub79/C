//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrBmpTool
#define	_EPC_GrBmpTool

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDrawTool.h>

//====================================================================
//constance

#define E_GrBmpFileId			0x4D42		//bmp file id

//====================================================================
//global macro


//====================================================================
//global type

#ifndef LINUX_APP
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct St_GrBmpIfHd //bitmap info head
{
	__u32		Size;
	__s32			Width;
	__s32			Height;
	__u16		Planes;
	__u16		BitCount;
	__u32		Compression;
	__u32		SizeImage;
	__s32			XPelsPerMeter;
	__s32			YPelsPerMeter;
	__u32		ClrUsed;
	__u32		ClrImportant;
}	__gccna	*Ptr_GrBmpIfHd;

typedef	struct St_GrBmpFlHead
{
	__u16			Id;		
	__u32			Size;
	__u16			_Rsv0;
	__u16			_Rsv1;
	__u32			OfsDib;

	St_GrBmpIfHd Bi;
} __gccna	*Ptr_GrBmpFlHead;

#ifndef LINUX_APP
#pragma pack(pop)
#endif

//====================================================================
//global function

BOOL8	GrBmpGetInfo( void* A_PtrBmp, __u32* A_PtrRtResX, __u32* A_PtrRtResY, 
									 __u32* A_PtrRtBit );
void*	GrBmpGetDibPtr( void* A_PtrBmp );
void*	GrBmpGetPalletPtr( void* A_PtrBmp );
__u32	GrBmpCalcSize( __u32 A_ResX, __u32 A_ResY, __u32 A_Bit );
void	GrBmpSetHeader( void* A_PtrBmp, __u32 A_ResX, __u32 A_ResY, __u32 A_Bit );
	
//====================================================================
//out Enviroment conditional
#endif

