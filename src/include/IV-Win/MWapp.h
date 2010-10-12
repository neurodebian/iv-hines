// ============================================================================
//
//                    <IV-Win/MWapp.h>
//
// Application class for the Microsoft-Windows layer.  
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
#ifndef MWapp_h
#define MWapp_h

#include <IV-Win/MWlib.h>

class MWapp
{
public:
	MWapp();
	~MWapp();

	int run();
	void quit(int ret = 0);
	void setModule(HINSTANCE, HINSTANCE, const char*, int);
	const char* appName();
	void appName(const char* nm);

public:
	HINSTANCE hinst;					// application module handle
	char scratch[1024];					// static heap for general use

private:
	HINSTANCE pinst;
	const char* cmdLine;                // command line text
	int cmdShow;

};

// ---- inline functions ----
inline void MWapp::quit(int ret)
	{ PostQuitMessage(ret); }

extern MWapp theApp;

#endif /* MWapp_h */
