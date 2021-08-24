/**
 * Node.js C++ Addons
 * https://nodejs.org/api/addons.html
 *
 * @author skitsanos, http://github.com/skitsanos
 * @version 1.0.2
 */

#include <napi.h>
#include <regex>

#include "iostream"
#include "DemoClass.h"

Napi::String methodHelloWorld(const Napi::CallbackInfo &args)
{
    Napi::Env env = args.Env();
    return Napi::String::New(env, "hello world!");
}

Napi::Number methodWithArguments(const Napi::CallbackInfo &args)
{
    Napi::Env env = args.Env();
    if (args.Length() < 2 || !args[0].IsNumber() || !args[1].IsNumber())
    {
        Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    }

    Napi::Number first = args[0].As<Napi::Number>();
    Napi::Number second = args[1].As<Napi::Number>();

    double result = first.DoubleValue() + second.DoubleValue();
    return Napi::Number::New(env, result);
}

void methodListInternals(const Napi::CallbackInfo &args)
{
    Napi::Env env = args.Env();

    auto global = env.Global().Get("global").As<Napi::Object>();

    auto props = global.GetPropertyNames();

    for (uint32_t i = 0; i < props.Length(); i++)
    {
        std::cout << "\t- " << props.Get(i).ToString().Utf8Value() << "\n";
    }
}

/**
 * Pass require as argument in js:
 * demo.callRequire(require)
 * @param args
 * @return
 */
Napi::String methodAcceptsRequire(const Napi::CallbackInfo &args)
{
    Napi::Env env = args.Env();
    if (args.Length() < 1 || !args[0].IsFunction())
    {
        Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
    }

    Napi::Function cb = args[0].As<Napi::Function>();
    Napi::Value module = cb.Call(env.Global(), {Napi::String::New(env, "./hi")});

    if (module.IsUndefined() || !module.IsFunction())
    {
        Napi::TypeError::New(env, "Module undefined").ThrowAsJavaScriptException();
    }

    Napi::Function func = module.As<Napi::Function>();
    Napi::String result = func.Call(env.Global(), {}).As<Napi::String>();

    return result;
}

Napi::Object CreateObject(const Napi::CallbackInfo &args)
{
    Napi::Env env = args.Env();
    Napi::Object obj = Napi::Object::New(env);

    if (args.Length() < 1 || !args[0].IsFunction())
    {
        Napi::TypeError::New(env, "Function expected").ThrowAsJavaScriptException();
    }

    //
    // verify arguments passed to ensure that it was the {require} function that was sent as an ergument
    //
    Napi::Function require = args[0].As<Napi::Function>();

    std::regex self_regex("^function require\\(path\\)",
                          std::regex_constants::ECMAScript | std::regex_constants::icase);

    if (!std::regex_search(require.ToString().Utf8Value().c_str(), self_regex))
    {
        Napi::TypeError::New(env, "{require} Function expected").ThrowAsJavaScriptException();
    }

    obj.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, methodHelloWorld));

    return obj;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    //static methods within add-on
    exports.Set(Napi::String::New(env, "listInternals"), Napi::Function::New(env, methodListInternals));
    exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, methodHelloWorld));
    exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, methodWithArguments));
    exports.Set(Napi::String::New(env, "callRequire"), Napi::Function::New(env, methodAcceptsRequire));

    //return exports;
    //return Napi::Function::New(env, CreateObject, "addon");

    //new class within add-on that has own methods
    Napi::String name = Napi::String::New(env, "DemoClass");
    exports.Set(name, DemoClass::GetClass(env));
    return exports;
}

NODE_API_MODULE(hello, Init)