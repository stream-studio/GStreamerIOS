#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "GstreamerBackend.h"
#import "GstreamerIOS-Bridging-Header.h"
#import "gst_ios_init.h"
#import "libstudio.h"

FOUNDATION_EXPORT double GStreamerIOSVersionNumber;
FOUNDATION_EXPORT const unsigned char GStreamerIOSVersionString[];

