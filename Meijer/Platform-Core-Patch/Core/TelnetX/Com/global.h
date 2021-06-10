#ifndef __GLOBAL_INCLUDED
#define __GLOBAL_INCLUDED

#include <vector>
using namespace std;

#pragma once
typedef struct {
	long Length;
	LPTSTR Data;
} tagOutBuffer;

typedef vector<void *> typeOutputBuffer;

#pragma once
typedef struct ErrTable
{
	int code;
	LPTSTR msg;
}

ErrTable, FAR * LPErrTable;


extern ErrTable StateLookupTable[];
extern TCHAR* GetStateString(int err);

typedef enum SecureStoreLocations
{
	StoreLocalMachine=0,
	StoreCurrentService,
	StoreCurrentUser,
	StoreCurrentUserGroupPolicy,
	StoreLocalMachineEnterprise,
	StoreLocalMachineGroupPolicy,
	StoreServices,
	StoreUsers
} SecureStoreLocations;


#endif