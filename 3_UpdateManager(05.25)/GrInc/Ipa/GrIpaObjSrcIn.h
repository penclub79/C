/*
	Gause Image process object source image
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaObjSrcIn
#define	_EPC_GrIpaObjSrcIn

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrGdibTool.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define		E_GrIpaObjSrcInPlugCnt				1

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaObjSrcInPara
{
	__u32	SrcId;		// source ID
}	*Ptr_GrIpaObjSrcInPara;

typedef struct St_GrIpaObjSrcInPlugCtrl
{
	St_GrIpaStrmInfoImg	StrmInfo;
}	*Ptr_GrIpaObjSrcInPlugCtrl;

//====================================================================
//global class
class Cls_GrIpaObjSrcIn : public	Cls_GrIpaObjBase
{
	protected:
		St_GrIpaObjSrcInPara	m_ParaSrcIn;

		__u32	m_StrmType;
		void*	m_PtrStrmInfo;
		__u32	m_StrmInfoSize;

		Cls_GrGdib*		m_ObjGdib;

		virtual	BOOL8	LcParaSubDec( __u16 A_Type, void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		virtual	void	LcParaSubEnc( void );
		virtual	void	LcParaInfoSubDump( void );
#endif

	public:
		Cls_GrIpaObjSrcIn( void );
		virtual	~Cls_GrIpaObjSrcIn( void );

		virtual	void*	PlugStrmInfoGet( __u32 A_PlugIdx, __u32* A_PtrRtStrmType, __u32* A_PtrRtSize );


		// exclusive 

		// runtime
		virtual	BOOL8	RtlSetupSource( void );
		virtual	BOOL8	RtlProcessSource( void );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

