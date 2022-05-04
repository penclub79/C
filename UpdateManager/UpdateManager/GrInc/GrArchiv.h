/*
	Arhive tool

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrArchiv
#define	_EPC_GrArchiv

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>

#include <GrFileCtrl.h>

//====================================================================
//constance

#define	E_GrArchivHeadFcc						Mac_GrMakeFcc('A','R','V','1')
#define	E_GrArchivVer								1

#define	E_GrArchivChunkFcc					Mac_GrMakeFcc('C','H','U','K')

#define	E_GrArchivUserDataSize			12

#define	E_GrArchivChunkTypeFoler		0
#define	E_GrArchivChunkTypeFile			1
#define	E_GrArchivChunkTypeRaw			2
#define	E_GrArchivChunkTypeNandRaw	3		/* not support */

#define	E_GrArchivChunkTypeCnt			4

#define	E_GrArchivFileTypeNormal		0
#define	E_GrArchivFileTypeLink			1
#define	E_GrArchivFileTypeCharDev		2
#define	E_GrArchivFileTypeBlockDev	3

#define	E_GrArchivInputTypeNone			0
#define	E_GrArchivInputTypeMemory		1
#define	E_GrArchivInputTypeFile			2

#define	E_GrArchivSectionMaxCnt			16

#define	E_GrArchivWorkBufSize				0x40000		/* 256 kbyte */

#define	E_GrArchivRunModeChunkHead	0
#define	E_GrArchivRunModeFolder			1
#define	E_GrArchivRunModeFileHead		2
#define	E_GrArchivRunModeRaw				3
#define	E_GrArchivRunModeNandRaw		4
#define	E_GrArchivRunModeWriteFile	5
#define	E_GrArchivRunModeWriteRaw		6


//#define	E_GrArchivMode

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrArchivHeader
{
	__u32	Fcc;
	__u32	Ver;
	__u32	PayloadSize;
	__u32	PayloadChk;

	__u32	DataId;
	__u32	UserData[3];

	__u32	_rsvd0[8];

}	*Ptr_GrArchivHeader;

typedef struct St_GrArchivChunkHead
{
	__u32	Fcc;
	__u8	Section;
	__u8	Type;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u32	Size;
	__u32	Para;
}	*Ptr_GrArchivChunkHead;

typedef struct St_GrArchivChunkFolderA
{
	__u32	Owner;
	Def_StrGrFnPath	StrPath;
}	*Ptr_GrArchivChunkFolderA;

typedef struct St_GrArchivChunkFileA
{
	__u32	Owner;
	__u8	Type;
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;
	__u32	Size;
	__u32	Para;
	Def_StrGrFnPath	StrPath;

}	*Ptr_GrArchivChunkFileA;

typedef struct St_GrArchivSectionItem
{
	Def_StrGrFnPath	StrDev;		// output device
}	*Ptr_GrArchivSectionItem;


//====================================================================
//class

class	Cls_GrArchDec
{
	protected:
		__u8	m_InType;
		void*	m_PtrIn;
		Cls_GrFileCtrl*	m_FcIn;

		__u32	m_InSize;
		__u32	m_Progress;

		__u8	m_RunMode;

#ifndef LINUX_APP
		BOOL8	m_IsNoEmu;
#endif

		St_GrArchivSectionItem	m_SectTbl[E_GrArchivSectionMaxCnt];

		St_GrArchivChunkHead	m_ChukHd;

		St_GrArchivChunkFileA	m_FileHd;
		Cls_GrFileCtrl*	m_FcFile;

		__u32	m_FileRemain;

		__u8	m_WorkBuf[E_GrArchivWorkBufSize];

		BOOL8	LcInDataRead(void* A_PtrBuf, __u32 A_Size);
		BOOL8	LcInDataSkip(__u32 A_Size);
		BOOL8	LcArchHeadCheck(void);

		void	LcPrcChunkHead(void);
		void	LcPrcFolder(void);
		void	LcPrcFileHead(void);
		void	LcPrcFileNormal(void);
		void	LcPrcFileLink(void);
		void	LcPrcFileCdev(void);
		void	LcPrcFileBdev(void);
		void	LcPrcWriteFile(void);
		virtual	void	LcPrcRaw(void);
		virtual	void	LcPrcWriteRaw(void);
		
	public:
    Cls_GrArchDec( BOOL8 A_IsNoEmu );
    virtual		~Cls_GrArchDec();

		BOOL8	SectionDeviceSet(__u8 A_Section, char* A_StrName);
		void	Reset(void);

		BOOL8	OpenFromMemory(void* A_PtrBuf, __u32 A_Size);
		BOOL8	OpenFromFile(char* A_StrFn, BOOL8 A_IsNoEmu);

		BOOL8	DoProcess(void);

		BOOL8	Close(void);

		BOOL8	ExtracFromMemory(void* A_PtrBuf, __u32 A_Size);
		BOOL8	ExtracFromFile(char* A_StrFn, BOOL8 A_IsNoEmu);

		__u32	TotalSizeGet(void);
		__u32	ProgressSizeGet(void);
};
	
//====================================================================
//global function
BOOL8	GrArchivGetFileInfo(char* A_StrFn, __u32* A_PtrRtPayloadSize, __u32* A_PtrRtChksum, __u32* A_PtrRtId, void* A_PtrRtUserData,BOOL8 A_IsNoEmu);
BOOL8	GrArchivGetFileInfo(WCHAR* A_StrFn, __u32* A_PtrRtPayloadSize, __u32* A_PtrRtChksum, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu);

__u32	GrArchivCalChksum(__u32 A_PrevChksum, void* A_PtrBuf, __u32 A_Size);
BOOL8	GrArchivGetFileChksum(char* A_StrFn, __u32 A_PayloadSize, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtChksum, BOOL8 A_IsNoEmu);
BOOL8	GrArchivGetFileChksum(WCHAR* A_StrFn, __u32 A_PayloadSize, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtChksum, BOOL8 A_IsNoEmu);
BOOL8	GrArchivChkFile(char* A_StrFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu);
BOOL8	GrArchivChkFile(WCHAR* A_StrFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu);

BOOL8	GrArchivAddFile(char* A_StrFnArchiv, char* A_StrFnAdd, __u8 A_Section, char* A_StrSetFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, void* A_PtrUserData = NULL, BOOL8 A_IsNoEmu = TRUE);
BOOL8	GrArchivAddFile(WCHAR* A_StrFnArchiv, WCHAR* A_StrFnAdd, __u8 A_Section, WCHAR* A_StrSetFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, void* A_PtrUserData = NULL, BOOL8 A_IsNoEmu = TRUE);

//====================================================================
//out Enviroment conditional
#endif

