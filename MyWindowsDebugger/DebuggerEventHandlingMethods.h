#pragma once
#include<Windows.h>
#include"ProcessInfo.h"

void OutputDebugStringEventHandler(const OUTPUT_DEBUG_STRING_INFO& event, const ProcessInfo& processInfo);