#pragma once

#include "../Utilities/String.h"
#include <cstdint>

#include "../Utilities/Pointer.h"

enum class ImageFormat
{
	RGBA8 = 0x0408,
	RGB8 = 0x0308,
	R8 = 0x0108,

	RGBA16 = 0x0410,
	RGB16 = 0x0310,
	R16 = 0x0110
};

class Image
{
public:
	Image(const AnsiString& filepath);

	Image(const Image&) = delete;

	Image& operator=(const Image&) = delete;

	bool Empty() const noexcept { return m_pData == nullptr; }

	ImageFormat GetFormat() const noexcept { return m_Format; }

	const uint8_t* GetData() const noexcept { return m_pData.get(); }

	const uint16_t* GetData16() const noexcept { return reinterpret_cast<uint16_t*>(m_pData.get()); }

	int Width() const noexcept { return m_Width; }
	int Height() const noexcept { return m_Height; }

private:
	UniquePtr<uint8_t[]> m_pData;
	size_t m_szData;
	ImageFormat m_Format;
	int m_Width;
	int m_Height;
};