
// RCclientDlg.cpp : ʵ���ļ�
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


// CRCclientDlg �Ի���

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


// CRCclientDlg ��Ϣ�������

BOOL CRCclientDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	HANDLE hToken;//����Ȩ��
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);

	TOKEN_PRIVILEGES Privilege;
	LookupPrivilegeValue(NULL,"Seshutdownprivilege",&Privilege.Privileges[0].Luid);
	Privilege.PrivilegeCount=1;
	Privilege.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

	DWORD dwLen;//����Ȩ��
	AdjustTokenPrivileges(hToken,false,&Privilege,sizeof(TOKEN_PRIVILEGES),NULL,&dwLen);

	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);//��ʼ��GDI+��

	WSADATA wsData;//��ʼ��Winsocket
	::WSAStartup(MAKEWORD(2,2), &wsData);
	//��ȡ����IP
	char    HostName[100];
    gethostname(HostName, sizeof(HostName));// ��ñ���������.

	hostent* phost=gethostbyname(HostName);
	char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);
	/*char* localIP=inet_ntoa(*(struct in_addr *)*phost->h_addr_list);*/
	/*char* localIP="127.0.0.1";*/
	//

	m_Addr.sin_family = AF_INET;//����
	m_Addr.sin_port   = htons(5002);
	m_Addr.sin_addr.S_un.S_addr = inet_addr("171.94.196.36");//���÷�����IP


	sockaddr_in addr;//����
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr=inet_addr(localIP);

	addr.sin_port=htons(5001);
	m_Socket=socket(AF_INET,SOCK_DGRAM,0);//����udp socket                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
	if(m_Socket==INVALID_SOCKET)
	{
		MessageBox("�׽��ִ���ʧ�ܣ�");
	}
	char* len="9999";
	if(setsockopt(m_Socket,SOL_SOCKET,SO_SNDBUF,len,4)!=0)
	{
		MessageBox("����ʧ�ܣ�");
	}
	if(bind(m_Socket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)
	{
		MessageBox("�׽��ְ�ʧ�ܣ�");
	}
	//
	//���÷�������IP
	m_ServerIP="127.0.0.1";
	//
	//
	m_pSendBuf=new char[1024*1024];
	m_pHeader=m_pSendBuf;
	int ret=WSAAsyncSelect(m_Socket,m_hWnd,CM_RECEIVED,FD_READ);//CM_RECEIVED��ʾ������������Ϣ�����յ�����ʱ

	/*listen(m_Socket,5);*/

	m_Confirm=TRUE;
	OnOK();
	m_Received=FALSE;
	m_Counter=0;
	SetTimer(1,1000,NULL);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRCclientDlg::OnPaint()
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
HCURSOR CRCclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���յ�����������Ϣ��ִ������Ĳ���
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
		//���������Ϣִ�в�ͬ�Ĳ���
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
			MessageBox(Package.Content,"��ʾ");
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
//ץȡ����DC
//
void CRCclientDlg::OnOK() 
{
	CDC* pDeskDC =  GetDesktopWindow()->GetDC();		//��ȡ���滭������
	CRect rc;
	GetDesktopWindow()->GetClientRect(rc);				//��ȡ��Ļ�Ŀͻ�����

	CDC  memDC;											//����һ���ڴ滭��
	memDC.CreateCompatibleDC(pDeskDC);					//����һ�����ݵĻ���
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDeskDC,rc.Width(),rc.Height());	//��������λͼ
	memDC.SelectObject(&bmp);							//ѡ��λͼ����	
	BITMAP bitmap;
	bmp.GetBitmap(&bitmap);

	int panelsize  = 0;									//��¼��ɫ���С
	if (bitmap.bmBitsPixel<16)							//�ж��Ƿ�Ϊ���ɫλͼ
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
	//����ÿ��λͼ���͵Ĵ���
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
//������5�벻��Ӧ�������·���
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
	//������ݱ�
	*(WORD*)&pPackage[0] = index;	
	if (index != frames-1)			
		*(WORD*)&pPackage[2] = 0;		
	else
		*(WORD*)&pPackage[2] = 1;
	//���λͼ����
	pSendBuf += bmpsize*index;
	memcpy(&pPackage[4],pSendBuf,packsize-12);
	//���λͼ��С
	*(int*)&pPackage[packsize-12] = bmpsize;
	//���JPG�����ܴ�С
	*(int*)&pPackage[packsize-8] = totalsize;
	//������ݱ���С
	*(int*)&pPackage[packsize-4] = packsize;
	m_Confirm = FALSE;

	char* len="9999";//���÷��͵��׽���m_Socket
	/*SOCKET  socketBro=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	setsockopt(socketBro,SOL_SOCKET,SO_SNDBUF,len,4);*/

	int ret   = sendto(m_Socket,pPackage,packsize,0,(sockaddr*)&addr,sizeof(addr));
	
	int res=WSAGetLastError();

	delete[] pPackage;
	return ret;

}


