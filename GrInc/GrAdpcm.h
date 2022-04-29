//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrAdpcm
#define	_EPC_GrAdpcm

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance
#define E_GrAdpcmGr201Negative		0x80

#define E_GrAdpcmGr201Sft0				0
#define E_GrAdpcmGr201Sft1				1
#define E_GrAdpcmGr201Sft2				3
#define E_GrAdpcmGr201Sft3				4
#define E_GrAdpcmGr201Sft4				6
#define E_GrAdpcmGr201Sft5				7
#define E_GrAdpcmGr201Sft6				9
#define E_GrAdpcmGr201Sft7				11

#define E_GrAdpcmGr201Val0				0
#define E_GrAdpcmGr201Val1				(1 << E_GrAdpcmGr201Sft1)
#define E_GrAdpcmGr201Val2				(1 << E_GrAdpcmGr201Sft2)
#define E_GrAdpcmGr201Val3				(1 << E_GrAdpcmGr201Sft3)
#define E_GrAdpcmGr201Val4				(1 << E_GrAdpcmGr201Sft4)
#define E_GrAdpcmGr201Val5				(1 << E_GrAdpcmGr201Sft5)
#define E_GrAdpcmGr201Val6				(1 << E_GrAdpcmGr201Sft6)
#define E_GrAdpcmGr201Val7				(1 << E_GrAdpcmGr201Sft7)

#define E_GrAdpcmGr201EncPeriod		5
#define E_GrAdpcmGr201DecPeriod		3

#define	E_GrAdpcmGac2ValRngMin		-8192
#define	E_GrAdpcmGac2ValRngMax		8191

#define E_GrAdpcmGac2SclPtCnt			8
#define E_GrAdpcmGac2SclNtCnt			7
#define	E_GrAdpcmGac2SclTtCnt			(E_GrAdpcmGac2SclPtCnt + E_GrAdpcmGac2SclNtCnt)

#define	E_GrAdpcmGac2CodeSilence	0x0F
/*
#define	E_GrAdpcmGac2SilenceMin		-32
#define	E_GrAdpcmGac2SilenceMax		31
*/
#define	E_GrAdpcmGac2SilenceMin		-16
#define	E_GrAdpcmGac2SilenceMax		16

// u Law
#define	E_GrUlawSrMax							32635
#define	E_GrUlawBias							0x84

//#define	E_GrG723Dbg


//====================================================================
//global macro

//====================================================================
//global type

// signed 16bit 2:1 ADPCM
typedef struct St_GrAdpcmGr201Enc
{
	__u8	Seq;
}	*Ptr_GrAdpcmGr201Enc;

typedef struct St_grAdpcmGr201Dec
{
	__s16	LastDc;
	__u8	Seq;
}	*Ptr_grAdpcmGr201Dec;

// Gause voice codec V2
typedef struct St_GrAdpcmGac2Enc
{
	__u8	PrgBit;
	__u8	_rsvd0;
	__s16	Estm;
	void*	PtrOut;
	__u32	PrgByte;
}	*Ptr_GrAdpcmGac2Enc;

typedef struct St_GrAdpcmGac2Dec
{
	__u8	PrgBit;
	__u8	_rsvd0;
	__s16	Prev;		// previous value
	void*	PtrBuf;
	__u32	BufSize;
	__u32	PrgByte;
}	*Ptr_GrAdpcmGac2Dec;

typedef struct St_GrG72xStat 
{
	void*	PtrBuf;
	__u32	PrgByte;
	__u32	BufSize;

	__s32		yl;	/* Locked or steady state step size multiplier. */
	__s16 yu;	/* Unlocked or non-steady state step size multiplier. */
	__s16 dms;	/* Short term energy estimate. */
	__s16 dml;	/* Long term energy estimate. */
	__s16 ap;	/* Linear weighting coefficient of 'yl' and 'yu'. */

	__s16 a[2];	/* Coefficients of pole portion of prediction filter. */
	__s16 b[6];	/* Coefficients of zero portion of prediction filter. */
	__s16 pk[2];	/*
			 * Signs of previous two samples of a partially
			 * reconstructed signal.
			 */
	__s16 dq[6];	/*
			 * Previous 6 samples of the quantized difference
			 * signal represented in an internal floating point
			 * format.
			 */
	__s16 sr[2];	/*
			 * Previous 2 samples of the quantized difference
			 * signal represented in an internal floating point
			 * format.
			 */
	__s8 td;	/* delayed tone detect, new in 1988 version - may be voice codec not need */

	__u8	PrgBit;

}	*Ptr_GrG72xStat;

//====================================================================
//global function

// ADPCM 201

void	GrAdpcmGr201EncInit( Ptr_GrAdpcmGr201Enc A_PtrEnv );
__u32	GrAdpcmGr201Encode( Ptr_GrAdpcmGr201Enc A_PtrEnv, void* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg );

void	GrAdpcmGr201DecInit( Ptr_grAdpcmGr201Dec A_PtrEnv );
__u32	GrAdpcmGr201Decode( Ptr_grAdpcmGr201Dec A_PtrEnv, void* A_PtrSr, __u32 A_EncSize, void* A_PtrTg );

__u8	GrAdpcmGr201EncDC( __s16 A_Sample );
__s16	GrAdpcmGr201DecDC( __u8 A_Val );


// ADPCM GAV2
void	GrAdpcmGac2EncInit( Ptr_GrAdpcmGac2Enc A_PtrEnv, void* A_PtrOut, BOOL8 A_IsCont );
__u32	GrAdpcmGac2EncOne( Ptr_GrAdpcmGac2Enc A_PtrEnv, __s16 A_Val );
__u32	GrAdpcmGac2EncFinish( Ptr_GrAdpcmGac2Enc A_PtrEnv );

void	GrAdpcmGac2DecInit( Ptr_GrAdpcmGac2Dec A_PtrEnv, void* A_PtrBuf, __u32 A_BufSize, BOOL8 A_IsCont );
BOOL8	GrAdpcmGac2DecOne( Ptr_GrAdpcmGac2Dec A_PtrEnv, __s16* A_PtrOut );

__u32	GrAdpcmGac2EncBuf( __s16* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg );
__u32	GrAdpcmGac2DecBuf( void* A_PtrSr, __u32 A_Size, __s16* A_PtrTg );

// G.723_24
__s32	GrG723_24_Fmult( __s32 A_An, __s32 A_Srn );
__s32	GrG723_24_PrdtZero( Ptr_GrG72xStat A_PtrStat );
__s32	GrG723_24_PrdtPole( Ptr_GrG72xStat A_PtrStat );
__s32	GrG723_24_StepSize( Ptr_GrG72xStat A_PtrStat );
__s32	GrG723_24_Quantize( __s32	A_Val, __s32 A_StepMul  );
__s32	GrG723_24_Reconstruct( __s32 A_Sign, __s32 A_Dqln, __s32 A_StepMul );
void	GrG723_24_Update( __s32	A_Y, __s32 A_Wi, __s32 A_Fi, __s32 A_Dq, __s32 A_Sr, __s32 A_Dqsez,	Ptr_GrG72xStat A_PtrStat );

void	GrG723_24_EncInit( Ptr_GrG72xStat A_PtrEnv, void* A_PtrOut, BOOL8 A_IsCont );
__u32	GrG723_24_EncOne( Ptr_GrG72xStat A_PtrEnv, __s16 A_Val );
__u32	GrG723_24_EncFinish( Ptr_GrG72xStat A_PtrEnv );

void	GrG723_24_DecInit( Ptr_GrG72xStat A_PtrEnv, void* A_PtrBuf, __u32 A_BufSize, BOOL8 A_IsCont );
BOOL8	GrG723_24_DecOne( Ptr_GrG72xStat A_PtrEnv, __s16* A_PtrOut );

__u32	GrG723_24_EncBuf( __s16* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg );
__u32	GrG723_24_DecBuf( void* A_PtrSr, __u32 A_Size, __s16* A_PtrTg );

// G.711 u-Law
__u8	GrUlawEncOne(__s16 A_Sr);
__u32	GrUlawEncStrm(__s16* A_PtrSr, __u32 A_StrmSize, __u8* A_PtrOut);

__s16	GrUlawDecOne(__u8 A_Sr);
__u32	GrUlawDecStrm(__u8* A_PtrSr, __u32 A_StrmSize,__s16* A_PtrOut);


//====================================================================
//out Environment conditional
#endif

