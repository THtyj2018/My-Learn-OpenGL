#include "Graphics.h"

#include "Application.h"
#include "GfxConfigs.h"
#include "Pipeline.h"

#include <glad/glad.h>

#include <cassert>
#include <cstdio>

static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
	GLenum severity, GLsizei length,
	const char* message, const void* userParam);

Graphics::Graphics() :
	m_Pipeline(nullptr)
{
}

Graphics::~Graphics()
{
}

Graphics* Graphics::GetSingleton() noexcept
{
	static Graphics S_Gfx = Graphics();
	return &S_Gfx;
}

void Graphics::UsePipeline(SharedPtr<Pipeline> pipeline)
{
	m_Pipeline = pipeline;
	GLuint pid = pipeline ? pipeline->GetProgramID() : 0;
	glUseProgram(pid);
}

SharedPtr<Pipeline> Graphics::GetCurrentPipeline() const noexcept
{
	return m_Pipeline;
}

int Graphics::Initialize()
{
	printf("Info: Initializing Graphics module.\n");
	int32_t result = gladLoadGL();
	if (result == GL_FALSE)
	{
		printf("Error: OpenGL load failed!\n");
		return 10;
	}
	else
	{
		printf("Info: OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);
		const GfxConfigs& config = Application::GetSingleton()->GetConfig();

		// Set the depth buffer to be entirely cleared to 1.0 values.
		glClearDepth(1.0f);

		glDisable(GL_DITHER);
		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);
		// Enable MSAA
		glEnable(GL_MULTISAMPLE);

		// Set the polygon winding to front facing for the right handed system.
		glFrontFace(GL_CCW);

		// Enable back face culling.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glViewport(0, 0, config.ScreenWidth, config.ScreenHeight);

		if (GLAD_GL_VERSION_4_3)
		{
			int flags;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				printf("Info: OpenGL Debug context enabled.\n");
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
		}
	}

	return 0;
}

void Graphics::Finalize()
{
	UsePipeline(nullptr);
}

void Graphics::OnSize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Graphics::Clear()
{
	// Set the color to clear the screen to.
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	// Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
	GLenum severity, GLsizei length,
	const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	const char* srcstr = nullptr;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		srcstr = "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		srcstr = "Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		srcstr = "Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		srcstr = "Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		srcstr = "Application";
		break;
	default:
		srcstr = "Other";
		break;
	}

	const char* typestr = nullptr;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		typestr = "Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		typestr = "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		typestr = "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		typestr = "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		typestr = "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		typestr = "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		typestr = "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		typestr = "Pop Group";
		break;
	default:
		typestr = "Other";
		break;
	}

	const char* sevstr = nullptr;
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		sevstr = "high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		sevstr = "medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		sevstr = "low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		sevstr = "notification";
		break;
	default:
		sevstr = "???";
		break;
	}

	printf("glDebugOutput: Message:%s\n---- Source: %s; Type: %s, Severity: %s\n", message, srcstr, typestr, sevstr);
}