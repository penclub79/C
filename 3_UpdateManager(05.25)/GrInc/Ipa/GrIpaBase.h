/*
	Gause image process adaptor base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaBase
#define	_EPC_GrIpaBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance

// script
#define	E_GrIpaScptSctFcc					0x4853		/* SH */
#define	E_GrIpaScptObjFcc					0x534F		/* OS */
#define	E_GrIpaScptChainFcc				0x5343		/* CS */

// section type
#define	E_GrIpaSectObjPara				1			/* object parametter */
#define	E_GrIpaSectObjChain				2			/* object relationship */


#define	E_GrIpaSectUserBase				0x80	/* section user */

// adaptor type
#define	E_GrIpaAdptTypeNone				0			/* none */
#define	E_GrIpaAdptTypeSrcIn			1			/* source in */
#define	E_GrIpaAdptTypeMonOut			2			/* monitor out */
#define	E_GrIpaAdptTypeSplitYuv		3			/* split YUV */
#define	E_GrIpaAdptTypeBindYuv		4			/* bind YUV */
#define	E_GrIpaAdptTypeBinary			5			/* binary 2D stream */
#define	E_GrIpaAdptTypeWdr				6			/* WDR */

// script id
#define	E_GrIpaScptIdNone					0			/* script id none */
#define	E_GrIpaScptIdBase					1			/* script id base */

// steram type
#define	E_GrIpaStrmTypeNone				0			/* stream type none */
#define	E_GrIpaStrmTypeImage			1			/* stream type still image, St_GrIpaStrmInfoImg  */
#define	E_GrIpaStrmType2dByte			2			/* stream type 2D array byte type, St_GrIpaStrmInfo2d  */




#ifdef GR_IPA_EDIT

#define	E_GrIpaAdptInfoCnt				6			/* adaptor information count */



#endif

//====================================================================
//global macro

//====================================================================
//global type

//---------------------------------
// stream info
//---------------------------------
typedef struct St_GrIpaStrmInfoImg
{
	__u32	FccImg;		// image FourCC
	__u32	ResX;
	__u32	ResY;
	__u32	Pitch;
}	*Ptr_GrIpaStrmInfoImg;

typedef struct St_GrIpaStrmInfo2d
{
	__u32	ResX;
	__u32	ResY;
}	*Ptr_GrIpaStrmInfo2d;

//---------------------------------
// script 
//---------------------------------
typedef struct St_GrIpaScptObjHead
{
	__u16	Identify;
	__u16	Type;
	__u16	ScptId;
	__u16	Size;
}	*Ptr_GrIpaScptObjHead;

typedef struct St_GrIpaScptParaHead
{
	__u16	Type;		// Adaptor type
	__u16	Size;		// parameter size
}	*Ptr_GrIpaScptParaHead;

typedef struct St_GrIpaScptChainHead
{
	__u16	Identify;
	__u16	_rsvd0;
	__u16	ScptId;
	__u16	Size;
}	*Ptr_GrIpaScptChainHead;

typedef struct St_GrIpaScptChainItem
{
	__u16	ScptId;
	__u8	PlugId;
	__u8	ConsentId;
}	*Ptr_GrIpaScptChainItem;


typedef struct St_GrIpaScptSctHead
{
	__u16	IdSection;
	__u8	Type;
	__u8	Para;
	__u32	Size;
}	*Ptr_GrIpaScptSctHead;

#ifdef GR_IPA_EDIT

typedef struct St_GrIpaAdptInfo
{
	__u16	TypeId;
	Def_WstrName	StrName;

}	*Ptr_GrIpaAdptInfo;

typedef struct St_GrIpaAdptParaInfo
{
	WCHAR*	StrName;
	__u8		VarType;
	__u8		Cnt;
	void*		Ptr;
	WCHAR*	StrDesc;
}	*Ptr_GrIpaAdptParaInfo;

#endif

//====================================================================
//global class

//====================================================================
//global function

#ifdef GR_IPA_EDIT

WCHAR*	GrIpaAdoptNameGet( __u16 A_Type );

#endif

//====================================================================
//out Enviroment conditional
#endif

