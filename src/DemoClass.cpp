/**
 *
 * @author skitsanos, http://github.com/skitsanos
 * @version 1.0.2
 */

#include <regex>
#include "DemoClass.h"

using namespace Napi;

////https://napi.inspiredware.com/getting-started/objectwrap.html

DemoClass::DemoClass(const CallbackInfo &args) : ObjectWrap(args)
{
    this->_env = args.Env();

    if (args.Length() < 1 || !args[0].IsFunction())
    {
        Napi::TypeError::New(this->_env, "Function expected").ThrowAsJavaScriptException();
    }

    //
    // verify arguments passed to ensure that it was the {require} function that was sent as an ergument
    //
    Napi::Function require = args[0].As<Napi::Function>();

    std::regex self_regex("^function require\\(path\\)",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);

    if (!std::regex_search(require.ToString().Utf8Value().c_str(), self_regex))
    {
        Napi::TypeError::New(this->_env, "{require} Function expected").ThrowAsJavaScriptException();
    }

    this->_require = Persistent(require);
}

Napi::Function DemoClass::GetClass(Napi::Env env)
{
    return DefineClass(env, "DemoClass", {
            DemoClass::InstanceMethod("sayHi",
                                      reinterpret_cast<InstanceMethodCallback>(&DemoClass::sayHi)),

            DemoClass::InstanceMethod("cryptoRandomBytes",
                                      reinterpret_cast<InstanceMethodCallback>(&DemoClass::cryptoRandomBytes)),

            DemoClass::InstanceMethod("stringToBuffer",
                                      reinterpret_cast<InstanceMethodCallback>(&DemoClass::stringToBuffer)),
    });
}

Napi::String DemoClass::sayHi(const CallbackInfo &args)
{
    Napi::Env env = args.Env();

    std::vector<napi_value> jsArgs = {String::New(env, "./hi")};

    Napi::Function f = this->_require.Call(jsArgs).As<Function>();
    auto result = f.Call({}).As<String>().Utf8Value();

    return Napi::String::New(this->_env, result);
}

Napi::Buffer<char> DemoClass::stringToBuffer(const CallbackInfo &args)
{
    if (args.Length() < 1 || !args[0].IsString())
    {
        Napi::TypeError::New(this->_env, "Function expected").ThrowAsJavaScriptException();
    }

    Napi::Env env = args.Env();

    //convert string to Buffer
    auto data = args[0].As<String>().Utf8Value();
    auto buf = Napi::Buffer<char>::Copy(env, data.c_str(), data.size());


    return buf;
}

/**
 * Example on how to call Node.js's crypto.randomBytes() function
 * @param args {CallbackInfo}
 * @return {Napi::Buffer<char>}
 */
Napi::Buffer<char> DemoClass::cryptoRandomBytes(const CallbackInfo &args)
{
    if (args.Length() < 1 || !args[0].IsNumber())
    {
        Napi::TypeError::New(this->_env, "Number expected").ThrowAsJavaScriptException();
    }

    Napi::Env env = args.Env();

    std::vector<napi_value> jsArgs = {String::New(env, "crypto")};

    Napi::Object crypto = this->_require.Call(jsArgs).As<Object>();
    Napi::Function randomBytes = crypto.Get("randomBytes").As<Function>();

    auto result = randomBytes.Call({Napi::Number::New(env, 20)}).As<Buffer<char>>();

    return result;
}





