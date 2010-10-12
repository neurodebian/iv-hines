// ==========================================================================
//
//                       <IV-Win/brush.h>
//
//
// MS-Windows implementation of the InterViews Brush class
//
// If a programmer that needs full access to the windows capabilities and
// can afford portability problems, this class can be manipulated directly
// to control the pen and brush used in the canvas.  The way to do this is:
//
// 	a) create the Brush instance.
//  b) fill in the BrushRep values desired.
//  c) use the Brush.
//
//
// 1.1
// 1997/03/28 17:35:58
//
// InterViews port to the Windows 3.1/NT operating systems 
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
#ifndef iv_win_brush_h
#define iv_win_brush_h

#include <IV-Win/MWlib.h>

class BrushRep
{
public:
	DWORD* dashList;
    int dashCount;
	Coord penWidth;
};

#endif
