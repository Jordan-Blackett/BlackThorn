#include "btpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace BlackThorn {

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragment)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    BT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragment);
		}

		BT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}