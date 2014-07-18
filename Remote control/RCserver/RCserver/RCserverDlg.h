
// RCserverDlg.h : 头文件
//

#pragma once
#include"ControlList.h"

#define CM_RECEIVED  WM_USER+1001
// CRCserverDlg 对话框
class CRCserverDlg : public CDialogEx
{
// 构造
public:
	CRCserverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	/**************************定义命令类型***********************
CT_Confirm: 确认信息
CT_HideDesk:隐藏桌面
CT_ShowDesk:显示桌面
CT_ShutDown:关机
CT_ReRun:重新启动
CT_Logout:注销
CT_SendText:发送文本
CT_CloseClient:结束客户端程序
**************************************************************/
	enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
				CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
	{
		CmdType	MsgType;		//命令类型
		char	Content[512];	//文本信息,只有在命令类型为CT_SendText时合法
	};

	CControlList  m_CtrlList;
	ULONG_PTR m_pGdiToken;
    GdiplusStartupInput m_gdiplusStartupInput;

	SOCKET  m_Socket;					//定义套接字
	char*	m_BmpData;					//位图数据指针
	char*   m_Header;					//临时指针
	BOOL    m_ShowBmp;					//是否显示位图
	char*   m_TempData;					//临时数据指针
	int     m_BmpSize;					//记录位图大小
	int     m_RecSize;					//实际接收的大小
	CString m_ClientIP;					//客户端IP
	UINT    m_ClientPort;				//客户端端口
	DWORD   m_JPGSize;					//JPG数据的总大小	
	BOOL    m_IsScreen;					//是否全屏显示
	WINDOWPLACEMENT m_OldPlacement;		//原来的窗口模式
	CRect m_FullScreenRect;				//全屏显示区域
	IStream *m_pStm;
	Bitmap  *m_pNewBmp;
	sockaddr_in m_Addr;

	LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
	void ShowJPEG(void* pData,int DataSize);
	virtual BOOL PreTranslateMessage(MSG* pMsg);//重写消息函数，实现快捷键

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
