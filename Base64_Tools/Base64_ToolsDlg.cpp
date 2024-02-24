
// Base64_ToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Base64_Tools.h"
#include "Base64_ToolsDlg.h"
#include "afxdialogex.h"
#include <string>
#include "FileUtil.h"
#include "EncodeUtil.h"
#include "FastData.h"
#include <sys/timeb.h>
#include "md5.h"
#include "mongoose.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBase64_ToolsDlg dialog

#define SET_CONTORL_TEXT(IDC_XX, TEXT_STR) SetDlgItemText(IDC_XX, TEXT_STR);


long long getCurrentTime()
{
	timeb now;
	ftime(&now);
	return now.time*1000 + now.millitm;
}


CBase64_ToolsDlg::CBase64_ToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBase64_ToolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBase64_ToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_reShowMsg);
}

BEGIN_MESSAGE_MAP(CBase64_ToolsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ENCODE_BUTTON, &CBase64_ToolsDlg::OnBnClickedEncodeUtilButton)
	ON_BN_CLICKED(IDC_DECODE_BUTTON, &CBase64_ToolsDlg::OnBnClickedDecodeUtilButton)

	ON_BN_CLICKED(IDC_SET_ENC_PATH_BUTTON, &CBase64_ToolsDlg::OnBnClickedSetEncPathButton)
	ON_BN_CLICKED(IDC_SET_DEC_PATH_BUTTON2, &CBase64_ToolsDlg::OnBnClickedSetDecPathButton2)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CBase64_ToolsDlg::OnBnClickedClearButton)
	ON_BN_CLICKED(IDC_MD5_CMP_BUTTON, &CBase64_ToolsDlg::OnBnClickedMd5CmpButton)
	ON_BN_CLICKED(IDC_FILE_PATH_A_BUTTON, &CBase64_ToolsDlg::OnBnClickedFilePathAButton)
	ON_BN_CLICKED(IDC_FILE_PATH_B_BUTTON, &CBase64_ToolsDlg::OnBnClickedFilePathBButton)

	ON_BN_CLICKED(IDC_DECODE_MG_BUTTON, &CBase64_ToolsDlg::OnBnClickedDecodeMgButton)
	ON_BN_CLICKED(IDC_ENCODE_MG_BUTTON, &CBase64_ToolsDlg::OnBnClickedEncodeMgButton)
END_MESSAGE_MAP()


// CBase64_ToolsDlg message handlers

BOOL CBase64_ToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBase64_ToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBase64_ToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBase64_ToolsDlg::outPrint(CString strText, int SetColor)
{
	CHARFORMAT cFmt={0};
	cFmt.cbSize = sizeof(CHARFORMAT);
	if (SetColor==1)
	{
		cFmt.crTextColor = RGB(255,0,0); 
		cFmt.dwEffects |= (CFE_BOLD);	
	}
	else if(SetColor == 2)
	{
		cFmt.crTextColor = RGB(128,0,128); 
		cFmt.dwEffects |= CFE_BOLD;
	}
	else if(SetColor == 3)
	{
		cFmt.crTextColor = RGB(0,0,255); 
		cFmt.dwEffects |= CFE_BOLD;	
	}
	else
	{
		cFmt.crTextColor = RGB(64,64,64);
		cFmt.dwEffects &= ~CFE_BOLD;
	}

	cFmt.dwMask  =  CFM_BOLD | CFM_COLOR|CFM_SIZE;		
	_tcscpy(cFmt.szFaceName, _T("微软雅黑"));

	cFmt.yHeight = 11 * 20;
	CTime time = CTime::GetCurrentTime();
	CString strcurTime = time.Format(_T("%H:%M:%S"));

	CString szText;
	CHARRANGE  SelDate ;
	SelDate.cpMax =-1;
	SelDate.cpMin = -1;
	m_reShowMsg.SetSel(SelDate);

	szText.Format(_T("   %s:  %s  \r\n"),strcurTime,strText);

	BOOL a = m_reShowMsg.SetSelectionCharFormat(cFmt);
	m_reShowMsg.ReplaceSel(szText);
	m_reShowMsg.SetSel(SelDate);
	m_reShowMsg.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CBase64_ToolsDlg::OnBnClickedClearButton()
{
	m_reShowMsg.SetSel(0, -1);  // 选中所有字符
	m_reShowMsg.ReplaceSel(_T(""));
}


void CBase64_ToolsDlg::OnBnClickedSetEncPathButton()
{
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");   

	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal())   
	{
		m_strEncodeFilePath = fileDlg.GetPathName();
		SET_CONTORL_TEXT(IDC_ENC_PATH_EDIT, m_strEncodeFilePath);
	}
}


void CBase64_ToolsDlg::OnBnClickedSetDecPathButton2()
{
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");   

	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal())   
	{
		m_strDecodeFilePath = fileDlg.GetPathName();   
		SET_CONTORL_TEXT(IDC_DEC_PATH_EDIT, m_strDecodeFilePath);
	}
}

void CBase64_ToolsDlg::OnBnClickedMd5CmpButton()
{
	std::string strPath_A(m_strMD5FilePath_A.GetBuffer(0));
	std::string strPath_B(m_strMD5FilePath_B.GetBuffer(0));
	m_strMD5FilePath_A.ReleaseBuffer();
	m_strMD5FilePath_B.ReleaseBuffer();
	if (strPath_A.empty() || strPath_B.empty())
	{
		this->outPrint(TEXT("请先选择需要比对的文件路径"), OutPrintError::Error);

		return;
	}

	std::vector<unsigned char> vecDataFile_A;
	FileUtil::readFile(strPath_A.c_str(), vecDataFile_A);
	if (vecDataFile_A.empty())
	{
		this->outPrint(TEXT("读取文件A出错！"), OutPrintError::Error);

		return;
	}

	std::vector<unsigned char> vecDataFile_B;
	FileUtil::readFile(strPath_B.c_str(), vecDataFile_B);
	if (vecDataFile_B.empty())
	{
		this->outPrint(TEXT("读取文件B出错！"), OutPrintError::Error);

		return;
	}

	MD5 md5_FileA(vecDataFile_A.data(), vecDataFile_A.size());
	MD5 md5_FileB(vecDataFile_B.data(), vecDataFile_B.size());

	if (md5_FileA.toString() == md5_FileB.toString())
	{
		CString strMsg;
		strMsg.Format(TEXT("[%s]与[%s]内容一致！"), strPath_A.c_str(), strPath_B.c_str());
		this->outPrint(strMsg, OutPrintError::Succeed);
	} 
	else
	{
		CString strMsg;
		strMsg.Format(TEXT("[%s]与[%s]内容不同！！"), strPath_A.c_str(), strPath_B.c_str());
		this->outPrint(strMsg, OutPrintError::Error);
	}
}


void CBase64_ToolsDlg::OnBnClickedFilePathAButton()
{
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");   

	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal())   
	{
		m_strMD5FilePath_A = fileDlg.GetPathName();   
		SET_CONTORL_TEXT(IDC_CMP_PATH_A_EDIT, m_strMD5FilePath_A);
	}
}


void CBase64_ToolsDlg::OnBnClickedFilePathBButton()
{
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");   

	CFileDialog fileDlg(TRUE, _T("dat"), NULL, 0, szFilter, this);

	if (IDOK == fileDlg.DoModal())   
	{
		m_strMD5FilePath_B = fileDlg.GetPathName();   
		SET_CONTORL_TEXT(IDC_CMP_PATH_B_EDIT2, m_strMD5FilePath_B);
	}
}


void CBase64_ToolsDlg::OnBnClickedEncodeUtilButton()
{
	std::string strPath(m_strEncodeFilePath.GetBuffer(0));
	m_strEncodeFilePath.ReleaseBuffer();
	if (strPath.empty())
	{
		this->outPrint(TEXT("请先选择需要编码的文件的路径"), OutPrintError::Error);

		return;
	}

	// TODO: 编码后文件名为xxx-yyyy-enc.dat

	std::vector<unsigned char> vecData;
	FileUtil::readFile(strPath.c_str(), vecData);

	std::string strEncodedData = EncodeUtil::Base64Encode(vecData.data(), vecData.size());

	if(strEncodedData.empty())
	{
		CString strEndShow;
		strEndShow.Format(TEXT("文件解码失败[%s]!"), strPath.c_str());
		this->outPrint(strEndShow, OutPrintError::Error);

		return;
	}

	// 解码后文件名为xxx-yyyy-enc.dat
	std::string strEncodedFilePath(strPath);
	size_t pos = strPath.find_last_of(".");
	if (pos != std::string::npos)
	{
		strEncodedFilePath = strPath.substr(0, pos);
	}

	long long currTimespan = getCurrentTime();
	//strEncodedFilePath += "-" + std::to_string(currTimespan);
	strEncodedFilePath += "-enc_util.dat";

	FileUtil::saveFile(strEncodedFilePath.c_str(), strEncodedData.c_str(), strEncodedData.size());

	CString strEndShow;
	strEndShow.Format(TEXT("文件编码完成[%s]!"), strEncodedFilePath.c_str());
	this->outPrint(strEndShow, OutPrintError::Succeed);
}

void CBase64_ToolsDlg::OnBnClickedDecodeUtilButton()
{
	std::string strPath(m_strDecodeFilePath.GetBuffer(0));
	m_strDecodeFilePath.ReleaseBuffer();
	if (strPath.empty())
	{
		this->outPrint(TEXT("请先选择需要编码的文件的路径"), OutPrintError::Error);

		return;
	}

	// TODO: 编码后文件名为xxx-yyyy-enc.dat

	std::vector<unsigned char> vecData;
	FileUtil::readFile(strPath.c_str(), vecData);

	std::string strFF((const char*)vecData.data());
	FastData fData = EncodeUtil::Base64DecodeToBytes(strFF);
	int decLen = fData.getSize();

	if(decLen <= 0)
	{
		CString strEndShow;
		strEndShow.Format(TEXT("文件解码util失败[%s]!"), strPath.c_str());
		this->outPrint(strEndShow, OutPrintError::Error);

		return;
	}

	// 解码后文件名为xxx-enc.dat
	std::string strDecodedFilePath(strPath);
	size_t pos = strPath.find_last_of(".");
	if (pos != std::string::npos)
	{
		strDecodedFilePath = strPath.substr(0, pos);
	}

	long long currTimespan = getCurrentTime();
	//strDecodedFilePath += "-" + std::to_string(currTimespan);
	strDecodedFilePath += "-dec_util.dat";

	FileUtil::saveFile(strDecodedFilePath.c_str(), (const char*)fData.getBytes(), decLen);

	CString strEndShow;
	strEndShow.Format(TEXT("文件解码util完成[%s],长度为：[%d]!"), strDecodedFilePath.c_str(), decLen);
	this->outPrint(strEndShow, OutPrintError::Succeed);
}


void CBase64_ToolsDlg::OnBnClickedDecodeMgButton()
{
	std::string strPath(m_strDecodeFilePath.GetBuffer(0));
	m_strDecodeFilePath.ReleaseBuffer();
	if (strPath.empty())
	{
		this->outPrint(TEXT("请先选择需要编码的文件的路径"), OutPrintError::Error);

		return;
	}

	// TODO: 编码后文件名为xxx-yyyy-enc.dat

	std::vector<unsigned char> vecData;
	FileUtil::readFile(strPath.c_str(), vecData);

	std::string strFF((const char*)vecData.data());

	// TODO
	int decodeSize = vecData.size();
	std::vector<unsigned char*> vecDecodedData;
	vecDecodedData.resize(decodeSize);

	cs_base64_decode((unsigned char*)vecData.data(), vecData.size(), (char*)vecDecodedData.data(), &decodeSize);
	vecDecodedData.resize(decodeSize);

	if(decodeSize <= 0)
	{
		CString strEndShow;
		strEndShow.Format(TEXT("文件解码mg失败[%s]!"), strPath.c_str());
		this->outPrint(strEndShow, OutPrintError::Error);

		return;
	}

	// 解码后文件名为xxx-yyyy-enc.dat
	std::string strDecodedFilePath(strPath);
	size_t pos = strPath.find_last_of(".");
	if (pos != std::string::npos)
	{
		strDecodedFilePath = strPath.substr(0, pos);
	}

	long long currTimespan = getCurrentTime();
	//strDecodedFilePath += "-" + std::to_string(currTimespan);
	strDecodedFilePath += "-dec_mg.dat";

	FileUtil::saveFile(strDecodedFilePath.c_str(), (const char*)vecDecodedData.data(), vecDecodedData.size());

	CString strEndShow;
	strEndShow.Format(TEXT("文件解码mg完成[%s]!"), strDecodedFilePath.c_str());
	this->outPrint(strEndShow, OutPrintError::Succeed);
}


void CBase64_ToolsDlg::OnBnClickedEncodeMgButton()
{
	// TODO: Add your control notification handler code here

	std::string strPath(m_strEncodeFilePath.GetBuffer(0));
	m_strEncodeFilePath.ReleaseBuffer();
	if (strPath.empty())
	{
		this->outPrint(TEXT("请先选择需要编码的文件的路径"), OutPrintError::Error);

		return;
	}

	// 编码后文件名为xxx-yyyy-enc.dat

	std::vector<unsigned char> vecData;
	FileUtil::readFile(strPath.c_str(), vecData);

#if 1
	char* strEncode = new char[vecData.size()*2];
	memset(strEncode, 0, vecData.size()*2);

	cs_base64_encode(vecData.data(), vecData.size(), strEncode);

	std::string strEncodedData = strEncode;

	delete[] strEncode; strEncode=nullptr;
#endif

	if(strEncodedData.empty())
	{
		CString strEndShow;
		strEndShow.Format(TEXT("文件解码失败[%s]!"), strPath.c_str());
		this->outPrint(strEndShow, OutPrintError::Error);

		return;
	}

	// 解码后文件名为xxx-yyyy-enc.dat
	std::string strEncodedFilePath(strPath);
	size_t pos = strPath.find_last_of(".");
	if (pos != std::string::npos)
	{
		strEncodedFilePath = strPath.substr(0, pos);
	}

	long long currTimespan = getCurrentTime();
	//strEncodedFilePath += "-" + std::to_string(currTimespan);
	strEncodedFilePath += "-enc_mg.dat";

	FileUtil::saveFile(strEncodedFilePath.c_str(), strEncodedData.c_str(), strEncodedData.size());

	CString strEndShow;
	strEndShow.Format(TEXT("文件编码完成[%s]!"), strEncodedFilePath.c_str());
	this->outPrint(strEndShow, OutPrintError::Succeed);
}
