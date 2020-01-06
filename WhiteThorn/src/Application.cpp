#include <BlackThorn.h>
#include <BlackThorn/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderLayer2D.h"

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
		PushLayer(new RenderLayer2D());
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}