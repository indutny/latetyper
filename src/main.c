#include <stdio.h>
#include <stdlib.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>

#include "uv.h"

static int pending;
static uint64_t last;
static CGEventRef keydown;
static CGEventRef keyup;

int main() {
  CGDirectDisplayID display;
  CGDisplayStreamRef stream;
  CFDictionaryRef stream_props;
  CFRunLoopTimerRef timer;

  display = CGMainDisplayID();

  stream_props = NULL;
  stream = CGDisplayStreamCreate(
      display, 1, 1, 'BGRA', stream_props,
      ^(CGDisplayStreamFrameStatus status,
        uint64_t displayTime,
        IOSurfaceRef frameSurface,
        CGDisplayStreamUpdateRef updateRef) {
    size_t rect_count;
    const CGRect* rects;
    uint64_t now;
    float delta;

    if (!pending)
      return;

    rects = CGDisplayStreamUpdateGetRects(updateRef,
                                          kCGDisplayStreamUpdateRefreshedRects,
                                          &rect_count);
    if (rect_count == 0)
      return;

    pending = 0;
    now = uv_hrtime();

    delta = (now - last) / 1000000.0;

    fprintf(stderr, "%f\n", delta);
  });

  CFRunLoopAddSource(CFRunLoopGetCurrent(),
                     CGDisplayStreamGetRunLoopSource(stream),
                     kCFRunLoopDefaultMode);

  if (CGDisplayStreamStart(stream) != kCGErrorSuccess)
    abort();

  /* Create timer */
  timer = CFRunLoopTimerCreateWithHandler(kCFAllocatorDefault, 0, 0.1, 0, 0,
                                          ^(CFRunLoopTimerRef t) {
    CGEventPost(kCGSessionEventTap, keydown);
    CGEventPost(kCGSessionEventTap, keyup);

    pending = 1;
    last = uv_hrtime();
  });
  CFRunLoopAddTimer(CFRunLoopGetCurrent(), timer, kCFRunLoopDefaultMode);

  /* Create key event */
  keydown = CGEventCreateKeyboardEvent(NULL, 6, true);
  keyup = CGEventCreateKeyboardEvent(NULL, 6, false);

  CFRunLoopRun();

  return 0;
}
