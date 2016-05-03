#include "stdafx.h"
#include "Define.h"
#include "CExportUtils.h"
#include "zip.h"
#include "unzip.h"
#include "CAlertDlg.h"
#include "CProgressDlg.h"
#include "MainFrm.h"

bool CExportUtils::ExportFilesToFolder(int inCollectionID, CString inFolder, bool inIncludeDB)
{
	CTFiles lFilesTable;
	if (!PrepareFileList(inCollectionID, &lFilesTable))
		return false;

	CString lFolder = inFolder;
	if (lFolder.Right(1) == _T("\\"))
		lFolder.Delete(lFolder.GetLength()-1);

	//check if folder exists
	CreateDirectory(inFolder, NULL);

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Exporting files to folder..."));
	lProgressDlg->CreateProgressDlg();

	if (inCollectionID != FILTERTREE_CURRENT_SELECTION)
	{
		while(!lFilesTable.mQ.eof())
		{
			lFilesTable.LoadCurrentRow();
			lFilesTable.UpdateFileOnDiskFromMembers();

			lProgressDlg->SetMessage(lFilesTable.mName);
			lProgressDlg->StepIt();
			if (lProgressDlg->IsCancelled()) break;

			CString lNewPath = lFolder + _T("\\") + lFilesTable.mName; 
			CopyFile(lFilesTable.mPath, lNewPath, FALSE);

			lFilesTable.mQ.nextRow();
		}
		lProgressDlg->StepIt();
	}
	else
	{
		//get file ids from current selection
		CMainFrame* lMainFrm = (CMainFrame*)AfxGetMainWnd();
		int lCount = lMainFrm->m_wndView.GetListSelectedCount();
		if(lCount > 0)
		{
			POSITION lPos = lMainFrm->m_wndView.GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = lMainFrm->m_wndView.GetListNextSelectedItem(lPos);
				int lFileID = CUtils::sFilesArray[lSelItemIdx].mID;

				lFilesTable.GetFileByID(lFileID);
				lFilesTable.UpdateFileOnDiskFromMembers();
				lFilesTable.LoadCurrentRow();

				lProgressDlg->SetMessage(lFilesTable.mName);
				lProgressDlg->StepIt();
				if (lProgressDlg->IsCancelled()) break;

				CString lNewPath = lFolder + _T("\\") + lFilesTable.mName; 
				CopyFile(lFilesTable.mPath, lNewPath, FALSE);
			}
		}
	}

	delete lProgressDlg;
	lProgressDlg = NULL;

	return true;
}

bool CExportUtils::ExportFilesToZip(int inCollectionID, CString inZipPath, CString inPassword, bool inIncludeDB)
{
	CTFiles lFilesTable;
	if (!PrepareFileList(inCollectionID, &lFilesTable))
		return false;

	CProgressDlg* lProgressDlg = new CProgressDlg;
	lProgressDlg->SetMax(lFilesTable.GetCount());
	lProgressDlg->SetCaption(_T("Exporting files to zip archive..."));
	lProgressDlg->CreateProgressDlg();

	bool lCanceled = false;

	HZIP hz = CreateZip(inZipPath, CT2CA(inPassword));

	if (inCollectionID != FILTERTREE_CURRENT_SELECTION)
	{
		while(!lFilesTable.mQ.eof())
		{
			lFilesTable.LoadCurrentRow();

			lProgressDlg->SetMessage(lFilesTable.mName);
			
			ZipAdd(hz, lFilesTable.mName, lFilesTable.mPath);

			if (lProgressDlg->IsCancelled())
			{
				lCanceled = true;
				break;
			}

			lFilesTable.mQ.nextRow();
		}
	}
	else
	{
		//get file ids from current selection
		CMainFrame* lMainFrm = (CMainFrame*)AfxGetMainWnd();
		int lCount = lMainFrm->m_wndView.GetListSelectedCount();
		if(lCount > 0)
		{
			POSITION lPos = lMainFrm->m_wndView.GetListFirstSelectedItemPosition();
			while(lPos)
			{
				int lSelItemIdx = lMainFrm->m_wndView.GetListNextSelectedItem(lPos);
				int lFileID = CUtils::sFilesArray[lSelItemIdx].mID;

				lFilesTable.GetFileByID(lFileID);
				lFilesTable.LoadCurrentRow();

				lProgressDlg->SetMessage(lFilesTable.mName);
				lProgressDlg->StepIt();
				if (lProgressDlg->IsCancelled()) break;

				ZipAdd(hz, lFilesTable.mName, lFilesTable.mPath);
			}
		}
	}


	delete lProgressDlg;
	lProgressDlg = NULL;

	CloseZip(hz);

	if (lCanceled)
	{	
		DeleteFile(inZipPath);
	}

	return true;
}

bool CExportUtils::PrepareFileList(int inCollectionID, CTFiles* outFilesTable)
{
	CDBSQL lFilesSQL;
	lFilesSQL.Clear();

	CString lCondition;

	lFilesSQL.AddTable(_T("Files"));

	CUtils::UpdateSQLFromCollectionPanel(inCollectionID, &lFilesSQL);

	CString lSQLStr = lFilesSQL.GetSelect();
	if (!lSQLStr.IsEmpty()) 
	{
		outFilesTable->mQ = gDB.mDB.execQuery(lSQLStr);
		return true;
	}

	return false;
}