// IMAGE_Dialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "RGB_CAMERA_IMAGE.h"
#include "RGB_CAMERA_IMAGEDlg.h"
#include "IMAGE_Dialog.h"
#include "afxdialogex.h"


// IMAGE_Dialog ��ȭ �����Դϴ�.

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

// IMAGE_Dialog �޽��� ó�����Դϴ�.
BOOL IMAGE_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	strPath.Format(_T("D:\\ǥ���̹���\\happy.jpg"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialogEx::OnClose();

	CRGB_CAMERA_IMAGEDlg *pMainWnd = (CRGB_CAMERA_IMAGEDlg *)AfxGetMainWnd();
	pMainWnd->img_dlg = NULL;
	delete this;
}
