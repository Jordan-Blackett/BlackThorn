#include "RenderLayer2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderLayer2D::RenderLayer2D()
	: Layer("RenderLayer2D"), m_CameraController(1280.0f / 720.0f)
{
}


RenderLayer2D::~RenderLayer2D()
{
}

void RenderLayer2D::OnAttach()
{
	BT_PROFILE_FUNCTION();

	m_CheckerboardTexture = BlackThorn::Texture2D::Create("assets/textures/Checkerboard.png");
}

void RenderLayer2D::OnDetach()
{
	BT_PROFILE_FUNCTION();
}

void RenderLayer2D::OnUpdate(BlackThorn::Timestep ts)
{
	BT_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		BT_PROFILE_SCOPE("Renderer Prep");
		BlackThorn::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		BlackThorn::RenderCommand::Clear();
	}

	{
		BT_PROFILE_SCOPE("Renderer Draw");
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
	BT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	ImGui::End();
}

void RenderLayer2D::OnEvent(BlackThorn::Event& e)
{
	m_CameraController.OnEvent(e);
}