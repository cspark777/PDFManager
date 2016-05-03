#pragma once

//custom definitions
//THE TREE ITEM IDs
//Collections 1 - 0xFFFF
//Smart Collections 0x10001 - 0x1FFFF
//Custom Filters	0x20000 - 0x2FFFF
#define FILTERTREE_DEFAULT				0x0
//#define FILTERTREE_COLLECTION			0x00001
//#define FILTERTREE_COLLECTION_MIN		0x0000f
#define FILTERTREE_COLLECTION_MIN		0x00001
#define FILTERTREE_COLLECTION_MAX		0x0FFFF
#define FILTERTREE_SMART_COLLECTION_MIN	0x10000
#define FILTERTREE_SMART_COLLECTION_MAX	0x1FFFF

#define FILTERTREE_MAJOR_BASE			0x30000
#define FILTERTREE_MAJOR_ALL			FILTERTREE_MAJOR_BASE + 1
#define FILTERTREE_MAJOR_DOC			FILTERTREE_MAJOR_BASE + 2
#define FILTERTREE_MAJOR_PRES			FILTERTREE_MAJOR_BASE + 3
#define FILTERTREE_MAJOR_SPREAD			FILTERTREE_MAJOR_BASE + 4
#define FILTERTREE_MAJOR_IMAGES			FILTERTREE_MAJOR_BASE + 5
#define FILTERTREE_MAJOR_VIDEOS			FILTERTREE_MAJOR_BASE + 6
#define FILTERTREE_MAJOR_OTHER			FILTERTREE_MAJOR_BASE + 7

#define FILTERTREE_PREDEF_COLL_BASE		0x40000
#define FILTERTREE_COLLECTION_UNSORTED	(FILTERTREE_PREDEF_COLL_BASE + 1)
#define FILTERTREE_COLLECTION_REC_MODIF	(FILTERTREE_PREDEF_COLL_BASE + 2)
#define FILTERTREE_COLLECTION_REC_ADDED	(FILTERTREE_PREDEF_COLL_BASE + 3)

#define FILTERTREE_CURRENT_SELECTION	0xFFFFFFFF


/*
//Major Filters
#define MAJOR_FILTER_BASE		0	//
#define MAJOR_FILTER_ID_ALL		1
#define MAJOR_FILTER_ID_DOC		2
#define MAJOR_FILTER_ID_PRES	3
#define MAJOR_FILTER_ID_SPREAD	4
#define MAJOR_FILTER_ID_IMAGES	5
//Predefined collections
#define COLLECTION_TYPE_UNINITED		0x0
#define COLLECTION_TYPE_ROOT				0xF00
#define COLLECTION_TYPE_UNSORTED			COLLECTION_TYPE_ROOT + 1
#define COLLECTION_TYPE_REC_MODIFIED		COLLECTION_TYPE_ROOT + 2
#define COLLECTION_TYPE_REC_ADDED			COLLECTION_TYPE_ROOT + 3
#define COLLECTION_TYPE_CUSTOM				COLLECTION_TYPE_ROOT + 100
//Smart Collections

#define COLLECTION_TYPE_ROOT			0xffff //this is the maximum number of the collection, user can create!
#define COLLECTION_TYPE_UNSORTED		COLLECTION_TYPE_ROOT + 1
#define COLLECTION_TYPE_REC_MODIFIED	COLLECTION_TYPE_ROOT + 2
#define COLLECTION_TYPE_REC_ADDED		COLLECTION_TYPE_ROOT + 3
#define COLLECTION_TYPE_CUSTOM			(COLLECTION_TYPE_ROOT + 20)// use the idCollection from the DB!
*/

#define RECENTLY_ROWS_SHOW_NUM 25

#define TABLEID_FILES 1
#define TABLEID_WATCHEDFOLDERS 2

enum DLDropFlags
{
	DL_ACCEPT_FILES =       0x01,	// Allow files to be dropped
	DL_ACCEPT_FOLDERS =		0x02,	// Allow folders to be droppped
	DL_ALLOW_DUPLICATES =	0x04,	// Allow a pathname to be dropped even if its already in the list (ignored if you specify a callback function)
	DL_FILTER_EXTENSION =	0x10,	// Only accept files with the specified extension. Specify in csFileExt
	DL_USE_CALLBACK =		0x20,	// Receive a callback for each item dropped, specified in pfnCallback (you have responsibility for inserting items into the list)
	DL_FOLDER_TYPE =		0x40,	// Returned to the callback function - indicating the type of path dropped
	DL_FILE_TYPE =			0x80
};

#define QUICKPDF_LICENSE_KEY		"jo9da5u85dc7zw5ir6by6nb5y"

#define UM_TRAYNOTIFY	(WM_USER + 1)

//for context menu IDs
#define ID_COLLECTIONID_MENUROOT 35500 //Tono, I hope nobody else uses this ID
#define ID_REMOVE_FROM_COLLECTION_ID_MENUROOT 36500
#define ID_COLLECTIONID_MAXNUM 1000//I hope there will be no more entries in the Collection table
#define ID_ADD_TO_TAGS_ID_MENUROOT 38000
#define ID_REMOVE_FROM_TAGS_ID_MENUROOT 39500
#define ID_TAGSID_MAXNUM 1000

typedef struct _PDF_Entry
{
	CString mPath;
	CString mName;
	CString mTitle;
	CString mAuthor;
	CString mSubject;
	CString mKeywords;
	int mFastWebWiew;
	int mPDFVersion;


// Entries for transfer of non-PDF metadata (messy, but it works)
	CString mComments;
	//int mSelItemIdx;

}PDF_Entry, FAR * LPPDF_Entry;

// static  
BOOL LoadPDFMetadata(CString inFile, PDF_Entry* outEntry);
