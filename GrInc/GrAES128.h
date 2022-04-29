/*
AES128 
Key Length 16byte
ECB Mode
No Padding ( only 64byte will use , PlainText = 64byte )

Plaintext -> Encrypt -> Ciphertext
Ciphertext -> Decrypt -> Plaintext
*/
//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrAES128
#define	_EPC_GrAES128

#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#ifdef LINUX_APP
#include	<stdlib.h>
#include	<string.h>
#include	<memory.h>
#endif

//====================================================================
#define LARGE_TABLES
//====================================================================
//const
#define E_GrAES128_ECB		1
#define E_GrAES128_CBC		2

#define E_GrAES128_NO_PADDING			1
#define E_GrAES128_PKCS_PADDING			2

#define E_GrAES128_BLOCK_LEN			16
#define E_GrAES128_NO_ROUNDKEY			68

#define E_GrAES128_Success					0
#define E_GrAES128_FATAL_ERROR				0x1001
#define E_GrAES128_INVALID_USERKEYLEN		0x1002
#define E_GrAES128_PAD_CHECK_ERROR			0x1003
#define E_GrAES128_DATA_LEN_ERROR			0x1004
#define E_GrAES128_CIPHER_LEN_ERROR			0x1005
//====================================================================
//type
typedef struct St_GrAES128Info
{
	__u32		ModeID;									//	ECB or CBC
	__u32		PadType;								//	Padding type
	__u8		IV[E_GrAES128_BLOCK_LEN];			//	Initial Vector
	__u8		ChainVar[E_GrAES128_BLOCK_LEN];		//	Chaining Variable
	__u8		Buffer[E_GrAES128_BLOCK_LEN];		//	Buffer for unfilled block
	__u32		BufLen; 								//	Buffer length
	__u32		RoundKey[E_GrAES128_NO_ROUNDKEY];	//	number of round key
} *Ptr_GrAES128Info;


typedef struct V_GrAES128_CphrKey
{
	__u32	k_len;
	__u32	RK[64];
} *Ptr_GrAES128_CphrKey;


typedef struct GrAES128_test
{
	__u32		UserKeyLngth;
	__u8		UserKey[16];
	__u32		PlaintTextLngth;
	__u8		PlaintText[64];
	__u32		CipherTextLngth;
	__u8		CipherText[64];
} *Ptr_GrAES128_test;

//====================================================================
//function


__u32 GrAES128_EncInit( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info	A_PtrInfo );
__u32 GrAES128_EncUpdate( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Plaintxt, __u32 A_PlaintxtLen, __u8 *A_Ciphertxt, __u32 *A_CiphertxtLen );
__u32 GrAES128_EncFinal( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Ciphertxt, __u32 *A_CiphertxtLen );

__u32 GrAES128_DecInit( __u8 *A_UserKey, __u32 A_UserKeyLen, Ptr_GrAES128Info A_PtrInfo );
__u32 GrAES128_DecUpdate( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Ciphertxt, __u32 A_CiphertxtLen,	__u8 *A_Plaintxt, __u32 *A_PlaintxtLen );
__u32 GrAES128_DecFinal( Ptr_GrAES128Info A_PtrInfo, __u8 *A_Plaintxt, __u32 *A_PlaintxtLen );

//====================================================================
//out Enviroment conditional
#endif
