#include <iostream>
#include <napi.h>
#include "Windows.h"
#include <math.h>
#include <time.h>

DWORD sleepDuration = 10;

Napi::Value MouseDelay(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        // Napi::Error::New(env, "Need a parametera!").ThrowAsJavaScriptException();
        return Napi::Number::New(env, sleepDuration);
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Need a number type parameter!").ThrowAsJavaScriptException();
        return env.Null();
    }

    int sleepDur = info[0].As<Napi::Number>().Int32Value();

    if (sleepDur < 0)
    {
        Napi::RangeError::New(env, "Parameter must be larger than 0!").ThrowAsJavaScriptException();
        return env.Null();
    }
    else
    {
        sleepDuration = (unsigned long)sleepDur;
    }
    return Napi::Number::New(env, sleepDuration);
}

Napi::Value MousePos(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    POINT lptr;
    BOOL result = GetCursorPos(&lptr);

    if (result == TRUE)
    {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("x", lptr.x);
        obj.Set("y", lptr.y);
        return obj;
    }
    else
    {
        // Napi::Error::New(env, "Cannot get mouse cursor!").ThrowAsJavaScriptException();
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("x", -1);
        obj.Set("y", -1);
        return obj;
    }
}

Napi::Value MouseMove(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        Napi::Error::New(env, "Need 2 parameters!").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Need 2 number type parameters!").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();

    BOOL result = SetCursorPos(x, y);

    Sleep(sleepDuration);

    if (result)
    {
        return Napi::Boolean::New(env, true);
    }
    else
    {
        return Napi::Boolean::New(env, false);
    }
}

// Napi::Value MouseMoveSmoothly(const Napi::CallbackInfo &info)
// {
//     Napi::Env env = info.Env();

//     POINT lptr;
//     BOOL result = GetCursorPos(&lptr);

//     uint32_t x = info[0].As<Napi::Number>().Uint32Value();
//     uint32_t y = info[1].As<Napi::Number>().Uint32Value();

//     uint32_t disX = x - lptr.x;
//     uint32_t disY = y - lptr.y;

//     uint32_t frame;

//     x > y ? frame = y : frame = x;

//     srand(time(0));

//     for (int i = 0; i < frame; i++)
//     {
//         BOOL result = SetCursorPos(x + disX * i / frame, y + disY * i / frame);
//         if (!result)
//         {
//             return Napi::Boolean::New(env, false);
//         }
//         Sleep(ceil(rand() * 3));
//     }
//     return Napi::Boolean::New(env, true);
// }

void MouseDown(const Napi::CallbackInfo &info)
{
    bool isRight = false;

    if (info.Length() > 0 && info[0].IsBoolean())
    {
        isRight = info[0].As<Napi::Boolean>().Value();
    }

    if (isRight)
    {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    }
    else
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    }

    Sleep(sleepDuration);
}

void MouseUp(const Napi::CallbackInfo &info)
{
    bool isRight = false;

    if (info.Length() > 0 && info[0].IsBoolean())
    {
        isRight = info[0].As<Napi::Boolean>().Value();
    }

    if (isRight)
    {
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }
    else
    {
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    Sleep(sleepDuration);
}

void MousePress(const Napi::CallbackInfo &info)
{
    bool isRight = false;
    DWORD slpdur = 0;

    if (info.Length() > 0 && info[0].IsBoolean())
    {
        isRight = info[0].As<Napi::Boolean>().Value();
    }

    if (info.Length() > 1 && info[1].IsNumber())
    {
        if (info[1].ToNumber().Int32Value() > 0)
        {
            slpdur = info[1].ToNumber().Uint32Value();
        }
    }

    if (isRight)
    {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        if (slpdur > 0)
            Sleep(slpdur);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }
    else
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        if (slpdur > 0)
            Sleep(slpdur);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    // Sleep(sleepDuration);
}

void MouseWheel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    bool isHorizon = false;

    if (info.Length() < 1)
    {
        Napi::Error::New(env, "Need a parametera!").ThrowAsJavaScriptException();
        return;
    }

    if (!info[0].IsNumber())
    {
        Napi::TypeError::New(env, "Need a number type parameter!").ThrowAsJavaScriptException();
        return;
    }

    if (info.Length() > 1 && info[1].IsBoolean())
    {
        isHorizon = info[1].As<Napi::Boolean>().Value();
    }

    int delta = info[0].As<Napi::Number>().Int32Value();

    if (isHorizon)
    {
        mouse_event(MOUSEEVENTF_HWHEEL, 0, 0, delta, 0);
    }
    else
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, delta, 0);
    }

    Sleep(sleepDuration);
}

Napi::String Method(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    return Napi::String::New(env, "Hello.");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "mousePos"), Napi::Function::New(env, MousePos));
    exports.Set(Napi::String::New(env, "mouseMove"), Napi::Function::New(env, MouseMove));
    // exports.Set(Napi::String::New(env, "mouseMoveSmoothly"), Napi::Function::New(env, MouseMoveSmoothly));
    exports.Set(Napi::String::New(env, "mouseDown"), Napi::Function::New(env, MouseDown));
    exports.Set(Napi::String::New(env, "mouseUp"), Napi::Function::New(env, MouseUp));
    exports.Set(Napi::String::New(env, "mousePress"), Napi::Function::New(env, MousePress));
    exports.Set(Napi::String::New(env, "mouseWheel"), Napi::Function::New(env, MouseWheel));
    exports.Set(Napi::String::New(env, "mouseDelay"), Napi::Function::New(env, MouseDelay));
    return exports;
}

NODE_API_MODULE(hidemu, Init);