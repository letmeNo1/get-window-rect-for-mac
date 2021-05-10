#include <napi.h>
#include <iostream>
#include <Carbon/Carbon.h>

#define MAX_DISPLAYS 16
// 定义 Add 函数
Napi::Value getDesktopCapture(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // 接收参数
    std::string ags0 = info[0].As<Napi::String>();
    double ags1 = info[1].As<Napi::Number>().DoubleValue();
    double x = info[2].As<Napi::Number>().DoubleValue();
    double y = info[3].As<Napi::Number>().DoubleValue();
    double w = info[4].As<Napi::Number>().DoubleValue();
    double h = info[5].As<Napi::Number>().DoubleValue();
    std::string screenshotName = info[6].As<Napi::String>();



    std::string baseImageOutput = "file:///"+ags0;
    std::string pathSeparator = "/";
    std::string imageExtension = ".png";
    CGDisplayErr      dErr;
    CGDisplayCount    displayCount, i;
    CGDirectDisplayID mainDisplay;
    CGDisplayCount    maxDisplays = MAX_DISPLAYS;
    CGDirectDisplayID onlineDisplays[MAX_DISPLAYS];
    CGImageRef image;

    dErr = CGGetOnlineDisplayList(maxDisplays, onlineDisplays, &displayCount);
    if (dErr != kCGErrorSuccess) {
            fprintf(stderr, "CGGetOnlineDisplayList: error %d.\n", dErr);
            exit(1);
    }

    std::string imagePath = baseImageOutput + pathSeparator + screenshotName + imageExtension;
    const char *charPath = imagePath.c_str();
    CFStringRef imageOutputPath = CFStringCreateWithCString(kCFAllocatorDefault, charPath, kCFURLPOSIXPathStyle);
    CGDirectDisplayID dID = ags1;
    
    image = CGDisplayCreateImageForRect(dID,CGRectMake(x,y,w,h));
 
   
    CFURLRef url = CFURLCreateWithString(kCFAllocatorDefault, imageOutputPath, NULL);
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, kUTTypePNG, 1, NULL);
    if (!destination) {
      std::cout<< "The destination does not exist: " << imagePath << std::endl;
      CGImageRelease(image);
      }
    CGImageDestinationAddImage(destination, image, NULL);
    if (!CGImageDestinationFinalize(destination)) {
      std::cout << x << std::endl;
      std::cout << y << std::endl;
      std::cout << w << std::endl;
      std::cout << h << std::endl;
      std::cout << "Failed to write image to the path" << std::endl;
      CFRelease(destination);
      CGImageRelease(image);
    }
    CFRelease(destination);
    CGImageRelease(image);    

    Napi::String imagePath2 = Napi::String::New(env, imagePath);
    return imagePath2;
}
// 入口函数，用于注册我们的函数、对象等等
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // 将一个名为 add 的函数挂载到 exports 上
  exports.Set(Napi::String::New(env, "getDesktopCapture"), Napi::Function::New(env, getDesktopCapture));
  return exports;
}

// 固定的宏使用
NODE_API_MODULE(addon, Init)