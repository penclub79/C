/*
	emulation able file control

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrFileCtrl
#define	_EPC_GrFileCtrl

//====================================================================
// uses
#include <GrTypeBase.h>

#ifdef LINUX_APP

#include <unistd.h>

#endif

//====================================================================
//constance

#ifdef LINUX_APP

#define	E_GrFileCtrlPosBegin		SEEK_SET
#define	E_GrFileCtrlPosNow			SEEK_CUR
#define	E_GrFileCtrlPosEnd			SEEK_END

#else

#define	E_GrFileCtrlPosBegin		FILE_BEGIN
#define	E_GrFileCtrlPosNow			FILE_CURRENT
#define	E_GrFileCtrlPosEnd			FILE_END

#endif

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrFileCtrl
{
	private:
		BOOL8			m_IsOpened;
		BOOL8			m_IsSync;

		#ifdef LINUX_APP
			__s32				m_HndlFile;
		#else
			HANDLE		m_HndlFile;
		#endif
	protected:

	public:
    Cls_GrFileCtrl( char* A_StrFileName, BOOL8 A_IsWrite, BOOL8 A_IsCreateAble, 
										BOOL8 A_IsExcute = FALSE, BOOL8 A_IsNonEmu = FALSE, BOOL8 A_IsSync = FALSE, 
										BOOL8 A_IsDirect = FALSE );
		Cls_GrFileCtrl( WCHAR* A_StrFileName, BOOL8 A_IsWrite, BOOL8 A_IsCreateAble, 
										BOOL8 A_IsExcute = FALSE, BOOL8 A_IsNonEmu = FALSE, BOOL8 A_IsSync = FALSE, 
										BOOL8 A_IsDirect = FALSE );
    virtual		~Cls_GrFileCtrl();

		__s64	Seek( __s64 A_Position, __u32 A_Mode = E_GrFileCtrlPosBegin);
		__u32		Read( void* A_Buf, __u32 A_Size );
		__u32		Write( void* A_Buf, __u32 A_Size );

		__s64	GetPos( void );
		__s64	GetSize( void );

		BOOL8		IsOpened( void );
		void	Flush(void);
};
	
//====================================================================
//global function

__u32	GrFileCtrlRead(char* A_StrFn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsNonEmu = FALSE);
__u32	GrFileCtrlRead( WCHAR* A_StrFn, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsNonEmu = FALSE );
__u32	GrFileCtrlWrite(char* A_StrFn, BOOL8 A_IsCreate, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsNonEmu = FALSE, BOOL8 A_IsSync = FALSE, BOOL8 A_IsAdd = FALSE);
__u32	GrFileCtrlWrite( WCHAR* A_StrFn, BOOL8 A_IsCreate, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsNonEmu = FALSE, BOOL8 A_IsSync = FALSE, BOOL8 A_IsAdd = FALSE );

void	GrFileCtrlEmuPathSet(char* A_StrPath);
void	GrFileCtrlEmuPathSet( WCHAR* A_StrPath );
char*	GrFileCtrlEmuPathGetA( void );
WCHAR*	GrFileCtrlEmuPathGetW( void );

//====================================================================
//out Enviroment conditional
#endif

