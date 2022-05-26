/*
	audio base
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrAdoBase
#define	_EPC_GrAdoBase

//====================================================================
// uses

#include	<GrTypeBase.h>
	
//====================================================================
//constance

//fcc audio
#define E_GrFccPcm16		0x464D4350					/*	16bit normal pcm	*/
#define E_GrFccPcm8			0x384D4350					/*	8bit normal pcm	*/
#define E_GrFccUlaw			0x57414C55					/*	u-law */
#define E_GrFccAlaw			0x57414C41					/*	a-law */
#define E_GrFccG723_24	0x41333237					/*  g723 24bit mpc mlq */

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class

class Cls_GrAdoDec
{
	protected:
		__u32		m_Fcc;
		BOOL8		m_IsSrcEmpty;
		BOOL8		m_IsDstBufFull;
	public:
		Cls_GrAdoDec();
		virtual	~Cls_GrAdoDec();

		//info
		virtual	__u32		GetFcc( void );
		virtual	BOOL8		IsSrcEmpty( void );
		virtual	BOOL8		IsDstBufFull( void );

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
//out Enviroment conditional
#endif
