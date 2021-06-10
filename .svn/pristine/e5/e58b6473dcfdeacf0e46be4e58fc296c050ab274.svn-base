#ifndef _TBS_TRANSLIST_TYPE
#define _TBS_TRANSLIST_TYPE

typedef struct 
{
  char name[50];
  int id;

  // The following fields are initialized to false and
  // calculated as the tool reads in the transition list

  DWORD numTimes;  // # of time this dynaframe entered
  DWORD totalTime; // total time spent in this dynaframe
  int   minTime;   // min time spent in dynaframe
  int   maxTime;   // max time spent in dynaframe
} TBSLISTTYPE;

extern TBSLISTTYPE tbsListArray[];
extern int tbsListArraySize;

#endif