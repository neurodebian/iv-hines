// =======================================================================
//
//                     <MWlib.h>
//
//  General definitions for the MW library, a library implementing an 
//  object oriented structure on top of MS-Windows.
//
// 1.1
// 1997/03/28 17:35:55
//
// Windows 3.1/NT Port 
// Copyright (c) 1993 Tim Prinzing
//
// This media contains programs and data which are proprietary
// to Tim Prinzing.
//
// These contents are provided under a Tim Prinzing software source
// license, which prohibits their unauthorized resale or distribution 
// outside of the buyer's organization.
// 
// THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
//
// IN NO EVENT SHALL Tim Prinzing BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
// INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
// RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE 
// POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR 
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
// ============================================================================
#ifndef MWlib_h
#define MWlib_h

// ---- MS-Windows includes ----
// since cpp will still read the file even if ifdef'd out, and the windows
// file is quite large, a check for previous inclusion is done outside the
// include directive!
#ifdef WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#ifndef __WINDOWS_H
#include <windows.h>
#endif
#endif

#include <stdlib.h>

void MWinitialize();
void MWcleanup();

// ---- error handling support ----
void mwAssertion(const char* msg, const char* file, unsigned int line);
#ifdef NDEBUG
#define MWassert(test) ((void) 0)
#else
#define MWassert(test) ((void)((test) || (mwAssertion(#test,__FILE__,__LINE__),1)))
#endif

// global new and delete operators
extern void* operator new(size_t size);
extern void operator delete(void* ptr);

#define EXPORT


#include <InterViews/_enter.h>
class Style;

void dumpStyle(Style*, const char* p = "");
	// dumps the given style to the given file in the form that it is
	// normally read.  If the file is null, the style hierarchy is 
	// written out to the appropriate debug stream for the given platform.
	// This function is recursive, and the last argument is the current
	// path built up so far.  If a fixed path is desired to be prepended
	// to each line that is not present in the style, it can be supplied
	// as the path argument to the dumpStyle() call to the root style.

#include <InterViews/_leave.h>

#endif /* MWlib_h */
