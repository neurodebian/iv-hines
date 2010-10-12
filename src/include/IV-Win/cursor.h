// =======================================================================
//
//                     <IV-Win/Cursor.h>
//
//  MS-Windows implementation of the InterViews cursor class.  
// 
//  The CursorRep class is basically a small wrapper around an MS-Widows 
//  HCURSOR.  Cursors have a lot of limitations in MS-Windows, so some 
//  differences from the X11 counterpart are unavoidable.  The biggest 
//  difference is the lack of color (or lack documentation to make them color)!
//
//
// 1.1
// 1997/03/28 17:36:01
//
// Windows 3.1/NT InterViews Port 
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
// =======================================================================
#ifndef iv_win_cursor_h
#define iv_win_cursor_h

#include <IV-Win/MWlib.h>


class Color;
class Display;
class WindowVisual;

class CursorRep
{
public:
	CursorRep(int);
	CursorRep(const char*);
	CursorRep(BitmapRep& data, BitmapRep& mask);
	CursorRep(short, short, const int* data, const int* mask);
    ~CursorRep();

	HCURSOR cursorOf()
    	{ return cursor; }

private:
	HCURSOR cursor;

};


#endif
