/*
	Gause Image process object monitor out
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjMonOut
#define	_EPC_GrIpaObjMonOut

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjMonOutPara
{
	__u32	MonIdx;			// monitor index
}	*Ptr_GrIpaObjMonOutPara;


//====================================================================
//global class
class Cls_GrIpaObjMonOut : public	Cls_GrIpaObjBase
{
	protected:

		St_GrIpaObjMonOutPara		m_ParaMonOut;

		BOOL8	m_IsNeedCvt;			// convert need
		Cls_GrGdib*		m_GdibSr;	// source GDIB

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjMonOut( void );
		virtual	~Cls_GrIpaObjMonOut( void );

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

