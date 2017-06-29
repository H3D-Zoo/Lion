#include "particle.h"
#include "Math/gmlangle.h"

void ParticleInstance::UpdateParticles(unsigned int elapsedMillionSecond)
{
	for (int i = 0; i < m_particleCount;)
	{
		Particle& particle = m_particles[i];

		particle.Update(elapsedMillionSecond);

		if (particle.IsDead())
		{
			particle = m_particles[m_particleCount - 1];
			m_particleCount -= 1;
		}
		else
		{
			i++;
		}
	}
}

template<class T> T RandomRangeI(T min, T max) { return min + rand() % (max - min + 1); }
extern float RandomRangeF(float  min, float max);

void ParticleInstance::SpawnParticle()
{
	if (m_particles.size() <= m_particleCount)
	{
		m_particles.push_back(Particle());
	}
	Particle& particle = m_particles[m_particleCount++];

	particle.lifeTime = RandomRangeI(800, 1200);

	//box range
	const float kBoxRange = 1.0f;
	particle.position.x = RandomRangeF(-kBoxRange, kBoxRange);
	particle.position.y = RandomRangeF(-kBoxRange, kBoxRange);
	particle.position.z = RandomRangeF(-kBoxRange, kBoxRange);

	//horizontal acceleration
	const gml::degree accDirection(RandomRangeF(0, 360.0f));
	float cosa = gml::cos(accDirection);
	float sina = gml::sin(accDirection);
	float gravity = RandomRangeF(-8, -10);
	particle.acceleration = gml::vec3(cosa, gravity, sina);

	//upward velocity
	particle.velocity.y = RandomRangeF(4, 7);

}

ParticleInstance::ParticleInstance()
{
	for (int i = 0; i < 10; i++)
	{
		SpawnParticle();
	}
}

void ParticleInstance::Update(unsigned int elapsedMillionSecond)
{
	UpdateParticles(elapsedMillionSecond);

	accumulatedSwanTime += elapsedMillionSecond;
	while (accumulatedSwanTime > kSpawnInterval)
	{
		SpawnParticle();
		accumulatedSwanTime -= kSpawnInterval;
	}
}

int ParticleInstance::FillVertexBuffer(gml::vec3 * posVBPtr, int offset)
{
	const int kParticleCount = 256;
	int fillCount = __min(kParticleCount, m_particleCount - offset);

	for (int i = 0; i < fillCount; i++)
	{
		int vIndex = i * 4;
		gml::vec3& v0 = posVBPtr[vIndex];
		gml::vec3& v1 = posVBPtr[vIndex + 1];
		gml::vec3& v2 = posVBPtr[vIndex + 2];
		gml::vec3& v3 = posVBPtr[vIndex + 3];
		Particle& p = m_particles[offset + i];
		v0 = v1 = v2 = v3 = p.position;
	}
	return fillCount;
}

void ParticleInstance::Particle::Update(unsigned int deltaTime)
{
	if (deltaTime >= lifeTime)
	{
		lifeTime = 0;
	}
	else
	{
		lifeTime -= deltaTime;
		float second = deltaTime * 0.001f;

		position += velocity * second + 0.5f *acceleration * second * second;
		velocity += acceleration * second;
	}
}
