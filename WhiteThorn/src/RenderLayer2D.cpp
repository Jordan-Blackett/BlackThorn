#include "RenderLayer2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

RenderLayer2D::RenderLayer2D()
	: Layer("RenderLayer2D"), m_CameraController(1280.0f / 720.0f)
{
}


RenderLayer2D::~RenderLayer2D()
{
}

void RenderLayer2D::OnAttach()
{
	m_CheckerboardTexture = BlackThorn::Texture2D::Create("assets/textures/Checkerboard.png");
}

void RenderLayer2D::OnDetach()
{
}

void RenderLayer2D::OnUpdate(BlackThorn::Timestep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	// Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		BlackThorn::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		BlackThorn::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		BlackThorn::Renderer2D::BeginScene(m_CameraController.GetCamera());
		BlackThorn::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		BlackThorn::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		BlackThorn::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		BlackThorn::Renderer2D::EndScene();
	}

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void RenderLayer2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	for (auto& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	
	ImGui::End();
}

void RenderLayer2D::OnEvent(BlackThorn::Event& e)
{
	m_CameraController.OnEvent(e);
}