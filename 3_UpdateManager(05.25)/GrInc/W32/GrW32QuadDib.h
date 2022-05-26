/*
	win32 dib quad
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrW32QuadDib
#define	_EPC_GrW32QuadDib

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Dvr/GrDvrQuad.h>
	
//====================================================================
//constance

#define		E_GrW32QuadDibMaxChCnt		16

//max input resolution
#define		E_GrW32QuadDibInResMaxX		704
#define		E_GrW32QuadDibInResMaxY		576

//text pos
#define		E_GrW32QuadDibTxtGapX			6
#define		E_GrW32QuadDibTxtGapY			6

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class
class Cls_GrW32QuadDib	:	public	Cls_GrDvrQuad
{
	private:
		__u32				m_DibBitCnt;
		HBITMAP			m_HbtmScr;
		HDC					m_HdcScr;
		//HGDIOBJ			m_HbtmOld;
		//HGDIOBJ			m_HfontOld;
		
		Def_StrName	m_StrId[E_GrW32QuadDibMaxChCnt];
	protected:
		void		DrawChWaku( __u32 A_Ch );
		void		DrawAllChWaku( void );
		void		FreeScr( void );
	public:
		Cls_GrW32QuadDib( __u32 A_DibBitCnt );
		virtual	~Cls_GrW32QuadDib();

		virtual	void		ClearQuad( void );

		virtual	void		SetScrRes( __u32 A_ResX, __u32 A_ResY );

		virtual	BOOL8		QuadSetMode( __u32 A_Mode, __u32 A_Page );

		//update dib
		void		SetChDib( __u32 A_Ch, BITMAPINFO* A_PtrBi, void* A_PtrDib, __u16 A_ResX, __u16 A_ResY );

		//draw function
		HDC			GetHdc( void );
		HBITMAP	GetBitmap( void );
		void		SetChName( __u32 A_Ch, __s8* A_Str );
		
};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

