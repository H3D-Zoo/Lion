#pragma once
#include "gmlutility.h"
#include <cassert>
#include <cmath>

namespace gml
{
	class vec2;
	class vec3;
	class vec4;

	class vec2
	{
	public:
		constexpr static const int DIMENSION = 2;
		static const vec2& zero();
		static const vec2& one();
		static const vec2& left();
		static const vec2& right();
		static const vec2& up();
		static const vec2& down();

	public:
		float x = 0.0f;
		float y = 0.0f;

	public:
		constexpr vec2() = default;

		constexpr vec2(float _x, float _y) : x(_x), y(_y) { }

		constexpr explicit vec2(const vec3& v3);

		constexpr explicit vec2(const vec4& v4);

		friend bool operator==(const vec2& lhs, const vec2& rhs);

		friend inline bool operator!=(const vec2& lhs, const vec2& rhs) { return !(lhs == rhs); }

		constexpr const vec2& operator+() const { return *this; }

		constexpr vec2 operator-() const;

		friend constexpr vec2 operator+(const vec2& lhs, const vec2& rhs);

		friend constexpr vec2 operator+(const vec2& lhs, float rhs);

		friend constexpr vec2 operator+(float lhs, const vec2& rhs);

		friend constexpr vec2 operator-(const vec2& lhs, const vec2& rhs);

		friend constexpr vec2 operator-(const vec2& lhs, float rhs);

		friend constexpr vec2 operator-(float lhs, const vec2& rhs);

		friend constexpr vec2 operator*(const vec2& lhs, const vec2& rhs);

		friend constexpr vec2 operator*(const vec2& lhs, float rhs);

		friend constexpr vec2 operator*(float lhs, const vec2& rhs);

		friend vec2& operator+=(vec2& lhs, const vec2& rhs);

		friend vec2& operator+=(vec2& lhs, float rhs);

		friend vec2& operator-=(vec2& lhs, const vec2& rhs);

		friend vec2& operator-=(vec2& lhs, float rhs);

		friend vec2& operator*=(vec2& lhs, const vec2& rhs);

		friend vec2& operator*=(vec2& lhs, float rhs);

		float& operator[](int index);//hack

		const float& operator[](int index) const;

		explicit operator float*();

		constexpr explicit operator const float*() const { return &(x); }

		void set(float _x, float _y);

		void set(const vec3& v3);

		void set(const vec4& v4);

		void normalize();

		vec2 normalized() const;

		void inverse();

		vec2 inversed() const;

		float length() const;

		constexpr float length_sqr() const;
	};

	class vec3
	{
	public:
		constexpr static const int DIMENSION = 3;
		static const vec3& zero();
		static const vec3& one();
		static const vec3& left();
		static const vec3& right();
		static const vec3& up();
		static const vec3& down();
		static const vec3& forward();
		static const vec3& backward();

	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

	public:
		constexpr vec3() = default;

		constexpr vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

		constexpr vec3(const vec2& v2, float _z) : x(v2.x), y(v2.y), z(_z) { }

		constexpr explicit vec3(const vec4& v4);

		friend bool operator==(const vec3& lhs, const vec3& rhs);

		friend inline bool operator!=(const vec3& lhs, const vec3& rhs) { return !(lhs == rhs); }

		constexpr const vec3& operator+() const { return *this; }

		constexpr vec3 operator-() const;

		friend constexpr vec3 operator+(const vec3& lhs, const vec3& rhs);

		friend constexpr vec3 operator+(const vec3& lhs, float rhs);

		friend constexpr vec3 operator+(float lhs, const vec3& rhs);

		friend constexpr vec3 operator-(const vec3& lhs, const vec3& rhs);

		friend constexpr vec3 operator-(const vec3& lhs, float rhs);

		friend constexpr vec3 operator-(float lhs, const vec3& rhs);

		friend constexpr vec3 operator*(const vec3& lhs, const vec3& rhs);

		friend constexpr vec3 operator*(const vec3& lhs, float rhs);

		friend constexpr vec3 operator*(float lhs, const vec3& rhs);

		friend vec3& operator+=(vec3& lhs, const vec3& rhs);

		friend vec3& operator+=(vec3& lhs, float rhs);

		friend vec3& operator-=(vec3& lhs, const vec3& rhs);

		friend vec3& operator-=(vec3& lhs, float rhs);

		friend vec3& operator*=(vec3& lhs, const vec3& rhs);

		friend vec3& operator*=(vec3& lhs, float rhs);

		float& operator[](int index);//hack

		const float& operator[](int index) const;

		explicit operator float*();

		constexpr explicit operator const float*() const { return &(x); }

		void set(float _x, float _y, float _z);

		void set(const vec2& v2, float _z);

		void set(const vec4& v4);

		void replace(const vec2& v2);

		void normalize();

		vec3 normalized() const;

		void inverse();

		vec3 inversed() const;

		float length() const;

		constexpr float length_sqr() const;
	};

	class vec4
	{
	public:
		constexpr static const int DIMENSION = 4;
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

	public:
		constexpr vec4() = default;

		constexpr vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }

		constexpr vec4(const vec2& v2, float _z, float _w) : x(v2.x), y(v2.y), z(_z), w(_w) { }

		constexpr vec4(const vec3& v3, float _w) : x(v3.x), y(v3.y), z(v3.z), w(_w) { }

		friend bool operator==(const vec4& lhs, const vec4& rhs);

		friend inline bool operator!=(const vec4& lhs, const vec4& rhs) { return !(lhs == rhs); }

		constexpr const vec4& operator+() const { return *this; }

		constexpr vec4 operator-() const;

		friend constexpr vec4 operator*(const vec4& lhs, const vec4& rhs);

		friend constexpr vec4 operator*(const vec4& lhs, float rhs);

		friend constexpr vec4 operator*(float lhs, const vec4& rhs);

		friend vec4 operator*=(vec4& lhs, const vec4& rhs);

		friend vec4 operator*=(vec4& lhs, float rhs);

		float& operator[](int index);//hack

		const float& operator[](int index) const;

		explicit operator float*();

		constexpr explicit operator const float*() const { return &(x); }

		void set(float _x, float _y, float _z, float _w);

		void set(const vec2& v2, float _z, float _w);

		void set(const vec3& v3, float _w);

		void replace(const vec2& v2);

		void replace(const vec3& v2);

		float length() const;

		constexpr float length_sqr() const;
	};

	constexpr float dot(const vec2& lhs, const vec2& rhs);

	constexpr float dot(const vec3& lhs, const vec3& rhs);

	constexpr float dot(const vec4& lhs, const vec4& rhs);

	constexpr float cross(const vec2& lhs, const vec2& rhs);

	constexpr vec3 cross(const vec3& lhs, const vec3& rhs);

	constexpr vec2 min_combine(const vec2& lhs, const vec2& rhs);

	constexpr vec3 min_combine(const vec3& lhs, const vec3& rhs);

	constexpr vec2 max_combine(const vec2& lhs, const vec2& rhs);

	constexpr vec3 max_combine(const vec3& lhs, const vec3& rhs);

	constexpr float det22(const vec2& row1, const vec2& row2);

	constexpr float det33(const vec3& row1, const vec3& row2, const vec3& row3);

	constexpr float det44(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4);

	constexpr float det22_t(const vec2& row1, const vec2& row2);

	constexpr float det33_t(const vec3& row1, const vec3& row2, const vec3& row3);

	constexpr float det44_t(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4);

	struct _0; struct _1;
	struct _X { constexpr static const int SwizzleIndex = 0; };
	struct _Y { constexpr static const int SwizzleIndex = 1; };
	struct _Z { constexpr static const int SwizzleIndex = 2; };
	struct _W { constexpr static const int SwizzleIndex = 3; };

	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec2& v2);

	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec3& v3);

	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec4& v4);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec2& v2);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec3& v3);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec4& v4);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec2& v2);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec3& v3);

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec4& v4);
}

namespace gml
{
	inline const vec2& vec2::zero()
	{
		static vec2 v(0.0f, 0.0f);
		return v;
	}
	inline const vec2& vec2::one()
	{
		static vec2 v(1.0f, 1.0f);
		return v;
	}
	inline const vec2& vec2::left()
	{
		static vec2 v(-1.0f, 0.0f);
		return v;
	}
	inline const vec2& vec2::right()
	{
		static vec2 v(1.0f, 0.0f);
		return v;
	}
	inline const vec2& vec2::up()
	{
		static vec2 v(0.0f, 1.0f);
		return v;
	}
	inline const vec2& vec2::down()
	{
		static vec2 v(0.0f, -1.0f);
		return v;
	}

	constexpr vec2::vec2(const vec3& v3) : x(v3.x), y(v3.y) { }

	constexpr vec2::vec2(const vec4& v4) : x(v4.x), y(v4.y) { }

	inline bool operator==(const vec2& lhs, const vec2& rhs)
	{
		if (&lhs != &rhs)
		{
			return fequal(lhs.x, rhs.x) && fequal(lhs.y, rhs.y);
		}
		else
		{
			return true;
		}
	}

	constexpr vec2 vec2::operator-() const
	{
		return vec2(-x, -y);
	}

	constexpr vec2 operator+(const vec2& lhs, const vec2& rhs)
	{
		return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	constexpr vec2 operator+(const vec2& lhs, float rhs)
	{
		return vec2(lhs.x + rhs, lhs.y + rhs);
	}

	constexpr vec2 operator+(float lhs, const vec2& rhs)
	{
		return rhs + lhs;
	}

	constexpr vec2 operator-(const vec2& lhs, const vec2& rhs)
	{
		return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	constexpr vec2 operator-(const vec2& lhs, float rhs)
	{
		return vec2(lhs.x - rhs, lhs.y - rhs);
	}

	constexpr vec2 operator-(float lhs, const vec2& rhs)
	{
		return vec2(lhs - rhs.x, lhs - rhs.y);
	}

	constexpr vec2 operator*(const vec2& lhs, const vec2& rhs)
	{
		return vec2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	constexpr vec2 operator*(const vec2& lhs, float rhs)
	{
		return vec2(lhs.x * rhs, lhs.y * rhs);
	}

	constexpr vec2 operator*(float lhs, const vec2& rhs)
	{
		return rhs * lhs;
	}

	inline vec2& operator+=(vec2& lhs, const vec2& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}

	inline vec2& operator+=(vec2& lhs, float rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		return lhs;
	}

	inline vec2& operator-=(vec2& lhs, const vec2& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}
	inline vec2& operator-=(vec2& lhs, float rhs)
	{
		lhs.x -= rhs;
		lhs.y -= rhs;
		return lhs;
	}

	inline vec2& operator*=(vec2& lhs, const vec2& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}

	inline vec2& operator*=(vec2& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}

	inline float& vec2::operator[](int index)
	{
		return const_cast<float&>(const_cast<const vec2*>(this)->operator[](index));
	}

	inline const float& vec2::operator[](int index) const
	{
		assert(index >= 0 || index < DIMENSION);
		return *(&x + index);
	}

	inline vec2::operator float*()
	{
		return const_cast<float*>(const_cast<const vec2*>(this)->operator const float*());
	}

	inline void vec2::set(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	inline void vec2::set(const vec3& v3)
	{
		set(v3.x, v3.y);
	}

	inline void vec2::set(const vec4& v4)
	{
		set(v4.x, v4.y);
	}

	inline void vec2::normalize()
	{
		float length2 = length_sqr();
		if (!fequal(length2, 0.0f))
		{
			if (!fequal(1.0f, length2))
			{
				auto invLength = 1.0f / sqrtf(length2);
				x *= invLength;
				y *= invLength;
			}
		}
		else
		{
			*this = zero();
		}
	}

	inline vec2 vec2::normalized() const
	{
		vec2 copy(*this);
		copy.normalize();
		return copy;
	}

	inline void vec2::inverse()
	{
		float length2 = length_sqr();
		if (!fequal(length2, 0.0f))
		{
			x /= length2;
			y /= length2;
		}
		else
		{
			*this = zero();
		}
	}

	inline vec2 vec2::inversed() const
	{
		vec2 copy(*this);
		copy.inverse();
		return copy;
	}

	inline float vec2::length() const
	{
		return sqrtf(length_sqr());
	}

	constexpr float vec2::length_sqr() const
	{
		return x*x + y*y;
	}

	inline const vec3& vec3::zero()
	{
		static vec3 v(0.0f, 0.0f, 0.0f);
		return v;
	}
	inline const vec3& vec3::one()
	{
		static vec3 v(1.0f, 1.0f, 1.0f);
		return v;
	}
	inline const vec3& vec3::left()
	{
		static vec3 v(-1.0f, 0.0f, 0.0f);
		return v;
	}
	inline const vec3& vec3::right()
	{
		static vec3 v(1.0f, 0.0f, 0.0f);
		return v;
	}
	inline const vec3& vec3::up()
	{
		static vec3 v(0.0f, 1.0f, 0.0f);
		return v;
	}
	inline const vec3& vec3::down()
	{
		static vec3 v(0.0f, -1.0f, 0.0f);
		return v;
	}
	inline const vec3& vec3::forward()
	{
		static vec3 v(0.0f, 0.0f, 1.0f);
		return v;
	}
	inline const vec3& vec3::backward()
	{
		static vec3 v(0.0f, 0.0f, -1.0f);
		return v;
	}

	constexpr vec3::vec3(const vec4& v4) : x(v4.x), y(v4.y), z(v4.z) { }

	inline bool operator==(const vec3& lhs, const vec3& rhs)
	{
		if (&lhs != &rhs)
		{
			return fequal(lhs.x, rhs.x) &&
				fequal(lhs.y, rhs.y) &&
				fequal(lhs.z, rhs.z);
		}
		else
		{
			return true;
		}
	}

	constexpr vec3 vec3::operator-() const
	{
		return vec3(-x, -y, -z);
	}

	constexpr vec3 operator+(const vec3& lhs, const vec3& rhs)
	{
		return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	constexpr vec3 operator+(const vec3& lhs, float rhs)
	{
		return vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}

	constexpr vec3 operator+(float lhs, const vec3& rhs)
	{
		return rhs + lhs;
	}

	constexpr vec3 operator-(const vec3& lhs, const vec3& rhs)
	{
		return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	constexpr vec3 operator-(const vec3& lhs, float rhs)
	{
		return vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
	}

	constexpr vec3 operator-(float lhs, const vec3& rhs)
	{
		return vec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
	}

	constexpr vec3 operator*(const vec3& lhs, const vec3& rhs)
	{
		return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	constexpr vec3 operator*(const vec3& lhs, float rhs)
	{
		return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}

	constexpr vec3 operator*(float lhs, const vec3& rhs)
	{
		return rhs * lhs;
	}

	inline vec3& operator+=(vec3& lhs, const vec3& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}

	inline vec3& operator+=(vec3& lhs, float rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
		return lhs;
	}

	inline vec3& operator-=(vec3& lhs, const vec3& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}

	inline vec3& operator-=(vec3& lhs, float rhs)
	{
		lhs.x -= rhs;
		lhs.y -= rhs;
		lhs.z -= rhs;
		return lhs;
	}

	inline vec3& operator*=(vec3& lhs, const vec3& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		return lhs;
	}

	inline vec3& operator*=(vec3& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		return lhs;
	}

	inline float& vec3::operator[](int index)
	{
		return const_cast<float&>(const_cast<const vec3*>(this)->operator[](index));
	}

	inline const float& vec3::operator[](int index) const
	{
		assert(index >= 0 || index < DIMENSION);
		return *(&x + index);
	}

	inline vec3::operator float*()
	{
		return const_cast<float*>(const_cast<const vec3*>(this)->operator const float*());
	}

	inline void vec3::set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	inline void vec3::set(const vec2& v2, float _z)
	{
		set(v2.x, v2.y, _z);
	}

	inline void vec3::set(const vec4& v4)
	{
		set(v4.x, v4.y, v4.z);
	}

	inline void vec3::replace(const vec2& v2)
	{
		x = v2.x;
		y = v2.y;
	}

	inline void vec3::normalize()
	{
		float length2 = length_sqr();
		if (!fequal(length2, 0.0f))
		{
			if (!fequal(1.0f, length2))
			{
				float invLength = 1.0f / sqrtf(length2);
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
		}
		else
		{
			*this = zero();
		}
	}

	inline vec3 vec3::normalized() const
	{
		vec3 copy(*this);
		copy.normalize();
		return copy;
	}

	inline void vec3::inverse()
	{
		float length2 = length_sqr();
		if (!fequal(length2, 0.0f))
		{
			x /= length2;
			y /= length2;
			z /= length2;
		}
		else
		{
			*this = zero();
		}
	}

	inline vec3 vec3::inversed() const
	{
		vec3 copy(*this);
		copy.inverse();
		return copy;
	}

	inline float vec3::length() const
	{
		return sqrtf(length_sqr());
	}

	constexpr float vec3::length_sqr() const
	{
		return x*x + y*y + z*z;
	}

	constexpr vec4 vec4::operator-() const
	{
		return vec4(-x, -y, -z, -w);
	}

	inline bool operator==(const vec4& lhs, const vec4& rhs)
	{
		if (&lhs != &rhs)
		{
			return fequal(lhs.x, rhs.x) &&
				fequal(lhs.y, rhs.y) &&
				fequal(lhs.z, rhs.z) &&
				fequal(lhs.w, rhs.w);
		}
		else
		{
			return true;
		}
	}

	constexpr vec4 operator*(const vec4& lhs, const vec4& rhs)
	{
		return vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}

	constexpr vec4 operator*(const vec4& lhs, float rhs)
	{
		return vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}

	constexpr vec4 operator*(float lhs, const vec4& rhs)
	{
		return rhs * lhs;
	}

	inline vec4 operator*=(vec4& lhs, const vec4& rhs)
	{
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.w *= rhs.w;
		return lhs;
	}

	inline vec4 operator*=(vec4& lhs, float rhs)
	{
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
		return lhs;
	}

	inline float& vec4::operator[](int index)
	{
		return const_cast<float&>(const_cast<const vec4*>(this)->operator[](index));
	}

	inline const float& vec4::operator[](int index) const
	{
		assert(index >= 0 || index < DIMENSION);
		return *(&x + index);
	}

	inline vec4::operator float*()
	{
		return const_cast<float*>(const_cast<const vec4*>(this)->operator const float*());
	}

	inline void vec4::set(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	inline void vec4::set(const vec2& v2, float _z, float _w)
	{
		set(v2.x, v2.y, _z, _w);
	}

	inline void vec4::set(const vec3& v3, float _w)
	{
		set(v3.x, v3.y, v3.z, _w);
	}

	inline void vec4::replace(const vec2& v2)
	{
		x = v2.x;
		y = v2.y;
	}

	inline void vec4::replace(const vec3& v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
	}

	inline float vec4::length() const
	{
		return sqrtf(length_sqr());
	}

	constexpr float vec4::length_sqr() const
	{
		return x*x + y*y + z*z + w*w;
	}


	constexpr float dot(const vec2& lhs, const vec2& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	constexpr float cross(const vec2& lhs, const vec2& rhs)
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}

	constexpr vec2 min_combine(const vec2& lhs, const vec2& rhs)
	{
		return vec2(
			lhs.x < rhs.x ? lhs.x : rhs.x,
			lhs.y < rhs.y ? lhs.y : rhs.y);
	}

	constexpr vec2 max_combine(const vec2& lhs, const vec2& rhs)
	{
		return vec2(
			lhs.x > rhs.x ? lhs.x : rhs.x,
			lhs.y > rhs.y ? lhs.y : rhs.y);
	}

	constexpr float det22(const vec2& row1, const vec2& row2)
	{
		return gml_impl::determinant(
			row1.x, row1.y,
			row2.x, row2.y);
	}

	constexpr float det22_t(const vec2& row1, const vec2& row2)
	{
		return gml_impl::determinant(
			row1.x, row2.x,
			row1.y, row2.y);
	}

	constexpr float dot(const vec3& lhs, const vec3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	constexpr float dot(const vec4& lhs, const vec4& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	constexpr vec3 cross(const vec3& lhs, const vec3& rhs)
	{
		return vec3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
			);
	}

	constexpr vec3 min_combine(const vec3& lhs, const vec3& rhs)
	{
		return vec3(
			lhs.x < rhs.x ? lhs.x : rhs.x,
			lhs.y < rhs.y ? lhs.y : rhs.y,
			lhs.z < rhs.z ? lhs.z : rhs.z
			);
	}

	constexpr vec3 max_combine(const vec3& lhs, const vec3& rhs)
	{
		return vec3(
			lhs.x > rhs.x ? lhs.x : rhs.x,
			lhs.y > rhs.y ? lhs.y : rhs.y,
			lhs.z > rhs.z ? lhs.z : rhs.z
			);
	}

	constexpr float det33(const vec3& row1, const vec3& row2, const vec3& row3)
	{
		return gml_impl::determinant(
			row1.x, row1.y, row1.z,
			row2.x, row2.y, row2.z,
			row3.x, row3.y, row3.z);
	}

	constexpr float det44(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4)
	{
		return gml_impl::determinant(
			row1.x, row1.y, row1.z, row1.w,
			row2.x, row2.y, row2.z, row2.w,
			row3.x, row3.y, row3.z, row3.w,
			row4.x, row4.y, row4.z, row4.w
			);
	}

	constexpr float det33_t(const vec3& row1, const vec3& row2, const vec3& row3)
	{
		return gml_impl::determinant(
			row1.x, row2.x, row3.x,
			row1.y, row2.y, row3.y,
			row1.z, row2.z, row3.z);
	}

	constexpr float det44_t(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4)
	{
		return gml_impl::determinant(
			row1.x, row2.y, row3.z, row4.w,
			row1.x, row2.y, row3.z, row4.w,
			row1.x, row2.y, row3.z, row4.w,
			row1.x, row2.y, row3.z, row4.w
			);
	}


	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec2& v2)
	{
		return vec2(
			gml_impl::Component(v2, SwizzleX::SwizzleIndex),
			gml_impl::Component(v2, SwizzleY::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec3& v3)
	{
		return vec2(
			gml_impl::Component(v3, SwizzleX::SwizzleIndex),
			gml_impl::Component(v3, SwizzleY::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY>
	constexpr vec2 swizzle(const vec4& v4)
	{
		return vec2(
			gml_impl::Component(v4, SwizzleX::SwizzleIndex),
			gml_impl::Component(v4, SwizzleY::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec2& v2)
	{
		return vec3(
			gml_impl::Component(v2, SwizzleX::SwizzleIndex),
			gml_impl::Component(v2, SwizzleY::SwizzleIndex),
			gml_impl::Component(v2, SwizzleZ::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec3& v3)
	{
		return vec3(
			gml_impl::Component(v3, SwizzleX::SwizzleIndex),
			gml_impl::Component(v3, SwizzleY::SwizzleIndex),
			gml_impl::Component(v3, SwizzleZ::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ>
	constexpr vec3 swizzle(const vec4& v4)
	{
		return vec3(
			gml_impl::Component(v4, SwizzleX::SwizzleIndex),
			gml_impl::Component(v4, SwizzleY::SwizzleIndex),
			gml_impl::Component(v4, SwizzleZ::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec2& v2)
	{
		return vec4(
			gml_impl::Component(v2, SwizzleX::SwizzleIndex),
			gml_impl::Component(v2, SwizzleY::SwizzleIndex),
			gml_impl::Component(v2, SwizzleZ::SwizzleIndex),
			gml_impl::Component(v2, SwizzleW::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec3& v3)
	{
		return vec4(
			gml_impl::Component(v3, SwizzleX::SwizzleIndex),
			gml_impl::Component(v3, SwizzleY::SwizzleIndex),
			gml_impl::Component(v3, SwizzleZ::SwizzleIndex),
			gml_impl::Component(v3, SwizzleW::SwizzleIndex));
	}

	template<typename SwizzleX, typename SwizzleY, typename SwizzleZ, typename SwizzleW>
	constexpr vec4 swizzle(const vec4& v4)
	{
		return vec4(
			gml_impl::Component(v4, SwizzleX::SwizzleIndex),
			gml_impl::Component(v4, SwizzleY::SwizzleIndex),
			gml_impl::Component(v4, SwizzleZ::SwizzleIndex),
			gml_impl::Component(v4, SwizzleW::SwizzleIndex));
	}
}

