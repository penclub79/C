/*
	Gause Image process object bind YUV
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjBindYuv
#define	_EPC_GrIpaObjBindYuv

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define	E_GrIpaObjBindYuvConsentCnt			3
#define	E_GrIpaObjBindYuvPlugCnt				1

#define	E_GrIpaObjBindYuvConsentY				0
#define	E_GrIpaObjBindYuvConsentU				1
#define	E_GrIpaObjBindYuvConsentV				2

#define	E_GrIpaObjBindYuvPlugImgFcc			E_GrFccRGB24

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjBindYuvPara
{
	BOOL8	IsFix[E_GrIpaObjBindYuvConsentCnt];
	__u8	FixVal[E_GrIpaObjBindYuvConsentCnt];

	__u8	_rsvd0[2];
}	*Ptr_GrIpaObjBindYuvPara;

typedef struct St_GrIpaObjBindYuvConsentCtrl
{
	St_GrIpaStrmInfo2d	StrmInfo;
	void*	PtrStrm;
}	*Ptr_GrIpaObjBindYuvConsentCtrl;

//====================================================================
//global class
class Cls_GrIpaObjBindYuv : public	Cls_GrIpaObjBase
{
	protected:
		St_GrIpaObjBindYuvPara	m_ParaBindYuv;

		St_GrIpaObjBindYuvConsentCtrl	m_ConsentCtrlTbl[E_GrIpaObjBindYuvConsentCnt];
		BOOL8	m_IsBindAble;

		St_GrIpaStrmInfoImg	m_PlugStrmInfo;
		__u32	m_PlugStrmSize;
		Cls_GrGdib*	m_GdibPlug;


		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

		void	LcBindBufFree( void );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjBindYuv( void );
		virtual	~Cls_GrIpaObjBindYuv( void );

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

