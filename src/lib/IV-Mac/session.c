// =======================================================================
//
//                      <IV-MAC/session.c>
//
// Session management for an application.  This class is a clearing house
// for the control part of an application.
//
//
// 1.1
// $Date:   4 AUG 1996 
//
/*
 * THIS FILE CONTAINS PORTIONS OF THE InterViews 3.1 DISTRIBUTION THAT 
 * CONTAINED THE FOLLOWING COPYRIGHT:
 * 
 * Copyright (c) 1987, 1988, 1989, 1990, 1991 Stanford University
 * Copyright (c) 1991 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */
// =======================================================================

#include <InterViews/window.h>
#include <InterViews/style.h>
#include <InterViews/session.h>
#include <InterViews/cursor.h>
#include <InterViews/display.h>
#include <InterViews/event.h>
#include <IV-MAC/session.h>
#include <IV-MAC/event.h>
#include <IV-MAC/window.h>
#include <OS/file.h>
#include <OS/string.h>

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <stdlib.h>

extern "C" { void debugfile(const char*, ...);}

//Constant setting the time to yield when no events have been received
#define SLEEP_TIME 10000


//-------------------------------------------------------------------
#ifdef MAC

static PropertyData properties[] = {
{"*gui", "sgimotif"},
{"*PopupWindow*overlay", "true"},
{"*PopupWindow*saveUnder", "on"},
{"*TransientWindow*saveUnder", "on"},
{"*background", "#ffffff"},
{"*brush_width", "0"},
{"*double_buffered", "on"},
{"*flat", "#aaaaaa"},
{"*font", "*Arial*bold*--12*"},
{"*MenuBar*font", "*Arial*bold*--12*"},
{"*MenuItem*font", "*Arial*bold*--12*"},
{"*foreground", "#000000"},
{"*synchronous", "off"},
{"*malloc_debug", "on"},

{"*Scene_background", "#ffffff"},
{"*Scene_foreground", "#000000"},
{"*FieldEditor*background", "#ffffff"},
{"*background", "#cfffff"},
{"*default_brush", "0"},
{"*view_margin", ".25"},
{"*dismiss_button", "on"},
{"*use_transient_windows", "yes"},
{"*nrn_library", " $(NEURONHOME)/lib"},
{"*view_pick_epsilon", "2"},
{"*pwm_canvas_height", "120"},
{"*pwm_paper_height", "11"},
{"*pwm_paper_width", "8.5"},
{"*pwm_paper_resolution", ".5"},
{"*pwm_pixel_resolution", "0"},
{"*window_manager_offset_x", "5."},
{"*window_manager_offset_y", "26."},
{"*pwm_print_file_filter", "*.ps"},
{"*pwm_idraw_file_filter", "*.id"},
{"*pwm_ascii_file_filter", "*"},
{"*pwm_save_file_filter", "*.ses"},
{"*pwm_idraw_prologue", "$(NEURONHOME)/lib/prologue.idraw"},
{"*pwm_postscript_filter", "sed 's;/Adobe-;/;'"},
{"*SlowSlider*autorepeatStart", "0."},
{"*scene_print_border", "1"},
{"*radioScale", ".9"},
{"*xvalue_format", "%.5g"},
{"*graph_axis_default", "0"},
{"*shape_scale_file", "$(NEURONHOME)/lib/shape.cmap2"},
{"*CBWidget_ncolor", "10"},
{"*CBWidget_nbrush", "10"},
	 { nil }
};

#endif

Session* SessionRep::instance_;

// I DO NOT KNOW IF THIS IS NECESSARY ... BUT I DIDN'T WANT TO TAKE IT OUT
#if 0
const char* SECTION_NAME = "InterViews";
	// This is the name of the section in the WIN.INI file where the
	// configuration information should be fetched from. The .ini files
	// are used in Win32 so that things will work with Win32s.

const char* LOCATION_KEY = "location";
	// This is the key used to fetch the value of the location of the
	// top of the configuration directory.  Things like application
	// defaults live at this location.

const char* APPDEF_DEFAULT = SECTION_NAME;
	// Name of the default application defaults file.  This should contain
	// style definitions that are defaults for the entire library.
const char* APPDEF_DEFAULT_ALT = "intervie"; // win3.1 install and using NT

const char* APPDEF_DIRECTORY = "app-defaults";
	// This is the name of the directory where the default style information
	// lives.  In X-Windows this is called app-defaults.  Each application
	// is expected to have a file in this directory to establish reasonable
	// default behavior.
const char* APPDEF_DIR_ALT = "app-defa"; // win3.1 install and using NT

static char* INSTALL_LOCATION = NULL;
	// pathname of the installation location.... determined by reading the
	// win.ini file and querying the location key.
#endif

// -----------------------------------------------------------------------
// Predefined command-line options.
// -----------------------------------------------------------------------
static OptionDesc defoptions[] =
{
	 { "-background", "*background", OptionValueNext },
	 { "-bg", "*background", OptionValueNext },
	 { "-dbuf", "*double_buffered", OptionValueImplicit, "on" },
	 { "-display", "*display", OptionValueNext },
	 { "-dpi", "*dpi", OptionValueNext },
	 { "-fg", "*foreground", OptionValueNext },
	 { "-flat", "*flat", OptionValueNext },
	 { "-fn", "*font", OptionValueNext },
	 { "-font", "*font", OptionValueNext },
	 { "-foreground", "*foreground", OptionValueNext },
	 { "-geometry", "*geometry", OptionValueNext },
	 { "-iconic", "*iconic", OptionValueImplicit, "on" },
	 { "-monochrome", "*gui", OptionValueImplicit, "monochrome" },
	 { "-motif", "*gui", OptionValueImplicit, "Motif" },
	 { "-mswin", "*gui", OptionValueImplicit, "mswin" },
	 { "-name", "*name", OptionValueNext },
	 { "-nodbuf", "*double_buffered", OptionValueImplicit, "off" },
	 { "-noshape", "*shaped_windows", OptionValueImplicit, "off" },
	 { "-openlook", "*gui", OptionValueImplicit, "OpenLook" },
	 { "-reverse", "*reverseVideo", OptionValueImplicit, "on" },
	 { "-rv", "*reverseVideo", OptionValueImplicit, "on" },
	 { "-shape", "*shaped_windows", OptionValueImplicit, "on" },
	 { "-smotif", "*gui", OptionValueImplicit, "SGIMotif" },
	 { "-synchronous", "*synchronous", OptionValueImplicit, "on" },
	 { "+synchronous", "*synchronous", OptionValueImplicit, "off" },
	 { "-title", "*title", OptionValueNext },
	 { "-visual", "*visual", OptionValueNext },
	 { "-visual_id", "*visual_id", OptionValueNext },
	 { "-xrm", nil, OptionPropertyNext },
#ifdef sgi
	 { "-malloc", "*malloc_debug", OptionValueImplicit, "on" },
#endif
	 { nil }
};



// #######################################################################
// #####################  SessionRep class
// #######################################################################

#if USE_SIOUX_WINDOW
extern "C"{
		short InstallConsole(short fd);
}
#endif

#define EmergencyMemorySize 100000
extern "C" {
static char** emergency_memory_;
static pascal long emergency_memory(Size) {
	int a5 = SetCurrentA5();
	long rval = 0;
	if (emergency_memory_) {
		EmptyHandle(emergency_memory_);
		emergency_memory_ = 0;
		rval = EmergencyMemorySize;
		printf("Warning: released emergency memory. Next call here will abort.\n");
	}
	SetA5(a5);
	if (rval == 0) {
		abort();
	}
	return rval;
}
}


SessionRep::SessionRep()
{
	MaxApplZone();						/* take all the memory we can */
	emergency_memory_ = NewHandle(EmergencyMemorySize);
	//SetGrowZone();
	SessionRep::makeChecks();
	SessionRep::initToolboxFunctions();
#if USE_SIOUX_WINDOW
	InstallConsole(0);
#endif
}

SessionRep::~SessionRep()
{
	delete name_;
    Resource::unref(style_);
	delete argv_;
}


Style* iv_display_style_; // see display.cpp

// makeChecks -
//	Make sure that the system is able to handle the inteviews code
// ie. has color QuickDraw, and is System 7.0 or better.
// Checkout EasyApp file init.c line 109
void SessionRep::makeChecks(void){
	OSErr	error;
	long 	systemVersion;
	long 	response;
	
	/* 	get the system version, must be 7 or better	 */
	error = Gestalt(gestaltSystemVersion, &systemVersion);
	if (error){
		printf("Error reading system\n");
		exit(1);
	}
	if (systemVersion < 0x700){ 
		printf("InterViews requires System 7.0 or later\n");
		exit(1);
	}
	
	/* get quickdraw version */
	error = Gestalt(gestaltQuickdrawVersion, &response);
	if (error){
		printf("Error reading system\n");
		exit(1);
	}
	// must be color quick draw
	if(response < gestalt8BitQD){
		printf("InterViews requires Color QuickDraw\n");
		exit(1);
	}
	
	//If drag and drop supported an additional check will need to be made for it.
}


// initToolboxFunctions -
//	Initialize the Macintosh toolboxes for use by the program
void SessionRep::initToolboxFunctions (void)
{
	/* Initialize all the needed managers. */
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();
	FlushEvents(everyEvent,0);
}


void SessionRep::set_style(Display* d)
{
#if 0 //!OCSMALL
	char buf[512];
	sprintf(buf, "%s\\%s\\%s", Session::installLocation(), APPDEF_DIRECTORY, APPDEF_DEFAULT);
	FILE* f;
	if ((f = fopen(buf, "r")) == (FILE*)0) {
		char buf2[512];
		sprintf(buf2, "%s\\%s\\%s", Session::installLocation(), APPDEF_DIR_ALT, APPDEF_DEFAULT_ALT);
		if ((f = fopen(buf2, "r")) == (FILE*)0) {
			char buf3[1024];
			sprintf(buf3, "Can't open InterViews resources file in either\n%s or\n%s",
				buf, buf2);
			//MessageBox(NULL, buf3, "Invalid Installation", MB_OK);
			//abort();
		}
		APPDEF_DIRECTORY = APPDEF_DIR_ALT;
		APPDEF_DEFAULT = APPDEF_DEFAULT_ALT;
	}
	fclose(f);
	 Style* s = new Style(*style_);
	 load_app_defaults(s, APPDEF_DEFAULT, -5);
	 load_props(s, props_, -10);
	 load_app_defaults(s, classname_, -5);
#else
	 Style* s = new Style(*style_);

#ifdef MAC
	load_props(s, properties, -5);
#else
	load_props(s, props_, -5);
#endif

#endif
	iv_display_style_ = s;
	s->ref();
	//d->style(s);
}

void SessionRep::load_app_defaults(Style* s, const char* leafName, int priority)
{
#if 0
	const char* topDir = Session::installLocation();
	if (topDir)
	{
		char subPath[80];
		sprintf(subPath,"/%s/%s", APPDEF_DIRECTORY, leafName);
		load_path(s, topDir, subPath, priority);
	 }
#endif
}

// -----------------------------------------------------------------------
// Property parsing errors
// -----------------------------------------------------------------------
void SessionRep::missing_colon(const String& s)
{
	char buff[135];
	const char* property = s.string();
	 sprintf("Missing colon in property: %s", property);
	//WindowRep::errorMessage(buff);
	// NOT REACHED
}

void SessionRep::bad_property_name(const String& s)
{
	char buff[135];
	const char* property = s.string();
	sprintf("Bad property name: %s", property);
	//WindowRep::errorMessage(buff);
	// NOT REACHED
}

void SessionRep::bad_property_value(const String& s)
{
	char buff[135];
	const char* property = s.string();
	sprintf("Bad property value: %s", property);
	//WindowRep::errorMessage(buff);
	// NOT REACHED
}

// -----------------------------------------------------------------------
// Use ICCCM rules to find an application's instance name from the command
// line or an environment variable.
// -----------------------------------------------------------------------
String* SessionRep::find_name()
{
	 String name;
	if (find_arg(String("-name"), name))
	{
		return new String(name);
    }

	if (argc_ > 0)
	{
		String s(argv_[0]);
		int slash = s.rindex('/');
		if (slash >= 0) {
	    	s = s.right(slash + 1);
		}
		return new String(s);
	 }

	 return new String("noname");
}

// -----------------------------------------------------------------------
// Open the default display and initialize its style information.  For the
// current Mac version, this doesn't do much since we are only considering
// one display.
// -----------------------------------------------------------------------
void SessionRep::init_display()
{
	set_style(nil);
	display_ = Display::open();
	display_->style(iv_display_style_);
	connect(display_);
}

void SessionRep::connect(Display* d)
{
	 set_style(d);
}


// -----------------------------------------------------------------------
// Report that an argument is bad and exit.  A caller of this function
// may assume that it does not return.
//
// We also assume that arg is null-terminated (because it came
// from argv).
// -----------------------------------------------------------------------
void SessionRep::bad_arg(const char* fmt, const String& arg)
{
	char buff[135];
	sprintf(buff, fmt, arg.string());
	//WindowRep::errorMessage(buff);
	 // NOT REACHED
}



// #######################################################################
// #####################  Session (unportable part)
// #######################################################################

// -----------------------------------------------------------------------
// This function is an extension of the InterViews distribution.
// A pathname of location of the installation directory tree is
// returned, that can be used to locate various pieces of configuration
// information (such as application defaults).
//
// This location is found under MS-Windows using the win.ini file (Uses
// the Win16 interface so that Win32s works... which doesn't support the
// registry at this time).
// -----------------------------------------------------------------------
const char* Session::installLocation()
{
	printf("Session::installLocation() not implemented\n");
	return nil;	
}



static Point where;
// Session::read -
//	This function is called by Session::Run when there is an event pending.  Because
// we are generating mouse moved events with every pixel of movement, events should
// be coming fast and furiously.  This style of handling mouse movement was taken from
// the Fresco ... natively, the Macintosh does not consider general mouse movements an
// event.
void Session::read(Event& e)
{	
	RgnHandle region = NewRgn();
	SetRectRgn(region, where.h, where.v, (where.h + 1), (where.v +1));	
	WaitNextEvent(everyEvent, e.rep()->getEventRecord(), SLEEP_TIME, region);
	DisposeRgn(region);
	where = e.rep()->getEventRecord()->where;	
}

// our problem is that pending does not deal with move events.
boolean read_if_pending(Event& e)
{
	RgnHandle region = NewRgn();
	SetRectRgn(region, where.h, where.v, (where.h + 1), (where.v +1));	
	boolean b = WaitNextEvent(everyEvent, e.rep()->getEventRecord(), 0, region);
	DisposeRgn(region);
	if (b) {
		where = e.rep()->getEventRecord()->where;
	}
	return b;	
}

/*
 * Read an event as above, but time out after a given (sec, usec) delay.
 * Return true if an event was read, false if the time-out expired.
 */
boolean Session::read(long, long, Event&)
{
	printf("Session::read - unsupported\n");
	// NOT REACHED
	 return false;
}

/*
 * Check for a pending event, returning it if there is one.
 */

boolean SessionRep::check(Event&)
{
	printf("Session::check - unsupported\n");
	return false;
}

/*
 * Put an event back from whence it came.
 */
void Session::unread(Event&)
{
	printf("Session::unread - unsupported\n");
}

/*
 * Poll an event (implies the event already has an associated display).
 */
void Session::poll(Event&)
{
	printf("Session::poll - unsupported\n");
}

// #######################################################################
// ###############  Session class (portable part)
// #######################################################################

Session::Session(
	const char* classname,
	int& argc,
	char** argv,
	const OptionDesc* opts,
	const PropertyData* initprops)
{
    SessionRep::instance_ = this;
    rep_ = new SessionRep();
    rep_->init(classname, argc, argv, opts, initprops);
}

Session::~Session()
{
    delete rep_;
}

Session* Session::instance()
{
	return SessionRep::instance_;
}

const char* Session::name() const { return rep_->name_->string(); }
const char* Session::classname() const { return rep_->classname_; }
int Session::argc() const { return rep_->argc_; }
char** Session::argv() const { return rep_->argv_; }

Style* Session::style() const
{
	 SessionRep* s = rep_;
	if (s->display_ != nil)
		return s->display_->style(); 
	return s->style_;
}

// -----------------------------------------------------------------------
// These have no equivalent in MS-Windows as there is no network-based
// graphics available.  HASN'T BEEN THOUGHT ABOUT FOR THE MAC
// -----------------------------------------------------------------------
void Session::default_display(Display*)
{
}
Display* Session::default_display() const
{
	return rep_->display_;
}

Display* Session::connect(const String&)
{
	return rep_->display_;
}

Display* Session::connect(const char*)
{
	return rep_->display_;
}
    
void Session::disconnect(Display*)
{
}

#if USE_SIOUX_WINDOW  
//These functions and variables are included for the SIOUX interface which 
//Neuron uses
extern "C" {
	long SIOUXHandleOneEvent(EventRecord *);
	long ReadCharsFromConsole(char *buffer, long n);
	extern WindowPtr SIOUXTextWindow;
	extern Boolean	SIOUXUseWaitNextEvent;
	Boolean IVOCGoodLine;
	char* hoc_console_buffer;		
}
#endif

void Session::screen_update() {
    		while(WindowRep::update_list.count() > 0){
    			WindowRep::update_list.item(0)->repair();
    			WindowRep::update_list.item(0)->update_ = false;
    			WindowRep::update_list.remove(0);
    		}
}

// -----------------------------------------------------------------------
// Event loops
// -----------------------------------------------------------------------
int Session::run()
{
    Event e;
    boolean& done = rep_->done_;
    EventRecord event;
    done = false;
    
#if USE_SIOUX_WINDOW     
   	WindowPtr temp = 0;
   	boolean eventHandled;
   	SIOUXUseWaitNextEvent = false;
   	char buffer[100];
    int n;
#endif
    
    do {

#if USE_SIOUX_WINDOW    	
    	eventHandled = false;
    	IVOCGoodLine = false; //also set to false in SIOUX.c in Read from console
    	
    	//This code was an attempt to compensate for the case that there is only a SIOUX
		//window on the screen.  Haven't really had the opportunity to test it out.  
   		//if(MACwindow::noInterviewsWindows()){
   		//	SIOUXUseWaitNextEvent = true;
   		//	n = ReadCharsFromConsole(*hoc_console_buffer, 256);
		//	if(IVOCGoodLine){
		//		eventHandled = true;
		//		(*hoc_console_buffer)[n] = '\0';
		//		(*hoc_console_buffer)[(n - 1)] = '\n';
		//		return 0;
		//	}
		//}
#endif 	
    	//if there are no events available repair damaged Interviews Windows	
    	if (!(EventAvail(everyEvent, &event))) {
    		screen_update();
		}

#if USE_SIOUX_WINDOW 	
		//check to see if the SIOUX window needs to handle the event
		if(event.what != osEvt){  //avoid checking with the mouse movement calls
			FindWindow(event.where, &temp);
			if(((event.what == updateEvt) || (event.what == activateEvt)) &&
			   ((WindowPtr)(event.message) == SIOUXTextWindow)){ 
			    eventHandled = true;
			    SIOUXUseWaitNextEvent = true;
				SIOUXHandleOneEvent(nil);
				SIOUXUseWaitNextEvent = false;
			} else if((temp == SIOUXTextWindow) &&
			   (FrontWindow() == temp)){
				n = ReadCharsFromConsole(hoc_console_buffer, 256);
				if(IVOCGoodLine){
					eventHandled = true;
					hoc_console_buffer[n] = '\0';
					hoc_console_buffer[(n - 1)] = '\n';
					return 0;
				}
			}
		}
#endif
		
		//let Interviews handle the event if SIOUX did not.
#if USE_SIOUX_WINDOW 
		if(!(eventHandled)){
#endif
			read(e);
			e.handle();

#if USE_SIOUX_WINDOW 
			
		}
#endif
		
	} while (!done);
#if USE_SIOUX_WINDOW //This code was merely for testing with the SIOUX console in
					 //the InterViews project. 
	//Reinstated for ivoc testing
	printf("InterViews has exited");
	SIOUXUseWaitNextEvent = true;
	ReadCharsFromConsole(buffer, 100);
#endif 
    return 0;
}

int Session::run_window(Window* w)
{
    w->map();
    return run();
}

void Session::quit()
{
    rep_->done_ = true;
}
void Session::unquit() {
	rep_->done_ = false;
}

/*
 * Return loop status.
 */

boolean Session::done() const
{
	return rep_->done_;
}

/*
 * Check if an event is pending on any display.
 */
boolean Session::pending() const
{
	EventRecord event;
    return EventAvail(everyEvent, &event);
}

// -----------------------------------------------------------------------
// Initialize.... loading the styles based upon command line arguments
// and the application defaults settings.
// -----------------------------------------------------------------------
void SessionRep::init(
	const char* name,
	int& argc,
	char** argv,
	const OptionDesc* opts,
	const PropertyData* initprops)
{
	done_ = false;
    argc_ = argc;
    argv_ = new char*[argc + 1];
	for (int i = 0; i < argc; i++)
	{
		argv_[i] = argv[i];
    }
    argv_[argc_] = nil;

    init_style(name, initprops);
	if (opts != nil)
	{
		parse_args(argc, argv, opts);
    }
	parse_args(argc, argv, defoptions);
    init_display();

	// ---- build default cursors ----
	Cursor::init();

#ifdef sgi
    if (style_->value_is_on("malloc_debug")) {
	mallopt(M_DEBUG, 1);
    }
#endif
}

/*
 * Parse the argument list, setting any properties that are specified
 * by the option list.  Matching arguments are removed (in-place)
 * from the argument list.
 */
void SessionRep::parse_args(int& argc, char** argv, const OptionDesc* opts)
{
    int i;
    int newargc = 1;
    char* newargv[1024];
    newargv[0] = argv[0];
	for (i = 1; i < argc; i++)
	{
		boolean matched = false;
		String arg(argv[i]);
		for (const OptionDesc* o = &opts[0]; o->name != nil; o++)
		{
			if (match(arg, *o, i, argc, argv))
			{
				matched = true;
				break;
	    	}
		}
		if (!matched)
		{
	    	newargv[newargc] = argv[i];
	    	++newargc;
		}
    }
	if (newargc < argc)
	{
		for (i = 1; i < newargc; i++)
		{
	    	argv[i] = newargv[i];
		}
		argc = newargc;
		argv[argc] = nil;
    }
}

/*
 * See if the given argument matches the option description.
 */

boolean SessionRep::match(
	const String& arg,
	const OptionDesc& o,
	int& i,
	int argc,
	char** argv)
{
    String opt(o.name);
	if (arg != opt)
	{
		if (o.style == OptionValueAfter)
		{
	    	int n = opt.length();
			if (opt == arg.left(n))
			{
				style_->attribute(String(o.path), arg.right(n));
				return true;
	    	}
		}
		return false;
    }
    String name, value;
    extract(arg, o, i, argc, argv, name, value);
    style_->attribute(name, value);
    return true;
}

/*
 * Extract an attribute <name, value> from a given argument.
 */

void SessionRep::extract(
	const String& arg,
	const OptionDesc& o,
	int& i,
	int argc,
	char** argv,
	String& name,
	String& value)
{
    int colon;
	switch (o.style)
	{
    case OptionPropertyNext:
		value = next_arg(i, argc, argv, "missing property after '%s'", arg);
		colon = value.index(':');
		if (colon < 0)
		{
	    	bad_arg("missing ':' in '%s'", value);
		}
		else
		{
	    	name = value.left(colon);
	    	value = value.right(colon+1);
		}
		break;
    case OptionValueNext:
		name = o.path;
		value = next_arg(i, argc, argv, "missing value after '%s'", arg);
		break;
    case OptionValueImplicit:
		name = o.path;
		value = o.value;
		break;
    case OptionValueIsArg:
		name = o.path;
		value = arg;
		break;
    case OptionValueAfter:
		bad_arg("missing value in '%s'", arg);
		break;
    }
}

/*
 * Make sure there is another argument--if not generate an error.
 */

String SessionRep::next_arg(
	int& i,
	int argc,
	char** argv,
	const char* message,
	const String& arg)
{
    ++i;
	if (i == argc)
	{
		bad_arg(message, arg);
    }
    return String(argv[i]);
}

/*
 * Find the value for a specific argument.
 */
boolean SessionRep::find_arg(
	const String& arg,
	String& value)
{
    int last = argc_ - 1;
	for (int i = 1; i < last; i++)
	{
		if (arg == argv_[i])
		{
	    	value = String(argv_[i+1]);
	    	return true;
		}
    }
    return false;
}

/*
 * Initialize style information for the session.
 */
void SessionRep::init_style(
	const char* name,
	const PropertyData* props)
{
    classname_ = name;
    name_ = find_name();
    style_ = new Style(*name_);
    Resource::ref(style_);
    style_->alias(classname_);
    props_ = props;
}

void SessionRep::load_props(
	Style* s,
	const PropertyData* props,
	int priority)
{
	if (props != nil)
	{
		for (const PropertyData* p = &props[0]; p->path != nil; p++)
		{
	    	s->attribute(String(p->path), String(p->value), priority);
		}
    }
}

void SessionRep::load_path(
	Style* s,
	const char* head,
	const char* tail,
	int priority)
{
    String h(head);
    String t(tail);
    char* buff = new char[strlen(head) + strlen(tail) + 1];
    sprintf(buff, "%s%s", head, tail);
    load_file(s, buff, priority);
    delete buff;
}

void SessionRep::load_file(
	Style* s,
	const char* filename,
	int priority)
{
    InputFile* f = InputFile::open(String(filename));
	if (f == nil)
	{
		return;
    }
    const char* start;
    int len = f->read(start);
	if (len > 0)
	{
		load_list(s, String(start, len), priority);
    }
    f->close();
    delete f;
}

void SessionRep::load_list(
	Style* s,
	const String& str,
	int priority)
{
    const char* p = str.string();
    const char* q = p + str.length();
    const char* start = p;
	for (; p < q; p++)
	{
		if (*p == '\n')
		{
			if (p > start && *(p-1) != '\\')
			{
				load_property(s, String(start, (int)(p - start)), priority);
				start = p + 1;
	    	}
		}
    }
}

void SessionRep::load_property(
	Style* s,
	const String& prop,
	int priority)
{
	String p(strip(prop));
	if (p.length() == 0 || p[0] == '!')
	{
		return;
	}
	int colon = p.index(':');
	if (colon < 0)
	{
		missing_colon(p);
	}
	else
	{
		String name(strip(p.left(colon)));
		String value(strip(p.right(colon + 1)));
		if (name.length() <= 0)
		{
			bad_property_name(name);
		}
		else if (value.length() <= 0)
		{
			bad_property_value(value);
		}
		else
		{
			s->attribute(name, value, priority);
		}
	}
}

String SessionRep::strip(const String& s)
{
    int i = 0;
    int len = s.length();
    for (i = 0; i < len && isspace(s[i]); i++);
    int j = len - 1;
    for (; j >= 0 && isspace(s[j]); j--);
    return s.substr(i, j - i + 1);
}

