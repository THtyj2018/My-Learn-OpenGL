#pragma once

#include "../Math/vec4.h"
#include "../Utilities/Pointer.h"

class Image;

enum class TextureWrapMode
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

class Texture
{
public:
	explicit Texture(SharedPtr<Image> pImage,
		TextureWrapMode modeX = TextureWrapMode::Repeat, TextureWrapMode modeY = TextureWrapMode::Repeat,
		Color border_coor = Color::black());

	Texture(const Texture&) = delete;

	Texture& operator=(const Texture&) = delete;

	bool Valid() const noexcept { return m_texid; }

	SharedPtr<Image> GetImage() const noexcept;

	TextureWrapMode GetTextureWrapModeX() const noexcept { return m_ModeX; }

	TextureWrapMode GetTextureWrapModeY() const noexcept { return m_ModeY; }

	Color GetBorderColor() const noexcept { return m_colorBorder; }

	uint32_t GetTextureID() const noexcept { return m_texid; }

private:
	SharedPtr<Image> m_pImage;
	TextureWrapMode m_ModeX;
	TextureWrapMode m_ModeY;
	Color m_colorBorder;
	uint32_t m_texid;
};