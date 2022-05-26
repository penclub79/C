/*
	linux pci tool

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrLnxPci
#define	_EPC_GrLnxPci

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

//====================================================================
//constance
#define E_GrLnxPciBadHndl			0
#define E_GrLnxPciMaxDevCnt		16
#define E_GrLnxPciMaxRsrcCnt	7

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct St_GrLnxPciInfo 
{
	__u32		Vpid;		//vendor id  and product id
	__u32		Irq;
	__u32		Rsrc[E_GrLnxPciMaxRsrcCnt];
} *Ptr_GrLnxPciInfo;

typedef	struct St_GrLnxPciFind 
{
	__u32		Vpid;				//find vpid
	__u32		FindIdx;		//initialize = 0
}	*Ptr_GrLnxPciFind;

//====================================================================
//class

class Cls_GrLnxPci
{
	private:
		__u32		m_DevCnt;
		St_GrLnxPciInfo	m_DevTbl[E_GrLnxPciMaxDevCnt];
	protected:
		void		DevsPars( void* A_PtrBuf );
		void		PrsLine( __s8* A_Str );
	public:
		Cls_GrLnxPci();
		virtual	~Cls_GrLnxPci();

		__u32	FindDevFirst( __u32 A_Vid, __u32 A_Pid, Ptr_GrLnxPciFind A_PtrFind );
		__u32	FindDevContinue( Ptr_GrLnxPciFind A_PtrFind );

		__u32	GetListFirst( Ptr_GrLnxPciFind A_PtrFind );
		__u32	GetListContinue( Ptr_GrLnxPciFind A_PtrFind );

		__u32	GetDevCnt( void );			//device all count

		__u32	IrqGet( __u32 A_Hndl );
		__u32	RsrcGet( __u32 A_Hndl, __u32 A_Idx, BOOL8* A_PtrRtIsIo );
		__u32	VidGet( __u32 A_Hndl );
		__u32	PidGet( __u32 A_Hndl );
};

//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

