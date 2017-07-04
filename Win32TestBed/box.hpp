#pragma once
#include <vector>
#include <RenderAPI.h>
#include "math/gmlvector.h"
#include "math/gmlcolor.h"

struct BoxVertex
{
	gml::vec3 position;
	gml::vec2 texcoord;
	unsigned int color = gml::color4::random().rgba();
};


class BoxMesh
{
public:
	BoxMesh() : m_vertices(kVertexCount), m_indices(kIndexCount), m_elements(3)
	{
		FillBoxVertices();
		FillBoxIndices();

		m_elements[0].Format = RenderAPI::INPUT_Float3;
		m_elements[0].SemanticName = RenderAPI::SEMANTIC_POSITION;

		m_elements[1].Format = RenderAPI::INPUT_Float2;
		m_elements[1].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;

		m_elements[2].Format = RenderAPI::INPUT_Color4;
		m_elements[2].SemanticName = RenderAPI::SEMANTIC_COLOR;
	}

	RenderAPI::VertexElement* GetElementsPtr() { return &(m_elements[0]); }

	unsigned int GetElementCount() { return m_elements.size(); }

	BoxVertex* GetVerticesPtr() { return &(m_vertices[0]); }

	unsigned int GetVertexCount() { return m_vertices.size(); }

	unsigned short* GetIndicesPtr() { return &(m_indices[0]); }

	unsigned int GetIndexCount() { return m_indices.size(); }

private:
	const int kVertexCount = 24;
	const int kIndexCount = 36;

	std::vector<BoxVertex> m_vertices;
	std::vector<unsigned short> m_indices;

	std::vector<RenderAPI::VertexElement> m_elements;

	void FillBoxVertices()
	{
		const float kBoxSize = 1.0f;
		gml::vec3 vertexPositions[] =
		{
			{ -kBoxSize, -kBoxSize , -kBoxSize },
			{ +kBoxSize, -kBoxSize , -kBoxSize },
			{ +kBoxSize, +kBoxSize , -kBoxSize },
			{ -kBoxSize, +kBoxSize , -kBoxSize },
			{ -kBoxSize, -kBoxSize , +kBoxSize },
			{ +kBoxSize, -kBoxSize , +kBoxSize },
			{ +kBoxSize, +kBoxSize , +kBoxSize },
			{ -kBoxSize, +kBoxSize , +kBoxSize },
		};

		int index = 0;
		//front
		m_vertices[index++].position = vertexPositions[0];
		m_vertices[index++].position = vertexPositions[1];
		m_vertices[index++].position = vertexPositions[2];
		m_vertices[index++].position = vertexPositions[3];

		//back
		m_vertices[index++].position = vertexPositions[5];
		m_vertices[index++].position = vertexPositions[4];
		m_vertices[index++].position = vertexPositions[7];
		m_vertices[index++].position = vertexPositions[6];

		//left	 
		m_vertices[index++].position = vertexPositions[4];
		m_vertices[index++].position = vertexPositions[0];
		m_vertices[index++].position = vertexPositions[3];
		m_vertices[index++].position = vertexPositions[7];

		//right
		m_vertices[index++].position = vertexPositions[1];
		m_vertices[index++].position = vertexPositions[5];
		m_vertices[index++].position = vertexPositions[6];
		m_vertices[index++].position = vertexPositions[2];

		//top
		m_vertices[index++].position = vertexPositions[3];
		m_vertices[index++].position = vertexPositions[2];
		m_vertices[index++].position = vertexPositions[6];
		m_vertices[index++].position = vertexPositions[7];

		//botton
		m_vertices[index++].position = vertexPositions[4];
		m_vertices[index++].position = vertexPositions[5];
		m_vertices[index++].position = vertexPositions[1];
		m_vertices[index++].position = vertexPositions[0];


		for (index = 0; index < 6; index++)
		{
			int vIndex = index * 4;

			m_vertices[vIndex++].texcoord.set(0, 0);
			m_vertices[vIndex++].texcoord.set(1, 0);
			m_vertices[vIndex++].texcoord.set(1, 1);
			m_vertices[vIndex++].texcoord.set(0, 1);
		}
	}

	void FillBoxIndices()
	{
		int index = 0;
		for (int i = 0; i < 6; i++)
		{

			m_indices[index++] = i * 4 + 0;
			m_indices[index++] = i * 4 + 2;
			m_indices[index++] = i * 4 + 1;

			m_indices[index++] = i * 4 + 0;
			m_indices[index++] = i * 4 + 3;
			m_indices[index++] = i * 4 + 2;
		}
	}
};