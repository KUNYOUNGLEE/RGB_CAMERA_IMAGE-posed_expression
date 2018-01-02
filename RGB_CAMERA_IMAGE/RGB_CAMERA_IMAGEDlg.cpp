
// RGB_CAMERA_IMAGEDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RGB_CAMERA_IMAGE.h"
#include "RGB_CAMERA_IMAGEDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string>
#include <cv.h>
#include <highgui.h>
#include <time.h>
#include "mmsystem.h"
#include "CvvImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char fileName[1024] = { 0 };
CString Name;
CString Name_exp;
CString Count_string;
int capture_count = 0;

void CreateDir(char* path)
{
	char DirName[256];
	char *p = path;
	char *q = DirName;

	while (*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p - 1))
			{
				CreateDirectoryA(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	CreateDirectoryA(DirName, NULL);
}

char * ConvertWCtoC(wchar_t* str)

{
	//반환할 char* 변수 선언
	char* pStr;
	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//char* 메모리 할당
	pStr = new char[strSize];
	//형 변환 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}

CRGB_CAMERA_IMAGEDlg::CRGB_CAMERA_IMAGEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RGB_CAMERA_IMAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	img_dlg = NULL;
	hThread = NULL;
	dwThreadParam = NULL;
	dwThreadID = NULL;
}

void CRGB_CAMERA_IMAGEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIDEO_PANEL, m_video);
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDC_CAPTURE_COUNT, m_count);
}

BEGIN_MESSAGE_MAP(CRGB_CAMERA_IMAGEDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_HAPPY, &CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy)
	ON_BN_CLICKED(ID_SAD, &CRGB_CAMERA_IMAGEDlg::OnBnClickedSad)
	ON_BN_CLICKED(ID_SURPRISE, &CRGB_CAMERA_IMAGEDlg::OnBnClickedSurprise)
	ON_BN_CLICKED(ID_FEAR, &CRGB_CAMERA_IMAGEDlg::OnBnClickedFear)
	ON_BN_CLICKED(ID_ANGER, &CRGB_CAMERA_IMAGEDlg::OnBnClickedAnger)
	ON_BN_CLICKED(ID_DISGUST, &CRGB_CAMERA_IMAGEDlg::OnBnClickedDisgust)
	ON_BN_CLICKED(ID_RECORD, &CRGB_CAMERA_IMAGEDlg::OnBnClickedRecord)
	ON_BN_CLICKED(ID_NAME_INPUT, &CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput)
	ON_BN_CLICKED(ID_NAME_INPUT2, &CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput2)
	ON_BN_CLICKED(ID_HAPPY2, &CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy2)
END_MESSAGE_MAP()


// CRGB_CAMERA_IMAGEDlg 메시지 처리기

BOOL CRGB_CAMERA_IMAGEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_count.SetWindowTextW(L"촬영 횟수: 0번");
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRGB_CAMERA_IMAGEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRGB_CAMERA_IMAGEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//카메라 영상 촬영 스레드 함수 cvCreateVideoWriter로 저장경로받아서 영상 저장함
DWORD CRGB_CAMERA_IMAGEDlg::ThreadFunction(LPVOID lpParam)
{
	IplImage *img;
	CvCapture *capture;
	CvVideoWriter *writer;
	CStatic *m_video;
	m_video =(CStatic *)lpParam;
	int key;
	wchar_t *name = NULL;
	wchar_t *name_exp = NULL;
	name = Name.GetBuffer(Name.GetLength());
	name_exp = Name_exp.GetBuffer(Name.GetLength());

	capture = cvCaptureFromCAM(-1);
	
	img = cvQueryFrame(capture);
	//영상 저장할 파일경로 설정
	sprintf(fileName, "D:\\Record\\%s\\RGB_Avi\\", ConvertWCtoC(name));
	CreateDir(fileName);

	struct tm *t;
	time_t timer;
	char buf[80];
	timer = time(NULL);    // 현재 시각을 초 단위로 얻기
	t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	sprintf(buf, "%02d_%02d_%02d_%02d_%02d%02d_",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec
	);

	//파일경로+파일명
	sprintf(buf, "%s%s", buf, ConvertWCtoC(name_exp));
	sprintf(fileName, "%s%s.avi", fileName, buf);

	int fps = 30;
	int width = img->width;
	int height = img->height;
	DWORD dwStartTime = timeGetTime();
	DWORD dwEndTime;
	writer = cvCreateVideoWriter(fileName, CV_FOURCC('D', 'I', 'V', 'X'), fps, cvSize(width, height), 1);
	CDC *pDC;
	CRect rect;
	CvvImage cvv_image;

	while (1)
	{
		img = cvQueryFrame(capture);

		pDC = m_video->GetDC();
		m_video->GetClientRect(&rect);
		cvv_image.CopyOf(img);
		cvv_image.DrawToHDC(pDC->m_hDC, rect);
		m_video->ReleaseDC(pDC);

		cvWriteFrame(writer, img);

		dwEndTime = timeGetTime();
		key = cvWaitKey(30);
		if ((dwEndTime - dwStartTime)>=15000) break;//ESC key
	}

	pDC = m_video->GetDC();
	CBrush background(RGB(220,220,220));
	m_video->GetClientRect(&rect);
	pDC->FillRect(&rect, &background);
	m_video->ReleaseDC(pDC);

	//저장 비디오 프레임 해제
	cvReleaseVideoWriter(&writer);
	//카메라를 해제
	cvReleaseCapture(&capture);
	return 0;
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedRecord()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD dwExitCode = 0;
	::GetExitCodeThread(hThread, &dwExitCode);

	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	else if (Name_exp.IsEmpty())
	{
		MessageBox(_T("이미지창을 띄우세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}

	if (dwExitCode != STILL_ACTIVE) {
		hThread = CreateThread(NULL, 0, ThreadFunction, &m_video, 0, &dwThreadID);
	}
	capture_count++;
	Count_string.Format(_T("촬영 횟수: %d번"), capture_count);
	m_count.SetWindowTextW(Count_string);
}


void CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Name.Empty();
	m_name.GetWindowTextW(Name);
	capture_count = 0;
	Count_string.Format(_T("촬영 횟수: %d번"), capture_count);
	m_count.SetWindowTextW(Count_string);

	if (!Name.IsEmpty())
	{
		MessageBox(_T("이름이 입력되었습니다. 촬영을 시작하세요"), _T("입력완료"), NULL); return;
	}
	else {
		MessageBox(_T("이름을 다시 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
}

//////////////////////////////////////////////////////////////// 아래부터의 코드는 이미지 바꿔주는 버튼 이벤트 (이미지 윈도우 다이얼로그 관련)/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_happy");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\Happy.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedSad()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_sad");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\sad.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedSurprise()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_surprise");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\surprise.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedFear()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_fear");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\fear.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedAnger()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_anger");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\anger.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedDisgust()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_disgust");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\disgust.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}



void CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	capture_count = 0;
	m_name.SetWindowTextW(L"");
	m_count.SetWindowTextW(L"촬영 횟수: 0번");
	Name.Empty();
	Name_exp.Empty();
}


void CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy2()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("이름을 입력하세요"), _T("오류"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	Name_exp = Name + _T("_neutral");
	m_name.SetWindowTextW(Name_exp);

	if (img_dlg != NULL)
	{
		img_dlg->SetFocus();
	}
	else {
		img_dlg = new IMAGE_Dialog;
		img_dlg->Create(IDD_IMAGE, this);
		img_dlg->ShowWindow(SW_SHOW);
	}

	if (img_dlg != NULL)
	{
		img_dlg->strPath.Format(_T("D:\\표정이미지\\neutral.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}
