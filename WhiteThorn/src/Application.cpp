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
		if (BlackThorn::Input::IsKeyPressed(BT_KEY_TAB))
		{
			BT_TRACE("Tab key is pressed!");
		}
	}

	void OnEvent(BlackThorn::Event& event) override
	{
		if (event.GetEventType() == BlackThorn::EventType::KeyPressed)
		{
			BlackThorn::KeyPressedEvent& e = (BlackThorn::KeyPressedEvent&)event;
			BT_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new BlackThorn::ImGuiLayer());
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}