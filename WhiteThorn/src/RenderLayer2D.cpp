#include "RenderLayer2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"


RenderLayer2D::RenderLayer2D()
	: Layer("RenderLayer2D"), m_CameraController(1280.0f / 720.0f)
{
}


RenderLayer2D::~RenderLayer2D()
{
}

void RenderLayer2D::OnAttach()
{
	m_SquareVA = BlackThorn::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	BlackThorn::Ref<BlackThorn::VertexBuffer> squareVB;
	squareVB.reset(BlackThorn::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ BlackThorn::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	BlackThorn::Ref<BlackThorn::IndexBuffer> squareIB;
	squareIB.reset(BlackThorn::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = BlackThorn::Shader::Create("assets/shaders/FlatColor.glsl");
}

void RenderLayer2D::OnDetach()
{
}

void RenderLayer2D::OnUpdate(BlackThorn::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	BlackThorn::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	BlackThorn::RenderCommand::Clear();

	BlackThorn::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	BlackThorn::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	BlackThorn::Renderer::EndScene();
}

void RenderLayer2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void RenderLayer2D::OnEvent(BlackThorn::Event& e)
{
	m_CameraController.OnEvent(e);
}