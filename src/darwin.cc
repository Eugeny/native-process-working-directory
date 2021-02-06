#include <napi.h>
#include <libproc.h>
#include <string>

Napi::Value getWorkingDirectoryFromPID(const Napi::CallbackInfo& info) {
  auto pid = (int64_t)info[0].ToNumber();
  auto env = info.Env();

  struct proc_vnodepathinfo vpi;
  auto nb = proc_pidinfo(pid, PROC_PIDVNODEPATHINFO, 0, &vpi, sizeof(vpi));
  if (nb <= 0) {
    auto err = errno;
    auto error = Napi::Error::New(env, strerror(err));
    error.Set("errno", Napi::Number::New(env, err));
    error.ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!vpi.pvi_cdir.vip_path[0]) {
    return env.Null();
  }

  return Napi::String::New(env, vpi.pvi_cdir.vip_path);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("getWorkingDirectoryFromPID", Napi::Function::New(env, getWorkingDirectoryFromPID));
  return exports;
}

NODE_API_MODULE(addon, Init)
