# node-napi-cpp
Example on how to use Node.js N-API in C++

### Dependencies

- *node-cmake* - A CMake-based build system for Node.js native modules
- *node-addon-api* - This module contains header-only C++ wrapper classes which simplify the use of the C based Node-API provided by Node.js when using C++. It provides a C++ object model and exception handling semantics with low overhead
- *cmake-js* - CMake.js is a Node.js/io.js native addon build tool which works (almost) exactly like node-gyp, but instead of gyp, it is based on CMake build system.

Install dependencies before doing anything:

```sh
yarn add node-cmake node-addon-api cmake-js
```

### CMakeLists.txt

Tested on Node.js 12, 14 and 16, configuration made in a way that it usses `node-cmake` and `node-addon-api` bits installed earlier, so all your headers are in place and no additional configuration required. Well, just change your project name instead `demo` into something you like more at line #14.

### Calling JS module from C++

`DemoClass.cpp` demonstrates how to call `require` from C++. There is one thing about it that not really documented on Node.js references and manuals -- `require` is not a part of Node.js internals and available only at run time, so you can't call it directly in any way within your C++ add-on, but you can pass it to your add-on.

```js
const addon = require('../build/Release/demo');
const demo = new addon.DemoClass(require);
```

So let's say you have a JavaScript module `hi.js` that will be called in DemoClass:

```js
const welcome = () => 'Welcome back!';

module.exports = welcome;
```

In DemoClass constructor we pass `require` and we keep it for the future use:

```cpp
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
```

Since we know the `require` function signature we can check if it is the right one with a basic regular expression... Now we can build a method that will consume that `require`. So, literally, we tell C++ to perform `require('./hi')` that will return a fucntion and then call that function.


```cpp
Napi::String DemoClass::sayHi(const CallbackInfo &args) {
    Napi::Env env = args.Env();

    std::vector<napi_value> jsArgs = {String::New(env, "./hi")};

    Napi::Function f = this->_require.Call(jsArgs).As<Function>();
    auto result = f.Call({}).As<String>().Utf8Value();

    return Napi::String::New(this->_env, result);
}
```

So now, when we call this methods from Javascript:

```js
console.log(demo.sayHi());
```

It will print out our 'Welcome back!' message
