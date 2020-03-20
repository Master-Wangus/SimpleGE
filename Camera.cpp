#include "Camera.h"

Camera::Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors)
{
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->scaleFactors = scaleFactors;

	D3DXMatrixOrthoLH(&orthographicMatrix, width, -height, 0.0f, 1.0f);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	cameraX = this->width / 2;
	cameraY = this->height / 2;
	if (this->following)
	{
		//insert follwing target here
	}
	//set camera scrolling
	if (cameraX < width /2)
	{
		cameraX = width / 2;
	}
	if (cameraX > GAME_WIDTH - width / 2)
	{
		cameraX = GAME_WIDTH - width /2 ;
	}

	if (cameraY > GAME_HEIGHT - height / 2)
	{
		cameraY = GAME_HEIGHT - height / 2;
	}
	if (cameraY < height / 2)
	{
		cameraY = height / 2;
	}
	this->viewMatrix = D3DXMATRIX(
		scaleFactors.x * cos(angle), scaleFactors.x * sin(angle), 0, 0,
		-scaleFactors.y * sin(angle), scaleFactors.y * cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-cameraX * scaleFactors.x * cos(angle) + cameraY * scaleFactors.y * sin(angle), -cameraX * scaleFactors.y * sin(angle) - cameraY * scaleFactors.y * cos(angle), 0, 1
	);
}
void Camera::ImageUpdate()
{
	cameraX = this->width / 2;
	cameraY = this->height / 2;

	if (this -> following1)
	{
		cameraX = this->following1->getCenterX();
		cameraY = this->following1->getCenterY();
	}
	this->viewMatrix = D3DXMATRIX(
		scaleFactors.x * cos(angle), scaleFactors.x * sin(angle), 0, 0,
		-scaleFactors.y * sin(angle), scaleFactors.y * cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-cameraX * scaleFactors.x * cos(angle) + cameraY * scaleFactors.y * sin(angle), -cameraX * scaleFactors.y * sin(angle) - cameraY * scaleFactors.y * cos(angle), 0, 1
	);
}

void Camera::Follow(Entity* following)
{
	this->following = following;
}
void Camera::FollowImage(Image* following)
{
	this->following1 = following;
}

void Camera::Unfollow()
{
	this->following = nullptr;
}

bool Camera::IsFollowing() const
{
	return this->following != nullptr;
}

void Camera::SetTransform(Graphics* gDevice) const
{
	gDevice->device3d->SetTransform(D3DTS_PROJECTION, &orthographicMatrix);
	gDevice->device3d->SetTransform(D3DTS_VIEW, &viewMatrix);
}
void Camera::setCameraScreen(int w, int h)
{
	width = w;
	height = h;
}






