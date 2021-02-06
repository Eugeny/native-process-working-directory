#include <napi.h>
#include <string>
#include <windows.h>
#include <winternl.h>


typedef NTSTATUS(NTAPI* _NtQueryInformationProcess)(
  HANDLE ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID ProcessInformation,
  ULONG ProcessInformationLength,
  PULONG ReturnLength
);

Napi::Value throwError(std::string what,  const Napi::Env &env) {
  Napi::Error::New(env, what).ThrowAsJavaScriptException();
  return env.Null();
}

Napi::Value getWorkingDirectory(const HANDLE hProcess, const Napi::Env &env) {
  // By @segrey: https://github.com/rprichard/winpty/pull/159
  HMODULE ntdllHandle = GetModuleHandle(L"ntdll.dll");
  if (!ntdllHandle) {
    return throwError("Cannot get handle to ntdll.dll", env);
  }
  _NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(ntdllHandle, "NtQueryInformationProcess");

  BOOL wow;
  IsWow64Process(GetCurrentProcess(), &wow);
  if (wow) {
    return throwError("Cannot fetch current directory for WoW64 process", env);
  }

  PROCESS_BASIC_INFORMATION pbi;

  NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
  if (!NT_SUCCESS(status)) {
    return throwError("NtQueryInformationProcess failed to fetch ProcessBasicInformation" + std::to_string(status), env);
  }

  PEB peb;
  if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL))
  {
    return throwError("Failed to read PROCESS_BASIC_INFORMATION.PebBaseAddress", env);
  }

  RTL_USER_PROCESS_PARAMETERS procParams;
  if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &procParams, sizeof(procParams), NULL))
  {
    return throwError("Failed to read PEB.ProcessParameters", env);
  }

  // Unfortunately, CurrentDirectory is not declared in _RTL_USER_PROCESS_PARAMETERS, it's somewhere in the Reserved2 area.
  // Use WinDbg, run "dt ntdll!_PEB - r2" command, find ProcessParameters, then find CurrentDirectory: offset 0x38.
  UNICODE_STRING currentDirUnicodeStr = *(UNICODE_STRING*)((PCHAR)&procParams + 0x38);

  if (currentDirUnicodeStr.Length <= 0 || currentDirUnicodeStr.MaximumLength <= 0
    || currentDirUnicodeStr.Length >= currentDirUnicodeStr.MaximumLength || currentDirUnicodeStr.MaximumLength > 8192) {
    return throwError("Bad current directory: Length=" + std::to_string(currentDirUnicodeStr.Length)
      + ", MaximumLength=" + std::to_string(currentDirUnicodeStr.MaximumLength), env);
  }

  LPWSTR lpCurrentDir = new WCHAR[currentDirUnicodeStr.MaximumLength / sizeof(WCHAR) + 1];
  if (!ReadProcessMemory(hProcess, currentDirUnicodeStr.Buffer, lpCurrentDir, currentDirUnicodeStr.MaximumLength, NULL))
  {
    delete[] lpCurrentDir;
    return throwError("Failed to read ProcessParameters.CurrentDirectory", env);
  }

  auto result = Napi::String::New(env, reinterpret_cast<char16_t*>(lpCurrentDir));
  delete[] lpCurrentDir;
  return result;
}

Napi::Value getWorkingDirectoryFromPID(const Napi::CallbackInfo& info) {
  auto pid = (int64_t)info[0].ToNumber();

  HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
  auto result = getWorkingDirectory(h, info.Env());
  CloseHandle(h);

  return result;
}

Napi::Value getWorkingDirectoryFromHandle(const Napi::CallbackInfo& info) {
  auto handle = (int64_t)info[0].ToNumber();
  return getWorkingDirectory((HANDLE)handle, info.Env());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("getWorkingDirectoryFromPID", Napi::Function::New(env, getWorkingDirectoryFromPID));
  exports.Set("getWorkingDirectoryFromHandle", Napi::Function::New(env, getWorkingDirectoryFromHandle));
  return exports;
}

NODE_API_MODULE(addon, Init)
