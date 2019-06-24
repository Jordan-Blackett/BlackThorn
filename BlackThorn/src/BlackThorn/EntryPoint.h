#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern BlackThorn::Application* BlackThorn::CreateApplication();

int main(int argc, char** argv)
{
	BlackThorn::Log::Init();
	BT_CORE_WARN("Initialized Log!");
	int a = 5;
	BT_WARN("Hello! {0}", a);

	printf("BlackThorn Engine\n");
	auto app = BlackThorn::CreateApplication();
	app->Run();
	delete app;
}

#endif

