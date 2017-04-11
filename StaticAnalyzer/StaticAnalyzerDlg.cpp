
// StaticAnalyzerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"
#include "afxdialogex.h"
#include <Shlobj.h>
//#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int CALLBACK BrowseForFolder_CallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// CStaticAnalyzerDlg ��ȭ ����
CStaticAnalyzerDlg::CStaticAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaticAnalyzerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStaticAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStaticAnalyzerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE_OPEN, &CStaticAnalyzerDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(IDC_SAVE_PATH, &CStaticAnalyzerDlg::OnBnClickedSavePath)
	ON_BN_CLICKED(IDC_SECTION, &CStaticAnalyzerDlg::OnBnClickedSection)
	ON_BN_CLICKED(IDC_SYMBOL, &CStaticAnalyzerDlg::OnBnClickedSymbol)
	ON_BN_CLICKED(IDC_FUNCTION, &CStaticAnalyzerDlg::OnBnClickedFunction)
	ON_BN_CLICKED(IDC_OBJECT, &CStaticAnalyzerDlg::OnBnClickedObject)
END_MESSAGE_MAP()


// CStaticAnalyzerDlg �޽��� ó����

BOOL CStaticAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(FALSE);
	GetDlgItem(IDC_FILE_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_FUNCTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_OBJECT)->EnableWindow(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CStaticAnalyzerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CStaticAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStaticAnalyzerDlg::OnBnClickedFileOpen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog insDlg(TRUE, NULL, _T("*.elf"), OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("ELF Files (*.elf)|*.elf||"));
	CString currentDirName;
	GetDlgItemText(IDC_EDIT_PATH, currentDirName);
	insDlg.m_ofn.lpstrInitialDir =  currentDirName; 
	// ���� ��ȭ���� ����
	if(insDlg.DoModal() == IDOK){
		TargetPath.targetBinaryFile = insDlg.m_ofn.lpstrFile;
		SetDlgItemText(IDC_EDIT_BINARY, TargetPath.targetBinaryFile);
		GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_SYMBOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_FUNCTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_OBJECT)->EnableWindow(TRUE);
	}  
}

void CStaticAnalyzerDlg::OnBnClickedSavePath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	wchar_t currentDirName[MAX_PATH];
	wchar_t selectPath[MAX_PATH];
    BROWSEINFO   pathInfo; 
    LPITEMIDLIST  pathIdList;
 
    memset(&pathInfo, 0x00, sizeof(BROWSEINFO));
    pathInfo.hwndOwner   =   this->m_hWnd; 
	GetCurrentDirectory(MAX_PATH, currentDirName);
	pathInfo.lpfn = BrowseForFolder_CallbackProc;
	pathInfo.lParam = (LPARAM)(LPCSTR)currentDirName;
    pathInfo.ulFlags  =  BIF_EDITBOX| BIF_NEWDIALOGSTYLE; 
 
    pathIdList = SHBrowseForFolder(&pathInfo);
    if (pathIdList) { 
		SHGetPathFromIDList(pathIdList, selectPath);
		TargetPath.targetSavePath = selectPath;
		SetDlgItemText(IDC_EDIT_PATH, TargetPath.targetSavePath);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILE_OPEN)->EnableWindow(TRUE);
    }
}


void CStaticAnalyzerDlg::OnBnClickedSection()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_SECTION)->EnableWindow(FALSE);
	sectionTabelThread = AfxBeginThread(SectionTable, (LPVOID)this);	

}


void CStaticAnalyzerDlg::OnBnClickedSymbol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	symbolListThread = AfxBeginThread(SymbolList, (LPVOID)this);	
}


void CStaticAnalyzerDlg::OnBnClickedFunction()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_FUNCTION)->EnableWindow(FALSE);
	functionListThread = AfxBeginThread(FunctionList, (LPVOID)this);	
}


void CStaticAnalyzerDlg::OnBnClickedObject()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_OBJECT)->EnableWindow(FALSE);
	objectFileListThread = AfxBeginThread(ObjectFileList, (LPVOID)this);	

}

static int CALLBACK BrowseForFolder_CallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lpData);
	return 0;
}
