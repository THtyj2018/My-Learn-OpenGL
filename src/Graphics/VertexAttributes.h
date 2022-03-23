#pragma once

enum class VertexAttrib
{
	None = 0x0000,
	Position = 0x0103,
	Normal = 0x0203,
	Color = 0x0304,
	TexCoord = 0x0402
};

class VertexAttributes
{
public:
	explicit VertexAttributes() noexcept;

	explicit VertexAttributes(VertexAttrib attr) noexcept;

	VertexAttributes(const VertexAttributes&) = default;

	VertexAttributes& operator<<(VertexAttrib attr) noexcept;

	bool HasAttrib(VertexAttrib attr) const noexcept;

	static constexpr size_t max_count = 8;

	unsigned TotalDims() const noexcept;

	/// @brief Get vectore total dims before attr
	/// @param attr Attribute to be query
	/// @return -1 if attr is not included 
	unsigned GetDimsBefore(VertexAttrib attr) const noexcept;

	const VertexAttrib* const GetAttribArray() const noexcept { return m_attrs; }

	const size_t GetAttribsCount() const noexcept { return m_cnt; }

private:
	void Sort() noexcept;

	VertexAttrib m_attrs[max_count];
	size_t m_cnt;
};