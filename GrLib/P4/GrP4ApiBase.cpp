/*
	GAUSE Platform API base

*/

#include <P4/GrP4ApiBase.h>
#include <P4/GrP4Bios.h>
#include <P4/GrP4Mng.h>
#include <P4/GrP4Cfg.h>
#include <P4/GrP4Script.h>
#include <P4/GrP4DoWin.h>
#include <GrDebug.h>
#include <Dvr/GrDvrBase.h>
#include <GrDumyTool.h>

#ifdef GR_P4_EDITOR
#include <P4/Dvl/GrP4DvlBase.h>
#endif

#include <GrStrTool.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
#include <Dvr/PtzV2/GrPtzV2Mng.h>

#include <P4/GrP4UiCmd.h>

#include <P4/Sio/GrP4SioMng.h>

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

void	GrP4ApiTimeGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiCfgLangUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiCfgReset(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiCfgAssign(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiCfgDateFmtUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiNetModelSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiRecEmgcSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiLogIn(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiLogOut(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPasswdRuleChk(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiQuadModeNext(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiQuadPageNext(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiQuadModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiQuadModeGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiAutoSeqSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiFwVerGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiRmtTmpAble(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiRmtIdSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiFocusIdGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiKeyBlockSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiLayerUpClose(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiPlayModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiCtlgItemValGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiCtlgItemStrGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiUiAlphaUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiUiMenuModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiUiLoginModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiUiMsgBoxModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiAdoLiveChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiAdoLiveVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiAdoPlayChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiAdoPlayVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

void	GrP4ApiPtzPanTilt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzZoom(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzFocus(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzIris(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzStop(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzAutoFocus(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuCall(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuExit(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuLeft(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuRight(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuUp(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuDown(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuOk(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzMenuCancel(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzPresetSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzPresetMove(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzTourRun(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzIdView(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzIdSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzModelTxtAdd(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzCtrlLock(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiPtzTourSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);
void	GrP4ApiCtrlModelTxtAdd(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost);

//--------------------------------------------------------------------
//global var
extern	Cls_GrP4Mng*	V_GrP4Mng;
extern	__u32	V_GrP4UartSpeedTbl[E_GrP4CfgUartSpeedCnt];

Fnc_GrP4Api	V_GrP4ApiTbl[E_GrP4ApiTblCnt]	=	
{
	NULL,												/* 0 - E_GrP4ApiQuit  */
	GrP4ApiTimeGet,							/* 1 - E_GrP4ApiTimeGet  */
	NULL,												/* 2 - E_GrP4ApiTimeSet  */
	NULL,												/* 3 - E_GrP4ApiCfgLoadFromRom */
	NULL,												/* 4 - E_GrP4ApiCfgSaveToRom */
	NULL,												/* 5 - E_GrP4ApiCfgRestore */
	NULL,												/* 6 - E_GrP4ApiCfgMtnMapUpdt */
	NULL,												/* 7 - E_GrP4ApiCfgPrivacyMapUpdt */
	NULL,												/* 8 - (reserved) */
	GrP4ApiCfgReset,						/* 9 - E_GrP4ApiCfgReset */
	GrP4ApiCfgLangUpdt,					/* 10 - E_GrP4ApiCfgLangUpdt */
	NULL,												/* 11 - E_GrP4ApiCfgIsChanged */
	GrP4ApiCfgAssign,						/* 12 - E_GrP4ApiCfgAssign */
	GrP4ApiUiAlphaUpdt,					/* 13 - E_GrP4ApiCfgAlphaUpdt */
	GrP4ApiFwVerGet,						/* 14 - E_GrP4ApiFwVerGet */
	NULL,												/* 15 - (reserved) */
	NULL,												/* 16 - (reserved) */
	GrP4ApiCfgDateFmtUpdt,			/* 17 - E_GrP4ApiCfgDateFmtUpdt */
	GrP4ApiLogIn,								/* 18 - E_GrP4ApiLogIn */
	GrP4ApiLogOut,							/* 19 - E_GrP4ApiLogOut */
	GrP4ApiQuadModeNext,				/* 20 - E_GrP4ApiQuadModeNext */
	GrP4ApiQuadPageNext,				/* 21 - E_GrP4ApiQuadPageNext */
	GrP4ApiQuadModeSet,					/* 22 - E_GrP4ApiQuadModeSet */
	GrP4ApiQuadModeGet,					/* 23 - E_GrP4ApiQuadModeGet */
	NULL,												/* 24 - E_GrP4ApiQuadLock */
	NULL,												/* 25 - E_GrP4ApiQuadUnlock */
	GrP4ApiAutoSeqSet,					/* 26 - E_GrP4ApiAutoSeqSet */
	NULL,												/* 27 - E_GrP4ApiDispResTextAdd */
	NULL,												/* 28 - E_GrP4ApiDispResToStr */
	NULL,												/* 29 - E_GrP4ApiDispResCntGet */
	GrP4ApiFocusIdGet,					/* 30 - E_GrP4ApiFocusIdGet */
	GrP4ApiKeyBlockSet,					/* 31 - E_GrP4ApiKeyBlockSet */
	GrP4ApiLayerUpClose,				/* 32 - E_GrP4ApiLayerUpClose */
	GrP4ApiUiMenuModeSet,				/* 33 - E_GrP4ApiMenuModeSet */
	GrP4ApiUiLoginModeSet,			/* 34 - E_GrP4ApiLoginModeSet */
	GrP4ApiUiMsgBoxModeSet,			/* 35 - E_GrP4ApiMsgBoxMode */
	NULL,												/* 36 - (reserved) */
	NULL,												/* 37 - (reserved) */
	NULL,												/* 38 - (reserved) */
	NULL,												/* 39 - (reserved) */
	NULL,												/* 40 - E_GrP4ApiMacAdrGetGvu */
	NULL,												/* 41 - E_GrP4ApiMacAdrSetGvu */
	NULL,												/* 42 - E_GrP4ApiWebCodeGet */
	NULL,												/* 43 - E_GrP4ApiLanCableStat */
	NULL,												/* 44 - (reserved) */
	NULL,												/* 45 - (reserved) */
	NULL,												/* 46 - (reserved) */
	NULL,												/* 47 - (reserved) */
	NULL,												/* 48 - (reserved) */
	NULL,												/* 49 - (reserved) */
	NULL,												/* 50 - E_GrP4ApiHddFormat */
	NULL,												/* 51 - E_GrP4ApiHddRecSizeGet */
	NULL,												/* 52 - E_GrP4ApiHddStatGet */
	NULL,												/* 53 - E_GrP4ApiGroupRecSizeGet */
	NULL,												/* 54 - E_GrP4ApiGroupRecStatGet */
	NULL,												/* 55 - E_GrP4ApiGroupDayMapGet */
	GrP4ApiRecEmgcSet,					/* 56 - E_GrP4ApiRecEmgcSet */
	NULL,												/* 57 - E_GrP4ApiGroupTimeRngGet */
	NULL,												/* 58 - E_GrP4ApiRecSizeExpect */
	NULL,												/* 59 - E_GrP4ApiRecCfgChk */
	GrP4ApiPlayModeSet,					/* 60 - E_GrP4ApiPlayModeSet */
	NULL,												/* 61 - E_GrP4ApiPlayGroupSel */
	NULL,												/* 62 - E_GrP4ApiPlayGoTime */
	NULL,												/* 63 - E_GrP4ApiPlaySpeedSet */
	NULL,												/* 64 - E_GrP4ApiPlayDirSet */
	NULL,												/* 65 - E_GrP4ApiPlayDo */
	NULL,												/* 66 - E_GrP4ApiPlayPuase */
	NULL,												/* 67 - E_GrP4ApiPlayTimeGet */
	NULL,												/* 68 - E_GrP4ApiPlayGoFirst */
	NULL,												/* 69 - E_GrP4ApiPlayGoRecent */
	NULL,												/* 70 - E_GrP4ApiPlayNextFrm */
	NULL,												/* 71 - E_GrP4ApiPlayPrevFrm */
	NULL,												/* 72 - (reserved) */
	NULL,												/* 73 - (reserved) */
	NULL,												/* 74 - (reserved) */
	NULL,												/* 75 - (reserved) */
	NULL,												/* 76 - (reserved) */
	NULL,												/* 77 - (reserved) */
	NULL,												/* 78 - (reserved) */
	NULL,												/* 79 - (reserved) */
	NULL,												/* 80 - E_GrP4ApiCamBriSet */
	NULL,												/* 81 - E_GrP4ApiCamConSet */
	NULL,												/* 82 - E_GrP4ApiCamHueSet */
	NULL,												/* 83 - E_GrP4ApiCamSatSet */
	NULL,												/* 84 - E_GrP4ApiCamGainSet */
	NULL,												/* 85 - E_GrP4ApiCamAutoGainSet */
	NULL,												/* 86 - E_GrP4ApiCamTypeGet */
	NULL,												/* 87 - E_GrP4ApiCamTypeToStr */
	NULL,												/* 88 - (reserved) */
	NULL,												/* 89 - (reserved) */
	GrP4ApiCtlgItemValGet,			/* 90 - E_GrP4ApiCtlgItemValGet */
	GrP4ApiCtlgItemStrGet,			/* 91 - E_GrP4ApiCtlgItemStrGet */
	NULL,												/* 92 - (reserved) */
	NULL,												/* 93 - (reserved) */
	NULL,												/* 94 - (reserved) */
	NULL,												/* 95 - (reserved) */
	NULL,												/* 96 - (reserved) */
	NULL,												/* 97 - (reserved) */
	NULL,												/* 98 - (reserved) */
	NULL,												/* 99 - (reserved) */
	NULL,												/* 100 - E_GrP4ApiFwUpgdRun */
	NULL,												/* 101 - E_GrP4ApiMtnViewMapUpdt */
	NULL,												/* 102 - E_GrP4ApiCfgFileLoad */
	NULL,												/* 103 - E_GrP4ApiCfgFileSave */
	NULL,												/* 104 - (reserved) */
	NULL,												/* 105 - (reserved) */
	NULL,												/* 106 - (reserved) */
	NULL,												/* 107 - (reserved) */
	NULL,												/* 108 - (reserved) */
	NULL,												/* 109 - (reserved) */
	NULL,												/* 110 - (reserved) */
	NULL,												/* 111 - (reserved) */
	NULL,												/* 112 - (reserved) */
	NULL,												/* 113 - (reserved) */
	NULL,												/* 114 - (reserved) */
	NULL,												/* 115 - (reserved) */
	NULL,												/* 116 - (reserved) */
	NULL,												/* 117 - (reserved) */
	NULL,												/* 118 - (reserved) */
	NULL,												/* 119 - (reserved) */
	GrP4ApiPasswdRuleChk,				/* 120 - E_GrP4ApiPasswdRuleChk */
	NULL,												/* 121 - (reserved) */
	NULL,												/* 122 - E_GrP4ApiEvtLogAdd */
	NULL,												/* 123 - (reserved) */
	NULL,												/* 124 - (reserved) */
	NULL,												/* 125 - (reserved) */
	GrP4ApiNetModelSet,					/* 126 - E_GrP4ApiNetModelSet */
	GrP4ApiRmtTmpAble,					/* 127 - E_GrP4ApiRmtTmpAbl */
	GrP4ApiRmtIdSet,						/* 128 - E_GrP4ApiRmtIdSet */
	NULL,												/* 129 - E_GrP4ApiNtpTest */
	GrP4ApiAdoLiveChSet,				/* 130 - E_GrP4ApiAdoLiveChSet */
	GrP4ApiAdoLiveVolSet,				/* 131 - E_GrP4ApiAdoLiveVolSet */
	GrP4ApiAdoPlayChSet,				/* 132 - E_GrP4ApiAdoPlayChSet */
	GrP4ApiAdoPlayVolSet,				/* 133 - E_GrP4ApiAdoPlayVolSet */
	NULL,												/* 134 - (reserved) */
	NULL,												/* 135 - (reserved) */
	NULL,												/* 136 - (reserved) */
	NULL,												/* 137 - (reserved) */
	NULL,												/* 138 - (reserved) */
	NULL,												/* 139 - (reserved) */
	GrP4ApiPtzPanTilt,					/* 140 - E_GrP4ApiPtzPanTilt */
	GrP4ApiPtzZoom,							/* 141 - E_GrP4ApiPtzZoom */
	GrP4ApiPtzFocus,						/* 142 - E_GrP4ApiPtzFocus */
	GrP4ApiPtzIris,							/* 143 - E_GrP4ApiPtzIris */
	GrP4ApiPtzStop,							/* 144 - E_GrP4ApiPtzStop */
	GrP4ApiPtzAutoFocus,				/* 145 - E_GrP4ApiPtzAutoFocus */
	GrP4ApiPtzMenuCall,					/* 146 - E_GrP4ApiPtzMenuCall */
	GrP4ApiPtzMenuExit,					/* 147 - E_GrP4ApiPtzMenuExit */
	GrP4ApiPtzMenuLeft,					/* 148 - E_GrP4ApiPtzMenuLeft */
	GrP4ApiPtzMenuRight,				/* 149 - E_GrP4ApiPtzMenuRight */
	GrP4ApiPtzMenuUp,						/* 150 - E_GrP4ApiPtzMenuUp */
	GrP4ApiPtzMenuDown,					/* 151 - E_GrP4ApiPtzMenuDown */
	GrP4ApiPtzMenuOk,						/* 152 - E_GrP4ApiPtzMenuOk */
	GrP4ApiPtzMenuCancel,				/* 153 - E_GrP4ApiPtzMenuCancel */
	GrP4ApiPtzPresetSet,				/* 154 - E_GrP4ApiPtzPresetSet */
	GrP4ApiPtzPresetMove,				/* 155 - E_GrP4ApiPtzPresetMove */
	GrP4ApiPtzTourRun,					/* 156 - E_GrP4ApiPtzTourRun */
	GrP4ApiPtzTourSet,					/* 157 - E_GrP4ApiPtzTourSet */
	GrP4ApiPtzIdView,						/* 158 - E_GrP4ApiPtzIdView */
	GrP4ApiPtzIdSet,						/* 159 - E_GrP4ApiPtzIdSet */
	GrP4ApiPtzModelTxtAdd,			/* 160 - E_GrP4ApiPtzModelTxtAdd */
	GrP4ApiPtzCtrlLock,					/* 161 - E_GrP4ApiPtzCtrlLock */
	GrP4ApiCtrlModelTxtAdd,			/* 162 - E_GrP4ApiCtrlModelTxtAdd */
	NULL,												/* 163 - (reserved) */
	NULL,												/* 164 - (reserved) */
	NULL,												/* 165 - (reserved) */
	NULL,												/* 166 - (reserved) */
	NULL,												/* 167 - (reserved) */
	NULL,												/* 168 - (reserved) */
	NULL,												/* 169 - (reserved) */
	NULL,												/* 170 - E_GrP4ApiBkupMediaSizeGet */
	NULL,												/* 171 - E_GrP4ApiBkupRequest */
	NULL,												/* 172 - E_GrP4ApiBkupCancel */
	NULL,												/* 173 - (reserved) */
	NULL,												/* 174 - (reserved) */
	NULL,												/* 175 - E_GrP4ApiLogBkupRequest */
	NULL,												/* 176 - E_GrP4ApiLogBkupCancel */
	NULL,												/* 177 - (reserved) */
	NULL,												/* 178 - (reserved) */
	NULL,												/* 179 - (reserved) */
	NULL,												/* 180 - E_GrP4ApiErrBuzOff */
	NULL,												/* 181 - E_GrP4ApiDoSet */
	NULL,												/* 182 - E_GrP4ApiBuzRun */
	NULL,												/* 183 - (reserved) */
	NULL,												/* 184 - (reserved) */
	NULL,												/* 185 - (reserved) */
	NULL,												/* 186 - (reserved) */
	NULL,												/* 187 - (reserved) */
	NULL,												/* 188 - (reserved) */
	NULL,												/* 189 - (reserved) */
	NULL,												/* 190 - E_GrP4ApiMtnSetChk */
	NULL,												/* 191 - E_GrP4ApiPrivacyChUpdt */
	NULL,												/* 192 - E_GrP4ApiPwrOff */
	NULL,												/* 193 - E_GrP4ApiRgstLoad */
	NULL,												/* 194 - E_GrP4ApiRgstSave */
	NULL,												/* 195 - (reserved) */
	NULL,												/* 196 - E_GrP4ApiMtnDmsInfo */
	NULL,												/* 197 - E_GrP4ApiMtnSenGet */
	NULL,												/* 198 - E_GrP4ApiMtnSenSet */
	NULL,												/* 199 - E_GrP4ApiMtnViewMapGet */
	NULL,												/* 200 - E_GrP4ApiGroupHddFormat */
	NULL,												/* 201 - E_GrP4ApiEmailTest */
	NULL,												/* 202 - E_GrP4ApiVrecStatGet */
	NULL,												/* 203 - E_GrP4ApiVrecFpsTextAdd */
	NULL,												/* 204 - E_GrP4ApiVrecFpsToStr */
	NULL,												/* 205 - E_GrP4ApiVrecFpsCntGet */
	NULL,												/* 206 - (reserved) */
	NULL,												/* 207 - (reserved) */
	NULL,												/* 208 - (reserved) */
	NULL,												/* 209 - (reserved) */
	NULL,												/* 210 - E_GrP4ApiCtlgFwUpgdLoad */
	NULL,												/* 211 - E_GrP4ApiCtlgFwUpgdItemValGet */
	NULL,												/* 212 - E_GrP4ApiCtlgFwUpgdItemStrGet */
	NULL,												/* 213 - E_GrP4ApiCtlgFwUpgdNext */
	NULL,												/* 214 - E_GrP4ApiCtlgFwUpgdPrev */
	NULL,												/* 215 - E_GrP4ApiCtlgCfgLoad */
	NULL,												/* 216 - E_GrP4ApiCtlgCfgItemValGet */
	NULL,												/* 217 - E_GrP4ApiCtlgCfgItemStrGet */
	NULL,												/* 218 - E_GrP4ApiCtlgCfgNext */
	NULL,												/* 219 - E_GrP4ApiCtlgCfgPrev */
	NULL,												/* 220 - E_GrP4ApiCtlgLogLoad */
	NULL,												/* 221 - E_GrP4ApiCtlgLogItemValGet */
	NULL,												/* 222 - E_GrP4ApiCtlgLogItemStrGet */
	NULL,												/* 223 - E_GrP4ApiCtlgLogNext */
	NULL,												/* 224 - E_GrP4ApiCtlgLogPrev */
	NULL,												/* 225 - (reserved) */
	NULL,												/* 226 - (reserved) */
	NULL,												/* 227 - (reserved) */
	NULL,												/* 228 - (reserved) */
	NULL,												/* 229 - (reserved) */
	NULL,												/* 230 - (reserved) */
	NULL,												/* 231 - (reserved) */
	NULL,												/* 232 - (reserved) */
	NULL,												/* 233 - (reserved) */
	NULL,												/* 234 - (reserved) */
	NULL,												/* 235 - (reserved) */
	NULL,												/* 236 - (reserved) */
	NULL,												/* 237 - (reserved) */
	NULL,												/* 238 - (reserved) */
	NULL,												/* 239 - (reserved) */
	NULL,												/* 240 - (reserved) */
	NULL,												/* 241 - (reserved) */
	NULL,												/* 242 - (reserved) */
	NULL,												/* 243 - (reserved) */
	NULL,												/* 244 - (reserved) */
	NULL,												/* 245 - (reserved) */
	NULL,												/* 246 - (reserved) */
	NULL,												/* 247 - (reserved) */
	NULL,												/* 248 - (reserved) */
	NULL,												/* 249 - (reserved) */
	NULL,												/* 250 - (reserved) */
	NULL,												/* 251 - (reserved) */
	NULL,												/* 252 - (reserved) */
	NULL,												/* 253 - (reserved) */
	NULL,												/* 254 - (reserved) */
	NULL,												/* 255 - (reserved) */

};

#ifdef GR_P4_EDITOR

St_GrP4DvlApiInfo	V_GrP4ApiInfoTbl[E_GrP4ApiCnt]	=	
{
	{	E_GrP4ApiQuit,						L"Quit :: " },
	{	E_GrP4ApiTimeGet,					L"TimeGet :: out : (0) time" },
	{	E_GrP4ApiTimeSet,					L"TimeSet :: in : (1) time" },
	{ E_GrP4ApiFwVerGet,				L"FwVerGet :: out : (0) firmware version" },
	//{ E_GrP4ApiModelGet,				L"ModelGet :: out : (0) model number" },
	{ E_GrP4ApiNetModelSet,			L"Network Model Set :: in : (1) network model number" },
	{	E_GrP4ApiRmtTmpAbl,				L"REMOCON temporary input able :: in : (1) 0 = disable, 1 = able" },
	{	E_GrP4ApiRmtIdSet,				L"REMOCON running ID set :: in : (1) ID number" },
	{	E_GrP4ApiEvtLogAdd,				L"EvtLogAdd :: in : (1)type (2)channel mask (3)para0 (4) para1" },

	{ E_GrP4ApiLogIn,						L"LogIn :: in : (1) id index (2) password treg index (3) dual password treg index :: out : (0) 0=fail,1=success" },
	{ E_GrP4ApiLogOut,					L"LogOut :: " },
	{ E_GrP4ApiPasswdRuleChk,		L"PasswdRuleChk :: out : (0) o=bad, 1=good" },
	{ E_GrP4ApiBuzRun,					L"BuzRun :: in : (1) run count " },
	{ E_GrP4ApiErrBuzOff,				L"ErrBuzOff :: " },

	{	E_GrP4ApiQuadModeNext,		L"QuadModeNext :: in : (1) console index " },
	{	E_GrP4ApiQuadPageNext,		L"QuadPageNext :: in : (1) console index " },
	{	E_GrP4ApiQuadModeSet,			L"QuadModeSet :: in : (1) console index, (2) mode, (3) page" },
	{	E_GrP4ApiQuadModeGet,			L"QuadModeGet :: in : (1) console index :: out : (0) mode, (1) page" },
	{	E_GrP4ApiQuadLock,				L"QuadLock :: " },
	{	E_GrP4ApiQuadUnlock,			L"QuadUnlock :: " },

	{	E_GrP4ApiAutoSeqSet,			L"AutoSeqSet :: in : (1) 0 = off, 1= on" },
	{	E_GrP4ApiDispResTextAdd,	L"DispResTextAdd :: in : (1) consol index (2) window id (3) control id" },
	{	E_GrP4ApiDispResToStr,		L"DispResToStr :: in : (1) resolution index :: out : (T0) text" },
	{	E_GrP4ApiDispResCntGet,		L"DispResCntGet :: out : (0) count" },

	{	E_GrP4ApiCfgReset,				L"CfgReset :: in : (1) 0 = normal , 1 = factroy default " },
	{ E_GrP4ApiCfgLoadFromRom,	L"CfgLoadFromRom :: " },
	{ E_GrP4ApiCfgSaveToRom,		L"CfgSaveToRom :: " },
	{	E_GrP4ApiCfgRestore,			L"CfgRestore :: in : " },
	{	E_GrP4ApiCfgLangUpdt,			L"CfgLangUpdt :: " },
	{	E_GrP4ApiCfgIsChanged,		L"CfgIsChanged :: out : (0) 1=changed" },
	{	E_GrP4ApiCfgAlphaUpdt,		L"CfgAlphaUpdt :: " },
	{	E_GrP4ApiCfgDateFmtUpdt,	L"CfgDateFmtUpdt :: " },
	{ E_GrP4ApiCfgMtnMapUpdt,		L"CfgMtnMapUpdt :: in : (1) channel " },
	{ E_GrP4ApiCfgPrivacyMapUpdt, L"CfgPrivacyMapUpdt :: in : (1) channel " },

	{	E_GrP4ApiFocusIdGet,			L"FocusIdGet :: in : (1) console index :: out : (0) script id " },
	{	E_GrP4ApiKeyBlockSet,			L"KeyBlockSet :: in : (1) key code" },
	{	E_GrP4ApiLayerUpClose,		L"LayerUpClose :: in : (1) console index (2) start layer" },
	{	E_GrP4ApiMenuModeSet,			L"MenuModeSet :: in : (1) menu mode" },
	{	E_GrP4ApiLoginModeSet,		L"LoginModeSet :: in : (1) login mode" },
	{	E_GrP4ApiMsgBoxMode,			L"MsgBoxModeSet :: in : (1) message box mode" },

	{	E_GrP4ApiCfgAssign,				L"CfgAssign :: " },

	{	E_GrP4ApiMacAdrGetGvu,		L"MacAdrGetGvu :: in : (1) recive GVU var index" },
	{	E_GrP4ApiMacAdrSetGvu,		L"MacAdrSetGvu :: in : (1) store GVU var index" },
	{	E_GrP4ApiWebCodeGet,			L"WebCodeGet :: in : (1) recive text register index" },
	{ E_GrP4ApiLanCableStat,		L"LanCableStat :: out : (1) 0=not connect, 1=connected" },
	{	E_GrP4ApiHddFormat,				L"HddFormat :: in : (1) hdd index, (2) 1=logformat" },
	{ E_GrP4ApiGroupHddFormat,	L"HddGroupFormat :: in : (1) group type, (2) 1=logformat" },
	{	E_GrP4ApiHddRecSizeGet,		L"HddRecSizeGet :: in : (1) hdd index :: out : (2) disk size, (3) record size" },
	{	E_GrP4ApiHddStatGet,			L"HddStatGet :: in : (1) hdd index :: out : (0) disk record status" },
	{	E_GrP4ApiGroupRecSizeGet,	L"GroupRecSizeGet :: in : (1) group index :: out : (2) total disk size, (3) total record size" },
	{	E_GrP4ApiGroupRecStatGet,	L"GroupRecStatGet :: in : (1) group index :: out : (0) group record status" },
	{	E_GrP4ApiGroupDayMapGet,	L"GroupRecDayMapGet :: in : (1) group index, (2) time, (3) day count :: out : (0) record day map" },
	{	E_GrP4ApiGroupTimeRngGet,	L"GroupTimeRngGet :: in : (1) group index :: out : (0) start time (1) end time" },
	{ E_GrP4ApiRecEmgcSet,			L"RecEmgcSet :: in : (1) 0=off,1=on" },
	{ E_GrP4ApiRecSizeExpect,		L"RecSizeExpect :: in : (1) group index (2) channel mask (3) start time (4) end time :: out (0) MByte unit size" },
	{	E_GrP4ApiRecCfgChk,				L"RecCfgChk :: out : (0) 0=good,1=bad" },
	{	E_GrP4ApiVrecStatGet,			L"VrecStatGet :: in : (1) channel :: out : (0) status" },
	{	E_GrP4ApiVrecFpsTextAdd,	L"VrecFpsTextAdd :: in : (1) console (2) window id (3) control id (4) channel" },
	{	E_GrP4ApiVrecFpsToStr,		L"VrecFpsToStr :: in : (1) channel (2) speed value :: out : (T0) speed text" },
	{	E_GrP4ApiVrecFpsCntGet,		L"VrecFpsCntGet :: (1) channel :: out : (0) count" },

	{ E_GrP4ApiCamAutoGainSet,	L"CamAutoGainSet :: in : (1) channel index, (2) 0=Manual,1=Auto " },
	{ E_GrP4ApiCamGainSet,			L"CamGainSet :: in : (1) channel index, (2) value" },
	{ E_GrP4ApiCamBriSet,				L"CamBriSet :: in : (1) channel index, (2) value" },
	{ E_GrP4ApiCamConSet,				L"CamConSet :: in : (1) channel index, (2) value" },
	{ E_GrP4ApiCamHueSet,				L"CamHueSet :: in : (1) channel index, (2) value" },
	{ E_GrP4ApiCamSatSet,				L"CamSatSet :: in : (1) channel index, (2) value" },
	{ E_GrP4ApiCamTypeGet,			L"CamTypeGet :: in : (1) channel index :: out : (0) camera type" },
	{ E_GrP4ApiCamTypeToStr,		L"CamTypeToStr :: in : (1) camera type :: out (T0) camera type text" },

	{ E_GrP4ApiPlayModeSet,			L"PlayModeSet :: in : (1) 0=live,1=play" },
	{ E_GrP4ApiPlayGroupSel,		L"PlayGroupSel :: in : (1) 0=normal,1=mirror" },
	{ E_GrP4ApiPlayGoTime,			L"PlayGoTime :: in : (1) time" },
	{ E_GrP4ApiPlaySpeedSet,		L"PlaySpeedSet :: in : (1) speed" },
	{ E_GrP4ApiPlayDirSet,			L"PlayDirSet :: in : (1) 0=normal,1=back" },
	{ E_GrP4ApiPlayDo,					L"PlayDo :: " },
	{ E_GrP4ApiPlayPuase,				L"PlayPause :: " },
	{ E_GrP4ApiPlayTimeGet,			L"PlayTimeGet :: out : (0) time (1) summer time" },
	{ E_GrP4ApiPlayGoFirst,			L"PlayGoFirst :: " },
	{ E_GrP4ApiPlayGoRecent,		L"PlayGoRecent :: " },
	{ E_GrP4ApiPlayNextFrm,			L"PlayNextFrm :: " },
	{ E_GrP4ApiPlayPrevFrm,			L"PlayPrevFrm :: " },

	{ E_GrP4ApiAdoLiveChSet,		L"Live audio channel set :: in : (1) channel, 254=off, 255=auto" },
	{ E_GrP4ApiAdoLiveVolSet,		L"Live audio volume set :: in : (1) channel (2) volume 0 ~ 100" },
	{ E_GrP4ApiAdoPlayChSet,		L"Play audio channel set :: in : (1) channel, 254=off, 255=auto" },
	{ E_GrP4ApiAdoPlayVolSet,		L"Play audio volume set :: in : (1) channel (2) volume 0 ~ 100" },

	{ E_GrP4ApiCtlgItemValGet,	L"CtlgItemValGet :: in : (1) handle, (2) element (3) add value :: out : (0) success , (1) result value" },
	{ E_GrP4ApiCtlgItemStrGet,	L"CtlgItemStrGet :: in : (1) handle, (2) element (3) add value :: out : (0) success , (T0) result string" },

	{ E_GrP4ApiFwUpgdRun,				L"FwUpgdRun :: in : (1) Catalog Handle :: out : (0) success" },
	{ E_GrP4ApiFwUpgdSpcRun,		L"FwUpgdSpcRun :: out : (0) success" },

	{ E_GrP4ApiCfgFileLoad,			L"CfgFileLoad :: in : (1) Catalog Handle :: out : (0) success" },
	{ E_GrP4ApiCfgFileSave,			L"CfgFileSave :: in : (T0) file name :: out : (0) success" },

	{ E_GrP4ApiPtzPanTilt,			L"PtzPanTilt :: in : (1) channel (2) left (3) right (4) up (5) down  " },
	{ E_GrP4ApiPtzZoom,					L"PtzZoom :: in : (1) channel (2) 0=tele,1=wide " },
	{ E_GrP4ApiPtzFocus,				L"PtzFocus :: in : (1) channel (2) 0=near,1=far " },
	{ E_GrP4ApiPtzIris,					L"PtzIris :: in : (1) channel (2) 0=close,1=open " },
	{ E_GrP4ApiPtzStop,					L"PtzStop :: in : (1) channel" },
	{ E_GrP4ApiPtzAutoFocus,		L"PtzAutoFocus :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuCall,			L"PtzMenuCall :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuExit,			L"PtzMenuExit :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuLeft,			L"PtzMenuLeft :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuRight,		L"PtzMenuRight :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuUp,				L"PtzMenuUp :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuDown,			L"PtzMenuDown :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuOk,				L"PtzMenuOk :: in : (1) channel" },
	{ E_GrP4ApiPtzMenuCancel,		L"PtzMenuCancel :: in : (1) channel" },
	{ E_GrP4ApiPtzPresetSet,		L"PtzPresetSet :: in : (1) channel (2) id" },
	{ E_GrP4ApiPtzPresetMove,		L"PtzPresetMove :: in : (1) channel (2) id" },
	{ E_GrP4ApiPtzTourRun,			L"PtzTourRun :: in : (1) channel (2) 0=stop,1=run" },
	{ E_GrP4ApiPtzTourSet,			L"PtzTourSet :: in : (1) channel " },
	{ E_GrP4ApiPtzIdView,				L"PtzIdView :: in : (1) channel (2) 0=off,1=on" },
	{ E_GrP4ApiPtzIdSet,				L"PtzIdSet :: in : (1) channel (2) new id" },
	{ E_GrP4ApiPtzModelTxtAdd,	L"PtzModelTxtAdd :: in : (1) console index (2) window id (3) control id" },
	{ E_GrP4ApiPtzCtrlLock,			L"PtzCtrlLock :: in : (1) 0=unlock, 1=lock" },
	{ E_GrP4ApiCtrlModelTxtAdd,	L"CtrlModelTxtAdd :: in : (1) console index (2) window id (3) control id" },

	{ E_GrP4ApiBkupMediaSizeGet,	L"BkupMediaSizeGet :: in : (1) 0=usb, 1=odd :: out : (0) Mbyte unit size" },
	{ E_GrP4ApiBkupRequest,			L"BkupRequest :: in : (1) group index (2) channel map (3) time start (4) time end (5) 0=usb 1=odd (6) viewer combine (T0) password :: out : (0) 0= fail 1= success" },
	{ E_GrP4ApiBkupCancel,			L"BkupCancel :: " },

	{ E_GrP4ApiNtpTest,					L"NtpTest :: " },

	{ E_GrP4ApiLogBkupRequest,	L"LogRequest :: in : (1) group index (2) channel map (3) event map (4) time start (5) time end (T0) file name :: out : (0) 0= fail 1= success" },
	{ E_GrP4ApiLogBkupCancel,		L"LogCancel :: " },

	{ E_GrP4ApiDoSet,						L"DoSet :: in : (1) do index (2) 0=auto, 1=off, 2= on " },
	{ E_GrP4ApiMtnSetChk,				L"Motion set check :: in : (1) channel :: out : (0) motion set " },
	{ E_GrP4ApiMtnDmsInfo,			L"MtnDmsInfo :: out : (0) 1=S/W (1) horizontal cell (2) vertical cell " },
	{ E_GrP4ApiMtnSenGet,				L"MtnSenGet :: in : (1) channel :: out : (0) sensitive " },
	{ E_GrP4ApiMtnSenSet,				L"MtnSenSet :: in : (1) channel (2) sensitive  " },
	{ E_GrP4ApiMtnViewMapGet,		L"[SYS]MtnViewMapGet :: in : (1) channel :: out : (0) map pointer " },
	{ E_GrP4ApiMtnViewMapUpdt,	L"[SYS]MtnViewMapUpdt :: in : (1) channel :: out : (0) success " },
	{ E_GrP4ApiPrivacyChUpdt,		L"Privacy ch update :: in : (1) channel " },
	{ E_GrP4ApiPwrOff,					L"Dvr Pwr Off" },
	{ E_GrP4ApiRgstLoad,				L"RgstLoad :: " },
	{ E_GrP4ApiRgstSave,				L"RgstSave :: " },

	
	{ E_GrP4ApiEmailTest,				L"Email Test :: " },

	{ E_GrP4ApiCtlgFwUpgdLoad,	L"CtlgFwUpgdLoad :: in : (1) page size :: out : (0) success (1) loaded size (2) total size (3) page count" },
	{ E_GrP4ApiCtlgFwUpgdItemValGet, L"CtlgFwUpgdItemValGet :: in : (1) index (2) code (3) adder :: out : (0) success (1) value" },
	{ E_GrP4ApiCtlgFwUpgdItemStrGet, L"CtlgFwUpgdItemStrGet :: in : (1) index (2) code (3) adder :: out : (0) success (T0) string" },
	{ E_GrP4ApiCtlgFwUpgdNext,	L"CtlgFwUpgdNext :: out : (0) success (1) count (2) total (3) page count (4) now page" },
	{ E_GrP4ApiCtlgFwUpgdPrev,	L"CtlgFwUpgdPrev :: out : (0) success (1) count (2) total (3) page count (4) now page" },

	{ E_GrP4ApiCtlgCfgLoad,			L"CtlgCfgLoad :: in : (1) page size :: out : (0) success (1) loaded size (2) total size (3) page count" },
	{ E_GrP4ApiCtlgCfgItemValGet, L"CtlgCfgItemValGet :: in : (1) index (2) code (3) adder :: out : (0) success (1) value" },
	{ E_GrP4ApiCtlgCfgItemStrGet, L"CtlgCfgItemStrGet :: in : (1) index (2) code (3) adder :: out : (0) success (T0) string" },
	{ E_GrP4ApiCtlgCfgNext,			L"CtlgCfgNext :: out : (0) success (1) count (2) total (3) page count (4) now page" },
	{ E_GrP4ApiCtlgCfgPrev,			L"CtlgCfgPrev :: out : (0) success (1) count (2) total (3) page count (4) now page" },

	{ E_GrP4ApiCtlgLogLoad,			L"CtlgLogLoad :: in : (1) page size :: out : (0) success (1) loaded size (2) total size (3) page count" },
	{ E_GrP4ApiCtlgLogItemValGet, L"CtlgLogItemValGet :: in : (1) index (2) code (3) adder :: out : (0) success (1) value" },
	{ E_GrP4ApiCtlgLogItemStrGet, L"CtlgLogItemStrGet :: in : (1) index (2) code (3) adder :: out : (0) success (T0) string" },
	{ E_GrP4ApiCtlgLogNext,			L"CtlgLogNext :: out : (0) success (1) count (2) total (3) page count (4) now page" },
	{ E_GrP4ApiCtlgLogPrev,			L"CtlgLogPrev :: out : (0) success (1) count (2) total (3) page count (4) now page" },

};

#endif



//====================================================================
//--------------------------------------------------------------------
BOOL8	GrP4ApiCall(__u16 A_ApiId, __s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		BOOL8	Tv_Result;
#ifdef GR_P4_EDITOR
		BOOL8	Tv_IsFound;
		__u16	Tv_WkIdx;
		Def_StrShort	Tv_StrTxt;
#endif
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ApiTblCnt > A_ApiId )
		{
			if ( NULL != V_GrP4ApiTbl[A_ApiId] )
			{
				Tv_Result	=	TRUE;
				V_GrP4ApiTbl[A_ApiId](A_PtrRint, A_PtrRstr, A_ObjHost);
			}
			else
			{
#ifdef GR_P4_EDITOR
				Tv_IsFound	=	FALSE;
				for (Tv_WkIdx=0;Tv_WkIdx < E_GrP4ApiCnt;Tv_WkIdx++)
				{
					if ( A_ApiId == V_GrP4ApiInfoTbl[Tv_WkIdx].Cmd )
					{
						GrStrWstrToStr( Tv_StrTxt, V_GrP4ApiInfoTbl[Tv_WkIdx].StrName );
						DbgMsgPrint("GrP4ApiCall - API (%d)[ %s ] not implemented.\n", A_ApiId, Tv_StrTxt );
						Tv_IsFound	=	TRUE;
						break;
					}
				}
				if ( !Tv_IsFound )
				{
					DbgMsgPrint("GrP4ApiCall - bad API (%d) call\n", A_ApiId);
				}
#else
				DbgMsgPrint("GrP4ApiCall - API (%d) not implemented.\n", A_ApiId);	
#endif
			}
		}
		else
		{
			DbgMsgPrint("GrP4ApiCall - bad API (%d) call\n", A_ApiId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrP4ApiRegist( __u16 A_ApiId, Fnc_GrP4Api A_FncPtr )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( E_GrP4ApiTblCnt > A_ApiId )
		{
			if ( NULL != V_GrP4ApiTbl[A_ApiId] )
			{
				DbgMsgPrint("GrP4ApiRegist - API (%d) overwrite\n", A_ApiId);
			}
			V_GrP4ApiTbl[A_ApiId]	=	A_FncPtr;
		}
		else
		{
			DbgMsgPrint("GrP4ApiRegist - bad API (%d) ID input\n", A_ApiId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Fnc_GrP4Api	GrP4ApiPtrGet( __u16 A_ApiId )
{
	// local -------------------
		Fnc_GrP4Api	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrP4ApiTblCnt > A_ApiId )
		{
			Tv_Result	=	V_GrP4ApiTbl[A_ApiId];
		}
		else
		{
			DbgMsgPrint("GrP4ApiPtrGet - bad API (%d) ID input\n", A_ApiId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrP4ApiCfgLangUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8*	Tv_PtrBt;
	// code --------------------
		Tv_PtrBt	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsLang );
		V_GrP4Mng->LangSelSet( *Tv_PtrBt );
}
//--------------------------------------------------------------------
void	GrP4ApiCfgReset(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4ScptCfgMdl	Tv_PtrCfgMdl;
		void*		Tv_PtrCfg;
		__u32*	Tv_PtrDw;
		__u16*	Tv_PtrWd;
		St_GrP4MngVmRpara	Tv_Rpara;
	// code --------------------
		// init
		Tv_PtrCfg	=	(__u32*)V_GrP4Mng->SetupDataPtrGet( 0 );
		// check factory default
		if ( 0 != A_PtrRint[1] )
		{
			// reset all data
			GrDumyZeroMem( Tv_PtrCfg, E_GrP4MngSdaSize );
		}

		// build header
		// make FCC
		Tv_PtrDw	=	(__u32*)( (__u32)Tv_PtrCfg + E_GrP4CfgOfsFcc );
		*Tv_PtrDw	=	E_GrP4CfgFccId;
		// set firmware version
		Tv_PtrWd	=	(__u16*)( (__u32)Tv_PtrCfg + E_GrP4CfgOfsCfgVer );
		*Tv_PtrWd	=	(__u16)V_GrP4Mng->FirmwareVerGet();
		// model number 
		Tv_PtrWd	=	(__u16*)( (__u32)Tv_PtrCfg + E_GrP4CfgOfsModel );
		*Tv_PtrWd	=	(__u16)V_GrP4Mng->FirmwareVerGet();

		// quad map reset
		GrDvrQuadMapReset( V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsQuadMap), V_GrP4Mng->VdoMaxChCntGet() );

		// ADMIN privilege
		Tv_PtrWd	=	(__u16*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsUserAuth );
		*Tv_PtrWd	=	0xFFFF;

		// run configuration reset code
		Tv_PtrCfgMdl	=	V_GrP4Mng->ConfigMdlAtbGet();
		if ( E_GrP4AsmIdNone != Tv_PtrCfgMdl->AsmCfgReset )
		{
			Tv_Rpara.In.Cnt		=	0;

			Tv_Rpara.In.Ridx[Tv_Rpara.In.Cnt]	=	1;
			Tv_Rpara.In.Rval[Tv_Rpara.In.Cnt]	=	A_PtrRint[1];
			Tv_Rpara.In.Cnt ++;

			Tv_Rpara.Out.Cnt	=	0;

			V_GrP4Mng->VmRunCode( Tv_PtrCfgMdl->AsmCfgReset, NULL, &Tv_Rpara, TRUE );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiQuadModeNext(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8	Tv_ConIdx;
		Cls_GrP4DoBase*	Tv_HostObj;
		Cls_GrP4DoWin*	Tv_HostWin;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP4MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	(Cls_GrP4DoBase*)A_ObjHost;
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP4DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP4ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP4Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeNext();
		}
		else
		{
			DbgMsgPrint( "GrP4ApiQuadModeNext - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiQuadModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8	Tv_ConIdx;
		Cls_GrP4DoBase*	Tv_HostObj;
		Cls_GrP4DoWin*	Tv_HostWin;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP4MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	(Cls_GrP4DoBase*)A_ObjHost;
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP4DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP4ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP4Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeSet( (__u8)A_PtrRint[2], (__u8)A_PtrRint[3] );
		}
		else
		{
			DbgMsgPrint( "GrP4ApiQuadModeSet - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiQuadModeGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8	Tv_ConIdx;
		__u8	Tv_Mode;
		__u8	Tv_Page;
		Cls_GrP4DoBase*	Tv_HostObj;
		Cls_GrP4DoWin*	Tv_HostWin;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon	=	NULL;

		Tv_ConIdx	=	(__u8)A_PtrRint[1];
		if ( E_GrP4MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	(Cls_GrP4DoBase*)A_ObjHost;
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP4DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP4ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP4Mng->ConsoleGet( Tv_ConIdx );
		}

		// check exist console
		if ( NULL != Tv_ObjCon )
		{
			Tv_ObjCon->QuadModeGet( &Tv_Mode, &Tv_Page );
			A_PtrRint[0]	=	(__s32)Tv_Mode;
			A_PtrRint[1]	=	(__s32)Tv_Page;
		}
		else
		{
			DbgMsgPrint( "GrP4ApiQuadModeGet - bad console index ( %d ).\n " , Tv_ConIdx );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPlayModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
	// code --------------------
		V_GrP4Mng->RtlPlayModeSet( (BOOL8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP4ApiCfgAssign(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		// call BIOS
		
		GrP4BiosCfgAssign( A_PtrRint, A_PtrRstr, A_ObjHost );
		// assign setup to p3 manager
		V_GrP4Mng->RtlCfgUpdate();
}
//--------------------------------------------------------------------
void	GrP4ApiFocusIdGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrP4ConBase*	Tv_Con;
		Cls_GrP4DoBase*		Tv_Obj;
	// code --------------------
		A_PtrRint[0]	=	E_GrP4ScptIdNone;

		Tv_Con	=	V_GrP4Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Obj	=	Tv_Con->FocusObjGet();
			if ( NULL != Tv_Obj )
			{
				A_PtrRint[0]	=	(__s32)Tv_Obj->ScptIdGet();
			}
		}
		
}
//--------------------------------------------------------------------
void	GrP4ApiCtlgItemValGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		A_PtrRint[0]	=	(__s32)V_GrP4Mng->CtlgItemValueGet( (__u32)A_PtrRint[1], (__u8)A_PtrRint[2], A_PtrRint[3], &A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP4ApiCtlgItemStrGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		A_PtrRint[0]	=	(__s32)V_GrP4Mng->CtlgItemStrGet( (__u32)A_PtrRint[1], (__u8)A_PtrRint[2], A_PtrRint[3], A_PtrRstr[0] );
}
//--------------------------------------------------------------------
void	GrP4ApiTimeGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
	// code --------------------
		A_PtrRint[0]	=	(__s32)GrTimeGet( (__u32*)&A_PtrRint[1], (BOOL8*)&A_PtrRint[2] );
}
//--------------------------------------------------------------------
void	GrP4ApiUiAlphaUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8*	Tv_PtrVal;
		__s32		Tv_Rint[2];
	// code --------------------
		Tv_PtrVal	=	(__u8*)V_GrP4Mng->SetupDataPtrGet( E_GrP4CfgOfsOsdAlpha );
#ifndef GR_P4_EDITOR
		V_GrP4Mng->UiAlpahSet( *Tv_PtrVal );
#endif
		Tv_Rint[1]	=	(__s32)*Tv_PtrVal;
		GrP4BiosHwAlphaUpdt(Tv_Rint, NULL, A_ObjHost);
}
//--------------------------------------------------------------------
void	GrP4ApiFwVerGet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		A_PtrRint[0]	=	V_GrP4Mng->FirmwareVerGet();
}
//--------------------------------------------------------------------
void	GrP4ApiCfgDateFmtUpdt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		V_GrP4Mng->RtlDateDispFmtUpdt();
}
//--------------------------------------------------------------------
void	GrP4ApiQuadPageNext(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u8		Tv_ConIdx;
		Cls_GrP4DoBase*	Tv_HostObj;
		Cls_GrP4DoWin*	Tv_HostWin;
		Cls_GrP4ConBase*	Tv_ObjCon;
	// code --------------------
		Tv_ObjCon		= NULL;
		Tv_ConIdx		= (__u8)A_PtrRint[1];

		if ( E_GrP4MngConMaxCnt <= Tv_ConIdx )
		{
			Tv_HostObj	=	(Cls_GrP4DoBase*)A_ObjHost;
			if ( NULL != Tv_HostObj )
			{
				Tv_HostWin	=	(Cls_GrP4DoWin*)Tv_HostObj->WindowGet();
				Tv_ObjCon		=	(Cls_GrP4ConBase*)Tv_HostWin->ConsoleGet();
			}
		}
		else
		{
			Tv_ObjCon	=	V_GrP4Mng->ConsoleGet( Tv_ConIdx );
		}

		if(NULL != Tv_ObjCon)
		{
			Tv_ObjCon->QuadPageNext();
		}
		else
		{
			DbgMsgPrint( "GrP4ApiQuadPageNext - bad console index ( %d ).\n " , Tv_ConIdx );
		}

}
//--------------------------------------------------------------------
void	GrP4ApiLogIn(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		__u32	Tv_Tidx1;
		__u32	Tv_Tidx2;
	// code --------------------
		A_PtrRint[0]	=	FALSE;
		Tv_Tidx1	=	(__u32)A_PtrRint[2];
		Tv_Tidx2	=	(__u32)A_PtrRint[3];
		if ( (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx1) && (E_GrP4AsmMaxTxtRegCnt > Tv_Tidx2) )
		{
			if ( V_GrP4Mng->RtlLogIn( (__u8)A_PtrRint[1], A_PtrRstr[Tv_Tidx1], A_PtrRstr[Tv_Tidx2] ) )
			{
				A_PtrRint[0]	=	TRUE;
			}
		}
		else
		{
			DbgMsgPrint( "GrP4ApiLogIn - text register index over.\n" );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiLogOut(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
	// code --------------------
		V_GrP4Mng->RtlLogOut();
}
//--------------------------------------------------------------------
void	GrP4ApiPasswdRuleChk(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
	// code --------------------
		A_PtrRint[0]	=	FALSE;
		if ( V_GrP4Mng->RtlPasswdRuleChk() )
		{
			A_PtrRint[0]	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	GrP4ApiRecEmgcSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		V_GrP4Mng->RtlTryRecEmgc( (BOOL8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP4ApiNetModelSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		Tv_PtrGvs->NetModel	=	(__u32)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP4ApiKeyBlockSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		V_GrP4Mng->KeyBlockSet( (__u8)A_PtrRint[1] );
}
//--------------------------------------------------------------------
void	GrP4ApiLayerUpClose(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		St_GrP4MsgLayer	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.Cmd			=	E_GrP4MsgCmdConLayUpClose;
		Tv_Msg.Hd.ConIdx	=	(__u8)A_PtrRint[1];
		Tv_Msg.Hd.WinId		=	E_GrP4WinIdNone;
		Tv_Msg.Hd.ObjId		=	E_GrP4ScptIdNone;
		Tv_Msg.Layer			=	(__u8)A_PtrRint[2];
		V_GrP4Mng->UiMsgPost( (Ptr_GrP4MsgHd)&Tv_Msg, sizeof(Tv_Msg) );

}
//--------------------------------------------------------------------
void	GrP4ApiAdoLiveChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		Tv_PtrGvs->AdoLiveCh	=	(__u8)A_PtrRint[1];
		GrP4BiosAdoLiveChSet(A_PtrRint, A_PtrRstr, A_ObjHost);
}
//--------------------------------------------------------------------
void	GrP4ApiAdoLiveVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
		__u8	Tv_Ch;
		__u8	Tv_Vol;
	// code --------------------
		Tv_Ch	=	(__u8)A_PtrRint[1];
		if ( E_GrDvrMaxChCnt > Tv_Ch )
		{
			Tv_Vol	=	(__u8)A_PtrRint[2];
			if ( 100 >= Tv_Vol )
			{
				Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
				Tv_PtrGvs->AdoLiveVol[Tv_Ch]	=	Tv_Vol;
				GrP4BiosAdoLiveVolSet(A_PtrRint, A_PtrRstr, A_ObjHost);
			}
			else
			{
				DbgMsgPrint( "GrP4ApiAdoLiveVolSet - bad volume value %d \n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP4ApiAdoLiveVolSet - bad channel number %d \n", A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiAdoPlayChSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		Tv_PtrGvs->AdoPlayCh	=	(__u8)A_PtrRint[1];
		GrP4BiosAdoPlayChSet( A_PtrRint, A_PtrRstr,A_ObjHost );
}
//--------------------------------------------------------------------
void	GrP4ApiAdoPlayVolSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
		__u8	Tv_Ch;
		__u8	Tv_Vol;
	// code --------------------
		Tv_Ch	=	(__u8)A_PtrRint[1];
		if ( E_GrDvrMaxChCnt > Tv_Ch )
		{
			Tv_Vol	=	(__u8)A_PtrRint[2];
			if ( 100 >= Tv_Vol )
			{
				Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
				Tv_PtrGvs->AdoPlayVol[Tv_Ch]	=	Tv_Vol;
				GrP4BiosAdoPlayVolSet(A_PtrRint, A_PtrRstr, A_ObjHost);
			}
			else
			{
				DbgMsgPrint( "GrP4ApiAdoPlayVolSet - bad volume value %d \n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP4ApiAdoPlayVolSet - bad channel number %d \n", A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzPanTilt(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPanTilt( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], (BOOL8)A_PtrRint[3], (BOOL8)A_PtrRint[4], (BOOL8)A_PtrRint[5],
				E_GrPtzV2DfltSpeed, E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzZoom(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoZoom( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzFocus(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoFocus( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzIris(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoIris( (__u8)A_PtrRint[1], (BOOL8)A_PtrRint[2], E_GrPtzV2DfltSpeed, 0 );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzStop(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoStop( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzAutoFocus(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoAutoFocus( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuCall(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuCall( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuExit(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuExit( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuLeft(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuLeft( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuRight(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuRight( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuUp(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuUp( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuDown(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuDown( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuOk(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuOk( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzMenuCancel(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoMenuCancel( (__u8)A_PtrRint[1] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzPresetSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPresetSet( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2] );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzPresetMove(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeDoPresetMove( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2], E_GrPtzV2DfltSpeed );
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzTourRun(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		Ptr_GrP4Gvs	Tv_PtrGvs;
		__u16		Tv_Msk;
		__u16		Tv_TourMsk;
		BOOL8		Tv_IsSaveMode;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();

		if ( NULL != Tv_PtzMng )
		{
			Tv_PtzMng->SafeTouringRun( (__u8)A_PtrRint[1], (__u8)A_PtrRint[2] );

			Tv_TourMsk		= Tv_PtrGvs->PtzTourMsk;
			Tv_IsSaveMode	= TRUE;
			if(0 != (__u8)A_PtrRint[2])
			{
				Tv_Msk = 1 << ((__u8)A_PtrRint[1]);
				Tv_TourMsk = Tv_PtrGvs->PtzTourMsk | Tv_Msk;
			}
			else
			{
				Tv_Msk = ~(1 << ((__u8)A_PtrRint[1]));
				Tv_TourMsk = Tv_PtrGvs->PtzTourMsk & Tv_Msk;

				if(Tv_TourMsk == Tv_PtrGvs->PtzTourMsk)
				{
					Tv_IsSaveMode		= FALSE;
				}
			}
			Tv_PtrGvs->PtzTourMsk = Tv_TourMsk;

			if(Tv_IsSaveMode)
			{
				GrP4ApiCall(E_GrP4ApiRgstSave, NULL, NULL, A_ObjHost);
			}
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzTourSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		void*	Tv_PtrStp;
		__u8	Tv_Ch;
		__u16	Tv_Ofs;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch	=	(__u8)A_PtrRint[1];
			if ( Tv_Ch < V_GrP4Mng->VdoMaxChCntGet() )
			{
				// calculate channel
				Tv_Ofs		=	( ((__u16)Tv_Ch) * 12 * 4) + E_GrP4CfgOfsPtzTourCfg;
				Tv_PtrStp	=	V_GrP4Mng->SetupDataPtrGet( Tv_Ofs );

				Tv_PtzMng->SafeTourSave( Tv_Ch, (Ptr_GrPtzV2TourItm)Tv_PtrStp, 12 );
			}
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzIdView(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		__u32	Tv_Ch;
		__u8*	Tv_PtrStp;
		__u32	Tv_Baud;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch			=	(__u32)A_PtrRint[1];
			if ( E_GrDvrMaxChCnt > Tv_Ch )
			{
				Tv_PtrStp	=	(__u8*)V_GrP4Mng->SetupDataPtrGet(0);
				Tv_Baud		=	9600;
				if ( E_GrP4CfgUartSpeedCnt > Tv_PtrStp[E_GrP4CfgOfsPtzBaud + Tv_Ch] )
				{
					Tv_Baud	=	V_GrP4UartSpeedTbl[Tv_PtrStp[E_GrP4CfgOfsPtzBaud + Tv_Ch]];
				}
				Tv_PtzMng->SafeExIdView( Tv_PtrStp[E_GrP4CfgOfsPtzType + Tv_Ch], Tv_PtrStp[E_GrP4CfgOfsPtzId + Tv_Ch],
					Tv_PtrStp[E_GrP4CfgOfsPtzPort + Tv_Ch], Tv_Baud, Tv_PtrStp[E_GrP4CfgOfsPtzDataBit + Tv_Ch],
					Tv_PtrStp[E_GrP4CfgOfsPtzParity + Tv_Ch], Tv_PtrStp[E_GrP4CfgOfsPtzStopBit + Tv_Ch], 
					(BOOL8)A_PtrRint[2] );
			}
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzIdSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrPtzV2Mng*	Tv_PtzMng;
		__u32	Tv_Ch;
		__u8*	Tv_PtrStp;
		__u32	Tv_Baud;
	// code --------------------
		Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
		if ( NULL != Tv_PtzMng )
		{
			Tv_Ch			=	(__u32)A_PtrRint[1];
			if ( E_GrDvrMaxChCnt > Tv_Ch )
			{
				Tv_PtrStp	=	(__u8*)V_GrP4Mng->SetupDataPtrGet(0);
				Tv_Baud		=	9600;
				if ( E_GrP4CfgUartSpeedCnt > Tv_PtrStp[E_GrP4CfgOfsPtzBaud + Tv_Ch] )
				{
					Tv_Baud	=	V_GrP4UartSpeedTbl[Tv_PtrStp[E_GrP4CfgOfsPtzBaud + Tv_Ch]];
				}
				Tv_PtzMng->SafeExIdSet( Tv_PtrStp[E_GrP4CfgOfsPtzType + Tv_Ch], Tv_PtrStp[E_GrP4CfgOfsPtzId + Tv_Ch],
					Tv_PtrStp[E_GrP4CfgOfsPtzPort + Tv_Ch], Tv_Baud, Tv_PtrStp[E_GrP4CfgOfsPtzDataBit + Tv_Ch],
					Tv_PtrStp[E_GrP4CfgOfsPtzParity + Tv_Ch], Tv_PtrStp[E_GrP4CfgOfsPtzStopBit + Tv_Ch], 
					(__u8)A_PtrRint[2] );
			}
		}
}
//--------------------------------------------------------------------
void	GrP4ApiPtzModelTxtAdd(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrP4ConBase*	Tv_Con;
		Cls_GrP4DoWin*		Tv_Win;
		Cls_GrP4DoBase*		Tv_Obj;
		WCHAR*	Tv_Rstr[2];
		__u8	Tv_ModelCnt;
		__u8	Tv_MdlIdx;
		Cls_GrPtzV2Mng*	Tv_PtzMng;
	// code --------------------
		Tv_Con	=	V_GrP4Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Win	=	Tv_Con->WinFindByScptId( (__u16)A_PtrRint[2] );
			if ( NULL != Tv_Win )
			{
				Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( (__u16)A_PtrRint[3] );
				if ( NULL != Tv_Obj )
				{
					Tv_PtzMng	=	V_GrP4Mng->PtzMngGet();
					if ( NULL != Tv_PtzMng )
					{
						// update
						Tv_ModelCnt	=	Tv_PtzMng->ModelCntGet();
						for ( Tv_MdlIdx=0;Tv_MdlIdx <= Tv_ModelCnt;Tv_MdlIdx++ )
						{
							Tv_Rstr[0]	=	Tv_PtzMng->ModelNameGet( Tv_MdlIdx );
							Tv_Obj->RtlCommand( E_GrP4UpiTextAdd, A_PtrRint, Tv_Rstr );
						}
					}
				}
				else
				{
					DbgMsgPrint( "GrP4ApiPtzModelTxtAdd - bad object script id.\n", A_PtrRint[3] );
				}
			}
			else
			{
				DbgMsgPrint( "GrP4ApiPtzModelTxtAdd - bad window script id.\n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP4ApiPtzModelTxtAdd - bad console index.\n", A_PtrRint[1] );
		}

}
//--------------------------------------------------------------------
void	GrP4ApiCtrlModelTxtAdd(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Cls_GrP4ConBase*	Tv_Con;
		Cls_GrP4DoWin*		Tv_Win;
		Cls_GrP4DoBase*		Tv_Obj;
		WCHAR*	Tv_Rstr[2];
		Def_WstrTag	Tv_StrTmp;
		__u8	Tv_ModelCnt;
		__u8	Tv_MdlIdx;
		Cls_GrP4SioMng*	Tv_SioMng;
	// code --------------------
		Tv_Con	=	V_GrP4Mng->ConsoleGet( (__u8)A_PtrRint[1] );
		if ( NULL != Tv_Con )
		{
			Tv_Win	=	Tv_Con->WinFindByScptId( (__u16)A_PtrRint[2] );
			if ( NULL != Tv_Win )
			{
				Tv_Obj	=	Tv_Win->FindSlaveObjByScptId( (__u16)A_PtrRint[3] );
				if ( NULL != Tv_Obj )
				{
					Tv_SioMng	=	V_GrP4Mng->SioMngGet();
					if ( NULL != Tv_SioMng )
					{
						// update
						Tv_Rstr[0]	=	Tv_StrTmp;
						Tv_Rstr[1]	=	Tv_StrTmp;
						Tv_ModelCnt	=	Tv_SioMng->ModelCntGet();
						for ( Tv_MdlIdx=0;Tv_MdlIdx < Tv_ModelCnt;Tv_MdlIdx++ )
						{
							Tv_SioMng->ModelNameGet( Tv_MdlIdx, Tv_Rstr[0] );
							Tv_Obj->RtlCommand( E_GrP4UpiTextAdd, A_PtrRint, Tv_Rstr );
						}
					}
				}
				else
				{
					DbgMsgPrint( "GrP4ApiCtrlModelTxtAdd - bad object script id.\n", A_PtrRint[3] );
				}
			}
			else
			{
				DbgMsgPrint( "GrP4ApiCtrlModelTxtAdd - bad window script id.\n", A_PtrRint[2] );
			}
		}
		else
		{
			DbgMsgPrint( "GrP4ApiCtrlModelTxtAdd - bad console index.\n", A_PtrRint[1] );
		}

}
//--------------------------------------------------------------------
void	GrP4ApiRmtTmpAble(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		Tv_PtrGvs->IsRmtTmpAble	=	(BOOL8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP4ApiRmtIdSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		Tv_PtrGvs->RunRmtId	=	(__u8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP4ApiAutoSeqSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
		V_GrP4Mng->RtlDispAutoSeqSet( (BOOL8)A_PtrRint[1] );
#ifdef GR_P4_EDITOR
		if ( 0 == A_PtrRint[1] )
		{
			DbgMsgPrint( "Auto sequence off!\n" );
		}
		else
		{
			DbgMsgPrint( "Auto sequence on!\n" );
		}
#endif
}
//--------------------------------------------------------------------
void	GrP4ApiPtzCtrlLock(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();
		
		Tv_PtrGvs->IsPtzCtrlLock	=	(BOOL8)A_PtrRint[1];
}
//--------------------------------------------------------------------
void	GrP4ApiUiMenuModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
		__s32		Tv_Rint[6];
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();

		// check enter setup mode
		if ( (1 != Tv_PtrGvs->MenuMode) && (1 == A_PtrRint[1]) )
		{
			Tv_Rint[1]	=	E_GrDvrLogTypeSetupIn;
			Tv_Rint[2]	=	E_GrDvrLogChAny;
			Tv_Rint[3]	=	(__s32)Tv_PtrGvs->LoginIdx;
			Tv_Rint[4]	=	0;
			Tv_Rint[5]	=	0;
			GrP4ApiCall(E_GrP4ApiEvtLogAdd, Tv_Rint, NULL, A_ObjHost);
		}
		// check leave setup mode
		if ( (1 == Tv_PtrGvs->MenuMode) && (1 != A_PtrRint[1]) )
		{
			Tv_Rint[1]	=	E_GrDvrLogTypeSetupOut;
			Tv_Rint[2]	=	E_GrDvrLogChAny;
			Tv_Rint[3]	=	(__s32)Tv_PtrGvs->LoginIdx;
			Tv_Rint[4]	=	0;
			Tv_Rint[5]	=	0;
			GrP4ApiCall(E_GrP4ApiEvtLogAdd, Tv_Rint, NULL, A_ObjHost);
		}

		// update menu mode
		Tv_PtrGvs->MenuMode	=	(__u8)A_PtrRint[1];
		DbgMsgPrint( "MenuMode = %d\n", Tv_PtrGvs->MenuMode );
}
//--------------------------------------------------------------------
void	GrP4ApiUiLoginModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();

		Tv_PtrGvs->IsLoginMenu	=	(BOOL8)A_PtrRint[1];

		DbgMsgPrint( "LoginMode = %d\n", Tv_PtrGvs->IsLoginMenu );
}
//--------------------------------------------------------------------
void	GrP4ApiUiMsgBoxModeSet(__s32* A_PtrRint, WCHAR** A_PtrRstr, void* A_ObjHost)
{
	// local -------------------
		Ptr_GrP4Gvs	Tv_PtrGvs;
	// code --------------------
		Tv_PtrGvs	=	(Ptr_GrP4Gvs)V_GrP4Mng->GvsPtrGet();

		Tv_PtrGvs->MsgBoxMode	=	(__u8)A_PtrRint[1];

		DbgMsgPrint( "MsgBoxMode = %d\n", Tv_PtrGvs->MsgBoxMode );
}
//--------------------------------------------------------------------

