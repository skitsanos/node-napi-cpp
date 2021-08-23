#include <regex>
#include "DemoClass.h"

using namespace Napi;

////https://napi.inspiredware.com/getting-started/objectwrap.html

DemoClass::DemoClass(const CallbackInfo &args) : ObjectWrap(args) {
    this->_env = args.Env();

    if (args.Length() < 1 || !args[0].IsFunction()) {
        Napi::TypeError::New(this->_env, "Function expected").ThrowAsJavaScriptException();
    }

    //
    // verify arguments passed to ensure that it was the {require} function that was sent as an ergument
    //
    Napi::Function require = args[0].As<Napi::Function>();

    std::regex self_regex("^function require\\(path\\)",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);

    if (!std::regex_search(require.ToString().Utf8Value().c_str(), self_regex)) {
        Napi::TypeError::New(this->_env, "{require} Function expected").ThrowAsJavaScriptException();
    }

    this->_require = Persistent(require);
}

Napi::Function DemoClass::GetClass(Napi::Env env) {
    return DefineClass(env, "DemoClass", {
            DemoClass::InstanceMethod("sayHi", reinterpret_cast<InstanceMethodCallback>(&DemoClass::sayHi)),
    });
}

Napi::String DemoClass::sayHi(const CallbackInfo &args) {
    Napi::Env env = args.Env();

    std::vector<napi_value> jsArgs = {String::New(env, "./hi")};

    Napi::Function f = this->_require.Call(jsArgs).As<Function>();
    auto result = f.Call({}).As<String>().Utf8Value();

    return Napi::String::New(this->_env, result);
}





