#pragma once
#include "afxwin.h"


// IMAGE_Dialog ��ȭ �����Դϴ�.

class IMAGE_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(IMAGE_Dialog)

public:
	IMAGE_Dialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~IMAGE_Dialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStatic IMAGE_PANEL;
	afx_msg void OnPaint();
	CStatic PANEL;
	CString strPath;

	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
};
