/*
	Gause Image process manager
	
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrIpaMng
#define	_EPC_GrIpaMng

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <Ipa/GrIpaBase.h>
#include <Ipa/GrIpaObjBase.h>

//====================================================================
//constance

#define	E_GrIpaMngMaxObjCnt				256

#define	E_GrIpaMngCmdBufSize			0x10000

// command
#define	E_GrIpaMngCmdSetup				1		/* setup	-	St_GrIpaMngCmdSetup */
#define	E_GrIpaMngCmdProcess			2		/* process stream	-	St_GrIpaMngCmdProcess */

//====================================================================
//global macro

//====================================================================
//global type

typedef struct St_GrIpaMngCmdBase
{
	__u16	Cmd;
	__u16	_rsvd0;
}	*Ptr_GrIpaMngCmdBase;

typedef struct St_GrIpaMngCmdSetup
{
	St_GrIpaMngCmdBase	CmdHd;
	__u16	ScptId;
	__u16	_rsvd0;
	__u32	PlugIdx;
	__u32	StrmType;
	void*	PtrStrmInfo;
	__u32	StrmInfoSize;
}	*Ptr_GrIpaMngCmdSetup;

typedef struct St_GrIpaMngCmdProcess
{
	St_GrIpaMngCmdBase	CmdHd;
	__u16	ScptId;
	__u16	_rsvd0;
	__u32	PlugIdx;
	void*	PtrStrm;
	__u32	StrmSize;
}	*Ptr_GrIpaMngCmdProcess;


//====================================================================
//global class
class	Cls_GrIpaMng
{
	protected:
		Cls_GrIpaObjBase*	m_ObjTbl[E_GrIpaMngMaxObjCnt];

		__u8	m_CmdBuf[E_GrIpaMngCmdBufSize];

#ifdef GR_IPA_EDIT
		void*		m_PtrScptEnc;
		__u32		m_ScptEncPrgs;
#endif

		Cls_GrIpaObjBase*	LcObjCreate( __u16 A_Type );
		Cls_GrIpaObjBase*	LcObjByScptId( __u16 A_Id );

		BOOL8	LcChainAdaptor( void );		// adaptor chain
		BOOL8	LcSetupAdaptor( void );		// adaptor setup
		BOOL8	LcProcess( void );

		BOOL8	LcDoCmd( Ptr_GrIpaMngCmdBase A_PtrCmd );
		void	LcPcmdSetup( Ptr_GrIpaMngCmdSetup A_PtrCmd );
		void	LcPcmdProcess( Ptr_GrIpaMngCmdProcess A_PtrCmd );

		virtual	BOOL8	LcSectionDecode( __u8 A_Type, __u8 A_Para, void* A_PtrBuf, __u32 A_Size );
		BOOL8	LcScptParaDec( void* A_PtrScpt, __u32 A_ScptSize );
		BOOL8	LcScptChainDec( void* A_PtrScpt, __u32 A_ScptSize );

#ifdef GR_IPA_EDIT
		void	LcScptEncObjPara( void );
		void	LcScptEncObjChain( void );
#endif

	public:
		Cls_GrIpaMng( void );
		virtual	~Cls_GrIpaMng( void );

		Cls_GrIpaObjBase*	ObjCreate( __u16 A_Type, __u16 A_ScptId );
		
		Cls_GrIpaObjBase*	ObjGet( __u16 A_ScptId );

		BOOL8	CommandInput( Ptr_GrIpaMngCmdBase A_PtrCmd, __u32 A_CmdSize );
		BOOL8	CommandOnePump( void );		// one command pump

		void	Reset( void );
		BOOL8	ScriptDecode( void* A_PtrBuf, __u32 A_Size );

#ifdef GR_IPA_EDIT
		__u32	ScriptEncode( void* A_PtrBuf );
#endif

		virtual	void*	SourceInfoGet( __u32 A_SourceId, __u32* A_PtrRtStrmType, __u32* A_PtrRtInfoSize );
		virtual	void*	SourceStrmGet( __u32 A_SourceId, __u32* A_PtrRtStrmSize );
		virtual	BOOL8	MonitorSetup( __u32 A_MonId, __u32 A_StrmTYpe, void* A_PtrInfo, __u32 A_InfoSize );
		virtual	BOOL8	MonitorOut( __u32 A_MonId, void* A_PtrStrm, __u32 A_StrmSize );

		virtual	BOOL8	Run( void );
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

