/*
	g 723 8Khz MPC MLQ

*/

//====================================================================
//include

#include <GrLnxApp.h>
#include <GrG723D24.h>
#ifdef LINUX_APP
#include <stdlib.h>
#endif

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//global var

__s16	V_GrG723d24Dqlntab[8]	= {-2048, 135, 273, 373, 373, 273, 135, -2048};
__s16	V_GrG723d24Witab[8]		= {-128, 960, 4384, 18624, 18624, 4384, 960, -128};
__s16	V_GrG723d24Fitab[8]		= {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};
__s16 V_GrG723d24Qtab[3]		= {8, 218, 331};
__s16 V_GrG723d24Power2[15] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
																0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrG723d24::Cls_GrG723d24():
Cls_GrAdoDec()
{
		m_Fcc			=	E_GrFccG723_24;
		m_Vol			=	E_GrG723d24VolDflt;
}
//--------------------------------------------------------------------
Cls_GrG723d24::~Cls_GrG723d24()
{
}
//--------------------------------------------------------------------
void		Cls_GrG723d24::Init( void )
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//table init
		m_Yl					= 34816;
		m_Yu					= 544;
		m_Dms					= 0;
		m_Dml					= 0;
		m_Ap					= 0;
		for (Tv_WkIdx = 0; Tv_WkIdx < 2; Tv_WkIdx++) 
		{
			m_A[Tv_WkIdx]			= 0;
			m_Pk[Tv_WkIdx]		= 0;
			m_Sr[Tv_WkIdx]		= 32;
		}
		for (Tv_WkIdx = 0; Tv_WkIdx < 6; Tv_WkIdx++) 
		{
			m_B[Tv_WkIdx]			= 0;
			m_Dq[Tv_WkIdx]		= 32;
		}
		m_Td					= 0;
}
//--------------------------------------------------------------------
void		Cls_GrG723d24::SetSrcData( void* A_PtrSrc, __u32 A_SizeSrc )
{
	// local -------------------
	// code --------------------
		//source buf init
		m_PtrSrc			=	(__u8*)A_PtrSrc;
		m_SrcSize			=	A_SizeSrc;
		m_SrcLchInBit	=	0;
		m_SrcLatch		=	0;
		//inherited
		Cls_GrAdoDec::SetSrcData( A_PtrSrc, A_SizeSrc );
}
//--------------------------------------------------------------------
void		Cls_GrG723d24::SetDestBuf( void* A_PtrDst, __u32 A_SizeDst )
{
		m_PtrDst		=	(__s16*)A_PtrDst;
		m_DstSize		=	A_SizeDst;
		Cls_GrAdoDec::SetDestBuf( A_PtrDst, A_SizeDst );
}
//--------------------------------------------------------------------
BOOL8		Cls_GrG723d24::GetCode( __u8* A_PtrRtSmp )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Code;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( NULL != m_PtrSrc )
		{
			//load buffer to latch
			if ( m_SrcLchInBit < E_GrG723d24CodeBitUnit )
			{
				if ( 0 != m_SrcSize )
				{
					Tv_Code		=	*m_PtrSrc;
					m_SrcLatch	=	m_SrcLatch | (Tv_Code << m_SrcLchInBit);
					m_SrcLchInBit	=	m_SrcLchInBit + 8;
					m_SrcSize --;
					m_PtrSrc ++;
				}
				else
				{
					m_IsSrcEmpty		=	TRUE;
				}
			}

			//get code
			if ( m_SrcLchInBit >= E_GrG723d24CodeBitUnit )
			{
				*A_PtrRtSmp		=	(__u8)(m_SrcLatch & ((1 << E_GrG723d24CodeBitUnit) - 1));
				m_SrcLatch		=	m_SrcLatch >> E_GrG723d24CodeBitUnit;
				m_SrcLchInBit	=	m_SrcLchInBit - E_GrG723d24CodeBitUnit;
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32		Cls_GrG723d24::Decode( void )
{
	// local -------------------
		__u32		Tv_Result;
		__u8		Tv_Code;
	// code --------------------
		//init
		Tv_Result		=	0;

		//run decode
		while ( 0 != m_DstSize )
		{
			//check full dest buffer
			if ( GetCode( &Tv_Code ) )
			{
				//decode
				*m_PtrDst		=	CodeDecToPcm16( (__s32)Tv_Code );
				m_PtrDst ++;
				m_DstSize		=	m_DstSize - 2;
				//next
				Tv_Result		=	Tv_Result + 2;
			}
			else
			{
				break;
			}
		}
		//update stat
		if ( 0 == m_DstSize )
		{
			m_IsDstBufFull		=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::FindQtzIdx( __s32 A_Val, __s16* A_PtrTbl, __s32 A_Size )
{
	// local -------------------
		__s32		i;
	// code --------------------
		for (i = 0; i < A_Size; i++)
			if (A_Val < *A_PtrTbl++)
				break;
		return (i);
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::Fmult( __s32 A_An, __s32 A_Srn )
{
	// local -------------------
		__s16		anmag, anexp, anmant;
		__s16		wanexp, wanmant;	//wanmag
		__s16		retval;
	// code --------------------
		anmag = (A_An > 0) ? A_An : ((-A_An) & 0x1FFF);
		anexp = FindQtzIdx(anmag, V_GrG723d24Power2, 15) - 6;
		anmant = (anmag == 0) ? 32 :
			(anexp >= 0) ? anmag >> anexp : anmag << -anexp;
		wanexp = anexp + ((A_Srn >> 6) & 0xF) - 13;

		wanmant = (anmant * (A_Srn & 077) + 0x30) >> 4;
		retval = (wanexp >= 0) ? ((wanmant << wanexp) & 0x7FFF) :
			(wanmant >> -wanexp);

		return (((A_An ^ A_Srn) < 0) ? -retval : retval);
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::PredictZero( void )
{
	// local -------------------
		__s32		i;
		__s32		sezi;
	// code --------------------
		sezi = Fmult(m_B[0] >> 2, m_Dq[0]);
		for (i = 1; i < 6; i++)			/* ACCUM */
			sezi += Fmult(m_B[i] >> 2, m_Dq[i]);
		return (sezi);
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::PredictPole( void )
{
	return (Fmult(m_A[1] >> 2, m_Sr[1]) +
		Fmult(m_A[0] >> 2, m_Sr[0]));
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::GetQtzStep( void )
{
	// local -------------------
		__s32		y;
		__s32		dif;
		__s32		al;
	// code --------------------
		if (m_Ap >= 256)
			return (m_Yu);
		else {
			y = m_Yl >> 6;
			dif = m_Yu - y;
			al = m_Ap >> 2;
			if (dif > 0)
				y += (dif * al) >> 6;
			else if (dif < 0)
				y += (dif * al + 0x3F) >> 6;
			return (y);
		}
}
//--------------------------------------------------------------------
__s32			Cls_GrG723d24::Reconstruct( __s32 A_Sign, __s32 A_Dqln, __s32 A_Y )
{
	// local -------------------
		__s16		dql;	/* Log of 'dq' magnitude */
		__s16		dex;	/* Integer part of log */
		__s16		dqt;
		__s16		dq;	/* Reconstructed difference signal sample */
	// code --------------------
		dql = A_Dqln + (A_Y >> 2);	/* ADDA */

		if (dql < 0) {
			return ((A_Sign) ? -0x8000 : 0);
		} else {		/* ANTILOG */
			dex = (dql >> 7) & 15;
			dqt = 128 + (dql & 127);
			dq = (dqt << 7) >> (14 - dex);
			return ((A_Sign) ? (dq - 0x8000) : dq);
		}
}
//--------------------------------------------------------------------
void		Cls_GrG723d24::UpdtCalc(	__s32	A_code_size, __s32 A_y, __s32 A_wi, __s32 A_fi,
																	__s32 A_dq, __s32 A_sr, __s32 A_dqsez )
{
	// local -------------------
		__s32		cnt;
		__s16		mag, exp;	/* Adaptive predictor, FLOAT A */	//, mant
		__s16		a2p;		/* LIMC */
		__s16		a1ul;		/* UPA1 */
		__s16		pks1;	/* UPA2 */	//ua2, 
		__s16		fa1;	//uga2a, 
		//__s16		uga2b;
		__s8		tr;		/* tone/transition detector */
		__s16		ylint, thr2, dqthr;
		__s16  		ylfrac, thr1;
		__s16		pk0;
	// code --------------------
		pk0 = (A_dqsez < 0) ? 1 : 0;	/* needed in updating predictor poles */

		mag = A_dq & 0x7FFF;		/* prediction difference magnitude */
		/* TRANS */
		ylint = m_Yl >> 15;	/* exponent part of yl */
		ylfrac = (m_Yl >> 10) & 0x1F;	/* fractional part of yl */
		thr1 = (32 + ylfrac) << ylint;		/* threshold */
		thr2 = (ylint > 9) ? 31 << 10 : thr1;	/* limit thr2 to 31 << 10 */
		dqthr = (thr2 + (thr2 >> 1)) >> 1;	/* dqthr = 0.75 * thr2 */
		if (m_Td == 0)		/* signal supposed voice */
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
		m_Yu = A_y + ((A_wi - A_y) >> 5);

		/* LIMB */
		if (m_Yu < 544)	/* 544 <= yu <= 5120 */
			m_Yu = 544;
		else if (m_Yu > 5120)
			m_Yu = 5120;

		/* FILTE & DELAY */
		/* update steady state step size multiplier */
		m_Yl += m_Yu + ((-m_Yl) >> 6);

		/*
		* Adaptive predictor coefficients.
		*/
		if (tr == 1) 
		{			/* reset a's and b's for modem signal */
			m_A[0] = 0;
			m_A[1] = 0;
			m_B[0] = 0;
			m_B[1] = 0;
			m_B[2] = 0;
			m_B[3] = 0;
			m_B[4] = 0;
			m_B[5] = 0;
		} 
		else 
		{			/* update a's and b's */
			pks1 = pk0 ^ m_Pk[0];		/* UPA2 */

			/* update predictor pole a[1] */
			a2p = m_A[1] - (m_A[1] >> 7);
			if (A_dqsez != 0) 
			{
				fa1 = (pks1) ? m_A[0] : -m_A[0];
				if (fa1 < -8191)	/* a2p = function of fa1 */
					a2p -= 0x100;
				else if (fa1 > 8191)
					a2p += 0xFF;
				else
					a2p += fa1 >> 5;

				if (pk0 ^ m_Pk[1])
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
			m_A[1] = a2p;

			/* UPA1 */
			/* update predictor pole a[0] */
			m_A[0] -= m_A[0] >> 8;
			if (A_dqsez != 0)
				if (pks1 == 0)
					m_A[0] += 192;
				else
					m_A[0] -= 192;

			/* LIMD */
			a1ul = 15360 - a2p;
			if (m_A[0] < -a1ul)
				m_A[0] = -a1ul;
			else if (m_A[0] > a1ul)
				m_A[0] = a1ul;

			/* UPB : update predictor zeros b[6] */
			for (cnt = 0; cnt < 6; cnt++) {
				if (A_code_size == 5)		/* for 40Kbps G.723 */
					m_B[cnt] -= m_B[cnt] >> 9;
				else			/* for G.721 and 24Kbps G.723 */
					m_B[cnt] -= m_B[cnt] >> 8;
				if (A_dq & 0x7FFF) {			/* XOR */
					if ((A_dq ^ m_Dq[cnt]) >= 0)
						m_B[cnt] += 128;
					else
						m_B[cnt] -= 128;
				}
			}
		}

		for (cnt = 5; cnt > 0; cnt--)
			m_Dq[cnt] = m_Dq[cnt-1];
		/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
		if (mag == 0) {
			m_Dq[0] = (A_dq >= 0) ? 0x20 : 0xFC20;
		} else {
			exp = FindQtzIdx(mag, V_GrG723d24Power2, 15);
			m_Dq[0] = (A_dq >= 0) ?
				(exp << 6) + ((mag << 6) >> exp) :
			(exp << 6) + ((mag << 6) >> exp) - 0x400;
		}

		m_Sr[1] = m_Sr[0];
		/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
		if (A_sr == 0) {
			m_Sr[0] = 0x20;
		} else if (A_sr > 0) {
			exp = FindQtzIdx(A_sr, V_GrG723d24Power2, 15);
			m_Sr[0] = (exp << 6) + ((A_sr << 6) >> exp);
		} else if (A_sr > -32768) {
			mag = -A_sr;
			exp = FindQtzIdx(mag, V_GrG723d24Power2, 15);
			m_Sr[0] =  (exp << 6) + ((mag << 6) >> exp) - 0x400;
		} else
			m_Sr[0] = (__s16)0xFC20;

		/* DELAY A */
		m_Pk[1] = m_Pk[0];
		m_Pk[0] = pk0;

		/* TONE */
		if (tr == 1)		/* this sample has been treated as data */
			m_Td = 0;	/* next one will be treated as voice */
		else if (a2p < -11776)	/* small sample-to-sample correlation */
			m_Td = 1;	/* signal may be data */
		else				/* signal is voice */
			m_Td = 0;

		/*
		* Adaptation speed control.
		*/
		m_Dms += (A_fi - m_Dms) >> 5;		/* FILTA */
		m_Dml += (((A_fi << 2) - m_Dml) >> 7);	/* FILTB */

		if (tr == 1)
			m_Ap = 256;
		else if (A_y < 1536)					/* SUBTC */
			m_Ap += (0x200 - m_Ap) >> 4;
		else if (m_Td == 1)
			m_Ap += (0x200 - m_Ap) >> 4;
		else if (abs((m_Dms << 2) - m_Dml) >=	(m_Dml >> 3))
			m_Ap += (0x200 - m_Ap) >> 4;
		else
			m_Ap += (-m_Ap) >> 4;

}
//--------------------------------------------------------------------
__s16		Cls_GrG723d24::CodeDecToPcm16( __u8 A_Code )
{
	// local -------------------
		__s16		Tv_Result;
		__s16		Tv_sezi, Tv_sei, Tv_sez, Tv_se;	/* ACCUM */
		__s16		Tv_y;			/* MIX */
		__s16		Tv_sr;			/* ADDB */
		__s16		Tv_dq;
		__s16		Tv_dqsez;

	// code --------------------
		Tv_sezi = PredictZero();
		Tv_sez	= Tv_sezi >> 1;
		Tv_sei	= Tv_sezi + PredictPole();
		Tv_se		= Tv_sei >> 1;			/* se = estimated signal */

		Tv_y		= GetQtzStep();			/* adaptive quantizer step size */
		Tv_dq		= Reconstruct(A_Code & 0x04, V_GrG723d24Dqlntab[A_Code], Tv_y); /* unquantize pred diff */

		Tv_sr = (Tv_dq < 0) ? (Tv_se - (Tv_dq & 0x3FFF)) : (Tv_se + Tv_dq); /* reconst. signal */

		Tv_dqsez = Tv_sr - Tv_se + Tv_sez;			/* pole prediction diff. */

		UpdtCalc(3, Tv_y, V_GrG723d24Witab[A_Code], V_GrG723d24Fitab[A_Code], Tv_dq, Tv_sr, Tv_dqsez );

		//Tv_Result		=	Tv_sr * 4;
		Tv_Result		=	Tv_sr << m_Vol;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrG723d24::SetVolum( __u32 A_Volum )
{
		if ( E_GrG723d24VolMin > A_Volum )
		{
			m_Vol		=	E_GrG723d24VolMin;
		}
		else if ( E_GrG723d24VolMax < A_Volum )
		{
			m_Vol			=	E_GrG723d24VolMax;
		}
		else
		{
			m_Vol			=	A_Volum;
		}
}
//--------------------------------------------------------------------
__u32		Cls_GrG723d24::GetVolum( void )
{
		return	m_Vol;
}
//--------------------------------------------------------------------
__u32		Cls_GrG723d24::GetVolMin( void )
{
		return	E_GrG723d24VolMin;
}
//--------------------------------------------------------------------
__u32		Cls_GrG723d24::GetVolMax( void )
{
		return	E_GrG723d24VolMax;
}
//--------------------------------------------------------------------

