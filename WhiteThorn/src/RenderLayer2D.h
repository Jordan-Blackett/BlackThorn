#pragma once

#include "BlackThorn.h"

class RenderLayer2D : public BlackThorn::Layer
{
public:
	RenderLayer2D();
	~RenderLayer2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(BlackThorn::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BlackThorn::Event& e) override;

private:
	BlackThorn::OrthographicCameraController m_CameraController;

	// Temp
	BlackThorn::Ref<BlackThorn::VertexArray> m_SquareVA;
	BlackThorn::Ref<BlackThorn::Shader> m_FlatColorShader;

	BlackThorn::Ref<BlackThorn::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
