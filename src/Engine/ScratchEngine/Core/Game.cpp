﻿#include <string>

#include "Game.h"

#include "../Core/Global.h"
#include "../Core/Scene.h"
#include "../Physics/PhysicsEngine.h"
#include "../Rendering/RenderingEngine.h"
#include "../Rendering/Mesh.h"

using namespace DirectX;
using namespace ScratchEngine;
using namespace ScratchEngine::Physics;
using namespace ScratchEngine::Rendering;
using namespace ScratchEngine::Animation;


Material* ScratchEngine::Game::greenMaterial = nullptr;
Material* ScratchEngine::Game::redMaterial = nullptr;

ScratchEngine::Game::Game(HINSTANCE hInstance, char* name) : DXCore(hInstance, name, 1280, 720, true), frameBarrier(2)
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	vsZPrepass = nullptr;

	greenMaterial = nullptr;
	redMaterial = nullptr;

	zPrepassDepthStencilState = nullptr;
	
	sphereMesh = nullptr;
	cubeMesh = nullptr;

	Global::SetScreenRatio(1280.0f / 720.0f);

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

ScratchEngine::Game::~Game()
{
	if (vertexShader)
		delete vertexShader;

	if (pixelShader)
		delete pixelShader;
	
	if (vsZPrepass)
		delete vsZPrepass;

	if (greenMaterial)
		delete greenMaterial;

	if (redMaterial)
		delete redMaterial;

	if (zPrepassDepthStencilState)
		zPrepassDepthStencilState->Release();

	delete Scene::GetCurrentScene();

	if (sphereMesh)
		delete sphereMesh;
	
	if (cubeMesh)
		delete cubeMesh;

	RenderingEngine::Stop();
}

void ScratchEngine::Game::Init()
{
	LoadShaders();
	CreateMatrces();
	CreateBasicGeometry();
}

void ScratchEngine::Game::LoadShaders()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	
	std::string spath = std::string(buffer).substr(0, pos).c_str();
	std::wstring wpath = std::wstring(spath.begin(), spath.end());
	std::wstring wVertex = wpath + std::wstring(L"/VertexShader.cso");
	std::wstring wPixel = wpath + std::wstring(L"/PixelShader.cso");
	const wchar_t* vertex = wVertex.c_str();
	const wchar_t* pixel = wPixel.c_str();

	vsZPrepass = new SimpleVertexShader(device, context);
	vsZPrepass->LoadShaderFile((wpath + std::wstring(L"/vs_zprepass.cso")).c_str());

	vsSkeleton = new SimpleVertexShader(device, context);
	vsSkeleton->LoadShaderFile((wpath + std::wstring(L"/vs_skeleton.cso")).c_str());

	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(vertex);

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(pixel);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &zPrepassDepthStencilState);
}

void ScratchEngine::Game::CreateMatrces()
{

}

void ScratchEngine::Game::CreateBasicGeometry()
{
	sphereMesh = new Mesh(device, (char*)"../Assets/Models/sphere.obj");
	cubeMesh = new Mesh(device, (char*)"../Assets/Models/cube.obj");
	model = new Model(device,(const std::string&) "../Assets/Models/001.fbx");

	greenMaterial = new Material(vertexShader, pixelShader, nullptr, nullptr);
	greenMaterial->SetTint(0, 1, 0);

	redMaterial = new Material(vsSkeleton, pixelShader, nullptr, nullptr);
	redMaterial->SetTint(1, 0, 0);


	camera = new GameObject();
	camera->AddComponent<Camera>();


	GameObject* directionalLightObject = new GameObject();
	directionalLightObject->SetRotation(-90, 0, 0);
	directionalLight = directionalLightObject->AddComponent<DirectionalLight>();
	

	go1 = new GameObject();
	go1->AddComponent<Renderer>(redMaterial, model);
	go1->SetPosition(0, 0, 6);
	go1->SetLocalScale(0.01, 0.01, 0.01);
	//go1->AddComponent<BoxCollider>();

	//go2 = new GameObject();
	//go2->SetParent(go1);
	//go2->SetLocalPosition(0, 0, 5);
	//go2->AddComponent<Renderer>(greenMaterial, cubeMesh);

	//GameObject* go3 = new GameObject();
	/*go3->SetParent(go2);
	go3->SetLocalPosition(0, 2, 0);
	go3->AddComponent<Renderer>(greenMaterial, sphereMesh);*/

	/*go4 = new GameObject();
	go4->AddComponent<Renderer>(greenMaterial, cubeMesh);*/
	//go4->AddComponent<BoxCollider>();
}

void ScratchEngine::Game::OnResize()
{
	DXCore::OnResize();

	Global::SetScreenRatio((float)width / height);
}

void ScratchEngine::Game::Update()
{
	model->anim->Update(0.1f);
	while (isRunning)
	{
		UpdateTimer();
		if (titleBarStats)
			UpdateTitleBarStats();

		frameBarrier.Wait();

		if (GetAsyncKeyState(VK_ESCAPE)) Quit();

		if (GetAsyncKeyState('W') & 0x8000)
			camera->Translate(0.0f, 0.0f, deltaTime, SELF);

		if (GetAsyncKeyState('A') & 0x8000)
			camera->Translate(-deltaTime, 0.0f, 0.0f, SELF);

		if (GetAsyncKeyState('S') & 0x8000)
			camera->Translate(0.0f, 0.0f, -deltaTime, SELF);

		if (GetAsyncKeyState('D') & 0x8000)
			camera->Translate(deltaTime, 0.0f, 0.0f, SELF);

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			camera->Translate(0.0f, deltaTime, 0.0f, SELF);

		if (GetAsyncKeyState('X') & 0x8000)
			camera->Translate(0.0f, -deltaTime, 0.0f, SELF);

		//go1->Rotate(20 * deltaTime, 0, 0);
		//go2->Rotate(0, 0, -50 * deltaTime);
		//go4->SetLocalPosition(0, 5 * sin(totalTime), 10);

		PhysicsEngine* physicsEngine = PhysicsEngine::GetSingleton();
		
		physicsEngine->UpdateBoundingVolumes();
		physicsEngine->SolveCollisions();
		frameBarrier.Wait();
	}

	allThreadBarrier.Wait();
}

void ScratchEngine::Game::Draw()
{
	RenderingEngine* renderingEngine = RenderingEngine::GetSingleton();

	while (isRunning)
	{
		renderingEngine->UpdateRenderables();
		renderingEngine->UpdateViewers();
		renderingEngine->UpdateLightSources();
		renderingEngine->SortRenderables();


		frameBarrier.Wait();


		const float color[4] = { 0.7,  0.7, 0.7, 0 };

		context->ClearRenderTargetView(backBufferRTV, color);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		context->OMSetDepthStencilState(nullptr, 0);
		renderingEngine->PerformZPrepass(vsZPrepass, context);

		context->OMSetDepthStencilState(zPrepassDepthStencilState, 0);
		renderingEngine->DrawForward(context);

		swapChain->Present(0, 0);


		frameBarrier.Wait();
	}

	allThreadBarrier.Wait();
}

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void ScratchEngine::Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void ScratchEngine::Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void ScratchEngine::Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
	{
		//camera->SetRotationX((y - prevMousePos.y) * 0.001f);
		//camera->SetRotationY((x - prevMousePos.x) * 0.001f);
	}

	if (buttonState & 0x0002)
		camera->Rotate((y - prevMousePos.y) / 31.41592653579f, (x - prevMousePos.x) / 31.41592653579f, 0.0f);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void ScratchEngine::Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion

