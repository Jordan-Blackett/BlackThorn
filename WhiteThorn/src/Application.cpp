#include <BlackThorn.h>

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}