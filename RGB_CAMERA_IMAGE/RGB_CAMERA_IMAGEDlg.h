
// RGB_CAMERA_IMAGEDlg.h : 헤더 파일
//
#include "IMAGE_Dialog.h"

#pragma once


// CRGB_CAMERA_IMAGEDlg 대화 상자
class CRGB_CAMERA_IMAGEDlg : public CDialogEx
{
// 생성입니다.
public:
	CRGB_CAMERA_IMAGEDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	// CRGB_CAMERA_IMAGEDlg 대화 상자
	IMAGE_Dialog *img_dlg;
	HANDLE hThread;
	DWORD dwThreadParam;
	DWORD dwThreadID;
	CStatic m_video;
	CStatic m_name;
	CStatic m_count;

	static DWORD  WINAPI ThreadFunction(LPVOID lpParam);


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RGB_CAMERA_IMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
