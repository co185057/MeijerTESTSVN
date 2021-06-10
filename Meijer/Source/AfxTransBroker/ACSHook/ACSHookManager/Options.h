#pragma once

namespace Options
{
   void Initialize(long terminalNumber);

   BOOL SetOptionValue(LPCTSTR szSectionName, LPCTSTR szOptName, LPCTSTR szOptValue);

   CString OptionValue(LPCTSTR szOptName);
   CString OptionValue(LPCTSTR szOptName, LPCTSTR szDefaultValue, BOOL bUpdateIfNoExist = TRUE);

   BOOL BoolOptionValue(LPCTSTR szOptName);
   BOOL BoolOptionValue(LPCTSTR szOptName, BOOL bDefaultValue, BOOL bUpdateIfNoExist = TRUE);

   BOOL BoolVal(CString strVal);
   long LongVal(LPCTSTR szVal);
   float FloatVal(LPCTSTR szVal);	
}
