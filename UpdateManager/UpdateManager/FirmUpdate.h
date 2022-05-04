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
#define E_FirmUpHdFcc				0x75707302//UPS1
#define E_FirmUpEdFcc				0x75706502//UPE1
#define E_FirmUpVer					1

#define	E_FirmUpInfoCnt				16
#define E_FirmUpEntityCnt			16

#define	E_FirmUpInfoTypeMaxIdx		24
#define	E_FirmUpInfoTypeNone		0
/*#define	E_FirmUpInfoTypeJa1704	1
#define	E_FirmUpInfoTypeJa1708	2
#define	E_FirmUpInfoTypeJa1716	3*/

#define	E_FirmUpInfoTypeJa1704		21
#define	E_FirmUpInfoTypeJa1708		22
#define	E_FirmUpInfoTypeJa1716		23

#define	E_FirmUpEntityNone			0
#define	E_FirmUpEntityLoader		1
#define	E_FirmUpEntityFdt			2
#define	E_FirmUpEntityUboot			3
#define	E_FirmUpEntityKernel		4
#define	E_FirmUpEntityLogo			5
#define	E_FirmUpEntityRootfs		6

#define E_FirmUpErrCode				-1
#define E_FirmUpErrExistType		-2
#define E_FirmUpErrExistEntity		-3
#define E_FirmUpErrExistData		-4

//====================================================================
//global macro

//====================================================================
//global type
typedef struct _stFirmUpEntity
{
	unsigned int		Type;
	unsigned int		Offset;
	unsigned int		Size;
} *pFirmUpEntity;

typedef struct _stFirmUpInfo
{
	unsigned int		ModelType;
	_stFirmUpEntity		Entity[E_FirmUpEntityCnt];
} *pFirmUpInfo;

typedef struct _stFirmUpHeader
{
	unsigned int		Fcc; // 파일 맞는지 아닌지.(1)
	unsigned int		UpgdVer;
	unsigned int		Size;
	_stFirmUpInfo		FirmInfo[E_FirmUpInfoCnt];
} *pFirmUpHeader;

typedef struct _stFirmUpEnd
{
	unsigned int		Fcc; // 파일 맞는지 아닌지.(2)
} *pFirmUpEnd;

//====================================================================
//class
class CFirmUpdate
{

public:
	CFirmUpdate();
	virtual	~CFirmUpdate();

	void	Init();
	
	// 모델 추가
	int		AddModelType(unsigned int _uiModelType);
	// 모델 삭제
	void	DelModelType(unsigned int _uiModelType);
	// 버전 파일 선택
	int		AddVerFile(unsigned int _uiModelType, unsigned int _uiVersionType, void* _pSrc, unsigned int _uiSize);
	// 버전 파일 삭제
	void	DelVerFile(unsigned int _uiModelType, unsigned int _uiVersionType);

	void*	GetMkUpdt(unsigned int* _puiSize);
	void	SetUpdtVer(unsigned int _uiVer);

	int		GetModelTblIdx(unsigned int _uiModelType);

protected:
	int		LcChkModelType(unsigned int _uiModelType);
	int		LcChkEntityType(unsigned int _uiVersionType, pFirmUpEntity _stEntity);

private:
	_stFirmUpHeader		m_stFirmHeader;
	_stFirmUpEnd		m_stFirmEd;
	void*				m_pData[E_FirmUpInfoCnt][E_FirmUpEntityCnt];
	void*				m_pMkUpdt;
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif
