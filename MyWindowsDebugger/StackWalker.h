#pragma once

#include<Windows.h>
#include<DbgHelp.h>


void RetrieveCallStack(HANDLE threadHandle, HANDLE processHandle);