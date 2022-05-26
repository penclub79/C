/*
	//solo mpeg4 video information
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrVdoInfo
#define	_EPC_GrVdoInfo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#ifdef	LINUX_APP
	#include	<sys/time.h>
	#include	<unistd.h>
#else
	#include	<windows.h>
#endif

//====================================================================
//constance
//encode format type
#define E_I420FORMAT		0

//dedoce type
#define E_INPUT_VOL			0
#define E_INPUT_VOP			1

#define USE_PNM		0
#define USE_TGA		1
//bpp
#define XVID_BPP_I420 1
#define XVID_BPP_YUY2 4
//fcc
#define FCC_DX50 0x30355844
#define FCC_YUY2 0x32595559
#define FCC_UYVY 0x59565955
#define FCC_I420 0x30323449

//====================================================================
//global macro

//====================================================================
//global type
struct VIDEO_HEADER
{
	__u32 	szData;
	__s32			vopType;
	__s32			picCount;
	__s32			curVideoPacket;// 1 base...
	__s32			totVideoPacket;
	
	__s32			channel,hsize,vsize,interlace,ntpal,interval;
	__s32			bMP4P;

	unsigned __s32 crypt_key; 
	unsigned __s32 watermark_key; 

	long		nSec;
	long		nUSec;
};

struct AUDIO_HEADER
{
	__u32		szData;
	__s32			idxCurPacket;
	long		nSec;
	long		nUSec;
};

struct St_VdoHeader
{
	VIDEO_HEADER	VdoHead;
	__u32					Channel;
};

struct St_DecTimeInfo
{
	__u32		TotFrmCnt;
	__u32		TotTime;
};
//====================================================================
//class

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif