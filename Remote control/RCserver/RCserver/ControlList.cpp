// ControlList.cpp : 实现文件
//

#include "stdafx.h"
#include "RCserver.h"
#include "ControlList.h"
#include "afxdialogex.h"
#include "RCserverDlg.h"


// CControlList 对话框

IMPLEMENT_DYNAMIC(CControlList, CDialogEx)

CControlList::CControlList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlList::IDD, pParent)
{

}

CControlList::~CControlList()
{
}

void CControlList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Text);
}


BEGIN_MESSAGE_MAP(CControlList, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CControlList::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CControlList::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CControlList::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CControlList::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CControlList::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CControlList::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CControlList::OnBnClickedButton7)
END_MESSAGE_MAP()


// CControlList 消息处理程序


void CControlList::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	CString str;
	m_Text.GetWindowTextA(str);
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_SendText;
	strncpy(Package.Content,str,511);
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_HideDesk;
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_ShowDesk;
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton4()
{
		CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_ShutDown;
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_ReRun;
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_Logout;
	sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
}


void CControlList::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CRCserverDlg * pDlg = (CRCserverDlg*)AfxGetMainWnd();
	DataPackage Package;
	memset(&Package,0,sizeof(DataPackage));
	Package.MsgType=CT_CloseClient;
	int ret=sendto(pDlg->m_Socket,(char*)&Package,sizeof(DataPackage),0,(sockaddr*)&pDlg->m_Addr,sizeof(sockaddr_in));
	 int res=WSAGetLastError();
}
