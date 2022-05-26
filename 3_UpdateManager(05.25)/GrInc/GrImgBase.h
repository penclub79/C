/*
	image base
	uses
		GrTypeBase.h

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrImgBase
#define	_EPC_GrImgBase

//====================================================================
// uses

#include	<GrTypeBase.h>
	
//====================================================================
//constance

//FourCc
//standard fourcc --------------------
//rgb image format
					/*standard RGB*/
#define		E_GrFccRGB							0

//yuv image format
#define		E_GrFccYUY2							0x32595559
#define		E_GrFccUYVY							0x59565955
#define		E_GrFccI420							0x30323449			/* not implemented */
#define		E_GrFccIF09							0x39304649			/* not implemented */
#define		E_GrFccYV12							0x32315659			/* not implemented */
#define		E_GrFccNV12							0x3231564E			/* not implemented */

//custom image fourcc 
#define		E_GrFccRGB555						Mac_GrMakeFcc('R','G','B','0')
#define		E_GrFccRGB565						Mac_GrMakeFcc('R','G','B','1')
#define		E_GrFccRGB888						Mac_GrMakeFcc('R','G','B','2')

#define		E_GrFccARGB1555					Mac_GrMakeFcc('r','g','b','0')
#define		E_GrFccARGB4444					Mac_GrMakeFcc('r','g','b','1')
#define		E_GrFccARGB8888					Mac_GrMakeFcc('r','g','b','2')

#define		E_GrFccBGR888						Mac_GrMakeFcc('B','G','R','0')

#define		E_GrFccABGR8888					Mac_GrMakeFcc('b','g','r','0')

#define		E_GrFccUYVYF						Mac_GrMakeFcc('U','Y','V','F')		/* uyvu + fx */
#define		E_GrFccUVYA							Mac_GrMakeFcc('U','V','Y','A')		/* u,v,y,a */

//avi codec FourCc
#define		E_GrFccMsVideo1					0x6376736D   
#define		E_GrFccCinepack					0x64697663   
#define		E_GrFccLoselsJpeg				0x6A6D6970 
#define		E_GrFccWavelet2k				0x32777670
#define		E_GrFccMjpeg2k					0x63326A6D
#define		E_GrFccIntelI263				0x33363269
#define		E_GrFccXvidMpeg4				0x64697678
#define		E_GrFccDivx							0x78766964

#define		E_GrFccMJPG							0x47504A4D
#define		E_GrFccDivxFast					0x34564944
#define		E_GrFccDivOpen					0x58564944
#define		E_GrFccMpeg4Fast				0x3447504D
#define		E_GrFccIntelVideo32			0x32335649
#define		E_GrFccIntelVideo40			0x30345649
#define		E_GrFccIntelVideo49			0x39345649
#define		E_GrFccIntelVideo50			0x30355649
#define		E_GrFccMS263						0x3336324D
#define		E_GrFccGTRON						0x49564152
#define		E_GrFccCanopus					0x43564443
#define		E_GrFccGPEG							0x47504547
#define		E_GrFccGLZW							0x575A4C47
#define		E_GrFccMaze							0x485A534D
#define		E_GrFccZlib							0x42494C5A
#define		E_GrFccHufYuv						0x55594648
#define		E_GrFccUbMpeg4					0x34504D55
#define		E_GrFccMsMpeg4_1				0x3334504D
#define		E_GrFccMsMpeg4_2				0x3234504D
#define		E_GrFccDivx5_6					0x30355844
#define		E_GrFccH264							0x34363248

#define		E_GrFccFont							0xFFFFFFFE			/*	ui font ver2 */
#define		E_GrFccNone							0xFFFFFFFF			/*	none format	*/

//alpha
#define	E_GrImgAlphaLow						0
#define	E_GrImgAlphaMax						255
	
//====================================================================
//global macro

#define Mac_GrMakeRgb24(val_r, val_g, val_b)	\
	(((__u32)(__u8)(val_r) << 16) | ((__u32)(__u8)(val_g) << 8) | ((__u32)(__u8)(val_b)))

#define Mac_GrMakeArgb32(val_a, val_r, val_g, val_b)	\
	(((__u32)(__u8)(val_a) << 24) | ((__u32)(__u8)(val_r) << 16) | ((__u32)(__u8)(val_g) << 8) | ((__u32)(__u8)(val_b)))

//====================================================================
//global type

typedef	struct	St_GrImgFmtYUV
{
	__u8		Y;
  __u8		U;
  __u8    V;
} *Ptr_GrImgFmtYUV;

typedef	struct	St_GrImgFmtYUY2
{
	__u8		Y1;
  __u8		U;
  __u8    Y2;
  __u8    V;
} *Ptr_GrImgFmtYUY2;

typedef	struct	St_GrImgFmtUYVY
{
	__u8		U;
  __u8    Y1;
  __u8    V;
  __u8    Y2;
} *Ptr_GrImgFmtUYVY;

typedef	struct	St_GrImgFmtRGB888
{
	__u8    B;
	__u8    G;
	__u8    R;
}	*Ptr_GrImgFmtRGB888;

typedef	struct	St_GrImgFmtBGR888
{
	__u8    R;
	__u8    G;
	__u8    B;
}	*Ptr_GrImgFmtBGR888;

typedef	struct	St_GrImgFmtARGB8888
{
	__u8    B;
	__u8    G;
	__u8    R;
	__u8    A;
}	*Ptr_GrImgFmtARGB8888;

typedef	struct	St_GrImgFmtABGR8888
{
	__u8    B;
	__u8    G;
	__u8    R;
	__u8    A;
}	*Ptr_GrImgFmtABGR8888;

typedef	struct	St_GrImgFmtUVYA
{
	__u8		U;
	__u8		V;
	__u8    Y;
	__u8    A;
} *Ptr_GrImgFmtUVYA;

//====================================================================
//class

//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
