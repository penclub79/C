/*
	ADPCM
		
*/

//====================================================================
// uses
#include <GrAdpcm.h>
#include <GrDumyTool.h>
#include <GrDebug.h>

#include <stdlib.h>

//====================================================================
//local type


//====================================================================
// var

__s16	V_GrAdpcmGr201ShftTbl[8]	=	
{	E_GrAdpcmGr201Sft0,E_GrAdpcmGr201Sft1,E_GrAdpcmGr201Sft2,E_GrAdpcmGr201Sft3,
	E_GrAdpcmGr201Sft4,E_GrAdpcmGr201Sft5,E_GrAdpcmGr201Sft6,E_GrAdpcmGr201Sft7};

__s16	V_GrAdpcmGac2SfctTbl[E_GrAdpcmGac2SclTtCnt]	=	
{
	0	,	128	,	256,	512,	1024,	2048,	4096,	8192,	-64	,	-256	,	-512,	-1024,	-2048,	-4096,	-8192
};


__s16	V_GrG723_24_Dqlntab[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};
__s16	V_GrG723_24_Witab[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};
__s16	V_GrG723_24_Fitab[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static __s16	_dqlntab[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};
static __s16	_witab[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};
static __s16	_fitab[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static __s16 qtab_723_24[3] = {8, 218, 331};


//====================================================================
// local function

__s32 g723_24_encoder(
		__s32 sample,
		struct St_GrG72xStat *state_ptr);

__s32 g723_24_decoder(
		__s32 code,
		struct St_GrG72xStat *state_ptr);

//====================================================================

static __s16 power2[15] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
			0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};

/*
 * quan()
 *
 * quantizes the input val against the table of size __s16 integers.
 * It returns i if table[i - 1] <= val < table[i].
 *
 * Using linear search for simple coding.
 */
static __s32
quan(
	__s32		val,
	__s16		*table,
	__s32		size)
{
	__s32		i;

	for (i = 0; i < size; i++)
		if (val < *table++)
			break;
	return (i);
}

/*
 * fmult()
 *
 * returns the integer product of the 14-bit integer "an" and
 * "floating point" representation (4-bit exponent, 6-bit mantessa) "srn".
 */
static __s32
fmult(
	__s32		an,
	__s32		srn)
{
	__s16		anmag, anexp, anmant;
	__s16		wanexp, wanmag, wanmant;
	__s16		retval;

	anmag = (an > 0) ? an : ((-an) & 0x1FFF);
	anexp = quan(anmag, power2, 15) - 6;
	anmant = (anmag == 0) ? 32 :
	    (anexp >= 0) ? anmag >> anexp : anmag << -anexp;
	wanexp = anexp + ((srn >> 6) & 0xF) - 13;

	wanmant = (anmant * (srn & 077) + 0x30) >> 4;
	retval = (wanexp >= 0) ? ((wanmant << wanexp) & 0x7FFF) :
	    (wanmant >> -wanexp);

	return (((an ^ srn) < 0) ? -retval : retval);
}

/*
 * g72x_init_state()
 *
 * This routine initializes and/or resets the St_GrG72xStat structure
 * pointed to by 'state_ptr'.
 * All the initial state values are specified in the CCITT G.721 document.
 */
void
g72x_init_state(
	struct St_GrG72xStat *state_ptr)
{
	__s32		cnta;

	state_ptr->yl = 34816;
	state_ptr->yu = 544;
	state_ptr->dms = 0;
	state_ptr->dml = 0;
	state_ptr->ap = 0;
	for (cnta = 0; cnta < 2; cnta++) {
		state_ptr->a[cnta] = 0;
		state_ptr->pk[cnta] = 0;
		state_ptr->sr[cnta] = 32;
	}
	for (cnta = 0; cnta < 6; cnta++) {
		state_ptr->b[cnta] = 0;
		state_ptr->dq[cnta] = 32;
	}
	state_ptr->td = 0;
}

/*
 * predictor_zero()
 *
 * computes the estimated signal from 6-zero predictor.
 *
 */
__s32
predictor_zero(
	struct St_GrG72xStat *state_ptr)
{
	__s32		i;
	__s32		sezi;
	//__s32		testi;

#if 0
	sezi = fmult(state_ptr->b[0] >> 2, state_ptr->dq[0]);
	for (i = 1; i < 6; i++)			/* ACCUM */
		sezi += fmult(state_ptr->b[i] >> 2, state_ptr->dq[i]);
	return (sezi);
#else
	/*
	testi = fmult(11122 >> 2, -508);
	sezi = GrG723_24_Fmult(11122 >> 2, -508);
	if ( testi != sezi )
	{
		DbgMsgPrint( "miss fmul\n" );
	}
	*/
	sezi = GrG723_24_Fmult(state_ptr->b[0] >> 2, state_ptr->dq[0]);
	for (i = 1; i < 6; i++)			/* ACCUM */
		sezi += GrG723_24_Fmult(state_ptr->b[i] >> 2, state_ptr->dq[i]);
	return (sezi);
#endif
}
/*
 * predictor_pole()
 *
 * computes the estimated signal from 2-pole predictor.
 *
 */
__s32
predictor_pole(
	struct St_GrG72xStat *state_ptr)
{
#if 1
	return (fmult(state_ptr->a[1] >> 2, state_ptr->sr[1]) +
	    fmult(state_ptr->a[0] >> 2, state_ptr->sr[0]));
#else
	return (GrG723_24_Fmult(state_ptr->a[1] >> 2, state_ptr->sr[1]) +
	    GrG723_24_Fmult(state_ptr->a[0] >> 2, state_ptr->sr[0]));
#endif
}
/*
 * step_size()
 *
 * computes the quantization step size of the adaptive quantizer.
 *
 */
__s32
step_size(
	struct St_GrG72xStat *state_ptr)
{
	__s32		y;
	__s32		dif;
	__s32		al;

	if (state_ptr->ap >= 256)
		return (state_ptr->yu);
	else {
		y = state_ptr->yl >> 6;
		dif = state_ptr->yu - y;
		al = state_ptr->ap >> 2;
		if (dif > 0)
			y += (dif * al) >> 6;
		else if (dif < 0)
			y += (dif * al + 0x3F) >> 6;
		return (y);
	}
}

/*
 * quantize()
 *
 * Given a raw sample, 'd', of the difference signal and a
 * quantization step size scale factor, 'y', this routine returns the
 * ADPCM codeword to which that sample gets quantized.  The step
 * size scale factor division operation is done in the log base 2 domain
 * as a subtraction.
 */
__s32
quantize(
	__s32		d,	/* Raw difference signal sample */
	__s32		y,	/* Step size multiplier */
	__s16		*table,	/* quantization table */
	__s32		size)	/* table size of __s16 integers */
{
	__s16		dqm;	/* Magnitude of 'd' */
	__s16		exp;	/* Integer part of base 2 log of 'd' */
	__s16		mant;	/* Fractional part of base 2 log */
	__s16		dl;	/* Log of magnitude of 'd' */
	__s16		dln;	/* Step size scale factor normalized log */
	__s32		i;

	/*
	 * LOG
	 *
	 * Compute base 2 log of 'd', and store in 'dl'.
	 */
	dqm = abs(d);
	exp = quan(dqm >> 1, power2, 15);
	mant = ((dqm << 7) >> exp) & 0x7F;	/* Fractional portion. */
	dl = (exp << 7) + mant;

	/*
	 * SUBTB
	 *
	 * "Divide" by step size multiplier.
	 */
	dln = dl - (y >> 2);

	/*
	 * QUAN
	 *
	 * Obtain codword i for 'd'.
	 */
	i = quan(dln, table, size);
	if (d < 0)			/* take 1's complement of i */
		return ((size << 1) + 1 - i);
	else if (i == 0)		/* take 1's complement of 0 */
		return ((size << 1) + 1); /* new in 1988 */
	else
		return (i);
}
/*
 * reconstruct()
 *
 * Returns reconstructed difference signal 'dq' obtained from
 * codeword 'i' and quantization step size scale factor 'y'.
 * Multiplication is performed in log base 2 domain as addition.
 */
__s32
reconstruct(
	__s32		sign,	/* 0 for non-negative value */
	__s32		dqln,	/* G.72x codeword */
	__s32		y)	/* Step size multiplier */
{
	__s16		dql;	/* Log of 'dq' magnitude */
	__s16		dex;	/* Integer part of log */
	__s16		dqt;
	__s16		dq;	/* Reconstructed difference signal sample */

	dql = dqln + (y >> 2);	/* ADDA */

	if (dql < 0) {
		return ((sign) ? -0x8000 : 0);
	} else {		/* ANTILOG */
		dex = (dql >> 7) & 15;
		dqt = 128 + (dql & 127);
		dq = (dqt << 7) >> (14 - dex);
		return ((sign) ? (dq - 0x8000) : dq);
	}
}


/*
 * update()
 *
 * updates the state variables for each output code
 */
void
update(
	__s32		code_size,	/* distinguish 723_40 with others */
	__s32		y,		/* quantizer step size */
	__s32		wi,		/* scale factor multiplier */
	__s32		fi,		/* for long/__s16 term energies */
	__s32		dq,		/* quantized prediction difference */
	__s32		sr,		/* reconstructed signal */
	__s32		dqsez,		/* difference from 2-pole predictor */
	struct St_GrG72xStat *state_ptr)	/* coder state pointer */
{
	__s32		cnt;
	__s16		mag, exp, mant;	/* Adaptive predictor, FLOAT A */
	__s16		a2p;		/* LIMC */
	__s16		a1ul;		/* UPA1 */
	__s16		ua2, pks1;	/* UPA2 */
	__s16		uga2a, fa1;
	__s16		uga2b;
	__s8		tr;		/* tone/transition detector */
	__s16		ylint, thr2, dqthr;
	__s16  		ylfrac, thr1;
	__s16		pk0;

	pk0 = (dqsez < 0) ? 1 : 0;	/* needed in updating predictor poles */

	mag = dq & 0x7FFF;		/* prediction difference magnitude */
	/* TRANS */
	ylint = state_ptr->yl >> 15;	/* exponent part of yl */
	ylfrac = (state_ptr->yl >> 10) & 0x1F;	/* fractional part of yl */
	thr1 = (32 + ylfrac) << ylint;		/* threshold */
	thr2 = (ylint > 9) ? 31 << 10 : thr1;	/* limit thr2 to 31 << 10 */
	dqthr = (thr2 + (thr2 >> 1)) >> 1;	/* dqthr = 0.75 * thr2 */
	if (state_ptr->td == 0)		/* signal supposed voice */
		tr = 0;
	else if (mag <= dqthr)		/* supposed data, but small mag */
		tr = 0;			/* treated as voice */
	else				/* signal is data (modem) */
		tr = 1;

	/*
	 * Quantizer scale factor adaptation.
	 */

	/* FUNCTW & FILTD & DELAY */
	/* update non-steady state step size multiplier */
	state_ptr->yu = y + ((wi - y) >> 5);

	/* LIMB */
	if (state_ptr->yu < 544)	/* 544 <= yu <= 5120 */
		state_ptr->yu = 544;
	else if (state_ptr->yu > 5120)
		state_ptr->yu = 5120;

	/* FILTE & DELAY */
	/* update steady state step size multiplier */
	state_ptr->yl += state_ptr->yu + ((-state_ptr->yl) >> 6);

	/*
	 * Adaptive predictor coefficients.
	 */
	if (tr == 1) {			/* reset a's and b's for modem signal */
		state_ptr->a[0] = 0;
		state_ptr->a[1] = 0;
		state_ptr->b[0] = 0;
		state_ptr->b[1] = 0;
		state_ptr->b[2] = 0;
		state_ptr->b[3] = 0;
		state_ptr->b[4] = 0;
		state_ptr->b[5] = 0;
	} else {			/* update a's and b's */
		pks1 = pk0 ^ state_ptr->pk[0];		/* UPA2 */

		/* update predictor pole a[1] */
		a2p = state_ptr->a[1] - (state_ptr->a[1] >> 7);
		if (dqsez != 0) {
			fa1 = (pks1) ? state_ptr->a[0] : -state_ptr->a[0];
			if (fa1 < -8191)	/* a2p = function of fa1 */
				a2p -= 0x100;
			else if (fa1 > 8191)
				a2p += 0xFF;
			else
				a2p += fa1 >> 5;

			if (pk0 ^ state_ptr->pk[1])
				/* LIMC */
				if (a2p <= -12160)
					a2p = -12288;
				else if (a2p >= 12416)
					a2p = 12288;
				else
					a2p -= 0x80;
			else if (a2p <= -12416)
				a2p = -12288;
			else if (a2p >= 12160)
				a2p = 12288;
			else
				a2p += 0x80;
		}

		/* TRIGB & DELAY */
		state_ptr->a[1] = a2p;

		/* UPA1 */
		/* update predictor pole a[0] */
		state_ptr->a[0] -= state_ptr->a[0] >> 8;
		if (dqsez != 0)
			if (pks1 == 0)
				state_ptr->a[0] += 192;
			else
				state_ptr->a[0] -= 192;

		/* LIMD */
		a1ul = 15360 - a2p;
		if (state_ptr->a[0] < -a1ul)
			state_ptr->a[0] = -a1ul;
		else if (state_ptr->a[0] > a1ul)
			state_ptr->a[0] = a1ul;

		/* UPB : update predictor zeros b[6] */
		for (cnt = 0; cnt < 6; cnt++) {
			if (code_size == 5)		/* for 40Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 9;
			else			/* for G.721 and 24Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 8;
			if (dq & 0x7FFF) {			/* XOR */
				if ((dq ^ state_ptr->dq[cnt]) >= 0)
					state_ptr->b[cnt] += 128;
				else
					state_ptr->b[cnt] -= 128;
			}
		}
	}

	for (cnt = 5; cnt > 0; cnt--)
		state_ptr->dq[cnt] = state_ptr->dq[cnt-1];
	/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
	if (mag == 0) {
		state_ptr->dq[0] = (dq >= 0) ? 0x20 : 0xFC20;
	} else {
		exp = quan(mag, power2, 15);
		state_ptr->dq[0] = (dq >= 0) ?
		    (exp << 6) + ((mag << 6) >> exp) :
		    (exp << 6) + ((mag << 6) >> exp) - 0x400;
	}

	state_ptr->sr[1] = state_ptr->sr[0];
	/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
	if (sr == 0) {
		state_ptr->sr[0] = 0x20;
	} else if (sr > 0) {
		exp = quan(sr, power2, 15);
		state_ptr->sr[0] = (exp << 6) + ((sr << 6) >> exp);
	} else if (sr > -32768) {
		mag = -sr;
		exp = quan(mag, power2, 15);
		state_ptr->sr[0] =  (exp << 6) + ((mag << 6) >> exp) - 0x400;
	} else
		state_ptr->sr[0] = 0xFC20;

	/* DELAY A */
	state_ptr->pk[1] = state_ptr->pk[0];
	state_ptr->pk[0] = pk0;

	/* TONE */
	if (tr == 1)		/* this sample has been treated as data */
		state_ptr->td = 0;	/* next one will be treated as voice */
	else if (a2p < -11776)	/* small sample-to-sample correlation */
		state_ptr->td = 1;	/* signal may be data */
	else				/* signal is voice */
		state_ptr->td = 0;

	/*
	 * Adaptation speed control.
	 */
	state_ptr->dms += (fi - state_ptr->dms) >> 5;		/* FILTA */
	state_ptr->dml += (((fi << 2) - state_ptr->dml) >> 7);	/* FILTB */

	if (tr == 1)
		state_ptr->ap = 256;
	else if (y < 1536)					/* SUBTC */
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if (state_ptr->td == 1)
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if (abs((state_ptr->dms << 2) - state_ptr->dml) >=
	    (state_ptr->dml >> 3))
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else
		state_ptr->ap += (-state_ptr->ap) >> 4;
}



//--------------------------------------------------------------------
void	GrAdpcmGr201EncInit( Ptr_GrAdpcmGr201Enc A_PtrEnv )
{
		GrDumyZeroMem( A_PtrEnv, sizeof(St_GrAdpcmGr201Enc) );
}
//--------------------------------------------------------------------
__u32	GrAdpcmGr201Encode( Ptr_GrAdpcmGr201Enc A_PtrEnv, void* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg )
{
	// local -------------------
		__u32	Tv_Result;
		__s16*	Tv_PtrSr;
		__u8*		Tv_PtrTg;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result	=	0;
		Tv_Remain	=	A_SmpCnt;
		Tv_PtrSr	=	(__s16*)A_PtrSr;
		Tv_PtrTg	=	(__u8*)A_PtrTg;
		while ( 0 != Tv_Remain )
		{
			*Tv_PtrTg	=	GrAdpcmGr201EncDC( *Tv_PtrSr );
			*Tv_PtrTg ++;
			Tv_Result ++;
			Tv_PtrSr ++;
			Tv_Remain --;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrAdpcmGr201DecInit( Ptr_grAdpcmGr201Dec A_PtrEnv )
{
		GrDumyZeroMem( A_PtrEnv, sizeof(St_grAdpcmGr201Dec) );
}
//--------------------------------------------------------------------
__u32	GrAdpcmGr201Decode( Ptr_grAdpcmGr201Dec A_PtrEnv, void* A_PtrSr, __u32 A_EncSize, void* A_PtrTg )
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_Remain;
		__u8*	Tv_PtrSr;
		__s16*	Tv_PtrTg;
		__s16	Tv_Dc;
	// code --------------------
		Tv_Result	=	0;

		// init
		Tv_Remain	=	A_EncSize;
		Tv_PtrSr	=	(__u8*)A_PtrSr;
		Tv_PtrTg	=	(__s16*)A_PtrTg;
		while ( 0 != Tv_Remain )
		{
			Tv_Dc	=	GrAdpcmGr201DecDC( *Tv_PtrSr );
			// update DC
			*Tv_PtrTg	=	Tv_Dc;
			Tv_PtrTg ++;
			Tv_Result ++;

			// next
			Tv_PtrSr ++;
			Tv_Remain --;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	GrAdpcmGr201EncDC( __s16 A_Sample )
{
	// local -------------------
		__u8	Tv_Result;
		__s16	Tv_Data;
	// code --------------------
		// init
		Tv_Result	=	0;
		Tv_Data		=	A_Sample;

		// check sign
		if ( 0 > Tv_Data )
		{
			if ( -32768 == Tv_Data )
			{
				Tv_Data	=	-32767;
			}
			Tv_Data	=	-Tv_Data;
			Tv_Result	=	E_GrAdpcmGr201Negative;
		}

		if ( E_GrAdpcmGr201Val7 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (7 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft7) );
		}
		else if ( E_GrAdpcmGr201Val6 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (6 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft6) );
		}
		else if ( E_GrAdpcmGr201Val5 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (5 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft5) );
		}
		else if ( E_GrAdpcmGr201Val4 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (4 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft4) );
		}
		else if ( E_GrAdpcmGr201Val3 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (3 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft3) );
		}
		else if ( E_GrAdpcmGr201Val2 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (2 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft2) );
		}
		else if ( E_GrAdpcmGr201Val1 <= Tv_Data )
		{
			Tv_Result	=	Tv_Result | (1 << 4) | (__u8)( 0x0F & (Tv_Data >> E_GrAdpcmGr201Sft1) );
		}
		else
		{
			Tv_Result	=	Tv_Result | (__u8)( 0x0F & Tv_Data );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s16	GrAdpcmGr201DecDC( __u8 A_Val )
{
	// local -------------------
		__s16	Tv_Result;
		__u8		Tv_Shift;
	// code --------------------
		Tv_Shift	=	(A_Val >> 4) & 0x07;
		Tv_Result	=	(__s16)(A_Val & 0x0F);
		Tv_Result	=	Tv_Result << V_GrAdpcmGr201ShftTbl[Tv_Shift];
		if ( 0 != ( E_GrAdpcmGr201Negative & A_Val ) )
		{
			Tv_Result	=	-Tv_Result;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrAdpcmGac2EncInit( Ptr_GrAdpcmGac2Enc A_PtrEnv, void* A_PtrOut, BOOL8 A_IsCont )
{
		if ( A_IsCont )
		{
			A_PtrEnv->PrgByte	=	0;
			A_PtrEnv->PrgBit	=	0;
		}
		else
		{
			GrDumyZeroMem( A_PtrEnv, sizeof(St_GrAdpcmGac2Enc) );
		}
		A_PtrEnv->PtrOut	=	A_PtrOut;
}
//--------------------------------------------------------------------
__u32	GrAdpcmGac2EncOne( Ptr_GrAdpcmGac2Enc A_PtrEnv, __s16 A_Val )
{
	// local -------------------
		__u32	Tv_Result;
		__s16	Tv_Smp;
		__s16	Tv_Dif;
		__s16	Tv_Min;
		__s16	Tv_Max;
		__u8	Tv_WkIdx;
		__u8	Tv_Val;
		__u8*	Tv_PtrOut;
	// code --------------------
		// scale down
		Tv_Smp	=	A_Val >> 2;

		// check silence
		if ( (E_GrAdpcmGac2SilenceMin <= Tv_Smp) && (E_GrAdpcmGac2SilenceMax >= Tv_Smp) )
		{
			// silence
			Tv_Val	=	E_GrAdpcmGac2CodeSilence;
		}
		else
		{
			// get diff
			Tv_Dif	=	Tv_Smp - A_PtrEnv->Estm;
			// check sign
			if ( 0 <= Tv_Dif )
			{
				// positive
				Tv_Val	=	0;
				if ( Tv_Dif >= (V_GrAdpcmGac2SfctTbl[1] >> 1) )
				{
					Tv_Val	=	E_GrAdpcmGac2SclPtCnt - 1;
					for (Tv_WkIdx=1;Tv_WkIdx < (E_GrAdpcmGac2SclPtCnt - 1);Tv_WkIdx++)
					{
						// round scale factor
						Tv_Min	=	((V_GrAdpcmGac2SfctTbl[Tv_WkIdx-1] + V_GrAdpcmGac2SfctTbl[Tv_WkIdx]) >> 1);
						Tv_Max	=	((V_GrAdpcmGac2SfctTbl[Tv_WkIdx+1] + V_GrAdpcmGac2SfctTbl[Tv_WkIdx]) >> 1);
						if ( (Tv_Dif >=  Tv_Min) && (Tv_Dif < Tv_Max) )
						{
							Tv_Val	=	Tv_WkIdx;
							break;
						}
					}
				}
			}
			else
			{
				// negative
				Tv_Val	=	E_GrAdpcmGac2SclPtCnt;
				if ( Tv_Dif <= (V_GrAdpcmGac2SfctTbl[E_GrAdpcmGac2SclPtCnt +1] >> 1 ) )
				{
					Tv_Val	=	E_GrAdpcmGac2SclTtCnt - 1;
					for (Tv_WkIdx = E_GrAdpcmGac2SclPtCnt + 1;Tv_WkIdx < E_GrAdpcmGac2SclTtCnt;Tv_WkIdx++)
					{
						Tv_Min	=	((V_GrAdpcmGac2SfctTbl[Tv_WkIdx+1] + V_GrAdpcmGac2SfctTbl[Tv_WkIdx]) >> 1);
						Tv_Max	=	((V_GrAdpcmGac2SfctTbl[Tv_WkIdx-1] + V_GrAdpcmGac2SfctTbl[Tv_WkIdx]) >> 1);
						if ( (Tv_Dif >  Tv_Min) && (Tv_Dif <= Tv_Max) )
						{
							Tv_Val	=	Tv_WkIdx;
							break;
						}
					}
				}
			}
		}

		// feedback
		if ( E_GrAdpcmGac2CodeSilence == Tv_Val )
		{
			A_PtrEnv->Estm	=	0;
		}
		else
		{
			A_PtrEnv->Estm	=	A_PtrEnv->Estm + V_GrAdpcmGac2SfctTbl[Tv_Val];
			if ( E_GrAdpcmGac2ValRngMin > A_PtrEnv->Estm )
			{
				A_PtrEnv->Estm	=	E_GrAdpcmGac2ValRngMin;
			}
			else if ( E_GrAdpcmGac2ValRngMax < A_PtrEnv->Estm )
			{
				A_PtrEnv->Estm	=	E_GrAdpcmGac2ValRngMax;
			}
		}

		// update buffer
		Tv_PtrOut	=	(__u8*)( (__u32)A_PtrEnv->PtrOut + A_PtrEnv->PrgByte );
		if ( 0 == A_PtrEnv->PrgBit )
		{
			*Tv_PtrOut	=	Tv_Val;
		}
		else
		{
			Tv_Val	=	Tv_Val << 4;
			*Tv_PtrOut	=	*Tv_PtrOut | Tv_Val;
		}

		// go next
		A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit ^ 4;
		if ( 0 == A_PtrEnv->PrgBit )
		{
			A_PtrEnv->PrgByte	=	A_PtrEnv->PrgByte + 1;
		}

		// result size
		Tv_Result	=	A_PtrEnv->PrgByte;
		if ( 0 != A_PtrEnv->PrgBit )
		{
			Tv_Result ++;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrAdpcmGac2EncFinish( Ptr_GrAdpcmGac2Enc A_PtrEnv )
{
	// local -------------------
	// code --------------------
		if ( 0 != A_PtrEnv->PrgBit )
		{
			GrAdpcmGac2EncOne( A_PtrEnv, 0 );	// add zero data
		}
		return	A_PtrEnv->PrgByte;
}
//--------------------------------------------------------------------
void	GrAdpcmGac2DecInit( Ptr_GrAdpcmGac2Dec A_PtrEnv, void* A_PtrBuf, __u32 A_BufSize, BOOL8 A_IsCont )
{
		if ( A_IsCont )
		{
			A_PtrEnv->PrgByte	=	0;
			A_PtrEnv->PrgBit	=	0;
		}
		else
		{
			GrDumyZeroMem( A_PtrEnv, sizeof(St_GrAdpcmGac2Dec) );
		}
		A_PtrEnv->PtrBuf	=	A_PtrBuf;
		A_PtrEnv->BufSize	=	A_BufSize;
}
//--------------------------------------------------------------------
BOOL8	GrAdpcmGac2DecOne( Ptr_GrAdpcmGac2Dec A_PtrEnv, __s16* A_PtrOut )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8*	Tv_PtrVal;
		__u8	Tv_Idx;
		__s16	Tv_Vact;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( A_PtrEnv->PrgByte < A_PtrEnv->BufSize )
		{
			// get index
			Tv_PtrVal	=	(__u8*)( (__u32)A_PtrEnv->PtrBuf + A_PtrEnv->PrgByte );
			Tv_Idx		=	*Tv_PtrVal;
			Tv_Idx		=	Tv_Idx >> A_PtrEnv->PrgBit;
			Tv_Idx		=	Tv_Idx & 0x0F;

			if ( E_GrAdpcmGac2CodeSilence == Tv_Idx )
			{
				A_PtrEnv->Prev	=	0;
			}
			else
			{
				Tv_Vact		=	V_GrAdpcmGac2SfctTbl[Tv_Idx];
				A_PtrEnv->Prev	=	A_PtrEnv->Prev + Tv_Vact;
				// patch
				if ( E_GrAdpcmGac2ValRngMin > A_PtrEnv->Prev )
				{
					A_PtrEnv->Prev	=	E_GrAdpcmGac2ValRngMin;
				}
				else if ( E_GrAdpcmGac2ValRngMax < A_PtrEnv->Prev )
				{
					A_PtrEnv->Prev	=	E_GrAdpcmGac2ValRngMax;
				}
			}
			// update value
			*A_PtrOut	=	(A_PtrEnv->Prev) << 2;
			Tv_Result	=	TRUE;
			// next
			A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit ^ 4;
			if ( 0 == A_PtrEnv->PrgBit )
			{
				A_PtrEnv->PrgByte	=	A_PtrEnv->PrgByte + 1;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrAdpcmGac2EncBuf( __s16* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg )
{
	// local -------------------
		St_GrAdpcmGac2Enc	Tv_Env;
		__u32	Tv_WkIdx;
	// code --------------------
		GrAdpcmGac2EncInit( &Tv_Env, A_PtrTg, FALSE );
		for ( Tv_WkIdx=0;Tv_WkIdx < A_SmpCnt;Tv_WkIdx++ )
		{
			GrAdpcmGac2EncOne( &Tv_Env, A_PtrSr[Tv_WkIdx] );
		}

		return	GrAdpcmGac2EncFinish( &Tv_Env );
}
//--------------------------------------------------------------------
__u32	GrAdpcmGac2DecBuf( void* A_PtrSr, __u32 A_Size, __s16* A_PtrTg )
{
	// local -------------------
		St_GrAdpcmGac2Dec	Tv_Env;
		__u32	Tv_WkIdx;
	// code --------------------
		GrAdpcmGac2DecInit( &Tv_Env, A_PtrSr, A_Size, FALSE );
		Tv_WkIdx	=	0;
		while ( GrAdpcmGac2DecOne( &Tv_Env, &A_PtrTg[Tv_WkIdx]) )
		{
			// next
			Tv_WkIdx ++;	
		}

		return	Tv_WkIdx;
}
//--------------------------------------------------------------------
__s32	GrG723_24_Fmult( __s32 A_An, __s32 A_Srn )
{
	// local -------------------
		__s16	Tv_Result;
		__s16	Tv_AnMag;
		__s16	Tv_AnExp;
		__s16	Tv_Quan;
		__s16	Tv_AnMant;
		__s16	Tv_WanExp;
		__s16	Tv_WanMant;
	// code --------------------
		// absolute AN
		Tv_AnMag	=	A_An;
		if ( 0 > A_An )
		{
			Tv_AnMag	=	(-A_An) & 0x1FFF;
		}
		// 
		Tv_Quan		=	1;
		for (Tv_AnExp = 0; Tv_AnExp < 15; Tv_AnExp++)
		{
			if ( Tv_Quan > Tv_AnMag )
			{
				break;
			}
			// next
			Tv_Quan	=	Tv_Quan << 1;
		}
		Tv_AnExp	=	Tv_AnExp - 6;

		Tv_AnMant	=	32;
		if ( 0 != Tv_AnMag )
		{
			Tv_AnMant	=	Tv_AnMag >> Tv_AnExp;
			if ( 0 > Tv_AnExp )
			{
				Tv_AnMant	=	Tv_AnMag << (-Tv_AnExp);
			}
		}

		Tv_WanExp		=	Tv_AnExp + ((A_Srn >> 6) & 0x0F) - 13;

		Tv_WanMant	=	(Tv_AnMant * (A_Srn & 077) + 0x30) >> 4;

		Tv_Result	=	((Tv_WanMant << Tv_WanExp) & 0x7FFF);
		if ( 0 > Tv_WanExp )
		{
			Tv_Result	=	Tv_WanMant >> (-Tv_WanExp);
		}

		if ( 0 > (A_An ^ A_Srn) )
		{
			Tv_Result	=	-Tv_Result;
		}

		return	(__s32)Tv_Result;
}
//--------------------------------------------------------------------
__s32	GrG723_24_PrdtZero( Ptr_GrG72xStat A_PtrStat )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_WkIdx;
	// code --------------------
		Tv_Result	=	GrG723_24_Fmult( A_PtrStat->b[0] >> 2 , A_PtrStat->dq[0]);
		for (Tv_WkIdx = 1; Tv_WkIdx < 6; Tv_WkIdx++)			/* ACCUM */
		{
			Tv_Result	=	Tv_Result +  GrG723_24_Fmult( A_PtrStat->b[Tv_WkIdx] >> 2, A_PtrStat->dq[Tv_WkIdx] );

		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	GrG723_24_PrdtPole( Ptr_GrG72xStat A_PtrStat )
{
	// local -------------------
	// code --------------------
		return 	( GrG723_24_Fmult( A_PtrStat->a[1] >> 2, A_PtrStat->sr[1] ) + 
	    GrG723_24_Fmult(A_PtrStat->a[0] >> 2, A_PtrStat->sr[0] ) );
}
//--------------------------------------------------------------------
__s32	GrG723_24_StepSize( Ptr_GrG72xStat A_PtrStat )
{
	// local -------------------
		__s32		Tv_Result;
		__s32		Tv_Dif;
		__s32		Tv_Al;
	// code --------------------
		Tv_Result	=	A_PtrStat->yu;

		if ( 256 > A_PtrStat->ap )
		{
			Tv_Result = A_PtrStat->yl >> 6;
			Tv_Dif		= A_PtrStat->yu - Tv_Result;
			Tv_Al = A_PtrStat->ap >> 2;
			if (Tv_Dif > 0)
			{
				Tv_Result	=	Tv_Result + ((Tv_Dif * Tv_Al) >> 6);
			}
			else if (Tv_Dif < 0)
			{
				Tv_Result	=	Tv_Result + ((Tv_Dif * Tv_Al + 0x3F) >> 6);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	GrG723_24_Quantize( __s32	A_Val, __s32 A_StepMul )
{
	// local -------------------
		__s32			Tv_Result;
		__s16		Tv_Dqm;			/* Magnitude of A_Val */
		__s16		Tv_Quan;
		__s16		Tv_Exp;			/* Integer part of base 2 log of A_Val */
		__s16		Tv_Mant;		/* Fractional part of base 2 log */
		__s16		Tv_Dl;			/* Log of magnitude of A_Val */
		__s16		Tv_Dln;			/* Step size scale factor normalized log */
	// code --------------------
		/*	LOG Compute base 2 log of A_Val, and store in Tv_Dl */
		Tv_Dqm	=	A_Val;
		if ( 0 > Tv_Dqm )
		{
			Tv_Dqm	=	-Tv_Dqm;
		}

		Tv_Quan		=	1;
		for (Tv_Exp = 0; Tv_Exp < 15; Tv_Exp++)
		{
			if ( Tv_Quan > (Tv_Dqm >> 1) )
			{
				break;
			}
			// next
			Tv_Quan	=	Tv_Quan << 1;
		}
		
		Tv_Mant = ((Tv_Dqm << 7) >> Tv_Exp) & 0x7F;	/* Fractional portion. */
		Tv_Dl		= (Tv_Exp << 7) + Tv_Mant;

		/* SUBTB   Divide by step size multiplier	 */
		Tv_Dln	= Tv_Dl - (A_StepMul >> 2);

		/* QUAN Obtain codword result for A_Val */
		Tv_Result	=	0;
		if ( 8 <= Tv_Dln )
		{
			Tv_Result	++;
			if ( 218 <= Tv_Dln )
			{
				Tv_Result ++;
				if ( 331 <= Tv_Dln )
				{
					Tv_Result ++;
				}
			}
		}

		// make 3 bit code
		if ( 0 > A_Val )
		{
			// negative code
			Tv_Result	=	7 - Tv_Result;
		}
		else if ( 0 == Tv_Result )
		{
			// same 
			Tv_Result	=	7;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32	GrG723_24_Reconstruct( __s32 A_Sign, __s32 A_Dqln, __s32 A_StepMul )
{
	// local -------------------
		__s16		Tv_Result;
		__s16		Tv_Dql;		/* Log of 'dq' magnitude */
		__s16		Tv_Dex;		/* Integer part of log */
		__s16		Tv_Dqt;

	// code --------------------
				
		Tv_Dql	= A_Dqln + (A_StepMul >> 2);	/* ADDA */

		if ( 0 >  Tv_Dql) 
		{
			Tv_Result	=	0;
			if ( 0 != A_Sign )
			{
				Tv_Result	=	-0x8000;
			}
		} 
		else 
		{		
			/* ANTILOG */
			Tv_Dex		= (Tv_Dql >> 7) & 15;
			Tv_Dqt		= 128 + (Tv_Dql & 127);
			Tv_Result	= (Tv_Dqt << 7) >> (14 - Tv_Dex);

			if ( 0 != A_Sign )
			{
				Tv_Result	=	Tv_Result - 0x8000;
			}
		}

		return	(__s32)Tv_Result;
}
//--------------------------------------------------------------------
void	GrG723_24_Update( __s32	A_Y, __s32 A_Wi, __s32 A_Fi, __s32 A_Dq, __s32 A_Sr, __s32 A_Dqsez,	Ptr_GrG72xStat A_PtrStat )
{
	// local -------------------
		__s32			Tv_Cnt;
		__s16		Tv_Mag;
		__s16		Tv_Exp;
		__s16		Tv_A2p;		/* LIMC */
		__s16		Tv_A1ul;		/* UPA1 */
		__s16		Tv_Pks1;	/* UPA2 */
		__s16		Tv_Fa1;
		__s8		Tv_Tr;		/* tone/transition detector */
		__s16		Tv_Ylint;
		__s16		Tv_Thr2;
		__s16		Tv_Dqthr;
		__s16  	Tv_Ylfrac;
		__s16		Tv_Thr1;
		__s16		Tv_Pk0;
		__s16		Tv_Quan;
		__s16		Tv_Tmp;
	// code --------------------

		Tv_Pk0	=	0;
		if ( 0 > A_Dqsez )
		{
			Tv_Pk0	=	1;
		}
		Tv_Pk0 = (A_Dqsez < 0) ? 1 : 0;	/* needed in updating predictor poles */

		Tv_Mag = A_Dq & 0x7FFF;					/* prediction difference magnitude */

		/* TRANS */
		Tv_Ylint	= A_PtrStat->yl >> 15;						/* exponent part of yl */
		Tv_Ylfrac = (A_PtrStat->yl >> 10) & 0x1F;		/* fractional part of yl */
		Tv_Thr1		= (32 + Tv_Ylfrac) << Tv_Ylint;		/* threshold */
		Tv_Thr2		=	Tv_Thr1;
		if ( 9 < Tv_Ylint )	/* limit thr2 to 31 << 10 */
		{
			Tv_Thr2	=	31 << 10;
		}

		Tv_Dqthr	= (Tv_Thr2 + (Tv_Thr2 >> 1)) >> 1;	/* dqthr = 0.75 * thr2 */
		if (0 == A_PtrStat->td)		/* signal supposed voice */
		{
			Tv_Tr	= 0;
		}
		else if ( Tv_Mag <= Tv_Dqthr)		/* supposed data, but small mag */
		{
			Tv_Tr	= 0;			/* treated as voice */
		}
		else							/* signal is data (modem) */
		{
			Tv_Tr	= 1;
		}

		/* Quantizer scale factor adaptation. */
		/* FUNCTW & FILTD & DELAY */
		/* update non-steady state step size multiplier */
		A_PtrStat->yu	= A_Y + (( A_Wi - A_Y) >> 5);

		/* LIMB */
		if (A_PtrStat->yu < 544)	/* 544 <= yu <= 5120 */
		{
			A_PtrStat->yu = 544;
		}
		else if (A_PtrStat->yu > 5120)
		{
			A_PtrStat->yu = 5120;
		}

		/* FILTE & DELAY */
		/* update steady state step size multiplier */
		A_PtrStat->yl	=	A_PtrStat->yl + A_PtrStat->yu + ((-A_PtrStat->yl) >> 6);

		/* Adaptive predictor coefficients. */
		if ( 1 == Tv_Tr) 
		{		
			/* reset a's and b's for modem signal - may not need */
			A_PtrStat->a[0] = 0;
			A_PtrStat->a[1] = 0;
			A_PtrStat->b[0] = 0;
			A_PtrStat->b[1] = 0;
			A_PtrStat->b[2] = 0;
			A_PtrStat->b[3] = 0;
			A_PtrStat->b[4] = 0;
			A_PtrStat->b[5] = 0;
		} 
		else 
		{			
			/* update a's and b's */
			Tv_Pks1 = Tv_Pk0 ^ A_PtrStat->pk[0];		/* UPA2 */

			/* update predictor pole a[1] */
			Tv_A2p	= A_PtrStat->a[1] - (A_PtrStat->a[1] >> 7);
			if ( 0 != A_Dqsez ) 
			{
				Tv_Fa1	=	A_PtrStat->a[0];
				if ( 0 == Tv_Pks1 )
				{
					Tv_Fa1	=	-A_PtrStat->a[0];
				}
				
				if ( Tv_Fa1 < -8191)	/* a2p = function of fa1 */
				{
					Tv_A2p	=	Tv_A2p - 0x100;
				}
				else if ( Tv_Fa1 > 8191)
				{
					Tv_A2p	=	Tv_A2p + 0xFF;
				}
				else
				{
					Tv_A2p	=	Tv_A2p + (Tv_Fa1 >> 5);
				}

				if ( Tv_Pk0 ^ A_PtrStat->pk[1])
				{
					/* LIMC */
					if ( Tv_A2p <= -12160)
					{
						Tv_A2p = -12288;
					}
					else if ( Tv_A2p >= 12416)
					{
						Tv_A2p = 12288;
					}
					else
					{
						Tv_A2p -= 0x80;
					}
				}
				else if ( Tv_A2p <= -12416)
				{
					Tv_A2p = -12288;
				}
				else if ( Tv_A2p >= 12160)
				{
					Tv_A2p = 12288;
				}
				else
				{
					Tv_A2p	=	Tv_A2p + 0x80;
				}
			}

			/* TRIGB & DELAY */
			A_PtrStat->a[1] = Tv_A2p;

			/* UPA1 */
			/* update predictor pole a[0] */
			A_PtrStat->a[0]	=	A_PtrStat->a[0] - (A_PtrStat->a[0] >> 8);
			if ( A_Dqsez != 0)
			{
				if ( Tv_Pks1 == 0)
				{
					A_PtrStat->a[0]	=	A_PtrStat->a[0] + 192;
				}
				else
				{
					A_PtrStat->a[0]	=	A_PtrStat->a[0] - 192;
				}
			}

			/* LIMD */
			Tv_A1ul	= 15360 - Tv_A2p;
			if (A_PtrStat->a[0] < -Tv_A1ul)
			{
				A_PtrStat->a[0] = -Tv_A1ul;
			}
			else if (A_PtrStat->a[0] > Tv_A1ul)
			{
				A_PtrStat->a[0] = Tv_A1ul;
			}

			/* UPB : update predictor zeros b[6] */
			for (Tv_Cnt = 0; Tv_Cnt < 6; Tv_Cnt++) 
			{
				A_PtrStat->b[Tv_Cnt]	=	A_PtrStat->b[Tv_Cnt] - (A_PtrStat->b[Tv_Cnt] >> 8);
				if (A_Dq & 0x7FFF) 
				{			/* XOR */
					if ((A_Dq ^ A_PtrStat->dq[Tv_Cnt]) >= 0)
					{
						A_PtrStat->b[Tv_Cnt]	=	A_PtrStat->b[Tv_Cnt] + 128;
					}
					else
					{
						A_PtrStat->b[Tv_Cnt]	=	A_PtrStat->b[Tv_Cnt] - 128;
					}
				}
			}
		}

		for (Tv_Cnt = 5; Tv_Cnt > 0; Tv_Cnt--)
		{
			A_PtrStat->dq[Tv_Cnt] = A_PtrStat->dq[Tv_Cnt-1];
		}
		/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
		if ( Tv_Mag == 0) 
		{
			A_PtrStat->dq[0] = (__s16)0xFC20;
			if ( A_Dq >= 0 )
			{
				A_PtrStat->dq[0] = 0x20;
			}
		} 
		else 
		{
			Tv_Quan		=	1;
			for (Tv_Exp = 0; Tv_Exp < 15; Tv_Exp++)
			{
				if ( Tv_Quan > Tv_Mag )
				{
					break;
				}
				// next
				Tv_Quan	=	Tv_Quan << 1;
			}

			A_PtrStat->dq[0]	=	( Tv_Exp << 6) + ((Tv_Mag << 6) >> Tv_Exp);
			if ( 0 > A_Dq )
			{
				A_PtrStat->dq[0]	=	A_PtrStat->dq[0] - 0x400;
			}
		}

		A_PtrStat->sr[1] = A_PtrStat->sr[0];
		/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
		if ( 0 == A_Sr) 
		{
			A_PtrStat->sr[0] = 0x20;
		} 
		else if ( 0 < A_Sr) 
		{
			Tv_Quan		=	1;
			for (Tv_Exp = 0; Tv_Exp < 15; Tv_Exp++)
			{
				if ( Tv_Quan > A_Sr )
				{
					break;
				}
				// next
				Tv_Quan	=	Tv_Quan << 1;
			}
			A_PtrStat->sr[0] = (Tv_Exp << 6) + (( A_Sr << 6) >> Tv_Exp);
		} 
		else if ( -32768 < A_Sr) 
		{
			Tv_Mag = -A_Sr;
			Tv_Quan		=	1;
			for (Tv_Exp = 0; Tv_Exp < 15; Tv_Exp++)
			{
				if ( Tv_Quan > Tv_Mag )
				{
					break;
				}
				// next
				Tv_Quan	=	Tv_Quan << 1;
			}
			A_PtrStat->sr[0] =  (Tv_Exp << 6) + ((Tv_Mag << 6) >> Tv_Exp) - 0x400;
		} 
		else
		{
			A_PtrStat->sr[0] = (__s16)0xFC20;
		}

		/* DELAY A */
		A_PtrStat->pk[1]	=	A_PtrStat->pk[0];
		A_PtrStat->pk[0]	=	Tv_Pk0;

		/* TONE */
		if ( Tv_Tr == 1)		/* this sample has been treated as data */
		{
			A_PtrStat->td = 0;	/* next one will be treated as voice */
		}
		else if (Tv_A2p < -11776)	/* small sample-to-sample correlation */
		{
			A_PtrStat->td = 1;	/* signal may be data */
		}
		else				/* signal is voice */
		{
			A_PtrStat->td = 0;
		}

		/* Adaptation speed control. */
		A_PtrStat->dms	=	A_PtrStat->dms + (( A_Fi - A_PtrStat->dms) >> 5);		/* FILTA */
		A_PtrStat->dml	=	A_PtrStat->dml + (((A_Fi << 2) - A_PtrStat->dml) >> 7);	/* FILTB */

		Tv_Tmp	=	(A_PtrStat->dms << 2) - A_PtrStat->dml;
		if ( 0 > Tv_Tmp )
		{
			Tv_Tmp	=	-Tv_Tmp;
		}
		if (Tv_Tr == 1)
		{
			A_PtrStat->ap = 256;
		}
		else if ( A_Y < 1536)					/* SUBTC */
		{
			A_PtrStat->ap	=	A_PtrStat->ap + ((0x200 - A_PtrStat->ap) >> 4);
		}
		else if (A_PtrStat->td == 1)
		{
			A_PtrStat->ap	=	A_PtrStat->ap + ((0x200 - A_PtrStat->ap) >> 4);
		}
		else if (Tv_Tmp >= (A_PtrStat->dml >> 3))
		{
			A_PtrStat->ap =	A_PtrStat->ap + ((0x200 - A_PtrStat->ap) >> 4);
		}
		else
		{
			A_PtrStat->ap =	A_PtrStat->ap + ((-A_PtrStat->ap) >> 4);
		}

}
//--------------------------------------------------------------------
void	GrG723_24_EncInit( Ptr_GrG72xStat A_PtrEnv, void* A_PtrOut, BOOL8 A_IsCont )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		if ( !A_IsCont )
		{
#ifdef E_GrG723Dbg
			g72x_init_state( A_PtrEnv );
#else
			A_PtrEnv->yl	= 34816;
			A_PtrEnv->yu	= 544;
			A_PtrEnv->dms = 0;
			A_PtrEnv->dml = 0;
			A_PtrEnv->ap	= 0;
			for (Tv_WkIdx = 0; Tv_WkIdx < 2; Tv_WkIdx++) 
			{
				A_PtrEnv->a[Tv_WkIdx]		= 0;
				A_PtrEnv->pk[Tv_WkIdx]	= 0;
				A_PtrEnv->sr[Tv_WkIdx]	= 32;
			}
			for (Tv_WkIdx = 0; Tv_WkIdx < 6; Tv_WkIdx++) 
			{
				A_PtrEnv->b[Tv_WkIdx]		= 0;
				A_PtrEnv->dq[Tv_WkIdx]	= 32;
			}
			A_PtrEnv->td	= 0;
#endif
		}	

		A_PtrEnv->PrgByte	=	0;
		A_PtrEnv->PrgBit	=	0;
		A_PtrEnv->PtrBuf	=	A_PtrOut;

		// reset first byte
		*((__u8*)A_PtrOut)	=	0;

}
//--------------------------------------------------------------------
__u32	GrG723_24_EncOne( Ptr_GrG72xStat A_PtrEnv, __s16 A_Val )
{
	// local -------------------
		__u32		Tv_Result;
		__s16		Tv_Sl;
		__s16		Tv_Sezi;
		__s16		Tv_Sez;
		__s16		Tv_Sei;
		__s16		Tv_Se;
		__s16		Tv_D;			/* SUBTA */
		__s16		Tv_Y;			/* MIX */
		__s16		Tv_Code;
		__s16		Tv_Dq;
		__s16		Tv_Sr;			/* ADDB */
		__s16		Tv_Dqsez;			/* ADDC */

		__u8*		Tv_PtrTg;
		__u8		Tv_WtVal;
	// code --------------------

#ifdef E_GrG723Dbg
		Tv_Code	=	(__u8)g723_24_encoder( A_Val, A_PtrEnv );
#else

		Tv_Sl		=	A_Val >> 2;

		Tv_Sezi	=	GrG723_24_PrdtZero( A_PtrEnv );
		Tv_Sez	=	Tv_Sezi >> 1;	
		Tv_Sei	= Tv_Sezi + GrG723_24_PrdtPole(A_PtrEnv);
		Tv_Se		= Tv_Sei >> 1;				/* se = estimated signal */

		Tv_D		= Tv_Sl - Tv_Se;			/* d = estimation diff. */

		/* quantize prediction difference d */
		Tv_Y		= GrG723_24_StepSize(A_PtrEnv);	/* quantizer step size */
		Tv_Code	=	GrG723_24_Quantize( Tv_D, Tv_Y );	/* ADPCM code */
		Tv_Dq		= GrG723_24_Reconstruct(Tv_Code & 4, V_GrG723_24_Dqlntab[Tv_Code], Tv_Y); /* quantized diff. */

		if ( 0 > Tv_Dq )	/* reconstructed signal */
		{
			Tv_Sr		= Tv_Se - (Tv_Dq & 0x3FFF);
		}
		else
		{
			Tv_Sr		= Tv_Se + Tv_Dq;
		}

		Tv_Dqsez = Tv_Sr + Tv_Sez - Tv_Se;		/* pole prediction diff. */

		GrG723_24_Update( Tv_Y, V_GrG723_24_Witab[Tv_Code], V_GrG723_24_Fitab[Tv_Code], Tv_Dq, Tv_Sr, Tv_Dqsez, A_PtrEnv );

#endif

		// update code
		Tv_PtrTg	=	(__u8*)( (__u32)A_PtrEnv->PtrBuf + A_PtrEnv->PrgByte );

		Tv_WtVal	=	(__u8)( (Tv_Code << A_PtrEnv->PrgBit) & 0xFF );
		*Tv_PtrTg	=	*Tv_PtrTg	| Tv_WtVal;

		A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit + 3;
		if ( 8 <= A_PtrEnv->PrgBit )
		{
			A_PtrEnv->PrgByte ++;
			Tv_PtrTg ++;
			*Tv_PtrTg	=	0;
			A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit - 8;
			if ( 0 != A_PtrEnv->PrgBit )
			{
				Tv_WtVal	=	(__u8)( (Tv_Code >> (3 - A_PtrEnv->PrgBit)) & 0xFF );
				*Tv_PtrTg	=	*Tv_PtrTg	| Tv_WtVal;
			}
		}

		// result size
		Tv_Result	=	A_PtrEnv->PrgByte;
		if ( 0 != A_PtrEnv->PrgBit )
		{
			Tv_Result ++;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrG723_24_EncFinish( Ptr_GrG72xStat A_PtrEnv )
{
		while ( 0 != A_PtrEnv->PrgBit )
		{
			GrG723_24_EncOne( A_PtrEnv, 0 );
		} 
		return	A_PtrEnv->PrgByte;
}
//--------------------------------------------------------------------
void	GrG723_24_DecInit( Ptr_GrG72xStat A_PtrEnv, void* A_PtrBuf, __u32 A_BufSize, BOOL8 A_IsCont )
{
	// local -------------------
		__s32		Tv_WkIdx;
	// code --------------------
		if ( !A_IsCont )
		{
#ifdef E_GrG723Dbg
			g72x_init_state( A_PtrEnv );
#else
			A_PtrEnv->yl	= 34816;
			A_PtrEnv->yu	= 544;
			A_PtrEnv->dms = 0;
			A_PtrEnv->dml = 0;
			A_PtrEnv->ap	= 0;
			for (Tv_WkIdx = 0; Tv_WkIdx < 2; Tv_WkIdx++) 
			{
				A_PtrEnv->a[Tv_WkIdx]		= 0;
				A_PtrEnv->pk[Tv_WkIdx]	= 0;
				A_PtrEnv->sr[Tv_WkIdx]	= 32;
			}
			for (Tv_WkIdx = 0; Tv_WkIdx < 6; Tv_WkIdx++) 
			{
				A_PtrEnv->b[Tv_WkIdx]		= 0;
				A_PtrEnv->dq[Tv_WkIdx]	= 32;
			}
			A_PtrEnv->td	= 0;
#endif
		}
		A_PtrEnv->PrgByte	=	0;
		A_PtrEnv->PrgBit	=	0;
		A_PtrEnv->PtrBuf	=	A_PtrBuf;
		A_PtrEnv->BufSize	=	A_BufSize;
}
//--------------------------------------------------------------------
BOOL8	GrG723_24_DecOne( Ptr_GrG72xStat A_PtrEnv, __s16* A_PtrOut )
{
	// local -------------------
		BOOL8		Tv_Result;

		__u8*		Tv_PtrSr;
		__u8		Tv_Code;
		__u8		Tv_Mask;

		__s16		Tv_Sezi;
		__s16		Tv_Sez;
		__s16		Tv_Sei;
		__s16		Tv_Se;
		__s16		Tv_Y;			/* MIX */
		__s16		Tv_Sr;			/* ADDB */
		__s16		Tv_Dq;
		__s16		Tv_Dqsez;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( A_PtrEnv->PrgByte < A_PtrEnv->BufSize )
		{
			// gat one code
			Tv_PtrSr	=	(__u8*)( (__u32)A_PtrEnv->PtrBuf + A_PtrEnv->PrgByte );
			Tv_Code		=	*Tv_PtrSr;
			Tv_Code		=	Tv_Code >> A_PtrEnv->PrgBit;

			A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit + 3;
			if ( 8 <= A_PtrEnv->PrgBit )
			{
				A_PtrEnv->PrgByte ++;
				Tv_PtrSr ++;
				A_PtrEnv->PrgBit	=	A_PtrEnv->PrgBit - 8;
				if ( 0 != A_PtrEnv->PrgBit )
				{
					Tv_Mask	=	(1 << (3 - A_PtrEnv->PrgBit)) - 1;
					Tv_Code	=	Tv_Code & Tv_Mask;
					Tv_Code	=	Tv_Code | ((*Tv_PtrSr) << (3 - A_PtrEnv->PrgBit));
				}
			}

			Tv_Code	=	Tv_Code & 0x07;

#ifdef E_GrG723Dbg
			*A_PtrOut	=	(__s16)g723_24_decoder( Tv_Code, A_PtrEnv );
#else

			Tv_Sezi	=	GrG723_24_PrdtZero( A_PtrEnv );
			Tv_Sez	= Tv_Sezi >> 1;
			Tv_Sei	= Tv_Sezi + GrG723_24_PrdtPole( A_PtrEnv );
			Tv_Se		= Tv_Sei >> 1;			/* se = estimated signal */

			Tv_Y		= GrG723_24_StepSize(A_PtrEnv);	/* adaptive quantizer step size */
			Tv_Dq		= GrG723_24_Reconstruct( (__s32)(Tv_Code & 0x04), V_GrG723_24_Dqlntab[Tv_Code], Tv_Y); /* unquantize pred diff */

			if ( 0 > Tv_Dq )
			{
				Tv_Sr	=	(Tv_Se - (Tv_Dq & 0x3FFF));
			}
			else
			{
				Tv_Sr	= ( Tv_Se + Tv_Dq );
			}

			Tv_Dqsez	= Tv_Sr - Tv_Se + Tv_Sez;			/* pole prediction diff. */

			GrG723_24_Update( Tv_Y, V_GrG723_24_Witab[Tv_Code], V_GrG723_24_Fitab[Tv_Code], Tv_Dq, Tv_Sr, Tv_Dqsez, A_PtrEnv);

			// output to buffer
			*A_PtrOut	=	Tv_Sr << 2;
#endif

			Tv_Result	=	TRUE;

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrG723_24_EncBuf( __s16* A_PtrSr, __u32 A_SmpCnt, void* A_PtrTg )
{
	// local -------------------
		St_GrG72xStat	Tv_Env;
		__u32	Tv_WkIdx;
	// code --------------------
		GrG723_24_EncInit( &Tv_Env, A_PtrTg, FALSE );
		for ( Tv_WkIdx=0;Tv_WkIdx < A_SmpCnt;Tv_WkIdx++ )
		{
			GrG723_24_EncOne( &Tv_Env, A_PtrSr[Tv_WkIdx] );
		}

		return	GrG723_24_EncFinish( &Tv_Env );
}
//--------------------------------------------------------------------
__u32	GrG723_24_DecBuf( void* A_PtrSr, __u32 A_Size, __s16* A_PtrTg )
{
	// local -------------------
		St_GrG72xStat	Tv_Env;
		__u32	Tv_WkIdx;
	// code --------------------
		GrG723_24_DecInit( &Tv_Env, A_PtrSr, A_Size, FALSE );
		Tv_WkIdx	=	0;
		while ( GrG723_24_DecOne( &Tv_Env, &A_PtrTg[Tv_WkIdx]) )
		{
			// next
			Tv_WkIdx ++;	
		}

		return	Tv_WkIdx;
}
//--------------------------------------------------------------------
__s32	
	g723_24_encoder(
	__s32		sl,
	struct St_GrG72xStat *state_ptr)
{
	__s16		sei, sezi, se, sez;	/* ACCUM */
	__s16		d;			/* SUBTA */
	__s16		y;			/* MIX */
	__s16		sr;			/* ADDB */
	__s16		dqsez;			/* ADDC */
	__s16		dq, i;

	sl >>= 2;		/* sl of 14-bit dynamic range */

#if 0
	sezi = predictor_zero(state_ptr);
#else
	sezi = GrG723_24_PrdtZero(state_ptr);
#endif
	sez = sezi >> 1;
#if 0
	sei = sezi + predictor_pole(state_ptr);
#else
	sei = sezi + GrG723_24_PrdtPole(state_ptr);
#endif
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation diff. */

	/* quantize prediction difference d */
	y = step_size(state_ptr);	/* quantizer step size */
	i = quantize(d, y, qtab_723_24, 3);	/* i = ADPCM code */
	dq = reconstruct(i & 4, _dqlntab[i], y); /* quantized diff. */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* pole prediction diff. */

	update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	return (i);
}

//--------------------------------------------------------------------
__s32	g723_24_decoder(	__s32		i,	struct St_GrG72xStat *state_ptr)
{
	__s16		sezi, sei, sez, se;	/* ACCUM */
	__s16		y;			/* MIX */
	__s16		sr;			/* ADDB */
	__s16		dq;
	__s16		dqsez;

	i &= 0x07;			/* mask to get proper bits */
#if 0
	sezi = predictor_zero(state_ptr);
#else
	sezi = GrG723_24_PrdtZero(state_ptr);
#endif
	sez = sezi >> 1;
#if 0
	sei = sezi + predictor_pole(state_ptr);
#else
	sei = sezi + GrG723_24_PrdtPole(state_ptr);
#endif
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x04, _dqlntab[i], y); /* unquantize pred diff */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

		return (sr << 2);	/* sr was of 14-bit dynamic range */
}
//--------------------------------------------------------------------
__u8	GrUlawEncOne(__s16 A_Sr)
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_Sign;
		__s16	Tv_Sr;
		__u8	Tv_Exponent;
		__u16	Tv_ExpMask;
		__u16	Tv_Mantissa;
	// code --------------------
		Tv_Sr		=	A_Sr;

		Tv_Sign	=	0;
		if(0 > Tv_Sr)
		{
			Tv_Sign	=	0x80;
			Tv_Sr		=	-Tv_Sr;
		}

		if(E_GrUlawSrMax <= Tv_Sr)
		{
			Tv_Sr	=	E_GrUlawSrMax;
		}
		Tv_Sr	=	Tv_Sr + E_GrUlawBias;

		Tv_Exponent	=	7;

		for( Tv_ExpMask = 0x4000; 0 == (Tv_Sr & Tv_ExpMask); Tv_Exponent --, Tv_ExpMask	=	Tv_ExpMask >> 1)
		{
		}

		Tv_Mantissa	=	(Tv_Sr >> (Tv_Exponent + 3)) & 0x0F;

		Tv_Result	=	Tv_Sign | (Tv_Exponent << 4) | (__u8)Tv_Mantissa;

		return	~Tv_Result;
}
//--------------------------------------------------------------------
__s16	GrUlawDecOne(__u8 A_Sr)
{
	// local -------------------
		__u8	Tv_Sr;
		__u8	Tv_Exponent;
		__u16	Tv_Data;
	// code --------------------
		Tv_Sr	=	~A_Sr;

		Tv_Exponent	=	(Tv_Sr & 0x70) >> 4;
		Tv_Data			=	(__u16)(Tv_Sr & 0x0F);
		Tv_Data			=	Tv_Data | 0x10;
		Tv_Data			=	Tv_Data << 1;
		Tv_Data ++;

		Tv_Data			=	Tv_Data << (Tv_Exponent + 2);
		Tv_Data			=	Tv_Data - E_GrUlawBias;

		if(0 != (0x80 & Tv_Sr))
		{
			Tv_Data	=	-Tv_Data;
		}

		return	Tv_Data;
}
//--------------------------------------------------------------------
__u32	GrUlawEncStrm(__s16* A_PtrSr, __u32 A_StrmSize, __u8* A_PtrOut)
{
	// local -------------------
		__u32	Tv_Result;
		__u32	Tv_SmpCnt;
	// code --------------------
		Tv_Result	=	0;
		Tv_SmpCnt	=	A_StrmSize >> 1;
		while(Tv_Result < Tv_SmpCnt)
		{
			A_PtrOut[Tv_Result]	=	GrUlawEncOne(A_PtrSr[Tv_Result]);

			// next
			Tv_Result ++;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrUlawDecStrm(__u8* A_PtrSr, __u32 A_StrmSize, __s16* A_PtrOut)
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		while(Tv_Result < A_StrmSize)
		{
			A_PtrOut[Tv_Result]	=	GrUlawDecOne(A_PtrSr[Tv_Result]);

			// next
			Tv_Result ++;
		}
		return	(Tv_Result << 1);
}
//--------------------------------------------------------------------
