//
// Created by EviK on 8/19/2021.
//

#ifndef DEMO_DEMOCLASS_H
#define DEMO_DEMOCLASS_H

#include <napi.h>


class DemoClass : public Napi::ObjectWrap<DemoClass> {
private:
    Napi::FunctionReference _require;
    Napi::Env _env = Napi::Env(nullptr);

public:
    explicit DemoClass(const Napi::CallbackInfo &args);

    Napi::String sayHi(const Napi::CallbackInfo &args);

    static Napi::Function GetClass(Napi::Env);
};


#endif //DEMO_DEMOCLASS_H
