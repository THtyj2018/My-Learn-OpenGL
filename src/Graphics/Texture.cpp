#include "Texture.h"

#include "../Resources/Image.h"

#include <glad/glad.h>

GLenum GetGLTextureWrapMode(TextureWrapMode mode) noexcept
{
	switch (mode)
	{
	case TextureWrapMode::Repeat:
		return GL_REPEAT;
	case TextureWrapMode::MirroredRepeat:
		return GL_MIRRORED_REPEAT;
	case TextureWrapMode::ClampToEdge:
		return GL_CLAMP_TO_EDGE;
	case TextureWrapMode::ClampToBorder:
		return GL_CLAMP_TO_BORDER;
	default:
		return GL_REPEAT;
	}
}

GLenum GetGLTextureInternalFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGBA8:
		return GL_RGBA8;
	case ImageFormat::RGB8:
		return GL_RGB8;
	case ImageFormat::R8:
		return GL_R8;
	case ImageFormat::RGBA16:
		return GL_RGBA16;
	case ImageFormat::RGB16:
		return GL_RGB16;
	case ImageFormat::R16:
		return GL_R16;
	default:
		return GL_RGBA8;
	}
}

GLenum GetGLTextureFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::RGBA8:
	case ImageFormat::RGB16:
		return GL_RGBA;
	case ImageFormat::RGB8:
	case ImageFormat::RGBA16:
		return GL_RGB;
	case ImageFormat::R8:
	case ImageFormat::R16:
		return GL_R;
	default:
		return GL_RGBA;
	}
}

Texture::Texture(SharedPtr<Image> pImage, TextureWrapMode modeX, TextureWrapMode modeY, Color border_coor) :
	m_pImage(pImage), m_ModeX(modeX), m_ModeY(modeY), m_colorBorder(border_coor.clamp01())
{
	if (m_pImage == nullptr || m_pImage->Empty())
	{
		m_texid = 0;
		return;
	}
	glGenTextures(1, &m_texid);
	glBindTexture(GL_TEXTURE_2D, m_texid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLTextureWrapMode(m_ModeX));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLTextureWrapMode(m_ModeY));
	if (m_ModeX == TextureWrapMode::ClampToBorder || m_ModeY == TextureWrapMode::ClampToEdge)
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, m_colorBorder);

	ImageFormat format = m_pImage->GetFormat();
	glTexImage2D(GL_TEXTURE_2D, 0, GetGLTextureInternalFormat(format),
		m_pImage->Width(), m_pImage->Height(), 0,
		GetGLTextureFormat(format),
		((static_cast<int>(format) & 0xFF) == 16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_BYTE,
		m_pImage->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

SharedPtr<Image> Texture::GetImage() const noexcept
{
	return m_pImage;
}
