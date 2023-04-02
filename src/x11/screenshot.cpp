#include "screenshot.hpp"

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <cassert>

namespace squint::x11 {
  XHandle<XImage> takeScreenshot(const squint::x11::DisplayHandle& display) {
    Window rootWindow = XDefaultRootWindow(display.get());
    assert(rootWindow);

    XWindowAttributes rootAttribs;
    XGetWindowAttributes(display.get(), rootWindow, &rootAttribs);
    XRenderPictFormat* sourceFormat = XRenderFindVisualFormat(display.get(), rootAttribs.visual);
    XRenderPictureAttributes sourceAttribs;
    sourceAttribs.subwindow_mode = IncludeInferiors; // Not entirely sure what this does

    Picture source = XRenderCreatePicture(display.get(), rootWindow, sourceFormat, CPSubwindowMode, &sourceAttribs);
    assert(source);

    Pixmap pixmap = XCreatePixmap(display.get(), rootWindow, rootAttribs.width, rootAttribs.height, 32);
    assert(pixmap);

    // TODO: check if destinationFormat matches what i3 expects
    XRenderPictFormat* destinationFormat = XRenderFindStandardFormat(display.get(), PictStandardARGB32);
    XRenderPictureAttributes destinationAttribs;
    Picture destination = XRenderCreatePicture(display.get(), pixmap, destinationFormat, 0, &destinationAttribs);
    assert(destination);

    XRenderColor black { 0x0000, 0x0000, 0x0000, 0x0000 };
    XRenderFillRectangle(display.get(), PictOpSrc, destination, &black, 0, 0, rootAttribs.width, rootAttribs.height);
    XRenderComposite(display.get(), PictOpSrc, source, 0, destination, 0, 0, 0, 0, 0, 0, rootAttribs.width, rootAttribs.height);

    XImage* output = XGetImage(display.get(), pixmap, 0, 0, rootAttribs.width, rootAttribs.height, AllPlanes, ZPixmap);
    assert(output);
    return XHandle<XImage>{output};
  }
}
