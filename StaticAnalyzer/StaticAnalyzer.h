
// StaticAnalyzer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CStaticAnalyzerApp:
// �� Ŭ������ ������ ���ؼ��� StaticAnalyzer.cpp�� �����Ͻʽÿ�.
//

class CStaticAnalyzerApp : public CWinApp
{
public:
	CStaticAnalyzerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CStaticAnalyzerApp theApp;