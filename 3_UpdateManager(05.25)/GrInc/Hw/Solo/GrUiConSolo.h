/*
	solo ui console 
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrUiConSolo
#define	_EPC_GrUiConSolo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Ui/GrUiConsole.h>
#include	<Hw/Solo/GrDevSolo.h>

//====================================================================
//constance

#define E_GrUiConSoloQdLnCntMd1			4	
#define E_GrUiConSoloQdLnCntMd4			6	
#define E_GrUiConSoloQdLnCntMd6			9
#define E_GrUiConSoloQdLnCntMd8			10
#define E_GrUiConSoloQdLnCntMd9			8
#define E_GrUiConSoloQdLnCntMd10		9
#define E_GrUiConSoloQdLnCntMd13		12
#define E_GrUiConSoloQdLnCntMd16		10

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//class

class Cls_GrUiConSolo	:	public Cls_GrUiConsole
{
	private:
		Cls_GrDevSolo*		m_ObjDev;
		BOOL8		m_IsHwGridShow;

		__u8		m_MaxVdoChCnt;
		__u8		m_QuadChMap[E_GrDvrQuadMapSize];		//quad channel map
		__u16		m_NoSigMap;
		__u8*		m_PtrPageCntTbl;
		__u8		m_QuadMaxMode;

		__u16		m_ChUseMap;
	protected:
		//hal
		virtual	BOOL8	HalPntReqTransfer( Ptr_GrRect A_PtrRect );			//request paint transfer - ni
		virtual	void	HalMouseCusOnOff( void );						//mouse cursor on / off status update

	public:
		Cls_GrUiConSolo( Cls_GrDevSolo* A_DevSolo );
		virtual	~Cls_GrUiConSolo();

		virtual	__u32		GetResY( void );

		virtual	void	MouseCusPosSet( __s32 A_X, __s32 A_Y );		//hw mouse cursor position set
		virtual	void	MouseCusImgSet( __u32 A_ImgIdx );			//hw cursor image set

		virtual	__u8	QuadViewIdxByPos( __s32 A_X, __s32 A_Y );
		virtual	__u8	QuadLineTblGet( Ptr_GrLinePos A_PtrRtBuf );
		virtual	BOOL8	QuadModeSet( __u8 A_Mode, __u8 A_Page, BOOL8 A_IsNotify = TRUE );		//quad mode set - inherite result true
		virtual	BOOL8	QuadRectGet( __u8 A_ViewIdx, Ptr_GrRect A_PtrRtRect );
		virtual	__u8	QuadGetChByView( __u8 A_View );
		virtual	__u8	QuadPageCntByMode( __u8 A_Mode );
		virtual	__u8	QuadMaxModeGet( void );		//max quad mode
		virtual	BOOL8	IsQuadHwGrid( void );		//hardware grid
		virtual	void	QuadGridShow( BOOL8 A_IsShow );
		virtual	void	CorvertMapSet( __u16 A_Map );	//implement value update only
		virtual	void	PipShow( __u8 A_Ch, __u8 A_Scale, __s32 A_X, __s32 A_Y );	//ni
		virtual	void	PipHide( void );	//ni
		virtual	void	PlayModeSet( BOOL8 A_IsPlay );
		virtual	__u16	NoSigMapGet( void );					//hal
		virtual	void	NoSigMapSet( __u16 A_Map );

		virtual	__u8	RecAtbGet( __u8 A_Ch );
		virtual	__u8	PlayAtbGet( __u8 A_Ch );

		virtual	__u16	ChUseMapGet( void );
		virtual	void	ChUseMapSet( __u16 A_Map );

		//exclusive api
		void*		QuadGetChMapPtr( void );
		__u32		QuadGetChMapSize( void );
		__u8*		QuadGetPageChMapPtr( __u8 A_Mode, __u8 A_Page );
};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
