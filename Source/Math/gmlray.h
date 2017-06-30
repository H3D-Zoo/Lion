#pragma once
#include "gmlvector.h"

namespace gml
{
	class ray
	{
	public:
		constexpr const vec3& origin() const { return m_origin; }

		constexpr const vec3& direction() const { return m_dir; }

		void set_origin(const vec3& origin);

		void set_dir(const vec3& dir);

		constexpr vec3 get_offset(float length) const;

	private:
		vec3 m_origin;
		vec3 m_dir;
	};

}

namespace gml
{
	inline void ray::set_origin(const vec3& origin)
	{
		m_origin = origin;
	}

	inline void ray::set_dir(const vec3& dir)
	{
		m_dir = dir.normalized();
	}

	constexpr vec3 ray::get_offset(float length) const
	{
		return m_origin + m_dir * length;
	}
}
