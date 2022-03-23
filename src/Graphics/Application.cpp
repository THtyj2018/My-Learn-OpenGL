#include "Application.h"

#include "Graphics.h"
#include "GfxConfigs.h"
#include "../Math/Mathf.h"
#include "../Control/GamePlay.h"

#include <GLFW/glfw3.h>

#include <chrono>
#include <thread>
#include <cstdio>

Application::Application() :
	m_pConfig(new GfxConfigs), m_pWindow(nullptr), m_bInitialized(false)
{
}

Application::~Application()
{
}

bool Application::Config(const GfxConfigs& config) noexcept
{
	if (m_bInitialized)
		return false;
	*m_pConfig = config;
	return true;
}

const GfxConfigs& Application::GetConfig() const noexcept
{
	return *m_pConfig;
}

bool Application::Running() const noexcept
{
	return glfwWindowShouldClose(m_pWindow) == GLFW_FALSE;
}

void Application::Quit() noexcept
{
	glfwSetWindowShouldClose(m_pWindow, GLFW_TRUE);
}

Application* Application::GetSingleton() noexcept
{
	static Application S_App = Application();
	return &S_App;
}

int Application::Exec()
{
	GamePlay* control = m_pConfig->Controller;
	if (control == nullptr)
	{
		printf("Error: No game controller!\n");
		return -999;
	}

	Graphics* gfx = Graphics::GetSingleton();

	Mathf::srand(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

	int result = Initialize();
	if (result)
	{
		Finalize();
		return result;
	}

	result = gfx->Initialize();
	if (result)
	{
		Graphics::GetSingleton()->Finalize();
		Finalize();
		return result;
	}
	result = control->Initialize();
	if (result != 0)
	{
		printf("Error: User initialise failed!\n");
		Graphics::GetSingleton()->Finalize();
		Finalize();
		return result;
	}

	auto lasttime = std::chrono::steady_clock::now();
	const auto dt = std::chrono::nanoseconds(static_cast<unsigned>(1e9f / m_pConfig->FPS));
	while (Running())
	{
		glfwPollEvents();

		control->Update(dt.count() / 1e9f);

		gfx->Clear();
		control->Draw();
		glfwSwapBuffers(m_pWindow);

		lasttime += dt;
		std::this_thread::sleep_until(lasttime);
	}

	control->Finalize();
	delete control;
	gfx->Finalize();
	Finalize();

	return 0;
}

int Application::Initialize()
{
	int result;

	glfwSetErrorCallback([](int error, const char* desc)->auto{printf("GLFW Error[%d]: %s\n", error, desc); });
	result = glfwInit();
	if (result == GLFW_FALSE)
	{
		printf("Error: GLFW initalize failed!\n");
		return 1;
	}

	int compbits = (m_pConfig->ColorBitsPerChannel >= 16 ? 16 : 8);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_pConfig->GLMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_pConfig->GLMinor);
	glfwWindowHint(GLFW_RED_BITS, compbits);
	glfwWindowHint(GLFW_GREEN_BITS, compbits);
	glfwWindowHint(GLFW_BLUE_BITS, compbits);
	glfwWindowHint(GLFW_ALPHA_BITS, compbits);
	glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(m_pConfig->DepthBits));
	glfwWindowHint(GLFW_SAMPLES, static_cast<int>(m_pConfig->MSAASamples)); // MSAA
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, static_cast<int>(m_pConfig->GLDebug));
	AnsiString title = m_pConfig->Title + " | Press ESC to close the window";
	m_pWindow = glfwCreateWindow(m_pConfig->ScreenWidth, m_pConfig->ScreenHeight, title.c_str(), nullptr, nullptr);

	if (m_pWindow == NULL)
	{
		printf("Error: GLFW create window failed!");
		return 2;
	}

	glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow*)->auto{});
	glfwSetWindowSizeCallback(m_pWindow, WindowSizeCallback);
	// glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow*, double xpos, double ypos)->auto{
		GetSingleton()->GetConfig().Controller->OnMouseMove(float(xpos), float(ypos));
	});
	glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int scancode, int action, int modifier)->auto{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
	});

	glfwMakeContextCurrent(m_pWindow);
	// glfwSwapInterval(1);

	m_bInitialized = true;
	return 0;
}

void Application::Finalize()
{
	if (m_pWindow)
	{
		glfwMakeContextCurrent(NULL);
		glfwDestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
	glfwTerminate();
}

void Application::WindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	Application* app = Application::GetSingleton();
	app->m_pConfig->Controller->OnSize(width, height);
	Graphics::GetSingleton()->OnSize(width, height);
	glfwGetWindowSize(app->m_pWindow, &app->m_pConfig->ScreenWidth, &app->m_pConfig->ScreenHeight);
}