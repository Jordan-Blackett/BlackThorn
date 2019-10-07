#pragma once

#include "Core.h"

#include "Window.h"
#include "BlackThorn/LayerStack.h"
#include "Events/Event.h"
#include "BlackThorn/Events/ApplicationEvent.h"

#include "Core/TimeStep.h"

#include "BlackThorn/ImGui/ImGuiLayer.h"

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
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}