#pragma once
#include <vector>
#include "Math/gmlvector.h"

class ParticleInstance
{
	struct Particle
	{
		unsigned int lifeTime;
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

	int FillVertexBuffer(gml::vec3* posVBPtr, int offset);
};
