#include "VertexAttributes.h"

#include <algorithm>
#include <cstdio>
#include <memory.h>

VertexAttributes::VertexAttributes() noexcept :
	m_cnt(0)
{
	memset(m_attrs, 0, sizeof(VertexAttrib) * max_count);
}

VertexAttributes::VertexAttributes(VertexAttrib attr) noexcept :
	m_cnt(0)
{
	memset(m_attrs, 0, sizeof(VertexAttrib) * max_count);
	if (attr > VertexAttrib::Normal)
	{
		m_attrs[0] = attr;
		++m_cnt;
	}
}

VertexAttributes& VertexAttributes::operator<<(VertexAttrib attr) noexcept
{
	if (attr > VertexAttrib::Normal && !HasAttrib(attr))
	{
		m_attrs[m_cnt] = attr;
		++m_cnt;
		Sort();
	}
	return *this;
}

bool VertexAttributes::HasAttrib(VertexAttrib attr) const noexcept
{
	for (size_t i = 0; i < m_cnt; ++i)
		if (m_attrs[i] == attr)
			return true;
	return false;
}

unsigned VertexAttributes::TotalDims() const noexcept
{
	// Position + Normal
	unsigned dim = 3 + 3;
	for (size_t i = 0; i < m_cnt; ++i)
		dim += static_cast<unsigned>(m_attrs[i]) & 0xFF;
	return dim;
}

unsigned VertexAttributes::GetDimsBefore(VertexAttrib attr) const noexcept
{
	// Position + Normal
	unsigned dim = 3 + 3;
	for (size_t i = 0; i < m_cnt; ++i)
	{
		if (attr != m_attrs[i])
			dim += static_cast<unsigned>(m_attrs[i]) & 0xFF;
		else
			return dim;
	}
	return -1;
}

void VertexAttributes::Sort() noexcept
{
	std::sort(m_attrs, m_attrs + m_cnt);
}