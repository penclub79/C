/*
AES128 
Key Length 16byte
ECB Mode
No Padding ( only 64byte will use , PlainText = 64byte )

Plaintext -> Encrypt -> Ciphertext
Ciphertext -> Decrypt -> Plaintext
*/
#include <GrAES128.h>
//====================================================================
// local macro
#define rotl	ROTL_DWORD
#define rotr	ROTR_DWORD

#define ROTL_DWORD(x, n) ( (__u32)((x) << (n)) | (__u32)((x) >> (32-(n))) )
#define ROTR_DWORD(x, n) ( (__u32)((x) >> (n)) | (__u32)((x) << (32-(n))) )

#define ENDIAN_REVERSE_DWORD(dwS)	( (ROTL_DWORD((dwS),  8) & 0x00ff00ff)	\
									 | (ROTL_DWORD((dwS), 24) & 0xff00ff00) )

#define BIG_B2D(B, D)		D = ENDIAN_REVERSE_DWORD(*(__u32 *)(B))
#define BIG_D2B(D, B)		*(__u32 *)(B) = ENDIAN_REVERSE_DWORD(D)
#define LITTLE_B2D(B, D)	D = *(__u32 *)(B)
#define LITTLE_D2B(D, B)	*(__u32 *)(B) = (__u32)(D)

#define shiftdown_byte(x,n)	((__u8)((x) >> (8 * n)))

#define BlockCopy(pbDst, pbSrc) {					\
	((__u32 *)(pbDst))[0] = ((__u32 *)(pbSrc))[0];	\
	((__u32 *)(pbDst))[1] = ((__u32 *)(pbSrc))[1];	\
	((__u32 *)(pbDst))[2] = ((__u32 *)(pbSrc))[2];	\
	((__u32 *)(pbDst))[3] = ((__u32 *)(pbSrc))[3];	\
}
#define BlockXor(pbDst, phSrc1, phSrc2) {			\
	((__u32 *)(pbDst))[0] = ((__u32 *)(phSrc1))[0]	\
						  ^ ((__u32 *)(phSrc2))[0];	\
	((__u32 *)(pbDst))[1] = ((__u32 *)(phSrc1))[1]	\
						  ^ ((__u32 *)(phSrc2))[1];	\
	((__u32 *)(pbDst))[2] = ((__u32 *)(phSrc1))[2]	\
						  ^ ((__u32 *)(phSrc2))[2];	\
	((__u32 *)(pbDst))[3] = ((__u32 *)(phSrc1))[3]	\
						  ^ ((__u32 *)(phSrc2))[3];	\
}


#define f_nround(bo, bi, k) {				\
	bo[0] = V_GrAES128_ft_tbl[0][shiftdown_byte(bi[0],0)]		\
		  ^ V_GrAES128_ft_tbl[1][shiftdown_byte(bi[1],1)]		\
		  ^ V_GrAES128_ft_tbl[2][shiftdown_byte(bi[2],2)]		\
		  ^ V_GrAES128_ft_tbl[3][shiftdown_byte(bi[3],3)] ^ k[0];\
	bo[1] = V_GrAES128_ft_tbl[0][shiftdown_byte(bi[1],0)]		\
		  ^ V_GrAES128_ft_tbl[1][shiftdown_byte(bi[2],1)]		\
		  ^ V_GrAES128_ft_tbl[2][shiftdown_byte(bi[3],2)]		\
		  ^ V_GrAES128_ft_tbl[3][shiftdown_byte(bi[0],3)] ^ k[1];\
	bo[2] = V_GrAES128_ft_tbl[0][shiftdown_byte(bi[2],0)]		\
		  ^ V_GrAES128_ft_tbl[1][shiftdown_byte(bi[3],1)]		\
		  ^ V_GrAES128_ft_tbl[2][shiftdown_byte(bi[0],2)]		\
		  ^ V_GrAES128_ft_tbl[3][shiftdown_byte(bi[1],3)] ^ k[2];\
	bo[3] = V_GrAES128_ft_tbl[0][shiftdown_byte(bi[3],0)]		\
		  ^ V_GrAES128_ft_tbl[1][shiftdown_byte(bi[0],1)]		\
		  ^ V_GrAES128_ft_tbl[2][shiftdown_byte(bi[1],2)]		\
		  ^ V_GrAES128_ft_tbl[3][shiftdown_byte(bi[2],3)] ^ k[3];\
	k += 4;									\
}

#define i_nround(bo, bi, k) {				\
	bo[0] = V_GrAES128_it_tbl[0][shiftdown_byte(bi[0],0)]		\
		  ^ V_GrAES128_it_tbl[1][shiftdown_byte(bi[3],1)]		\
		  ^ V_GrAES128_it_tbl[2][shiftdown_byte(bi[2],2)]		\
		  ^ V_GrAES128_it_tbl[3][shiftdown_byte(bi[1],3)] ^ k[0];\
	bo[1] = V_GrAES128_it_tbl[0][shiftdown_byte(bi[1],0)]		\
		  ^ V_GrAES128_it_tbl[1][shiftdown_byte(bi[0],1)]		\
		  ^ V_GrAES128_it_tbl[2][shiftdown_byte(bi[3],2)]		\
		  ^ V_GrAES128_it_tbl[3][shiftdown_byte(bi[2],3)] ^ k[1];\
	bo[2] = V_GrAES128_it_tbl[0][shiftdown_byte(bi[2],0)]		\
		  ^ V_GrAES128_it_tbl[1][shiftdown_byte(bi[1],1)]		\
		  ^ V_GrAES128_it_tbl[2][shiftdown_byte(bi[0],2)]		\
		  ^ V_GrAES128_it_tbl[3][shiftdown_byte(bi[3],3)] ^ k[2];\
	bo[3] = V_GrAES128_it_tbl[0][shiftdown_byte(bi[3],0)]		\
		  ^ V_GrAES128_it_tbl[1][shiftdown_byte(bi[2],1)]		\
		  ^ V_GrAES128_it_tbl[2][shiftdown_byte(bi[1],2)]		\
		  ^ V_GrAES128_it_tbl[3][shiftdown_byte(bi[0],3)] ^ k[3];\
	k += 4;					\
}

#ifdef LARGE_TABLES
	#define f_lround(bo, bi, k) {				\
		bo[0] = V_GrAES128_fl_tbl[0][shiftdown_byte(bi[0],0)]		\
			  ^ V_GrAES128_fl_tbl[1][shiftdown_byte(bi[1],1)]		\
			  ^ V_GrAES128_fl_tbl[2][shiftdown_byte(bi[2],2)]		\
			  ^ V_GrAES128_fl_tbl[3][shiftdown_byte(bi[3],3)] ^ k[0];\
		bo[1] = V_GrAES128_fl_tbl[0][shiftdown_byte(bi[1],0)]		\
			  ^ V_GrAES128_fl_tbl[1][shiftdown_byte(bi[2],1)]		\
			  ^ V_GrAES128_fl_tbl[2][shiftdown_byte(bi[3],2)]		\
			  ^ V_GrAES128_fl_tbl[3][shiftdown_byte(bi[0],3)] ^ k[1];\
		bo[2] = V_GrAES128_fl_tbl[0][shiftdown_byte(bi[2],0)]		\
			  ^ V_GrAES128_fl_tbl[1][shiftdown_byte(bi[3],1)]		\
			  ^ V_GrAES128_fl_tbl[2][shiftdown_byte(bi[0],2)]		\
			  ^ V_GrAES128_fl_tbl[3][shiftdown_byte(bi[1],3)] ^ k[2];\
		bo[3] = V_GrAES128_fl_tbl[0][shiftdown_byte(bi[3],0)]		\
			  ^ V_GrAES128_fl_tbl[1][shiftdown_byte(bi[0],1)]		\
			  ^ V_GrAES128_fl_tbl[2][shiftdown_byte(bi[1],2)]		\
			  ^ V_GrAES128_fl_tbl[3][shiftdown_byte(bi[2],3)] ^ k[3];\
	}

	#define i_lround(bo, bi, k) {				\
		bo[0] = V_GrAES128_il_tab[0][shiftdown_byte(bi[0],0)]		\
			  ^ V_GrAES128_il_tab[1][shiftdown_byte(bi[3],1)]		\
			  ^ V_GrAES128_il_tab[2][shiftdown_byte(bi[2],2)]		\
			  ^ V_GrAES128_il_tab[3][shiftdown_byte(bi[1],3)] ^ k[0];\
		bo[1] = V_GrAES128_il_tab[0][shiftdown_byte(bi[1],0)]		\
			  ^ V_GrAES128_il_tab[1][shiftdown_byte(bi[0],1)]		\
			  ^ V_GrAES128_il_tab[2][shiftdown_byte(bi[3],2)]		\
			  ^ V_GrAES128_il_tab[3][shiftdown_byte(bi[2],3)] ^ k[1];\
		bo[2] = V_GrAES128_il_tab[0][shiftdown_byte(bi[2],0)]		\
			  ^ V_GrAES128_il_tab[1][shiftdown_byte(bi[1],1)]		\
			  ^ V_GrAES128_il_tab[2][shiftdown_byte(bi[0],2)]		\
			  ^ V_GrAES128_il_tab[3][shiftdown_byte(bi[3],3)] ^ k[2];\
		bo[3] = V_GrAES128_il_tab[0][shiftdown_byte(bi[3],0)]		\
			  ^ V_GrAES128_il_tab[1][shiftdown_byte(bi[2],1)]		\
			  ^ V_GrAES128_il_tab[2][shiftdown_byte(bi[1],2)]		\
			  ^ V_GrAES128_il_tab[3][shiftdown_byte(bi[0],3)] ^ k[3];\
		}
#else
	#define f_rl(bo, bi, n, k)										\
		bo[n] = (__u32)V_GrAES128_sbx_tbl[shiftdown_byte(bi[n],0)] ^					\
			rotl(((__u32)V_GrAES128_sbx_tbl[shiftdown_byte(bi[(n + 1) & 3],1)]),  8) ^	\
			rotl(((__u32)V_GrAES128_sbx_tbl[shiftdown_byte(bi[(n + 2) & 3],2)]), 16) ^	\
			rotl(((__u32)V_GrAES128_sbx_tbl[shiftdown_byte(bi[(n + 3) & 3],3)]), 24) ^ *(k + n)

	#define i_rl(bo, bi, n, k)										\
		bo[n] = (__u32)V_GrAES128_isb_tbl[shiftdown_byte(bi[n],0)] ^					\
			rotl(((__u32)V_GrAES128_isb_tbl[shiftdown_byte(bi[(n + 3) & 3],1)]),  8) ^	\
			rotl(((__u32)V_GrAES128_isb_tbl[shiftdown_byte(bi[(n + 2) & 3],2)]), 16) ^	\
			rotl(((__u32)V_GrAES128_isb_tbl[shiftdown_byte(bi[(n + 1) & 3],3)]), 24) ^ *(k + n)

	#define f_lround(bo, bi, k) \
		f_rl(bo, bi, 0, k); 	\
		f_rl(bo, bi, 1, k); 	\
		f_rl(bo, bi, 2, k); 	\
		f_rl(bo, bi, 3, k)

	#define i_lround(bo, bi, k) \
		i_rl(bo, bi, 0, k); 	\
		i_rl(bo, bi, 1, k); 	\
		i_rl(bo, bi, 2, k); 	\
		i_rl(bo, bi, 3, k)
#endif


#define star_x(x) (((x) & 0x7f7f7f7f) << 1) ^ ((((x) & 0x80808080) >> 7) * 0x1b)

#define imix_col(y,x)		\
	 u	= star_x(x);		\
	 v	= star_x(u);		\
	 w	= star_x(v);		\
	 t	= w ^ (x);			\
	(y) = u ^ v ^ w;		\
	(y) ^= rotr(u ^ t,  8) ^ \
		  rotr(v ^ t, 16) ^ \
		  rotr(t,24)

		  
#define ff_mult(a,b)	(a && b ? V_GrAES128_pow_tbl[(V_GrAES128_log_tbl[a] + V_GrAES128_log_tbl[b]) % 255] : 0)

#define ls_box(x)				 \
	( V_GrAES128_fl_tbl[0][shiftdown_byte(x, 0)] ^	 \
	V_GrAES128_fl_tbl[1][shiftdown_byte(x, 1)] ^	 \
	V_GrAES128_fl_tbl[2][shiftdown_byte(x, 2)] ^	 \
	V_GrAES128_fl_tbl[3][shiftdown_byte(x, 3)] )

//====================================================================
// global var

__u8 V_GrAES128_log_tbl[256]; 
__u8 V_GrAES128_pow_tbl[256];
__u8 V_GrAES128_sbx_tbl[256];
__u8 V_GrAES128_isb_tbl[256];
__u32 V_GrAES128_ft_tbl[4][256];
__u32 V_GrAES128_it_tbl[4][256];
__u32 V_GrAES128_fl_tbl[4][256];
__u32 V_GrAES128_il_tab[4][256];

//====================================================================
// local functions

void GrAES128KeySchedule( __u8 *A_UserKey, __u32 k_len, __u32 *e_key);
void GrAES128_Encrypt( void *CipherKey, __u8 *Data );			
void GrAES128_Decrypt( void *CipherKey, __u8 *Data );
__u32 GrAES128PadSet( __u8 *pbOutBuffer, __u32 dRmdLen, __u32 dBlockLen, __u32 dPaddingType );
__u32 GrAES128PadChck( __u8 *pbOutBuffer, __u32 dBlockLen, __u32 dPaddingType );

void GrAES128Set( __u32 ModeID, __u32 PadType, __u8 *IV, Ptr_GrAES128Info A_PtrInfo );
__u32 GrAES128_EncKeySch( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info	A_PtrInfo );
__u32 GrAES128_DecKeySch( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info A_PtrInfo );

//--------------------------------------------------------------------
void GrAES128KeySchedule( __u8 *A_UserKey, __u32 k_len, __u32 *e_key )
{
	// local -------------------
		__u32	Tv_Idx, Tv_r;
		__u32	Tv_t;
		__u8	Tv_p, Tv_q;
		__u32	Tv_rco_tbl[10]; 
		__u32	Tv_tbl_gen = 0;
	// code --------------------	
		//generate tables_start
		V_GrAES128_log_tbl[7] = 0;
		for( Tv_Idx = 0,Tv_p = 1; Tv_Idx < 256; ++Tv_Idx )
		{
			V_GrAES128_pow_tbl[Tv_Idx] = (__u8)Tv_p;
			V_GrAES128_log_tbl[Tv_p] = (__u8)Tv_Idx;
			Tv_p = (__u8)(Tv_p ^ (Tv_p << 1) ^ (Tv_p & 0x80 ? 0x01b : 0));
		}

		V_GrAES128_log_tbl[1] = 0;
		Tv_p = 1;

		for( Tv_Idx = 0; Tv_Idx < 10; ++Tv_Idx )
		{
			Tv_rco_tbl[Tv_Idx] = Tv_p; 
			Tv_p = (__u8)((Tv_p << 1) ^ (Tv_p & 0x80 ? 0x1b : 0));
		}

		for( Tv_Idx = 0; Tv_Idx < 256; ++Tv_Idx )
		{	
			Tv_p = (__u8)(Tv_Idx ? V_GrAES128_pow_tbl[255 - V_GrAES128_log_tbl[Tv_Idx]] : 0);
			Tv_q = Tv_p;
			Tv_q = (__u8)((Tv_q >> 7) | (Tv_q << 1));
			Tv_p ^= Tv_q;
			Tv_q = (__u8)((Tv_q >> 7) | (Tv_q << 1));
			Tv_p ^= Tv_q;
			Tv_q = (__u8)((Tv_q >> 7) | (Tv_q << 1));
			Tv_p ^= Tv_q;
			Tv_q = (__u8)((Tv_q >> 7) | (Tv_q << 1));
			Tv_p ^= Tv_q ^ 0x63;
			V_GrAES128_sbx_tbl[Tv_Idx] = (__u8)Tv_p;
			V_GrAES128_isb_tbl[Tv_p] = (__u8)Tv_Idx;
		}

		for( Tv_Idx = 0; Tv_Idx < 256; ++Tv_Idx )
		{
			Tv_p = V_GrAES128_sbx_tbl[Tv_Idx]; 

	#ifdef	LARGE_TABLES
			Tv_t = Tv_p;
			V_GrAES128_fl_tbl[0][Tv_Idx] = Tv_t;
			V_GrAES128_fl_tbl[1][Tv_Idx] = rotl(Tv_t,  8);
			V_GrAES128_fl_tbl[2][Tv_Idx] = rotl(Tv_t, 16);
			V_GrAES128_fl_tbl[3][Tv_Idx] = rotl(Tv_t, 24);
	#endif
			Tv_t = ((__u32)ff_mult(2, Tv_p)) |
				((__u32)Tv_p <<  8) |
				((__u32)Tv_p << 16) |
				((__u32)ff_mult(3, Tv_p) << 24);
		
			V_GrAES128_ft_tbl[0][Tv_Idx] = Tv_t;
			V_GrAES128_ft_tbl[1][Tv_Idx] = rotl(Tv_t,	8);
			V_GrAES128_ft_tbl[2][Tv_Idx] = rotl(Tv_t, 16);
			V_GrAES128_ft_tbl[3][Tv_Idx] = rotl(Tv_t, 24);

			Tv_p = V_GrAES128_isb_tbl[Tv_Idx]; 

	#ifdef	LARGE_TABLES
			Tv_t = Tv_p; 
			V_GrAES128_il_tab[0][Tv_Idx] = Tv_t; 
			V_GrAES128_il_tab[1][Tv_Idx] = rotl(Tv_t,	8); 
			V_GrAES128_il_tab[2][Tv_Idx] = rotl(Tv_t, 16); 
			V_GrAES128_il_tab[3][Tv_Idx] = rotl(Tv_t, 24);
	#endif 
			Tv_t = ((__u32)ff_mult(14, Tv_p)) |
				((__u32)ff_mult( 9, Tv_p) <<	8) |
				((__u32)ff_mult(13, Tv_p) << 16) |
				((__u32)ff_mult(11, Tv_p) << 24);
		
			V_GrAES128_it_tbl[0][Tv_Idx] = Tv_t; 
			V_GrAES128_it_tbl[1][Tv_Idx] = rotl(Tv_t,	8); 
			V_GrAES128_it_tbl[2][Tv_Idx] = rotl(Tv_t, 16); 
			V_GrAES128_it_tbl[3][Tv_Idx] = rotl(Tv_t, 24); 
		}

		Tv_tbl_gen = 1;
		//generate tables_end	

		LITTLE_B2D(&(A_UserKey[ 0]), e_key[0]);
		LITTLE_B2D(&(A_UserKey[ 4]), e_key[1]);
		LITTLE_B2D(&(A_UserKey[ 8]), e_key[2]);
		LITTLE_B2D(&(A_UserKey[12]), e_key[3]);

		switch(k_len)
		{
			case 4:
					Tv_r = e_key[3];
					for(Tv_Idx = 0; Tv_Idx < 10; ++Tv_Idx)
					{
						Tv_r = ls_box(rotr(Tv_r,  8)) ^ Tv_rco_tbl[Tv_Idx];
						Tv_r ^= e_key[4 * Tv_Idx];     e_key[4 * Tv_Idx + 4] = Tv_r;
						Tv_r ^= e_key[4 * Tv_Idx + 1]; e_key[4 * Tv_Idx + 5] = Tv_r;
						Tv_r ^= e_key[4 * Tv_Idx + 2]; e_key[4 * Tv_Idx + 6] = Tv_r;
						Tv_r ^= e_key[4 * Tv_Idx + 3]; e_key[4 * Tv_Idx + 7] = Tv_r;
					}
					break;

			case 6:
					LITTLE_B2D(&(A_UserKey[16]), e_key[4]);
					LITTLE_B2D(&(A_UserKey[20]), e_key[5]);
					Tv_r = e_key[5];
					for(Tv_Idx = 0; Tv_Idx < 8; ++Tv_Idx) 
					{
						Tv_r = ls_box(rotr(Tv_r,	8)) ^ Tv_rco_tbl[Tv_Idx];
						Tv_r ^= e_key[6 * Tv_Idx];	   e_key[6 * Tv_Idx + 6] = Tv_r;
						Tv_r ^= e_key[6 * Tv_Idx + 1]; e_key[6 * Tv_Idx + 7] = Tv_r;
						Tv_r ^= e_key[6 * Tv_Idx + 2]; e_key[6 * Tv_Idx + 8] = Tv_r;
						Tv_r ^= e_key[6 * Tv_Idx + 3]; e_key[6 * Tv_Idx + 9] = Tv_r;
						Tv_r ^= e_key[6 * Tv_Idx + 4]; e_key[6 * Tv_Idx + 10] = Tv_r;
						Tv_r ^= e_key[6 * Tv_Idx + 5]; e_key[6 * Tv_Idx + 11] = Tv_r;
					}
					break;

			case 8:
					LITTLE_B2D(&(A_UserKey[16]), e_key[4]);
					LITTLE_B2D(&(A_UserKey[20]), e_key[5]);
					LITTLE_B2D(&(A_UserKey[24]), e_key[6]);
					LITTLE_B2D(&(A_UserKey[28]), e_key[7]);
					Tv_r = e_key[7];
					for(Tv_Idx = 0; Tv_Idx < 7; ++Tv_Idx)
					{
						Tv_r = ls_box(rotr(Tv_Idx,	8)) ^ Tv_rco_tbl[Tv_Idx];
						Tv_r ^= e_key[8 * Tv_Idx];	   e_key[8 * Tv_Idx + 8] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 1]; e_key[8 * Tv_Idx + 9] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 2]; e_key[8 * Tv_Idx + 10] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 3]; e_key[8 * Tv_Idx + 11] = Tv_r;
						Tv_r  = e_key[8 *Tv_Idx + 4] ^ ls_box(Tv_r);
						e_key[8 * Tv_Idx + 12] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 5]; e_key[8 * Tv_Idx + 13] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 6]; e_key[8 * Tv_Idx + 14] = Tv_r;
						Tv_r ^= e_key[8 * Tv_Idx + 7]; e_key[8 * Tv_Idx + 15] = Tv_r;
					}
					break;
		}
}
//--------------------------------------------------------------------
void GrAES128_Encrypt( void *CipherKey, __u8 *Data )
{
	// local -------------------
		Ptr_GrAES128_CphrKey	Tv_PtrCphrKey;
		__u32	*e_key;
		__u32	k_len;
		__u32	b0[4], b1[4], *kp;
	// code --------------------
		Tv_PtrCphrKey = (Ptr_GrAES128_CphrKey)CipherKey;
		e_key=Tv_PtrCphrKey->RK;
		k_len=Tv_PtrCphrKey->k_len;

		LITTLE_B2D(&(Data[ 0]), b0[0]);
		LITTLE_B2D(&(Data[ 4]), b0[1]);
		LITTLE_B2D(&(Data[ 8]), b0[2]);
		LITTLE_B2D(&(Data[12]), b0[3]);

		b0[0] ^= e_key[0];
		b0[1] ^= e_key[1];
		b0[2] ^= e_key[2];
		b0[3] ^= e_key[3];

		kp = e_key + 4;

		switch( k_len ) 
		{
			case 8 :
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
			case 6 :
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
			case 4 :
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
				f_nround(b1, b0, kp); f_nround(b0, b1, kp);
				f_nround(b1, b0, kp); f_lround(b0, b1, kp);
		}
	
		LITTLE_D2B(b0[0], &(Data[ 0]));
		LITTLE_D2B(b0[1], &(Data[ 4]));
		LITTLE_D2B(b0[2], &(Data[ 8]));
		LITTLE_D2B(b0[3], &(Data[12]));
}
//--------------------------------------------------------------------
void GrAES128_Decrypt( void *CipherKey, __u8 *Data )
{
	// local -------------------
		Ptr_GrAES128_CphrKey	Tv_PtrCphrKey;
		__u32	*d_key;
		__u32	k_len;
		__u32	b0[4], b1[4], *kp;
	// code --------------------	
		Tv_PtrCphrKey = (Ptr_GrAES128_CphrKey)CipherKey;
		d_key = Tv_PtrCphrKey->RK;
		k_len = Tv_PtrCphrKey->k_len;

		LITTLE_B2D(&(Data[ 0]), b0[0]);
		LITTLE_B2D(&(Data[ 4]), b0[1]);
		LITTLE_B2D(&(Data[ 8]), b0[2]);
		LITTLE_B2D(&(Data[12]), b0[3]);

		b0[0] ^= d_key[0];
		b0[1] ^= d_key[1];
		b0[2] ^= d_key[2];
		b0[3] ^= d_key[3];

		kp = d_key + 4;

		switch( k_len ) 
		{
			case 8 :
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
			case 6 :
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
			case 4 :
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
				i_nround(b1, b0, kp); i_nround(b0, b1, kp);
				i_nround(b1, b0, kp); i_lround(b0, b1, kp);
		}

		LITTLE_D2B(b0[0], &(Data[ 0]));
		LITTLE_D2B(b0[1], &(Data[ 4]));
		LITTLE_D2B(b0[2], &(Data[ 8]));
		LITTLE_D2B(b0[3], &(Data[12]));
}
//--------------------------------------------------------------------
__u32 GrAES128PadSet( __u8 *pbOutBuffer, __u32 dRmdLen, __u32 dBlockLen, __u32 dPaddingType )
{
	// local -------------------
		__u32 dPadLen;
	// code --------------------	
		switch( dPaddingType ) 
		{
			case E_GrAES128_NO_PADDING :
				if( dRmdLen==0 )	return 0;
				else				return E_GrAES128_DATA_LEN_ERROR;

			case E_GrAES128_PKCS_PADDING :
				dPadLen = dBlockLen - dRmdLen;
				memset(pbOutBuffer+dRmdLen, (__s8)dPadLen, (__s32)dPadLen);
				return dPadLen;

			default :
				return E_GrAES128_FATAL_ERROR;
		}
}
//--------------------------------------------------------------------
__u32 GrAES128PadChck( __u8 *pbOutBuffer, __u32 dBlockLen, __u32 dPaddingType )
{
	// local -------------------
		__u32 i, dPadLen;
	// code --------------------
		switch( dPaddingType )
		{
			case E_GrAES128_NO_PADDING :
				return 0;
			
			case E_GrAES128_PKCS_PADDING :
				dPadLen = pbOutBuffer[dBlockLen-1];
				if( ((__s32)dPadLen<=0) || (dPadLen>(__s32)dBlockLen) )
				{
					return E_GrAES128_PAD_CHECK_ERROR;
				}
				for( i=1; i<=dPadLen; i++)
				{
					if( pbOutBuffer[dBlockLen-i] != dPadLen )
					{
						return E_GrAES128_PAD_CHECK_ERROR;
					}
				}
				return dPadLen;

			default :
				return E_GrAES128_FATAL_ERROR;
		}
}
//--------------------------------------------------------------------
void GrAES128Set( __u32 ModeID, __u32 PadType, __u8 *IV, Ptr_GrAES128Info A_PtrInfo )
{
	// local -------------------
	// code --------------------
		A_PtrInfo->ModeID = ModeID;
		A_PtrInfo->PadType = PadType;

		if( IV!=NULL )
		{
			memcpy(A_PtrInfo->IV, IV, E_GrAES128_BLOCK_LEN);
		}
		else
		{
			memset(A_PtrInfo->IV, 0, E_GrAES128_BLOCK_LEN);
		}
}
//--------------------------------------------------------------------
__u32 GrAES128_EncKeySch( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info A_PtrInfo )
{
	// local -------------------	
		Ptr_GrAES128_CphrKey Tv_PtrCphrKey;
		__u32	*e_key;
		__u32	k_len;
	// code --------------------
		Tv_PtrCphrKey=(Ptr_GrAES128_CphrKey)A_PtrInfo->RoundKey;
		e_key=Tv_PtrCphrKey->RK;
	
		if( (A_UserKeyLen!=16) && (A_UserKeyLen!=24) && (A_UserKeyLen!=32) )
		{
			return E_GrAES128_INVALID_USERKEYLEN;
		}
		
		k_len = (A_UserKeyLen + 3) / 4;
		Tv_PtrCphrKey->k_len = k_len;

		GrAES128KeySchedule(A_UserKey, k_len, e_key);

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32 GrAES128_DecKeySch( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info A_PtrInfo )
{
	// local -------------------	
		Ptr_GrAES128_CphrKey	Tv_PtrCphrKey;	
		__u32	*d_key;
		__u32	k_len, t_key[64];
		__u32	i, t, u, v, w;
	// code --------------------
		Tv_PtrCphrKey=(Ptr_GrAES128_CphrKey)A_PtrInfo->RoundKey;
		d_key=Tv_PtrCphrKey->RK;
	
		if( (A_UserKeyLen!=16) && (A_UserKeyLen!=24) && (A_UserKeyLen!=32) )
		{
			return E_GrAES128_INVALID_USERKEYLEN;
		}

		k_len = (A_UserKeyLen + 3) / 4;
		Tv_PtrCphrKey->k_len = k_len;

		GrAES128KeySchedule(A_UserKey, k_len, t_key);

		d_key[0] = t_key[4 * k_len + 24];
		d_key[1] = t_key[4 * k_len + 25];
		d_key[2] = t_key[4 * k_len + 26];
		d_key[3] = t_key[4 * k_len + 27];

		for( i=4; i<4*(k_len+6); i+=4) 
		{
			imix_col(d_key[i+0], t_key[4*k_len+24-i+0]);
			imix_col(d_key[i+1], t_key[4*k_len+24-i+1]);
			imix_col(d_key[i+2], t_key[4*k_len+24-i+2]);
			imix_col(d_key[i+3], t_key[4*k_len+24-i+3]);
		}
		d_key[i+0] = t_key[4*k_len+24-i+0];
		d_key[i+1] = t_key[4*k_len+24-i+1];
		d_key[i+2] = t_key[4*k_len+24-i+2];
		d_key[i+3] = t_key[4*k_len+24-i+3];

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32	GrAES128_EncInit( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info	A_PtrInfo )
{
	// local -------------------	
		Ptr_GrAES128_CphrKey Tv_PtrCphrKey;
		__u32	*e_key;
		__u32	k_len;
	// code --------------------
		//GrAES128Set
		//A_PtrInfo->ModeID = E_GrAES128_ECB;
		A_PtrInfo->ModeID = E_GrAES128_CBC;
		A_PtrInfo->PadType = E_GrAES128_NO_PADDING;	
		//memset(A_PtrInfo->IV, 0, E_GrAES128_BLOCK_LEN);		
	
		//GrAES128_EncKeySch
		Tv_PtrCphrKey=(Ptr_GrAES128_CphrKey)A_PtrInfo->RoundKey;
		e_key=Tv_PtrCphrKey->RK;
	
		if( (A_UserKeyLen!=16) && (A_UserKeyLen!=24) && (A_UserKeyLen!=32) )
		{
			return E_GrAES128_INVALID_USERKEYLEN;
		}
		
		k_len = (A_UserKeyLen + 3) / 4;
		Tv_PtrCphrKey->k_len = k_len;

		GrAES128KeySchedule(A_UserKey, k_len, e_key);
	
		//EncInit
		A_PtrInfo->BufLen = 0;
		if( A_PtrInfo->ModeID!=E_GrAES128_ECB )
		{
			memcpy(A_PtrInfo->ChainVar, A_PtrInfo->IV, E_GrAES128_BLOCK_LEN);
		}
		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32 GrAES128_EncUpdate( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Plaintxt, __u32 A_PlaintxtLen,		__u8 *A_Ciphertxt, __u32 *A_CiphertxtLen )
{
	// local -------------------
		__u32 *ScheduledKey;
		__u32 BlockLen;
		__u32 BufLen;
	// code --------------------
		ScheduledKey = A_PtrInfo->RoundKey;
		BlockLen = E_GrAES128_BLOCK_LEN;
		BufLen = A_PtrInfo->BufLen;
	
		*A_CiphertxtLen = BufLen + A_PlaintxtLen;

		if( *A_CiphertxtLen<BlockLen )
		{
			memcpy(A_PtrInfo->Buffer+BufLen, A_Plaintxt, (__s32)A_PlaintxtLen);
			A_PtrInfo->BufLen += A_PlaintxtLen;
			*A_CiphertxtLen = 0;
			return E_GrAES128_Success;
		}

		if( A_Plaintxt==A_Ciphertxt )
		{
			return E_GrAES128_FATAL_ERROR;
		}
		memcpy(A_PtrInfo->Buffer+BufLen, A_Plaintxt, (__s32)(BlockLen - BufLen));
		A_Plaintxt += BlockLen - BufLen;
		A_PlaintxtLen -= BlockLen - BufLen;
		
		if( E_GrAES128_CBC == A_PtrInfo->ModeID )
		{
			BlockXor(A_Ciphertxt, A_PtrInfo->ChainVar, A_PtrInfo->Buffer);
			GrAES128_Encrypt(ScheduledKey, A_Ciphertxt);
			A_Ciphertxt += BlockLen;
			while( A_PlaintxtLen>=BlockLen )
			{
				BlockXor(A_Ciphertxt, A_Ciphertxt-BlockLen, A_Plaintxt);
				GrAES128_Encrypt(ScheduledKey, A_Ciphertxt);
				A_Plaintxt += BlockLen;
				A_Ciphertxt += BlockLen;
				A_PlaintxtLen -= BlockLen;
			}
			BlockCopy( A_PtrInfo->ChainVar, A_Ciphertxt-BlockLen );
		}
		else
		{
			BlockCopy(A_Ciphertxt, A_PtrInfo->Buffer);
			GrAES128_Encrypt(ScheduledKey, A_Ciphertxt);
			A_Ciphertxt += BlockLen;
			while( A_PlaintxtLen>=BlockLen )
			{
				BlockCopy(A_Ciphertxt, A_Plaintxt);
				GrAES128_Encrypt(ScheduledKey, A_Ciphertxt);
				A_Plaintxt += BlockLen;
				A_Ciphertxt += BlockLen;
				A_PlaintxtLen -= BlockLen;
			}
		}

		memcpy(A_PtrInfo->Buffer, A_Plaintxt, (__s32)A_PlaintxtLen);
		A_PtrInfo->BufLen = A_PlaintxtLen;
		*A_CiphertxtLen -= A_PlaintxtLen;

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32 GrAES128_EncFinal( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Ciphertxt, __u32 *A_CiphertxtLen )
{
	// local -------------------
		__u32		*ScheduledKey;
		__u32		BlockLen;
		__u32		BufLen;
		__u32		PaddByte;
	// code --------------------	
		ScheduledKey = A_PtrInfo->RoundKey;
		BlockLen     = E_GrAES128_BLOCK_LEN;
		BufLen       = A_PtrInfo->BufLen;

		PaddByte = GrAES128PadSet(A_PtrInfo->Buffer, BufLen, BlockLen, A_PtrInfo->PadType);
		if( PaddByte>BlockLen )
		{
			return PaddByte;
		}

		if( PaddByte==0 )
		{
			*A_CiphertxtLen = 0;
			return E_GrAES128_Success;
		}
		if( E_GrAES128_CBC == A_PtrInfo->ModeID )
		{
			BlockXor( A_Ciphertxt, A_PtrInfo->Buffer, A_PtrInfo->ChainVar );
			GrAES128_Encrypt( ScheduledKey, A_Ciphertxt );
			BlockCopy(A_PtrInfo->ChainVar, A_Ciphertxt );
		}
		else
		{
			BlockCopy(A_Ciphertxt, A_PtrInfo->Buffer);
			GrAES128_Encrypt(ScheduledKey, A_Ciphertxt);
		}
		
		*A_CiphertxtLen = BlockLen;

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32	GrAES128_DecInit( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info A_PtrInfo )
{
	// local -------------------	
		Ptr_GrAES128_CphrKey	Tv_PtrCphrKey;	
		__u32	*d_key;
		__u32	k_len, t_key[64];
		__u32	i, t, u, v, w;
	// code --------------------	
		//GrAES128Set
		A_PtrInfo->ModeID = E_GrAES128_CBC;
		A_PtrInfo->PadType = E_GrAES128_NO_PADDING;	
		//memset(A_PtrInfo->IV, 0, E_GrAES128_BLOCK_LEN);
	
		//GrAES128_DecKeySch
		Tv_PtrCphrKey=(Ptr_GrAES128_CphrKey ) A_PtrInfo->RoundKey;
		d_key=Tv_PtrCphrKey->RK;
	
		if( (A_UserKeyLen!=16) && (A_UserKeyLen!=24) && (A_UserKeyLen!=32) )
		{
			return E_GrAES128_INVALID_USERKEYLEN;
		}

		k_len = (A_UserKeyLen + 3) / 4;
		Tv_PtrCphrKey->k_len = k_len;

		GrAES128KeySchedule(A_UserKey, k_len, t_key);

		d_key[0] = t_key[4 * k_len + 24];
		d_key[1] = t_key[4 * k_len + 25];
		d_key[2] = t_key[4 * k_len + 26];
		d_key[3] = t_key[4 * k_len + 27];

		for( i=4; i<4*(k_len+6); i+=4) 
		{
			imix_col(d_key[i+0], t_key[4*k_len+24-i+0]);
			imix_col(d_key[i+1], t_key[4*k_len+24-i+1]);
			imix_col(d_key[i+2], t_key[4*k_len+24-i+2]);
			imix_col(d_key[i+3], t_key[4*k_len+24-i+3]);
		}
		d_key[i+0] = t_key[4*k_len+24-i+0];
		d_key[i+1] = t_key[4*k_len+24-i+1];
		d_key[i+2] = t_key[4*k_len+24-i+2];
		d_key[i+3] = t_key[4*k_len+24-i+3];
	
		//DecInit
		A_PtrInfo->BufLen = 0;
		if( A_PtrInfo->ModeID!=E_GrAES128_ECB )
		{
			memcpy(A_PtrInfo->ChainVar, A_PtrInfo->IV, E_GrAES128_BLOCK_LEN);
		}
		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32 GrAES128_DecUpdate( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Ciphertxt, __u32 A_CiphertxtLen,	__u8 *A_Plaintxt, __u32 *A_PlaintxtLen )
{
	// local -------------------	
		__u32		*ScheduledKey;
		__u32		BlockLen;
		__u32		BufLen;
	// code --------------------
		BufLen=A_PtrInfo->BufLen;
		BlockLen=E_GrAES128_BLOCK_LEN;
		ScheduledKey=A_PtrInfo->RoundKey;
	
		*A_PlaintxtLen = BufLen + A_CiphertxtLen;

		if( BufLen+A_CiphertxtLen <= BlockLen )
		{
			memcpy(A_PtrInfo->Buffer+BufLen, A_Ciphertxt, (__s32)A_CiphertxtLen);
			A_PtrInfo->BufLen += A_CiphertxtLen;
			*A_PlaintxtLen = 0;
			return E_GrAES128_Success;
		}

		if( A_Ciphertxt==A_Plaintxt )
		{	
			return E_GrAES128_FATAL_ERROR;
		}

		*A_PlaintxtLen = BufLen + A_CiphertxtLen;
		memcpy(A_PtrInfo->Buffer+BufLen, A_Ciphertxt, (__s32)(BlockLen - BufLen));
		A_Ciphertxt += BlockLen - BufLen;
		A_CiphertxtLen -= BlockLen - BufLen;

		BlockCopy(A_Plaintxt, A_PtrInfo->Buffer);
		GrAES128_Decrypt(ScheduledKey, A_Plaintxt);
		BlockXor(A_Plaintxt, A_Plaintxt, A_PtrInfo->ChainVar);
		A_Plaintxt += BlockLen;

		if(A_CiphertxtLen <= BlockLen)
		{
			BlockCopy(A_PtrInfo->ChainVar, A_PtrInfo->Buffer);
		}
		else
		{
			if(A_CiphertxtLen > BlockLen)
			{
				BlockCopy(A_Plaintxt, A_Ciphertxt);
				GrAES128_Decrypt(ScheduledKey, A_Plaintxt);
				BlockXor(A_Plaintxt, A_Plaintxt, A_PtrInfo->Buffer);
				A_Ciphertxt += BlockLen;
				A_Plaintxt += BlockLen;
				A_CiphertxtLen -= BlockLen;
			}
			while(A_CiphertxtLen>BlockLen)
			{
				BlockCopy(A_Plaintxt, A_Ciphertxt);
				GrAES128_Decrypt(ScheduledKey, A_Plaintxt);
				BlockXor(A_Plaintxt, A_Plaintxt, A_Ciphertxt-BlockLen);
				A_Ciphertxt += BlockLen;
				A_Plaintxt += BlockLen;
				A_CiphertxtLen -= BlockLen;
			}
			BlockCopy(A_PtrInfo->ChainVar, A_Ciphertxt-BlockLen);
		}
	
		//save remained data
		memcpy(A_PtrInfo->Buffer, A_Ciphertxt, (__s32)A_CiphertxtLen);
		A_PtrInfo->BufLen = (A_PtrInfo->BufLen&0xF0000000) + A_CiphertxtLen;
		*A_PlaintxtLen -= A_CiphertxtLen;

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
__u32 GrAES128_DecFinal( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Plaintxt, __u32 *A_PlaintxtLen )
{
	// local -------------------
		__u32 *ScheduledKey;
		__u32 BlockLen;
		__u32 BufLen;
		__u32 ret;
	// code --------------------	
		ScheduledKey = A_PtrInfo->RoundKey;
		BlockLen = E_GrAES128_BLOCK_LEN;
		BufLen = A_PtrInfo->BufLen;

		if( BufLen==0 )
		{
			*A_PlaintxtLen = 0;
			return E_GrAES128_Success;
		}
		*A_PlaintxtLen = BlockLen;

		if( BufLen!=BlockLen )
		{
			return E_GrAES128_CIPHER_LEN_ERROR;
		}

		/*BlockCopy(A_Plaintxt, A_PtrInfo->Buffer);
		GrAES128_Decrypt(ScheduledKey, A_Plaintxt);

		ret = GrAES128PadChck(A_Plaintxt, BlockLen, A_PtrInfo->PadType);
		if( ret==(__u32)-3 )	return E_GrAES128_PAD_CHECK_ERROR;
		if( ret==(__u32)-1 )	return E_GrAES128_FATAL_ERROR;

		*A_PlaintxtLen = BlockLen - ret;

		return E_GrAES128_Success;*/

		BlockCopy(A_Plaintxt, A_PtrInfo->Buffer);
		GrAES128_Decrypt(ScheduledKey, A_Plaintxt);
		BlockXor(A_Plaintxt, A_Plaintxt, A_PtrInfo->ChainVar);
		BlockCopy(A_PtrInfo->ChainVar, A_PtrInfo->Buffer);

		ret = GrAES128PadChck(A_Plaintxt, BlockLen, A_PtrInfo->PadType);
		if( ret==(__u32)-3 )	return E_GrAES128_PAD_CHECK_ERROR;
		if( ret==(__u32)-1 )	return E_GrAES128_FATAL_ERROR;

		*A_PlaintxtLen = BlockLen - ret;

		return E_GrAES128_Success;
}
//--------------------------------------------------------------------
