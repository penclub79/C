/*
	Gause Image process object base
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjBase
#define	_EPC_GrIpaObjBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Ipa/GrIpaBase.h>

//====================================================================
//constance
#define	E_GrIpaObjConsentMaxCnt			4
#define	E_GrIpaObjPlugMaxCnt				4

#ifdef GR_IPA_EDIT

#define	E_GrIpaObjExtraDataSize			64

#endif

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjBindInfo
{
	__u32		BindIdx;
	void*		Adaptor;
}	*Ptr_GrIpaObjBindInfo;


//====================================================================
//global class
class	Cls_GrIpaObjBase
{
	protected:
		__u16	m_AdaptorType;		// adaptor type

		__u16	m_ScptId;					// script id

		BOOL8	m_IsSource;
		BOOL8	m_IsMonitor;

		__u32	m_ConsentCnt;
		St_GrIpaObjBindInfo	m_ConsentBindTbl[E_GrIpaObjConsentMaxCnt];

		__u32	m_PlugCnt;
		St_GrIpaObjBindInfo	m_PlugBindTbl[E_GrIpaObjPlugMaxCnt];

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		// encode
		__u32	m_EncPrgs;				// encode progress
		void*	m_PtrEnc;					// encode pointer

		__u8	m_ExtraData[E_GrIpaObjExtraDataSize];

		// para info
		void*	m_PtrParaInfo;
		__u32	m_ParaInfoPrgs;
#endif

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		void		LcParaInfoAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjBase( void );
		virtual	~Cls_GrIpaObjBase( void );

		__u16	AdaptorType( void );
		__u16	ScptIdGet( void );
		void	ScptIdSet( __u16 A_Id );

		__u32	ConsentCount( void );
		Cls_GrIpaObjBase*	ConsentGetConnObj( __u32 A_ConsentIdx, __u32* A_PtrRtPlugIdx );

		__u32	PlugCount( void );
		Cls_GrIpaObjBase*	PlugGetConnObj( __u32 A_PlugIdx, __u32* A_PtrRtConsentIdx );
		virtual	void*	PlugStrmInfoGet( __u32 A_PlugIdx, __u32* A_PtrRtStrmType, __u32* A_PtrRtSize );

		BOOL8		ChainPlug( __u32 A_PlugIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_ConsentIdx );
		BOOL8		ChainConsent( __u32 A_ConsentIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_PlugIdx );

		BOOL8		UnchainPlug( __u32 A_PlugIdx );
		BOOL8		UnchainConsent( __u32 A_ConsIdx );

		virtual	void	ResetProcess( void );				// process reset
		virtual	void	ReleaseConnection( void );	// release all connection

		// runtime
		virtual	BOOL8	RtlChainPlug( __u32 A_PlugIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_ConsentIdx );
		virtual	BOOL8	RtlChainConsent( __u32 A_ConsentIdx, Cls_GrIpaObjBase* A_Adaptor, __u32 A_PlugIdx );

		virtual	BOOL8	RtlUnchainPlug( __u32 A_PlugIdx );
		virtual	BOOL8	RtlUnchainConsent( __u32 A_ConsIdx );

		virtual	BOOL8	RtlSetupSource( void );
		virtual	BOOL8	RtlSetupPlug( __u32 A_PlugIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize );
		virtual	BOOL8	RtlSetupConsent( __u32 A_ConsentIdx, __u32 A_StrmType, void* A_PtrStrmInfo, __u32 A_StrmInfoSize );

		virtual	BOOL8	RtlProcessSource( void );
		virtual	BOOL8	RtlPlugSend( __u32 A_PlugIdx, void* A_PtrStrm, __u32 A_StrmSize );
		virtual	BOOL8	RtlConsentRecv( __u32 A_ConsentIdx, void* A_PtrStrm, __u32 A_StrmSize );


		virtual	void	RtlScptUpdated( void );		// script parameter update finish
		BOOL8		RtlParaDec( void* A_PtrScpt, __u32 A_Size );	// decode parameter
		void		RtlChainDec( void* A_PtrScpt, __u32 A_Size );	// decode chain

#ifdef GR_IPA_EDIT
		__u32	ParaEnc( void* A_PtrBuf );
		__u32	ChainEnc( void* A_PtrBuf );
		WCHAR*	AdaptorName( void );
		void*	ExtraDataGet( void );

		__u32	ParaInfoDump( void* A_PtrBuf );
#endif


};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

