#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern BlackThorn::Application* BlackThorn::CreateApplication();

int main(int argc, char** argv)
{
	BlackThorn::Log::Init();

	printf("BlackThorn Engine\n");
	BT_PROFILE_BEGIN_SESSION("Startup", "BlackThornProfile-Startup.json");
	auto app = BlackThorn::CreateApplication();
	BT_PROFILE_END_SESSION();

	BT_PROFILE_BEGIN_SESSION("Runtime", "BlackThornProfile-Runtime.json");
	app->Run();
	BT_PROFILE_END_SESSION();

	BT_PROFILE_BEGIN_SESSION("Startup", "BlackThornProfile-Shutdown.json");
	delete app;
	BT_PROFILE_END_SESSION();
}

#endif

