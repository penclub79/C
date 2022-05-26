/*
	Platform 5 Text

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP5Text
#define	_EPC_GrP5Text

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>


//====================================================================
//constance

// base
#define	E_GrP5TxtIdNone						0			/* none */

#define	E_GrP5TxtLanguageIdAuto		0xFF	/* auto language */

// dedicated text (fix text)
#define	E_GrP5TxtIdFixBase			1

// language
#define E_GrP5TxtIdLang0					(0 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLang1					(1 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang2					(2 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang3					(3 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang4					(4 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang5					(5 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang6					(6 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang7					(7 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang8					(8 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang9					(9 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang10					(10 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang11					(11 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang12					(12 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang13					(13 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang14					(14 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang15					(15 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang16					(16 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang17					(17 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang18					(18 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang19					(19 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang20					(20 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang21					(21 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang22					(22 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang23					(23 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang24					(24 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang25					(25 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang26					(26 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang27					(27 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang28					(28 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdLang29					(29 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLang30					(30 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLang31					(31 + E_GrP5TxtIdFixBase) 

// day unit
#define E_GrP5TxtIdUnitYear				(32 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdUnitMonth			(33 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdUnitDay				(34 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdUnitHour				(35 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdUnitMin				(36 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdUnitSec				(37 + E_GrP5TxtIdFixBase) 

// am / pm
#define E_GrP5TxtIdTimeAm					(38 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdTimePm					(39 + E_GrP5TxtIdFixBase) 

// month
#define E_GrP5TxtIdMonth1					(40 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth2					(41 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth3					(42 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth4					(43 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth5					(44 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth6					(45 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth7					(46 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth8					(47 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth9					(48 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth10				(49 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth11				(50 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdMonth12				(51 + E_GrP5TxtIdFixBase) 

// week
#define E_GrP5TxtIdWeekSun				(52 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekMon				(53 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekTue				(54 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekWed				(55 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekThu				(56 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekFri				(57 + E_GrP5TxtIdFixBase) 
#define E_GrP5TxtIdWeekSat				(58 + E_GrP5TxtIdFixBase) 

// noun - 1
#define E_GrP5TxtIdChannel				(59 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdBegining				(60 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdEndining				(61 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdResolution			(62 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtMotion			(63 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdVideoQuility		(64 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdRecSpeed				(65 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtSensor			(66 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtVdoloss			(67 + E_GrP5TxtIdFixBase)
#define	E_GrP5TxtIdMotion					(68 + E_GrP5TxtIdFixBase)
#define	E_GrP5TxtIdSensor					(69 + E_GrP5TxtIdFixBase)
#define	E_GrP5TxtIdAlarm					(70 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtVdoRcvy			(71 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtPowerOn			(72 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdAudio					(73 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtPowerOff		(74 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdPassword				(75 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCamera					(76 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtLogin				(77 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdNetwork				(78 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLive						(79 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdSearch					(80 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdBackup					(81 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdSetup					(82 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtLogout			(83 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtSetupIn			(84 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtSetupOut		(85 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtBkupSt			(86 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLogIn					(87 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLogOut					(88 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtBkupEd			(89 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdUsb						(90 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdOdd						(91 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdEvent					(92 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtHddErr			(93 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMacaddress			(94 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdIp							(95 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdPort						(96 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdSensitivity		(97 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtHddFull			(98 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtHddFormat		(99 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtFsFix				(100 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdSystem					(101 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtFwUpgd			(102 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdDate						(103 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdTime						(104 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtRtcErr			(105 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtNetLogin		(106 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdId							(107 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtNetDiscon		(108 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdEmail					(109 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdDdns						(110 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtNetSetup		(111 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtEmgcSt			(112 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtEmgyEd			(113 + E_GrP5TxtIdFixBase)

// verd - 1
#define E_GrP5TxtIdLgtCfgRecPara	(114 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtCfgMotion		(115 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtCfgIdPass		(116 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtCfgRecSchd	(117 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtCfgHdd			(118 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailTime				(119 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailEvtTitle		(120 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailMotion			(121 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdFormat					(122 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdEnter					(123 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailSensor			(124 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailVloss			(125 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailLogin			(126 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdUpgrade				(127 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailRecErr			(128 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailDskFull		(129 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdStop						(130 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdPlay						(131 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailDvrErr			(132 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailMacAdr			(133 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdFastForward		(134 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdFastRewind			(135 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtCfgChg			(136 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtReboot			(137 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLgtChgTime			(138 + E_GrP5TxtIdFixBase)

// quality
#define E_GrP5TxtIdHighest				(139 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdHigh						(140 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdNormal					(141 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdLow						(142 + E_GrP5TxtIdFixBase)

// channel number
#define E_GrP5TxtIdCh1						(143 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh2						(144 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh3						(145 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh4						(146 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh5						(147 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh6						(148 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh7						(149 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh8						(150 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh9						(151 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh10						(152 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh11						(153 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh12						(154 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh13						(155 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh14						(156 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh15						(157 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh16						(158 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh17						(159 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh18						(160 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh19						(161 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh20						(162 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh21						(163 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh22						(164 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh23						(165 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh24						(166 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh25						(167 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh26						(168 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh27						(169 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh28						(170 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh29						(171 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh30						(172 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh31						(173 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCh32						(174 + E_GrP5TxtIdFixBase)

#define E_GrP5TxtIdCfgInit				(175 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgPtz					(176 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgNet					(177 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgCamSeq			(178 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgSpot				(179 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgLoad				(180 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgSave				(181 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgNetFrm			(182 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdCfgDispRes			(183 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdMailSendErr		(184 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdWdogVdo				(185 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdWdogAdo				(186 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtILogSpcSen				(187 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtILogSoftErr			(188 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtILogFsUpgrade		(189 + E_GrP5TxtIdFixBase)
#define E_GrP5TxtIdHddTemp				(190 + E_GrP5TxtIdFixBase)

// dedicated text count
#define	E_GrP5TxtDedicatedCnt			E_GrP5TxtIdHddTemp

// user text
#define	E_GrP5TxtIdUserBase				1000			/* user text start */

//====================================================================
//global macro


//====================================================================
//global type


//====================================================================
//global class


//====================================================================
//global function

#ifdef GR_P5_EDITOR

WCHAR*	GrP5TextFixNameGet(__u16 A_TxtId);

#endif

//====================================================================
//out Environment conditional
#endif

