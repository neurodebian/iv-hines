// ===========================================================================
//
//                            <IV-Win/bitmap.h>
//
// MS-Windows implementation of the InterViews bitmap class.  
//
// The windows data structure BITMAP is used until the bitmap is needed 
// for actual rendering in the canvas, at which time a temporary GDI object 
// is created.  Operations on this bitmap are therefore very light-weight.
//
//
// 1.1
// 1997/03/28 17:35:58
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
#ifndef iv_win_bitmap_h
#define iv_win_bitmap_h

// ---- InterViews includes ----
#include <InterViews/coord.h>
#include <InterViews/iv.h>

// ---- windows includes ----
#include <windows.h>

class BitmapRep
{
public:
	friend class MWcanvas32;
	friend class MWcanvas16;
	friend class CursorRep;

    enum { copy, fliph, flipv, rot90, rot180, rot270, inv };

	BitmapRep();
	BitmapRep(const void*, unsigned int, unsigned int, int, int);
	BitmapRep(const BitmapRep&);
    ~BitmapRep();

	// ---- load from a resource ----
	boolean Load(const char* name);

    // ---- should phase this stuff out ------
	Coord left_;
    Coord bottom_;
    Coord right_;
    Coord top_;
    Coord width_;
	Coord height_;

	// ---- data access functions ----
	virtual void poke(boolean set, int x, int y);		// set or clear a bit
	virtual boolean peek(int x, int y) const;           // test a bit

	unsigned int pwidth()								// width in pixels
		{ return bm_.bmWidth; }
	unsigned int pheight()								// height in pixels
		{ return bm_.bmHeight; }

protected:
	void SyncSize();			// sync size fields to bitmap size

protected:
	// ---- windows bitmap representation ----
	BITMAP bm_;					// logical bitmap representation

	int hot_x_;					// x-coordinate of hot spot (for cursors)
	int hot_y_;					// y-coordinate of hot spot (for cursors)

};


#endif
