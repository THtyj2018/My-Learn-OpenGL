#pragma once

#include "VertexAttributes.h"
#include "../Math/mat4.h"
#include "../Utilities/Pointer.h"

class Pipeline;
class Material;

enum class PrimitiveType
{
	/// @brief For N>=0, vertex N renders a point.
	PointList = 'PLst',
	/// @brief For N>=0, vertices [N*2+0, N*2+1] render a line.
	LineList = 'LLst',
	/// @brief For N>=0, vertices [N, N+1] render a line.
	LineStrip = 'LStr',
	/// @brief Like <c>LineStrip</c>, but the first and last vertices also render a line.
	LineLoop = 'LLop',
	/// @brief For N>=0, vertices [N*3+0, N*3+1, N*3+2] render a triangle.
	TriangleList = 'TLst',
	/// @brief For N>=0, vertices [0, (N+1)%M, (N+2)%M] render a triangle, where M is the vertex count.
	TriangleFan = 'TFan',
	/// @brief For N>=0, vertices [N*2+0, N*2+1, N*2+2] and [N*2+2, N*2+1, N*2+3] render triangles.
	TriangleStrip = 'TStr',
	/// @brief Used for tessellation.
	Patch = 'Pach',
	/// @brief For N>=0, vertices [N*4..N*4+3] render a line from [1, 2].
	/// Lines [0, 1] and [2, 3] are adjacent to the rendered line.
	LineListAdjacency = 'LLAd',
	/// @brief For N>=0, vertices [N+1, N+2] render a line.
	/// Lines [N, N+1] and [N+2, N+3] are adjacent to the rendered line.
	LineStripAdjacency = 'LSAd',
	/// @brief For N>=0, vertices [N*6..N*6+5] render a triangle from [0, 2, 4].
	/// Triangles [0, 1, 2] [4, 2, 3] and [5, 0, 4] are adjacent to the rendered triangle.
	TriangleListAdjacency = 'TLAd',
	/// @brief For N>=0, vertices [N*4..N*4+6] render a triangle from [0, 2, 4] and [4, 2, 6].
	/// Odd vertices Nodd form adjacent triangles with indices min(Nodd+1,Nlast) and max(Nodd-3,Nfirst).
	TriangleStripAdjacency = 'TSAd',
};

enum class MeshShape
{
	Other = 0,
	Cuboid = 1,
	Cube = 1,
	Ellipsoid = 2,
	Sphere = 2
};

class Mesh
{
public:
	Mesh(SharedPtr<Pipeline> pipeline,
		size_t cntVertices, size_t cntIndices, const VertexAttributes& attrs,
		PrimitiveType type = PrimitiveType::TriangleList, SharedPtr<Material> material = nullptr);

	Mesh(const Mesh&) = delete;

	Mesh& operator=(const Mesh&) = delete;

	~Mesh();

	void Draw();

	static Mesh* NewCuboid(SharedPtr<Pipeline> pipeline, float x, float y, float z,
		const VertexAttributes& attr = VertexAttributes(), SharedPtr<Material> material = nullptr) noexcept;

	static Mesh* NewCube(SharedPtr<Pipeline> pipeline, float a,
		const VertexAttributes& attr, SharedPtr<Material> material = nullptr) noexcept;

	static Mesh* NewEllipsoid(SharedPtr<Pipeline> pipeline, float x, float y, float z, unsigned accuracy = 32,
		const VertexAttributes& attr = VertexAttributes(), SharedPtr<Material> material = nullptr);

	static Mesh* NewSphere(SharedPtr<Pipeline> pipeline, float d, unsigned accuracy = 32,
		const VertexAttributes& attr = VertexAttributes(), SharedPtr<Material> material = nullptr);

	void FillColor(const Color& color);

	/// @brief Enabled only if the mesh has a cuboid(or cube) shape and has TexCoord VertexAttrib
	/// @param minS Max texture coord S value
	/// @param minT Max texture coord T value
	/// @param maxS Max texture coord S value
	/// @param maxT Max texture coord T value
	void FillCuboidTexCoords(float minS = 0.0f, float minT = 0.0f, float maxS = 1.0f, float maxT = 1.0f);

	/// @brief Enabled only if the mesh has a ellipsoid(or sphere) shape and has TexCoord VertexAttrib
	void FillEllipsoidTexCoords();

	SharedPtr<Pipeline> GetPipeline() const noexcept;

	SharedPtr<Material> GetMaterial() const noexcept;

	const float* GetVerticesData() const noexcept { return m_pVerticesData.get(); }

	float* GetVerticesData() noexcept { return m_pVerticesData.get(); }

	const uint16_t* GetIndicesData() const noexcept { return m_pIndicesData.get(); }

	uint16_t* GetIndicesData() noexcept { return m_pIndicesData.get(); }

	const float* GetVerticesAttribData(VertexAttrib attr) const noexcept;

	float* GetVerticesAttribData(VertexAttrib attr) noexcept;

	void BindGPUResources();

	mat4 transform;

private:
	void UnbindGPUResources();

	void EnableVertexAttribs();

	SharedPtr<Pipeline> m_Pipeline;
	SharedPtr<Material> m_Material;
	UniquePtr<float[]> m_pVerticesData;
	UniquePtr<uint16_t[]> m_pIndicesData;
	const size_t m_cntVertices;
	const size_t m_cntIndices;
	const VertexAttributes m_Attrs;
	const PrimitiveType m_PrimtiveType;
	MeshShape m_Shape;
	uint32_t m_vbo;
	uint32_t m_ibo;
	uint32_t m_vao;
};