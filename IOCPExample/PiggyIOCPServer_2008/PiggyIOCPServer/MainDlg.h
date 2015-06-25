// MainDlg.h : ͷ�ļ�
//

#pragma once

#include "IOCPModel.h"

// CMainDlg �Ի���
class CMainDlg : public CDialog
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PIGGYIOCPSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// ��ʼ����
	afx_msg void OnBnClickedOk();
	// ֹͣ����
	afx_msg void OnBnClickedStop();
	// "�˳�"��ť
	afx_msg void OnBnClickedCancel();
	///////////////////////////////////////////////////////////////////////
	//	ϵͳ�˳���ʱ��Ϊȷ����Դ�ͷţ�ֹͣ���������Socket���
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnNewMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:

	// ��ʼ��Socket���Լ�������Ϣ
	void Init();

	// ��ʼ��List�ؼ�
	void InitListCtrl();

public:

	// ��ǰ�ͻ���������Ϣ������ʱ��������������ʾ�µ�������Ϣ(����CIOCPModel�е���)
	// Ϊ�˼��ٽ�������Ч�ʵ�Ӱ�죬�˴�ʹ��������
	inline void AddInformation(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pList->InsertItem(0,strInfo);
	}

private:

	CIOCPModel m_IOCP;                         // ��Ҫ������ɶ˿�ģ��

};
