
// Base64_ToolsDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CBase64_ToolsDlg dialog
class CBase64_ToolsDlg : public CDialogEx
{
// Construction
public:
	CBase64_ToolsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BASE64_TOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void outPrint(CString strText, int SetColor);

public:
	afx_msg void OnBnClickedEncodeUtilButton();
	afx_msg void OnBnClickedDecodeUtilButton();
	afx_msg void OnBnClickedSetEncPathButton();
	afx_msg void OnBnClickedSetDecPathButton2();
	afx_msg void OnBnClickedClearButton();

	CRichEditCtrl m_reShowMsg;
	CString m_strEncodeFilePath;
	CString m_strDecodeFilePath;

	afx_msg void OnBnClickedMd5CmpButton(); 
	afx_msg void OnBnClickedFilePathAButton();
	afx_msg void OnBnClickedFilePathBButton();
	CString m_strMD5FilePath_A;
	CString m_strMD5FilePath_B;
	afx_msg void OnBnClickedDecodeMgButton();
	afx_msg void OnBnClickedEncodeMgButton();
};
