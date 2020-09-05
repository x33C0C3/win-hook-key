#include <locale.h>
#include <stdio.h>
#include <windows.h>

HHOOK hKeyboardHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (0 <= nCode && WM_KEYDOWN == wParam) {
    INPUT input[4];
    switch (((LPKBDLLHOOKSTRUCT)lParam)->vkCode) {
    case 'H':
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = VK_CONTROL;
        input[0].ki.wScan = 0;
        input[0].ki.time = 0;
        input[0].ki.dwFlags = KEYEVENTF_KEYUP;
        input[0].ki.dwExtraInfo = 0;
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = VK_BACK;
        input[1].ki.wScan = 0;
        input[1].ki.time = 0;
        input[1].ki.dwFlags = 0;
        input[1].ki.dwExtraInfo = 0;
        input[2].type = INPUT_KEYBOARD;
        input[2].ki.wVk = VK_BACK;
        input[2].ki.wScan = 0;
        input[2].ki.time = 0;
        input[2].ki.dwFlags = KEYEVENTF_KEYUP;
        input[2].ki.dwExtraInfo = 0;
        input[3].type = INPUT_KEYBOARD;
        input[3].ki.wVk = VK_CONTROL;
        input[3].ki.wScan = 0;
        input[3].ki.time = 0;
        input[3].ki.dwFlags = 0;
        input[3].ki.dwExtraInfo = 0;
        SendInput(4, input, sizeof(INPUT));
        return 1;
      }
      break;
    case 'J':
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = VK_CONTROL;
        input[0].ki.wScan = 0;
        input[0].ki.time = 0;
        input[0].ki.dwFlags = KEYEVENTF_KEYUP;
        input[0].ki.dwExtraInfo = 0;
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = VK_RETURN;
        input[1].ki.wScan = 0;
        input[1].ki.time = 0;
        input[1].ki.dwFlags = 0;
        input[1].ki.dwExtraInfo = 0;
        input[2].type = INPUT_KEYBOARD;
        input[2].ki.wVk = VK_RETURN;
        input[2].ki.wScan = 0;
        input[2].ki.time = 0;
        input[2].ki.dwFlags = KEYEVENTF_KEYUP;
        input[2].ki.dwExtraInfo = 0;
        input[3].type = INPUT_KEYBOARD;
        input[3].ki.wVk = VK_CONTROL;
        input[3].ki.wScan = 0;
        input[3].ki.time = 0;
        input[3].ki.dwFlags = 0;
        input[3].ki.dwExtraInfo = 0;
        SendInput(4, input, sizeof(INPUT));
        return 1;
      }
      break;
    case 'M':
      if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = 0;
        input[0].ki.wScan = L'\r';
        input[0].ki.time = 0;
        input[0].ki.dwFlags = KEYEVENTF_UNICODE;
        input[0].ki.dwExtraInfo = 0;
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = 0;
        input[1].ki.wScan = L'\r';
        input[1].ki.time = 0;
        input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        input[1].ki.dwExtraInfo = 0;
        input[2].type = INPUT_KEYBOARD;
        input[2].ki.wVk = 0;
        input[2].ki.wScan = L'\n';
        input[2].ki.time = 0;
        input[2].ki.dwFlags = KEYEVENTF_UNICODE;
        input[2].ki.dwExtraInfo = 0;
        input[3].type = INPUT_KEYBOARD;
        input[3].ki.wVk = 0;
        input[3].ki.wScan = L'\n';
        input[3].ki.time = 0;
        input[3].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        input[3].ki.dwExtraInfo = 0;
        SendInput(2, input, sizeof(INPUT));
        return 1;
      }
      break;
    }
  }
  return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void PrintErrorW(LPCWSTR s) {
  LPVOID lpMsgBuf;
  DWORD error = GetLastError();
  if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                         FORMAT_MESSAGE_FROM_SYSTEM |
                         FORMAT_MESSAGE_IGNORE_INSERTS,
                     NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     (LPWSTR)&lpMsgBuf, 0, NULL)) {
    fwprintf(stderr, L"%ls: %ls", s, lpMsgBuf);
    LocalFree(lpMsgBuf);
  }
}

DWORD dwMainThreadId;

BOOL CALLBACK HandlerRoutine(DWORD dwCtrlType) {
  if (!PostThreadMessage(dwMainThreadId, WM_QUIT, 0, 0)) {
    PrintErrorW(L"PostThreadMessage");
    return FALSE;
  }
  return TRUE;
};

int wmain(int argc, wchar_t *argv[], wchar_t *envp[]) {
  HINSTANCE hInstance;
  MSG messages;

  setlocale(LC_CTYPE, "");

  hInstance = GetModuleHandleW(NULL);
  if (NULL == hInstance) {
    PrintErrorW(L"GetModuleHandleW");
    return 1;
  }

  dwMainThreadId = GetCurrentThreadId();

  if (!SetConsoleCtrlHandler(&HandlerRoutine, TRUE)) {
    PrintErrorW(L"SetConsoleCtrlHandler");
    return 1;
  }

  hKeyboardHook =
      SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
  if (NULL == hKeyboardHook) {
    PrintErrorW(L"SetWindowsHookEx");
    return 1;
  }

  while (GetMessage(&messages, NULL, 0, 0)) {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
  }

  if (!UnhookWindowsHookEx(hKeyboardHook)) {
    PrintErrorW(L"UnhookWindowsHookEx");
    return 1;
  }

  return 0;
}
