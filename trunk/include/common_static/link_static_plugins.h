/*
The MIT License

Copyright (c) 2012 by Frank Richter

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef __ARES_LINK_STATIC_PLUGINS_H__
#define __ARES_LINK_STATIC_PLUGINS_H__

/**\file
 * Helper to ensure linkage against the static plugins commonly used by both
 * ares and aresed. 
 */

#ifdef ARES_BUILD_SHARED_LIBS
  #ifdef ARES_COMMON_STATIC_LIB
    #define ARES_COMMON_STATIC_EXPORT             CS_EXPORT_SYM_DLL
  #else
    #define ARES_COMMON_STATIC_EXPORT             CS_IMPORT_SYM_DLL
  #endif
#else
  #define ARES_COMMON_STATIC_EXPORT
#endif

void ARES_COMMON_STATIC_EXPORT AresLinkCommonStaticPlugins ();

#define ARES_LINK_COMMON_STATIC_PLUGIN                          \
  namespace                                                     \
  {                                                             \
    struct _AresCommonPlugins_                                  \
    {                                                           \
      _AresCommonPlugins_ () { AresLinkCommonStaticPlugins(); } \
    };                                                          \
    _AresCommonPlugins_ _ares_common_plugins_;                  \
  }

#endif // __ARES_LINK_STATIC_PLUGINS_H__
