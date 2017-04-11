#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"

UINT SymbolList(LPVOID IParam)
{
	CStaticAnalyzerDlg* Dlg = (CStaticAnalyzerDlg* )IParam;
	CString savePath = Dlg->TargetPath.targetSavePath;
	CString tmpFilePath = savePath + "\\tmpSymbol.txt";
	CString symbolFilePath = savePath + "\\Symbol.txt";
	CString binaryFile = Dlg->TargetPath.targetBinaryFile;
	CString cmdStr;
	cmdStr.Format(_T(".\\nm.exe > %s %s -a -n -S"),tmpFilePath,binaryFile);
	system((CStringA)(cmdStr));

	while(!PathFileExists(tmpFilePath));
	CStdioFile tmpFile;
	CFile symbolFile;
	tmpFile.Open(tmpFilePath,CFile::shareDenyNone | CFile::modeRead);
	symbolFile.Open(symbolFilePath,CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite);

	CString readBuf;
	while(tmpFile.ReadString(readBuf)){
		if(readBuf.Find(_T(" D "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			symbolFile.Seek(0L, CFile::end);
			symbolFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T(" B "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			symbolFile.Seek(0L, CFile::end);
			symbolFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T(" d "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			symbolFile.Seek(0L, CFile::end);
			symbolFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T(" b "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			symbolFile.Seek(0L, CFile::end);
			symbolFile.Write(outputString,strlen(outputString));
		}
	}
	tmpFile.Close();
	symbolFile.Close();
	cmdStr.Format(_T("del %s"),tmpFilePath);
	system((CStringA)(cmdStr));

	AfxMessageBox(_T("Symbol List Gen"));
	AfxGetMainWnd()->GetDlgItem(IDC_SYMBOL)->EnableWindow(TRUE);
	return 0;
}