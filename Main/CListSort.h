#ifndef _CListSort_H_
#define _CListSort_H_

#if _MSC_VER > 1000
#pragma once
#endif

enum ListCompareType 
{  
	ELCT_INTEGER = 0,
	ELCT_DOUBLE,
	ELCT_STRING_CASE,
	ELCT_STRING_NOCASE,
	ELCT_DATE
};
enum ListOperatorType 
{ 
	ELOT_LT = 0,    //  <   less than
	ELOT_GT,		//  >   greater than
	ELOT_LTE,		//  <=  less than or equal
	ELOT_GTE,		//  >=  greather than or equal
	ELOT_EQ			//  ==  equal
};
#endif