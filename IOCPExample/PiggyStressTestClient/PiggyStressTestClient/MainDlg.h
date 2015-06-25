// MainDlg.h : ͷ�ļ�
//

#pragma once
#include "Client.h"

// CMainDlg �Ի���
class CMainDlg : public CDialog
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PIGGYSTRESSTESTCLIENT_DIALOG };

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
	// �˳�
	afx_msg void OnBnClickedCancel();
	// �Ի�������
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:

	// Ϊ�����������Ϣ��Ϣ(����CIOCPModel�е���)
	// Ϊ�˼��ٽ�������Ч�ʵ�Ӱ�죬�˴�ʹ��������
	inline void AddInformation(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pList->InsertItem(0,strInfo);
	}

private:

	// ��ʼ��������Ϣ
	void InitGUI();

	// ��ʼ��List�ؼ�
	void InitListCtrl();

private:

	CClient                m_Client;                          // �ͻ������Ӷ���


};
