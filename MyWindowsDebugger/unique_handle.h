#pragma once
#include<Windows.h>

class unique_handle {
	HANDLE m_handle;
public:
	unique_handle(HANDLE other) noexcept;
	unique_handle(unique_handle&& other) noexcept;
	unique_handle(const unique_handle&) = delete;
	unique_handle& operator=(unique_handle&& other) noexcept;
	unique_handle& operator=(const unique_handle&) = delete;
	HANDLE getHandle() const noexcept {
		return this->m_handle;
	}
	~unique_handle() noexcept;
};