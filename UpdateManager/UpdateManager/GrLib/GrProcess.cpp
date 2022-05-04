/*
	process library

*/

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrProcess.h>

#ifdef		LINUX_APP
	#include	<unistd.h>
	#include	<sched.h>
#endif

//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
void		GrPrcSleep( __u32 A_Mili )
{
	#ifdef LINUX_APP
		if ( 1000 <= A_Mili )
		{
			sleep( (A_Mili / 1000) );
		}
		else if ( 0 != A_Mili )
		{
			usleep( (A_Mili * 1000) );
		}
		else
		{
			usleep( 1 );
		}

	#else
		Sleep( A_Mili );
	#endif
}
//--------------------------------------------------------------------
BOOL8		GrPrcThrdCreate( Ptr_GrPrcThrd A_PtrMng, Fnc_GrPrcThrdProc A_Proc, Def_GrPrcThrdPara A_Para )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		#ifdef LINUX_APP
			if ( 0 <= pthread_create( &A_PtrMng->Hndl, NULL, A_Proc, A_Para ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			A_PtrMng->HndlThrd	=	CreateThread( NULL, 0, A_Proc, A_Para, 0, (LPDWORD)&A_PtrMng->ThrdId );
			if ( NULL != A_PtrMng->ThrdId )			
			{
				Tv_Result		=	TRUE;
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrPrcThrdRslt		GrPrcThrdRelease( Ptr_GrPrcThrd A_PtrMng )
{
	// local -------------------
		Def_GrPrcThrdRslt		Tv_Result;
	// code --------------------
		#ifdef LINUX_APP
			pthread_join( A_PtrMng->Hndl, &Tv_Result );
		#else
			Tv_Result	=	0;
			if ( WAIT_OBJECT_0 == WaitForSingleObject( A_PtrMng->HndlThrd, INFINITE ) )
			{
				GetExitCodeThread( A_PtrMng->HndlThrd, (LPDWORD)&Tv_Result );
				CloseHandle( A_PtrMng->HndlThrd );
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcThrdTerminate( Ptr_GrPrcThrd A_PtrMng )
{
	// local -------------------
		BOOL8		Tv_Result;
		#ifdef LINUX_APP
			Def_GrPrcThrdRslt		Tv_Rval;
		#endif
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		#ifdef LINUX_APP
			pthread_cancel( A_PtrMng->Hndl );
			pthread_join( A_PtrMng->Hndl, &Tv_Rval );
			Tv_Result		=	TRUE;
		#else
			Tv_Result		=	TerminateThread( A_PtrMng->HndlThrd, 0 );
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrPrcThrdId	GrPrcThrdGetIdByMng( Ptr_GrPrcThrd A_PtrMng )
{
		#ifdef LINUX_APP
			return	A_PtrMng->Hndl;
		#else
			return	A_PtrMng->ThrdId;
		#endif
}
//--------------------------------------------------------------------
Def_GrPrcThrdId	GrPrcThrdGetCurId( void )
{
		#ifdef LINUX_APP
			return	pthread_self();
		#else
			return	GetCurrentThreadId();
		#endif
}
//--------------------------------------------------------------------
BOOL8	GrPrcThrdIsEqual( Def_GrPrcThrdId A_ThrdA, Def_GrPrcThrdId A_ThrdB )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		#ifdef LINUX_APP
			if ( 0 != pthread_equal( A_ThrdA, A_ThrdB ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			if ( A_ThrdA == A_ThrdB )
			{
				Tv_Result	=	TRUE;
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcCritInit( Ptr_GrPrcCritSct A_PtrCrit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		#ifdef LINUX_APP
			Tv_Result		=	FALSE;
			if ( 0 == pthread_mutex_init( (pthread_mutex_t*)A_PtrCrit, NULL ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			InitializeCriticalSection( (LPCRITICAL_SECTION)A_PtrCrit );
			Tv_Result		=	TRUE;
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcCritDel( Ptr_GrPrcCritSct A_PtrCrit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		#ifdef LINUX_APP
			Tv_Result		=	FALSE;
			if ( 0 == pthread_mutex_destroy( (pthread_mutex_t*)A_PtrCrit ) )
			{
				Tv_Result	=	TRUE;
			}
		#else
			DeleteCriticalSection( (LPCRITICAL_SECTION)A_PtrCrit );
			Tv_Result	=	TRUE;
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcCritEnter( Ptr_GrPrcCritSct A_PtrCrit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		#ifdef	LINUX_APP
			Tv_Result		=	FALSE;
			if ( 0 == pthread_mutex_lock( (pthread_mutex_t*)A_PtrCrit ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			EnterCriticalSection( (LPCRITICAL_SECTION)A_PtrCrit );
			Tv_Result		=	TRUE;
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcCritTryEnter( Ptr_GrPrcCritSct A_PtrCrit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;
		#ifdef	LINUX_APP
			if ( 0 == pthread_mutex_trylock( (pthread_mutex_t*)A_PtrCrit ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			if ( TryEnterCriticalSection( (LPCRITICAL_SECTION)A_PtrCrit ) )
			{
				Tv_Result		=	TRUE;
			}
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrPrcCritLeave( Ptr_GrPrcCritSct A_PtrCrit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		#ifdef	LINUX_APP
			Tv_Result		=	FALSE;
			if ( 0 == pthread_mutex_unlock( (pthread_mutex_t*)A_PtrCrit ) )
			{
				Tv_Result		=	TRUE;
			}
		#else
			LeaveCriticalSection( (LPCRITICAL_SECTION)A_PtrCrit );
			Tv_Result		=	TRUE;
		#endif

		return	Tv_Result;
}
//--------------------------------------------------------------------


