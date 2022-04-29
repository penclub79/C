/*
	firmware upgrade
*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_FirmUpdt
#define	_EPC_FirmUpdt

//====================================================================
// uses
#include <GrTypeBase.h>

//====================================================================
//const
#define E_FirmUpHdFcc						0x75707302//UPS1
#define E_FirmUpEdFcc						0x75706502//UPE1
#define E_FirmUpVer							1

#define	E_FirmUpInfoCnt					16
#define E_FirmUpEntityCnt				16

#define	E_FirmUpInfoTypeMaxIdx	24
#define	E_FirmUpInfoTypeNone		0
/*#define	E_FirmUpInfoTypeJa1704	1
#define	E_FirmUpInfoTypeJa1708	2
#define	E_FirmUpInfoTypeJa1716	3*/

#define	E_FirmUpInfoTypeJa1704	21
#define	E_FirmUpInfoTypeJa1708	22
#define	E_FirmUpInfoTypeJa1716	23

#define	E_FirmUpEntityNone			0
#define	E_FirmUpEntityLoader		1
#define	E_FirmUpEntityFdt				2
#define	E_FirmUpEntityUboot			3
#define	E_FirmUpEntityKernel		4
#define	E_FirmUpEntityLogo			5
#define	E_FirmUpEntityRootfs		6

#define E_FirmUpErrCode					-1
#define E_FirmUpErrExistType		-2
#define E_FirmUpErrExistEntity	-3
#define E_FirmUpErrExistData		-4

//====================================================================
//global macro

//====================================================================
//global type
typedef struct St_FirmUpEntity
{
	__u32		Type;
	__u32		Offset;
	__u32		Size;
} *Ptr_FirmUpEntity;

typedef struct St_FirmUpInfo
{
	__u32		ModelType;
	St_FirmUpEntity Entity[E_FirmUpEntityCnt];
} *Ptr_FirmUpInfo;

typedef struct St_FirmUpHd
{
	__u32		Fcc;
	__u32		UpgdVer;
	__u32		Size;
	St_FirmUpInfo FirmInfo[E_FirmUpInfoCnt];
} *Ptr_FirmUpHd;

typedef struct St_FirmUpEd
{
	__u32		Fcc;
} *Ptr_FirmUpEd;

//====================================================================
//class
class Cls_FirmUpdt
{
	private:
		St_FirmUpHd		m_FirmHd;
		St_FirmUpEd		m_FirmEd;
		void*					m_PtrData[E_FirmUpInfoCnt][E_FirmUpEntityCnt];
		void*					m_PtrMkUpdt;
	protected:
		__s32		LcChkModelType(__u32 A_ModelType);
		__s32		LcChkEntityType(__u32 A_EntityType, Ptr_FirmUpEntity A_PtrEntity);
	public:
		Cls_FirmUpdt();
		virtual	~Cls_FirmUpdt();

		void	Init();
		__s32	AddType(__u32 A_ModelType);
		__s32	AddEntity(__u32 A_ModelType, __u32 A_EntityType, void* A_PtrSrc, __u32 A_Size);
		void	DelType(__u32 A_ModelType);
		void	DelEntity(__u32 A_ModelType, __u32 A_EntityType);

		void* GetMkUpdt(__u32* A_PtrSize);
		void	SetUpdtVer(__u32 A_Ver);

		__s32 GetModelTblIdx(__u32 A_ModelType);
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif
