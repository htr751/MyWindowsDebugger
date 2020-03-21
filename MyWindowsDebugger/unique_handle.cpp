#include"unique_handle.h"
unique_handle::unique_handle(HANDLE other)noexcept :m_handle(other) {}
unique_handle::unique_handle(unique_handle&& other)noexcept {
	this->m_handle = other.m_handle;
	other.m_handle = INVALID_HANDLE_VALUE;
}

unique_handle& unique_handle::operator=(unique_handle&& other) noexcept {
	if (this != &other) {
		if (this->m_handle != INVALID_HANDLE_VALUE)
			CloseHandle(this->m_handle);
		this->m_handle = other.m_handle;
		other.m_handle = INVALID_HANDLE_VALUE;
	}
	return *this;
}

unique_handle::~unique_handle() noexcept {
	if (this->m_handle != INVALID_HANDLE_VALUE)
		CloseHandle(this->m_handle);
}
