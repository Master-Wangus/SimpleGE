#include "PhysicsComponent.h"
#include "twotowers.h"


PhysicsComponent::PhysicsComponent() {
	mass = 100.0f;
	inv_mass_ = 1.0f / mass;
}

PhysicsComponent::PhysicsComponent(float mass, bool hasGravity = true) {
	this->mass = mass;
	if (mass == 0) {
		inv_mass_ = 0.0f;
	}
	else {
		inv_mass_ = 1.0f / mass;
	}
}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::init()
{

}
void PhysicsComponent::update(const float& frametime) {
	CalculateVelocity(frametime);
	CalculatePosition(frametime);

}

void PhysicsComponent::draw()
{

}
void PhysicsComponent::Bounce()
// reverse velocity when if entity collide with towers
{

}
void PhysicsComponent::StayWithinScreen()
{

}
void PhysicsComponent::CalculatePosition(const float& frametime) {
	VECTOR2 velocity2Add = velocity * frametime;

	VECTOR2 position = { entity->getComponent<TransformComponent>().position.x, entity->getComponent<TransformComponent>().position.y };
	position += velocity2Add;
	entity->getComponent<TransformComponent>().updatePosition(position.x, position.y);
}

void PhysicsComponent::CalculateVelocity(const float& frametime) {
	velocity += acceleration * frametime;
	if (velocity.x < 0.0f) {
		velocity.x = velocity.x < -max_x_velocity_.x ? -max_x_velocity_.x : velocity.x;
	}
	else {
		velocity.x = velocity.x > max_x_velocity_.y ? max_x_velocity_.y : velocity.x;
	}
	if (velocity.y < 0.0f) {
		velocity.y = velocity.y < -max_y_velocity_.x ? -max_y_velocity_.x : velocity.y;
	}
	else {
		velocity.y = velocity.y > max_y_velocity_.y ? max_y_velocity_.y : velocity.y;
	}
}

void PhysicsComponent::ApplyForce(VECTOR2 force) {
	acceleration += force / mass;
}

void PhysicsComponent::ApplyLinearDrag(const float& frametime)
{
	if (MagnitudeSq(velocity) > 1.0f) {
		velocity = velocity - (Normalized(velocity) * linear_drag_) * frametime;
	}
	else {
		velocity = { 0.0f, 0.0f };
	}
	if (MagnitudeSq(acceleration) > 1.0f) {
		acceleration = acceleration - (Normalized(acceleration) * linear_drag_) * frametime;
	}
	else {
		acceleration = { 0.0f, 0.0f };
	}
}

void PhysicsComponent::SetVelocity(VECTOR2& vel)
{
	velocity = vel;
}

void PhysicsComponent::AddVelocity(const VECTOR2& vel)
{
	velocity += vel;
}

VECTOR2 PhysicsComponent::GetVelocity()
{
	return velocity;
}

float PhysicsComponent::GetMass()
{
	return mass;
}

float PhysicsComponent::GetInvMass()
{
	return inv_mass_;
}

float PhysicsComponent::GetRestitution()
{
	return restitution_;
}

void PhysicsComponent::AddPosition(const VECTOR2& placement)
{
	VECTOR2 original = { entity->getComponent<TransformComponent>().position.x,entity->getComponent<TransformComponent>().position.y };
	VECTOR2 added = original + placement;
	entity->getComponent<TransformComponent>().updatePosition(added.x, added.y);
}

void PhysicsComponent::SetMass(const float& m)
{
	mass = m;
	inv_mass_ = 1 / m;
}
//magnitude calculations
float PhysicsComponent::Magnitude(VECTOR2 v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}
VECTOR2 PhysicsComponent::Normalized(VECTOR2 v)
{
	if (Magnitude(v) == 0.0f) {
		return { 0.0f, 0.0f };
	}
	return (v / Magnitude(v));
}
float PhysicsComponent::MagnitudeSq(VECTOR2 v)
{
	return v.x * v.x + v.y * v.y;
}



