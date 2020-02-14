#pragma once
#include<Windows.h>
#include<exception>
#include<stdexcept>
#include<string>
#include<iostream>
#include<memory>

class FileHandle {
	HANDLE m_handle;
	bool is_valid_handle;

public:
	FileHandle(HANDLE hfile);
	FileHandle(FileHandle&& otherFileHandle) noexcept;

	std::wstring getFullFileName() const;

	~FileHandle() noexcept;
};

void UnitTestingForFileHandle();