#pragma once

#include "Core.h"

namespace BlackThorn {

	class BLACKTHORN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}