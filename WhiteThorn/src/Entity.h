#pragma once

#include <BlackThorn.h>

class Entity
{
public:
	Entity();

	void LoadAssets();

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	const glm::vec2& GetPosition() const { return m_Position; }

private:
	glm::vec2 m_Position = { -10.0f, 0.0f };

	BlackThorn::Ref<BlackThorn::Texture2D> m_EntityTexture;
};