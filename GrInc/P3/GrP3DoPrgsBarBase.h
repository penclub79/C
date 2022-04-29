/*
	Gause Platform progressbar base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoPrgsBarBase
#define	_EPC_GrP3DoPrgsBarBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoPrgsBarBasePara
{
	__s16	GrpLeft;						// graph area left
	__s16	GrpTop;							// graph area top
	__s16	GrpRight;						// graph area right
	__s16	GrpBottom;					// graph area bottom
	__u8	GrpPosBss;					// graph position basis
	BOOL8	IsVertical;					// vertical type
	__u16	PntBg;							// background paint code
	__u16	PntGrp;							// graph paint code
	BOOL8	IsReverse;					// direction reverse
	__u8	_rsvd0;							
	__s32		ValMin;
	__s32		ValMax;
}	*Ptr_GrP3DoPrgsBarBasePara;

//====================================================================
//global class
class	Cls_GrP3DoPrgsBarBase	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoPrgsBarBasePara	m_ParaDoPrgsBarBase;

		__s32		m_Value;			// last value
		Def_WstrNum	m_StrPrgs;	// progress

		// local
		virtual	void	LcValLoad( void );
		virtual	void	LcValSave( void );

		void	LcPrgsStrUpdt( void );		// progress string update

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );			// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoPrgsBarBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoPrgsBarBase( void );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

