#include "Mesh.h"
#include "Pipeline.h"
#include "Graphics.h"
#include "Material.h"

#include <glad/glad.h>

#include <cassert>
#include <cstdio>
#include <memory.h>

static GLenum GetGLPrimitive(PrimitiveType type);

Mesh::Mesh(SharedPtr<Pipeline> pipeline, size_t cntVertices, size_t cntIndices,
	const VertexAttributes& attrs, PrimitiveType type, SharedPtr<Material> material) :
	m_Pipeline(pipeline), m_Material(material),
	m_cntVertices(cntVertices), m_cntIndices(cntIndices), m_Attrs(attrs), m_PrimtiveType(type),
	m_Shape(MeshShape::Other), m_vbo(0), m_ibo(0), m_vao(0)
{
	if (m_Material == nullptr)
		m_Material = Material::Default();
	if (cntIndices)
		m_pIndicesData = MakeUnique<uint16_t[]>(cntIndices);
	assert(cntVertices);
	m_pVerticesData = MakeUnique<float[]>(cntVertices * attrs.TotalDims());
}

Mesh::~Mesh()
{
	UnbindGPUResources();
}

void Mesh::Draw()
{
	m_Pipeline->SetShaderParam("MVP.Model", transform);
	glBindVertexArray(m_vao);
	if (m_cntIndices && m_ibo)
		glDrawElements(GetGLPrimitive(m_PrimtiveType), m_cntIndices, GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(GetGLPrimitive(m_PrimtiveType), 0, m_cntVertices);
	glBindVertexArray(0);
}

Mesh* Mesh::NewCuboid(SharedPtr<Pipeline> pipeline, float x, float y, float z,
	const VertexAttributes& attr, SharedPtr<Material> material) noexcept
{
	Mesh* mesh = new Mesh(pipeline, 24, 36, attr, PrimitiveType::TriangleList, material);

	vec3 fld = vec3(-x, -y, z) * 0.5f;
	vec3 frd = vec3(x, -y, z) * 0.5f;
	vec3 fru = vec3(x, y, z) * 0.5f;
	vec3 flu = vec3(-x, y, z) * 0.5f;
	vec3 bld = vec3(-x, -y, -z) * 0.5f;
	vec3 brd = vec3(x, -y, -z) * 0.5f;
	vec3 bru = vec3(x, y, -z) * 0.5f;
	vec3 blu = vec3(-x, y, -z) * 0.5f;

	vec3* pVertices = reinterpret_cast<vec3*>(mesh->GetVerticesData());
	vec3* pNormals = pVertices + 24;
	pVertices[0] = fld; pVertices[1] = frd; pVertices[2] = fru; pVertices[3] = flu;
	pNormals[0] = pNormals[1] = pNormals[2] = pNormals[3] = vec3::front();
	pVertices[4] = frd; pVertices[5] = brd; pVertices[6] = bru; pVertices[7] = fru;
	pNormals[4] = pNormals[5] = pNormals[6] = pNormals[7] = vec3::right();
	pVertices[8] = flu; pVertices[9] = fru; pVertices[10] = bru; pVertices[11] = blu;
	pNormals[8] = pNormals[9] = pNormals[10] = pNormals[11] = vec3::up();
	pVertices[12] = brd; pVertices[13] = bld; pVertices[14] = blu; pVertices[15] = bru;
	pNormals[12] = pNormals[13] = pNormals[14] = pNormals[15] = vec3::back();
	pVertices[16] = bld; pVertices[17] = fld; pVertices[18] = flu; pVertices[19] = blu;
	pNormals[16] = pNormals[17] = pNormals[18] = pNormals[19] = vec3::left();
	pVertices[20] = bld; pVertices[21] = fld; pVertices[22] = frd; pVertices[23] = brd;
	pNormals[20] = pNormals[21] = pNormals[22] = pNormals[23] = vec3::down();

	uint16_t* pIndices = mesh->GetIndicesData();
	pIndices[0] = 0; pIndices[1] = 1; pIndices[2] = 2;
	pIndices[3] = 0; pIndices[4] = 2; pIndices[5] = 3;
	pIndices[6] = 4; pIndices[7] = 5; pIndices[8] = 6;
	pIndices[9] = 4; pIndices[10] = 6; pIndices[11] = 7;
	pIndices[12] = 8; pIndices[13] = 9; pIndices[14] = 10;
	pIndices[15] = 8; pIndices[16] = 10; pIndices[17] = 11;
	pIndices[18] = 12; pIndices[19] = 13; pIndices[20] = 14;
	pIndices[21] = 12; pIndices[22] = 14; pIndices[23] = 15;
	pIndices[24] = 16; pIndices[25] = 17; pIndices[26] = 18;
	pIndices[27] = 16; pIndices[28] = 18; pIndices[29] = 19;
	pIndices[30] = 20; pIndices[31] = 21; pIndices[32] = 22;
	pIndices[33] = 20; pIndices[34] = 22; pIndices[35] = 23;

	mesh->m_Shape = MeshShape::Cuboid;
	return mesh;
}

Mesh* Mesh::NewCube(SharedPtr<Pipeline> pipeline, float a, const VertexAttributes& attr,
	SharedPtr<Material> material) noexcept
{
	return NewCuboid(pipeline, a, a, a, attr, material);
}

Mesh* Mesh::NewEllipsoid(SharedPtr<Pipeline> pipeline, float x, float y, float z, unsigned accuracy,
	const VertexAttributes& attr, const SharedPtr<Material> material)
{
	const unsigned ac = Mathf::clamp(accuracy, 1U, 179U);
	const unsigned latitudes = 2 * ac + 2;
	const size_t vtcnt = ac * (latitudes + 1) + 2 * latitudes;
	const size_t idxcnt = (2 * ac + 2) * latitudes;
	Mesh* mesh = new Mesh(pipeline, vtcnt, idxcnt, attr, PrimitiveType::TriangleStrip, material);
	vec3* pVertices = reinterpret_cast<vec3*>(mesh->GetVerticesData());

	float arg = -Mathf::Pi * 2.0f / latitudes;
	vec2* thetas = new vec2[latitudes + 1];
	for (unsigned j = 0; j <= latitudes; ++j)
		thetas[j] = vec2::unit(arg * j);

	float phi = Mathf::Pi / (ac + 1);
	vec3 scale = vec3(x, y, z) * 0.5f;
	for (unsigned i = 0; i < ac; ++i)
	{
		vec2 vphi = vec2::unit((i + 1) * phi);
		for (unsigned j = 0; j <= latitudes; ++j)
		{
			vec3 dir = vec3(vphi.y * thetas[j].x, vphi.x, vphi.y * thetas[j].y);
			float r = (dir / scale).rlength();
			unsigned sub = i * (latitudes + 1) + latitudes + j;
			// Position
			pVertices[sub] = r * dir;
			// Normal
			pVertices[sub + vtcnt] = 2.0f * pVertices[sub] / (scale * scale);
		}
	}
	const vec3 yv = vec3(0, y * 0.5f, 0);
	const vec3 yvn = yv.normalized();
	for (unsigned j = 0; j < latitudes; ++j)
	{
		pVertices[j] = yv;
		pVertices[j + vtcnt] = yvn;
		pVertices[vtcnt - latitudes + j] = -yv;
		pVertices[vtcnt - latitudes + j + vtcnt] = -yvn;
	}

	delete[] thetas;

	uint16_t* pIndices = mesh->GetIndicesData();
	for (unsigned j = 0; j < latitudes; ++j)
	{
		unsigned off = j * (2 * ac + 2);
		unsigned ib = j;
		unsigned ie = vtcnt - latitudes + j;
		if (j & 0x01)
		{
			pIndices[off] = ie;
			for (unsigned i = 0; i < ac; ++i)
			{
				// ??? CCW ???
				pIndices[off + 2 * i + 1] = static_cast<uint16_t>((ac - i - 1) * (latitudes + 1) + latitudes + j + 1);
				pIndices[off + 2 * i + 2] = static_cast<uint16_t>((ac - i - 1) * (latitudes + 1) + latitudes + j);
			}
			pIndices[off + (2 * ac + 1)] = ib;
		}
		else
		{
			pIndices[off] = ib;
			for (unsigned i = 0; i < ac; ++i)
			{
				pIndices[off + 2 * i + 1] = static_cast<uint16_t>(i * (latitudes + 1) + latitudes + j);
				pIndices[off + 2 * i + 2] = static_cast<uint16_t>(i * (latitudes + 1) + latitudes + j + 1);
			}
			pIndices[off + (2 * ac + 1)] = ie;
		}
	}

	mesh->m_Shape = MeshShape::Ellipsoid;
	return mesh;
}

Mesh* Mesh::NewSphere(SharedPtr<Pipeline> pipeline, float d, unsigned accuracy,
	const VertexAttributes& attr, SharedPtr<Material> material)
{
	return NewEllipsoid(pipeline, d, d, d, accuracy, attr, material);
}

void Mesh::FillColor(const Color& color)
{
	vec4 _color = color.clamp01();
	if (m_Attrs.HasAttrib(VertexAttrib::Color))
	{
		Color* pcolors = reinterpret_cast<Color*>(GetVerticesAttribData(VertexAttrib::Color));
		for (size_t i = 0; i < m_cntVertices; ++i)
			pcolors[i] = _color;
	}
}

void Mesh::FillCuboidTexCoords(float minS, float minT, float maxS, float maxT)
{
	if (m_Attrs.HasAttrib(VertexAttrib::TexCoord) == false)
		return;
	if (m_Shape != MeshShape::Cuboid || m_cntVertices != 24)
		return;
	minS = Mathf::abs(minS);
	minT = Mathf::abs(minT);
	maxS = Mathf::abs(maxS);
	maxT = Mathf::abs(maxT);

	vec2* texcoords = reinterpret_cast<vec2*>(GetVerticesAttribData(VertexAttrib::TexCoord));
	for (size_t i = 0; i < 24; i += 4)
	{
		texcoords[i] = vec2(minS, minT);
		texcoords[i + 1] = vec2(maxS, minT);
		texcoords[i + 2] = vec2(maxS, maxT);
		texcoords[i + 3] = vec2(minS, maxT);
	}
}

void Mesh::FillEllipsoidTexCoords()
{
	if (m_Attrs.HasAttrib(VertexAttrib::TexCoord) == false)
		return;
	if (m_Shape != MeshShape::Ellipsoid)
		return;

	const unsigned ac = static_cast<unsigned>((2 * m_cntVertices - m_cntIndices - 8) / 6);
	const unsigned latitudes = 2 * ac + 2;
	vec2* texcoords = reinterpret_cast<vec2*>(GetVerticesAttribData(VertexAttrib::TexCoord));

	for (unsigned j = 0; j < latitudes; ++j)
	{
		texcoords[j] = vec2((j + 0.5f) / latitudes, 1.0f);
		texcoords[m_cntVertices - latitudes + j] = vec2((j + 0.5f) / latitudes, 0.0f);
	}

	for (unsigned i = 0; i < ac; ++i)
	{
		const float T = (ac - i) / float(ac + 1);
		for (unsigned j = 0; j <= latitudes; ++j)
			texcoords[i * (latitudes + 1) + latitudes + j] = vec2(float(j) / latitudes, T);
	}
}

SharedPtr<Pipeline> Mesh::GetPipeline() const noexcept
{
	return m_Pipeline;
}

SharedPtr<Material> Mesh::GetMaterial() const noexcept
{
	return m_Material;
}

const float* Mesh::GetVerticesAttribData(VertexAttrib attr) const noexcept
{
	return GetVerticesData() + m_Attrs.GetDimsBefore(attr) * m_cntVertices;
}

float* Mesh::GetVerticesAttribData(VertexAttrib attr) noexcept
{
	return GetVerticesData() + m_Attrs.GetDimsBefore(attr) * m_cntVertices;
}

void Mesh::BindGPUResources()
{
	if (m_vao)
		return;

	size_t sz = m_cntVertices * m_Attrs.TotalDims() * sizeof(float);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sz), GetVerticesData(), GL_STATIC_DRAW);
	EnableVertexAttribs();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	if (m_cntIndices)
	{
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_cntIndices) * sizeof(uint16_t), GetIndicesData(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::UnbindGPUResources()
{
	if (m_vao == 0)
		return;

	glDeleteBuffers(1, &m_vbo);
	if (m_ibo)
		glDeleteBuffers(1, &m_ibo);
	glDeleteVertexArrays(1, &m_vao);

	m_vao = 0;
	m_vbo = 0;
	m_ibo = 0;
}

void Mesh::EnableVertexAttribs()
{
	GLuint offset = 0;

	GLint position = m_Pipeline->GetAttribLocation(VertexAttrib::Position);
	if (position < 0)
	{
		printf("Error: Can't find position attribute in current gl pipeline!\n");
		assert(0);
	}
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	offset += static_cast<GLuint>(sizeof(vec3) * m_cntVertices);

	GLint normal = m_Pipeline->GetAttribLocation(VertexAttrib::Normal);
	if (normal >= 0)
	{
		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, (void*)offset);
		offset += static_cast<GLuint>(sizeof(vec3) * m_cntVertices);
	}

	if (m_Attrs.HasAttrib(VertexAttrib::Color))
	{
		GLint color = m_Pipeline->GetAttribLocation(VertexAttrib::Color);
		if (color >= 0)
		{
			glEnableVertexAttribArray(color);
			glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += static_cast<GLuint>(sizeof(Color) * m_cntVertices);
		}
	}

	if (m_Attrs.HasAttrib(VertexAttrib::TexCoord))
	{
		GLint texcoord = m_Pipeline->GetAttribLocation(VertexAttrib::TexCoord);
		if (texcoord > 0)
		{
			glEnableVertexAttribArray(texcoord);
			glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);
			offset += static_cast<GLuint>(sizeof(vec2) * m_cntVertices);
		}
	}
}



GLenum GetGLPrimitive(PrimitiveType type)
{
	switch (type)
	{
	case PrimitiveType::PointList:
		return GL_POINTS;
	case PrimitiveType::LineList:
		return GL_LINES;
	case PrimitiveType::LineStrip:
		return GL_LINE_STRIP;
	case PrimitiveType::LineLoop:
		return GL_LINE_LOOP;
	case PrimitiveType::TriangleList:
		return GL_TRIANGLES;
	case PrimitiveType::TriangleFan:
		return GL_TRIANGLE_FAN;
	case PrimitiveType::TriangleStrip:
		return GL_TRIANGLE_STRIP;
	case PrimitiveType::Patch:
		return GL_PATCHES;
	case PrimitiveType::LineListAdjacency:
		return GL_LINES_ADJACENCY;
	case PrimitiveType::LineStripAdjacency:
		return GL_LINE_STRIP_ADJACENCY;
	case PrimitiveType::TriangleListAdjacency:
		return GL_TRIANGLES_ADJACENCY;
	case PrimitiveType::TriangleStripAdjacency:
		return GL_TRIANGLE_STRIP_ADJACENCY;
	default:
		printf("Error: Unexpected PrimitiveType enum value!\n");
		assert(0);
		return GL_NONE;
	}
}