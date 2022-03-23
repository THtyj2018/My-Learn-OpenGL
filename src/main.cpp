#include "Graphics/Application.h"
#include "Graphics/GfxConfigs.h"

#include "TestPlay.h"

static_assert(_HAS_CXX20, "Should enable C++20 features!");

int main()
{
	Application* app = Application::GetSingleton();

	GfxConfigs config;
	config.Title = "Magic Graphics";
	config.GLMajor = 4;
	config.GLMinor = 4;
	config.GLDebug = true;
	config.FPS = 120;
	config.Controller = new TestPlay;
	app->Config(config);

	return app->Exec();
}