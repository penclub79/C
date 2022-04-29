/*
	solo device
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDevSolo
#define	_EPC_GrDevSolo

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Hw/Solo/RegSolo.h>
#include	<Hw/Solo/GrDrvSolo.h>
#include	<GrGdibObj.h>
#include	<GrProcess.h>

//====================================================================
//constance

//#define E_GrDevSoloP2mEvtIdxOsgStart		0		/* osg transfer completed */
//#define E_GrDevSoloP2mEvtIdxOsgCnt			8		/* osg transfer event count */

#define	E_GrDevSoloVdecGap			0x40000

//====================================================================
//global macro


//====================================================================
//global type

typedef	struct St_GrDevSoloUartCh 
{
	St_GrPrcCritSct	CritRead;
	St_GrPrcCritSct	CritWrite;
}	*Ptr_GrDevSoloUartCh;

//====================================================================
//class

class Cls_GrDevSolo
{
	private:
		__s32			m_HndlDrv;			//driver handle
		__u8		m_DevIdx;				//device index
		__u32		m_PhysDrvMem;		//physical memory address driver memory
		__u32		m_PllMhz;				//pll value
		void*		m_PtrDrvMem;		//driver mapping pointer
		Ptr_GrDrvSoloMng	m_PtrDrvMng;

		Ptr_GrDrvSoloDevCtrl	m_PtrCtrl;		//contorl address

		//buffer
		void*		m_PtrBufMtnCfg;
		void*		m_PtrBufAdoDec;
		void*		m_PtrBufAdoSlc;
		void*		m_PtrBufEncOsd;
		void*		m_PtrBufNetAdo;
		void*		m_PtrBufJpegCap;
		void*		m_PtrBufImgCap;
		void*		m_PtrBufOsg;
		void*		m_PtrBufNetJpg;
		void*		m_PtrBufPlay;
		void*		m_PtrBufNetMpg;
		void*		m_PtrBufRec;

		//device info
		BOOL8		m_Is6110;				//solo 6110
		__u32		m_VdoChCnt;			//video channel count
		__u32		m_CodecPwr;			//codec power D1 unit
		BOOL8		m_IsIrqPump;		//interrupt pump mode
		__u32		m_AdoChCnt;			//audion encode channel count
		BOOL8		m_IsMtnMap;			//motion map adder
		__u32		m_VdoChMask;		//videochannel mask

		//video info
		__u32		m_VdoResX;			//base resolution
		__u32		m_VdoResY;			//base resolution
		__u32		m_VdoFps;				//video frame rate

		//control
		__u32		m_PlayChMap;		//play channel map		
		__u32		m_CovertMap;		//covert

		//osg
		Cls_GrGdib*	m_GdibOsg;

		//i2c
		St_GrPrcCritSct	m_I2cCrit;		//i2c critical section
		__u32		m_I2cTimeOut;		//time out tick unit

		//uart
		St_GrDevSoloUartCh	m_UartChTbl[E_SoloUartChCnt];

		//record
		__u32		m_RecMtnMap;
		__u32		m_RecDiMap;

		//play
		__u32		m_PlayDintlVal;
		BOOL8		m_IsPlaying;			//playing now

		//motion
		__u16		m_MtnSenTbl[E_SoloMaxChCnt];

		//runtime
		__u8		m_PlayAtb[E_SoloMaxChCnt];		//play attribute
		__u32		m_DecVreqId;		// video decode request id
		__u32		m_DecAreqId;		// audio decode request id

	protected:
		void	RegWrite( __u32 A_Adr, __u32 A_Val );
		__u32	RegRead( __u32 A_Adr );

		//local
		void	LcDevFinish( void );
		void	LcAvFinish( void );
		void	LcRecFinish( void );
		void	LcNetFinish( void );
		void	LcPumpIrq( void );

		//p2m
		BOOL8	P2mReqCopy(	__u32 A_Ch, void* A_PtrSys, __u32 A_PosLmem, __u32 A_Size, BOOL8 A_SysToLmem,
											__u8 A_ParaType, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2 );
		BOOL8	P2mReqBlt(	__u32 A_Ch, void* A_PtrSys, __u32 A_PosLmem, __u32 A_LineSize, __u32 A_LineCount, 
											BOOL8 A_SysToLmem, __u32 A_PitchSys, __u32 A_PitchLmem , 
											__u8 A_ParaType, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2 );

	public:
		Cls_GrDevSolo( __s32 A_HndlDrv, __u8 A_DevIdx, __u32 A_PhyDrvMem, void* A_PtrDrvMem, __u32 A_PllMhz );
		virtual	~Cls_GrDevSolo();

		//info
		__u32	VdoChCnt( void );		//video channel count
		__u32	AdoChCnt( void );		//audio channel count - value can read after AvConfig
		void*	RecBufPtrGet( void );			//record queue pointer get
		BOOL8	IsVdoPal( void );
		__u32	EncPowerGet( void );			//encode power
		__u32	DecPowerGet( void );			//encode power
		BOOL8	IsMtnMapGet( void );			//motion map mode 
		BOOL8	Is6110( void );						//solo 6110
		__u32	IrqOcuCountGet( void );		//interrupt occure count get
		void*	DevCtrlPtrGet( void );
		void*	DrvMemPtrGet( void );

		//config
		BOOL8	DevConfig( Ptr_GrDrvSoloBufCfg A_PtrCfgBuf, Ptr_GrDrvSoloGpioCfg A_PtrCfgGpio );		
		BOOL8	AvConfig( Ptr_GrDrvSoloAveCfg A_PtrCfgAv );			//av config
		BOOL8	RecConfig( Ptr_GrDrvSoloRecCfg A_PtrCfg );
		BOOL8	NetConfig( Ptr_GrDrvSoloNetCfg A_PtrCfg );
		BOOL8	RecChgPara( Ptr_GrDrvSoloRecCfg A_PtrCfg );

		//control
		void	IrqPumpMode( BOOL8 A_IsRun );
		void	TimeUpdt( void );				// time update
		void	TimeSync( void );				// time sync
		__u32	CovertMapGet( void );
		void	CovertMapSet( __u32 A_Map );
		void*	PosSysToPtr( __u32 A_PosSys );
		void	NosigMapSet( __u16 A_Map );
		void	CapBtwReset( void );

		//ntsc / pal setup

		//display
		void	DispErase( BOOL8 A_IsErase );
		
		//osg
		Cls_GrGdib*	OsgGdibObjGet( void );
		BOOL8	OsgUpdateRect( Ptr_GrRect A_PtrRect );
		void	OsgAlphaStrengthSet( __u32 A_Strength );

		//quad
		BOOL8	QuadWinSet( Ptr_GrWordRect A_PtrRcTbl, __u8* A_PtrSclTbl, __u8* A_PtrChTbl, __u32 A_PipMap, 
			__u8 A_Cnt, __u16* A_PtrBarPotTbl, __u32 A_BarMap, __u32 A_LineMap, __u8 A_QuadMode, BOOL8 A_IsExp );
		__u32	QuadViewChMapGet( void );
		//test
		void	DigitZoomMove( __u16 A_X, __u16 A_Y );
		void	DigitZoomOff();

		//mouse
		void	MseCusImgSet( __u32 A_Idx );					//mouse cusor image set ( 0 = off )
		void	MseCusPosSet( __u16 A_X, __u16 A_Y );

		//i2c
		BOOL8	I2cSlotRead( __u8 A_Ch, __u8 A_Slot, void* A_PtrData, BOOL8 A_IsWait );
		BOOL8	I2cStdWrite( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
		BOOL8	I2cStdRead( __u8 A_Ch, __u8 A_AdrDev, __u8 A_AdrSub, __u32 A_Size, __u8 A_Slot );
		BOOL8	I2cS16Write( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u8* A_PtrData, __u32 A_Size );
		BOOL8	I2cS16Read( __u8 A_Ch, __u8 A_AdrDev, __u16 A_AdrSub, __u32 A_Size, __u8 A_Slot );

		//uart
		BOOL8	UartSetup( __u8 A_Ch, __u32 A_BaudRate,	__u8 A_DataBit, __u8 A_StopBitMode, __u8 A_ParityMode );
		__u32	UartWrite( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, BOOL8 A_IsWait = FALSE	);
		__u32	UartRead( __u8 A_Ch, void* A_PtrBuf, __u32 A_Size, __u32 A_TimeOut = 0 );

		//gpio
		BOOL8	GpioOutSet( __u32 A_Map );
		BOOL8	GpioOutClear( __u32 A_Map );
		BOOL8	GpioWrite( __u32 A_Set, __u32 A_Clear );
		__u32	GpioRead( void );

		//record
		void	RecAtbSet( __u8 A_Ch, __u32 A_Atb );
		__u32	RecAtbGet( __u8 A_Ch );
		void	WaterMarkSet( BOOL8 A_IsWmrk );

		/*
		__u32	RecMotionMapGet( void );
		void	RecMotionMapSet( __u32 A_Map );
		__u32	RecSensorMapGet( void );
		void	RecSensorMapSet( __u32 A_Map );
		*/
		BOOL8	IsEncChAble( __u8 A_Ch );
		__u32	VencWatchdogCount( void );
		void	VencWatchdogReset( void );

		//play
		__u32	PlayChMapGet( void );
		void	PlayChMapSet( __u32 A_ChMap );
		void*	PlayBufPtrGet( void );
		__u32	PlayBufSizeGet( void );
		BOOL8	PlayVdoData( Ptr_GrDvrAvHd A_PtrFrm, BOOL8 A_IsNoDisp );
		BOOL8	PlayAdoData( Ptr_GrDvrAvHd A_PtrFrm );
		BOOL8	PlayAdoOob( void* A_PtrData );
		void	PlayAtbSet( __u8 A_Ch, __u8 A_Atb );
		__u8	PlayAtbGet( __u8 A_Ch );
		__u32	PlayVdoRemainCntGet( void );		//remain play data
		__u32	PlayAdoRemainCntGet( void );		//remain play data
		BOOL8	IsPlayDeinterlace( void );
		void	PlayDeinterlaceSet( BOOL8 A_IsSet );
		BOOL8	IsPlaying( void );		//now playing
		void	PlayingSet( BOOL8 A_IsSet );

		//motion
		void	MotionInit( void );
		void	MotionStart( void );
		__u32 MotionOcuMapGet( void );
		void	MotionChEnableSet( __u32 A_ChMap );
		void	MotionSenseMapSet( __u8 A_Ch, void* A_PtrMap );
		void	MotionSenseSet( __u8 A_Ch, __u16 A_Sense );
		void	MotionSenseMapAndSenseSet( __u8 A_Ch, void* A_PtrMap, __u16 A_Sense );
		void	MotionDisplaySet( BOOL8 A_IsOn );
		void	EncMtnMapSensSet( __u32 A_Sense );

		//network
		void*	NetManagerPtrGet( void );
		void*	NetLiveMpgBufPtrGet( void );
		void*	NetLiveAdoBufPtrGet( void );
		void*	NetLiveJpgBufPtrGet( void );

		//mosaic
		void MosaicValueSet( __u32 A_MosIdx, __u8 A_Ch, __u8 A_Brgt );//new-120905~mosaic
		__u32 MosaicValueGet( __u8 A_Ch );//new-120905~mosaic
		__u32 MosaicBrightValueGet( __u8 A_Ch );//new-120905~mosaic
};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
