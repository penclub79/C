/*
	GAUSE math library


*/

#include <GrMath.h>
#include <GrDumyTool.h>


//====================================================================
// local const


//====================================================================
// local type

//====================================================================
// local macro

#define SHR(x, n)      ((__u32)(x >> n))
#define ROTR(x, n)     ((__u32)(x >> n) | (__u32)(x << (32 - n)))

#define SIGU0(x)       (ROTR(x, 2 ) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SIGU1(x)       (ROTR(x, 6 ) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define SIGL0(x)       (ROTR(x, 7 ) ^ ROTR(x, 18) ^ SHR(x, 3 ))
#define SIGL1(x)       (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define CH(x, y, z)    (z ^ (x & (y ^ z)))
#define MAJ(x, y, z)   ((x & (y | z)) | (y & z))
#define WT(t, x, y, z) (Tv_Buf[t] = SIGL1(Tv_Buf[x]) + Tv_Buf[y] + SIGL0(Tv_Buf[z]) + Tv_Buf[t])

#define PR(a, b, c, d, e, f, g, h, k, w) {	\
	Tv_Tmp	= h + SIGU1(e) + CH(e, f, g) + k + w;	\
	d += Tv_Tmp;																	\
	h = Tv_Tmp + SIGU0(a) + MAJ(a, b, c); }


#define PUTU32(ct, st) {                           \
	(ct)[0] = (__u8)((st) >> 24); \
	(ct)[1] = (__u8)((st) >> 16); \
	(ct)[2] = (__u8)((st) >>  8); \
	(ct)[3] = (__u8)(st); }


#define GETU32(pt)     (((__u32)(pt)[0] << 24) ^ \
	((__u32)(pt)[1] << 16) ^  \
	((__u32)(pt)[2] <<  8) ^  \
	((__u32)(pt)[3]))

#define Mac_GrMathCrc32Do(x)	Tv_Result =	V_GrMathCrc32Tbl[(Tv_Result ^ (x)) & 255]	^ (Tv_Result >> 8)


//====================================================================
// global var

__u16 V_GrMathCrc16Tbl[] = 
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

__u32 V_GrMathCrc32Tbl[256] = 
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
	0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
	0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
	0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
	0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
	0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
	0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
	0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
	0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
	0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
	0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
	0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
	0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
	0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
	0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


//====================================================================
// local functions

void	GrMathSha256CalBuf( Ptr_GrMathSha256Ctx A_PtrCtx, __u8* A_PtrBuf );

//--------------------------------------------------------------------
#if 0
__s32		GrAbs32( __s32 A_Val )
{
	// local -------------------
		__s32		Tv_Result;
	// code --------------------
		Tv_Result		=	A_Val;

		if ( 0 > A_Val )
		{
			Tv_Result		=	-A_Val;
		}

		return	Tv_Result;
}
#endif

//--------------------------------------------------------------------
/*!
 * \brief
 * check has common area ( 1-D )
 * 
 * \param A_St1
 * start point of area 1
 * 
 * \param A_Ed1
 * end point of area 1
 * 
 * \param A_St2
 * start point of area 2
 * 
 * \param A_Ed2
 * end point of area 2
 * 
 * \returns
 * if will found do return TRUE.
 * 
 */
BOOL8		GrMathIsHaveCommon( __s32 A_St1, __s32 A_Ed1, __s32 A_St2, __s32 A_Ed2 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (A_St1 >= A_St2) && (A_St1 <= A_Ed2) )		{	Tv_Result		=	TRUE;	}
		if ( (A_Ed1 >= A_St2) && (A_Ed1 <= A_Ed2) )		{	Tv_Result		=	TRUE;	}
		if ( (A_St2 >= A_St1) && (A_St2 <= A_Ed1) )		{	Tv_Result		=	TRUE;	}
		if ( (A_Ed2 >= A_St1) && (A_Ed2 <= A_Ed1) )		{	Tv_Result		=	TRUE;	}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		GrMathIsHaveCommonQw( __u64 A_St1, __u64 A_Ed1, __u64 A_St2, __u64 A_Ed2 )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (A_St1 >= A_St2) && (A_St1 <= A_Ed2) )		{	Tv_Result		=	TRUE;	}
		if ( (A_Ed1 >= A_St2) && (A_Ed1 <= A_Ed2) )		{	Tv_Result		=	TRUE;	}
		if ( (A_St2 >= A_St1) && (A_St2 <= A_Ed1) )		{	Tv_Result		=	TRUE;	}
		if ( (A_Ed2 >= A_St1) && (A_Ed2 <= A_Ed1) )		{	Tv_Result		=	TRUE;	}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrMathSha256Init( Ptr_GrMathSha256Ctx A_PtrCtx )
{
		// init
		A_PtrCtx->Len			=	0;

		A_PtrCtx->hash[0]	= 0x6a09e667;
		A_PtrCtx->hash[1]	= 0xbb67ae85;
		A_PtrCtx->hash[2]	= 0x3c6ef372;
		A_PtrCtx->hash[3]	= 0xa54ff53a;
		A_PtrCtx->hash[4]	= 0x510e527f;
		A_PtrCtx->hash[5]	= 0x9b05688c;
		A_PtrCtx->hash[6]	= 0x1f83d9ab;
		A_PtrCtx->hash[7]	= 0x5be0cd19;
}
//--------------------------------------------------------------------
void	GrMathSha256DataIn( Ptr_GrMathSha256Ctx A_PtrCtx, void* A_PtrData, __u32 A_Len )
{
	// local -------------------
		__u32	Tv_Remain;
		__u32	Tv_CpSize;
		__u32	Tv_AbleSize;
		__u32	Tv_BufPos;
		void*	Tv_PtrDt;
	// code --------------------
		if ( 0 != A_Len )
		{
			Tv_Remain	=	A_Len;
			Tv_PtrDt	=	A_PtrData;
			while ( 0 != Tv_Remain )
			{
				// align 64 byte
				Tv_CpSize		=	Tv_Remain;
				Tv_BufPos		=	0x3F & A_PtrCtx->Len;
				Tv_AbleSize	=	64 - Tv_BufPos;
				if ( Tv_AbleSize < Tv_CpSize )
				{
					Tv_CpSize	=	Tv_AbleSize;
				}

				if ( 64 == Tv_CpSize )
				{
					GrMathSha256CalBuf( A_PtrCtx, (__u8*)Tv_PtrDt );
					// next
					A_PtrCtx->Len	=	A_PtrCtx->Len + 64;
					Tv_PtrDt	=	(void*)( (__u32)Tv_PtrDt + 64 );
					Tv_Remain	=	Tv_Remain - 64;
				}
				else
				{
					// in buffer
					GrDumyCopyMem( &(A_PtrCtx->buf[Tv_BufPos]), Tv_PtrDt, Tv_CpSize );
					// next
					A_PtrCtx->Len	=	A_PtrCtx->Len + Tv_CpSize;
					Tv_PtrDt	=	(void*)( (__u32)Tv_PtrDt + Tv_CpSize );
					Tv_Remain	=	Tv_Remain - Tv_CpSize;
					// check process able
					if ( 0 == (0x3F & A_PtrCtx->Len) )
					{
						// do process
						GrMathSha256CalBuf( A_PtrCtx, A_PtrCtx->buf );
					}

				}
			}

		}

}
//--------------------------------------------------------------------
void	GrMathSha256CalBuf( Ptr_GrMathSha256Ctx A_PtrCtx, __u8* A_PtrBuf )
{
	// local -------------------
		__u32	Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H;
		__u32	Tv_Tmp;
		__u32*	Tv_Buf;
	// code --------------------
		// init
		Tv_Buf	=	(__u32*)A_PtrCtx->buf;
		// endian change
		Tv_Buf[0]  = GETU32(A_PtrBuf     );
		Tv_Buf[1]  = GETU32(A_PtrBuf + 4 );
		Tv_Buf[2]  = GETU32(A_PtrBuf + 8 );
		Tv_Buf[3]  = GETU32(A_PtrBuf + 12);
		Tv_Buf[4]  = GETU32(A_PtrBuf + 16);
		Tv_Buf[5]  = GETU32(A_PtrBuf + 20);
		Tv_Buf[6]  = GETU32(A_PtrBuf + 24);
		Tv_Buf[7]  = GETU32(A_PtrBuf + 28);
		Tv_Buf[8]  = GETU32(A_PtrBuf + 32);
		Tv_Buf[9]  = GETU32(A_PtrBuf + 36);
		Tv_Buf[10] = GETU32(A_PtrBuf + 40);
		Tv_Buf[11] = GETU32(A_PtrBuf + 44);
		Tv_Buf[12] = GETU32(A_PtrBuf + 48);
		Tv_Buf[13] = GETU32(A_PtrBuf + 52);
		Tv_Buf[14] = GETU32(A_PtrBuf + 56);
		Tv_Buf[15] = GETU32(A_PtrBuf + 60);

		// load work variable
		Tv_A	=	A_PtrCtx->hash[0];
		Tv_B	=	A_PtrCtx->hash[1];
		Tv_C	=	A_PtrCtx->hash[2];
		Tv_D	=	A_PtrCtx->hash[3];
		Tv_E	=	A_PtrCtx->hash[4];
		Tv_F	=	A_PtrCtx->hash[5];
		Tv_G	=	A_PtrCtx->hash[6];
		Tv_H	=	A_PtrCtx->hash[7];

		// process message block
		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0x428a2f98, Tv_Buf[0]);
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0x71374491, Tv_Buf[1]);
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0xb5c0fbcf, Tv_Buf[2]);
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0xe9b5dba5, Tv_Buf[3]);
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x3956c25b, Tv_Buf[4]);
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0x59f111f1, Tv_Buf[5]);
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x923f82a4, Tv_Buf[6]);
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0xab1c5ed5, Tv_Buf[7]);
		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0xd807aa98, Tv_Buf[8]);
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0x12835b01, Tv_Buf[9]);
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0x243185be, Tv_Buf[10]);
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0x550c7dc3, Tv_Buf[11]);
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x72be5d74, Tv_Buf[12]);
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0x80deb1fe, Tv_Buf[13]);
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x9bdc06a7, Tv_Buf[14]);
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0xc19bf174, Tv_Buf[15]);

		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0xe49b69c1, WT( 0, 14,  9,  1));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0xefbe4786, WT( 1, 15, 10,  2));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0x0fc19dc6, WT( 2,  0, 11,  3));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0x240ca1cc, WT( 3,  1, 12,  4));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x2de92c6f, WT( 4,  2, 13,  5));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0x4a7484aa, WT( 5,  3, 14,  6));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x5cb0a9dc, WT( 6,  4, 15,  7));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0x76f988da, WT( 7,  5,  0,  8));
		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0x983e5152, WT( 8,  6,  1,  9));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0xa831c66d, WT( 9,  7,  2, 10));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0xb00327c8, WT(10,  8,  3, 11));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0xbf597fc7, WT(11,  9,  4, 12));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0xc6e00bf3, WT(12, 10,  5, 13));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0xd5a79147, WT(13, 11,  6, 14));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x06ca6351, WT(14, 12,  7, 15));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0x14292967, WT(15, 13,  8,  0));

		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0x27b70a85, WT( 0, 14,  9,  1));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0x2e1b2138, WT( 1, 15, 10,  2));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0x4d2c6dfc, WT( 2,  0, 11,  3));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0x53380d13, WT( 3,  1, 12,  4));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x650a7354, WT( 4,  2, 13,  5));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0x766a0abb, WT( 5,  3, 14,  6));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x81c2c92e, WT( 6,  4, 15,  7));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0x92722c85, WT( 7,  5,  0,  8));
		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0xa2bfe8a1, WT( 8,  6,  1,  9));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0xa81a664b, WT( 9,  7,  2, 10));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0xc24b8b70, WT(10,  8,  3, 11));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0xc76c51a3, WT(11,  9,  4, 12));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0xd192e819, WT(12, 10,  5, 13));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0xd6990624, WT(13, 11,  6, 14));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0xf40e3585, WT(14, 12,  7, 15));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0x106aa070, WT(15, 13,  8,  0));

		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0x19a4c116, WT( 0, 14,  9,  1));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0x1e376c08, WT( 1, 15, 10,  2));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0x2748774c, WT( 2,  0, 11,  3));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0x34b0bcb5, WT( 3,  1, 12,  4));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x391c0cb3, WT( 4,  2, 13,  5));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0x4ed8aa4a, WT( 5,  3, 14,  6));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0x5b9cca4f, WT( 6,  4, 15,  7));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0x682e6ff3, WT( 7,  5,  0,  8));
		PR(Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, 0x748f82ee, WT( 8,  6,  1,  9));
		PR(Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, 0x78a5636f, WT( 9,  7,  2, 10));
		PR(Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, 0x84c87814, WT(10,  8,  3, 11));
		PR(Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, Tv_E, 0x8cc70208, WT(11,  9,  4, 12));
		PR(Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, Tv_D, 0x90befffa, WT(12, 10,  5, 13));
		PR(Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, Tv_C, 0xa4506ceb, WT(13, 11,  6, 14));
		PR(Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, Tv_B, 0xbef9a3f7, WT(14, 12,  7, 15));
		PR(Tv_B, Tv_C, Tv_D, Tv_E, Tv_F, Tv_G, Tv_H, Tv_A, 0xc67178f2, WT(15, 13,  8,  0));

		// store hash value
		A_PtrCtx->hash[0]	=	A_PtrCtx->hash[0] + Tv_A;
		A_PtrCtx->hash[1]	=	A_PtrCtx->hash[1] + Tv_B;
		A_PtrCtx->hash[2]	=	A_PtrCtx->hash[2] + Tv_C;
		A_PtrCtx->hash[3]	=	A_PtrCtx->hash[3] + Tv_D;
		A_PtrCtx->hash[4]	=	A_PtrCtx->hash[4] + Tv_E;
		A_PtrCtx->hash[5]	=	A_PtrCtx->hash[5] + Tv_F;
		A_PtrCtx->hash[6]	=	A_PtrCtx->hash[6] + Tv_G;
		A_PtrCtx->hash[7]	=	A_PtrCtx->hash[7] + Tv_H;

}
//--------------------------------------------------------------------
void	GrMathSha256Finish( Ptr_GrMathSha256Ctx A_PtrCtx, __u8* A_PtrRtCode )
{
	// local -------------------
		__u32	Tv_BufPos;
		__u8*	Tv_PtrWk;
		__u32	Tv_PadLen;
		__u32	Tv_Tmp;
	// code --------------------
		Tv_BufPos	=	A_PtrCtx->Len & 0x3F;
		// mark finish
		Tv_PtrWk	=	(__u8*)( (Def_GrCalPtr)A_PtrCtx->buf + Tv_BufPos );
		*Tv_PtrWk	=	0x80;
		Tv_PtrWk ++;

		// check remain size
		if ( 56 > Tv_BufPos )
		{
			Tv_PadLen	=	56 - (Tv_BufPos + 1);
		}
		else
		{
			GrDumyZeroMem( Tv_PtrWk, 64 - (Tv_BufPos + 1) );
			GrMathSha256CalBuf( A_PtrCtx, A_PtrCtx->buf ); 
			Tv_PadLen	=	56;
			Tv_PtrWk	=	A_PtrCtx->buf;
		}
		
		if ( 0 != Tv_PadLen )
		{
			GrDumyZeroMem( Tv_PtrWk, Tv_PadLen );
		}
		Tv_PtrWk	=	Tv_PtrWk + Tv_PadLen;

		// append length
		Tv_Tmp	=	A_PtrCtx->Len >> 29;
		PUTU32(Tv_PtrWk,Tv_Tmp);
		Tv_PtrWk	=	Tv_PtrWk + 4;
		Tv_Tmp	=	A_PtrCtx->Len << 3;
		PUTU32(Tv_PtrWk,Tv_Tmp);
		GrMathSha256CalBuf( A_PtrCtx, A_PtrCtx->buf );

		// restore result
		PUTU32(A_PtrRtCode,      A_PtrCtx->hash[0]);
		PUTU32(A_PtrRtCode + 4,  A_PtrCtx->hash[1]);
		PUTU32(A_PtrRtCode + 8,  A_PtrCtx->hash[2]);
		PUTU32(A_PtrRtCode + 12, A_PtrCtx->hash[3]);
		PUTU32(A_PtrRtCode + 16, A_PtrCtx->hash[4]);
		PUTU32(A_PtrRtCode + 20, A_PtrCtx->hash[5]);
		PUTU32(A_PtrRtCode + 24, A_PtrCtx->hash[6]);
		PUTU32(A_PtrRtCode + 28, A_PtrCtx->hash[7]);

}
//--------------------------------------------------------------------
__u16	GrMathCrc16( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		__u16	Tv_Result;
		__u32	Tv_WkIdx;
		__u16	Tv_TblIdx;
		__u8*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	0;

		Tv_PtrBuf	=	(__u8*)A_PtrBuf;

		for ( Tv_WkIdx=0;Tv_WkIdx < A_Size;Tv_WkIdx++ )
		{
			Tv_TblIdx	=	((Tv_Result >> 8) ^ *Tv_PtrBuf) & 0xFF;
			Tv_Result	=	V_GrMathCrc16Tbl[Tv_TblIdx] ^ (Tv_Result << 8);
			// next
			Tv_PtrBuf ++;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrMathCrc32NoCmp( __u32 A_PrevCrc, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		__u32		Tv_Result;
		__u32*	Tv_PtrBuf;
		__u32		Tv_Remain;
		__u8*		Tv_PtrBt;
	// code --------------------
		Tv_Result	=	A_PrevCrc;
		Tv_PtrBuf	=	(__u32*)A_PtrBuf;
		Tv_Remain	=	A_Size;
		// align
		if ( (0 != A_Size) && (0 != (3 & ((Def_GrCalPtr)Tv_PtrBuf))) )
		{
			Tv_PtrBt	=	(__u8*)Tv_PtrBuf;
			do 
			{
				Mac_GrMathCrc32Do( *Tv_PtrBt );
				Tv_PtrBt ++;
				Tv_Remain --;
			} while((0 != Tv_Remain) && (0 != (3 & ((Def_GrCalPtr)Tv_PtrBt))));

			// update to buffer pointer
			Tv_PtrBuf	=	(__u32*)Tv_PtrBt;
		}

		// process 32bit unit
		while ( 4 <= Tv_Remain )
		{
			Tv_Result	=	Tv_Result ^ (*Tv_PtrBuf);
			Mac_GrMathCrc32Do(0);
			Mac_GrMathCrc32Do(0);
			Mac_GrMathCrc32Do(0);
			Mac_GrMathCrc32Do(0);
			// next
			Tv_PtrBuf ++;
			Tv_Remain	=	Tv_Remain - 4;
		}
		// least byte 
		Tv_PtrBt	=	(__u8*)Tv_PtrBuf;
		while ( 0 != Tv_Remain )
		{
			Mac_GrMathCrc32Do( *Tv_PtrBt );
			// next
			Tv_PtrBt ++;
			Tv_Remain --;
		}
		
		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32	GrMathCrc32( __u32 A_PrevCrc, void* A_PtrBuf, __u32 A_Size )
{
		return GrMathCrc32NoCmp( A_PrevCrc ^ 0xffffffff, A_PtrBuf, A_Size) ^ 0xffffffff;
}
//--------------------------------------------------------------------

