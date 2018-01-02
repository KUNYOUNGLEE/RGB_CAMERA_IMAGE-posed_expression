// IMAGE_Dialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RGB_CAMERA_IMAGE.h"
#include "RGB_CAMERA_IMAGEDlg.h"
#include "IMAGE_Dialog.h"
#include "afxdialogex.h"


// IMAGE_Dialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(IMAGE_Dialog, CDialogEx)

IMAGE_Dialog::IMAGE_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMAGE, pParent)
{
}

IMAGE_Dialog::~IMAGE_Dialog()
{
}

void IMAGE_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PANEL, PANEL);
}

BEGIN_MESSAGE_MAP(IMAGE_Dialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// IMAGE_Dialog 메시지 처리기입니다.
BOOL IMAGE_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	strPath.Format(_T("D:\\표정이미지\\happy.jpg"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void IMAGE_Dialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CImage Image;
	CRect rect;
	HRESULT hResult = Image.Load(strPath);
	if (FAILED(hResult)) {
		TRACE(strPath);
		return;
	}
	CDC* pDC = CDC::FromHandle(Image.GetDC());
	CStatic *staticSize = (CStatic *)GetDlgItem(IDC_PANEL);
	staticSize->GetClientRect(rect);
	Image.ReleaseDC();
	//Image.StretchBlt(dc.m_hDC, 10, 10, rect.Width(), rect.Height(), SRCCOPY);
	Image.BitBlt(dc.m_hDC, 360, 10, SRCCOPY);
}

void IMAGE_Dialog::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnClose();

	CRGB_CAMERA_IMAGEDlg *pMainWnd = (CRGB_CAMERA_IMAGEDlg *)AfxGetMainWnd();
	pMainWnd->img_dlg = NULL;
	delete this;
}
