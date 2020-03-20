#include "TwoTowers.h"

//=============================================================================
// Constructor
//=============================================================================
TwoTowers::TwoTowers()
{
	dxFontSmall = new TextDX();     // DirectX fonts
	dxFontMedium = new TextDX();
	dxFontLarge = new TextDX();
	camera = new Camera(WIN_WIDTH, WIN_HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

}
//=============================================================================
// Destructor
//=============================================================================
TwoTowers::~TwoTowers()
{
	releaseAll();           // call onLostDevice() for every graphics item
}



//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void TwoTowers::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError
	//adding healthbar
	if (dxFontSmall->initialize(graphics, 15, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	// 62 pixel high Arial
	if (dxFontMedium->initialize(graphics, 62, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	// 124 pixel high Arial
	if (dxFontLarge->initialize(graphics, 124, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void TwoTowers::update()
{
	
}


//=============================================================================
// Artificial Intelligence
//=============================================================================
void TwoTowers::ai()
{

}

//=============================================================================
// Handle collisions
//=============================================================================
void TwoTowers::collisions()
{

}

//=============================================================================
// Render game items
//=============================================================================
void TwoTowers::render()
{
	graphics->spriteBegin();
	camera->SetTransform(graphics);
	graphics->spriteEnd();                  // end drawing sprites

}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void TwoTowers::releaseAll()
{
	if (camera) { delete camera; camera = nullptr; }

	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void TwoTowers::resetAll()
{
	Game::resetAll();
	return;
}



