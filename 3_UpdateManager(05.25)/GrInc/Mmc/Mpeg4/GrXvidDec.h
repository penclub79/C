/*
	//xvid decode header
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrClsXvidDec
#define	_EPC_GrClsXvidDec

//====================================================================
// uses
#ifdef	LINUX_APP
	#include	<sys/time.h>
	#include	<unistd.h>
#else
	#include	<windows.h>
#endif
#include <Mpeg4/xvid.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrXvidDec		
{
	private:
		//init data
		__u32		m_Fcc;
		__u32		m_MaxResX;
		__u32		m_MaxResY;
		void*		m_PtrDib;				//out image pointer
		__u32		m_Pitch;
		__u32		m_DecResX;			//last res x
		__u32		m_DecResY;			//last res y
		void*		m_HndlXvid;			//handle xvid

		BOOL8		m_IsInited;			//initialized
		BOOL8		m_IsWithoutVol;	//without vol
		__u8		m_VolTbl[30];
		__s32			m_Csp;	

	protected:
		BOOL8		LcDecVol( __u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterace );
		__u32		LcDecData( void* A_PtrData, __u32 A_DataSize, BOOL8* A_PtrRtIsVol );
	public:	
		Cls_GrXvidDec();
		virtual	~Cls_GrXvidDec();	

		//info
		void		GetDecRes( __u32* A_PtrRtResX, __u32* A_PtrRtResY );

		BOOL8		DecInit( __u32 A_Fcc, __u32 A_MaxResX, __u32 A_MaxResY, void* A_PtrDib, __u32 A_Pitch, BOOL8 A_IsWithoutVol );
		BOOL8		DecFrame( void* A_PtrData, __u32 A_DataSize, __u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterace, BOOL8* A_PtrRtIsOut );
		void		DecFinish( void );

};
//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif