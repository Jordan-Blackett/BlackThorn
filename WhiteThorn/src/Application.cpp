#include <BlackThorn.h>

class ExampleLayer : public BlackThorn::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		BT_INFO("ExampleLayer::Update");
	}

	void OnEvent(BlackThorn::Event& event) override
	{
		BT_TRACE("{0}", event);
	}
};

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
		PushLayer(new ExampleLayer());
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}