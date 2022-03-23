#pragma once

#include "../Utilities/Pointer.h"

struct GLFWwindow;

struct GfxConfigs;

class Application final
{
public:
	bool Config(const GfxConfigs& config) noexcept;

	const GfxConfigs& GetConfig() const noexcept;

	bool Running() const noexcept;

	void Quit() noexcept;

	static Application* GetSingleton() noexcept;

	int Exec();

private:
	Application();

	~Application();

	int Initialize();

	void Finalize();

	static void WindowSizeCallback(GLFWwindow* pWindow, int width, int height);

	UniquePtr<GfxConfigs> m_pConfig;
	GLFWwindow* m_pWindow;
	bool m_bInitialized;
};