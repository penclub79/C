/*
 osd skin object
*/

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrGdibTool.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>
#include	<GrGdibBb2.h>
#include	<GrGdibByt.h>
#include	<GrGdibPk4.h>
#include	<GrGdibPk4a.h>
#include	<GrGdibPlt8.h>
#include	<GrGdibUyvy.h>
#include	<GrFileCtrl.h>
#include	<stdlib.h>
#include	<GrBmpTool.h>
#include	<GrGdibRgb32.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibCreate(	__u32 A_Fcc, __u32 A_ResX, __u32 A_ResY, 
													__u32 A_Pitch, void* A_PtrDib, 
													__u32 A_PltBitCnt, void* A_PtrPlt )
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		if ( 8 == A_PltBitCnt )
		{
			Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibPlt8( A_Fcc, A_ResX, A_ResY, A_Pitch, A_PtrDib, A_PtrPlt );
		}
		else
		{
			switch( A_Fcc )
			{
				case	E_GrFccRGB:
				case	E_GrFccRGB888:
				case	E_GrFccBGR888:
				case	E_GrFccUVYA:
				case	E_GrFccABGR8888:
					//byte
					Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibByt(	A_Fcc, A_ResX, A_ResY, A_Pitch, 
						A_PtrDib, A_PltBitCnt, A_PtrPlt );
					break;
				case	E_GrFccARGB8888:
					Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibRgb32(	A_Fcc, A_ResX, A_ResY, A_Pitch, 
						A_PtrDib, A_PltBitCnt, A_PtrPlt );
					break;
				case	E_GrFccYUY2:
					//pack 4byte
					Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibPk4(	A_Fcc, A_ResX, A_ResY, A_Pitch, 
						A_PtrDib, A_PltBitCnt, A_PtrPlt );
					break;
				case	E_GrFccUYVY:
				case  E_GrFccUYVYF:
					//uyvy 
					Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibUyvy(	A_Fcc, A_ResX, A_ResY, A_Pitch, 
											A_PtrDib, A_PltBitCnt, A_PtrPlt );
					break;
				case	E_GrFccRGB555:
				case	E_GrFccRGB565:
				case	E_GrFccARGB1555:
				case	E_GrFccARGB4444: 
					//bit in 2byte 
					Tv_Result	=	(Cls_GrGdib*) new Cls_GrGdibBb2(	A_Fcc, A_ResX, A_ResY, A_Pitch, 
						A_PtrDib, A_PltBitCnt, A_PtrPlt );
					break;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibOvlp( Ptr_GrGdib A_PtrGdib )
{
	// local -------------------
		void*				Tv_PtrPlt;
		__u32				Tv_PltBit;
	// code --------------------
		//init
		Tv_PltBit	=	0;
		Tv_PtrPlt	=	GrGdibGetPalletPtr( A_PtrGdib );
		if ( NULL != Tv_PtrPlt )
		{
			Tv_PltBit	=	A_PtrGdib->BitCnt;
		}

		return	GrGdibCreate( A_PtrGdib->FccImg, A_PtrGdib->ResX, A_PtrGdib->ResY, A_PtrGdib->Pitch,
													GrGdibGetDibPtr( A_PtrGdib ), Tv_PltBit, Tv_PtrPlt );
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibClone( Cls_GrGdib* A_ObjSrc )
{
	// local -------------------
		void*				Tv_PtrPlt;
		__u32				Tv_PltBit;
	// code --------------------
		Tv_PltBit	=	0;
		Tv_PtrPlt	=	A_ObjSrc->GetPltPtr();
		if ( NULL != Tv_PtrPlt )
		{
			Tv_PltBit	=	A_ObjSrc->GetPixelBit();
		}
		return	GrGdibCreate( A_ObjSrc->GetFcc(), A_ObjSrc->GetResX(), A_ObjSrc->GetResY(), A_ObjSrc->GetDibPitch(),
													A_ObjSrc->GetDibPtr(), Tv_PltBit, Tv_PtrPlt );
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibLoadFile(char* A_StrFn, __u32 A_LimResX, __u32 A_LimResY)
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
		St_GrGdib		Tv_GdibHd;
		Cls_GrFileCtrl*	Tv_ObjFl;
		void*				Tv_PtrPltTg;
		void*				Tv_PtrDibTg;
		__u32				Tv_PltBitCnt;
	// code --------------------
		//init
		Tv_Result	=	NULL;

		//load gdib head
		Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			if ( sizeof(St_GrGdib) == Tv_ObjFl->Read( &Tv_GdibHd, sizeof(St_GrGdib) ) )
			{
				//check current head
				if ( (E_GrGdibFcc == Tv_GdibHd.FccGdib) && (A_LimResX >= Tv_GdibHd.ResX) && (A_LimResY >= Tv_GdibHd.ResY) )
				{
					Tv_PltBitCnt	=	0;
					if ( Tv_GdibHd.IsPlt )
					{
						Tv_PltBitCnt	=	Tv_GdibHd.BitCnt;
					}
					
					Tv_Result	=	GrGdibCreate( Tv_GdibHd.FccImg, Tv_GdibHd.ResX, Tv_GdibHd.ResY, Tv_GdibHd.Pitch,
																		NULL, Tv_PltBitCnt, NULL );
					if ( NULL != Tv_Result )
					{
						//load pallete
						Tv_PtrPltTg	=	Tv_Result->GetPltPtr();
						if ( Tv_GdibHd.IsPlt && (NULL != Tv_PtrPltTg) )
						{
							Tv_ObjFl->Read( Tv_PtrPltTg, ((1 << Tv_PltBitCnt) * 4) );
						}
						//load dib
						Tv_PtrDibTg	=	Tv_Result->GetDibPtr();
						if ( NULL != Tv_PtrDibTg )
						{
							Tv_ObjFl->Read( Tv_PtrDibTg, (Tv_GdibHd.Pitch * Tv_GdibHd.ResY) );
						}
					}
				}
				
			}
		}
		delete	Tv_ObjFl;
			
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibLoadBmp(char* A_StrFn, __u32 A_LimResX, __u32 A_LimResY)
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
		St_GrBmpFlHead	Tv_BmpHd;
		BOOL8	Tv_IsRvsY;
		__u32	Tv_ResY;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u32	Tv_Fcc;
		__u32	Tv_PltBitCnt;
		void*	Tv_PtrPlt;
		void*	Tv_PtrDib;
		__u32	Tv_DibSize;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_IsRvsY		=	TRUE;
		//load bmp header
		Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			if ( sizeof(Tv_BmpHd) == Tv_ObjFl->Read( &Tv_BmpHd, sizeof(Tv_BmpHd) ) )
			{
				//check header
				if ( (E_GrBmpFileId == Tv_BmpHd.Id) && ( E_GrFccRGB == Tv_BmpHd.Bi.Compression ) )
				{
					//check flip y
					Tv_ResY	=	(__u32)Tv_BmpHd.Bi.Height;
					if ( 0 > Tv_BmpHd.Bi.Height )
					{
						Tv_ResY	=	(__u32)( -Tv_BmpHd.Bi.Height );
						Tv_IsRvsY		=	FALSE;
					}
					//check resolution
					if ( (A_LimResX >= (__u32)Tv_BmpHd.Bi.Width) && (A_LimResY >= Tv_ResY) )
					{
						//get info
						Tv_PltBitCnt	=	Tv_BmpHd.Bi.BitCount;
						switch( Tv_BmpHd.Bi.BitCount )
						{
							case 1:
								Tv_Fcc				=	E_GrFccRGB888;
								break;
							case 4:
								Tv_Fcc				=	E_GrFccRGB888;
								break;
							case 8:
								Tv_Fcc				=	E_GrFccRGB888;
								break;
							case 16:
								Tv_Fcc				=	E_GrFccRGB555;
								Tv_PltBitCnt	=	0;
								break;
							case 32:
								Tv_Fcc				=	E_GrFccARGB8888;
								Tv_PltBitCnt	=	0;
								break;
							default:
								Tv_Fcc				=	E_GrFccRGB888;
								Tv_PltBitCnt	=	0;
								break;
						}
						
						//make 
						Tv_Result	=	GrGdibCreate( Tv_Fcc, Tv_BmpHd.Bi.Width, Tv_ResY, 0, NULL, Tv_PltBitCnt, NULL );
						if ( NULL != Tv_Result )
						{
							//load pallete
							if ( 0 != Tv_PltBitCnt )
							{
								Tv_PtrPlt	=	Tv_Result->GetPltPtr();
								if ( NULL != Tv_PtrPlt )
								{
									Tv_ObjFl->Read( Tv_PtrPlt, ( 4 * (1 << Tv_PltBitCnt) ) );
								}
							}
							//load contents
							Tv_ObjFl->Seek( (__s64)Tv_BmpHd.OfsDib );
							Tv_DibSize	=	Tv_Result->GetDibSize();
							if ( Tv_IsRvsY )
							{
								Tv_PtrDib	=	(void*)malloc( Tv_DibSize );
							}
							else
							{
								Tv_PtrDib		=	Tv_Result->GetDibPtr();
							}
							Tv_ObjFl->Read( Tv_PtrDib, Tv_DibSize );
							//patch image
							if ( 1 == Tv_PltBitCnt )
							{
								//1bit image
								GrDumySwapByte1Bit( Tv_PtrDib, Tv_DibSize );
							}
							else if ( 4 == Tv_PltBitCnt )
							{
								//4bit image
								GrDumySwapByte4Bit( Tv_PtrDib, Tv_DibSize );
							}
							//reverse top and low
							if ( Tv_IsRvsY )
							{
								GrDrawDibCpyRsvVt(	Tv_PtrDib, Tv_Result->GetDibPitch(), 
																		Tv_Result->GetDibPtr(), Tv_Result->GetDibPitch(), 
																		Tv_ResY );
								free( Tv_PtrDib );
							}
							
						}
					}
				}
			}
		}
		delete	Tv_ObjFl;
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibLoadBmp( WCHAR* A_StrFn, __u32 A_LimResX, __u32 A_LimResY )
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
		St_GrBmpFlHead	Tv_BmpHd;
		BOOL8	Tv_IsRvsY;
		__u32	Tv_ResY;
		Cls_GrFileCtrl*	Tv_ObjFl;
		__u32	Tv_Fcc;
		__u32	Tv_PltBitCnt;
		void*	Tv_PtrPlt;
		void*	Tv_PtrDib;
		__u32	Tv_DibSize;
	// code --------------------
		//init
		Tv_Result		=	NULL;
		Tv_IsRvsY		=	TRUE;
		//load bmp header
		Tv_ObjFl	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( A_StrFn, FALSE, FALSE );
		if ( Tv_ObjFl->IsOpened() )
		{
			if ( sizeof(Tv_BmpHd) == Tv_ObjFl->Read( &Tv_BmpHd, sizeof(Tv_BmpHd) ) )
			{
				//check header
				if ( (E_GrBmpFileId == Tv_BmpHd.Id) && ( E_GrFccRGB == Tv_BmpHd.Bi.Compression ) )
				{
					//check flip y
					Tv_ResY	=	(__u32)Tv_BmpHd.Bi.Height;
					if ( 0 > Tv_BmpHd.Bi.Height )
					{
						Tv_ResY	=	(__u32)( -Tv_BmpHd.Bi.Height );
						Tv_IsRvsY		=	FALSE;
					}
					//check resolution
					if ( (A_LimResX >= (__u32)Tv_BmpHd.Bi.Width) && (A_LimResY >= Tv_ResY) )
					{
						//get info
						Tv_PltBitCnt	=	Tv_BmpHd.Bi.BitCount;
						switch( Tv_BmpHd.Bi.BitCount )
						{
						case 1:
							Tv_Fcc				=	E_GrFccRGB888;
							break;
						case 4:
							Tv_Fcc				=	E_GrFccRGB888;
							break;
						case 8:
							Tv_Fcc				=	E_GrFccRGB888;
							break;
						case 16:
							Tv_Fcc				=	E_GrFccRGB555;
							Tv_PltBitCnt	=	0;
							break;
						case 32:
							Tv_Fcc				=	E_GrFccARGB8888;
							Tv_PltBitCnt	=	0;
							break;
						default:
							Tv_Fcc				=	E_GrFccRGB888;
							Tv_PltBitCnt	=	0;
							break;
						}

						//make 
						Tv_Result	=	GrGdibCreate( Tv_Fcc, Tv_BmpHd.Bi.Width, Tv_ResY, 0, NULL, Tv_PltBitCnt, NULL );
						if ( NULL != Tv_Result )
						{
							//load pallete
							if ( 0 != Tv_PltBitCnt )
							{
								Tv_PtrPlt	=	Tv_Result->GetPltPtr();
								if ( NULL != Tv_PtrPlt )
								{
									Tv_ObjFl->Read( Tv_PtrPlt, ( 4 * (1 << Tv_PltBitCnt) ) );
								}
							}
							//load contents
							Tv_ObjFl->Seek( (__s64)Tv_BmpHd.OfsDib );
							Tv_DibSize	=	Tv_Result->GetDibSize();
							if ( Tv_IsRvsY )
							{
								Tv_PtrDib	=	(void*)malloc( Tv_DibSize );
							}
							else
							{
								Tv_PtrDib		=	Tv_Result->GetDibPtr();
							}
							Tv_ObjFl->Read( Tv_PtrDib, Tv_DibSize );
							//patch image
							if ( 1 == Tv_PltBitCnt )
							{
								//1bit image
								GrDumySwapByte1Bit( Tv_PtrDib, Tv_DibSize );
							}
							else if ( 4 == Tv_PltBitCnt )
							{
								//4bit image
								GrDumySwapByte4Bit( Tv_PtrDib, Tv_DibSize );
							}
							//reverse top and low
							if ( Tv_IsRvsY )
							{
								GrDrawDibCpyRsvVt(	Tv_PtrDib, Tv_Result->GetDibPitch(), 
									Tv_Result->GetDibPtr(), Tv_Result->GetDibPitch(), 
									Tv_ResY );
								free( Tv_PtrDib );
							}

						}
					}
				}
			}
		}
		delete	Tv_ObjFl;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrGdibCvt(	Cls_GrGdib* A_ObjSrc, __u32 A_Fcc, 
												__u32 A_Pitch, void* A_PtrDib, 
												__u32 A_PltBitCnt, void* A_PtrPlt )
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
	// code --------------------
		Tv_Result	=	GrGdibCreate( A_Fcc, A_ObjSrc->GetResX(), A_ObjSrc->GetResY(), 
								A_Pitch, A_PtrDib, A_PltBitCnt, A_PtrPlt );
		if ( NULL != Tv_Result )
		{
			A_ObjSrc->CvtTo( Tv_Result );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

