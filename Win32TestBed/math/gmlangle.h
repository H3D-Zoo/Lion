#pragma once
#include "gmlutility.h"
#include <cmath>

namespace gml
{
	class degree;
	class radian;

	class degree
	{
	public:
		float value = 0;

		constexpr degree() = default;

		constexpr explicit degree(float v) : value(v) { }

		constexpr degree(const radian& r);

		friend bool operator==(const degree& l, const degree& r);

		friend inline bool operator!=(const degree& l, const degree& r) { return !(l == r); }

		friend constexpr bool operator<(const degree& l, const degree& r);

		friend constexpr bool operator<=(const degree& l, const degree& r);

		friend constexpr bool operator>(const degree& l, const degree& r);

		friend constexpr bool operator>=(const degree& l, const degree& r);

		friend constexpr degree operator+(const degree& l, const degree& r);

		friend constexpr degree operator-(const degree& l, const degree& r);

		friend constexpr degree operator*(const degree& l, float r);

		friend constexpr degree operator*(float l, const degree& r);

		constexpr const degree& operator+() const;

		constexpr degree operator-() const;

		degree& operator+=(const degree& r);

		degree& operator-=(const degree& r);

		degree& operator*=(float r);

	private:
		static constexpr float r2d_factor = static_cast<float>(180.0 / PI_d);
	};

	class radian
	{
	public:
		float value = 0;

		constexpr radian() = default;

		constexpr explicit radian(float v) : value(v) { }

		constexpr radian(const degree& d);

		friend bool operator==(const radian& l, const radian& r);

		friend inline bool operator!=(const radian& l, const radian& r) { return !(l == r); }

		friend constexpr bool operator<(const radian& l, const radian& r);

		friend constexpr bool operator<=(const radian& l, const radian& r);

		friend constexpr bool operator>(const radian& l, const radian& r);

		friend constexpr bool operator>=(const radian& l, const radian& r);

		friend constexpr radian operator+(const radian& l, const radian& r);

		friend constexpr radian operator-(const radian& l, const radian& r);

		friend constexpr radian operator*(const radian& l, float r);

		friend constexpr radian operator*(float l, const radian& r);

		constexpr const radian& operator+() const;

		constexpr radian operator-() const;

		radian& operator+=(const radian& r);

		radian& operator-=(const radian& r);

		radian& operator*=(float r);

	private:
		static constexpr float d2r_factor = static_cast<float>(PI_d / 180.0);
	};

	float cos(const radian& r);

	float sin(const radian& r);
}

namespace gml
{
	constexpr degree::degree(const radian& r) : value(r.value * r2d_factor) { }

	inline bool operator==(const degree& l, const degree& r)
	{
		if (&l != &r)
		{
			return fequal(l.value, r.value);
		}
		else
		{
			return true;
		}
	}

	constexpr bool operator<(const degree& l, const degree& r)
	{
		return l.value < r.value;
	}

	constexpr bool operator<=(const degree& l, const degree& r)
	{
		return l.value <= r.value;
	}

	constexpr bool operator>(const degree& l, const degree& r)
	{
		return l.value > r.value;
	}

	constexpr bool operator>=(const degree& l, const degree& r)
	{
		return l.value >= r.value;
	}

	constexpr degree operator+(const degree& l, const degree& r)
	{
		return degree(l.value + r.value);
	}

	constexpr degree operator-(const degree& l, const degree& r)
	{
		return degree(l.value - r.value);
	}

	constexpr degree operator*(const degree& l, float r)
	{
		return degree(l.value * r);
	}

	constexpr degree operator*(float l, const degree& r)
	{
		return degree(l * r.value);
	}

	constexpr const degree& degree::operator+() const
	{
		return *this;
	}

	constexpr degree degree::operator-() const
	{
		return degree(-value);
	}

	inline degree& degree::operator+=(const degree& r)
	{
		value += r.value;
		return *this;
	}

	inline degree& degree::operator-=(const degree& r)
	{
		value -= r.value;
		return *this;
	}

	inline degree& degree::operator*=(float r)
	{
		value *= r;
		return *this;
	}

	constexpr radian::radian(const degree& d) : value(d.value * d2r_factor) { }

	inline bool operator==(const radian& l, const radian& r)
	{
		if (&l != &r)
		{
			return fequal(l.value, r.value);
		}
		else
		{
			return true;
		}
	}

	constexpr bool operator<(const radian& l, const radian& r)
	{
		return l.value < r.value;
	}

	constexpr bool operator<=(const radian& l, const radian& r)
	{
		return l.value <= r.value;
	}

	constexpr bool operator>(const radian& l, const radian& r)
	{
		return l.value > r.value;
	}

	constexpr bool operator>=(const radian& l, const radian& r)
	{
		return l.value >= r.value;
	}

	constexpr radian operator+(const radian& l, const radian& r)
	{
		return radian(l.value + r.value);
	}

	constexpr radian operator-(const radian& l, const radian& r)
	{
		return radian(l.value - r.value);
	}

	constexpr radian operator*(const radian& l, float r)
	{
		return radian(l.value * r);
	}

	constexpr radian operator*(float l, const radian& r)
	{
		return radian(l * r.value);
	}

	constexpr const radian& radian::operator+() const
	{
		return *this;
	}

	constexpr radian radian::operator-() const
	{
		return radian(-value);
	}

	inline radian& radian::operator+=(const radian& r)
	{
		value += r.value;
		return *this;
	}

	inline radian& radian::operator-=(const radian& r)
	{
		value -= r.value;
		return *this;
	}

	inline radian& radian::operator*=(float r)
	{
		value *= r;
		return *this;
	}

	inline float cos(const radian& r)
	{
		return cosf(r.value);
	}

	inline float sin(const radian& r)
	{
		return sinf(r.value);
	}

	inline radian limited_rotation(const radian& r)
	{
		if (r.value < 0)
		{
			float vtrunc = -trunc(r.value / PI2) + 1.0f;
			return radian(r.value + vtrunc * PI2);
		}
		else
		{
			return radian(fmod(r.value, PI2));
		}
	}
}