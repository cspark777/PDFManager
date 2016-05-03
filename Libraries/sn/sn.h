#pragma once

#include <atlbase.h>


typedef struct 
{
	CTime expDate;
}AppDatesRec;	

class sn
{
public:
	enum {eStateTrial, eStateExpired, eStateRegistered};

public:
	/*! 
		function state() returns current application state. 
		/param numDays - filled by number of days remaining to expiry. can be null
		/return value can be one of eStateTrial, eStateExpired or eStateRegistered. 
	*/
	static inline int state(int * numDays = NULL)
	{
		CString sn;
		if (read(sn) && check(sn)) return eStateRegistered;

		AppDatesRec dates;
		if (!read2(&dates))
		{
			dates.expDate = CTime::GetCurrentTime();
			
			CTimeSpan ts(14,23,59,59);
			dates.expDate += ts;

			write2(&dates);
		}

		CTime curTime = CTime::GetCurrentTime();

		CTimeSpan ts = dates.expDate - curTime;

		if (ts.GetTotalSeconds() < 0L)
			return eStateExpired;

		if (numDays != NULL)
			*numDays = (int)ts.GetDays();

		if((numDays != NULL) && *numDays > 15L)			//Tono: If the user installs the product with the date set foreward to 1.1.2010, the applications expires just on 15.1.2010 - that is a bug 
			return eStateExpired;

		return eStateTrial;
	}

	/*!
		function get() returns the serial number set for the application
		/return the serial number
	*/
	static inline CString get()
	{
		CString sn;
		if (read(sn) && check(sn))
			return sn;
		return _T("TRIAL");
	}
	
	/*!
		function set() checks and sets the serial number of correct
		/return true if successfull
	*/
	static inline bool set(CString serial)
	{
		return (check(serial) && write(serial));
	}

	/*!
		function generate() returns the newly generated serial number
		/return the serial number
	*/
	static inline CString generate()
	{
		CString lStr;

		int len =  snchr.GetLength();
		int key = 0;
		
		for (int i = 0; i < snlen-1; i++)
		{
			int x = getPos(len-1);
			lStr.AppendChar(snchr[x]);
		}

		int x = getPos(14);
		lStr.AppendChar(snchr[x]);

		//put the date stamp on the firs tour places
		CTime lTime = CTime::GetCurrentTime();
		char tm[20];
		_itoa_s((int)lTime.GetTime(), tm, 16);

		CString lTmStr = CA2CT(tm);

		lStr.SetAt(x, 'B');

		//format string
		CString sn;
		sn.Format(_T("%s-%s-%s-%s-%s"), lTmStr, lStr.Mid(0, 4), lStr.Mid(4, 4), lStr.Mid(8, 4), lStr.Mid(12, 4));
		sn.MakeUpper();



		return sn;
	}

	/*!
		function check() checks the serial number if is correct
		/return the serial number
	*/
	static inline bool check(CString serial)
	{
		CString lStr = serial;
		lStr.MakeUpper();

		int len = lStr.GetLength();

		if (len > 19)
		{
			//delete the timestamp
			lStr.Delete(0, lStr.Find('-')+1);
		}

		lStr.Remove('-');

		len = lStr.GetLength();
		if (len != 16)
			return false;

		TCHAR keyChar = lStr.GetAt(15);

		int pos = sn::snchr.Find(keyChar);
		if (pos > 15)
			return false;

		if (lStr[pos] != 'B')
			return false;

		return true;
	}

private:
	static bool read(CString & serial);
	static bool write(CString serial);
	static bool read2(AppDatesRec* dates);
	static bool write2(AppDatesRec* dates);


	static int getPos(int maxPos);
	static const CString snchr;
	static const int snlen;
};