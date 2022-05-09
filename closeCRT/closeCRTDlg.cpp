
// closeCRTDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "closeCRT.h"
#include "closeCRTDlg.h"
#include "afxdialogex.h"
#include "HotKey.h"
#include <chrono>
#include <thread>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcloseCRTDlg 对话框
/*
Value	           Meaning
MOD_ALT         Either ALT key was held down.
0x0001

MOD_CONTROL     Either CTRL key was held down.
0x0002

MOD_SHIFT       Either SHIFT key was held down.
0x0004

MOD_WIN        Either WINDOWS key was held down. These keys are labeled with the Windows logo. 
0x0008		   Hotkeys that involve the Windows key are reserved for use by the operating system.

*/
CcloseCRTDlg::CcloseCRTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLOSECRT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcloseCRTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcloseCRTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcloseCRTDlg::OnBnClickedButton1)
//	ON_WM_ACTIVATE()
//	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CcloseCRTDlg 消息处理程序
void closeCRT(int delay=3)
{
	std::chrono::seconds secs(delay);
	std::this_thread::sleep_for(secs);
	::SendMessage(::FindWindow(0, 0), WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}

BOOL CcloseCRTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
		
		}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	/*HotKey hotkey;   显示不正常,弃用,暂时保留注释.
	UINT mod = MOD_WIN;
	UINT key = 'c';
	std::function<void(int delay)> f_closeCRT = std::bind(closeCRT, 3); ;
	int ndelay = 3;
	hotkey.Register(mod, key, f_closeCRT);*/
	BOOL RET=RegisterHotKey(m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 'C');//热键必须大写,否则无效果.用MOD_WIN也没效果,奇怪.

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcloseCRTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcloseCRTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcloseCRTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcloseCRTDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
}



//int CcloseCRTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  在此添加您专用的创建代码
//	
//
//	return 0;
//}


void CcloseCRTDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	UnregisterHotKey(m_hWnd, 1001);
	// TODO: 在此处添加消息处理程序代码
}


void CcloseCRTDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == 1001)
	{
		//CWnd::SetForegroundWindow();//使得被激活窗口出现在前景 
		::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
		//用户可在此添加代码
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
