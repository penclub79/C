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
	__u32				Type;
	__u32				Offset;
	__u32				Size;
} *pFirmUpEntity;

typedef struct _stFirmUpInfo
{
	__u32				ModelType;
	_stFirmUpEntity		Entity[E_FirmUpEntityCnt];
} *pFirmUpInfo;

typedef struct _stFirmUpHeader
{
	__u32				Fcc; // ���� �´��� �ƴ���.(1)
	__u32				UpgdVer;
	__u32				Size;
	_stFirmUpInfo		FirmInfo[E_FirmUpInfoCnt];
} *pFirmUpHeader;

typedef struct _stFirmUpEnd
{
	__u32				Fcc; // ���� �´��� �ƴ���.(2)
} *pFirmUpEnd;

//====================================================================
//class
class CFirmUpdate
{

public:
	CFirmUpdate();
	virtual	~CFirmUpdate();

	// Firm Data �ʱ�ȭ
	void	FirmInit();

	// �� �߰�
	int		AddModelType(int _iModelType);
	
	// ���� ���� �߰�
	int		AddVerFile(int _iModelType, int _iVerFileType, char* _pSrc, int _iFileSize);

	int		GetModelTypeIdx(int _iModelType);

	void	SetUpdateVersion(unsigned int _uiVersion);

	// ��Ű�� ����� ������ ���
	void*	GetMakeUpdate(unsigned int* _puiSize);

	// �� ����
	void	DelModelType(int _iModelType);

	// ���� ���� ����
	void	DelVerFile(int _iModelType, int _iVerFileType);

protected:
	// ���� ���� üũ
	int		ChkEntityType(int _iVersionType, pFirmUpEntity _stEntity);

	// �� Ÿ�� üũ
	int		ChkModelType(unsigned int _uiModelType);

private:
	_stFirmUpHeader		m_stFirmHeader;
	_stFirmUpEnd		m_stFirmEd;
	void*				m_pData[E_FirmUpInfoCnt][E_FirmUpEntityCnt];
	void*				m_pMakeUpdate;

	
	void	SetUpdtVer(int _iVer);

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif
