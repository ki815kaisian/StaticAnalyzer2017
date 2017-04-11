#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"

UINT ObjectFileList(LPVOID IParam)
{
	CStaticAnalyzerDlg* Dlg = (CStaticAnalyzerDlg* )IParam;
	CString savePath = Dlg->TargetPath.targetSavePath;
	CString tmpFilePath = savePath + "\\tmpObject.txt";
	CString sectionFilePath = savePath + "\\Object.txt";
	CString binaryFile = Dlg->TargetPath.targetBinaryFile;
	CString cmdStr;
	cmdStr.Format(_T(".\\readelf.exe > %s %s -s"),tmpFilePath,binaryFile);
	system((CStringA)(cmdStr));
	//readelf·Î º¯°æ
	while(!PathFileExists(tmpFilePath));
	CStdioFile tmpFile;
	CFile objectFile;
	tmpFile.Open(tmpFilePath,CFile::shareDenyNone | CFile::modeRead);
	objectFile.Open(sectionFilePath,CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite);

	CString readBuf;
	CString readTmp1;
	CString readTmp2;
	int objectFlag = 0;
	while(tmpFile.ReadString(readBuf)){
		if(readBuf.Find(_T("00000000     0 FILE    ")) >= 0){
			objectFlag = 1;
			readTmp1 = readBuf;
			readTmp1.Append(_T("\r\n"));
			//CT2CA outputString(readTmp, CP_UTF8);
			//objectFile.Seek(0L, CFile::end);
			//objectFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T("OBJECT  LOCAL")) >= 0  && objectFlag == 1){
			CString tmpString = readBuf;
			readTmp1.Append(tmpString+_T("\r\n"));
		}else if(readBuf.Find(_T(".Ltext0")) >= 0  && objectFlag == 1){
			objectFlag = 2;
			readTmp2 = readBuf;
			readTmp2.Append(_T("\r\n"));
		}else if(readBuf.Find(_T(".Letext")) >= 0 && objectFlag ==2){
			objectFlag = 0;
			CT2CA outputStringTmp1(readTmp1, CP_UTF8);
			objectFile.Seek(0L, CFile::end);
			objectFile.Write(outputStringTmp1,strlen(outputStringTmp1));
			CT2CA outputStringTmp2(readTmp2, CP_UTF8);
			objectFile.Seek(0L, CFile::end);
			objectFile.Write(outputStringTmp2,strlen(outputStringTmp2));
			readBuf.Append(_T("\r\n"));
			CT2CA outputStringBuf(readBuf, CP_UTF8);
			objectFile.Seek(0L, CFile::end);
			objectFile.Write(outputStringBuf,strlen(outputStringBuf));
		}
	}
	tmpFile.Close();
	objectFile.Close();
	cmdStr.Format(_T("del %s"),tmpFilePath);
	system((CStringA)(cmdStr));

	AfxMessageBox(_T("Object File\r\n List Gen"));
	AfxGetMainWnd()->GetDlgItem(IDC_OBJECT)->EnableWindow(TRUE);
	return 0;
}