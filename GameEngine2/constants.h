// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 constants.h v1.0

#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr) { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr) { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR SETCOLOR_ARGB(0,255,0,255)	//transparent color(magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
//graphic images
const char NEBULA_IMAGE[] = "pictures\\orion.jpg";
const char TEXTURES_IMAGE[] = "pictures\\textures.png";

// window
const char CLASS_NAME[] = "Spacewar";
const char GAME_TITLE[] = "Spacewar";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels
 
// game
const double PI = 3.14159265;
const float FRAME_RATE  = 120.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 30.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;				//gravitational constant
const float MASS_PLANET = 1.0e14f;
const float MASS_SHIP = 5.0f;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR SHIP_LEFT_KEY = VK_LEFT;	//left arrow
const UCHAR SHIP_RIGHT_KEY = VK_RIGHT;	//right arrow
const UCHAR SHIP_UP_KEY = VK_UP;		//up arrow
const UCHAR SHIP_DOWN_KEY = VK_DOWN;	//down arrow

//weapon types
enum WEAPON { TORPEDO, SHIP, PLANET };

#endif
