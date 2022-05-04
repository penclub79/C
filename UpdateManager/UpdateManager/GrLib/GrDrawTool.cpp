/*
 draw tool

	check your cpu type , cpu type defined by GrCPUTYPE
		not defined or 0 = pentium 4
		1 = pentium 3
		2 = arm
		0xFFFFFFFF = unknown

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<stdlib.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>
#include	<GrDumyTool.h>

//====================================================================
//local const

#define E_GrDrawPxStTblCnt			13

#define E_GrDrawCvtParaEqual		0
#define E_GrDrawCvtParaRgb2Yuv	1
#define E_GrDrawCvtParaYuv2Rgb	2

//====================================================================
//local type

//====================================================================
//local function 

//====================================================================
//var
St_GrDrawPxStInfo	V_GrDrawPxStruct[E_GrDrawPxStTblCnt]	=	
{
	{ E_GrFccRGB,				FALSE, FALSE,	{ { {16,8},{8,8},{0,8} } , { {0,0},{0,0},{0,0} } } },		/* 0 */
	{ E_GrFccYUY2,			TRUE,		TRUE,	{ { {0,8},{8,8},{24,8} } , { {16,8},{8,8},{24,8} } } }, /* 1 */
	{ E_GrFccUYVY,			TRUE,		TRUE,	{ { {8,8},{0,8},{16,8} } , { {24,8},{0,8},{16,8} } } },	/* 2 */
	{ E_GrFccUYVYF,			TRUE,		TRUE,	{ { {8,8},{0,8},{16,8} } , { {24,8},{0,8},{16,8} } } },	/* 3 */
	{ E_GrFccRGB555,			FALSE, FALSE,	{ { {10,5},{5,5},{0,5} } , { {0,0},{0,0},{0,0} } } },		/* 4 */
	{ E_GrFccRGB565,			FALSE, FALSE,	{ { {11,5},{5,6},{0,5} } , { {0,0},{0,0},{0,0} } } },		/* 5 */
	{ E_GrFccRGB888,			FALSE, FALSE,	{ { {16,8},{8,8},{0,8} } , { {0,0},{0,0},{0,0} } } },		/* 6 */
	{ E_GrFccBGR888,		FALSE, FALSE,	{ { {0,8},{8,8},{16,8} } , { {0,0},{0,0},{0,0} } } },		/* 7 */
	{ E_GrFccARGB8888,			FALSE, FALSE,	{ { {16,8},{8,8},{0,8} } , { {0,0},{0,0},{0,0} } } },		/* 8 */
	{ E_GrFccABGR8888,		FALSE, FALSE,	{ { {0,8},{8,8},{16,8} } , { {0,0},{0,0},{0,0} } } },		/* 9 */
	{ E_GrFccUVYA,			TRUE,  FALSE,	{ { {0,0},{0,0},{0,0} }  , { {16,8},{0,8},{8,8} } } },	/* 10 */
	{ E_GrFccARGB1555,		FALSE, FALSE,	{ { {10,5},{5,5},{0,5} } , { {0,0},{0,0},{0,0} } } },		/* 11 */
	{ E_GrFccARGB4444,	FALSE, FALSE,	{ { {8,4},{4,4},{0,4} } , { {0,0},{0,0},{0,0} } } },		/* 12 */
};

St_GrDrawClrCvtPara	V_GrDrawCvtPara[]	=	
{
	{ { {{{0,1},{0,0},{0,0}},1,0}, {{{0,0},{0,1},{0,0}},1,0}, {{{0,0},{0,0},{0,1}},1,0} } },		/* equal type */
	{ { {{{0,66},{0,129},{0,25}},256,16}, {{{0,-38},{0,-74},{0,112}},256,128}, {{{0,112},{0,-94},{0,-18}},256,128} } },		/* rgb 2 yuv */
	{ { {{{-16,1192},{0,0},{-128,1634}},1024,0}, {{{-16,1192},{-128,-401},{-128,-833}},1024,0}, {{{-16,1192},{-128,2065},{0,0}},1024,0} } }			/* yuv 2 rgb */
};

//====================================================================
/*!
 * \brief
 * get line type
 * 
 * \param A_StX
 * line start positon x
 * 
 * \param A_StY
 * line start positon y
 * 
 * \param A_EdX
 * line end positon x
 * 
 * \param A_EdY
 * line end positon y
 * 
 * \returns
 * line type, E_GrDrawLineAngleNormal , E_GrDrawLineAngleVertical , E_GrDrawLineAngleHorizontal
 * 
 */
__u32		GrDrawGetLineType( __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY )
{
	// local -------------------
		__u32		Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	E_GrDrawLineAngleNormal;

		//check x size
		if			( A_StX == A_EdX )
		{
			Tv_Result		=	E_GrDrawLineAngleVertical;
		}
		else if	( A_StY == A_EdY )
		{
			Tv_Result		=	E_GrDrawLineAngleHorizontal;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
/*!
 * \brief
 * find intersection of two line. check angle only
 * 
 * \param A_Ln1StX
 * start x of line 1
 * 
 * \param A_Ln1StY
 * start y of line 1
 * 
 * \param A_Ln1EdX
 * end x of line 1
 * 
 * \param A_Ln1EdY
 * end y of line 1
 * 
 * \param A_Ln2StX
 * start x of line 2
 * 
 * \param A_Ln2StY
 * start y of line 2
 * 
 * \param A_Ln2EdX
 * end x of line 2
 * 
 * \param A_Ln2EdY
 * end y of line 2
 * 
 * \param A_PtrRtItsX
 * recive intersection coodinate x 
 * 
 * \param A_PtrRtItsY
 * recive intersection coodinate y
 * 
 * \returns
 * if cross both line, return TRUE, other FALSE
 * 
 */
BOOL8		GrDrawGetLineItscWithNoHV(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																		__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY,
																		__s32* A_PtrRtItsX, __s32* A_PtrRtItsY )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_L1Dx;
		__s32			Tv_L1Dy;
		double	Tv_L1Delta;
		double	Tv_L1Angle;
		__s32			Tv_L2Dx;
		__s32			Tv_L2Dy;
		double	Tv_L2Delta;
		double	Tv_L2Angle;
		double	Tv_ItsX;
		double	Tv_ItsY;

  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//calculate line 1
		Tv_L1Dx			=	A_Ln1EdX - A_Ln1StX;
		Tv_L1Dy			=	A_Ln1EdY - A_Ln1StY;
		Tv_L1Angle	=	(double)Tv_L1Dy / (double)Tv_L1Dx;
		Tv_L1Delta	=	(double)A_Ln1StY - ( Tv_L1Angle * (double)A_Ln1StX );

		//calculate line 2
		Tv_L2Dx			=	A_Ln2EdX - A_Ln2StX;
		Tv_L2Dy			=	A_Ln2EdY - A_Ln2StY;
		Tv_L2Angle	=	(double)Tv_L2Dy / (double)Tv_L2Dx;
		Tv_L2Delta	=	(double)A_Ln2StY - ( Tv_L2Angle * (double)A_Ln2StX );

		//check parallel
		if ( Tv_L1Angle != Tv_L2Angle )
		{
			//calculate interpointer
			Tv_ItsX		=	(Tv_L1Delta - Tv_L2Delta)	/	(Tv_L2Angle  - Tv_L1Angle);
			Tv_ItsY		=	Tv_L1Angle * Tv_ItsX + Tv_L1Delta; 
			//update result
			*A_PtrRtItsX	=	(__s32)Tv_ItsX;
			*A_PtrRtItsY	=	(__s32)Tv_ItsY;
			//success
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetLineItscWithHzLine(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																			__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, 
																			__s32* A_PtrRtItsX, __s32* A_PtrRtItsY )
{
	// local -------------------
		__s32			Tv_L1Dx;
		__s32			Tv_L1Dy;
		double	Tv_L1Delta;
		double	Tv_L1Angle;
		double	Tv_ItsX;
  // code --------------------
		//calculate line 1
		Tv_L1Dx			=	A_Ln1EdX - A_Ln1StX;
		Tv_L1Dy			=	A_Ln1EdY - A_Ln1StY;
		Tv_L1Angle	=	(double)Tv_L1Dy / (double)Tv_L1Dx;
		Tv_L1Delta	=	(double)A_Ln1StY - ( Tv_L1Angle * (double)A_Ln1StX );

		//calculate interpointer
		Tv_ItsX		=	((double)A_Ln2StY - Tv_L1Delta )	/	Tv_L1Angle;
		//update result
		*A_PtrRtItsX	=	(__s32)Tv_ItsX;
		*A_PtrRtItsY	=	A_Ln2StY;

		return		TRUE;
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetLineItscWithVtLine(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																			__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdY, 
																			__s32* A_PtrRtItsX, __s32* A_PtrRtItsY )
{
	// local -------------------
		__s32			Tv_L1Dx;
		__s32			Tv_L1Dy;
		double	Tv_L1Delta;
		double	Tv_L1Angle;
		double	Tv_ItsY;
  // code --------------------

		//calculate line 1
		Tv_L1Dx			=	A_Ln1EdX - A_Ln1StX;
		Tv_L1Dy			=	A_Ln1EdY - A_Ln1StY;
		Tv_L1Angle	=	(double)Tv_L1Dy / (double)Tv_L1Dx;
		Tv_L1Delta	=	(double)A_Ln1StY - ( Tv_L1Angle * (double)A_Ln1StX );

		//calculate interpointer
		Tv_ItsY		=	Tv_L1Angle * (double)A_Ln2StX + Tv_L1Delta;

		//update result
		*A_PtrRtItsX	=	A_Ln2StX;
		*A_PtrRtItsY	=	(__s32)Tv_ItsY;

		return		TRUE;	
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetLineItscHwithV(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX,
																	__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdY, 
																	__s32* A_PtrRtItsX, __s32* A_PtrRtItsY )
{
	// local -------------------

  // code --------------------
		//set result
		*A_PtrRtItsX		=	A_Ln2StX;
		*A_PtrRtItsY		=	A_Ln1StY;
		
		return		TRUE;
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetLineItscChkOnLine(	__s32 A_ItsX,		__s32	A_ItsY,
																		__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																		__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY	)
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_Ln1StX;
		__s32			Tv_Ln1StY;
		__s32			Tv_Ln1EdX;
		__s32			Tv_Ln1EdY;
		__s32			Tv_Ln2StX;
		__s32			Tv_Ln2StY;
		__s32			Tv_Ln2EdX;
		__s32			Tv_Ln2EdY;
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//sort line position for check range
		if ( A_Ln1StX <= A_Ln1EdX )
		{
			Tv_Ln1StX			=	A_Ln1StX;
			Tv_Ln1EdX			=	A_Ln1EdX;
		}
		else
		{
			Tv_Ln1StX			=	A_Ln1EdX;
			Tv_Ln1EdX			=	A_Ln1StX;
		}

		if ( A_Ln1StY <= A_Ln1EdY )
		{
			Tv_Ln1StY			=	A_Ln1StY;
			Tv_Ln1EdY			=	A_Ln1EdY;
		}
		else
		{
			Tv_Ln1StY			=	A_Ln1EdY;
			Tv_Ln1EdY			=	A_Ln1StY;
		}

		if ( A_Ln2StX <= A_Ln2EdX )
		{
			Tv_Ln2StX			=	A_Ln2StX;
			Tv_Ln2EdX			=	A_Ln2EdX;
		}
		else
		{
			Tv_Ln2StX			=	A_Ln2EdX;
			Tv_Ln2EdX			=	A_Ln2StX;
		}

		if ( A_Ln2StY <= A_Ln2EdY )
		{
			Tv_Ln2StY			=	A_Ln2StY;
			Tv_Ln2EdY			=	A_Ln2EdY;
		}
		else
		{
			Tv_Ln2StY			=	A_Ln2EdY;
			Tv_Ln2EdY			=	A_Ln2StY;
		}

		//check in line range
		if (	(A_ItsX >= Tv_Ln1StX) && (A_ItsX <= Tv_Ln1EdX) &&
					(A_ItsX >= Tv_Ln2StX) && (A_ItsX <= Tv_Ln2EdX) &&
					(A_ItsY >= Tv_Ln1StY) && (A_ItsY <= Tv_Ln1EdY) &&
					(A_ItsY >= Tv_Ln2StY) && (A_ItsY <= Tv_Ln2EdY)			)
		{
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetLineIntersection(	__s32 A_Ln1StX, __s32 A_Ln1StY, __s32 A_Ln1EdX, __s32 A_Ln1EdY,
																		__s32 A_Ln2StX, __s32 A_Ln2StY, __s32 A_Ln2EdX, __s32 A_Ln2EdY,
																		__s32* A_PtrRtItsX, __s32* A_PtrRtItsY )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Ln1Type;
		__u32		Tv_Ln2Type;
		__s32			Tv_ItsX;
		__s32			Tv_ItsY;

  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//get line type
		Tv_Ln1Type	=	GrDrawGetLineType( A_Ln1StX, A_Ln1StY, A_Ln1EdX, A_Ln1EdY );
		Tv_Ln2Type	=	GrDrawGetLineType( A_Ln2StX, A_Ln2StY, A_Ln2EdX, A_Ln2EdY );

		//analysis running type
		if			(	(E_GrDrawLineAngleNormal == Tv_Ln1Type) && (E_GrDrawLineAngleNormal == Tv_Ln2Type) )
		{
			//noram and normal
			Tv_Result		=	GrDrawGetLineItscWithNoHV(	A_Ln1StX,A_Ln1StY,A_Ln1EdX,A_Ln1EdY,
																								A_Ln2StX,A_Ln2StY,A_Ln2EdX,A_Ln2EdY,
																								&Tv_ItsX, &Tv_ItsY );
		}
		else if	(	(E_GrDrawLineAngleHorizontal ==	Tv_Ln1Type) && (E_GrDrawLineAngleNormal == Tv_Ln2Type)	)
		{
			//normal and horizontal
			Tv_Result		=	GrDrawGetLineItscWithHzLine(	A_Ln1StX,A_Ln1StY,A_Ln1EdX,A_Ln1EdY,
																									A_Ln2StX,A_Ln2StY,A_Ln2EdX,
																									&Tv_ItsX, &Tv_ItsY );
		}
		else if	(	(E_GrDrawLineAngleHorizontal ==	Tv_Ln2Type) && (E_GrDrawLineAngleNormal == Tv_Ln1Type)	)
		{
			//horizontal and normal
			Tv_Result		=	GrDrawGetLineItscWithHzLine(	A_Ln2StX,A_Ln2StY,A_Ln2EdX,A_Ln2EdY,
																									A_Ln1StX,A_Ln1StY,A_Ln1EdX,
																									&Tv_ItsX, &Tv_ItsY );
		}
		else if	(	(E_GrDrawLineAngleVertical ==	Tv_Ln1Type) && (E_GrDrawLineAngleNormal == Tv_Ln2Type)	)
		{
			//normal and vertical
			Tv_Result		=	GrDrawGetLineItscWithVtLine(	A_Ln1StX,A_Ln1StY,A_Ln1EdX,A_Ln1EdY,
																									A_Ln2StX,A_Ln2StY,A_Ln2EdY,
																									&Tv_ItsX, &Tv_ItsY );
		}
		else if (	(E_GrDrawLineAngleVertical ==	Tv_Ln2Type) && (E_GrDrawLineAngleNormal == Tv_Ln1Type)	)
		{
			//vertical and normal
			Tv_Result		=	GrDrawGetLineItscWithVtLine(	A_Ln2StX,A_Ln2StY,A_Ln2EdX,A_Ln2EdY,
																									A_Ln1StX,A_Ln1StY,A_Ln1EdY,
																									&Tv_ItsX, &Tv_ItsY );
		}
		else if	(	(E_GrDrawLineAngleVertical ==	Tv_Ln1Type) && (E_GrDrawLineAngleHorizontal == Tv_Ln2Type)	)
		{
			//vertical and horizontal
			Tv_Result		=	GrDrawGetLineItscHwithV(	A_Ln1StX,A_Ln1StY,A_Ln1EdX,
																							A_Ln2StX,A_Ln2StY,A_Ln2EdY,
																							&Tv_ItsX, &Tv_ItsY );
		}
		else if	(	(E_GrDrawLineAngleVertical ==	Tv_Ln2Type) && (E_GrDrawLineAngleHorizontal == Tv_Ln1Type)	)
		{
			//horizontal and vertical
			Tv_Result		=	GrDrawGetLineItscHwithV(	A_Ln2StX,A_Ln2StY,A_Ln2EdX,
																							A_Ln1StX,A_Ln1StY,A_Ln1EdY,
																							&Tv_ItsX, &Tv_ItsY );
		}

		//other = parallel , so error

		//check intersection pointer in line
		if ( Tv_Result )
		{
			Tv_Result	=	GrDrawGetLineItscChkOnLine( Tv_ItsX , Tv_ItsY,
																							A_Ln1StX,A_Ln1StY,A_Ln1EdX,A_Ln1EdY,
																							A_Ln2StX,A_Ln2StY,A_Ln2EdX,A_Ln2EdY		);
		}
		
		return		Tv_Result;
}

//--------------------------------------------------------------------
/*!
 * \brief
 * check position in rect
 * 
 * \param A_X
 * positino x
 * 
 * \param A_Y
 * position y
 * 
 * \param A_PtrRect
 * check area rect
 * 
 * \returns
 * if position merged in rect then return TRUE.
 * 
 */
BOOL8		GrDrawIsPointInRect( __s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRect )
{
	// local -------------------
		BOOL8		Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	FALSE;
		if	(	(A_X >= A_PtrRect->left) && (A_X < A_PtrRect->right) &&
					(A_Y >= A_PtrRect->top) && (A_Y < A_PtrRect->bottom)		)
		{
			//in rect
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
__u8	GrDrawGetLineTypeOnRect(	Ptr_GrRect A_PtrRect, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY,
																__s32* A_PtrRtStX, __s32* A_PtrRtStY, __s32* A_PtrRtEdX, __s32* A_PtrRtEdY )
{
	// local -------------------
		__u8		Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	E_GrDrawLineNotAble;
		*A_PtrRtStX	=	A_StX;
		*A_PtrRtStY	=	A_StY;
		*A_PtrRtEdX	=	A_EdX;
		*A_PtrRtEdY	=	A_EdY;

		//check horizontal line
		if ( A_StY == A_EdY )
		{
			//in vertical positon
			if ( (A_StY >= A_PtrRect->top) && (A_StY < A_PtrRect->bottom) )
			{
				//in horizontal position
				if (	((A_PtrRect->left <= A_StX) && (A_PtrRect->right > A_StX) ) || 
							((A_PtrRect->left <= A_EdX) && (A_PtrRect->right > A_EdX) ) ||
							((A_StX < A_StY) && ((A_PtrRect->left > A_StX) && (A_PtrRect->right <= A_EdX))) || 
							((A_StX > A_StY) && ((A_PtrRect->left > A_EdX) && (A_PtrRect->right <= A_StX))))
				{
					//patch start x
					if (A_PtrRect->left > A_StX )
					{
						*A_PtrRtStX		=	A_PtrRect->left;
					}
					if(A_PtrRect->left > A_EdX)
					{
						*A_PtrRtEdX		=	A_PtrRect->left;
					}
					//patch end x
					if ( A_PtrRect->right < A_EdX )
					{
						*A_PtrRtEdX	=	A_PtrRect->right - 1;
					}
					if(A_PtrRect->right < A_StX)
					{
						*A_PtrRtStX	=	A_PtrRect->right - 1;
					}
					Tv_Result	=	E_GrDrawLineHorizontal;
				}
			}
		}
		else if ( A_StX == A_EdX )		//vertical line
		{
			//in horizontal positon
			if ( (A_StX >= A_PtrRect->left) && (A_StX < A_PtrRect->right) )
			{
				//in vertical position
				if (	((A_PtrRect->top <= A_StY) && (A_PtrRect->bottom > A_StY) ) || 
							((A_PtrRect->top <= A_EdY) && (A_PtrRect->bottom > A_EdY) ) || 
							((A_StY < A_EdY) && ((A_PtrRect->top > A_StY) && (A_PtrRect->bottom  <= A_EdY))) || 
							((A_StY > A_EdY) && ((A_PtrRect->top > A_EdY) && (A_PtrRect->bottom  <= A_StY))))
				{
					//patch start y
					if (A_PtrRect->top > A_StY )
					{
						*A_PtrRtStY		=	A_PtrRect->top;
					}
					if(A_PtrRect->top > A_EdY)
					{
						*A_PtrRtEdY		=	A_PtrRect->top;
					}
					//patch end y
					if ( A_PtrRect->bottom < A_EdY )
					{
						*A_PtrRtEdY	=	A_PtrRect->bottom - 1;
					}
					if(A_PtrRect->bottom < A_StY)
					{
						*A_PtrRtStY	=	A_PtrRect->bottom - 1;
					}
					Tv_Result	=	E_GrDrawLineVertical;
				}
			}
		}
		else		//diagonal line
		{
			//diagonal
			Tv_Result	=	E_GrDrawLineDiagonal;
		}
		
		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawLineD2Init( Ptr_GrDrawLineD2 A_PtrLineStat, __s32 A_StX, __s32 A_StY, __s32 A_EdX, __s32 A_EdY,
													Ptr_GrRect A_PtrDrawAbleRect	)
{
	// local -------------------
		BOOL8		Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		A_PtrLineStat->RectDrawAble.left		=	0;
		A_PtrLineStat->RectDrawAble.top			=	0;
		A_PtrLineStat->RectDrawAble.right		=	0x7FFFFFFF;
		A_PtrLineStat->RectDrawAble.bottom	=	0x7FFFFFFF;
		A_PtrLineStat->IsPxShow		=	FALSE;

		//update drawable rect
		if ( NULL != A_PtrDrawAbleRect )
		{
			//update rect
			A_PtrLineStat->RectDrawAble	=	*A_PtrDrawAbleRect;
		}

		//check line position in drawable rect
		A_PtrLineStat->LineType	=	GrDrawGetLineTypeOnRect( &A_PtrLineStat->RectDrawAble, A_StX,A_StY,A_EdX,A_EdY,
															&A_PtrLineStat->StX,&A_PtrLineStat->StY,&A_PtrLineStat->EdX,&A_PtrLineStat->EdY	);
		if ( E_GrDrawLineNotAble != A_PtrLineStat->LineType	)
		{
			//calculate internerl value
			A_PtrLineStat->Dx		=	A_PtrLineStat->EdX - A_PtrLineStat->StX;
			A_PtrLineStat->Dy		=	A_PtrLineStat->EdY - A_PtrLineStat->StY;
			A_PtrLineStat->DirX	=	1;
			if ( 0 > A_PtrLineStat->Dx )
			{
				A_PtrLineStat->DirX	=	-1;
				A_PtrLineStat->Dx		=	-A_PtrLineStat->Dx;
			}
			A_PtrLineStat->DirY	=	1;
			if ( 0 > A_PtrLineStat->Dy )
			{
				A_PtrLineStat->DirY	=	-1;
				A_PtrLineStat->Dy		=	-A_PtrLineStat->Dy;
			}

			//other set
			A_PtrLineStat->Etm					=	0;
			A_PtrLineStat->NowX				=	A_PtrLineStat->StX;
			A_PtrLineStat->NowY				=	A_PtrLineStat->StY;
			A_PtrLineStat->IsInited		=	TRUE;
			A_PtrLineStat->IsDrawing	=	TRUE;

			//check show able
			if (	(A_PtrLineStat->NowX >= A_PtrLineStat->RectDrawAble.left) && (A_PtrLineStat->NowX < A_PtrLineStat->RectDrawAble.right) && 
						(A_PtrLineStat->NowY >= A_PtrLineStat->RectDrawAble.top) && (A_PtrLineStat->NowY < A_PtrLineStat->RectDrawAble.bottom)	)
			{
				A_PtrLineStat->IsPxShow	=	TRUE;
			}
			//check one pixel
			if ( (A_PtrLineStat->NowX == A_PtrLineStat->EdX) && (A_PtrLineStat->NowY == A_PtrLineStat->EdY) )
			{
				A_PtrLineStat->IsDrawing	=	FALSE;
			}
			//success try
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;

}

//--------------------------------------------------------------------
BOOL8		GrDrawLineD2CalNext(	Ptr_GrDrawLineD2 A_PtrLineStat )
{
	// local -------------------
		BOOL8		Tv_Result;
		BOOL8		Tv_IsShowAble;
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//check drawable
		if ( (A_PtrLineStat->IsInited) && (A_PtrLineStat->IsDrawing) )
		{
			switch( A_PtrLineStat->LineType )
			{
				case	E_GrDrawLineVertical:
					A_PtrLineStat->NowY		=	A_PtrLineStat->NowY + A_PtrLineStat->DirY;
					break;
				case	E_GrDrawLineHorizontal:
					A_PtrLineStat->NowX		=	A_PtrLineStat->NowX + A_PtrLineStat->DirX;
					break;
				case	E_GrDrawLineDiagonal:
					if(A_PtrLineStat->Dx > A_PtrLineStat->Dy)
					{
						A_PtrLineStat->NowX		=	A_PtrLineStat->NowX + A_PtrLineStat->DirX;
						A_PtrLineStat->Etm		=	A_PtrLineStat->Etm + A_PtrLineStat->Dy;
						if(A_PtrLineStat->Etm >= A_PtrLineStat->Dx)
						{
							A_PtrLineStat->Etm			=	A_PtrLineStat->Etm - A_PtrLineStat->Dx;
							A_PtrLineStat->NowY		=	A_PtrLineStat->NowY + A_PtrLineStat->DirY;
						}
					}
					else
					{
						A_PtrLineStat->NowY		=	A_PtrLineStat->NowY + A_PtrLineStat->DirY;
						A_PtrLineStat->Etm		=	A_PtrLineStat->Etm + A_PtrLineStat->Dx;
						if(A_PtrLineStat->Etm >= A_PtrLineStat->Dy)
						{
							A_PtrLineStat->Etm			=	A_PtrLineStat->Etm - A_PtrLineStat->Dy;
							A_PtrLineStat->NowX		=	A_PtrLineStat->NowX + A_PtrLineStat->DirX;
						}
					}

					//check able show
					Tv_IsShowAble	=	FALSE;
					if (	(A_PtrLineStat->NowX >= A_PtrLineStat->RectDrawAble.left) && (A_PtrLineStat->NowX < A_PtrLineStat->RectDrawAble.right) && 
						(A_PtrLineStat->NowY >= A_PtrLineStat->RectDrawAble.top) && (A_PtrLineStat->NowY < A_PtrLineStat->RectDrawAble.bottom)	)
					{
						Tv_IsShowAble	=	TRUE;
					}
					//check continuable
					if ( !Tv_IsShowAble )
					{
						if ( A_PtrLineStat->IsPxShow )
						{
							A_PtrLineStat->IsDrawing		=	FALSE;		//not continue
						}
					}
					A_PtrLineStat->IsPxShow		=	Tv_IsShowAble;

					break;
			}

			//check finish
			if ( (A_PtrLineStat->NowX == A_PtrLineStat->EdX) && (A_PtrLineStat->NowY == A_PtrLineStat->EdY) )
			{
				A_PtrLineStat->IsDrawing		=	FALSE;
			}
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}

//--------------------------------------------------------------------
__s32			GrDrawGetPixelByteByFcc( __u32 A_Fcc )
{
	// local -------------------
		__s32			Tv_Result;
  // code --------------------
		//analysis Fcc
		switch	( A_Fcc )
		{
			case	E_GrFccYUY2:
			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
			case	E_GrFccRGB555:
			case	E_GrFccRGB565:
			case E_GrFccARGB1555:
			case E_GrFccARGB4444:
				Tv_Result		=	2;
				break;

			case	E_GrFccRGB:
			case	E_GrFccRGB888:
			case	E_GrFccBGR888:
				Tv_Result		=	3;
				break;

			case	E_GrFccARGB8888:
			case	E_GrFccABGR8888:
			case	E_GrFccUVYA:
				Tv_Result		=	4;
				break;
			default:
				Tv_Result		=	0;
				break;
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
__s32			GrDrawCalcDefaultPitch( __s32 A_PixelByte, __s32 A_SizeX, __s32 A_PitchUnit )
{
	// local -------------------
		__s32			Tv_Result;
		__s32			Tv_ModUnit;
		__s32			Tv_Adder;
  // code --------------------
		//init
		Tv_Result		=	0;
		Tv_Adder		=	0;

		//calculate
		Tv_Result		=	A_PixelByte * A_SizeX;
		Tv_ModUnit	=	Tv_Result % A_PitchUnit;
		if ( 0 != Tv_ModUnit )
		{
			Tv_Adder	=	A_PitchUnit - Tv_ModUnit;			
		}

		Tv_Result		=	Tv_Result + Tv_Adder;

		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawIsCoupleFormat( __u32 A_Fcc )
{
	// local -------------------
		BOOL8			Tv_Result;
  // code --------------------

		//analysis
		switch	( A_Fcc )
		{
			case	E_GrFccYUY2:
			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
				Tv_Result		=	TRUE;
				break;

			default:
				Tv_Result		=	FALSE;
				break;
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrDrawIsAlphaFormat( __u32 A_Fcc )
{
	// local -------------------
		BOOL8			Tv_Result;
	// code --------------------

		//analysis
		switch	( A_Fcc )
		{
			case	E_GrFccARGB8888:
			case	E_GrFccABGR8888:
			case	E_GrFccUYVYF:
			case	E_GrFccUVYA:
				Tv_Result		=	TRUE;
				break;
			default:
				Tv_Result		=	FALSE;
				break;
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrDrawIsFccTypeYUV( __u32 A_Fcc )
{
	// local -------------------
		BOOL8		Tv_Result;
  // code --------------------
		//analysis fcc
		switch	( A_Fcc )
		{
			case	E_GrFccYUY2:
			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
			case	E_GrFccUVYA:
				Tv_Result		=	TRUE;
				break;

			default:
				Tv_Result		=	FALSE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawGetColorElements( __u32 A_Fcc, __u32 A_Color, __s32* A_PtrR,__s32* A_PtrG,__s32* A_PtrB, __s32* A_PtrA,
																__s32* A_PtrY, __s32* A_PtrU, __s32* A_PtrV )
{
	// local -------------------
		BOOL8		Tv_Result;				//is YUV

  // code --------------------
		//init
		Tv_Result		=	FALSE;
		*A_PtrR			=	0;
		*A_PtrG			=	0;
		*A_PtrB			=	0;
		*A_PtrA			=	0xFF;
		*A_PtrY			=	0;
		*A_PtrU			=	0;
		*A_PtrV			=	0;

		//analysis fcc
		switch	( A_Fcc )
		{
			case	E_GrFccRGB555:
			case	E_GrFccARGB1555:
				*A_PtrR		=	(__s32)(A_Color & 0x7C00) >> 7;
				*A_PtrG		=	(__s32)(A_Color & 0x03E0) >> 2;
				*A_PtrB		=	(__s32)(A_Color & 0x001F) << 3;
				break;
			case E_GrFccARGB4444:
				*A_PtrR		=	(__s32)(A_Color & 0x0F00) >> 4;
				*A_PtrG		=	(__s32)(A_Color & 0x00F0);
				*A_PtrB		=	(__s32)(A_Color & 0x000F) << 4;
				break;
			case	E_GrFccRGB565:
				*A_PtrR		=	(__s32)(A_Color & 0xF800) >> 8;
				*A_PtrG		=	(__s32)(A_Color & 0x07E0) >> 3;
				*A_PtrB		=	(__s32)(A_Color & 0x001F) << 3;
				break;

			case	E_GrFccRGB:
			case	E_GrFccRGB888:
				*A_PtrR		=	((Ptr_GrImgFmtRGB888)&A_Color)->R;
				*A_PtrG		=	((Ptr_GrImgFmtRGB888)&A_Color)->G;
				*A_PtrB		=	((Ptr_GrImgFmtRGB888)&A_Color)->B;
				break;

			case	E_GrFccARGB8888:
				*A_PtrR		=	((Ptr_GrImgFmtARGB8888)&A_Color)->R;
				*A_PtrG		=	((Ptr_GrImgFmtARGB8888)&A_Color)->G;
				*A_PtrB		=	((Ptr_GrImgFmtARGB8888)&A_Color)->B;
				*A_PtrA		=	((Ptr_GrImgFmtARGB8888)&A_Color)->A;
				break;
				
			case	E_GrFccBGR888:
				*A_PtrR		=	((Ptr_GrImgFmtBGR888)&A_Color)->R;
				*A_PtrG		=	((Ptr_GrImgFmtBGR888)&A_Color)->G;
				*A_PtrB		=	((Ptr_GrImgFmtBGR888)&A_Color)->B;
				break;

			case	E_GrFccABGR8888:
				*A_PtrR		=	((Ptr_GrImgFmtABGR8888)&A_Color)->R;
				*A_PtrG		=	((Ptr_GrImgFmtABGR8888)&A_Color)->G;
				*A_PtrB		=	((Ptr_GrImgFmtABGR8888)&A_Color)->B;
				*A_PtrA		=	((Ptr_GrImgFmtABGR8888)&A_Color)->A;
				break;

			case	E_GrFccYUY2:
				*A_PtrY		=	((Ptr_GrImgFmtYUY2)&A_Color)->Y1;
				*A_PtrU		=	((Ptr_GrImgFmtYUY2)&A_Color)->U;
				*A_PtrV		=	((Ptr_GrImgFmtYUY2)&A_Color)->V;
				Tv_Result	=	TRUE;
				break;

			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
				*A_PtrY		=	((Ptr_GrImgFmtUYVY)&A_Color)->Y1;
				*A_PtrU		=	((Ptr_GrImgFmtUYVY)&A_Color)->U;
				*A_PtrV		=	((Ptr_GrImgFmtUYVY)&A_Color)->V;
				Tv_Result	=	TRUE;	
				break;
			case	E_GrFccUVYA:
				*A_PtrY		=	((Ptr_GrImgFmtUVYA)&A_Color)->Y;
				*A_PtrU		=	((Ptr_GrImgFmtUVYA)&A_Color)->U;
				*A_PtrV		=	((Ptr_GrImgFmtUVYA)&A_Color)->V;
				*A_PtrA		=	((Ptr_GrImgFmtUVYA)&A_Color)->A;
				Tv_Result	=	TRUE;	
				break;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
__u32		GrDrawMakeColorByElements( __u32 A_Fcc, __s32 A_R_Y, __s32 A_G_U, __s32 A_B_V, __s32 A_A )
{
	// local -------------------
		__u32			Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	0;

		//analysis fcc
		switch	( A_Fcc )
		{
			case	E_GrFccRGB555:
				Tv_Result	=	(__u32)((A_R_Y << 7) & 0x7C00);
        Tv_Result  = Tv_Result | (((__u32)A_G_U << 2) & 0x03E0);
        Tv_Result  = Tv_Result | (((__u32)A_B_V >> 3) & 0x001F);
				break;
			case	E_GrFccARGB1555:
				Tv_Result	=	(__u32)((A_R_Y << 7) & 0x7C00);
				Tv_Result  = Tv_Result | (((__u32)A_G_U << 2) & 0x03E0);
				Tv_Result  = Tv_Result | (((__u32)A_B_V >> 3) & 0x001F);
				if(0 != A_A)
				{
					Tv_Result  = Tv_Result | 0x8000;
				}
				break;
			case E_GrFccARGB4444:
				Tv_Result	=	(__u32)((A_R_Y << 4) & 0x0F00);
				Tv_Result  = Tv_Result | (((__u32)A_G_U ) & 0x00F0);
				Tv_Result  = Tv_Result | (((__u32)A_B_V >> 4) & 0x000F);
				break;
			case	E_GrFccRGB565:
        Tv_Result  = ((__u32)A_R_Y << 8) & 0xF800;
        Tv_Result  = Tv_Result | (((__u32)A_G_U << 3) & 0x07E0);
        Tv_Result  = Tv_Result | (((__u32)A_B_V >> 3) & 0x001F);
				break;

			case	E_GrFccRGB:
			case	E_GrFccRGB888:
				((Ptr_GrImgFmtRGB888)&Tv_Result)->R	= (__u8)A_R_Y;
				((Ptr_GrImgFmtRGB888)&Tv_Result)->G	= (__u8)A_G_U;
				((Ptr_GrImgFmtRGB888)&Tv_Result)->B	= (__u8)A_B_V;
				break;

			case	E_GrFccARGB8888:
				((Ptr_GrImgFmtARGB8888)&Tv_Result)->R	= (__u8)A_R_Y;
				((Ptr_GrImgFmtARGB8888)&Tv_Result)->G	= (__u8)A_G_U;
				((Ptr_GrImgFmtARGB8888)&Tv_Result)->B	= (__u8)A_B_V;
				((Ptr_GrImgFmtARGB8888)&Tv_Result)->A	=	(__u8)A_A;
				break;
				
			case	E_GrFccBGR888:
				((Ptr_GrImgFmtBGR888)&Tv_Result)->R	= (__u8)A_R_Y;
				((Ptr_GrImgFmtBGR888)&Tv_Result)->G	= (__u8)A_G_U;
				((Ptr_GrImgFmtBGR888)&Tv_Result)->B = (__u8)A_B_V;
				break;

			case	E_GrFccABGR8888:
				((Ptr_GrImgFmtABGR8888)&Tv_Result)->R	= (__u8)A_R_Y;
				((Ptr_GrImgFmtABGR8888)&Tv_Result)->G	= (__u8)A_G_U;
				((Ptr_GrImgFmtABGR8888)&Tv_Result)->B	= (__u8)A_B_V;
				((Ptr_GrImgFmtABGR8888)&Tv_Result)->A	=	(__u8)A_A;
				break;

			case	E_GrFccYUY2:
        ((Ptr_GrImgFmtYUY2)&Tv_Result)->Y1	=	(__u8)A_R_Y;
				((Ptr_GrImgFmtYUY2)&Tv_Result)->U		=	(__u8)A_G_U;
				((Ptr_GrImgFmtYUY2)&Tv_Result)->V		=	(__u8)A_B_V;
				((Ptr_GrImgFmtYUY2)&Tv_Result)->Y2	=	(__u8)A_R_Y;
				break;

			case	E_GrFccUYVY:
				((Ptr_GrImgFmtUYVY)&Tv_Result)->Y1	=	(__u8)A_R_Y;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->U	=	(__u8)A_G_U;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->V	=	(__u8)A_B_V;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->Y2	=	(__u8)A_R_Y;
				break;
			case	E_GrFccUYVYF:
        ((Ptr_GrImgFmtUYVY)&Tv_Result)->Y1	=	(__u8)A_R_Y;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->U	=	(__u8)A_G_U;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->V	=	(__u8)A_B_V;
				((Ptr_GrImgFmtUYVY)&Tv_Result)->Y2	=	(__u8)A_R_Y;
				Tv_Result	=	Tv_Result & 0xFEFFFEFF;	//remove alpha
				break;
			case	E_GrFccUVYA:
				((Ptr_GrImgFmtUVYA)&Tv_Result)->Y		=	(__u8)A_R_Y;
				((Ptr_GrImgFmtUVYA)&Tv_Result)->U		=	(__u8)A_G_U;
				((Ptr_GrImgFmtUVYA)&Tv_Result)->V		=	(__u8)A_B_V;
				((Ptr_GrImgFmtUVYA)&Tv_Result)->A		=	(__u8)A_A;
				break;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
__u32		GrDrawCvtColorByFcc( __u32 A_FccSrc, __u32 A_ColorSrc, __u32 A_FccDst )
{
	// local -------------------
		__u32		Tv_Result;
		BOOL8		Tv_IsSrcYUV;
		BOOL8		Tv_IsDstYUV;

		__s32			Tv_ValY;
		__s32			Tv_ValU;
		__s32			Tv_ValV;

		__s32			Tv_ValR;
		__s32			Tv_ValG;
		__s32			Tv_ValB;
		__s32			Tv_ValA;

  // code --------------------
		//init
		Tv_Result		=	0;

		//check equal fcc

		//analysis type
		Tv_IsSrcYUV		=	GrDrawGetColorElements( A_FccSrc, A_ColorSrc, &Tv_ValR,&Tv_ValG,&Tv_ValB, &Tv_ValA,
										&Tv_ValY,&Tv_ValU,&Tv_ValV );
		Tv_IsDstYUV		=	GrDrawIsFccTypeYUV( A_FccDst );

		//check dest yuv
		if ( Tv_IsDstYUV )
		{
			//check convert
			if ( !Tv_IsSrcYUV )
			{
				//convert
        Tv_ValY		=	(( (66 * Tv_ValR) + (129 * Tv_ValG) +
                    ( 25 * Tv_ValB ) ) / 256 ) + 16;
        Tv_ValU   =	(	( -(38 * Tv_ValR)
											-(74 * Tv_ValG)
											+(112 * Tv_ValB)
                    ) / 256 ) + 128;
        Tv_ValV   =	((	(112 * Tv_ValR)-
                        (94 * Tv_ValG) -
                        (18 * Tv_ValB) ) / 256 )
                        + 128;
				//saturation
        if	(0 > Tv_ValY)
				{
					Tv_ValY		=	0;
				}
        else if (255 < Tv_ValY)
				{
					Tv_ValY		= 255;
				}

        if	(0 > Tv_ValU)
				{
					Tv_ValU		=	0;
				}
        else if (255 < Tv_ValU)
				{
					Tv_ValU		= 255;
				}

        if	(0 > Tv_ValV)
				{
					Tv_ValV		=	0;
				}
        else if (255 < Tv_ValV)
				{
					Tv_ValV		= 255;
				}
			}

			//update result
			Tv_Result		=	GrDrawMakeColorByElements( A_FccDst, Tv_ValY,Tv_ValU,Tv_ValV, 0 );
		}
		else			//dest type rgb
		{	
			//check need convert
			if ( Tv_IsSrcYUV )
			{
				//convert
        //red
        Tv_ValR		= (1192*(Tv_ValY - 16) + 1634*(Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValR )
				{
					Tv_ValR		=	0;
				}
				else if	( 255 < Tv_ValR )
				{
					Tv_ValR		=	255;
				}

        //green
        Tv_ValG		=	(1192*(Tv_ValY - 16) - 401*(Tv_ValU - 128)
                    - 833*(Tv_ValV - 128)) / 1024;
				if			( 0 > Tv_ValG )
				{
					Tv_ValG		=	0;
				}
				else if	( 255 < Tv_ValG )
				{
					Tv_ValG		=	255;
				}

        //blue
        Tv_ValB   =	(1192*(Tv_ValY - 16) + 2065*(Tv_ValU - 128)) / 1024;
				if			( 0 > Tv_ValB )
				{
					Tv_ValB		=	0;
				}
				else if	( 255 < Tv_ValB )
				{
					Tv_ValB		=	255;
				}
			}

			//update result
			Tv_Result		=	GrDrawMakeColorByElements( A_FccDst, Tv_ValR,Tv_ValG,Tv_ValB, Tv_ValA );
		}
		// patch color
#if 0
		switch ( A_FccDst )
		{
			case E_GrFccARGB4444:
				Tv_Result	=	Tv_Result | 0xF000;
				break;
			case E_GrFccARGB8888:
				Tv_Result	=	Tv_Result | 0xFF000000;
				break;
			case E_GrFccABGR8888:
				Tv_Result	=	Tv_Result | 0x000000FF;
				break;
		}
#endif

		return		Tv_Result;

}
//--------------------------------------------------------------------
BOOL8		GrDrawPatchRelatePosWithRect( Ptr_GrRect A_PtrRectBasis, Ptr_GrRect A_PtrRectSrc, 
																			__s32* A_PtrPosX, __s32* A_PtrPosY )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_X;
		__s32			Tv_Y;
		__s32			Tv_Dx;
		__s32			Tv_Dy;
  // code --------------------
		//init
		Tv_Result		=	FALSE;
		Tv_X				=	*A_PtrPosX;
		Tv_Y				=	*A_PtrPosY;
		Tv_Dx				=	A_PtrRectSrc->right - A_PtrRectSrc->left;
		Tv_Dy				=	A_PtrRectSrc->bottom - A_PtrRectSrc->top;

		//patch start position
		if ( Tv_X < A_PtrRectBasis->left )
		{	
			A_PtrRectSrc->left	=	A_PtrRectSrc->left - ( A_PtrRectBasis->left - Tv_X );
			Tv_X								=	A_PtrRectBasis->left;
		}

		if ( Tv_Y < A_PtrRectBasis->top )
		{
			A_PtrRectSrc->top		=	A_PtrRectSrc->top - ( A_PtrRectBasis->top - Tv_Y );
			Tv_Y								=	A_PtrRectBasis->top;
		}

		//check over size
		if ( (Tv_X + Tv_Dx) > A_PtrRectBasis->right )
		{
			A_PtrRectSrc->right		=	A_PtrRectSrc->right - ((Tv_X + Tv_Dx) - A_PtrRectBasis->right);
		}

		if ( (Tv_Y + Tv_Dy) > A_PtrRectBasis->bottom )
		{
			A_PtrRectSrc->bottom	=	A_PtrRectSrc->bottom - ((Tv_Y + Tv_Dy) - A_PtrRectBasis->bottom);
		}

		//check not usable rect
		if ( (A_PtrRectSrc->right > A_PtrRectSrc->left) && (A_PtrRectSrc->bottom > A_PtrRectSrc->top) )
		{
			//update result
			*A_PtrPosX	=	Tv_X;
			*A_PtrPosY	=	Tv_Y;
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
/*!
 * \brief
 * collect blt parameter
 * 
 * \param A_PtrRectDstBase
 * base rect of dest
 * 
 * \param A_PtrPosX
 * pointer of position x , it will be patch.
 * 
 * \param A_PtrPosY
 * pointer of position y , it will be patch.
 * 
 * \param A_PtrRectSrcBase
 * base rect of source
 * 
 * \param A_PtrRectSrcCont
 * draw rect of source , it will be patch.
 * 
 * \returns
 * TREU = Can draw.
 * 
 */
/*
		not use
BOOL8		GrDrawPatchBltPara( Ptr_GrRect A_PtrRectDstBase, __s32* A_PtrPosX, __s32* A_PtrPosY,
														Ptr_GrRect A_PtrRectSrcBase, Ptr_GrRect A_PtrRectSrcCont )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_X;
		__s32			Tv_Y;
		St_GrRect	Tv_RectSrc;
		__s32			Tv_Dx;
		__s32			Tv_Dy;
  // code --------------------
		//init
		Tv_Result		=	FALSE;
		Tv_X				=	*A_PtrPosX;
		Tv_Y				=	*A_PtrPosY;
		Tv_RectSrc	=	*A_PtrRectSrcCont;

		//patch source rect
		if ( Tv_RectSrc.left < A_PtrRectSrcBase->left )
		{
			Tv_X			=	Tv_X + (A_PtrRectSrcBase->left - Tv_RectSrc.left);
			Tv_RectSrc.left		=	A_PtrRectSrcBase->left;
		}
		if ( Tv_RectSrc.top < A_PtrRectSrcBase->top )
		{
			Tv_Y			=	Tv_Y + (A_PtrRectSrcBase->top - Tv_RectSrc.top);
			Tv_RectSrc.top		=	A_PtrRectSrcBase->top;
		}
		if ( Tv_RectSrc.right > A_PtrRectSrcBase->right )
		{
			Tv_RectSrc.right	=	A_PtrRectSrcBase->right;
		}
		if ( Tv_RectSrc.bottom > A_PtrRectSrcBase->bottom )
		{
			Tv_RectSrc.bottom	=	A_PtrRectSrcBase->bottom;
		}

		//patch start position
		if ( Tv_X < A_PtrRectDstBase->left )
		{	
			Tv_RectSrc.left	=	Tv_RectSrc.left + ( A_PtrRectDstBase->left - Tv_X );
			Tv_X						=	A_PtrRectDstBase->left;
		}
		if ( Tv_Y < A_PtrRectDstBase->top )
		{
			Tv_RectSrc.top		=	Tv_RectSrc.top + ( A_PtrRectDstBase->top - Tv_Y );
			Tv_Y							=	A_PtrRectDstBase->top;
		}

		//update delta
		Tv_Dx				=	Tv_RectSrc.right - Tv_RectSrc.left;
		Tv_Dy				=	Tv_RectSrc.bottom - Tv_RectSrc.top;

		//check over size
		if ( (Tv_X + Tv_Dx) > A_PtrRectDstBase->right )
		{
			Tv_RectSrc.right		=	Tv_RectSrc.right - ((Tv_X + Tv_Dx) - A_PtrRectDstBase->right);
		}
		if ( (Tv_Y + Tv_Dy) > A_PtrRectDstBase->bottom )
		{
			Tv_RectSrc.bottom	=	Tv_RectSrc.bottom - ((Tv_Y + Tv_Dy) - A_PtrRectDstBase->bottom);
		}

		//check not usable rect
		if ( (Tv_RectSrc.right > Tv_RectSrc.left) && (Tv_RectSrc.bottom > Tv_RectSrc.top) )
		{
			//check current source
			if (	(Tv_RectSrc.left >= A_PtrRectSrcBase->left) && 
						(Tv_RectSrc.right <= A_PtrRectSrcBase->right ) && 		
						(Tv_RectSrc.top >= A_PtrRectSrcBase->top ) && 		
						(Tv_RectSrc.bottom <= A_PtrRectSrcBase->bottom )
				)
			{
				//update result
				*A_PtrPosX	=	Tv_X;
				*A_PtrPosY	=	Tv_Y;
				*A_PtrRectSrcCont	=	Tv_RectSrc;
				Tv_Result		=	TRUE;
			}
		}

		return		Tv_Result;
}
*/
//--------------------------------------------------------------------
BOOL8		GrDrawPatchStretchPara(	Ptr_GrRect A_PtrRectDstMask, Ptr_GrRect A_PtrRectDstOrg, 
																Ptr_GrRect A_PtrRectSrcMask, Ptr_GrRect A_PtrRectSrcOrg,
																Ptr_GrRect A_PtrRtRectDst )
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_SrDx;
		__s32			Tv_SrDy;
		__s32			Tv_TgDx;
		__s32			Tv_TgDy;
		St_GrRect	Tv_RcPthSrc;
		St_GrRect	Tv_RcPthDst;
		St_GrRect	Tv_RcS2d;				//source to dest rect
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		Tv_SrDx			=	A_PtrRectSrcOrg->right - A_PtrRectSrcOrg->left;
		Tv_SrDy			=	A_PtrRectSrcOrg->bottom - A_PtrRectSrcOrg->top;
		Tv_TgDx			=	A_PtrRectDstOrg->right - A_PtrRectDstOrg->left;
		Tv_TgDy			=	A_PtrRectDstOrg->bottom - A_PtrRectDstOrg->top;
		if ( GrDrawCnGetComRect( &Tv_RcPthSrc, A_PtrRectSrcOrg, A_PtrRectSrcMask ) )
		{
			if ( GrDrawCnGetComRect( &Tv_RcPthDst, A_PtrRectDstOrg, A_PtrRectDstMask ) )
			{
				Tv_RcS2d.left		=	A_PtrRectDstOrg->left + (( Tv_RcPthSrc.left - A_PtrRectSrcOrg->left ) * Tv_TgDx ) / Tv_SrDx;
				Tv_RcS2d.top		=	A_PtrRectDstOrg->top + (( Tv_RcPthSrc.top - A_PtrRectSrcOrg->top ) * Tv_TgDy ) / Tv_SrDy;
				Tv_RcS2d.right	=	A_PtrRectDstOrg->left + (( Tv_RcPthSrc.right - A_PtrRectSrcOrg->left ) * Tv_TgDx ) / Tv_SrDx;
				Tv_RcS2d.bottom	=	A_PtrRectDstOrg->top + (( Tv_RcPthSrc.bottom - A_PtrRectSrcOrg->top ) * Tv_TgDy ) / Tv_SrDy;
				//check common
				if ( GrDrawCnGetComRect( A_PtrRtRectDst, &Tv_RcS2d, &Tv_RcPthDst ) )
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
/*!
 * \brief
 * get common rect from between two rect
 * 
 * \param A_PtrRtRect
 * pointer of recive result rect
 * 
 * \param A_PtrRect1
 * pointer of source rect 1
 * 
 * \param A_PtrRect2
 * pointer of source rect 2
 * 
 * \returns
 * if exist common rect , return TRUE
 * 
 */
BOOL8		GrDrawCnGetComRect( Ptr_GrRect A_PtrRtRect, Ptr_GrRect A_PtrRect1, 
															Ptr_GrRect A_PtrRect2 )
{
	// local -------------------
		BOOL8		Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	FALSE;
		*A_PtrRtRect	=	*A_PtrRect1;

		//check common rect
		if ( A_PtrRect1->left < A_PtrRect2->left )
		{
			A_PtrRtRect->left		=	A_PtrRect2->left;
		}

		if ( A_PtrRect1->top < A_PtrRect2->top )
		{
			A_PtrRtRect->top		=	A_PtrRect2->top;
		}

		if ( A_PtrRect1->right > A_PtrRect2->right )
		{
			A_PtrRtRect->right	=	A_PtrRect2->right;
		}

		if ( A_PtrRect1->bottom > A_PtrRect2->bottom )
		{
			A_PtrRtRect->bottom	=	A_PtrRect2->bottom;
		}

		//check valid rect
		if ( (A_PtrRtRect->left < A_PtrRtRect->right) && (A_PtrRtRect->top < A_PtrRtRect->bottom)	)
		{
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawCnGetRelPosX( Ptr_GrRect A_PtrRelateRect, __s32 A_X, __s32* A_PtrRtPos )
{
	// local -------------------
		BOOL8			Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//result position
		*A_PtrRtPos		=	A_X + A_PtrRelateRect->left;

		//check position in size
		if ( (*A_PtrRtPos >= A_PtrRelateRect->left) && (*A_PtrRtPos < A_PtrRelateRect->right) )
		{
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;

}

//--------------------------------------------------------------------
BOOL8		GrDrawCnGetRelPosY( Ptr_GrRect A_PtrRelateRect, __s32 A_Y, __s32* A_PtrRtPos )
{
	// local -------------------
		BOOL8			Tv_Result;
  // code --------------------
		//init
		Tv_Result		=	FALSE;

		//result position
		*A_PtrRtPos		=	A_Y + A_PtrRelateRect->top;

		//check position in size
		if ( (*A_PtrRtPos >= A_PtrRelateRect->top) && (*A_PtrRtPos < A_PtrRelateRect->bottom) )
		{
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;

}

//--------------------------------------------------------------------
BOOL8		GrDrawCnGetRelRect( Ptr_GrRect A_PtrRelateRect, Ptr_GrRect A_PtrRect, Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8			Tv_Result;

  // code --------------------
		//init
		Tv_Result		=	TRUE;

		//result rect
		A_PtrRtRect->left		=	A_PtrRect->left   + A_PtrRelateRect->left;
		A_PtrRtRect->top		=	A_PtrRect->top    + A_PtrRelateRect->top;
		A_PtrRtRect->right	=	A_PtrRect->right  + A_PtrRelateRect->left;
		A_PtrRtRect->bottom	=	A_PtrRect->bottom + A_PtrRelateRect->top;

		//check over range
		if	(	(A_PtrRtRect->left >= A_PtrRelateRect->left)&&(A_PtrRtRect->left < A_PtrRelateRect->right)&&
					(A_PtrRtRect->top >= A_PtrRelateRect->top)&&(A_PtrRtRect->top < A_PtrRelateRect->bottom)&&
					(A_PtrRtRect->right >= A_PtrRelateRect->left)&&(A_PtrRtRect->right < A_PtrRelateRect->right)&&
					(A_PtrRtRect->bottom >= A_PtrRelateRect->top)&&(A_PtrRtRect->bottom < A_PtrRelateRect->bottom)
				)
		{
			Tv_Result		=	TRUE;
		}

		return		Tv_Result;

}
//--------------------------------------------------------------------
__u32		GrDrawCnChkCoverRect( Ptr_GrRect A_PtrRect1, Ptr_GrRect A_PtrRect2 )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrDrawChkOvlpNotCover;

		//check cover rect1
		if ( (A_PtrRect1->left <= A_PtrRect2->left) && (A_PtrRect1->right >= A_PtrRect2->right) &&
			(A_PtrRect1->top <= A_PtrRect2->top) && (A_PtrRect1->bottom >= A_PtrRect2->bottom) )
		{
			Tv_Result		=	Tv_Result | E_GrDrawChkOvlpCoverPara1;
		}

		//check cover rect2
		if ( (A_PtrRect1->left >= A_PtrRect2->left) && (A_PtrRect1->right <= A_PtrRect2->right) &&
			(A_PtrRect1->top >= A_PtrRect2->top) && (A_PtrRect1->bottom <= A_PtrRect2->bottom) )
		{
			Tv_Result		=	Tv_Result | E_GrDrawChkOvlpCoverPara2;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawCnChkOvlpRect( Ptr_GrRect A_PtrRect1, Ptr_GrRect A_PtrRect2 )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	E_GrDrawChkOvlpNotOvlp;

		//check cover rect1 
		if ( (A_PtrRect1->left <= A_PtrRect2->left) && (A_PtrRect1->right >= A_PtrRect2->right) &&
			(A_PtrRect1->top <= A_PtrRect2->top) && (A_PtrRect1->bottom >= A_PtrRect2->bottom) )
		{
			Tv_Result		=	Tv_Result | E_GrDrawChkOvlpCoverPara1;
		}

		//check cover rect2
		if ( (A_PtrRect1->left >= A_PtrRect2->left) && (A_PtrRect1->right <= A_PtrRect2->right) &&
			(A_PtrRect1->top >= A_PtrRect2->top) && (A_PtrRect1->bottom <= A_PtrRect2->bottom) )
		{
			Tv_Result		=	Tv_Result | E_GrDrawChkOvlpCoverPara2;
		}

		//check part overlapped
		if ( E_GrDrawChkOvlpNotOvlp == Tv_Result )
		{
			if((A_PtrRect1->left < A_PtrRect2->right) && (A_PtrRect1->right > A_PtrRect2->left) && 
				(A_PtrRect1->top < A_PtrRect2->bottom) && (A_PtrRect1->bottom > A_PtrRect2->top))
			{
				Tv_Result		=	E_GrDrawChkOvlpOvlpPart;
			}
		}

		return	Tv_Result;
}

//--------------------------------------------------------------------
BOOL8		GrDrawCnPatRectBySize( __s32 A_SizeX, __s32 A_SizeY, Ptr_GrRect A_PtrSrRect, 
															Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		
		//patch left
		A_PtrRtRect->left		=	A_PtrSrRect->left;
		if ( 0 > A_PtrRtRect->left )
		{
			A_PtrRtRect->left		=	0;
		}

		//patch top
		A_PtrRtRect->top		=	A_PtrSrRect->top;
		if ( 0 > A_PtrRtRect->top )
		{
			A_PtrRtRect->top		=	0;
		}

		//patch right
		A_PtrRtRect->right	=	A_PtrSrRect->right;
		if ( A_SizeX < A_PtrRtRect->right )
		{
			A_PtrRtRect->right	=	A_SizeX;
		}

		//patch bottom
		A_PtrRtRect->bottom	=	A_PtrSrRect->bottom;
		if ( A_SizeY < A_PtrRtRect->bottom )
		{
			A_PtrRtRect->bottom	=	A_SizeY;
		}

		//check available 
		if ( (A_PtrRtRect->left < A_PtrRtRect->right ) && (A_PtrRtRect->top < A_PtrRtRect->bottom ) )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}

//--------------------------------------------------------------------
__u32		GrDrawSetColorLight( __u32 A_Fcc, __u32 A_ColorSrc, __u32 A_Ratio256base )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrImgFmtRGB888	Tv_PtrRgb32;
		__u32		Tv_ClrR;
		__u32		Tv_ClrG;
		__u32		Tv_ClrB;

	// code --------------------
		//init
		Tv_Result		=	A_ColorSrc;

		switch	( A_Fcc )
		{
			case	E_GrFccRGB:
			case	E_GrFccRGB888:
			case	E_GrFccBGR888:
			case	E_GrFccARGB8888:
			case	E_GrFccABGR8888:
				Tv_PtrRgb32	=		(Ptr_GrImgFmtRGB888)&A_ColorSrc;

				Tv_ClrR		=	(__u32)Tv_PtrRgb32->R;
				Tv_ClrG		=	(__u32)Tv_PtrRgb32->G;
				Tv_ClrB		=	(__u32)Tv_PtrRgb32->B;

				//calculate
				Tv_ClrR		=	(Tv_ClrR * A_Ratio256base) / 256;
				Tv_ClrG		=	(Tv_ClrG * A_Ratio256base) / 256;
				Tv_ClrB		=	(Tv_ClrB * A_Ratio256base) / 256;
				if ( 255 < Tv_ClrR )
				{
					Tv_ClrR		=	255;
				}
				if ( 255 < Tv_ClrG )
				{
					Tv_ClrG		=	255;
				}
				if ( 255 < Tv_ClrB )
				{
					Tv_ClrB		=	255;
				}

				//update result
				Tv_PtrRgb32	=	(Ptr_GrImgFmtRGB888)&Tv_Result;
				Tv_PtrRgb32->R	=	(__u8)Tv_ClrR;
				Tv_PtrRgb32->G	=	(__u8)Tv_ClrG;
				Tv_PtrRgb32->B	=	(__u8)Tv_ClrB;

				break;
			default:
				break;
		}

		return	Tv_Result;
}

//--------------------------------------------------------------------
__u32		GrDrawAddColorLight( __u32 A_Fcc, __u32 A_ColorSrc, __s32 A_Add )
{
	// local -------------------
		__u32		Tv_Result;
		Ptr_GrImgFmtRGB888	Tv_PtrRgb32;
		__s32			Tv_ClrR;
		__s32			Tv_ClrG;
		__s32			Tv_ClrB;

	// code --------------------
		//init
		Tv_Result		=	A_ColorSrc;

		switch	( A_Fcc )
		{
			case	E_GrFccRGB:
			case	E_GrFccRGB888:
			case	E_GrFccBGR888:
			case	E_GrFccARGB8888:
			case	E_GrFccABGR8888:
				Tv_PtrRgb32	=		(Ptr_GrImgFmtRGB888)&A_ColorSrc;

				Tv_ClrR		=	(__s32)(__u32)Tv_PtrRgb32->R;
				Tv_ClrG		=	(__s32)(__u32)Tv_PtrRgb32->G;
				Tv_ClrB		=	(__s32)(__u32)Tv_PtrRgb32->B;

				//calculate
				Tv_ClrR		=	Tv_ClrR + A_Add;
				Tv_ClrG		=	Tv_ClrG + A_Add;
				Tv_ClrB		=	Tv_ClrB + A_Add;
				if ( 0 > Tv_ClrR )
				{
					Tv_ClrR		=	0;
				}
				if ( 255 < Tv_ClrR )
				{
					Tv_ClrR		=	255;
				}
				if ( 0 > Tv_ClrG )
				{
					Tv_ClrG		=	0;
				}
				if ( 255 < Tv_ClrG )
				{
					Tv_ClrG		=	255;
				}
				if ( 0 > Tv_ClrB )
				{
					Tv_ClrB		=	0;
				}
				if ( 255 < Tv_ClrB )
				{
					Tv_ClrB		=	255;
				}

				//update result
				Tv_PtrRgb32	=	(Ptr_GrImgFmtRGB888)&Tv_Result;
				Tv_PtrRgb32->R	=	(__u8)Tv_ClrR;
				Tv_PtrRgb32->G	=	(__u8)Tv_ClrG;
				Tv_PtrRgb32->B	=	(__u8)Tv_ClrB;

				break;
			default:
				break;
		}

		return	Tv_Result;

}

//--------------------------------------------------------------------
__u32		GrDrawColorMix( __u32 A_Fcc, __u32 A_ColorSrc1, __u32 A_ColorSrc2 )
{
	// local -------------------
		__u32		Tv_Result;
		__s32			Tv_ClrR1;
		__s32			Tv_ClrG1;
		__s32			Tv_ClrB1;
		__s32			Tv_ClrA1;
		__s32			Tv_ClrY1;
		__s32			Tv_ClrU1;
		__s32			Tv_ClrV1;

		__s32			Tv_ClrGr;
		__s32			Tv_ClrG2;
		__s32			Tv_ClrB2;
		__s32			Tv_ClrA2;
		__s32			Tv_ClrY2;
		__s32			Tv_ClrU2;
		__s32			Tv_ClrV2;

		BOOL8		Tv_IsAlpha;

	// code --------------------
		//init
		GrDrawGetColorElements( A_Fcc, A_ColorSrc1, &Tv_ClrR1, &Tv_ClrG1, &Tv_ClrB1,
			&Tv_ClrA1, &Tv_ClrY1, &Tv_ClrU1, &Tv_ClrV1 );
		GrDrawGetColorElements( A_Fcc, A_ColorSrc2, &Tv_ClrGr, &Tv_ClrG2, &Tv_ClrB2,
			&Tv_ClrA2, &Tv_ClrY2, &Tv_ClrU2, &Tv_ClrV2 );

		Tv_IsAlpha	=	FALSE;
		if ( E_GrFccUYVYF == A_Fcc )
		{
			if ( 0 != (1 & Tv_ClrY1) )
			{
				Tv_IsAlpha	=	TRUE;
			}
		}

		//mix
		Tv_ClrR1	=	(Tv_ClrR1 + Tv_ClrGr) / 2;
		Tv_ClrG1	=	(Tv_ClrG1 + Tv_ClrG2) / 2;
		Tv_ClrB1	=	(Tv_ClrB1 + Tv_ClrB2) / 2;
		Tv_ClrA1	=	(Tv_ClrA1 + Tv_ClrA2) / 2;
		Tv_ClrY1	=	(Tv_ClrY1 + Tv_ClrY2) / 2;
		Tv_ClrU1	=	(Tv_ClrU1 + Tv_ClrU2) / 2;
		Tv_ClrV1	=	(Tv_ClrV1 + Tv_ClrV2) / 2;

		if ( E_GrFccUYVYF == A_Fcc )
		{
			if ( Tv_IsAlpha )
			{
				Tv_ClrY1	=	Tv_ClrY1 | 1;
			}
			else
			{
				Tv_ClrY1	=	Tv_ClrY1 & 0xFE;
			}
		}

		//make reslt
		if ( GrDrawIsFccTypeYUV(A_Fcc) )
		{
			Tv_Result	=	GrDrawMakeColorByElements( A_Fcc, Tv_ClrY1, Tv_ClrU1, Tv_ClrV1, Tv_ClrA1 );
		}
		else
		{
			Tv_Result	=	GrDrawMakeColorByElements( A_Fcc, Tv_ClrR1, Tv_ClrG1, Tv_ClrB1, Tv_ClrA1 );
		}

		return	Tv_Result;
}

//--------------------------------------------------------------------
__s32			GrDrawGetPixelBitByFcc( __u32 A_Fcc )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		switch( A_Fcc )
		{
			case E_GrFccRGB:
			case	E_GrFccRGB888:
			case	E_GrFccBGR888:
				Tv_Result		=	24;
				break;

			case	E_GrFccYUY2:
			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
			case	E_GrFccRGB565:
			case	E_GrFccARGB1555:
			case E_GrFccARGB4444:
				Tv_Result		=	16;
				break;
			case	E_GrFccRGB555:
				Tv_Result		=	15;
				break;

			case	E_GrFccARGB8888:
			case	E_GrFccABGR8888:
			case	E_GrFccUVYA:
				Tv_Result		=	32;
				break;

			case	E_GrFccFont:
				Tv_Result		=	2;
				break;

			default:
				Tv_Result		=	0;
		    break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32			GrDrawCalcPitchByFcc( __u32 A_Fcc, __u32 A_SizeX, __u32 A_PitchUnit )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_PixelBit;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check able
		Tv_PixelBit	=	GrDrawGetPixelBitByFcc( A_Fcc );
		if ( 0 != Tv_PixelBit )
		{
			Tv_Result	=	GrDrawCalcPitchByBitCnt( Tv_PixelBit, A_SizeX, A_PitchUnit );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawCalcPitchByBitCnt( __u32 A_BitCnt, __u32 A_SizeX, __u32 A_PitchUnit )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_PixelBit;
		__u32		Tv_UnitBit;
	// code --------------------
		Tv_PixelBit		=	A_BitCnt;
		//patch 15bit
		if ( 15 == Tv_PixelBit )
		{
			Tv_PixelBit	++;
		}

		//calculate pitch
		Tv_UnitBit	=	A_PitchUnit * 8;
		Tv_Result		=	(Tv_PixelBit * A_SizeX) / Tv_UnitBit;
		if ( 0 != ((Tv_PixelBit * A_SizeX) % Tv_UnitBit) )
		{
			Tv_Result	++;
		}

		Tv_Result		=	Tv_Result * A_PitchUnit;

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawGetRgbFcc( __u32 A_PixelBit, BOOL8 A_IsReverse )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch ( A_PixelBit )
		{
			case 15:
				Tv_Result		=	E_GrFccRGB555;
				break;
			case 16:
				Tv_Result		=	E_GrFccRGB565;
				break;
			case 24:
				Tv_Result		=	E_GrFccRGB888;
				if ( A_IsReverse )
				{
					Tv_Result		=	E_GrFccBGR888;
				}
				break;
			case 32:
				Tv_Result		=	E_GrFccARGB8888;
				if ( A_IsReverse )
				{
					Tv_Result		=	E_GrFccABGR8888;
				}
				break;
			default:
				Tv_Result		=	E_GrFccNone;
		    break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrDrawIsSimilarPosHz( Ptr_GrRect A_PtrRc1, Ptr_GrRect A_PtrRc2 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check horizontal range
		if ( (A_PtrRc1->left >= A_PtrRc2->left) && (A_PtrRc1->left < A_PtrRc2->right) )
		{
			Tv_Result		=	TRUE;
		}
		if ( (A_PtrRc1->right > A_PtrRc2->left) && (A_PtrRc1->right <= A_PtrRc2->right) )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrDrawIsSimilarPosVt( Ptr_GrRect A_PtrRc1, Ptr_GrRect A_PtrRc2 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		//check horizontal range
		if ( (A_PtrRc1->top >= A_PtrRc2->top) && (A_PtrRc1->top < A_PtrRc2->bottom) )
		{
			Tv_Result		=	TRUE;
		}
		if ( (A_PtrRc1->bottom > A_PtrRc2->top) && (A_PtrRc1->bottom <= A_PtrRc2->bottom) )
		{
			Tv_Result		=	TRUE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrDrawCalcRtoSize( __u32 A_SizeX, __u32 A_SizeY, __u32 A_RtoX, __u32 A_RtoY, 
													__u32* A_PtrRtSizeX, __u32* A_PtrRtSizeY )
{
	// local -------------------
		__u32		Tv_RsltX;
		__u32		Tv_RsltY;
	// code --------------------
		//init
		*A_PtrRtSizeX		=	A_SizeX;
		*A_PtrRtSizeY		=	A_SizeY;
		//calc x base position
		Tv_RsltY	=	(A_SizeX * A_RtoY) / A_RtoX;
		if ( Tv_RsltY <= A_SizeY )
		{
			*A_PtrRtSizeY		=	Tv_RsltY;
		}
		else
		{
			//y base
			Tv_RsltX		=	(A_SizeY * A_RtoX) / A_RtoY;
			*A_PtrRtSizeX	=	Tv_RsltX;
		}
}
//--------------------------------------------------------------------
Ptr_GrDrawPxStInfo	GrDrawFindPxStByFcc( __u32 A_Fcc )
{
	// local -------------------
		Ptr_GrDrawPxStInfo	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//find
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDrawPxStTblCnt;Tv_WkIdx++ )
		{
			if ( A_Fcc == V_GrDrawPxStruct[Tv_WkIdx].Fcc )
			{
				Tv_Result	=	&V_GrDrawPxStruct[Tv_WkIdx];
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawGetPxMask( Ptr_GrDrawPxStInfo A_PtrPxSt, BOOL8 A_IsEven )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_PxIdx;
					
	// code --------------------
		//init
		Tv_PxIdx		=	0;
		if ( A_IsEven )
		{
			Tv_PxIdx ++;
		}
		//calculate
		Tv_Result		=	( (__u32)((1 << A_PtrPxSt->PxSt[Tv_PxIdx][0].Size) - 1) << (__u32)A_PtrPxSt->PxSt[Tv_PxIdx][0].Pos ) |
									( (__u32)((1 << A_PtrPxSt->PxSt[Tv_PxIdx][1].Size) - 1) << (__u32)A_PtrPxSt->PxSt[Tv_PxIdx][1].Pos ) |
									( (__u32)((1 << A_PtrPxSt->PxSt[Tv_PxIdx][2].Size) - 1) << (__u32)A_PtrPxSt->PxSt[Tv_PxIdx][2].Pos );
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrDrawDibCpyRsvVt( void* A_PtrDibSr, __u32 A_PitchSr, void* A_PtrDibTg, __u32 A_PitchTg, __u32 A_ResY )
{
	// local -------------------
		__u32		Tv_HzCopyCnt;
		void*		Tv_PtrSr;
		void*		Tv_PtrTg;
		__u32*	Tv_PtrLcSr;
		__u32*	Tv_PtrLcTg;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
	// code --------------------
		Tv_HzCopyCnt	=	A_PitchSr;
		if ( Tv_HzCopyCnt > A_PitchTg )
		{
			Tv_HzCopyCnt		=	A_PitchTg;
		}
		Tv_HzCopyCnt	=	Tv_HzCopyCnt / 4;

		Tv_PtrSr		=	A_PtrDibSr;
		Tv_PtrTg		=	(void*)( (__u32)A_PtrDibTg + (A_PitchTg * (A_ResY - 1)) );

		for ( Tv_WkY = 0;Tv_WkY < A_ResY;Tv_WkY++ )
		{
			Tv_PtrLcSr		=	(__u32*)Tv_PtrSr;
			Tv_PtrLcTg		=	(__u32*)Tv_PtrTg;
			for ( Tv_WkX = 0;Tv_WkX < Tv_HzCopyCnt;Tv_WkX++ )
			{
				*Tv_PtrLcTg		=	*Tv_PtrLcSr;
				//next
				Tv_PtrLcSr ++;
				Tv_PtrLcTg ++;
			}
			//next
			Tv_PtrSr	=	(void*)( (__u32)Tv_PtrSr + A_PitchSr );
			Tv_PtrTg	=	(void*)( (__u32)Tv_PtrTg - A_PitchTg );
		}
}
//--------------------------------------------------------------------
void	GrDrawDibCpyNrm( void* A_PtrDibSr, __u32 A_PitchSr, void* A_PtrDibTg, __u32 A_PitchTg, __u32 A_ResY )
{
	// local -------------------
		__u32		Tv_HzCopyCnt;
		void*		Tv_PtrSr;
		void*		Tv_PtrTg;
		__u32*	Tv_PtrLcSr;
		__u32*	Tv_PtrLcTg;
		__u32		Tv_WkY;
		__u32		Tv_WkX;
	// code --------------------
		Tv_HzCopyCnt	=	A_PitchSr;
		if ( Tv_HzCopyCnt > A_PitchTg )
		{
			Tv_HzCopyCnt		=	A_PitchTg;
		}
		Tv_HzCopyCnt	=	Tv_HzCopyCnt / 4;

		Tv_PtrSr		=	A_PtrDibSr;
		Tv_PtrTg		=	A_PtrDibTg;

		for ( Tv_WkY = 0;Tv_WkY < A_ResY;Tv_WkY++ )
		{
			Tv_PtrLcSr		=	(__u32*)Tv_PtrSr;
			Tv_PtrLcTg		=	(__u32*)Tv_PtrTg;
			for ( Tv_WkX = 0;Tv_WkX < Tv_HzCopyCnt;Tv_WkX++ )
			{
				*Tv_PtrLcTg		=	*Tv_PtrLcSr;
				//next
				Tv_PtrLcSr ++;
				Tv_PtrLcTg ++;
			}
			//next
			Tv_PtrSr	=	(void*)( (__u32)Tv_PtrSr + A_PitchSr );
			Tv_PtrTg	=	(void*)( (__u32)Tv_PtrTg + A_PitchTg );
		}
}
//--------------------------------------------------------------------
void		GrDrawGetClrElemByPxst( __u32 A_PxVal, Ptr_GrDrawPxStInfo A_PtrPxSt, __u8* A_PtrRtEle )
{
	// local -------------------
		__u32		Tv_WkIdx;
		__u32		Tv_Ele;
		__u32		Tv_AddBitCnt;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDrawClrElemCnt;Tv_WkIdx++ )
		{
			Tv_Ele	=	(	( A_PxVal >> A_PtrPxSt->PxSt[0][Tv_WkIdx].Pos ) & 
									((1 << A_PtrPxSt->PxSt[0][Tv_WkIdx].Size) -1) );
			Tv_AddBitCnt	=	8 - A_PtrPxSt->PxSt[0][Tv_WkIdx].Size;
			Tv_Ele	=	(Tv_Ele << Tv_AddBitCnt) | (Tv_Ele >> (A_PtrPxSt->PxSt[0][Tv_WkIdx].Size - Tv_AddBitCnt));
			A_PtrRtEle[Tv_WkIdx]	=	(__u8)Tv_Ele;
		}
}
//--------------------------------------------------------------------
__u32		GrDrawMakeClrValByByPxst( Ptr_GrDrawPxStInfo A_PtrPxSt, __u8* A_PtrEle )
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result	=	0;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDrawClrElemCnt;Tv_WkIdx++ )
		{
			Tv_Result	=	Tv_Result | ( (__u32)(A_PtrEle[Tv_WkIdx] >> ( 8 - A_PtrPxSt->PxSt[0][Tv_WkIdx].Size ))
																<< A_PtrPxSt->PxSt[0][Tv_WkIdx].Pos );
		}
		if ( A_PtrPxSt->IsPacked )
		{
			Tv_Result	=	Tv_Result | ( (__u32)(A_PtrEle[0] >> ( 8 - A_PtrPxSt->PxSt[1][0].Size ))
																<< A_PtrPxSt->PxSt[1][0].Pos );
			if ( E_GrFccUYVYF == A_PtrPxSt->Fcc )
			{
				//remove alpha
				Tv_Result	=	Tv_Result & 0xFEFFFEFF;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrDrawClrCvtPara	GrDrawFindCvtParaByPxSt( Ptr_GrDrawPxStInfo A_PtrPxStSr, Ptr_GrDrawPxStInfo A_PtrPxStTg )
{
	// local -------------------
		Ptr_GrDrawClrCvtPara	Tv_Result;
	// code --------------------

		if ( (A_PtrPxStSr->IsYuv == A_PtrPxStTg->IsYuv ) )
		{
			//euqal
			Tv_Result	=	&V_GrDrawCvtPara[E_GrDrawCvtParaEqual];
		}
		else if ( A_PtrPxStTg->IsYuv )
		{
			//rgb to yuv
			Tv_Result	=	&V_GrDrawCvtPara[E_GrDrawCvtParaRgb2Yuv];
		}
		else
		{
			//yuv to rgb
			Tv_Result	=	&V_GrDrawCvtPara[E_GrDrawCvtParaYuv2Rgb];
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrDrawCvtEleByCvtPara( Ptr_GrDrawClrCvtPara A_PtrCvtPara, __u8* A_PtrEleSr, __u8* A_PtrRtEle )
{
	// local -------------------
		__u32		Tv_WkElTg;
		__s32			Tv_Ele;
		__u32		Tv_WkElSr;
	// code --------------------
		for ( Tv_WkElTg=0;Tv_WkElTg < E_GrDrawClrElemCnt;Tv_WkElTg++ )
		{
			Tv_Ele		=	0;
			for ( Tv_WkElSr=0;Tv_WkElSr < E_GrDrawClrElemCnt;Tv_WkElSr++ )
			{
				Tv_Ele	=	Tv_Ele + 
									( ( (__s32)((__u32)A_PtrEleSr[Tv_WkElSr]) +	A_PtrCvtPara->El[Tv_WkElTg].ElSr[Tv_WkElSr].Add )
										* A_PtrCvtPara->El[Tv_WkElTg].ElSr[Tv_WkElSr].Mul );
			}
			Tv_Ele	=	( Tv_Ele / A_PtrCvtPara->El[Tv_WkElTg].TtDiv ) + A_PtrCvtPara->El[Tv_WkElTg].TtAdd;
			//saturation
			if ( 0 > Tv_Ele )
			{
				Tv_Ele	=	0;
			}
			else if ( 255 < Tv_Ele )
			{
				Tv_Ele	=	255;
			}
			//write
			A_PtrRtEle[Tv_WkElTg]	=	(__u8)Tv_Ele;
		}
}
//--------------------------------------------------------------------
BOOL8		GrDrawPatchPaintRect( __s32 A_ScrSizeX, __s32 A_ScrSizeY, Ptr_GrRect A_PtrWinRect, 
														 Ptr_GrRect A_PtrReqRect, Ptr_GrRect A_PtrRtRect )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		//init
		*A_PtrRtRect	=	*A_PtrReqRect;

		//check common rect
		if ( A_PtrReqRect->left < A_PtrWinRect->left )
		{
			A_PtrRtRect->left		=	A_PtrWinRect->left;
		}

		if ( A_PtrReqRect->top < A_PtrWinRect->top )
		{
			A_PtrRtRect->top		=	A_PtrWinRect->top;
		}

		if ( A_PtrReqRect->right > A_PtrWinRect->right )
		{
			A_PtrRtRect->right	=	A_PtrWinRect->right;
		}

		if ( A_PtrReqRect->bottom > A_PtrWinRect->bottom )
		{
			A_PtrRtRect->bottom	=	A_PtrWinRect->bottom;
		}

		//check out range
		if ( 0 > A_PtrRtRect->left )
		{
			A_PtrRtRect->left		=	0;
		}
		if ( 0 > A_PtrRtRect->top )
		{
			A_PtrRtRect->top		=	0;
		}
		if ( A_ScrSizeX < A_PtrRtRect->right )
		{
			A_PtrRtRect->right	=	A_ScrSizeX;
		}
		if ( A_ScrSizeY < A_PtrRtRect->bottom )
		{
			A_PtrRtRect->bottom	=	A_ScrSizeY;
		}
		
		//check valid rect
		if ( (A_PtrRtRect->left < A_PtrRtRect->right) && (A_PtrRtRect->top < A_PtrRtRect->bottom)	)
		{
			Tv_Result		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrDrawPatchPaintBlt(Ptr_GrRect A_PtrRcUpdt,
	__s32* A_PtrX, __s32* A_PtrY, Ptr_GrRect A_PtrRcSrc)
{
	// local -------------------
		BOOL8		Tv_Result;
		__s32			Tv_X;
		__s32			Tv_Y;
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		Tv_X			=	*A_PtrX;
		Tv_Y			=	*A_PtrY;
		
		//check pos x
		if ( A_PtrRcUpdt->left > Tv_X )
		{
			//patch 
			A_PtrRcSrc->left		=	A_PtrRcSrc->left + ( A_PtrRcUpdt->left - Tv_X );
			Tv_X		=	A_PtrRcUpdt->left;
		}
		//check pos y
		if ( A_PtrRcUpdt->top > Tv_Y )
		{
			//patch 
			A_PtrRcSrc->top		=	A_PtrRcSrc->top + ( A_PtrRcUpdt->top - Tv_Y );
			Tv_Y		=	A_PtrRcUpdt->top;
		}
		//check horizontal size
		if ( A_PtrRcUpdt->right < (Tv_X + (A_PtrRcSrc->right - A_PtrRcSrc->left)) )
		{
			A_PtrRcSrc->right	=	A_PtrRcSrc->right - ( (Tv_X + (A_PtrRcSrc->right - A_PtrRcSrc->left)) - A_PtrRcUpdt->right );
		}
		//check vertical size
		if ( A_PtrRcUpdt->bottom < (Tv_Y + (A_PtrRcSrc->bottom - A_PtrRcSrc->top)) )
		{
			A_PtrRcSrc->bottom	=	A_PtrRcSrc->bottom - ( (Tv_Y + (A_PtrRcSrc->bottom - A_PtrRcSrc->top)) - A_PtrRcUpdt->bottom );
		}
		//check valid rect
		if ( (A_PtrRcSrc->left < A_PtrRcSrc->right) && (A_PtrRcSrc->top < A_PtrRcSrc->bottom)	)
		{
			*A_PtrX			=	Tv_X;
			*A_PtrY			=	Tv_Y;
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawGetColorBlack( __u32 A_Fcc )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_Fcc )
		{
			case	E_GrFccYUY2:
				Tv_Result	=	0x80108010;
				break;
			case	E_GrFccUYVY:
			case	E_GrFccUYVYF:
				Tv_Result	=	0x10801080;
				break;
			case	E_GrFccUVYA:
				Tv_Result	=	0x00108080;
				break;
			default:
				Tv_Result	=	0;
				break;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		GrDrawGetColorWhite( __u32 A_Fcc )
{
	// local -------------------
		__u32		Tv_Result;
	// code --------------------
		switch( A_Fcc )
		{
			case	E_GrFccYUY2:
				Tv_Result	=	0x80108010;
				break;
			case	E_GrFccUYVY:
				Tv_Result	=	0xEF80EF80;
				break;
			case	E_GrFccUYVYF:
				Tv_Result	=	0xEE80EE80;
				break;
			case	E_GrFccUVYA:
				Tv_Result	=	0x00EE8080;
				break;
			case	E_GrFccRGB555:
			case	E_GrFccARGB1555:
				Tv_Result	=	0x7FFF;
				break;
			case E_GrFccARGB4444:
			case	E_GrFccRGB565:
				Tv_Result	=	0xFFFF;
				break;
			default:
				Tv_Result	=	0xFFFFFF;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32			GrDrawDistanceRectByDir( Ptr_GrRect A_PtrRcBase, Ptr_GrRect A_PtrRcRef, __u32 A_Dir )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		switch( A_Dir )
		{
			case	E_GrDrawDirLeft:
				Tv_Result	=	A_PtrRcBase->left - A_PtrRcRef->right;
				break;
			case	E_GrDrawDirRight:
				Tv_Result	=	A_PtrRcRef->left - A_PtrRcBase->right;
				break;
			case	E_GrDrawDirDown:
				Tv_Result	=	A_PtrRcRef->top - A_PtrRcBase->bottom;
				break;
			default:
				//up
				Tv_Result	=	A_PtrRcBase->top - A_PtrRcRef->bottom;
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		GrDrawDistSimRectInit( Ptr_GrDrawFindDirDist A_PtrDist )
{
		A_PtrDist->Dist		=	0x7FFFFFFF;
		A_PtrDist->Ref		=	0x7FFFFFFF;
}
//--------------------------------------------------------------------
BOOL8		GrDrawDistSimRectByDir( Ptr_GrRect A_PtrRcBase, Ptr_GrRect A_PtrRcRef, __u8 A_Dir, Ptr_GrDrawFindDirDist A_PtrDist, BOOL8 A_IsWide )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		switch( A_Dir )
		{
			case	E_GrDrawDirLeft:
			case	E_GrDrawDirRight:
				//check similar vertical
				if (	A_IsWide ||
							((A_PtrRcBase->top >= A_PtrRcRef->top) && (A_PtrRcBase->top < A_PtrRcRef->bottom)) ||
							((A_PtrRcBase->bottom > A_PtrRcRef->top) && (A_PtrRcBase->bottom <= A_PtrRcRef->bottom)) ||
							((A_PtrRcBase->top <= A_PtrRcRef->top) && (A_PtrRcBase->bottom >= A_PtrRcRef->bottom)) )
				{
					A_PtrDist->Dist	=	A_PtrRcBase->left - A_PtrRcRef->right;
					if ( E_GrDrawDirRight == A_Dir )
					{
						A_PtrDist->Dist	=	A_PtrRcRef->left - A_PtrRcBase->right;
					}
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrDrawDirDown:
			case	E_GrDrawDirUp:
				//check similar horizontal
				if (	A_IsWide ||
							((A_PtrRcBase->left >= A_PtrRcRef->left) && (A_PtrRcBase->left < A_PtrRcRef->right)) ||
							((A_PtrRcBase->right > A_PtrRcRef->left) && (A_PtrRcBase->right <= A_PtrRcRef->right)) ||
							((A_PtrRcBase->left <= A_PtrRcRef->left) && (A_PtrRcBase->right >= A_PtrRcRef->right)) )
				{
					A_PtrDist->Dist	=	A_PtrRcBase->top - A_PtrRcRef->bottom;
					if ( E_GrDrawDirDown == A_Dir )
					{
						A_PtrDist->Dist	=	A_PtrRcRef->top - A_PtrRcBase->bottom;
					}
					A_PtrDist->Ref	=	A_PtrRcRef->left - A_PtrRcBase->left;
					Tv_Result	=	TRUE;
				}
				break;
			}
			
			return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrDrawIsAlphaColor( __u32 A_Fcc, __u32 A_Color )
{
	// local -------------------
		BOOL8			Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//analysis
		switch	( A_Fcc )
		{
			case	E_GrFccARGB8888:
				if(0xFF != ((Ptr_GrImgFmtARGB8888)&A_Color)->A)
				{
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrFccABGR8888:
				if(0xFF != ((Ptr_GrImgFmtABGR8888)&A_Color)->A)
				{
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrFccUYVYF:
				if ( 0 != (0x01000100 & A_Color) )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case	E_GrFccUVYA:
				if ( 0xFF != ((Ptr_GrImgFmtUVYA)&A_Color)->A )
				{
					Tv_Result	=	TRUE;
				}
				break;
			case E_GrFccARGB4444:
				if ( (0 != A_Color) && (0xF000 != ( 0xF000 & A_Color )) )
				{
					Tv_Result	=	TRUE;
				}
				break;
			default:
				Tv_Result		=	FALSE;
				break;
		}

		return		Tv_Result;
}
//--------------------------------------------------------------------
void		GrDrawColorAlpahSet( __u32 A_Fcc, void* A_PtrColor, BOOL8 A_IsSet )
{
		//analysis
		switch	( A_Fcc )
		{
			case	E_GrFccARGB8888:
				if ( A_IsSet )
				{
					((Ptr_GrImgFmtARGB8888)A_PtrColor)->A	=	0x80;
				}
				else
				{
					((Ptr_GrImgFmtARGB8888)A_PtrColor)->A	=	0xFF;
				}
				break;
			case	E_GrFccABGR8888:
				if ( A_IsSet )
				{
					((Ptr_GrImgFmtABGR8888)A_PtrColor)->A	=	0x80;
				}
				else
				{
					((Ptr_GrImgFmtABGR8888)A_PtrColor)->A	=	0xFF;
				}
				break;
			case	E_GrFccUYVYF:
				if ( 0 != *((__u32*)A_PtrColor) )
				{
					if ( A_IsSet )
					{
						*((__u32*)A_PtrColor)	=	*((__u32*)A_PtrColor) | 0x01000100;
					}
					else
					{
						*((__u32*)A_PtrColor)	=	*((__u32*)A_PtrColor) & 0xFEFFFEFF;
					}
				}
				break;
			case	E_GrFccUVYA:
				if ( A_IsSet )
				{
					((Ptr_GrImgFmtUVYA)A_PtrColor)->A	=	0x80;
				}
				else
				{
					((Ptr_GrImgFmtUVYA)A_PtrColor)->A	=	0xFF;
				}
				break;
			case E_GrFccARGB4444:
				if ( 0 != *((__u16*)A_PtrColor) )
				{
					if ( A_IsSet )
					{
						*((__u16*)(A_PtrColor))	=	( (*((__u16*)(A_PtrColor))) & 0x0FFF ) | 0x8000;
					}
					else
					{
						*((__u16*)(A_PtrColor))	=	(*((__u16*)(A_PtrColor))) | 0xF000;
					}
				}
				break;
		}
}
//--------------------------------------------------------------------


