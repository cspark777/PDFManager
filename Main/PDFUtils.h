#pragma once

bool pdfAttachFile(const String& pdfPath, const String& password, const String& filePath, const String& desc);
bool pdfRemoveFile(const String& pdfPath, const String& password, int index);
bool pdfRemoveAllFiles(const String& pdfPath, const String& password);
bool pdfRemoveAllAnnots(const String& pdfPath, const String& password);
int pdfSecurityInfo(DebenuPDFLibraryAX0912& qpdf, int analyseID, int code);
String pdfSecurityInfoStr(DebenuPDFLibraryAX0912& qpdf, int analyseID, int code);

CString DecodeDate(CString strEncodedDate);
CString DecodeMonth(CString strMonthNumber);
CString DecodeDay(CString strDayNumber);
