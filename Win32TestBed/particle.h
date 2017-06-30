#pragma once
#include <vector>
#include "Math/gmlvector.h"
#include "Math/gmlcolor.h"

struct ParticleVertex
{
	gml::vec3 position;
	gml::color3 color;
};
class ParticleInstance
{
	struct Particle
	{
		unsigned int lifeTime;
		gml::color3 color;
		gml::vec3 position;
		gml::vec3 velocity;
		gml::vec3 acceleration;

		void Update(unsigned int deltaTime);

		bool IsDead() { return lifeTime == 0; }
	};

	int m_particleCount = 0;
	std::vector<Particle> m_particles;

	const unsigned int kSpawnInterval = 5;
	unsigned int accumulatedSwanTime = 0;

	void UpdateParticles(unsigned int elapsedMillionSecond);

	void SpawnParticle();

public:
	ParticleInstance();

	int GetParticleCount() { return m_particleCount; }

	void Update(unsigned int elapsedMillionSecond);

	int FillVertexBuffer(ParticleVertex* posVBPtr, int offset);
};
