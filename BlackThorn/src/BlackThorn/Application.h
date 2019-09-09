#pragma once

#include "Core.h"

#include "Window.h"
#include "BlackThorn/LayerStack.h"
#include "Events/Event.h"
#include "BlackThorn/Events/ApplicationEvent.h"

#include "BlackThorn/ImGui/ImGuiLayer.h"

#include "BlackThorn/Renderer/Shader.h"
#include "BlackThorn/Renderer/Buffer.h"
#include "BlackThorn/Renderer/VertexArray.h"

namespace BlackThorn {

	class BLACKTHORN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}