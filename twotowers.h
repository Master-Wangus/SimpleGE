#pragma once
#ifndef _TWOTOWERS_H             // prevent multiple definitions if this 
#define _TWOTOWERS_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <fstream>
#include <string> 
#include <mmsystem.h>
#include "game.h"
#include "ECS.h"
#include "textDX.h"
#include "Components.h"
#include "Camera.h"
#include "textureManager.h"


#define PLAYER_RIGHT_KEY VK_RIGHT
#define PLAYER_LEFT_KEY VK_LEFT
#define PLAYER_UP_KEY VK_UP
#define PLAYER_DOWN_KEY VK_DOWN
#define MENU_SPACE_KEY VK_SPACE
#define PAUSE_MENU_KEY VK_ESCAPE
#define DELETE_LETTER_KEY VK_BACK
#define ENTER_KEY VK_RETURN
#define TEST_KEY  VK_CONTROL

//=============================================================================
// Create game class
//=============================================================================
class TwoTowers : public Game
{
private:
  
	TextDX* dxFontSmall;       // DirectX fonts
	TextDX* dxFontMedium;	
	TextDX* dxFontLarge;
public:	
	Camera *camera;

	enum groupLabels : std::size_t
	{
		//insert group tags here
	};
    // Constructor
	TwoTowers();

    // Destructor
    virtual ~TwoTowers();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
