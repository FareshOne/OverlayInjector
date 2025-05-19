#pragma once

typedef BOOL(WINAPI* WinHttpWriteDataFn)(HINTERNET, LPCVOID, DWORD, LPDWORD);

extern WinHttpWriteDataFn oWinHttpWriteData;
extern BOOL WINAPI hkWinHttpWriteData(HINTERNET, LPCVOID, DWORD, LPDWORD);
