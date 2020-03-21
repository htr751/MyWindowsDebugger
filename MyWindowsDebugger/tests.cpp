//#include<catch.hpp>
//#include"FileHandle.h"
//#include"Utillities.h"
//#include"PE_Parser.h"
//#include"unique_handle.h"
//#include"CommandParser.h"
//
//TEST_CASE("file handle test - check that getFullFileName works", "[file_handle]") {
//	FileHandle hfile = CreateFile(L"C:\\Users\\htr751\\Documents\\visualStudio2019Kty.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	REQUIRE(hfile.getFullFileName() == L"C:\\Users\\htr751\\Documents\\visualStudio2019Kty.txt");
//}
//
//TEST_CASE("check stringTowstring function", "[stringTowstring]") {
//	REQUIRE(stringTowstring("hello world") == L"hello world");
//}
//
//TEST_CASE("check wstringTostring function", "[wstringTostring]") {
//	REQUIRE(wstringTostring(L"hello world") == "hello world");
//}
//
//TEST_CASE("check if emptyQueue really empties the queue", "[Utillities][emptyQueue]") {
//	std::queue<int> queue;
//	queue.push(1);
//	queue.push(2);
//	queue.push(3);
//	queue.push(4);
//	queue.push(5);
//
//	CHECK(queue.size() == 5);
//	emptyQueue(queue);
//	REQUIRE(queue.size() == 0);
//}
//
//TEST_CASE("check if createVectorFromQueue creates vector from queue", "[Utillities][createVectorFromQueue]") {
//	std::queue<int> queue;
//	queue.push(1);
//	queue.push(2);
//	queue.push(3);
//	queue.push(4);
//	queue.push(5);
//
//	CHECK(queue.size() == 5);
//	auto vector = createVectorFromQueue(queue);
//	REQUIRE(vector.size() == 5);
//	REQUIRE(vector[2] == 3);
//}
//
//TEST_CASE("PE_Parser check", "[PE_Parser][GetDosHeader]") {
//	unique_handle processHandle{ GetCurrentProcess() };
//	HMODULE moduleHandle = GetModuleHandleA(NULL);
//	PE_Parser m_parser{ moduleHandle, processHandle.getHandle() };
//
//	SECTION("GetDosHeader check") {
//		auto DosHeader = m_parser.GetDosHeader();
//		REQUIRE(DosHeader.e_magic == IMAGE_DOS_SIGNATURE);
//	}
//	SECTION("GetImageFileHeaders check") {
//		auto imageHeaders = m_parser.GetImageFileHeaders();
//		REQUIRE(imageHeaders.Signature == IMAGE_NT_SIGNATURE);
//	}
//}
//
//TEST_CASE("unique handle check", "[unique_handle]") {
//	unique_handle processHandle{ GetCurrentProcess() };
//	unique_handle movedProcessHandle{ std::move(processHandle) };
//	REQUIRE(processHandle.getHandle() == INVALID_HANDLE_VALUE);
//}
//
//TEST_CASE("checking command lexer - TokenizeCommand method", "[CommandParser][TokenizeCommand]") {
//	auto commandTokenz = TokenizeCommand("breakPoint main:14");
//	REQUIRE(commandTokenz[0] == "breakPoint");
//	REQUIRE(commandTokenz[1] == "main:14");
//}