#pragma once
#ifndef _DETECTBOX_H               // Prevent multiple definitions if this 
#define _DETECTBOX_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace detectBoxNS
{
	const int   WIDTH = 64;                // image width
	const int   HEIGHT = 64;               // image height
	const int   X = GAME_WIDTH;
	const int   Y = GAME_HEIGHT;
	const int   COLS = 2;       // texture has 2 columns
	const int   START_FRAME = 0;        // starts at frame 1
	const int   END_FRAME = 0;          // no animation
}

class detectBox : public Entity            // inherits from Entity class
{
public:
	// constructor
	detectBox();
};
#endif

