#include <Windows.h>
#include <qdebug.h>
#include "processtable.h"

QString Proc_info::PrintAPIErrorMessage(DWORD dwErr)
{
    WCHAR   wszMsgBuff[512];  // Buffer for text.
    DWORD   dwChars;  // Number of chars returned.

    // Try to get the message from the system errors.
    dwChars = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
                             FORMAT_MESSAGE_IGNORE_INSERTS,
                             NULL,
                             dwErr,
                             0,
                             wszMsgBuff,
                             512,
                             NULL );

    if (0 == dwChars)
    {
        // The error code did not exist in the system errors.
        // Try Ntdsbmsg.dll for the error code.

        HINSTANCE hInst;

        // Load the library.
        hInst = LoadLibrary(L"Ntdsbmsg.dll");
        if ( NULL == hInst )
        {
            return QString("cannot load Ntdsbmsg.dll");
        }

        // Try getting message text from ntdsbmsg.
        dwChars = FormatMessage( FORMAT_MESSAGE_FROM_HMODULE |
                                 FORMAT_MESSAGE_IGNORE_INSERTS,
                                 hInst,
                                 dwErr,
                                 0,
                                 wszMsgBuff,
                                 512,
                                 NULL );

        // Free the library.
        FreeLibrary( hInst );

    }

    char errbuf[1024];
    // Display the error message, or generic text if not found.
    snprintf(errbuf, 1024, "#   Error value: %u | Message: %ws\n",
            dwErr,
            dwChars ? wszMsgBuff : L"Error message not found." );

    return QString(errbuf);
}



//
//  SetPrivilege enables/disables process token privilege.
//
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    LUID luid;
    BOOL bRet=FALSE;

    if (LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
    {
        TOKEN_PRIVILEGES tp;

        tp.PrivilegeCount=1;
        tp.Privileges[0].Luid=luid;
        tp.Privileges[0].Attributes=(bEnablePrivilege) ? SE_PRIVILEGE_ENABLED: 0;
        //
        //  Enable the privilege or disable all privileges.
        //
        if (AdjustTokenPrivileges(hToken,
                                  FALSE,
                                  &tp,
                                  sizeof(TOKEN_PRIVILEGES),
                                  (PTOKEN_PRIVILEGES)NULL,
                                  (PDWORD)NULL))
        {
            //
            //  Check to see if you have proper access.
            //  You may get "ERROR_NOT_ALL_ASSIGNED".
            //
            bRet=(GetLastError() == ERROR_SUCCESS);
        }
    }

    return bRet;
}

BOOL SetDebugPrivilege(HANDLE hProcess, BOOL flag)
{
    HANDLE hToken;

    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        SetPrivilege(hToken, SE_DEBUG_NAME, flag);
        SetPrivilege(hToken, SE_SECURITY_NAME , flag);

        CloseHandle(hToken);
        return true;
    }

    return false;
}

QVector<QStringList> GetAcessRights(ACCESS_MASK access_mask)
{
  DWORD constants[]
  {
      GENERIC_READ,
      GENERIC_WRITE,
      GENERIC_EXECUTE,
      GENERIC_ALL,
      DELETE,
      READ_CONTROL,
      WRITE_DAC,
      WRITE_OWNER,
      SYNCHRONIZE,
      STANDARD_RIGHTS_REQUIRED,
      STANDARD_RIGHTS_READ,
      STANDARD_RIGHTS_WRITE,
      STANDARD_RIGHTS_EXECUTE,
      STANDARD_RIGHTS_ALL,
      SPECIFIC_RIGHTS_ALL
  };

  QString str_constants[]
  {
      "GENERIC_READ",
      "GENERIC_WRITE",
      "GENERIC_EXECUTE",
      "GENERIC_ALL",
      "DELETE",
      "READ_CONTROL",
      "WRITE_DAC",
      "WRITE_OWNER",
      "SYNCHRONIZE",
      "STANDARD_RIGHTS_REQUIRED",
      "STANDARD_RIGHTS_READ",
      "STANDARD_RIGHTS_WRITE",
      "STANDARD_RIGHTS_EXECUTE",
      "STANDARD_RIGHTS_ALL",
      "SPECIFIC_RIGHTS_ALL"
  };


  QVector<QStringList> vec;

  for (int i = 0; i < sizeof(constants)/ sizeof(DWORD); i++)
  {
      vec.push_back(QStringList{str_constants[i], (access_mask & constants[i]) ? "Enabled" : "Disabled"});
  }

  return vec;
}

const char *AceTypeString(BYTE AceConstant)
{
    switch (AceConstant)
    {

    case ACCESS_ALLOWED_ACE_TYPE:
        return "ACCESS_ALLOWED_ACE_TYPE";

    case ACCESS_ALLOWED_CALLBACK_ACE_TYPE:
        return "ACCESS_ALLOWED_CALLBACK_ACE_TYPE";

    case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE:
        return "ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE";

    case ACCESS_ALLOWED_COMPOUND_ACE_TYPE:
         return "ACCESS_ALLOWED_COMPOUND_ACE_TYPE";

    case ACCESS_ALLOWED_OBJECT_ACE_TYPE:
         return "ACCESS_ALLOWED_OBJECT_ACE_TYPE";

    case ACCESS_DENIED_ACE_TYPE:
         return "ACCESS_DENIED_ACE_TYPE";

    case ACCESS_DENIED_CALLBACK_ACE_TYPE:
         return "ACCESS_DENIED_CALLBACK_ACE_TYPE";

    case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE:
         return "ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE";

    case ACCESS_DENIED_OBJECT_ACE_TYPE:
         return "ACCESS_DENIED_OBJECT_ACE_TYPE";

    case ACCESS_MAX_MS_ACE_TYPE:
         return "ACCESS_MAX_MS_ACE_TYPE";

    case ACCESS_MAX_MS_V2_ACE_TYPE:
         return "ACCESS_MAX_MS_V2_ACE_TYPE";

//    case ACCESS_MAX_MS_V3_ACE_TYPE:
//         return "ACCESS_MAX_MS_V3_ACE_TYPE";

//    case ACCESS_MAX_MS_V4_ACE_TYPE:
//         return "ACCESS_MAX_MS_V4_ACE_TYPE";

//    case ACCESS_MAX_MS_OBJECT_ACE_TYPE:
//         return "ACCESS_MAX_MS_OBJECT_ACE_TYPE";

//    case ACCESS_MIN_MS_ACE_TYPE:
//         return "ACCESS_MIN_MS_ACE_TYPE";

//    case ACCESS_MIN_MS_OBJECT_ACE_TYPE:
//         return "ACCESS_MIN_MS_OBJECT_ACE_TYPE";

//    case SYSTEM_ALARM_ACE_TYPE:
//         return "SYSTEM_ALARM_ACE_TYPE";

    case SYSTEM_ALARM_CALLBACK_ACE_TYPE:
         return "SYSTEM_ALARM_CALLBACK_ACE_TYPE";

    case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE:
        return "SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE";

//    case SYSTEM_ALARM_OBJECT_ACE_TYPE:
//        return "SYSTEM_ALARM_OBJECT_ACE_TYPE";

    case SYSTEM_AUDIT_ACE_TYPE:
         return "SYSTEM_AUDIT_ACE_TYPE";

    case SYSTEM_AUDIT_CALLBACK_ACE_TYPE:
         return "SYSTEM_AUDIT_CALLBACK_ACE_TYPE";

    case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE:
         return "SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE";

    case SYSTEM_AUDIT_OBJECT_ACE_TYPE:
         return "SYSTEM_AUDIT_OBJECT_ACE_TYPE";

    case SYSTEM_MANDATORY_LABEL_ACE_TYPE:
         return "SYSTEM_MANDATORY_LABEL_ACE_TYPE";
    };
}
