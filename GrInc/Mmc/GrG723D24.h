/*
	linux on win32 emulation
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_GrG723D24
#define	_GrG723D24

//====================================================================
// uses
#include	<GrTypeBase.h>
#include	<GrAdoBase.h>

//====================================================================
//constance
#define	E_GrG723d24CodeBitUnit		3

#define E_GrG723d24VolMin					0
#define E_GrG723d24VolMax					14
#define E_GrG723d24VolDflt				3

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class

class Cls_GrG723d24	:	public	Cls_GrAdoDec
{
	private:
		//manage
		__u32		m_Vol;

		//sample get
		__u8*		m_PtrSrc;
		__u32		m_SrcSize;
		__u32		m_SrcLchInBit;
		__u32		m_SrcLatch;

		//output
		__s16*	m_PtrDst;
		__u32		m_DstSize;

		//decode 
		__s32			m_Yl;						/* base multiplier */
		__s16		m_Yu;						/* additional multiplier */
		__s16		m_Dms;					/* estimate value - __s16 */
		__s16		m_Dml;					/* estimate value - long */
		__s16		m_Ap;						/* additional value of multiplier */

		__s16		m_A[2];					/* filter coefficients of high area */
		__s16		m_B[6];					/* filter coefficients of zero area */
		__s16		m_Pk[2];				/* sign of decoded */
		__s16		m_Dq[6];				/* stand of sample signe diffrent */
		__s16		m_Sr[2];				/* sample scaled and quantized	*/
		__s8		m_Td;						/* delayed tone detect */

	protected:
		BOOL8		GetCode( __u8* A_PtrRtSmp );
		__s16		CodeDecToPcm16( __u8 A_Code );		
		__s32			PredictZero( void );
		__s32			PredictPole( void );
		__s32			Fmult( __s32 A_An, __s32 A_Srn );
		__s32			FindQtzIdx( __s32 A_Val, __s16* A_PtrTbl, __s32 A_Size );
		__s32			GetQtzStep( void );
		__s32			Reconstruct( __s32 A_Sign, __s32 A_Dqln, __s32 A_Y );
		void		UpdtCalc( __s32	A_code_size, __s32 A_y, __s32 A_wi, __s32 A_fi,
						__s32 A_dq, __s32 A_sr, __s32 A_dqsez );
	public:
		Cls_GrG723d24( );
		virtual	~Cls_GrG723d24();

		//decode
		virtual	void		Init( void );
		virtual	void		SetSrcData( void* A_PtrSrc, __u32 A_SizeSrc );
		virtual	void		SetDestBuf( void* A_PtrDst, __u32 A_SizeDst );
		virtual	__u32		Decode( void );
		/*	return valu = decoded size	*/

		virtual	void		SetVolum( __u32 A_Volum );
		virtual	__u32		GetVolum( void );
		virtual	__u32		GetVolMin( void );
		virtual	__u32		GetVolMax( void );
};


//====================================================================
//global function


//====================================================================
//global variable

//====================================================================

//out Enviroment conditional
#endif


