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
	if (m_particles.size() <= (unsigned int)m_particleCount)
	{
		m_particles.push_back(Particle());
	}
	Particle& particle = m_particles[m_particleCount++];

	particle.color = gml::color4::random();
	particle.maxLifeTime = particle.lifeTime = RandomRangeI(800, 1200);

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

void ParticleInstance::FillStaticMesh()
{
	const float kParticleSize = 0.1f;
	for (int i = 0; i < kParticleCount; i++)
	{
		int vIndex = i * 4;
		int fIndex = i * 6;
		ParticleVertexS& v0 = m_sVertices[vIndex];
		ParticleVertexS& v1 = m_sVertices[vIndex + 1];
		ParticleVertexS& v2 = m_sVertices[vIndex + 2];
		ParticleVertexS& v3 = m_sVertices[vIndex + 3];
		unsigned short* face0 = &(m_indices[fIndex]);
		unsigned short* face1 = &(m_indices[fIndex + 3]);

		v0.offset.x = -kParticleSize;
		v0.offset.y = -kParticleSize;
		v0.offset.z = 0;
		v0.texcoord.x = 0;
		v0.texcoord.y = 0;

		v1.offset.x = +kParticleSize;
		v1.offset.y = -kParticleSize;
		v1.offset.z = 0;
		v1.texcoord.x = 1;
		v1.texcoord.y = 0;

		v2.offset.x = +kParticleSize;
		v2.offset.y = +kParticleSize;
		v2.offset.z = 0;
		v2.texcoord.x = 1;
		v2.texcoord.y = 1;

		v3.offset.x = -kParticleSize;
		v3.offset.y = +kParticleSize;
		v3.offset.z = 0;
		v3.texcoord.x = 0;
		v3.texcoord.y = 1;

		face0[0] = vIndex + 0;
		face0[1] = vIndex + 2;
		face0[2] = vIndex + 1;

		face1[0] = vIndex + 0;
		face1[1] = vIndex + 3;
		face1[2] = vIndex + 2;
	}
}

ParticleInstance::ParticleInstance()
	: m_sVertices(kParticleVertexCount)
	, m_indices(kParticleIndexCount)
	, m_sElements(2)
	, m_dElements(2)
{
	FillStaticMesh();
	for (int i = 0; i < 10; i++)
	{
		SpawnParticle();
	}

	m_sElements[0].Format = RenderAPI::INPUT_Float3;
	m_sElements[0].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	m_sElements[0].SemanticIndex = 0;

	m_sElements[1].Format = RenderAPI::INPUT_Float2;
	m_sElements[1].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	m_sElements[1].SemanticIndex = 1;

	m_dElements[0].Format = RenderAPI::INPUT_Float3;
	m_dElements[0].SemanticName = RenderAPI::SEMANTIC_POSITION;
	m_dElements[0].SemanticIndex = 0;

	m_dElements[1].Format = RenderAPI::INPUT_Color4;
	m_dElements[1].SemanticName = RenderAPI::SEMANTIC_COLOR;
	m_dElements[1].SemanticIndex = 0;
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

int ParticleInstance::FillVertexBuffer(ParticleVertexD* posVBPtr, int offset)
{
	int fillCount = __min(kParticleCount, m_particleCount - offset);

	for (int i = 0; i < fillCount; i++)
	{
		int vIndex = i * 4;
		ParticleVertexD& v0 = posVBPtr[vIndex];
		ParticleVertexD& v1 = posVBPtr[vIndex + 1];
		ParticleVertexD& v2 = posVBPtr[vIndex + 2];
		ParticleVertexD& v3 = posVBPtr[vIndex + 3];
		Particle& p = m_particles[offset + i];
		v0.color = v1.color = v2.color = v3.color = p.color.bgra();
		v0.position = v1.position = v2.position = v3.position = p.position;
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

		color.a = Alpha();
	}
}
