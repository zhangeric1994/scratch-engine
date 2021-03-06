#include "../Rendering/RenderingEngine.h"

#include "GameObject.h"
#include "Light.h"
#include "Scene.h"


ScratchEngine::Light::Light(LightType type) : Light(type, XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f))
{
}

ScratchEngine::Light::Light(LightType type, XMVECTOR color, f32 intensity)
{
	this->type = type;
	this->color = color;
	this->intensity = 1;
	this->shadow = nullptr;

	this->next = nullptr;
	this->previous = nullptr;


	Scene::GetCurrentScene()->AddLight(this);
}

ScratchEngine::Light::~Light()
{
	Scene::GetCurrentScene()->RemoveLight(this);
}


ScratchEngine::DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL)
{
}

ScratchEngine::DirectionalLight::DirectionalLight(XMVECTOR color, f32 intensity) : Light(LightType::DIRECTIONAL, color, intensity)
{
}


ScratchEngine::PointLight::PointLight() : Light(LightType::POINT)
{
}

ScratchEngine::PointLight::PointLight(XMVECTOR color, f32 intensity, f32 range) : Light(LightType::POINT, color, intensity)
{
	this->range = range;
}


ScratchEngine::SpotLight::SpotLight() : Light(LightType::SPOT)
{
	this->angle = 30.0f;
}

ScratchEngine::SpotLight::SpotLight(XMVECTOR color, f32 angle) : Light(LightType::SPOT, color)
{
	this->angle = angle;
}


ScratchEngine::AmbientLight::AmbientLight() : Light(LightType::AMBIENT)
{
}

ScratchEngine::AmbientLight::AmbientLight(XMVECTOR color) : Light(LightType::AMBIENT, color)
{
}

