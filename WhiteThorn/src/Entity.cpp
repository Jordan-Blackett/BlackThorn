#include "Entity.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace BlackThorn;

Entity::Entity()
{
}

void Entity::LoadAssets()
{
	m_EntityTexture = Texture2D::Create("assets/sprites/classes/wizard_spritesheet.png");
}

void Entity::OnUpdate(BlackThorn::Timestep ts)
{

}

void Entity::OnRender()
{
	m_EntityTexture->Bind();
	//Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.3f }, glm::radians(GetRotation()), m_EntityTexture);
}

void Entity::OnImGuiRender()
{
}