// PiggyStressTestClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif


// CPiggyStressTestClientApp:
// �йش����ʵ�֣������ PiggyStressTestClient.cpp
//

class CPiggyStressTestClientApp : public CWinApp
{
public:
	CPiggyStressTestClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPiggyStressTestClientApp theApp;