/*
	DVR Quad control

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrQuadCtrl
#define	_EPC_GrDvrQuadCtrl

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance

#define	E_GrDvrQuadLineDfltWidth	2

//quad mode
#define E_GrDvrQuadMode1			0
#define E_GrDvrQuadMode4			1
#define E_GrDvrQuadMode6			2
#define E_GrDvrQuadMode8			3
#define E_GrDvrQuadMode9			4
#define E_GrDvrQuadMode10			5
#define E_GrDvrQuadMode13			6
#define E_GrDvrQuadMode16			7
#define E_GrDvrQuadMode25			8
#define E_GrDvrQuadMode32			9

#define E_GrDvrQuadModeCnt		(E_GrDvrQuadMode32 + 1)

#define E_GrDvrQuadModeNow		0xFF		/* now quad mode page */

//quad able mask
#define E_GrDvrQuadAbleMask1	(1 << E_GrDvrQuadMode1)
#define E_GrDvrQuadAbleMask4	(1 << E_GrDvrQuadMode4)
#define E_GrDvrQuadAbleMask6	(1 << E_GrDvrQuadMode6)
#define E_GrDvrQuadAbleMask8	(1 << E_GrDvrQuadMode8)
#define E_GrDvrQuadAbleMask9	(1 << E_GrDvrQuadMode9)
#define E_GrDvrQuadAbleMask10	(1 << E_GrDvrQuadMode10)
#define E_GrDvrQuadAbleMask13	(1 << E_GrDvrQuadMode13)
#define E_GrDvrQuadAbleMask16	(1 << E_GrDvrQuadMode16)
#define E_GrDvrQuadAbleMask25	(1 << E_GrDvrQuadMode25)
#define E_GrDvrQuadAbleMask32	(1 << E_GrDvrQuadMode32)

//view count
#define E_GrDvrQuadVwCntMd1		1
#define E_GrDvrQuadVwCntMd4		4
#define E_GrDvrQuadVwCntMd6		6
#define E_GrDvrQuadVwCntMd8		8
#define E_GrDvrQuadVwCntMd9		9
#define E_GrDvrQuadVwCntMd10	10
#define E_GrDvrQuadVwCntMd13	13
#define E_GrDvrQuadVwCntMd16	16
#define E_GrDvrQuadVwCntMd25	25
#define E_GrDvrQuadVwCntMd32	33

#define	E_GrDvrQuadVwRcTblCnt	124	/* sum of all mode view count */

/*
page max count

mode 1 = 32
mode 4 = 8
mode 6 = 6
mode 8 = 4
mode 9 = 4
mode 10 = 4
mode 13 = 3
mode 16 = 2
mode 25 = 2
mode 32 = 1

map buf count = 361
*/

#define	E_GrDvrQuadCfgMaxSize	512


//scale
#define E_GrDvrQuadSclZero		0
#define E_GrDvrQuadScl1_1			1
#define E_GrDvrQuadScl1_2			2
#define E_GrDvrQuadScl1_3			3
#define E_GrDvrQuadScl2_3			4
#define E_GrDvrQuadScl1_4			5
#define E_GrDvrQuadScl2_4			2
#define E_GrDvrQuadScl3_4			6
#define E_GrDvrQuadScl1_5			7
#define E_GrDvrQuadScl2_5			8
#define E_GrDvrQuadScl3_5			9
#define E_GrDvrQuadScl4_5			10
#define E_GrDvrQuadScl1_6			11
#define E_GrDvrQuadScl2_6			12
#define E_GrDvrQuadScl3_6			2
#define E_GrDvrQuadScl4_6			13
#define E_GrDvrQuadScl5_6			14

#define E_GrDvrQuadSclCnt			(E_GrDvrQuadScl3_4 + 1)

// quad grid line count
#define	E_GrDvrQdLnMode1Cnt				0
#define	E_GrDvrQdLnMode4Cnt				2
#define	E_GrDvrQdLnMode6Cnt				4
#define	E_GrDvrQdLnMode8Cnt				6
#define	E_GrDvrQdLnMode9Cnt				4
#define	E_GrDvrQdLnMode10Cnt			5
#define	E_GrDvrQdLnMode13Cnt			8
#define	E_GrDvrQdLnMode16Cnt			6
#define	E_GrDvrQdLnMode25Cnt			8
#define	E_GrDvrQdLnMode32Cnt			12

#define	E_GrDvrQdLnTotCnt					59


//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrDvrQuadModeCtrl
{
	__u32	PageCnt;
	__s8*	PtrMap;
	Ptr_GrRect	PtrVwRc;		// view rect
	Ptr_GrRect	PtrLnRc;		// line rect
}	*Ptr_GrDvrQuadModeCtrl;


//====================================================================
//class

class	Cls_GrDvrQuadCtrl
{
	protected:
		__s32	m_XalgMask;
		__s32	m_XalgAdd;

		__s32	m_YalgMask;
		__s32	m_YalgAdd;

		St_GrRect	m_RcScr;
		
		__u32	m_ModeMap;
		__u32	m_ChMaxCnt;

		//E_GrDvrQuadModeCnt

		__u8	m_ModeNow;				// now mode
		__u8	m_PageNow;				// now page
		__u8	m_ModeMtLast;			// mode multi last
		__u8	m_ModeMax;

		St_GrDvrQuadModeCtrl	m_ModeTbl[E_GrDvrQuadModeCnt];
		St_GrRect	m_VwRcTbl[E_GrDvrQuadVwRcTblCnt];
		St_GrRect	m_LnRcTbl[E_GrDvrQdLnTotCnt];

		__u32	m_CfgSize;
		__s8	m_CfgBuf[E_GrDvrQuadCfgMaxSize];

		__u32	m_LineWidth;

		// method
		__s32		CalcScale(__s32 A_Val, __u8 A_Scale);
		void		ModeMaxUpdate(void);
		__s8		PageGetByCh(__u8 A_Mode, __u8 A_Ch);
		void		LineTableBuild(void);

	public:
    Cls_GrDvrQuadCtrl( __u32 A_ModeMap,__u32 A_MaxChCnt,Ptr_GrRect A_PtrRcScr, __u32 A_AlignX, __u32 A_AlignY );
    virtual		~Cls_GrDvrQuadCtrl();

		// info
		__u8	ViewNowCountGet(void);
		__u32	ViewChMapGet(void);

		// configuration
		__u32	CfgSizeGet(void);
		void	CfgDataGet(void* A_PtrBuf);
		void	CfgDataSet(void* A_PtrBuf);
		void	CfgReset(void);

		// update rect
		void	ScrRectSet(Ptr_GrRect A_PtrRc);

		// quad mode
		BOOL8	QuadModeSet(__u8 A_Mode, __u8 A_Page);
		void	QuadNowModeGet(__u8* A_PtrRtMode, __u8* A_PtrRtPage);
		void	QuadModeMaxSet(void);
		void	QuadModeNextSet(void);
		void	QuadModePrevSet(void);
		void	QuadPageNextSet(void);
		void	QuadPagePrevSet(void);
		void	QuadModeExchange(__u8 A_Ch);

		__s8	ChGetByViewIdx(__u8 A_ViewIdx);
		BOOL8	ViewRectGetByViewIdx(__u8 A_ViewIdx, Ptr_GrRect A_PtrRtRect);
		__s8	ViewIdxGetByPos(__s32 A_X, __s32 A_Y);
		__s8	ViewIdxGetByCh(__u8 A_Ch);
		Ptr_GrRect	ViewTblPtrGetNowMode(__u32* A_PtrRtCnt);

		// line draw
		void	LineWidthSet(__u32 A_Width);
		__u32	LineWidthGet(void);
		Ptr_GrRect	LineTblPtrGetByMode(__u8 A_Mode,__u32* A_PtrRtCnt);
		Ptr_GrRect	LineTblPtrGetNowMode(__u32* A_PtrRtCnt);
};

//====================================================================
//global function

__u8	GrDvrQuadViewCountGetByMode(__u8 A_Mode);

//====================================================================
//out Environment conditional
#endif


