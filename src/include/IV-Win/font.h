// =========================================================================
//
//				<IV-Win/font.h>
//
// MS-Windows implementation of the InterViews Font class.  
//
// The font information is stored in a LOGFONT structure until the font is 
// needed, at which time it can be created with CreateFontIndirect().  Some 
// functions require a device context to get information (such as metrics), 
// so the desktop window is used for such requests.  
//
// The user is given a constructor with a LOGFONT structure to use as a
// reference.  This is primarily for flexibility where one doesn't care
// about portability (one really shouldn't use the FontRep directly).  The
// user is cautioned however that there is no guarantee the fields won't
// be overwritten (lfEscapement and lfOrientation may change for example).
//
//
// 1.1
// 1997/03/28 17:36:02
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
// =========================================================================
#ifndef iv_win_font_h
#define iv_win_font_h

// ---- InterViews includes ----
#include <InterViews/iv.h>

// ---- MS-Windows includes ----
#include <IV-Win/MWlib.h>


class FontRep
{
public:
    FontRep(const char* face_name, int height, int sytle_flags = normal);
	FontRep(LOGFONT&);
    ~FontRep();

	// ---- recognized style flags ----
	enum { normal = 0, bold = 1, italic = 2, underline = 4, strikeout = 8 };

	HFONT Create();					// create for drawing... client responsible
	void Scale(float);			    // scale the font
	int Height();					// return font height
	TEXTMETRIC& Metrics();			// return font metric information
	float CharWidth(int ch);			// return width of a character
	boolean AssociateWith(HDC);		// associate with a device for metrics
	const char* TypefaceName();		// name of typeface 

	void orientation(int);
		// Establish the orientation angle to render the font.  The font
		// must be true-type for this function to affect the rendering.  
		// The angle given is in tenths of a degree, 0 to 3600.  This function
		// can be called repeatedly since the actual font is created when
		// needed and then destroyed.

protected:
	boolean CheckAssociation();     // makes sure metric data is available
	HWND fontWindow();				// window to get font information

protected:
	LOGFONT font_;					// font creation template
	TEXTMETRIC metrics;				// metric info for the font
	float* char_widths_;				// character width information

};

// ---- FontRep inline functions ----
inline HFONT FontRep::Create()
{
	return CreateFontIndirect(&font_);
}
inline int FontRep::Height()
{
	return font_.lfHeight;
}
inline TEXTMETRIC& FontRep::Metrics()
{
	CheckAssociation();
	return metrics;
}
inline const char* FontRep::TypefaceName()
	{ return font_.lfFaceName; }
inline void FontRep::orientation(int angle)
	{ font_.lfOrientation = angle; }

class MWfamilyPtrList;

class FontFamilyRep
{
public:
	FontFamilyRep(const char* family);
	~FontFamilyRep();

	boolean font(int size, const char* style,	// translate to font name
		const char*& name, float& scale);

protected:
	static MWfamilyPtrList name_list_;
    char* family_name_;
};


#endif  // iv_win_font_h

