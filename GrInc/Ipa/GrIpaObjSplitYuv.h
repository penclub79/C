/*
	Gause Image process object split YUV
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjSplitYuv
#define	_EPC_GrIpaObjSplitYuv

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define		E_GrIpaObjSplitYuvConsentCnt		1
#define		E_GrIpaObjSplitYuvPlugCnt				3

#define		E_GrIpaObjSplitYuvPlugY					0
#define		E_GrIpaObjSplitYuvPlugU					1
#define		E_GrIpaObjSplitYuvPlugV					2

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjSplitYuvPara
{
	__u32	_rsvd0;
}	*Ptr_GrIpaObjSplitYuvPara;

typedef struct St_GrIpaObjSplitYuvPlugCtrl
{
	void*	PtrBuf;
}	*Ptr_GrIpaObjSplitYuvPlugCtrl;

//====================================================================
//global class
class Cls_GrIpaObjSplitYuv : public	Cls_GrIpaObjBase
{
	protected:
		St_GrIpaObjSplitYuvPara	m_ParaSplitYuv;

		St_GrGdib		m_GdibConsent;

		St_GrIpaObjSplitYuvPlugCtrl	m_PlugCtrlTbl[E_GrIpaObjSplitYuvPlugCnt];
		St_GrIpaStrmInfo2d	m_PlugStrmInfo;
		__u32	m_PlugStrmSize;

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

		void	LcSplitBufFree( void );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjSplitYuv( void );
		virtual	~Cls_GrIpaObjSplitYuv( void );

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

