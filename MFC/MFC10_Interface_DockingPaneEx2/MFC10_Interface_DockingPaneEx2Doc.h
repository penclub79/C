
// MFC10_Interface_DockingPaneEx2Doc.h : CMFC10_Interface_DockingPaneEx2Doc Ŭ������ �������̽�
//


#pragma once


class CMFC10_Interface_DockingPaneEx2Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC10_Interface_DockingPaneEx2Doc();
	DECLARE_DYNCREATE(CMFC10_Interface_DockingPaneEx2Doc)

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
	virtual ~CMFC10_Interface_DockingPaneEx2Doc();
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
