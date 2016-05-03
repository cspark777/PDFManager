#include "stdafx.h"
#include "Define.h"
#include "DebenuPDFLibraryAX0912.h"
#include "PDFUtils.h"

bool pdfAttachFile(const String& pdfPath, const String& password, const String& filePath, const String& desc)
{
  DebenuPDFLibraryAX0912 qpdf;
  qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));

  String   fileName;
  TCHAR    drive[MAX_PATH];
  TCHAR    dir[MAX_PATH];
  TCHAR    file[MAX_PATH];
  TCHAR    ext[MAX_PATH];

  _tsplitpath_s(filePath.c_str(), drive, LENGTHOF(drive), dir, LENGTHOF(dir), file, LENGTHOF(file), ext, LENGTHOF(ext));

  String title;
  if (!desc.empty())
  {
    title = desc;
  }
  else
  {
    title = file;
    title += ext;
  }


  if (qpdf.LoadFromFile(pdfPath, password) != 1)
  {
    CString strMessage;
    strMessage.Format(L"Error accessing %s!", pdfPath.c_str());
    AfxMessageBox(strMessage, MB_ICONSTOP);
    return false;
  }
  CString strMessage;
  if (qpdf.EmbedFile(title, filePath, L"") != 1)
  {
    AfxMessageBox(L"Error attaching file!", MB_ICONSTOP);  
    return false;
  }

  return qpdf.SaveToFile(pdfPath) == 1;
}

bool pdfRemoveFile(const String& pdfPath, const String& password, int index)
{
  DebenuPDFLibraryAX0912 qpdf;
  qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));

  if (qpdf.LoadFromFile(pdfPath, password) != 1)
  {
    CString strMessage;
    strMessage.Format(L"Error accessing %s!", pdfPath.c_str());
    AfxMessageBox(strMessage, MB_ICONSTOP);
    return false;
  }
  CString strMessage;
  if (qpdf.RemoveEmbeddedFile(index) != 1)
  {
    AfxMessageBox(L"Error removing file!", MB_ICONSTOP);  
    return false;
  }

  return qpdf.SaveToFile(pdfPath) == 1;
}

bool pdfRemoveAllFiles(const String& pdfPath, const String& password)
{
  DebenuPDFLibraryAX0912 qpdf;

  qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
  if (qpdf.LoadFromFile(pdfPath, password) != 1)
  {
    CString strMessage;
    strMessage.Format(L"Error accessing %s!", pdfPath.c_str());
    AfxMessageBox(strMessage, MB_ICONSTOP);
    return false;
  }
  CString strMessage;
  int cnt = qpdf.EmbeddedFileCount();
  for (int k = 0; k < cnt; ++k)
  {
    qpdf.RemoveEmbeddedFile(1);
  }
  
  return qpdf.EmbeddedFileCount() == 0 && qpdf.SaveToFile(pdfPath) == 1;
}

bool pdfRemoveAllAnnots(const String& pdfPath, const String& password)
{
  DebenuPDFLibraryAX0912 qpdf;

  qpdf.UnlockKey(_T(QUICKPDF_LICENSE_KEY));
  if (qpdf.LoadFromFile(pdfPath, password) != 1)
  {
    CString strMessage;
    strMessage.Format(L"Error accessing %s!", pdfPath.c_str());
    AfxMessageBox(strMessage, MB_ICONSTOP);
    return false;
  }
  CString strMessage;
  int cnt = qpdf.AnnotationCount();
  for (int k = 0; k < cnt; ++k)
  {
    qpdf.DeleteAnnotation(1);
  }

  return qpdf.AnnotationCount() == 0 && qpdf.SaveToFile(pdfPath) == 1;

}

CString DecodeDate(CString strEncodedDate)
{
  CString strDecodedDate;
  strEncodedDate.MakeLower(); // convert all to lower case

  // Check this is a valid date
  if(strEncodedDate.Mid(0,2)!=_T("d:"))
  {
    return CString(_T("Unknown date"));
  }

  // Year
  CString strYear;
  strYear = strEncodedDate.Mid(2,4);

  // Month
  CString strMonth;
  strMonth = strEncodedDate.Mid(6,2);
  strMonth = DecodeMonth(strMonth);

  // Day
  CString strDay;
  CString strDayAsName;
  strDay = strEncodedDate.Mid(8,2);
  strDayAsName = DecodeDay(strDay);

  // Hour
  CString strHour;
  strHour = strEncodedDate.Mid(10,2);

  // Minute
  CString strMinute;
  strMinute = strEncodedDate.Mid(12,2);

  // Second
  CString strSecond;
  strSecond = strEncodedDate.Mid(14,2);

  //	strDecodedDate.Append(strDayAsName);
  //	strDecodedDate.Append(CString(_T(", ")));
  strDecodedDate.Append(strMonth);
  strDecodedDate.Append(CString(_T(" ")));
  strDecodedDate.Append(strDay);
  strDecodedDate.Append(CString(_T(" ")));
  strDecodedDate.Append(strYear);
  strDecodedDate.Append(CString(_T(", ")));
  strDecodedDate.Append(strHour);
  strDecodedDate.Append(CString(_T(":")));
  strDecodedDate.Append(strMinute);
  strDecodedDate.Append(CString(_T(":")));
  strDecodedDate.Append(strSecond);

  return strDecodedDate;
}

CString DecodeMonth(CString strMonthNumber)
{

  CString strMonth;
  long lMonthNumber = _wtoi(strMonthNumber);

  switch(lMonthNumber)
  {
  case 1:
    strMonth = CString(_T("January"));
    break;
  case 2:
    strMonth = CString(_T("February"));
    break;
  case 3:
    strMonth = CString(_T("March"));
    break;
  case 4:
    strMonth = CString(_T("April"));
    break;
  case 5:
    strMonth = CString(_T("May"));
    break;
  case 6:
    strMonth = CString(_T("June"));
    break;
  case 7:
    strMonth = CString(_T("July"));
    break;
  case 8:
    strMonth = CString(_T("August"));
    break;
  case 9:
    strMonth = CString(_T("September"));
    break;
  case 10:
    strMonth = CString(_T("October"));
    break;
  case 11:
    strMonth = CString(_T("November"));
    break;
  case 12:
    strMonth = CString(_T("December"));
    break;
  default:
    strMonth = CString(_T("Unknown"));
    break;
  }
  return strMonth;
}

CString DecodeDay(CString strDayNumber)
{

  /// NOT CORRECT ||||||||||||||||||||||||| need proper decoder

  CString strDay;
  long lDayNumber = _wtoi(strDayNumber);

  switch(lDayNumber)
  {
  case 1:
    strDay = CString(_T("Monday"));
    break;
  case 2:
    strDay = CString(_T("Tuesday"));
    break;
  case 3:
    strDay = CString(_T("Wednesday"));
    break;
  case 4:
    strDay = CString(_T("Thursday"));
    break;
  case 5:
    strDay = CString(_T("Friday"));
    break;
  case 6:
    strDay = CString(_T("Saturday"));
    break;
  case 7:
    strDay = CString(_T("Sunday"));
    break;
  default:
    strDay = CString(_T("Unknown"));
    break;
  }
  return strDay;
}

int pdfSecurityInfo(DebenuPDFLibraryAX0912& qpdf, int analyseID, int code)
{
  String retStr = qpdf.GetAnalysisInfo(analyseID, 20 + code);
  int nError = qpdf.LastErrorCode();
  int    ret = _wtoi(retStr.c_str());

  return ret;
}

String pdfSecurityInfoStr(DebenuPDFLibraryAX0912& qpdf, int analyseID, int code)
{
  int ret = pdfSecurityInfo(qpdf, analyseID, code);

  static const TCHAR* securityInfoDesc[] =
  {
    _T("None"),
    _T("Adobe Standard Security"),
    _T("No"),
    _T("Yes"),
    _T("Fully Allowed"),
    _T("Not Allowed"),
    _T("Allowed"),
    _T("40-bit RC4 (Acrobat 3.x, 4.x)"),
    _T("128-bit RC4 (Acrobat 5.x)"),
    _T("Unknown"),
    _T("Low Resolution"),
    _T("Blank"),
    _T("128-bit AES (Acrobat 7.x)")
  };

  if (ret > sizeof(securityInfoDesc) / sizeof(securityInfoDesc[0]))
  {
    ret = 0;
  }

  return securityInfoDesc[ret];
}
