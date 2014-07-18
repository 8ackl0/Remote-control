
// RCclientDlg.h : 头文件
//

#pragma once


// CRCclientDlg 对话框
class CRCclientDlg : public CDialogEx
{
// 构造
public:
	CRCclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//定义命令类型
enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
              CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
{
	CmdType  MsgType;	//命令类型
	char  Content[512]; //文本信息
};

	ULONG_PTR m_pGdiToken;
	GdiplusStartupInput m_gdiplusStartupInput;

	int  SendData(UINT index, int mod , int bmpsize ,int totalsize ,int frames,char *pSendBuf, sockaddr_in &addr);
	void OnCancel();

	SOCKET  m_Socket;		//客户端套接字
	CString m_ServerIP;		//服务器IP
	BOOL	m_Confirm;		//确认信息
	int     m_FrameIndex;	//记录索引
	int     m_Mod;			//记录余数
	int     m_Count;		//每一个位图分报的数量
	int     m_Bmpsize;		//位图的实际大小
	char*	m_pSendBuf;		//发送缓冲区指针
	char*	m_pHeader;		//临时指针
	sockaddr_in  m_Addr;	//套接字地址
	BOOL    m_Received;		//是否接收到服务器发来的确认信息
	int     m_Counter;		//记数器
	DWORD	m_JpegSize;		//JPEG数据总大小


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};
