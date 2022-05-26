/*
	GAUSE Platform basic input output system

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3Bios
#define	_EPC_GrP3Bios

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3AsmBase.h>

//====================================================================
//constance


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//global function

// system
void	GrP3BiosQuit( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// system quit
void	GrP3BiosErrBuzOff( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// system quit
void	GrP3BiosPwrOff( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// system power off

void	GrP3BiosTimeSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3BiosRgstRead( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// registry read
void	GrP3BiosRgstWrite( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// registry write
void	GrP3BiosRgstSave( __s32* A_PtrRint, WCHAR** A_PtrRstr );		// registry save

void	GrP3BiosEvtLogAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosDipSwGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// configuration
void	GrP3BiosCfgLoadFromRom( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgSaveToRom( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgRestore( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgIsChanged( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgAssign( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgMtnMapUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgBlkageMapUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgMtnSetChk(__s32* A_PtrRint, WCHAR** A_PtrRstr);

// av
void	GrP3BiosCamAutoGainSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamGainSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamConstSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamBrightSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamHueSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamSatSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnDmsInfo(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnViewSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnViewMapGet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnViewUpdate(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnNowSensGet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosMtnNowSensSet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosQuadLock(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosQuadUnlock(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamTypeGet(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosCamTypeToStr(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosDispResTextAdd(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosDispResToStr(__s32* A_PtrRint, WCHAR** A_PtrRstr);
void	GrP3BiosDispResCntGet(__s32* A_PtrRint, WCHAR** A_PtrRstr);

// network
void	GrP3BiosMacAdrGetGvu( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosMacAdrSetGvu( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosWebCodeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosIsLanOn( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosNtpTest( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosEmailTest( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// record
void	GrP3BiosHddFormat( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosHddRecSizeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosHddStatGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosGroupRecSizeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosGroupRecStatGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosGroupDayMapGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosGroupMinMapGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosGroupTimeRngGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosRecSizeExpect( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosRecCfgChk( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosHddGroupFormat( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosVrecStatGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosVrecFpsTextAdd( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosVrecFpsToStr( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosVrecFpsCntGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// play
void	GrP3BiosPlayGroupSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayGoTime( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayDo( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlaySpeedSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayDirSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayTimeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayPause( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayGoFirst( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayGoRecent( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayNextFrm( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosPlayPrevFrm( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// audio
void	GrP3BiosAdoLiveChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosAdoLiveVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosAdoPlayChSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosAdoPlayVolSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// UI
void	GrP3BiosHwAlphaUpdt( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// catalog
void	GrP3BiosCtlgFwUpgdLoad( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgFwUpgdNext( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgFwUpgdPrev( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgFwUpgdItemValGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgFwUpgdItemStrGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3BiosCtlgCfgLoad( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgCfgNext( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgCfgPrev( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgCfgItemValGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgCfgItemStrGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

void	GrP3BiosCtlgLogLoad( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgLogNext( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgLogPrev( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgLogItemValGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCtlgLogItemStrGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// firmware upgrade
void	GrP3BiosFwUpgdRun( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// configuration file save
void	GrP3BiosCfgFileLoad( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosCfgFileSave( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// miscellaneous
void	GrP3BiosBuzRun( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosDoSet( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// backup
void	GrP3BiosBkupMediaSizeGet( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosBkupRequest( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosBkupCancel( __s32* A_PtrRint, WCHAR** A_PtrRstr );

// log backup
void	GrP3BiosLogBkupRequest( __s32* A_PtrRint, WCHAR** A_PtrRstr );
void	GrP3BiosLogBkupCancel( __s32* A_PtrRint, WCHAR** A_PtrRstr );


//====================================================================
//out Environment conditional
#endif

