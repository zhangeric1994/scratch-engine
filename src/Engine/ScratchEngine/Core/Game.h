#pragma once

#include <DirectXMath.h>
#include <vector>

#include "../Core/GameObject.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Light.h"
#include "../Rendering/Material.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/SimpleShader.h"
#include "../Rendering/Vertex.h"
#include "../Multithreading/Barrier.h"

#include "DXCore.h"

using namespace ScratchEngine::Rendering;
using namespace ScratchEngine::Multithreading;

namespace ScratchEngine
{
	class __declspec(dllexport) Game final : public DXCore
	{
	public:
		Game(HINSTANCE hInstance, char* name);
		~Game();

		void Init();
		void OnResize();
		void Update();
		void Draw();

		void OnMouseDown(WPARAM buttonState, int x, int y);
		void OnMouseUp(WPARAM buttonState, int x, int y);
		void OnMouseMove(WPARAM buttonState, int x, int y);
		void OnMouseWheel(float wheelDelta, int x, int y);

	private:
		void LoadShaders();
		void CreateMatrces();
		void CreateBasicGeometry();

		POINT prevMousePos;

		SimpleVertexShader* vsZPrepass;
		SimpleVertexShader* vertexShader;
		SimplePixelShader* pixelShader;

		ID3D11DepthStencilState* zPrepassDepthStencilState;

		Mesh* sphereMesh;
		Mesh* cubeMesh;

		DirectionalLight* directionalLight;
		GameObject* camera;
		GameObject* go1;
		GameObject* go2;
		GameObject* go4;
		GameObject* go5;
		GameObject* go6;
		GameObject* go7;
		GameObject* go9;
		GameObject* go10;

		Barrier frameBarrier;

		
	public:
		static Material* greenMaterial;
		static Material* redMaterial;
	};
}