#include"unique_handle.h"
#include<catch.hpp>

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

TEST_CASE("unique handle check", "[unique_handle]") {
	unique_handle processHandle{ GetCurrentProcess() };
	unique_handle movedProcessHandle{ std::move(processHandle) };
	REQUIRE(processHandle.getHandle() == INVALID_HANDLE_VALUE);
}