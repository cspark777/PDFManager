//////////////////////////////////////////////////////////////////////
//	Implemented by Samuel Gonzalo 
//
//	You may freely use or modify this code 
//////////////////////////////////////////////////////////////////////
//
// FileFinder.h: interface for the CFileFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__FILEFINDER_H__)
#define __FILEFINDER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Path.h"

enum FindOptionsEnum 
{
	FIND_SIZE			= (1L << 0),
	FIND_DATEMODIFIED	= (1L << 1),
	FIND_DATECREATED	= (1L << 2),
	FIND_DATEACCESSED	= (1L << 3),
	FIND_ATTRIBUTES		= (1L << 4),
	FIND_TEXT			= (1L << 5),
};

enum FileFinderProcCodes
{
	FF_FOUND,
	FF_DISCARDED,
	FF_FOLDER,
	FF_FINDTEXT,
};

class CFileFinder;
typedef void (*FILEFINDERPROC) (CFileFinder *pFinder, DWORD dwCode, void *pCustomParam);

class CFileFinder  
{
public:
	int m_nTotalFileCount;
	
public:
	CFileFinder();
	virtual ~CFileFinder();

	class CFindOpts
	{
	public:
		CFindOpts() 
		{	
			Reset();
		}

		~CFindOpts() 
		{
			sExludedDirs.RemoveAll();
		}

		void operator=(const CFindOpts& objectSrc) 
		{
			sBaseFolder = objectSrc.sBaseFolder;	
			sFileMask = objectSrc.sFileMask;
			sFindText = objectSrc.sFindText;
			bSubfolders = objectSrc.bSubfolders;
			dwOptionsFlags = objectSrc.dwOptionsFlags;
			nMinSize = objectSrc.nMinSize;
			nMaxSize = objectSrc.nMaxSize;
			tMinCreated = objectSrc.tMinCreated;
			tMaxCreated = objectSrc.tMaxCreated;
			tMinModified = objectSrc.tMinModified;
			tMaxModified = objectSrc.tMaxModified;
			tMinAccessed = objectSrc.tMinAccessed;
			tMaxAccessed = objectSrc.tMaxAccessed;
			dwFileAttributes = objectSrc.dwFileAttributes;

			for(POSITION pos = objectSrc.sExludedDirs.GetHeadPosition();pos;)
				sExludedDirs.AddTail(objectSrc.sExludedDirs.GetNext(pos));
		}

		// Reset all values
		void Reset()
		{
			sBaseFolder.Empty();
			sFileMask = "*.*";
			bSubfolders = FALSE;
			nMinSize = nMaxSize = 0;
			dwFileAttributes = 0;
			dwOptionsFlags = 0;
			tMinCreated = CTime::GetCurrentTime();
			tMaxCreated = CTime::GetCurrentTime();
			tMinModified = CTime::GetCurrentTime();
			tMaxModified = CTime::GetCurrentTime();
			tMinAccessed = CTime::GetCurrentTime();
			tMaxAccessed = CTime::GetCurrentTime();
			sExludedDirs.RemoveAll();
		}

		// Add normal files (FILE_ATTRIBUTE_ARCHIVE) to the search
		void FindNormalFiles()
		{
			dwOptionsFlags |= FIND_ATTRIBUTES;
			dwFileAttributes |= (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_NORMAL); //NORMAL added by Tono, Jul 18, 2008 (Vista)
			dwFileAttributes = dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY; //do not find the folders, TONO, Jul 18 2008 (Vista)
		}

		// Add system files (FILE_ATTRIBUTE_SYSTEM) to the search
		void FindSystemFiles()
		{
			dwOptionsFlags |= FIND_ATTRIBUTES;
			dwFileAttributes |= (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_SYSTEM); //NORMAL added by Tono, Jul 18, 2008 (Vista)
			dwFileAttributes = dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY; //do not find the folders, TONO, Jul 18 2008 (Vista)
		}

		// Add temp files (FILE_ATTRIBUTE_TEMPORARY) to the search
		void FindTempFiles()
		{
			dwOptionsFlags |= FIND_ATTRIBUTES;
			dwFileAttributes |= (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_TEMPORARY); //NORMAL added by Tono, Jul 18, 2008 (Vista)
			dwFileAttributes = dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY; //do not find the folders, TONO, Jul 18 2008 (Vista)
		}

		// Add all files to the search (hidden, read-only, ...) but no directories
		void FindAllFiles()
		{
			dwOptionsFlags |= FIND_ATTRIBUTES;
			dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_COMPRESSED | 
								FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY | 
								FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY |
								FILE_ATTRIBUTE_NORMAL; // Normal added by TONO, Jul 18 2008 (Vista)
			dwFileAttributes = dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY; //do not find the folders, TONO, Jul 18 2008 (Vista)
		}

		// Add directories to the search
		void FindDirectories()
		{
			dwOptionsFlags |= FIND_ATTRIBUTES;
			dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
		}

		// Add directories to be excluded
		void AddExcludedDirs(CString sDir)
		{
			sExludedDirs.AddTail(sDir);
		}

		// Search for szText string in the files
		void FindText(LPCTSTR szText)
		{
			dwOptionsFlags |= FIND_TEXT;
			sFindText = szText;
		}

		CString		sBaseFolder;			// The starting folder for the search
		CString		sFileMask;				// File mask (e.g.: "*.txt")
		CString		sFindText;				// Text to find in the files
		BOOL		bSubfolders;			// TRUE for recursive search
		DWORD		dwOptionsFlags;			// Values in FindOptionsEnum
		__int64		nMinSize;				// File minimun size
		__int64		nMaxSize;				// File maximum file
		CTime		tMinCreated;			// File oldest creation date
		CTime		tMaxCreated;			// File newest creation date
		CTime		tMinModified;			// File oldest modified date
		CTime		tMaxModified;			// File newest modified date
		CTime		tMinAccessed;			// File oldest accessed date
		CTime		tMaxAccessed;			// File newest accessed date
		DWORD		dwFileAttributes;		// like in WIN32_FIND_DATA
		CStringList	sExludedDirs;			// folders list to be excluded from the search
	};

	// Find files matching the mask under the base folder
	int		FindFiles(LPCTSTR szBaseFolder, LPCTSTR szFileMask, BOOL bSubFolders = FALSE);
	// Find files matching the conditions stablished in the CFindOpts class parameter
	int		Find(CFileFinder::CFindOpts &opts);

	// Return TRUE if the text szText was found in the file szFile
	BOOL	FindTextInFile(LPCTSTR szFile, LPCTSTR szText);

	// Return the count of items found up to the moment
	int		GetFileCount();
	// Return szPath file index in the list or -1 if it wasn't found
	int		FindPathItem(LPCTSTR szPath);
	// Return a CPath object with the required file index path
	CPath	GetFilePath(int nIndex);

	// Remove item at nIndex position
	void	RemoveAt(int nIndex);
	// Remove all items from the list
	void	RemoveAll();

	// Set the find method callback function
	void	SetCallback(FILEFINDERPROC pFileFinderProc, void *pCustomParam);
	// Stop the search process started by a call to Find (or FindFiles)
	void	StopSearch();
	// Return the current folder being searched
	LPCTSTR	GetSearchingFolder();

private:
	CStringArray	_aFilesFound;
	bool			_bStopSearch;
	FILEFINDERPROC	_pFileFinderProc;
	void			*_pCustomParam;
	CString			_sSearchingFolder;
};

#endif // !defined(__FILEFINDER_H__)
