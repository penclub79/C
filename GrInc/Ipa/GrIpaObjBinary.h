/*
	Gause Image process object binary 2D stream
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjBinary
#define	_EPC_GrIpaObjBinary

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define	E_GrIpaObjBinaryConsentCnt			1
#define	E_GrIpaObjBinaryPlugCnt					1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjBinaryPara
{
	__u32	Radius;
}	*Ptr_GrIpaObjBinaryPara;


//====================================================================
//global class
class Cls_GrIpaObjBinary : public	Cls_GrIpaObjBase
{
	protected:
		St_GrIpaObjBinaryPara	m_ParaBinary;

		St_GrIpaStrmInfo2d	m_StrmInfo;
		__u32	m_StrmSize;
		void*	m_PtrStrmPlug;

		__u8	LcAvrLocalExcptMe( void* A_PtrStrm, __u32 A_X, __u32 A_Y, __u32 A_Radius );

		void	LcDoLbp( void* A_PtrStrm );

		void	LcBufFree( void );

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjBinary( void );
		virtual	~Cls_GrIpaObjBinary( void );

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

