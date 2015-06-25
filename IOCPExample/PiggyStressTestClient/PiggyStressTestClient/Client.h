
/*
==========================================================================

Purpose:

* 这个类CClient是本代码的核心类，用于产生用于指定的并发线程向指定服务器发送
  信息，测试服务器的响应及资源占用率情况，并使用了MFC对话框程序来进行说明

Notes:

* 客户端使用的是最简单的多线程阻塞式Socket，而且每个线程只发送一次数据
  如果需要可以修改成发送多次数据的情况

Author:

* PiggyXP【小猪】

Date:

* 2009/10/04

==========================================================================
*/

#pragma once

// 屏蔽deprecation警告
#pragma warning(disable: 4996)

// 缓冲区长度(8*1024字节)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // 默认端口
#define DEFAULT_IP            _T("127.0.0.1")            // 默认IP地址
#define DEFAULT_THREADS       100                        // 默认并发线程数
#define DEFAULT_MESSAGE       _T("Hello!")   // 默认的发送信息

class CClient;

// 用于发送数据的线程参数
typedef struct _tagThreadParams_WORKER
{
	CClient* pClient;                               // 类指针，用于调用类中的函数
	SOCKET   sock;                                  // 每个线程使用的Socket
	int      nThreadNo;                             // 线程编号
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;  

// 产生Socket连接的线程
typedef struct _tagThreadParams_CONNECTION
{
	CClient* pClient;                               // 类指针，用于调用类中的函数

} THREADPARAMS_CONNECTION,*PTHREADPARAM_CONNECTION; 


class CClient
{
public:
	CClient(void);
	~CClient(void);

public:

	// 加载Socket库
	bool LoadSocketLib();
	// 卸载Socket库，彻底完事
	void UnloadSocketLib() { WSACleanup(); }

	// 开始测试
	bool Start();
	//	停止测试
	void Stop();

	// 获得本机的IP地址
	CString GetLocalIP();

	// 设置连接IP地址
	void SetIP( const CString& strIP ) { m_strServerIP=strIP; }
	// 设置监听端口
	void SetPort( const int& nPort )   { m_nPort=nPort; }
	// 设置并发线程数量
	void SetThreads(const int& n)      { m_nThreads=n; }
	// 设置要按发送的信息
	void SetMessage( const CString& strMessage ) { m_strMessage=strMessage; }

	// 设置主界面的指针，用于调用其函数
	void SetMainDlg( CDialog* p )      { m_pMain=p; }

	// 在主界面中显示信息
	void ShowMessage( const CString strInfo,...);

private:

	// 建立连接
	bool EstablishConnections();
	// 向服务器进行连接
	bool ConnetToServer( SOCKET *pSocket, CString strServer, int nPort );
	// 用于建立连接的线程
	static DWORD WINAPI _ConnectionThread(LPVOID lpParam);
	// 用于发送信息的线程
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	// 释放资源
	void CleanUp();

private:

	CDialog*  m_pMain;                                      // 界面指针

	CString   m_strServerIP;                                // 服务器端的IP地址
	CString   m_strLocalIP;                                 // 本机IP地址
	CString   m_strMessage;                                 // 发给服务器的信息
	int       m_nPort;                                      // 监听端口
	int       m_nThreads;                                   // 并发线程数量

	HANDLE    *m_phWorkerThreads;
	HANDLE    m_hConnectionThread;                          // 接受连接的线程句柄
	HANDLE    m_hShutdownEvent;                             // 用来通知线程系统退出的事件，为了能够更好的退出线程

	THREADPARAMS_WORKER      *m_pParamsWorker;              // 线程参数
};
