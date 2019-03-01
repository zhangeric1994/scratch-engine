#pragma once

#include <DirectXMath.h>

using namespace DirectX;
class Collider 
{
public:
	Collider(XMFLOAT3 position, float radius, float mass, bool _gravity);
	void Update(float dt);
	void ApplyForce(XMFLOAT3 force);
	void BoundPostionWithObj();
	XMFLOAT3 Position;
	XMFLOAT3 Velocity;
	bool UseGravity;
	float Radius;
	float Mass;
	float dampen_time;
	// should add game entity reference in here
};



class SphereCollider : public Collider
{
public:
	SphereCollider(XMFLOAT3 position, XMFLOAT4 color, float radius, float mass);
	~SphereCollider();//this is to keep the rate at which collision energy is lost constant. This isn't a very physically accurate simulation, so we can cheat a little here
	XMFLOAT4 Color;
	
};

