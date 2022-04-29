/*
Platform 5 UI scroll bar standard

*/

//--------------------------------------------------------------------
// uses

#include <P5/GrP5UoSlidHzSint.h>

#include <GrDebug.h>
#include <GrStrTool.h>
#include <GrDumyTool.h>
#include <P5/GrP5ConBase.h>
#include <P5/GrP5SkinBase.h>
#include <P5/GrP5CtrlEvtBase.h>

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

#define	E_GrP5UoSlidHzSintParaInfoCnt							3

St_GrP5DvlScptParaInfo	V_GrP5UoSlidHzSintParaInfo[E_GrP5UoSlidHzSintParaInfoCnt]	=
{
	{L"Slid setup integer", E_GrP5DvlValTypeSeparator, 1, NULL, FALSE, 0, NULL},
	{L"StpAdr", E_GrP5DvlValTypeSetupAdr, 1, (void*)&((Ptr_GrP5UoSlidHzSintPara)0)->StpAdr, TRUE, E_GrP5DvlParaInfoActDefault, L"Setup address"},
	{L"VarSize", E_GrVarTypeVarSize, 1, (void*)&((Ptr_GrP5UoSlidHzSintPara)0)->VarSize, TRUE, E_GrP5DvlParaInfoActDefault, L"Variable size"},
};


#endif

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzSint::Cls_GrP5UoSlidHzSint(__u16 A_ScptId, Cls_GrP5UoBase* A_ObjPrnt, Ptr_GrP5RelRect A_PtrRelRect ) :
	Cls_GrP5UoSlidHzBase(A_ScptId, A_ObjPrnt, A_PtrRelRect)
{
	// local -------------------
	// code --------------------
		// init
		ClassRegist(E_GrP5ClassIdSlidHzSint, NULL, 0, NULL);
		

		// default setting
		GrDumyZeroMem(&m_ParaUoSlidHzSint, sizeof(m_ParaUoSlidHzSint));

}
//--------------------------------------------------------------------
Cls_GrP5UoSlidHzSint::~Cls_GrP5UoSlidHzSint(void)
{
	// local -------------------
	// code --------------------
		

}
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzSint::DvlParaInfoBuild(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSlidHzBase::DvlParaInfoBuild();

		// add
		DvlParaInfoAdd(V_GrP5UoSlidHzSintParaInfo, E_GrP5UoSlidHzSintParaInfoCnt, &m_ParaUoSlidHzSint);

}
#endif
//--------------------------------------------------------------------
#ifdef GR_P5_EDITOR
void	Cls_GrP5UoSlidHzSint::DvlParaInfoAction(__u32 A_Action)
{
	// local -------------------
	// code --------------------

		RtlValueRead();
		// inherited
		Cls_GrP5UoSlidHzBase::DvlParaInfoAction(A_Action);

}
#endif
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzSint::EvtScptDecDone(void)
{
	// local -------------------
	// code --------------------

		// inherited
		Cls_GrP5UoSlidHzBase::EvtScptDecDone();
		RtlValueRead();

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzSint::RtlValueRead(void)
{
	// local -------------------
		__s32	Tv_Val;
	// code --------------------
		switch(m_ParaUoSlidHzSint.VarSize)
		{
			case E_GrVarSize2byte:
				Tv_Val	=	(__s32)GrP5SetupGetU16(m_ParaUoSlidHzSint.StpAdr);
				break;
			case E_GrVarSize4byte:
				Tv_Val	=	(__s32)GrP5SetupGetU32(m_ParaUoSlidHzSint.StpAdr);
				break;
			default:
				Tv_Val	=	(__s32)GrP5SetupGetU8(m_ParaUoSlidHzSint.StpAdr);
				break;
		}
		// check value range
		if(Tv_Val < m_ParaUoSlidBase.ValMin)
		{
			Tv_Val	=	m_ParaUoSlidBase.ValMin;
		}
		if(Tv_Val > m_ParaUoSlidBase.ValMax)
		{
			Tv_Val	=	m_ParaUoSlidBase.ValMax;
		}

		m_Value		=	Tv_Val;

}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzSint::RtlValueWrite(void)
{
	// local -------------------
	// code --------------------
		switch(m_ParaUoSlidHzSint.VarSize)
		{
			case E_GrVarSize2byte:
				GrP5SetupPutU16(m_ParaUoSlidHzSint.StpAdr, (__u16)m_Value);
				break;
			case E_GrVarSize4byte:
				GrP5SetupPutU32(m_ParaUoSlidHzSint.StpAdr, (__u32)m_Value);
				break;
			default:
				GrP5SetupPutU8(m_ParaUoSlidHzSint.StpAdr, (__u8)m_Value);
				break;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5UoSlidHzSint::SetupAdrSet(__u16 A_Adr)
{
		m_ParaUoSlidHzSint.StpAdr	=	A_Adr;
}
//--------------------------------------------------------------------
__u16	Cls_GrP5UoSlidHzSint::SetupAdrGet(__u16 A_Adr)
{
		return	m_ParaUoSlidHzSint.StpAdr;
}
//--------------------------------------------------------------------

