
// RCserverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RCserver.h"
#include "RCserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_BUFF 99999
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CRCserverDlg �Ի���




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


// CRCserverDlg ��Ϣ�������

BOOL CRCserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_CtrlList.Create(IDD_DIALOG1);
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	//hostent* phost=gethostbyname("");
	////���Է���
	///*char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);*/
	//char* localIP="127.0.0.1";

		WSADATA wsData;//��ʼ��Winsocket
	::WSAStartup(MAKEWORD(2,2), &wsData);
	//��ȡ����IP
	char    HostName[100];
    gethostname(HostName, sizeof(HostName));// ��ñ���������.

	hostent* phost=gethostbyname(HostName);
	char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);

	m_Addr.sin_family=AF_INET;//����
	m_Addr.sin_addr.S_un.S_addr  = inet_addr("171.94.197.156");//���ÿͻ���IP
	m_Addr.sin_port  = htons(5001);

	sockaddr_in addr;//����
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr  = inet_addr(localIP);
	addr.sin_port  = htons(5002);

	m_Socket = socket(AF_INET,SOCK_DGRAM,0);
	if (m_Socket == INVALID_SOCKET)
	{
		MessageBox("�׽��ִ���ʧ��!");
	}
	
	if (bind(m_Socket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		MessageBox("�׽��ְ�ʧ��!");
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


	m_ClientIP = "127.0.0.1";//���ÿͻ���IP
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRCserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRCserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���յ��ͻ�����Ϣʱ
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
		//��¼���յ����ݱ���С
		m_RecSize += ret;
		//��ȡ���
		WORD orderID =  *(WORD*)&buffer[0];
		//��ȡ�������
		WORD endID = *(WORD*)&buffer[2];
		//��ȡλͼ��С
		int bmpsize = *(int*)&buffer[ret-12];
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port   = htons(m_ClientPort);
		addr.sin_addr.S_un.S_addr = inet_addr(m_ClientIP.GetBuffer(0));
		int size = sizeof(addr);
		m_JPGSize = 0;
		//����ȷ����Ϣ
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
//ʵ��˫��ȫ��
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


BOOL CRCserverDlg::PreTranslateMessage(MSG* pMsg) //ʵ��Ctrl�Ŀ�ݼ�
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