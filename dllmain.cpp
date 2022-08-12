


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ͷ�ļ�
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#include "include/detours.h"

#if _WIN64
#pragma comment(lib, "./lib.X64/detours.lib")
#else 
#pragma comment(lib, "./lib.X86/detours.lib")
#endif
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
#pragma comment(linker, "/EXPORT:GetFileVersionInfoA=_AheadLib_GetFileVersionInfoA,@1")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoByHandle=_AheadLib_GetFileVersionInfoByHandle,@2")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExA=_AheadLib_GetFileVersionInfoExA,@3")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoExW=_AheadLib_GetFileVersionInfoExW,@4")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeA=_AheadLib_GetFileVersionInfoSizeA,@5")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExA=_AheadLib_GetFileVersionInfoSizeExA,@6")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeExW=_AheadLib_GetFileVersionInfoSizeExW,@7")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoSizeW=_AheadLib_GetFileVersionInfoSizeW,@8")
#pragma comment(linker, "/EXPORT:GetFileVersionInfoW=_AheadLib_GetFileVersionInfoW,@9")
#pragma comment(linker, "/EXPORT:VerFindFileA=_AheadLib_VerFindFileA,@10")
#pragma comment(linker, "/EXPORT:VerFindFileW=_AheadLib_VerFindFileW,@11")
#pragma comment(linker, "/EXPORT:VerInstallFileA=_AheadLib_VerInstallFileA,@12")
#pragma comment(linker, "/EXPORT:VerInstallFileW=_AheadLib_VerInstallFileW,@13")
#pragma comment(linker, "/EXPORT:VerLanguageNameA=_AheadLib_VerLanguageNameA,@14")
#pragma comment(linker, "/EXPORT:VerLanguageNameW=_AheadLib_VerLanguageNameW,@15")
#pragma comment(linker, "/EXPORT:VerQueryValueA=_AheadLib_VerQueryValueA,@16")
#pragma comment(linker, "/EXPORT:VerQueryValueW=_AheadLib_VerQueryValueW,@17")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ԭ������ַָ��
PVOID pfnGetFileVersionInfoA;
PVOID pfnGetFileVersionInfoByHandle;
PVOID pfnGetFileVersionInfoExA;
PVOID pfnGetFileVersionInfoExW;
PVOID pfnGetFileVersionInfoSizeA;
PVOID pfnGetFileVersionInfoSizeExA;
PVOID pfnGetFileVersionInfoSizeExW;
PVOID pfnGetFileVersionInfoSizeW;
PVOID pfnGetFileVersionInfoW;
PVOID pfnVerFindFileA;
PVOID pfnVerFindFileW;
PVOID pfnVerInstallFileA;
PVOID pfnVerInstallFileW;
PVOID pfnVerLanguageNameA;
PVOID pfnVerLanguageNameW;
PVOID pfnVerQueryValueA;
PVOID pfnVerQueryValueW;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �궨��
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib �����ռ�
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// ԭʼģ����
	DWORD m_dwReturn[17] = { 0 };	// ԭʼ�������ص�ַ


	// ��ȡԭʼ������ַ
	FARPROC WINAPI GetAddress(PCSTR pszProcName, bool dontExit = false)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				wsprintfA(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			if (!dontExit) {
				wsprintf(tzTemp, TEXT("�޷��ҵ����� %hs�������޷��������С�"), pszProcName);
				MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
				ExitProcess(-2);
			}
		}

		return fpAddress;
	}

	// ��ʼ��ԭʼ������ַָ��
	inline VOID WINAPI InitializeAddresses()
	{
		pfnGetFileVersionInfoA = GetAddress("GetFileVersionInfoA");
		pfnGetFileVersionInfoByHandle = GetAddress("GetFileVersionInfoByHandle");
		pfnGetFileVersionInfoExA = GetAddress("GetFileVersionInfoExA", true); //win7��û��
		pfnGetFileVersionInfoExW = GetAddress("GetFileVersionInfoExW");
		pfnGetFileVersionInfoSizeA = GetAddress("GetFileVersionInfoSizeA");
		pfnGetFileVersionInfoSizeExA = GetAddress("GetFileVersionInfoSizeExA", true); //win7��û��
		pfnGetFileVersionInfoSizeExW = GetAddress("GetFileVersionInfoSizeExW");
		pfnGetFileVersionInfoSizeW = GetAddress("GetFileVersionInfoSizeW");
		pfnGetFileVersionInfoW = GetAddress("GetFileVersionInfoW");
		pfnVerFindFileA = GetAddress("VerFindFileA");
		pfnVerFindFileW = GetAddress("VerFindFileW");
		pfnVerInstallFileA = GetAddress("VerInstallFileA");
		pfnVerInstallFileW = GetAddress("VerInstallFileW");
		pfnVerLanguageNameA = GetAddress("VerLanguageNameA");
		pfnVerLanguageNameW = GetAddress("VerLanguageNameW");
		pfnVerQueryValueA = GetAddress("VerQueryValueA");
		pfnVerQueryValueW = GetAddress("VerQueryValueW");

		//GetFileVersionInfoA=FakeGetFileVersionInfoA @1
		//GetFileVersionInfoByHandle = FakeGetFileVersionInfoByHandle @2
		//GetFileVersionInfoExA
		//GetFileVersionInfoExW = FakeGetFileVersionInfoExW @3
		//GetFileVersionInfoSizeA = FakeGetFileVersionInfoSizeA @4
		//GetFileVersionInfoSizeExA
		//GetFileVersionInfoSizeExW = FakeGetFileVersionInfoSizeExW @5
		//GetFileVersionInfoSizeW = FakeGetFileVersionInfoSizeW @6
		//GetFileVersionInfoW = FakeGetFileVersionInfoW @7
		//VerFindFileA = FakeVerFindFileA @8
		//VerFindFileW = FakeVerFindFileW @9
		//VerInstallFileA = FakeVerInstallFileA @10
		//VerInstallFileW = FakeVerInstallFileW @11
		//VerLanguageNameA = FakeVerLanguageNameA @12
		//VerLanguageNameW = FakeVerLanguageNameW @13
		//VerQueryValueA = FakeVerQueryValueA @14
		//VerQueryValueW = FakeVerQueryValueW @15

	}

	// ����ԭʼģ��
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\version.dll"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("�޷����� %s�������޷��������С�"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		}
		else
		{
			InitializeAddresses();
		}

		return (m_hModule != NULL);
	}

	// �ͷ�ԭʼģ��
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}
}
using namespace AheadLib;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

wchar_t _proxy[512];
bool _hasProxy;

static DWORD(WINAPI* pMyGetEnvironmentVariableW)(
	LPCWSTR lpName,
	LPWSTR lpBuffer,
	DWORD nSize
	) = GetEnvironmentVariableW;

DWORD WINAPI MyGetEnvironmentVariableW(
	LPCWSTR lpName,
	LPWSTR lpBuffer,
	DWORD nSize) {

	wchar_t* name = new wchar_t[512];
	swprintf_s(name, 512, L"%s", lpName);
	//OutputDebugStringW(name);

	if (_hasProxy) {
		if (lstrcmpiW(name, L"HTTPS_PROXY") == 0 ||
			lstrcmpiW(name, L"https_proxy") == 0 ||
			lstrcmpiW(name, L"HTTP_PROXY") == 0 ||
			lstrcmpiW(name, L"http_proxy") == 0) {
			wmemcpy(lpBuffer, _proxy, lstrlenW(_proxy));
			delete[] name;
			return lstrlenW(_proxy);
		}
	}

	delete[] name;

	return pMyGetEnvironmentVariableW(lpName, lpBuffer, nSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static BOOL(WINAPI* pMyCreateProcessW)(
	LPCWSTR lpApplicationName,
	LPWSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCWSTR lpCurrentDirectory,
	LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	) = CreateProcessW;

BOOL WINAPI MyCreateProcessW(
	LPCWSTR lpApplicationName,
	LPWSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCWSTR lpCurrentDirectory,
	LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation) {

	auto cmd = std::wstring(lpCommandLine);
	std::string::size_type idx = cmd.find(L"\\Discord.exe");

	if (_hasProxy && idx != std::string::npos) {
		std::string::size_type proxyServerIdx = cmd.find(L"--proxy-server");
		if (proxyServerIdx == std::string::npos) {
			//��Ӳ���
			cmd.append(L" --proxy-server=");
			cmd.append(_proxy);
			return pMyCreateProcessW(lpApplicationName, (LPWSTR)cmd.c_str(), lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);;
		}
	}

	return pMyCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring Utf8toUtf16(const std::string& str) {
	if (str.empty())
		return std::wstring();

	size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), NULL, 0);
	if (charsNeeded == 0)
		throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

	std::vector<wchar_t> buffer(charsNeeded);
	int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
		str.data(), (int)str.size(), &buffer[0], buffer.size());
	if (charsConverted == 0)
		throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

	return std::wstring(&buffer[0], charsConverted);
}

BOOL APIENTRY LoadProxyConfig() {

	auto commandLine = GetCommandLineA();
	auto cmd = std::string(commandLine);
	auto cmdTag = "--proxy-server=";
	std::string::size_type proxyServerIdx = cmd.find(cmdTag);

	if (cmd.find(cmdTag) != std::string::npos) {
		using namespace std;
		//OutputDebugStringA(cmd.c_str());
		std::string regexp_string(".*--proxy-server(={1,2})(.[^\\s]*)(\\s{0,1}|$|.*)"); // Nothing to be escaped here, right?
		std::regex  regexp_to_match(regexp_string);
		smatch matchResult;
		if (std::regex_match(cmd, matchResult, regexp_to_match)) {
			if (matchResult.size() == 4) {
				//OutputDebugStringA(matchResult[2].str().c_str());
				auto wide = Utf8toUtf16(matchResult[2].str());
				wmemcpy(_proxy, wide.c_str(), wide.size());
				_hasProxy = true;
			}
		}
		else {
			//δ�ҵ��ؼ���
		}
		
	} else {

		//��һ�����÷�������ʱ��ʹ��
		const wchar_t* lpPath = L".\\proxy.ini";
		auto result = GetPrivateProfileStringW(L"Config", L"Proxy", L"", _proxy, 512, lpPath);
		_hasProxy = result != 0;
		
		//wchar_t* log = new wchar_t[512];
		//swprintf_s(log, 512, L"��ȡ�������� %d %s", _hasProxy, _proxy);
		//OutputDebugStringW(log);
		//delete[] log;
	}
	return _hasProxy;
}


BOOL APIENTRY InstallHook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach((void**)&pMyGetEnvironmentVariableW, MyGetEnvironmentVariableW);
	DetourAttach((void**)&pMyCreateProcessW, MyCreateProcessW);
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}

BOOL APIENTRY UnInstallHook() {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach((void**)&pMyGetEnvironmentVariableW, MyGetEnvironmentVariableW);
	DetourDetach((void**)&pMyCreateProcessW, MyCreateProcessW);
	LONG ret = DetourTransactionCommit();
	return ret == NO_ERROR;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��ں���
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		bool result = Load();
		LoadProxyConfig();
		InstallHook();
		return result;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoA(void)
{
	__asm JMP pfnGetFileVersionInfoA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoByHandle(void)
{
	__asm JMP pfnGetFileVersionInfoByHandle;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoExA(void)
{
	__asm JMP pfnGetFileVersionInfoExA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoExW(void)
{
	__asm JMP pfnGetFileVersionInfoExW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoSizeA(void)
{
	__asm JMP pfnGetFileVersionInfoSizeA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoSizeExA(void)
{
	__asm JMP pfnGetFileVersionInfoSizeExA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoSizeExW(void)
{
	__asm JMP pfnGetFileVersionInfoSizeExW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoSizeW(void)
{
	__asm JMP pfnGetFileVersionInfoSizeW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_GetFileVersionInfoW(void)
{
	__asm JMP pfnGetFileVersionInfoW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerFindFileA(void)
{
	__asm JMP pfnVerFindFileA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerFindFileW(void)
{
	__asm JMP pfnVerFindFileW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerInstallFileA(void)
{
	__asm JMP pfnVerInstallFileA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerInstallFileW(void)
{
	__asm JMP pfnVerInstallFileW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerLanguageNameA(void)
{
	__asm JMP pfnVerLanguageNameA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerLanguageNameW(void)
{
	__asm JMP pfnVerLanguageNameW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerQueryValueA(void)
{
	__asm JMP pfnVerQueryValueA;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������
ALCDECL AheadLib_VerQueryValueW(void)
{
	__asm JMP pfnVerQueryValueW;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
