/*
	Mpeg4 strema type
*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrMpeg4StrmTool
#define	_EPC_GrMpeg4StrmTool

//====================================================================
// uses
#include <GrTypeBase.h>
#include <Mpeg4/GrMpeg4Type.h>

//====================================================================
//constance

//====================================================================
//global macro


//====================================================================
//global type

//====================================================================
//class
/*
class Cls_GrMpg4Vol
{
	private:
		__u8		m_VolId;
		BOOL8		m_IsRandomAcc;
		__u8		m_VoTypeId;
		BOOL8		m_IsUseObjLayerId;
		__u8		m_VolVerId;
		__u8		m_VolPriority;
		__u8		m_AspectRatioInfo;
		__u8		m_ParWidth;
		__u8		m_ParHeight;
		BOOL8		m_IsUseVolCtrlPara;
		__u8		m_FmtChroma;
		BOOL8		m_IsLowDelay;
		BOOL8		m_IsUseVbvPara;
		__u16		m_FirstHalfBitRate;
		__u16		m_LatterHalfBitRate;
		__u16		m_FirstHalfVbvBufSize;
		__u8		m_LatterHalfVbvBufSize;
		__u16		m_FirstHalfVbvOccupancy;
		__u16		m_LatterHalfVbvOccupancy;

	protected:
	public:
		Cls_GrMpg4Vol();
		virtual	~Cls_GrMpg4Vol();

};
*/

//====================================================================
//global function

__u32		GrMpeg4GetVoId( __u32 A_VoStartCode );
BOOL8		GrMpeg4IsIvop( void* A_PtrVop );

//====================================================================
//out Enviroment conditional
#endif