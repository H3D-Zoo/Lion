#pragma once
#include <vector>
#include <RenderAPI.h>
#include "math/gmlvector.h"
#include "math/gmlcolor.h"

struct ParticleVertexD
{
	gml::vec3 position;
	unsigned int color;
};

struct ParticleVertexS
{
	gml::vec3 offset;
	gml::vec2 texcoord;
};

class ParticleInstance
{
	struct Particle
	{
		unsigned int maxLifeTime;
		unsigned int lifeTime;
		gml::color4 color;
		gml::vec3 position;
		gml::vec3 velocity;
		gml::vec3 acceleration;

		void Update(unsigned int deltaTime);

		bool IsDead() { return lifeTime == 0; }

		float Alpha() { return (float)lifeTime / maxLifeTime; }
	};

public:

	const int kParticleCount = 256;
	const int kParticleVertexCount = kParticleCount * 4;
	const int kParticleFaceCount = kParticleCount * 2;
	const int kParticleIndexCount = kParticleFaceCount * 3;

	ParticleInstance();

	RenderAPI::VertexElement* GetSElementsPtr() { return &(m_sElements[0]); }

	RenderAPI::VertexElement* GetDElementsPtr() { return &(m_dElements[0]); }

	unsigned int GetSElementCount() { return m_sElements.size(); }

	unsigned int GetDElementCount() { return m_dElements.size(); }

	ParticleVertexS* GetSVerticesPtr() { return &(m_sVertices[0]); }

	unsigned short* GetIndicesPtr() { return &(m_indices[0]); }

	int GetParticleCount() { return m_particleCount; }

	void Update(unsigned int elapsedMillionSecond);

	int FillVertexBuffer(ParticleVertexD* posVBPtr, int offset);

private:
	int m_particleCount = 0;
	std::vector<Particle> m_particles;
	std::vector<RenderAPI::VertexElement> m_sElements;
	std::vector<RenderAPI::VertexElement> m_dElements;

	std::vector<ParticleVertexS> m_sVertices;
	std::vector<unsigned short> m_indices;

	const unsigned int kSpawnInterval = 5;
	unsigned int accumulatedSwanTime = 0;

	void UpdateParticles(unsigned int elapsedMillionSecond);

	void SpawnParticle();

	void FillStaticMesh();
};
