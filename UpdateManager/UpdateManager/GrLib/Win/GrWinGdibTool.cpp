

//====================================================================
// uses

#include <Win\GrWinGdibTool.h>

#include <GrDumyTool.h>
#include <GrBufTool.h>
#include <GrStrTool.h>
#include <GrDrawTool.h>

//====================================================================
//const


//====================================================================
//global var


//====================================================================
//--------------------------------------------------------------------
CImage*	GrWinGdibToImage(Cls_GrGdib* A_Gdib)
{
	// local -------------------
		CImage*	Tv_Result;
		BOOL8		Tv_IsOk;
		__u32		Tv_ResX;
		__u32		Tv_ResY;
		__u32		Tv_PxBitCnt;
		__u32		Tv_PltCnt;
		__u32		Tv_PltIdx;
		__u32		Tv_ClrSr;
		__u32		Tv_ClrTg;
		__u32		Tv_FccSr;
		__u32		Tv_WkX;
		__u32		Tv_WkY;
		__u8*		Tv_PtrSr;
		__u8*		Tv_PtrTg;
		__u32		Tv_LineSize;
		__u32		Tv_PchSr;
	// code --------------------
		Tv_Result	=	NULL;

		// check exist gdib
		if(NULL != A_Gdib)
		{
			// create image object
			Tv_Result	=	(CImage*) new CImage();
			if(NULL != Tv_Result)
			{
				Tv_IsOk		=	FALSE;
				// get resolution
				Tv_ResX		=	A_Gdib->GetResX();
				Tv_ResY		=	A_Gdib->GetResY();
				Tv_PxBitCnt	=	A_Gdib->GetPixelBit();
				Tv_FccSr	=	A_Gdib->GetFcc();

				if(A_Gdib->IsPalleteUse())
				{
					// create image
					if((1 == Tv_PxBitCnt) || (4 == Tv_PxBitCnt) ||(8 == Tv_PxBitCnt))
					{
						Tv_Result->Create((int)Tv_ResX, (int)Tv_ResY, (int)Tv_PxBitCnt);

						// convert pallete
						Tv_PltCnt		=	1 << Tv_PxBitCnt;
						for(Tv_PltIdx = 0; Tv_PltIdx < Tv_PltCnt; Tv_PltIdx++)
						{
							Tv_ClrSr	=	A_Gdib->PalleteGet(Tv_PltIdx);
							Tv_ClrTg	=	GrDrawCvtColorByFcc(Tv_FccSr, Tv_ClrSr, E_GrFccARGB8888);
							Tv_Result->SetColorTable((UINT)Tv_PltIdx, (UINT)1, (RGBQUAD*)(&Tv_ClrTg));
						}

						// copy data
						if(4 == Tv_PxBitCnt)
						{
							Tv_PchSr		=	A_Gdib->GetDibPitch();
							Tv_PtrSr		=	(__u8*)A_Gdib->GetDibPtr();
							Tv_LineSize	=	(Tv_ResX >> 1) + (Tv_ResX & 1);
							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrTg	=	(__u8*)Tv_Result->GetPixelAddress(0, (int)Tv_WkY);
								for(Tv_WkX = 0; Tv_WkX < Tv_LineSize; Tv_WkX++)
								{
									Tv_PtrTg[Tv_WkX]	=	((Tv_PtrSr[Tv_WkX] & 0x0F) << 4) | ((Tv_PtrSr[Tv_WkX] & 0xF0) >> 4);
								}
								// next
								Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + (Def_GrCalPtr)Tv_PchSr);
							}
						}
						else if(8 == Tv_PxBitCnt)
						{
							Tv_PchSr		=	A_Gdib->GetDibPitch();
							Tv_PtrSr		=	(__u8*)A_Gdib->GetDibPtr();
							Tv_LineSize	=	Tv_ResX;
							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrTg	=	(__u8*)Tv_Result->GetPixelAddress(0, (int)Tv_WkY);
								for(Tv_WkX = 0; Tv_WkX < Tv_LineSize; Tv_WkX++)
								{
									Tv_PtrTg[Tv_WkX]	=	Tv_PtrSr[Tv_WkX];
								}
								// next
								Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + (Def_GrCalPtr)Tv_PchSr);
							}
						}
						else
						{
							Tv_PchSr		=	A_Gdib->GetDibPitch();
							Tv_PtrSr		=	(__u8*)A_Gdib->GetDibPtr();
							Tv_LineSize	=	(Tv_ResX * Tv_PxBitCnt) >> 3;
							if(0 != (0x07 & (Tv_ResX * Tv_PxBitCnt)))
							{
								Tv_LineSize	++;
							}
							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrTg	=	(__u8*)Tv_Result->GetPixelAddress(0, (int)Tv_WkY);
								GrDumyCopyMem(Tv_PtrTg, Tv_PtrSr, Tv_LineSize);
								// next
								Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + (Def_GrCalPtr)Tv_PchSr);
							}
						}

						// success
						Tv_IsOk		=	TRUE;
					}
				}
				else
				{
					Tv_Result->Create((int)Tv_ResX, (int)Tv_ResY, 32);
					// convert pixel
					for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
					{
						Tv_PtrTg	=	(__u8*)Tv_Result->GetPixelAddress(0, (int)Tv_WkY);
						for( Tv_WkX = 0; Tv_WkX < Tv_ResX; Tv_WkX++)
						{
							A_Gdib->GetPixelVal((__s32)Tv_WkX, (__s32)Tv_WkY, &Tv_ClrSr);
							Tv_ClrTg	=	GrDrawCvtColorByFcc(Tv_FccSr, Tv_ClrSr, E_GrFccARGB8888);

							*((__u32*)((Def_GrCalPtr)Tv_PtrTg))	=	*((__u32*)((Def_GrCalPtr)&Tv_ClrTg));
							// next
							Tv_PtrTg	=	(__u8*)((Def_GrCalPtr)Tv_PtrTg + 4);

#if 0
							*((__u16*)((Def_GrCalPtr)Tv_PtrTg))	=	*((__u16*)((Def_GrCalPtr)&Tv_ClrTg));
							*((__u8*)((Def_GrCalPtr)Tv_PtrTg + 2))	=	*((__u8*)((Def_GrCalPtr)&Tv_ClrTg + 2));
							// next
							Tv_PtrTg	=	(__u8*)((Def_GrCalPtr)Tv_PtrTg + 3);
#endif
						}
					}

					Tv_IsOk		=	TRUE;
				}
				// check success
				if(!Tv_IsOk)
				{
					delete	Tv_Result;
					Tv_Result	=	NULL;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrGdib*	GrWinImageToGdib(CImage* A_Image, __u32 A_ColorFcc)
{
	// local -------------------
		Cls_GrGdib*	Tv_Result;
		__u32	Tv_ResX;
		__u32	Tv_ResY;
		__u32		Tv_PxBitCnt;
		__u32		Tv_PltCnt;
		__u32		Tv_PltIdx;
		__u32		Tv_ClrSr;
		__u32		Tv_ClrTg;
		__u32		Tv_PchTg;
		__u8*		Tv_PtrSr;
		__u8*		Tv_PtrTg;
		__u32		Tv_LineSize;
		__u32	Tv_WkX;
		__u32	Tv_WkY;
		__u32	Tv_FccSr;
	// code --------------------
		Tv_Result	=	NULL;

		// check exist image
		if(NULL != A_Image)
		{
			// get resolution
			Tv_ResX	=	(__u32)A_Image->GetWidth();
			Tv_ResY	=	(__u32)A_Image->GetHeight();

			Tv_PxBitCnt	=	(__u32)A_Image->GetBPP();
			// check pallete mode
			if(8 >= Tv_PxBitCnt)
			{
				Tv_Result	=	GrGdibCreate(A_ColorFcc, Tv_ResX, Tv_ResY, 0, NULL, Tv_PxBitCnt);
				if(NULL != Tv_Result)
				{
					// convert pallete
					Tv_PltCnt		=	1 << Tv_PxBitCnt;
					for(Tv_PltIdx = 0; Tv_PltIdx < Tv_PltCnt; Tv_PltIdx++)
					{
						Tv_ClrSr	=	0;
						A_Image->GetColorTable((UINT)Tv_PltIdx, 1, (RGBQUAD*)&Tv_ClrSr);
						Tv_ClrTg	=	GrDrawCvtColorByFcc(E_GrFccRGB888, Tv_ClrSr, A_ColorFcc);
						Tv_Result->PalleteSet(Tv_PltIdx, Tv_ClrTg);
					}

					// copy data
					if(4 == Tv_PxBitCnt)
					{
						Tv_PchTg		=	Tv_Result->GetDibPitch();
						Tv_PtrTg		=	(__u8*)Tv_Result->GetDibPtr();
						Tv_LineSize	=	(Tv_ResX >> 1) + (Tv_ResX & 1);
						for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
						{
							Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
							for(Tv_WkX = 0; Tv_WkX < Tv_LineSize; Tv_WkX++)
							{
								Tv_PtrTg[Tv_WkX]	=	((Tv_PtrSr[Tv_WkX] & 0x0F) << 4) | ((Tv_PtrSr[Tv_WkX] & 0xF0) >> 4);
							}
							// next
							Tv_PtrTg	=	(__u8*)((Def_GrCalPtr)Tv_PtrTg + (Def_GrCalPtr)Tv_PchTg);
						}
					}
					else if(8 ==  Tv_PxBitCnt)
					{
						Tv_PchTg		=	Tv_Result->GetDibPitch();
						Tv_PtrTg		=	(__u8*)Tv_Result->GetDibPtr();
						Tv_LineSize	=	Tv_ResX;
						for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
						{
							Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
							for(Tv_WkX = 0; Tv_WkX < Tv_LineSize; Tv_WkX++)
							{
								Tv_PtrTg[Tv_WkX]	=	Tv_PtrSr[Tv_WkX];
							}
							// next
							Tv_PtrTg	=	(__u8*)((Def_GrCalPtr)Tv_PtrTg + (Def_GrCalPtr)Tv_PchTg);
						}
					}
					else
					{
						// other bit
						Tv_PchTg		=	Tv_Result->GetDibPitch();
						Tv_PtrTg		=	(__u8*)Tv_Result->GetDibPtr();
						Tv_LineSize	=	(Tv_ResX * Tv_PxBitCnt) >> 3;
						if(0 != (0x07 & (Tv_ResX * Tv_PxBitCnt)))
						{
							Tv_LineSize	++;
						}
						for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
						{
							Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
							GrDumyCopyMem(Tv_PtrTg, Tv_PtrSr, Tv_LineSize);
							// next
							Tv_PtrTg	=	(__u8*)((Def_GrCalPtr)Tv_PtrTg + (Def_GrCalPtr)Tv_PchTg);
						}
					}

				}
			}
			else
			{
				Tv_Result	=	GrGdibCreate(A_ColorFcc, Tv_ResX, Tv_ResY);
				if(NULL != Tv_Result)
				{
					switch(Tv_PxBitCnt)
					{
						case 16:
							Tv_FccSr	=	E_GrFccRGB565;

							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
								for(Tv_WkX = 0; Tv_WkX < Tv_ResX; Tv_WkX++)
								{
									Tv_ClrSr	=	0;
									*((__u16*)((Def_GrCalPtr)&Tv_ClrSr))	=	*((__u16*)((Def_GrCalPtr)Tv_PtrSr));
									Tv_ClrTg	=	GrDrawCvtColorByFcc(Tv_FccSr, Tv_ClrSr, A_ColorFcc);
									Tv_Result->SetPixelVal(Tv_WkX, Tv_WkY, Tv_ClrTg);
									// next
									Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + 2);
								}
							}
							break;
						case 24:
							Tv_FccSr	=	E_GrFccRGB888;

							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
								for(Tv_WkX = 0; Tv_WkX < Tv_ResX; Tv_WkX++)
								{
									Tv_ClrSr	=	0;
									*((__u16*)((Def_GrCalPtr)&Tv_ClrSr))	=	*((__u16*)((Def_GrCalPtr)Tv_PtrSr));
									*((__u8*)((Def_GrCalPtr)&Tv_ClrSr + 2))	=	*((__u8*)((Def_GrCalPtr)Tv_PtrSr + 2));
									Tv_ClrTg	=	GrDrawCvtColorByFcc(Tv_FccSr, Tv_ClrSr, A_ColorFcc);
									Tv_Result->SetPixelVal(Tv_WkX, Tv_WkY, Tv_ClrTg);
									// next
									Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + 3);
								}
							}
							break;
						default:
							Tv_FccSr	=	E_GrFccARGB8888;
							//Tv_FccSr	=	E_GrFccABGR8888;

							for(Tv_WkY = 0; Tv_WkY < Tv_ResY; Tv_WkY ++)
							{
								Tv_PtrSr	=	(__u8*)A_Image->GetPixelAddress(0, (int)Tv_WkY);
								for(Tv_WkX = 0; Tv_WkX < Tv_ResX; Tv_WkX++)
								{
									*((__u32*)((Def_GrCalPtr)&Tv_ClrSr))	=	*((__u32*)((Def_GrCalPtr)Tv_PtrSr));
									if(0 == (0xFF000000 &Tv_ClrSr))
									{
										Tv_ClrSr	=	0;
									}
									Tv_ClrTg	=	GrDrawCvtColorByFcc(Tv_FccSr, Tv_ClrSr, A_ColorFcc);
									Tv_Result->SetPixelVal(Tv_WkX, Tv_WkY, Tv_ClrTg);
									// next
									Tv_PtrSr	=	(__u8*)((Def_GrCalPtr)Tv_PtrSr + 4);
								}
							}
							break;
					}

				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------

