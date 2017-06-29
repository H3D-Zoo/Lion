#pragma once
#include "gmlutility.h"
#include "gmlrotation.h"
#include "gmlangle.h"

namespace gml
{
	class dquat
	{
	public:
		static const dquat& identity();

	public:
		quat real;
		quat dual;

	public:
		constexpr dquat() : real(1, 0, 0, 0), dual(0, 0, 0, 0) { };

		dquat(const quat& rotation, const vec3 translation);

		//rotate and translate
		dquat(const vec3& axis, const radian& r, const vec3& translation);

		//only rotate
		inline dquat(const vec3& axis, const radian& r) : real(axis, r), dual(0, 0, 0, 0) { }

		//only translation
		constexpr dquat(const vec3& translation) : real(1, 0, 0, 0), dual(0, translation * 0.5f) { }

		//only translation
		constexpr dquat(float x, float y, float z) : real(1, 0, 0, 0), dual(0, x*0.5f, y*0.5f, z*0.5f) { }

		constexpr quat get_rotation() const { return real; }

		constexpr vec3 get_translation() const { return (2.0f * dual * real.conjugated()).v; }

		radian get_rotate_radian() const;

		friend dquat operator+ (const dquat& lhs, const dquat& rhs);

		friend dquat operator* (const dquat& lhs, const dquat& rhs);

		friend dquat operator* (const dquat& lhs, float scaler);

		friend inline dquat operator*(float lhs, dquat& rhs) { return rhs * lhs; }

		friend dquat& operator+=(dquat& lhs, const dquat& rhs);

		friend dquat& operator*=(dquat& lhs, const dquat& rhs);

		friend dquat& operator*=(dquat& lhs, float rhs);

		void normalize();

		dquat normalized() const;

		void conjugate();

		dquat conjugated() const;

		void inverse();

		dquat inversed() const;

		void exponent(float t);

		dquat exponented(float t) const;

		inline float length() const { return sqrtf(length_sqr()); }

		constexpr float length_sqr() const { return dot(real, real); }

	public://unless you know what u r doing.
		constexpr dquat(const quat &r, const quat& d) : real(r), dual(d) { }
	};

	dquat add(const dquat& lhs, const dquat& rhs);

	void add_to(dquat& lhs, const dquat& rhs);

	constexpr float dot(const dquat& lhs, const dquat& rhs);

	dquat sc_lerp(const dquat& from, const dquat& to, float t);

	vec3 transform(const dquat& dq, const vec3& point);
}


namespace gml
{
	inline const dquat& dquat::identity()
	{
		static dquat dq;
		return dq;
	}

	inline dquat::dquat(const quat& rotation, const vec3 translation)
		: real(rotation)
		, dual(0, translation * 0.5f)
	{
		real.normalize();
		dual = dual * real;


	}

	//rotate and translate
	inline dquat::dquat(const vec3& axis, const radian& r, const vec3& translation)
		: real(axis, r)
		, dual(0, translation * 0.5f)
	{
		dual = dual * real;

	}

	inline radian dquat::get_rotate_radian() const
	{
		float w = frac(real.w);
		return radian(2 * acos(w));
	}

	inline dquat operator+ (const dquat& lhs, const dquat& rhs)
	{
		return dquat(lhs.real + rhs.real, lhs.dual + rhs.dual);
	}

	inline dquat operator* (const dquat& lhs, const dquat& rhs)
	{
		quat newR = lhs.real*rhs.real;
		quat newD = lhs.real*rhs.dual + lhs.dual*rhs.real; //noticing the order.
		return dquat(newR, newD);
	}

	inline dquat operator* (const dquat& lhs, float scaler)
	{
		quat newR = lhs.real*scaler;
		quat newD = lhs.dual*scaler;
		return dquat(newR, newD);
	}

	inline dquat& operator+=(dquat& lhs, const dquat& rhs)
	{
		lhs.real += rhs.real;
		lhs.dual += rhs.dual;
		return lhs;
	}

	inline dquat& operator*=(dquat& lhs, const dquat& rhs)
	{
		quat newR = lhs.real*rhs.real;
		quat newD = lhs.real*rhs.dual + lhs.dual*rhs.real; //noticing the order.
		lhs.real = newR;
		lhs.dual = newD;

		return lhs;
	}

	inline dquat& operator*=(dquat& lhs, float rhs)
	{
		lhs.real *= rhs;
		lhs.dual *= rhs;
		return lhs;
	}

	inline void dquat::normalize()
	{
		float l = length_sqr();
		if (!fequal(l, 0.0f))
		{
			if (!fequal(l, 1.0f))
			{
				auto invL = 1.0f / sqrtf(l);
				this->real *= invL;
				this->dual *= invL;
			}
		}
	}

	inline dquat dquat::normalized() const
	{
		dquat rst(*this);
		rst.normalize();
		return rst;
	}

	inline void dquat::conjugate()
	{
		real.conjugate();
		//q = -q && q.w =0;
		// => q.conjugate() = -q = q
		// so.... what....
		dual.conjugate();
	}

	inline dquat dquat::conjugated() const
	{
		dquat rst(*this);
		rst.conjugate();
		return rst;
	}

	inline void dquat::inverse()
	{
		normalize();
		conjugate();
	}

	inline dquat dquat::inversed() const
	{
		dquat rst(*this);
		rst.inverse();
		return rst;
	}

	inline void dquat::exponent(float t)
	{
		float invr = 1.0f / sqrtf(length_sqr());
		// change the pitch. //
		// Screw parameters
		radian r = get_rotate_radian();
		float pitch = dual.w * invr; //-2 * dual.w * invr
		vec3 direction = real.v * invr;
		vec3 moment = (dual.v + direction * pitch * real.w) * invr;	//(dual.v - direction * pitch * real.w * 0.5f) * invr

		// Exponential power 
		r *= t * 0.5f;
		pitch *= t;

		// Convert back to dual-quaternion
		float sinAngle = sin(r); // angle/2
		float cosAngle = cos(r); // angle/2

		real = quat(cosAngle, direction * sinAngle);

		//dual = quat(-pitch * sinAngle * 0.5f , sinAngle * moment + 0.5f * pitch * cosAngle * direction);
		dual = quat(pitch * sinAngle, sinAngle * moment - pitch * cosAngle * direction);
	}

	inline dquat dquat::exponented(float t) const
	{
		auto result(*this);
		result.exponent(t);
		return result;
	}

	inline dquat add(const dquat& lhs, const dquat& rhs)
	{
		if (dot(lhs.real, rhs.real) >= 0)
		{
			return dquat(lhs.real + rhs.real, lhs.dual + rhs.dual);
		}
		else
		{
			return dquat(lhs.real - rhs.real, lhs.dual - rhs.dual);
		}
	}

	inline void add_to(dquat& lhs, const dquat& rhs)
	{
		if (dot(lhs.real, rhs.real) >= 0)
		{
			lhs.real += rhs.real;
			lhs.dual += rhs.dual;
		}
		else
		{
			lhs.real -= rhs.real;
			lhs.dual -= rhs.dual;
		}

	}

	constexpr float dot(const dquat& lhs, const dquat& rhs)
	{
		return dot(lhs.real, rhs.real);
	}

	inline dquat sc_lerp(const dquat& from, const dquat& to, float t)
	{
		/* this code piece may cause an unexpected result,
		which the translation is oppsite from the right direction.

		// Shortest path
		float dotReal = dot(from.real, to.real);
		dquat to_n(to);
		if (dotReal < 0)
		{
			to_n.real = -to_n.real;
			to_n.dual = -to_n.dual;
		}
		*/

		// ScLERP = Qa * (Qa^-1 * Qb)^t 
		dquat diff = from.conjugated() * to;
		diff.exponent(t);
		return from * diff;
	}

	inline vec3 transform(const dquat& dq, const vec3& point)
	{
		dquat inv = dq.inversed();
		inv.dual = -inv.dual;

		dquat pt = dquat(quat::identity_pos(), quat(0, point));
		dquat rst = dq * pt * inv;
		return rst.dual.v;
	}
}