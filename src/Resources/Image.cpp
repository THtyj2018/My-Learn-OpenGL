#include "Image.h"

#include "File.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const AnsiString& filepath) :
	m_pData(nullptr), m_szData(0), m_Format(ImageFormat::RGBA8), m_Width(0), m_Height(0)
{
	File file(filepath, FileAccess::ReadOnly, true);
	if (file.IsOpen() == false)
		return;
	UniquePtr<uint8_t[]> buffer = file.ReadBinaryData();
	int len = static_cast<int>(file.GetDataSize());
	int compbits = stbi_is_16_bit_from_memory(buffer.get(), len) ? 16 : 8;
	int comps = 0;
	int result = stbi_info_from_memory(buffer.get(), len, &m_Width, &m_Height, &comps);
	if (result == 0)
	{
		printf("Error loading image \"%s\": %s\n", filepath.c_str(), stbi_failure_reason());
		return;
	}

	if (comps == 2)
		comps = 3;
	if (comps > 4)
		comps = 4;

	if (compbits == 16)
		m_pData = UniquePtr<uint8_t[]>(
			reinterpret_cast<uint8_t*>(stbi_load_16_from_memory(buffer.get(), len, &m_Width, &m_Height, 0, comps)));
	else
		m_pData = UniquePtr<uint8_t[]>(stbi_load_from_memory(buffer.get(), len, &m_Width, &m_Height, 0, comps));
	if (m_pData == nullptr)
	{
		printf("Error loading image \"%s\": %s\n", filepath.c_str(), stbi_failure_reason());
		return;
	}
	m_szData = comps * (compbits >> 3) * m_Width * m_Height;
	m_Format = static_cast<ImageFormat>(compbits + (comps << 8));
}
