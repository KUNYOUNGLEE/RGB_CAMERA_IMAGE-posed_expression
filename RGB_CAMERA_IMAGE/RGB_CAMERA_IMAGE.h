
// RGB_CAMERA_IMAGE.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRGB_CAMERA_IMAGEApp:
// �� Ŭ������ ������ ���ؼ��� RGB_CAMERA_IMAGE.cpp�� �����Ͻʽÿ�.
//

class CRGB_CAMERA_IMAGEApp : public CWinApp
{
public:
	CRGB_CAMERA_IMAGEApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRGB_CAMERA_IMAGEApp theApp;