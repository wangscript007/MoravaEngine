#include "Application.h"


Application* Application::s_Instance = nullptr;

Application* Application::Get()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Application();
		s_Instance->m_Window = nullptr;
		s_Instance->m_Scene = nullptr;
		s_Instance->m_Renderer = nullptr;
	}

	return s_Instance;
}

Window* Application::GetWindow()
{
	return m_Window;
}

Scene* Application::GetScene()
{
	return m_Scene;
}

RendererBasic* Application::GetRenderer()
{
	return m_Renderer;
}

void Application::SetWindow(Window* window)
{
	m_Window = window;
}

void Application::SetScene(Scene* scene)
{
	m_Scene = scene;
}

void Application::SetRenderer(RendererBasic* renderer)
{
	m_Renderer = renderer;
}
