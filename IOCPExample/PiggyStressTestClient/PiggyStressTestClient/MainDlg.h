// MainDlg.h : 头文件
//

#pragma once
#include "Client.h"

// CMainDlg 对话框
class CMainDlg : public CDialog
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PIGGYSTRESSTESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// 开始测试
	afx_msg void OnBnClickedOk();
	// 停止测试
	afx_msg void OnBnClickedStop();
	// 退出
	afx_msg void OnBnClickedCancel();
	// 对话框销毁
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public:

	// 为主界面添加信息信息(在类CIOCPModel中调用)
	// 为了减少界面代码对效率的影响，此处使用了内联
	inline void AddInformation(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_INFO);
		pList->InsertItem(0,strInfo);
	}

private:

	// 初始化界面信息
	void InitGUI();

	// 初始化List控件
	void InitListCtrl();

private:

	CClient                m_Client;                          // 客户端连接对象


};
