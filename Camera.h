#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
#include "ECS.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "graphics.h"
#include "Components.h"
#include "image.h"

using namespace std;
class Camera
{
public:
	int cameraX;
	int cameraY;
	//Constructors and Destructor
	Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors);
	~Camera();

	//Camera Functions
	void Update();
	void ImageUpdate();
	void Follow(Entity *following);
	void FollowImage(Image *following);
	void Unfollow();
	bool IsFollowing() const;
	//set the directX transformation changes to graphic device
	void SetTransform(Graphics *gDevice) const;
	int getCameraX() { return cameraX; };
	int getCameraY() { return cameraY; };
	void setCameraScreen(int w, int h);

private:
	float angle;
	//scalar value of the camera scaling 
	DirectX::XMFLOAT3 scaleFactors;
	//use to project an orthographic view
	D3DXMATRIX orthographicMatrix;
	//
	//D3DXMATRIX identityMatrix;

	D3DXMATRIX viewMatrix;

	Entity *following;
	Image *following1;

	int width;
	int height;
};

#endif /* CAMERA_H */
