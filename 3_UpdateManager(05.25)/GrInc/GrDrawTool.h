/*
	image base
	uese
		GrTypeBase.h
		GrImgBase.h
		GrDrawTool.h

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDrawTool
#define	_EPC_GrDrawTool

//====================================================================
// uses
#include	<GrTypeBase.h>
	
//====================================================================
//constance

//check cover or overlapped
#define		E_GrDrawChkOvlpNotCover				0
#define		E_GrDrawChkOvlpNotOvlp				0
#define		E_GrDrawChkOvlpCoverPara1			1
#define		E_GrDrawChkOvlpCoverPara2			2
#define		E_GrDrawChkOvlpMatchBoth			3
#define		E_GrDrawChkOvlpOvlpPart				4

//line angle
#define		E_GrDrawLineAngleNormal				0
#define		E_GrDrawLineAngleHorizontal		1
#define		E_GrDrawLineAngleVertical			2

//blt atb
#define		E_GrDrawDibOp1UseKeySrc				0x00000001
#define		E_GrDrawDibOp1UseKeyDst				0x00000002
#define		E_GrDrawDibOp1UseAlpha				0x00000004
#define		E_GrDrawDibOp1UseSmooth				0x00000008

#define		E_GrDrawDibOp1MaskBlt					( E_GrDrawDibOp1UseKeySrc | E_GrDrawDibOp1UseKeyDst | E_GrDrawDibOp1UseAlpha )

//yuv zero replacement 
#define		E_GrDrawYuvZeroRplY						0x54
#define		E_GrDrawYuvZeroRplU						0x59
#define		E_GrDrawYuvZeroRplV						0x79

#define		E_GrDrawYuvMaxY								239
#define		E_GrDrawYuvMinY								16
	
#define		E_GrDrawClrElemCnt						3		/* color element count */

//direction
#define		E_GrDrawDirLeft								0
#define		E_GrDrawDirUp									1
#define		E_GrDrawDirRight							2
#define		E_GrDrawDirDown								3

//line relation type
#define		E_GrDrawLineNotAble						0
#define		E_GrDrawLineVertical					1
#define		E_GrDrawLineHorizontal				2
#define		E_GrDrawLineDiagonal					3

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct	St_GrDrawLineD2
{
	__u8	LineType;		//line relation type
	__s32		StX;
	__s32		StY;
	__s32		EdX;
	__s32		EdY;

	St_GrRect		RectDrawAble;

	BOOL8	IsInited;
	BOOL8	IsDrawing;

	__s32		Dx;
	__s32		Dy;
	__s32		DirX;					//directional adder
	__s32		DirY;					//directional adder

	__s32		Etm;					//error term
	__s32		NowX;					//now position x
	__s32		NowY;					//now position y
	BOOL8	IsPxShow;			//visiable pixel

}	*Ptr_GrDrawLineD2;

typedef	struct St_GrDrawPxStItm		//pixel struct item
{
	__u8		Pos;			//bit position
	__u8		Size;			//bit size
}	*Ptr_GrDrawPxStItm;

typedef	struct St_GrDrawPxStInfo	//pixel structer info
{
	__u32		Fcc;			//fcc code
	BOOL8		IsYuv;
	BOOL8		IsPacked;
	St_GrDrawPxStItm	PxSt[2][E_GrDrawClrElemCnt];
}	*Ptr_GrDrawPxStInfo;

typedef	struct St_GrDrawClrCvtPrAm
{
	__s32		Add;
	__s32		Mul;
}	*Ptr_GrDrawClrCvtPrAm;

typedef	struct St_GrDrawClrCvtPrEl
{
	St_GrDrawClrCvtPrAm	ElSr[E_GrDrawClrElemCnt];	//element adder
	__s32		TtDiv;		//total dividor
	__s32		TtAdd;		//total adder
} *Ptr_GrDrawClrCvtPrEl; 

typedef	struct St_GrDrawClrCvtPara 
{
	St_GrDrawClrCvtPrEl	El[E_GrDrawClrElemCnt];
}	*Ptr_GrDrawClrCvtPara;

typedef	struct St_GrDrawFindDirDist
{
	__s32		Dist;
	__s32		Ref;
}	*Ptr_GrDrawFindDirDist;


//====================================================================
//class

//====================================================================
//global function
	
	//--------------------------------------------------------------------
	//coordinate
	//--------------------------------------------------------------------

	/*	get position by relate rect
			input - 
				A_PtrRelateRect	:	relate rect
				A_X			:	input x positino
				A_PtrRtPos	:	pointer of reciving position
			output - 
				*A_PtrRtPos	: reciving position
				Result	:	position is valid
	*/
	BOOL8		GrDrawCnGetRelPosX( Ptr_GrRect A_PtrRelateRect, __s32 A_X, __s32* A_PtrRtPos );

	/*		get position by relate rect
			input - 
				A_PtrRelateRect	:	relate rect
				A_Y			:	input y position
				A_PtrRtPos	:	pointer of reciving position
			output - 
				*A_PtrRtPos	: reciving position
				Result	:	position is valid
	*/
	BOOL8		GrDrawCnGetRelPosY( Ptr_GrRect A_PtrRelateRect, __s32 A_Y, __s32* A_PtrRtPos );

	/*	get rect by relate rect
			input - 
				A_PtrRelateRect	:	relate rect
				A_PtrRect		:	input rect
				A_PtrRtRect	:	pointer of reciving rect
			output - 
				*A_PtrRtRect	: reciving rect
				Result	:	rect is valid
	*/
	BOOL8		GrDrawCnGetRelRect( Ptr_GrRect A_PtrRelateRect, Ptr_GrRect A_PtrRect, Ptr_GrRect A_PtrRtRect );

	BOOL8		GrDrawCnGetComRect( Ptr_GrRect A_PtrRtRect, Ptr_GrRect A_PtrRect1, 
															Ptr_GrRect A_PtrRect2 );		

	/*	compare two rect
			input - 
				A_PtrRect1	:	pointer of source rect 1
				A_PtrRect2	:	pointer of source rect 2
			output - 
				Result			:	0 = not overlapped, 1 = cover rect1 > rect2 , 2 = cover rect1 < rect2,
											3 = match all , 4 = overlapped part
	*/
	__u32		GrDrawCnChkOvlpRect( Ptr_GrRect A_PtrRect1, Ptr_GrRect A_PtrRect2 );

	/*	check two rect
			input - 
				A_PtrRect1	:	pointer of source rect 1
				A_PtrRect2	:	pointer of source rect 2
			output - 
				Result			:	0 = not cover , 1 = cover rect1 > rect2 , 2 = cover rect1 < rect2,
											3 = match all 
	*/
	__u32		GrDrawCnChkCoverRect( Ptr_GrRect A_PtrRect1, Ptr_GrRect A_PtrRect2 );

	/*	patch rect by size
		input
			A_SizeX , A_SizeY	:	area size x, y
			A_PtrSrRect		:	soruce rect
		output
			A_PtrRtRect		:	patched rect - can equal A_PtrSrRect
			Result				:	avilable - TRUE/ FALSE
	*/
	BOOL8		GrDrawCnPatRectBySize( __s32 A_SizeX, __s32 A_SizeY, Ptr_GrRect A_PtrSrRect, Ptr_GrRect A_PtrRtRect );

	BOOL8		GrDrawPatchPaintRect( __s32 A_ScrSizeX, __s32 A_ScrSizeY, Ptr_GrRect A_PtrWinRect, 
																Ptr_GrRect A_PtrReqRect, Ptr_GrRect A_PtrRtRect );
	/* patch blt parametter on paint mode
		input
			A_PtrRcUpdt	:	update able object rect
			A_PtrX , A_PtrY , A_PtrRcSrc : blt source position
		output
			A_PtrX , A_PtrY , A_PtrRcSrc : patched blt position
	*/
	BOOL8	GrDrawPatchPaintBlt(	Ptr_GrRect A_PtrRcUpdt, 
		__s32* A_PtrX, __s32* A_PtrY, Ptr_GrRect A_PtrRcSrc);

	BOOL8		GrDrawPatchRelatePosWithRect( Ptr_GrRect A_PtrRectBasis, Ptr_GrRect A_PtrRectSrc, 
																				__s32* A_PtrPosX, __s32* A_PtrPosY );

	/*
	BOOL8		GrDrawPatchBltPara( Ptr_GrRect A_PtrRectDstBase, __s32* A_PtrPosX, __s32* A_PtrPosY,
															Ptr_GrRect A_PtrRectSrcBase, Ptr_GrRect A_PtrRectSrcCont );
															*/
	                                  
	BOOL8		GrDrawPatchStretchPara(	Ptr_GrRect A_PtrRectDstMask, Ptr_GrRect A_PtrRectDstOrg, 
																	Ptr_GrRect A_PtrRectSrcMask, Ptr_GrRect A_PtrRectSrcOrg,
																	Ptr_GrRect A_PtrRtRectDst );

	BOOL8		GrDrawIsSimilarPosHz( Ptr_GrRect A_PtrRc1, Ptr_GrRect A_PtrRc2 );
	BOOL8		GrDrawIsSimilarPosVt( Ptr_GrRect A_PtrRc1, Ptr_GrRect A_PtrRc2 );
	void		GrDrawCalcRtoSize( __u32 A_SizeX, __u32 A_SizeY, __u32 A_RtoX, __u32 A_RtoY, 
					__u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY );

	__s32			GrDrawDistanceRectByDir( Ptr_GrRect A_PtrRcBase, Ptr_GrRect A_PtrRcRef, __u32 A_Dir );
	void		GrDrawDistSimRectInit( Ptr_GrDrawFindDirDist A_PtrDist );
	BOOL8		GrDrawDistSimRectByDir( Ptr_GrRect A_PtrRcBase, Ptr_GrRect A_PtrRcRef, __u8 A_Dir, Ptr_GrDrawFindDirDist A_PtrDist, BOOL8 A_IsWide );

	//--------------------------------------------------------------------
	//color function
	//--------------------------------------------------------------------
	__s32			GrDrawGetPixelByteByFcc( __u32 A_Fcc );
	__s32			GrDrawGetPixelBitByFcc( __u32 A_Fcc );
	__s32			GrDrawCalcDefaultPitch( __s32 A_PixelByte, __s32 A_SizeX, __s32 A_PitchUnit = 4 );
	__u32		GrDrawCalcPitchByFcc( __u32 A_Fcc, __u32 A_SizeX, __u32 A_PitchUnit = 4 );
	__u32		GrDrawCalcPitchByBitCnt( __u32 A_BitCnt, __u32 A_SizeX, __u32 A_PitchUnit = 4 );
	BOOL8		GrDrawIsCoupleFormat( __u32 A_Fcc );
	BOOL8		GrDrawIsFccTypeYUV( __u32 A_Fcc );
	__u32		GrDrawGetRgbFcc( __u32 A_PixelBit, BOOL8 A_IsReverse );
	BOOL8		GrDrawIsAlphaFormat( __u32 A_Fcc );
	BOOL8		GrDrawIsAlphaColor( __u32 A_Fcc, __u32 A_Color );
	void		GrDrawColorAlpahSet( __u32 A_Fcc, void* A_PtrColor, BOOL8 A_IsSet );

	BOOL8		GrDrawGetColorElements( __u32 A_Fcc, __u32 A_Color, __s32* A_PtrR,__s32* A_PtrG,__s32* A_PtrB, __s32* A_PtrA,
																	__s32* A_PtrY, __s32* A_PtrU, __s32* A_PtrV );
	__u32		GrDrawMakeColorByElements( __u32 A_Fcc, __s32 A_R_Y, __s32 A_G_U, __s32 A_B_V, __s32 A_A );

	__u32		GrDrawCvtColorByFcc( __u32 A_FccSrc, __u32 A_ColorSrc, __u32 A_FccDst );
	__u32		GrDrawSetColorLight( __u32 A_Fcc, __u32 A_ColorSrc, __u32 A_Ratio256base );
	__u32		GrDrawAddColorLight( __u32 A_Fcc, __u32 A_ColorSrc, __s32 A_Add );
	
	__u32		GrDrawColorMix( __u32 A_Fcc, __u32 A_ColorSrc1, __u32 A_ColorSrc2 );
	__u32		GrDrawGetPxMask( Ptr_GrDrawPxStInfo A_PtrPxSt, BOOL8 A_IsEven = FALSE );

	Ptr_GrDrawPxStInfo	GrDrawFindPxStByFcc( __u32 A_Fcc );
	void		GrDrawGetClrElemByPxst( __u32 A_PxVal, Ptr_GrDrawPxStInfo A_PtrPxSt, __u8* A_PtrRtEle );
	__u32		GrDrawMakeClrValByByPxst( Ptr_GrDrawPxStInfo A_PtrPxSt, __u8* A_PtrEle );
	Ptr_GrDrawClrCvtPara	GrDrawFindCvtParaByPxSt( Ptr_GrDrawPxStInfo A_PtrPxStSr, Ptr_GrDrawPxStInfo A_PtrPxStTg );
	void		GrDrawCvtEleByCvtPara( Ptr_GrDrawClrCvtPara A_PtrCvtPara, __u8* A_PtrEleSr, __u8* A_PtrRtEle );

	__u32		GrDrawGetColorBlack( __u32 A_Fcc );
	__u32		GrDrawGetColorWhite( __u32 A_Fcc );

	//--------------------------------------------------------------------
	//pixel functions
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	//line functions
	//--------------------------------------------------------------------
	BOOL8		GrDrawGetLineIntersection(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																			__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY,
																			__s32* A_PtrRtItsX, __s32* A_PtrRtItsY );
	__u32		GrDrawGetLineType( __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY );
	BOOL8		GrDrawGetLineItscWithNoHV(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																			__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY,
																			__s32* A_PtrRtItsX, __s32* A_PtrRtItsY );
	BOOL8		GrDrawGetLineItscWithHzLine(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																				__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, 
																				__s32* A_PtrRtItsX, __s32* A_PtrRtItsY );
	BOOL8		GrDrawGetLineItscWithVtLine(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																				__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdY, 
																				__s32* A_PtrRtItsX, __s32* A_PtrRtItsY );
	BOOL8		GrDrawGetLineItscHwithV(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX,
																		__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdY, 
																		__s32* A_PtrRtItsX, __s32* A_PtrRtItsY );
	BOOL8		GrDrawGetLineItscChkOnLine(	__s32 A_ItsX,		__s32	A_ItsY,
																			__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																			__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY	);
	/*
		GrDrawIsPointInRect
				check point in rect
				input - 
					A_X					:	point x
					A_Y					:	point y
					A_PtrRect		:	basis rect
				output
					result			:	TRUE / FALSE
	*/
	BOOL8		GrDrawIsPointInRect( __s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRect );

	/*
		GrDrawGetLineTypeOnRect
				get line relaton type on rect
				input - 
					A_PtrRect		:	basis rect
					A_StX				:	line start pointer position x
					A_StY				:	line start pointer position y
					A_EdX				:	line end pointer position x
					A_EdY				:	line end pointer position y
					A_PtrRtStX	:	receive current start position x
					A_PtrRtStY	:	receive current start position y
					A_PtrRtEdX	:	receive current end position x
					A_PtrRtEdY	:	receive current end position y
				outpu - 0 == not able
								1 == able
								2 == diagonal line
				  
	*/
	__u8		GrDrawGetLineTypeOnRect( Ptr_GrRect A_PtrRect, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY,
																	__s32* A_PtrRtStX, __s32* A_PtrRtStY, __s32* A_PtrRtEdX, __s32* A_PtrRtEdY );

	/*
		GrDrawLineD2Init	
				initialize line drawing calculator - must drawing pixel after initialized
				input	-
					A_PtrLineStat	:	pointer line status structor
					A_StX		:	draw start position x
					A_StY		:	draw start position y
					A_EdX		:	draw end position x
					A_EdY		:	draw end position y
					A_PtrDrawAbleRect	:	drawing limite rect - NULL = Not limite ( excption : under zero )
				output - 
					Result	:	initialize success	-	TRUE / FALSE
					A_PtrLineStat->NowX = now draw position x
					A_PtrLineStat->NowY = now draw position y
					A_PtrLineStat->IsPxShow = pixel draw able

	*/
	BOOL8		GrDrawLineD2Init( Ptr_GrDrawLineD2 A_PtrLineStat, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY,
														Ptr_GrRect A_PtrDrawAbleRect );
	/*
		GrDrawLineD2CalNext
				calculate next draw position
				input - 
					A_PtrLineStat	:	pointer line status structor
				output -
					Result		:	drawable - TRUE / FALSE
					A_PtrLineStat->NowX = now draw position x
					A_PtrLineStat->NowY = now draw position y
					A_PtrLineStat->EtmX = strength of pixel x direction
					A_PtrLineStat->EtmY = strength of pixel y direction
					A_PtrLineStat->IsPxShow = pixel draw able

	*/
	BOOL8		GrDrawLineD2CalNext(	Ptr_GrDrawLineD2 A_PtrLineStat );


	//--------------------------------------------------------------------
	// dib function
	//--------------------------------------------------------------------
	void	GrDrawDibCpyNrm( void* A_PtrDibSr, __u32 A_PitchSr, void* A_PtrDibTg, __u32 A_PitchTg, __u32 A_ResY );
	void	GrDrawDibCpyRsvVt( void* A_PtrDibSr, __u32 A_PitchSr, void* A_PtrDibTg, __u32 A_PitchTg, __u32 A_ResY );

	//--------------------------------------------------------------------
	// make bmp file function
	//--------------------------------------------------------------------

//====================================================================
//out Enviroment conditional
#endif
