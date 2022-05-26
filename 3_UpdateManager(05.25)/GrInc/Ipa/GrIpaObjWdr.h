/*
	Gause Image process object wide dynamic range
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjWdr
#define	_EPC_GrIpaObjWdr

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define	E_GrIpaObjWdrConsentCnt				1
#define	E_GrIpaObjWdrPlugCnt					1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjWdrPara
{
	__u8	Basis;
	__u8	Velocity;
}	*Ptr_GrIpaObjWdrPara;

typedef struct St_GrIpaObjWdrClrItem
{
	__u8	ClrNew;
	__u32	PxCnt;
}	*Ptr_GrIpaObjWdrClrItem;

typedef struct St_GrIpaWdrHstgItem
{
	__u32	PxCnt;
	Ptr_GrIpaObjWdrClrItem	PtrClrItm;
}	*Ptr_GrIpaWdrHstgItem;

//====================================================================
//global class
class Cls_GrIpaObjWdr : public	Cls_GrIpaObjBase
{
	protected:
		St_GrIpaObjWdrPara	m_ParaWdr;

		St_GrIpaStrmInfo2d	m_StrmInfo;
		__u32	m_StrmSize;
		void*	m_PtrStrmPlug;

		St_GrIpaWdrHstgItem	m_HstgTbl[256];

		St_GrIpaObjWdrClrItem		m_ClrTbl[256];
		__u32	m_ClrCnt;


		void	LcHstgBuild(void* A_PtrStrm);		// histogram build
		void	LcClrTblBld(__u32 A_Tolerance);

		void	LcQuantize(void);
		void	LcNormalize(void);
		void	LcRemapClr(void* A_PtrStrm);

		void	LcDoWdr( void* A_PtrStrm );

		void	LcBufFree( void );

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjWdr( void );
		virtual	~Cls_GrIpaObjWdr( void );

		// exclusive 

		// runtime
		virtual	BOOL8	RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize );
		virtual	BOOL8	RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

