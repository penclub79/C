
// MFC_Message_KeyAndMouseEx_3Doc.h : CMFC_Message_KeyAndMouseEx_3Doc Ŭ������ �������̽�
//


#pragma once


class CMFC_Message_KeyAndMouseEx_3Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC_Message_KeyAndMouseEx_3Doc();
	DECLARE_DYNCREATE(CMFC_Message_KeyAndMouseEx_3Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CMFC_Message_KeyAndMouseEx_3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
