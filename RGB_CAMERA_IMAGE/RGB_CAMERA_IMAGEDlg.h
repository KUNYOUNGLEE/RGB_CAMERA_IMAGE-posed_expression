
// RGB_CAMERA_IMAGEDlg.h : ��� ����
//
#include "IMAGE_Dialog.h"

#pragma once


// CRGB_CAMERA_IMAGEDlg ��ȭ ����
class CRGB_CAMERA_IMAGEDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CRGB_CAMERA_IMAGEDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	// CRGB_CAMERA_IMAGEDlg ��ȭ ����
	IMAGE_Dialog *img_dlg;
	HANDLE hThread;
	DWORD dwThreadParam;
	DWORD dwThreadID;
	CStatic m_video;
	CStatic m_name;
	CStatic m_count;

	static DWORD  WINAPI ThreadFunction(LPVOID lpParam);


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RGB_CAMERA_IMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHappy();
	afx_msg void OnBnClickedSad();
	afx_msg void OnBnClickedSurprise();
	afx_msg void OnBnClickedFear();
	afx_msg void OnBnClickedAnger();
	afx_msg void OnBnClickedDisgust();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedNameInput();
	afx_msg void OnBnClickedNameInput2();
	afx_msg void OnBnClickedHappy2();
};
