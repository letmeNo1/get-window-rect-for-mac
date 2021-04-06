#include <napi.h>
#include <iostream>
#include <Carbon/Carbon.h>
// 定义 Add 函数
Napi::Value getWindowRect(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // 接收参数
  double arg0 = info[0].As<Napi::Number>().DoubleValue();

  CGWindowID windowID;
  CGRect rect;

  CFArrayRef windowList = CGWindowListCopyWindowInfo(
          kCGWindowListOptionOnScreenOnly, kCGNullWindowID);

  CFIndex numWindows = CFArrayGetCount( windowList );
  double* rectList = new double[4];

  double** windowsRectlist = new double*[(int)numWindows];
  for (int i=0;i<(int)numWindows;i++)
	{
                windowsRectlist[i] = new double[4];
	}

  for( int i = 0; i < (int)numWindows; i++ ) {
        CFDictionaryRef info = (CFDictionaryRef)CFArrayGetValueAtIndex(
                windowList, i);
        //获取窗口ID
        CFStringRef appPID = (CFStringRef)CFDictionaryGetValue(
                info, kCGWindowOwnerPID);
        //appPID
        CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue (info, kCGWindowBounds);
        CGRectMakeWithDictionaryRepresentation(bounds, &rect);
        //讲窗口id转换成CGWindowID类型
        CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(info, kCGWindowNumber),
                             kCGWindowIDCFNumberType,
                             &windowID);
        //这里要再将窗口Id转换成double类型以便跟我们传入的参数进行对比
        if (0 ==arg0) {
                  // 将坐标添加到c++数组中
                windowsRectlist[i][0] = (int) rect.origin.x;
                windowsRectlist[i][1] = (int) rect.origin.y;
                windowsRectlist[i][2] = (int) rect.size.width;
                windowsRectlist[i][3] = (int) rect.size.height;
            }
        else if ((double)windowID==arg0) {
                  // 将坐标添加到c++数组中
                rectList[0] = (int) rect.origin.x;
                rectList[1] = (int) rect.origin.y;
                rectList[2] = (int) rect.size.width;
                rectList[3] = (int) rect.size.height;
                break;
            }
    }
    Napi::Array arr2 = Napi::Array::New(env,(int)numWindows);

    // 将坐标添加到c++数组转换成js能识别的数组
    if (0 ==arg0) {
        for (int j = 0; j < (int)numWindows; j++) {
                Napi::Array arr = Napi::Array::New(env,4);
                for (int i = 0; i < 4; i++) {
                        arr[i] = Napi::Number::New(env, windowsRectlist[j][i]);
        }
        arr2[j]=arr;
    }
    return arr2;
    }else{
        Napi::Array arr = Napi::Array::New(env,4);
         for (int i = 0; i < 4; i++) {
                 arr[i] = Napi::Number::New(env, rectList[i]);
        }
        return arr;
    }

    
}

// 入口函数，用于注册我们的函数、对象等等
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // 将一个名为 add 的函数挂载到 exports 上
  exports.Set(Napi::String::New(env, "getWindowRect"), Napi::Function::New(env, getWindowRect));
  return exports;
}

// 固定的宏使用
NODE_API_MODULE(addon, Init)