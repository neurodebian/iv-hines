// =========================================================================
//
//						<IV-MAC/raster.c>
//
// MS-Windows implementation of the InterViews Raster class. 
//
// A memory device context is used to store the raster.  The peek and poke
// operations simply get translated to calls that get and set pixel values
// the the memory device context.
//
// 1.1
// 1997/03/28 17:36:17
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

#include <InterViews/raster.h>
#include <InterViews/session.h>
#include <InterViews/display.h>
#include <IV-MAC/raster.h>
#include <IV-MAC/color.h>

Raster::Raster(
	unsigned long width, 				// width of bitmap in pixels
	unsigned long height)				// height of bitmap in pixels
{
	
}

Raster::Raster(const Raster& ras)
{
	
}

Raster::~Raster()
{
	delete rep_;
}


// -------------------------------------------------------------------------
// These function simply query the resolution of the memory device context.
// The width() and height() functions also scale for the resolution of the
// current display in the session.
// -------------------------------------------------------------------------
Coord Raster::width() const
{
	return Coord(0);
}

Coord Raster::height() const
{
	return Coord(0);
}

unsigned long Raster::pwidth() const
{
	return 0;
}

unsigned long Raster::pheight() const
{
	return 0;
}

// -----------------------------------------------------------------------
// should provide reasonable return values.
// -----------------------------------------------------------------------
Coord Raster::left_bearing() const
	{ return Coord(0); }
Coord Raster::right_bearing() const
	{ return Coord(0); }
Coord Raster::ascent() const
	{ return Coord(0); }
Coord Raster::descent() const
	{ return Coord(0); }

// -----------------------------------------------------------------------
// Pixel access.  Note the the orgin of the Raster is the lower left 
// corner, but for the MS-Windows bitmap it is the upper left corner
// so translation is done for pixel access.
// -----------------------------------------------------------------------
void Raster::peek(
	unsigned long x, 					// x coordinate of pixel queried
	unsigned long y,					// y coordinate of pixel queried
	ColorIntensity& red, 
	ColorIntensity& green, 
	ColorIntensity& blue,
	float& alpha) const
{
	// ---- get the pixel value ----
	//COLORREF pixelColor = GetPixel(rep_->deviceContext(), x, rep_->height_-y);

	// --- convert from RGB to intensity ----
	//ColorIntensity scaleFactor(255.0);
	//red = GetRValue(pixelColor) / scaleFactor;
	//green = GetGValue(pixelColor) / scaleFactor;
	//blue = GetBValue(pixelColor) / scaleFactor;
	//alpha = 1.0;
}

void Raster::poke(
	unsigned long x, 
	unsigned long y,
	ColorIntensity red, 
	ColorIntensity green, 
	ColorIntensity blue,
	float alpha)
{
}

void Raster::flush() const
{
	// This is a no-op for the MS-Windows implementation.  
}

// #####################################################################
// ###################### class RasterRep
// #####################################################################

