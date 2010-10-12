// =========================================================================
//
//						<IV-Win/raster.c>
//
// MS-Windows implementation of the InterViews Raster class. 
//
// A memory device context is used to store the raster.  The peek and poke
// operations simply get translated to calls that get and set pixel values
// the the memory device context.
//
// 1.1
// 1997/03/28 17:36:03
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
// =========================================================================
#ifndef ivwin_raster_h
#define ivwin_raster_h

#include <IV-Win/MWlib.h>

class RasterRep
{
public:
	RasterRep(HDC hdc);
	~RasterRep();

	HDC deviceContext() const;

	unsigned long width_;
	unsigned long height_;
		// dimensions of the display surface... a query with GetDeviceCaps
		// returns the incorrect numbers for a memory-based display context.

private:
	HDC hdcMem;
};

inline HDC RasterRep::deviceContext() const
	{ return hdcMem; }

#endif /* ivwin_raster_h */

