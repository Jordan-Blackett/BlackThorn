#include "btpch.h"
#include "Application.h"

#include "BlackThorn/Events/ApplicationEvent.h"

namespace BlackThorn {

	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		BT_TRACE(e);

		while (true);
	}

}
