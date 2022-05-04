/*
Platform 5 UI object Button virtual key

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoBtnVkey.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5Base.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5WinVkbd.h>

#ifdef GR_P5_EDITOR

#include <P5\Dvl\GrP5DvlBase.h>

#endif

//--------------------------------------------------------------------
// local constant

//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;

#ifdef GR_P5_EDITOR

// relate position
#define	E_GrP5UoBtnVkeyParaInfoCnt							130

St_GrP5DvlScptParaInfo	V_GrP5UoBtnVkeyParaInfo[E_GrP5UoBtnVkeyParaInfoCnt]	=
{
	{L"Button text id", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"FontId", E_GrVarTypeU8, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->FontId, TRUE, E_GrP5DvlParaInfoActDefault, L"Font ID"},
	{L"Code0:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[0][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 0 : normal"},
	{L"Code0:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[0][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 0 : shift 1"},
	{L"Code0:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[0][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 0 : shift 2"},
	{L"Code0:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[0][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 0 : shift 3"},

	{L"Code1:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[1][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 1 : normal"},
	{L"Code1:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[1][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 1 : shift 1"},
	{L"Code1:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[1][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 1 : shift 2"},
	{L"Code1:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[1][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 1 : shift 3"},

	{L"Code2:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[2][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 2 : normal"},
	{L"Code2:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[2][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 2 : shift 1"},
	{L"Code2:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[2][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 2 : shift 2"},
	{L"Code2:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[2][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 2 : shift 3"},

	{L"Code3:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[3][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 3 : normal"},
	{L"Code3:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[3][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 3 : shift 1"},
	{L"Code3:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[3][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 3 : shift 2"},
	{L"Code3:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[3][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 3 : shift 3"},

	{L"Code4:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[4][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 4 : normal"},
	{L"Code4:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[4][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 4 : shift 1"},
	{L"Code4:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[4][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 4 : shift 2"},
	{L"Code4:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[4][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 4 : shift 3"},

	{L"Code5:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[5][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 5 : normal"},
	{L"Code5:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[5][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 5 : shift 1"},
	{L"Code5:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[5][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 5 : shift 2"},
	{L"Code5:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[5][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 5 : shift 3"},

	{L"Code6:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[6][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 6 : normal"},
	{L"Code6:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[6][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 6 : shift 1"},
	{L"Code6:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[6][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 6 : shift 2"},
	{L"Code6:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[6][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 6 : shift 3"},

	{L"Code7:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[7][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 7 : normal"},
	{L"Code7:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[7][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 7 : shift 1"},
	{L"Code7:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[7][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 7 : shift 2"},
	{L"Code7:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[7][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 7 : shift 3"},

	{L"Code8:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[8][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 8 : normal"},
	{L"Code8:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[8][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 8 : shift 1"},
	{L"Code8:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[8][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 8 : shift 2"},
	{L"Code8:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[8][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 8 : shift 3"},

	{L"Code9:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[9][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 9 : normal"},
	{L"Code9:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[9][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 9 : shift 1"},
	{L"Code9:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[9][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 9 : shift 2"},
	{L"Code9:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[9][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 9 : shift 3"},

	{L"Code10:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[10][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 10 : normal"},
	{L"Code10:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[10][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 10 : shift 1"},
	{L"Code10:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[10][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 10 : shift 2"},
	{L"Code10:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[10][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 10 : shift 3"},
	
	{L"Code11:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[11][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 11 : normal"},
	{L"Code11:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[11][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 11 : shift 1"},
	{L"Code11:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[11][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 11 : shift 2"},
	{L"Code11:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[11][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 11 : shift 3"},

	{L"Code12:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[12][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 12 : normal"},
	{L"Code12:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[12][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 12 : shift 1"},
	{L"Code12:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[12][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 12 : shift 2"},
	{L"Code12:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[12][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 12 : shift 3"},

	{L"Code13:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[13][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 13 : normal"},
	{L"Code13:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[13][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 13 : shift 1"},
	{L"Code13:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[13][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 13 : shift 2"},
	{L"Code13:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[13][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 13 : shift 3"},

	{L"Code14:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[14][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 14 : normal"},
	{L"Code14:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[14][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 14 : shift 1"},
	{L"Code14:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[14][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 14 : shift 2"},
	{L"Code14:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[14][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 14 : shift 3"},

	{L"Code15:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[15][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 15 : normal"},
	{L"Code15:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[15][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 15 : shift 1"},
	{L"Code15:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[15][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 15 : shift 2"},
	{L"Code15:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[15][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 15 : shift 3"},

	{L"Code16:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[16][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 16 : normal"},
	{L"Code16:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[16][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 16 : shift 1"},
	{L"Code16:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[16][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 16 : shift 2"},
	{L"Code16:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[16][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 16 : shift 3"},

	{L"Code17:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[17][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 17 : normal"},
	{L"Code17:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[17][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 17 : shift 1"},
	{L"Code17:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[17][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 17 : shift 2"},
	{L"Code17:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[17][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 17 : shift 3"},

	{L"Code18:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[18][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 18 : normal"},
	{L"Code18:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[18][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 18 : shift 1"},
	{L"Code18:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[18][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 18 : shift 2"},
	{L"Code18:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[18][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 18 : shift 3"},

	{L"Code19:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[19][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 19 : normal"},
	{L"Code19:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[19][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 19 : shift 1"},
	{L"Code19:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[19][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 19 : shift 2"},
	{L"Code19:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[19][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 19 : shift 3"},

	{L"Code20:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[20][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 20 : normal"},
	{L"Code20:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[20][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 20 : shift 1"},
	{L"Code20:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[20][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 20 : shift 2"},
	{L"Code20:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[20][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 20 : shift 3"},

	{L"Code21:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[21][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 21 : normal"},
	{L"Code21:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[21][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 21 : shift 1"},
	{L"Code21:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[21][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 21 : shift 2"},
	{L"Code21:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[21][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 21 : shift 3"},

	{L"Code22:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[22][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 22 : normal"},
	{L"Code22:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[22][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 22 : shift 1"},
	{L"Code22:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[22][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 22 : shift 2"},
	{L"Code22:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[22][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 22 : shift 3"},

	{L"Code23:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[23][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 23 : normal"},
	{L"Code23:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[23][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 23 : shift 1"},
	{L"Code23:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[23][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 23 : shift 2"},
	{L"Code23:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[23][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 23 : shift 3"},

	{L"Code24:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[24][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 24 : normal"},
	{L"Code24:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[24][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 24 : shift 1"},
	{L"Code24:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[24][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 24 : shift 2"},
	{L"Code24:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[24][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 24 : shift 3"},

	{L"Code25:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[25][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 25 : normal"},
	{L"Code25:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[25][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 25 : shift 1"},
	{L"Code25:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[25][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 25 : shift 2"},
	{L"Code25:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[25][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 25 : shift 3"},

	{L"Code26:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[26][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 26 : normal"},
	{L"Code26:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[26][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 26 : shift 1"},
	{L"Code26:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[26][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 26 : shift 2"},
	{L"Code26:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[26][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 26 : shift 3"},

	{L"Code27:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[27][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 27 : normal"},
	{L"Code27:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[27][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 27 : shift 1"},
	{L"Code27:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[27][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 27 : shift 2"},
	{L"Code27:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[27][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 27 : shift 3"},

	{L"Code28:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[28][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 28 : normal"},
	{L"Code28:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[28][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 28 : shift 1"},
	{L"Code28:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[28][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 28 : shift 2"},
	{L"Code28:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[28][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 28 : shift 3"},

	{L"Code29:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[29][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 29 : normal"},
	{L"Code29:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[29][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 29 : shift 1"},
	{L"Code29:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[29][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 29 : shift 2"},
	{L"Code29:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[29][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 29 : shift 3"},
	
	{L"Code30:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[30][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 30 : normal"},
	{L"Code30:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[30][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 30 : shift 1"},
	{L"Code30:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[30][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 30 : shift 2"},
	{L"Code30:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[30][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 30 : shift 3"},

	{L"Code31:0", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[31][0], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 31 : normal"},
	{L"Code31:1", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[31][1], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 31 : shift 1"},
	{L"Code31:2", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[31][2], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 31 : shift 2"},
	{L"Code31:3", E_GrVarTypeWchar, 1, (void*)&((Ptr_GrP5UoBtnVkeyPara)0)->Code[31][3], TRUE, E_GrP5DvlParaInfoActDefault, L"Code language 31 : shift 3"},
	
};

#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoBtnVkey::Cls_GrP5UoBtnVkey(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect) :
Cls_GrP5UoBtnBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdBtnVkey, &m_ParaUoBtnVkey, sizeof(m_ParaUoBtnVkey), &m_SkinCtlBtnVkey);
		
		// default setting
		GrDumyZeroMem(&m_ParaUoBtnVkey, sizeof(m_ParaUoBtnVkey));
		GrDumyZeroMem(&m_SkinCtlBtnVkey, sizeof(m_SkinCtlBtnVkey));

		m_ParaUoBtnVkey.TxtPara.Atb					=	E_GrP5FontAtbDrawText;
#ifdef GR_P5_EDITOR
		GrP5SkinDvlFontColorSet(&m_ParaUoBtnVkey.TxtPara);
#endif
		m_ParaUoBtnVkey.TxtRc.Right.Ratio		=	E_GrP5RelRectRatioBaseMax;
		m_ParaUoBtnVkey.TxtRc.Bottom.Ratio		=	E_GrP5RelRectRatioBaseMax;

		// reset 
		m_SkinCtlBtnVkey.LangIdx		=	0;
		m_SkinCtlBtnVkey.ShftIdx		=	0;

		// check parent
		m_IsPrntVkbd	=	FALSE;
		if(m_ObjWin->IsInheritClass(E_GrP5ClassIdWinVkbd))
		{
			m_IsPrntVkbd	=	TRUE;
		}

		m_SkinCtlBtnBase.Str	=	GrP5TxtBufAlloc(m_ObjId);
		if(NULL != m_SkinCtlBtnBase.Str)
		{
			GrStrClear(m_SkinCtlBtnBase.Str);
		}

}
//--------------------------------------------------------------------
Cls_GrP5UoBtnVkey::~Cls_GrP5UoBtnVkey(void)
{
	// local -------------------
	// code --------------------
		GrP5TxtBufFreeByObjId(m_ObjId);

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnVkey::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoBtnVkeyParaInfo, E_GrP5UoBtnVkeyParaInfoCnt, &m_ParaUoBtnVkey);
		DvlParaInfoTxtParaAdd(L"Button text para", &m_ParaUoBtnVkey.TxtPara);
		DvlParaInfoRelPosAdd(L"Button text rect", &m_ParaUoBtnVkey.TxtRc);
}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoBtnVkey::DvlParaInfoAction(__u32 A_Action)
{
		// inherited
		Cls_GrP5UoBtnBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnVkey::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------
		// inherited
		Cls_GrP5UoBtnBase::EvtScptDecDone();

		// update text
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnVkey::VkeyLanguageSet(__u32 A_LangIdx, __u32 A_ShftIdx)
{
	// local -------------------
	// code --------------------
		if((E_GrP5ScptLanguageMaxCnt > A_LangIdx) && (E_GrP5VkbdShftMax > A_ShftIdx))
		{
			m_SkinCtlBtnVkey.LangIdx	=	A_LangIdx;
			m_SkinCtlBtnVkey.ShftIdx	=	A_ShftIdx;
			ReqDraw(FALSE);
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnVkey::DoEvtBtnPress(void)
{

}
//--------------------------------------------------------------------
void	Cls_GrP5UoBtnVkey::DoEvtBtnRelease(void)
{
	// local -------------------
	// code --------------------
		if(m_IsPrntVkbd)
		{
			((Cls_GrP5WinVkbd*)m_ObjWin)->VkAddChar(m_ParaUoBtnVkey.Code[m_SkinCtlBtnVkey.LangIdx][m_SkinCtlBtnVkey.ShftIdx]);
		}
}
//--------------------------------------------------------------------
