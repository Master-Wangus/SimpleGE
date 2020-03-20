
#include "graphics.h"

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
    direct3d = NULL;
    device3d = NULL;
    fullscreen = false;
    width = WIN_WIDTH;    // width & height are replaced in initialize()
    height = WIN_HEIGHT;
    backColor = SETCOLOR_ARGB(255,0,0,128); // dark blue
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
    releaseAll();
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
    SAFE_RELEASE(device3d);
    SAFE_RELEASE(direct3d);
	SAFE_RELEASE(sprite);
}

//=============================================================================
// Initialize DirectX graphics
// throws GameError on error
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
    hwnd = hw;
    width = w;
    height = h;
    fullscreen = full;

    //initialize Direct3D
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3d == NULL)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

    initD3Dpp();        // init D3D presentation parameters
    if(fullscreen)      // if full-screen mode
    {
        if(isAdapterCompatible())   // is the adapter compatible
            // set the refresh rate with a compatible one
            d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
        else
            throw(GameError(gameErrorNS::FATAL_ERROR, 
            "The graphics device does not support the specified resolution and/or format."));
    }

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

    //create Direct3D device
    result = direct3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        behavior,
        &d3dpp, 
        &device3d);

    if (FAILED(result))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));
	result = D3DXCreateSprite(device3d, &sprite);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));
	// Configure for alpha blend of primitives
	device3d->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
    try{
        ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
        // fill in the parameters we need
        d3dpp.BackBufferWidth   = width;
        d3dpp.BackBufferHeight  = height;
        if(fullscreen)                                  // if fullscreen
            d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 bit color
        else
            d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // use desktop setting
        d3dpp.BackBufferCount   = 1;
        d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow     = hwnd;
        d3dpp.Windowed          = (!fullscreen);
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, 
                "Error initializing D3D presentation parameters"));

    }
}
//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
    result = E_FAIL;    // default to fail, replace on success
    // Display backbuffer to screen
    result = device3d->Present(NULL, NULL, NULL, NULL);
    return result;
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::isAdapterCompatible()
{
    UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, 
                                            d3dpp.BackBufferFormat);
    for(UINT i=0; i<modes; i++)
    {
        result = direct3d->EnumAdapterModes( D3DADAPTER_DEFAULT, 
                                        d3dpp.BackBufferFormat,
                                        i, &pMode);
        if( pMode.Height == d3dpp.BackBufferHeight &&
            pMode.Width  == d3dpp.BackBufferWidth &&
            pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
            return true;
    }
    return false;
}

//=============================================================================
// Draw the sprite described in SpriteData structure
// Color is optional, it is applied like a filter, WHITE is default (no change)
// Pre : sprite->Begin() is called
// Post: sprite->End() is called
// spriteData.rect defines the portion of spriteData.texture to draw
//   spriteData.rect.right must be right edge + 1
//   spriteData.rect.bottom must be bottom edge + 1
//=============================================================================
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)      // if no texture
		return;

	// Find center of sprite
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
		(float)(spriteData.height / 2 * spriteData.scale));
	// Screen position of the sprite
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);
	// Scaling X,Y
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	if (spriteData.flipHorizontal)  // if flip horizontal
	{
		scaling.x *= -1;            // negative X scale to flip
		// Get center of flipped image.
		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);
		// Flip occurs around left edge, translate right to put
		// Flipped image in same location as original.
		translate.x += (float)(spriteData.width*spriteData.scale);
	}
	if (spriteData.flipVertical)    // if flip vertical
	{
		scaling.y *= -1;            // negative Y scale to flip
		// Get center of flipped image
		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);
		// Flip occurs around top edge, translate down to put
		// Flipped image in same location as original.
		translate.y += (float)(spriteData.height*spriteData.scale);
	}
	// Create a matrix to rotate, scale and position our sprite
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,                // the matrix
		NULL,                   // keep origin at top left when scaling
		0.0f,                   // no scaling rotation
		&scaling,               // scale amount
		&spriteCenter,          // rotation center
		(float)(spriteData.angle),  // rotation angle
		&translate);            // X,Y location

	// Tell the sprite about the matrix "Hello Neo"
	sprite->SetTransform(&matrix);

	// Draw the sprite
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}
//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::getDeviceState()
{ 
    result = E_FAIL;    // default to fail, replace on success
    if (device3d == NULL)
        return  result;
    result = device3d->TestCooperativeLevel(); 
    return result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::reset()
{
    result = E_FAIL;    // default to fail, replace on success
    initD3Dpp();                        // init D3D presentation parameters
    result = device3d->Reset(&d3dpp);   // attempt to reset graphics device
    return result;
}
//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)      // if already in fullscreen mode
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false) // if already in window mode
				return;
			fullscreen = false; break;
		default:        // default to toggle window/fullscreen
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)  // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else            // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, WIN_WIDTH, WIN_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);   // get size of client area of window
			MoveWindow(hwnd,
				0,                                           // Left
				0,                                           // Top
				WIN_WIDTH + (WIN_WIDTH - clientRect.right),    // Right
				WIN_HEIGHT  + (WIN_HEIGHT- clientRect.bottom), // Bottom
				TRUE);                                       // Repaint the window
		}

	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, WIN_WIDTH, WIN_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			WIN_WIDTH + (WIN_WIDTH - clientRect.right),    // Right
			WIN_HEIGHT + (WIN_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}
}





