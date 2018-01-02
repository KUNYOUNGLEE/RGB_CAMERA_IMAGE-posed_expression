
// RGB_CAMERA_IMAGEDlg.cpp : ���� ����
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
	//��ȯ�� char* ���� ����
	char* pStr;
	//�Է¹��� wchar_t ������ ���̸� ����
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	//char* �޸� �Ҵ�
	pStr = new char[strSize];
	//�� ��ȯ 
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


// CRGB_CAMERA_IMAGEDlg �޽��� ó����

BOOL CRGB_CAMERA_IMAGEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_count.SetWindowTextW(L"�Կ� Ƚ��: 0��");
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CRGB_CAMERA_IMAGEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CRGB_CAMERA_IMAGEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//ī�޶� ���� �Կ� ������ �Լ� cvCreateVideoWriter�� �����ι޾Ƽ� ���� ������
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
	//���� ������ ���ϰ�� ����
	sprintf(fileName, "D:\\Record\\%s\\RGB_Avi\\", ConvertWCtoC(name));
	CreateDir(fileName);

	struct tm *t;
	time_t timer;
	char buf[80];
	timer = time(NULL);    // ���� �ð��� �� ������ ���
	t = localtime(&timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
	sprintf(buf, "%02d_%02d_%02d_%02d_%02d%02d_",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec
	);

	//���ϰ��+���ϸ�
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

	//���� ���� ������ ����
	cvReleaseVideoWriter(&writer);
	//ī�޶� ����
	cvReleaseCapture(&capture);
	return 0;
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedRecord()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	DWORD dwExitCode = 0;
	::GetExitCodeThread(hThread, &dwExitCode);

	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
	else if (Name_exp.IsEmpty())
	{
		MessageBox(_T("�̹���â�� ��켼��"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}

	if (dwExitCode != STILL_ACTIVE) {
		hThread = CreateThread(NULL, 0, ThreadFunction, &m_video, 0, &dwThreadID);
	}
	capture_count++;
	Count_string.Format(_T("�Կ� Ƚ��: %d��"), capture_count);
	m_count.SetWindowTextW(Count_string);
}


void CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Name.Empty();
	m_name.GetWindowTextW(Name);
	capture_count = 0;
	Count_string.Format(_T("�Կ� Ƚ��: %d��"), capture_count);
	m_count.SetWindowTextW(Count_string);

	if (!Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �ԷµǾ����ϴ�. �Կ��� �����ϼ���"), _T("�Է¿Ϸ�"), NULL); return;
	}
	else {
		MessageBox(_T("�̸��� �ٽ� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
	}
}

//////////////////////////////////////////////////////////////// �Ʒ������� �ڵ�� �̹��� �ٲ��ִ� ��ư �̺�Ʈ (�̹��� ������ ���̾�α� ����)/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\Happy.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedSad()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\sad.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedSurprise()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\surprise.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedFear()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\fear.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedAnger()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\anger.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}

void CRGB_CAMERA_IMAGEDlg::OnBnClickedDisgust()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\disgust.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}



void CRGB_CAMERA_IMAGEDlg::OnBnClickedNameInput2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	capture_count = 0;
	m_name.SetWindowTextW(L"");
	m_count.SetWindowTextW(L"�Կ� Ƚ��: 0��");
	Name.Empty();
	Name_exp.Empty();
}


void CRGB_CAMERA_IMAGEDlg::OnBnClickedHappy2()
{
	if (Name.IsEmpty())
	{
		MessageBox(_T("�̸��� �Է��ϼ���"), _T("����"), MB_ICONEXCLAMATION | MB_ICONWARNING); return;
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
		img_dlg->strPath.Format(_T("D:\\ǥ���̹���\\neutral.jpg"));
		img_dlg->Invalidate(TRUE);
	}
}
