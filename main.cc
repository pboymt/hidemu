#include <iostream>
#include <napi.h>
#include "Windows.h"

DWORD sleepDuration = 10;

void MouseDelay(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

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

    int sleepDur = info[0].As<Napi::Number>().Int32Value();

    if (sleepDur < 0)
    {
        Napi::RangeError::New(env, "Parameter must be larger than 0!").ThrowAsJavaScriptException();
        return;
    }
    else
    {
        sleepDuration = (unsigned long)sleepDur;
    }
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

    if (info.Length() > 0 && info[0].IsBoolean())
    {
        isRight = info[0].As<Napi::Boolean>().Value();
    }

    if (isRight)
    {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }
    else
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    Sleep(sleepDuration);
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
    exports.Set(Napi::String::New(env, "mouseDown"), Napi::Function::New(env, MouseDown));
    exports.Set(Napi::String::New(env, "mouseUp"), Napi::Function::New(env, MouseUp));
    exports.Set(Napi::String::New(env, "mousePress"), Napi::Function::New(env, MousePress));
    exports.Set(Napi::String::New(env, "mouseWheel"), Napi::Function::New(env, MouseWheel));
    exports.Set(Napi::String::New(env, "mouseDelay"), Napi::Function::New(env, MouseDelay));
    return exports;
}

NODE_API_MODULE(hidemu, Init);