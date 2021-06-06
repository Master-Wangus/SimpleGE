#pragma once
#include "Components.h"
#include <d3d9.h>
#include <d3dx9.h>

class Entity;
class PhysicsComponent : public Component {
private:

	VECTOR2 velocity = { 0.0f, 0.0f };
	VECTOR2  max_x_velocity_ = { 200.0f, 200.0f };
	VECTOR2  max_y_velocity_ = { 400.0f, 1000.0f };
	VECTOR2  acceleration = { 0.0f, 0.0f };
	float mass;
	float inv_mass_;
	float restitution_ = 0;
	float linear_drag_ = 10.0f;

	float max_acceleration_ = 10.0f;


	// Update postion by adding velocity to position
	void CalculatePosition(const float& frametime);

	// Update velocity by adding acceeration to velocity
	void CalculateVelocity(const float& frametime);

public:
	// constructor
	PhysicsComponent();
	// constructor with mass & gravity
	PhysicsComponent(float mass, bool hasGravity);
	// destructor
	~PhysicsComponent();

	void init() override;
	void update(const float& frametime) override;
	void draw() override;
	void ApplyForce(VECTOR2 force);
	void ApplyLinearDrag(const float& frametime);
	void SetVelocity(VECTOR2 & vel);
	void AddVelocity(const VECTOR2 & velocity);
	VECTOR2 GetVelocity();
	float GetMass();
	float GetInvMass();
	float GetRestitution();
	void AddPosition(const VECTOR2& placement);
	void Bounce();
	void StayWithinScreen();
	void SetMass(const float& m);
	float Magnitude(VECTOR2 v);
	VECTOR2 Normalized(VECTOR2 v);
	float MagnitudeSq(VECTOR2 v);
};

