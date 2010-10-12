// ===========================================================================
//
//                     <IV-Win/color.cpp>
//
//  MS-Windows implementation of the InterViews Color classes.
//
//
// 1.1
// 1997/03/28 17:36:00
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
// ===========================================================================
#ifndef iv_win_color_h
#define iv_win_color_h

#include <InterViews/iv.h>

#include <IV-Win/MWlib.h>
class MWpalette;
class Bitmap;

class ColorRep
{
public:
	ColorRep(int r, int g, int b);
	~ColorRep();

	COLORREF msColor();						// mswin color representation

	static MWpalette* defaultPalette();		// default palette to use
	float alpha;
	ColorOp op;
	Bitmap* stipple;						// stipple pattern

	static const char* nameToRGB(const char* colormap, const char* name);
		// translates a color name to the X11 string format of an rgb
		// specification (ie #??????).  The colormap name is basically a
		// section in the colormap.ini file.

	static const Color* rgbToColor(const char* name);
		// translates an rgb string (#?????? format) to a color instance,
		// or returns null if there is a problem.  This is for support of 
		// X11 style color names.

private:
	COLORREF color;

};

inline COLORREF ColorRep::msColor()
	{ return color; }

class MWpalette
{
public:
	MWpalette();							// construct an empty palette
    MWpalette(LOGPALETTE*);					// construct from logical palette
	~MWpalette();

	COLORREF addEntry(int r, int g, int b);
		// Add an entry into the palette for the given color rgb specification.
		// If the color already exists, nothing is done, otherwise it is added
		// and a reference is returned.

	boolean findEntry(int r, int g, int b, COLORREF& value);
		// Looks for the given rgb color specification in the palette.  If
		// found, true is returned and the value set to the corresponding 
		// reference.

	int realizeInto(HDC, BOOL);

private:
	class MWcolorTable* table;				// color lookup table
	HPALETTE palette; 						// the MS-Windows palette
	int numEntries;							// number of palette entries
	int palSize;							// size of the palette
};

#endif
