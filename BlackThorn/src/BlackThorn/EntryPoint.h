#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern BlackThorn::Application* BlackThorn::CreateApplication();

int main(int argc, char** argv)
{
	printf("BlackThorn Engine\n");
	auto app = BlackThorn::CreateApplication();
	app->Run();
	delete app;
}

#endif

