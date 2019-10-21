#include <BlackThorn.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class NetworkLayer : public BlackThorn::Layer
{
public:
	NetworkLayer()
		: Layer("NetworkLayer")
	{
		// Network
		if (BlackThorn::Network::Initialize())
		{
			BT_CORE_WARN("Initialized Network!");

			m_TCPListenerThread = std::thread(BlackThorn::Network::InitializeTCP, IP, PORT);
			m_UDPThread = std::thread(BlackThorn::Network::InitializeUDP, IP, PORT);
		}
	}

	virtual void OnUpdate(BlackThorn::Timestep ts) override
	{
	}

	virtual void OnImGuiRender() override
	{
	}

private:
	std::thread m_TCPListenerThread;
	std::thread m_UDPThread;
};

class RenderLayer : public BlackThorn::Layer
{
public:
	RenderLayer()
		: Layer("RenderLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(BlackThorn::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<BlackThorn::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(BlackThorn::VertexBuffer::Create(vertices, sizeof(vertices)));
		BlackThorn::BufferLayout layout = {
			{ BlackThorn::ShaderDataType::Float3, "a_Position" },
			{ BlackThorn::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<BlackThorn::IndexBuffer> indexBuffer;
		indexBuffer.reset(BlackThorn::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray.reset(BlackThorn::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<BlackThorn::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(BlackThorn::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({
			{ BlackThorn::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<BlackThorn::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(BlackThorn::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1);
				color = v_Color;
			}
		)";

		m_Shader.reset(BlackThorn::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1);
			}
		)";

		m_flatColorShader.reset(BlackThorn::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	virtual void OnUpdate(BlackThorn::Timestep ts) override
	{
		BT_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		if (BlackThorn::Input::IsKeyPressed(BT_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (BlackThorn::Input::IsKeyPressed(BT_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (BlackThorn::Input::IsKeyPressed(BT_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (BlackThorn::Input::IsKeyPressed(BT_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (BlackThorn::Input::IsKeyPressed(BT_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		if (BlackThorn::Input::IsKeyPressed(BT_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		BlackThorn::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		BlackThorn::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		BlackThorn::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				BlackThorn::Renderer::Submit(m_flatColorShader, m_SquareVertexArray, transform);
			}
		}

		//BlackThorn::Renderer::Submit(m_flatColorShader, m_SquareVertexArray);
		BlackThorn::Renderer::Submit(m_Shader, m_VertexArray);

		BlackThorn::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(BlackThorn::Event& event) override
	{
		//BlackThorn::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<BlackThorn::KeyPressedEvent>(BT_BIND_EVENT_FN(RenderLayer::OnKeyPressedEvent));
	}

	//bool OnKeyPressedEvent(BlackThorn::KeyPressedEvent& event)
	//{
	//	if (event.GetKeyCode() == BT_KEY_LEFT)
	//		m_CameraPosition.x -= m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_RIGHT)
	//		m_CameraPosition.x += m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_UP)
	//		m_CameraPosition.y += m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_DOWN)
	//		m_CameraPosition.y -= m_CameraSpeed;

	//	return false;
	//}

private:
	std::shared_ptr<BlackThorn::Shader> m_Shader;
	std::shared_ptr<BlackThorn::VertexArray> m_VertexArray;

	std::shared_ptr<BlackThorn::Shader> m_flatColorShader;
	std::shared_ptr<BlackThorn::VertexArray> m_SquareVertexArray;

	BlackThorn::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
		PushLayer(new NetworkLayer());
		PushLayer(new RenderLayer());
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}