/*
	linux on win32 emulation
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrProcess
#define	_GrProcess

//====================================================================
// uses
#ifdef LINUX_APP
	#include	<pthread.h>
#endif
	
//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
#ifdef LINUX_APP
	#define Def_GrPrcThrdId		pthread_t
	typedef pthread_mutex_t		St_GrPrcCritSct ,*Ptr_GrPrcCritSct;
#else
	#define Def_GrPrcThrdId		__u32
	typedef CRITICAL_SECTION	St_GrPrcCritSct ,*Ptr_GrPrcCritSct;
#endif

typedef	struct St_GrPrcThrd 
{
	#ifdef LINUX_APP
		pthread_t		Hndl;
	#else
		HANDLE		HndlThrd;
		__u32			ThrdId;
	#endif
}	*Ptr_GrPrcThrd;

#ifdef LINUX_APP
	#define		Def_GrPrcThrdRslt		void*
	#define		Def_GrPrcThrdClcb		
	#define		Def_GrPrcThrdPara		void*
	typedef		void*	(*Fnc_GrPrcThrdProc)( void* A_Para );
#else
	#define		Def_GrPrcThrdRslt		DWORD	
	#define		Def_GrPrcThrdClcb		WINAPI	
	#define		Def_GrPrcThrdPara		LPVOID
	typedef		DWORD (WINAPI *Fnc_GrPrcThrdProc)( LPVOID A_Para );
#endif

//====================================================================
//class

//====================================================================
//global function

void		GrPrcSleep( __u32 A_Mili );

BOOL8		GrPrcThrdCreate( Ptr_GrPrcThrd A_PtrMng, Fnc_GrPrcThrdProc A_Proc, Def_GrPrcThrdPara A_Para );
Def_GrPrcThrdRslt		GrPrcThrdRelease( Ptr_GrPrcThrd A_PtrMng );
BOOL8		GrPrcThrdTerminate( Ptr_GrPrcThrd A_PtrMng );

Def_GrPrcThrdId	GrPrcThrdGetIdByMng( Ptr_GrPrcThrd A_PtrMng );
Def_GrPrcThrdId	GrPrcThrdGetCurId( void );
BOOL8		GrPrcThrdIsEqual( Def_GrPrcThrdId A_ThrdA, Def_GrPrcThrdId A_ThrdB );

BOOL8		GrPrcCritInit( Ptr_GrPrcCritSct A_PtrCrit );
BOOL8		GrPrcCritDel( Ptr_GrPrcCritSct A_PtrCrit );
BOOL8		GrPrcCritEnter( Ptr_GrPrcCritSct A_PtrCrit );
BOOL8		GrPrcCritTryEnter( Ptr_GrPrcCritSct A_PtrCrit );
BOOL8		GrPrcCritLeave( Ptr_GrPrcCritSct A_PtrCrit );

//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


