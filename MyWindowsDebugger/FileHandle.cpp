#include"FileHandle.h"


FileHandle::FileHandle(HANDLE hfile){
	if (this->m_handle == INVALID_HANDLE_VALUE) {
		this->is_valid_handle = false;
		throw std::runtime_error("the passed handle is invalid");
	}
	this->m_handle = hfile;
	this->is_valid_handle = true;
}

FileHandle::FileHandle(FileHandle&& otherFileHandle) noexcept {
	this->m_handle = otherFileHandle.m_handle;
	this->is_valid_handle = true;
	otherFileHandle.is_valid_handle = false;
}

std::wstring FileHandle::getFullFileName() const {
	DWORD nameLength = GetFinalPathNameByHandle(this->m_handle, NULL, NULL, VOLUME_NAME_NT);
	std::unique_ptr<WCHAR[]> containerForNameString = std::make_unique<WCHAR[]>(nameLength);

	GetFinalPathNameByHandle(this->m_handle, containerForNameString.get(), nameLength, VOLUME_NAME_DOS);
	return std::wstring(containerForNameString.get()).substr(4);
}

FileHandle::~FileHandle() noexcept{
	if(this->is_valid_handle)
		CloseHandle(this->m_handle);//the handle is valid so the function will successed
}

void UnitTestingForFileHandle() {
	FileHandle hfile = CreateFile(L"C:\\Users\\htr751\\Documents\\visualStudio2019Kty.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	std::wcout << hfile.getFullFileName() << std::endl;

	FileHandle hfile2 = CreateFile(hfile.getFullFileName().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	std::wcout << hfile2.getFullFileName() << std::endl;
}
