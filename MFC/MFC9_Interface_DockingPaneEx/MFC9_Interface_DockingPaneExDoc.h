
// MFC9_Interface_DockingPaneExDoc.h : CMFC9_Interface_DockingPaneExDoc Ŭ������ �������̽�
//


#pragma once


class CMFC9_Interface_DockingPaneExDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMFC9_Interface_DockingPaneExDoc();
	DECLARE_DYNCREATE(CMFC9_Interface_DockingPaneExDoc)

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
	virtual ~CMFC9_Interface_DockingPaneExDoc();
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
