#ifndef _QPING_H
#define _QPING_H

void QPInitialize(void);
BOOL QuickPing2(CString csMachineName, long PingRetry, long PingTDStart, long PingTDIncr, bool bByNameOnly);

#endif
