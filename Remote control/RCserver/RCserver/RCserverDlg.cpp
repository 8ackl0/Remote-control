
// RCserverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RCserver.h"
#include "RCserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_BUFF 99999
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRCserverDlg 对话框




CRCserverDlg::CRCserverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRCserverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_JPGSize = 0;
	m_IsScreen = FALSE;
	m_pNewBmp = NULL;
}

void CRCserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRCserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(CM_RECEIVED,&OnReceived)
END_MESSAGE_MAP()


// CRCserverDlg 消息处理程序

BOOL CRCserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_CtrlList.Create(IDD_DIALOG1);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	//hostent* phost=gethostbyname("");
	////测试方便
	///*char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);*/
	//char* localIP="127.0.0.1";

		WSADATA wsData;//初始化Winsocket
	::WSAStartup(MAKEWORD(2,2), &wsData);
	//获取本机IP
	char    HostName[100];
    gethostname(HostName, sizeof(HostName));// 获得本机主机名.

	hostent* phost=gethostbyname(HostName);
	char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);

	m_Addr.sin_family=AF_INET;//发送
	m_Addr.sin_addr.S_un.S_addr  = inet_addr("171.94.197.156");//设置客户机IP
	m_Addr.sin_port  = htons(5001);

	sockaddr_in addr;//监听
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr  = inet_addr(localIP);
	addr.sin_port  = htons(5002);

	m_Socket = socket(AF_INET,SOCK_DGRAM,0);
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox("套接字创建失败!");
	}
	
	if (bind(m_Socket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		MessageBox("套接字绑定失败!");
	}
	m_BmpData = new char[1024*1024];
	memset(m_BmpData,0,1024*1024);
	m_TempData = new char[1024*1024*2];
	memset(m_TempData,0,1024*1024*2);
	m_Header = m_BmpData;
	WSAAsyncSelect(m_Socket,m_hWnd,CM_RECEIVED,FD_READ);

	/*listen(m_Socket,5);*/

	m_ShowBmp = FALSE;
	m_BmpSize = 0;
	m_RecSize = 0;
	m_ClientPort = 0;


	m_ClientIP = "127.0.0.1";//设置客户端IP
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRCserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRCserverDlg::OnPaint()
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
HCURSOR CRCserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//当收到客户端信息时
LRESULT CRCserverDlg::OnReceived(WPARAM wParam,LPARAM lParam)
{
		
	BYTE* buffer = new BYTE[MAX_BUFF];

	int factsize =sizeof(sockaddr);
	int ret = recvfrom(m_Socket,(char*)buffer,MAX_BUFF,0,(sockaddr*)&m_Addr,&factsize);

	if (ret != -1)
	{
		CString revIP = inet_ntoa(m_Addr.sin_addr);
	/*	if (revIP != m_ClientIP)
		{
			delete [] buffer;
			return;
		}	
		*/
		m_ClientPort = ntohs(m_Addr.sin_port);
		//记录接收的数据报大小
		m_RecSize += ret;
		//读取序号
		WORD orderID =  *(WORD*)&buffer[0];
		//读取结束标记
		WORD endID = *(WORD*)&buffer[2];
		//读取位图大小
		int bmpsize = *(int*)&buffer[ret-12];
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port   = htons(m_ClientPort);
		addr.sin_addr.S_un.S_addr = inet_addr(m_ClientIP.GetBuffer(0));
		int size = sizeof(addr);
		m_JPGSize = 0;
		//发送确认信息
		DataPackage Package;
		memset(&Package,0,sizeof(DataPackage));

		Package.MsgType = CT_Confirm;

		int nRet = sendto(m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&m_Addr,size);

		if (orderID==0)
		{
			m_BmpSize = bmpsize;
			m_BmpData = m_Header;
		}
		else
			m_BmpSize += bmpsize;
		memcpy(m_BmpData,&buffer[4],bmpsize);
		m_BmpData += bmpsize;
		m_ShowBmp = FALSE;
		if (endID == 1)
		{
			m_ShowBmp = TRUE;
			m_BmpData = m_Header;
			m_JPGSize = *(int*)&buffer[ret-8];
			memset(m_TempData,0,1024*1024*2);
			memcpy(m_TempData,m_Header,1024*1024);
			ShowJPEG(m_TempData,m_JPGSize);
			m_RecSize = 0;
		}
	}
	delete [] buffer;

	return TRUE;
}
//
//实现双击全屏
//
void CRCserverDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_IsScreen = !m_IsScreen;
	if (m_IsScreen)
	{
		GetWindowPlacement(&m_OldPlacement);
		CRect WndRect;
		GetWindowRect(&WndRect);
		int Width = GetSystemMetrics(SM_CXSCREEN);
		int Height = GetSystemMetrics(SM_CYSCREEN);
		CRect ClientRect;
		RepositionBars(0,0xffff,AFX_IDW_PANE_FIRST,reposQuery,&ClientRect);
		ClientToScreen(&ClientRect);

		m_FullScreenRect.left =WndRect.left-ClientRect.left;
		m_FullScreenRect.top = WndRect.top-ClientRect.top;
		m_FullScreenRect.right =WndRect.right-ClientRect.right+Width;
		m_FullScreenRect.bottom =WndRect.bottom-ClientRect.bottom+Height;

		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);
	}
	else
	{
		SetWindowPlacement(&m_OldPlacement);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


BOOL CRCserverDlg::PreTranslateMessage(MSG* pMsg) //实现Ctrl的快捷键
{
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_CONTROL)	
	{
		m_CtrlList.ShowWindow(SW_SHOW);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CRCserverDlg::OnCancel()
{	
	m_BmpData = m_Header;
	delete [] m_BmpData;
	delete [] m_TempData;
	if (m_pNewBmp)
	{
		delete  m_pNewBmp;
		m_pNewBmp = NULL;
	}
	::WSACleanup(); 
	GdiplusShutdown(m_pGdiToken);
	CDialog::OnCancel();
}

void CRCserverDlg::ShowJPEG(void* pData,int DataSize) 
{
	if (m_ShowBmp)
	{
		HGLOBAL m_hMem1 = GlobalAlloc(GMEM_MOVEABLE,DataSize);
		LPBYTE lpData1 = (LPBYTE)GlobalLock(m_hMem1);
		memcpy(lpData1,pData,DataSize);
		GlobalUnlock(m_hMem1);
		::CreateStreamOnHGlobal(m_hMem1,TRUE, &m_pStm);
		if (m_pNewBmp)
		{
			delete m_pNewBmp;
			m_pNewBmp = NULL;
		}
		m_pNewBmp = Bitmap::FromStream(m_pStm);
		CRect rc;
		GetClientRect(rc);
		HDC hDC = GetDC()->m_hDC;
		Graphics *graphics = Graphics::FromHDC(hDC);	
		graphics->DrawImage(m_pNewBmp,1,1,rc.Width(),rc.Height());	
		m_pStm->Release();
		m_pStm = NULL;
		delete graphics;
		GlobalFree(m_hMem1);
		::ReleaseDC(m_hWnd,hDC);
	}
}