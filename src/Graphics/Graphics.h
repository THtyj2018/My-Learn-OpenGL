#pragma once

#include "../Utilities/String.h"
#include <cstdint>

#include "../Utilities/Pointer.h"

class Pipeline;

class Graphics final
{
	friend class Application;

public:
	static Graphics* GetSingleton() noexcept;

	void UsePipeline(SharedPtr<Pipeline> pipeline);

	SharedPtr<Pipeline> GetCurrentPipeline() const noexcept;

private:
	int Initialize();

	void Finalize();

	void OnSize(int width, int height);

	void Clear();

	Graphics();

	~Graphics();

	SharedPtr<Pipeline> m_Pipeline;
};