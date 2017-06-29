#pragma once

namespace gml
{
	enum class it_mode
	{
		none,
		hit,
		contain,
		inside,
		same,
	};

	struct _1 { constexpr static const int SwizzleIndex = -2; };
	struct _0 { constexpr static const int SwizzleIndex = -1; };

	constexpr double PI_d = 3.14159265358979323846;

	constexpr double PI2_d = PI_d * 2;

	constexpr float PI = static_cast<float>(PI_d);

	constexpr float PI2 = static_cast<float>(PI2_d);

	bool fequal(float lhs, float rhs);

	template<typename T>
	constexpr T lerp(T left, T right, float fac);

	template<typename T>
	void swap(T& lhs, T& rhs);

	template<typename T>
	T clamp(T value, T minValue, T maxValue);

	template<typename T>
	T clamp01(T value);

	template<class T>
	constexpr T exp(T n, unsigned int iexp);

	constexpr float frac(float);
}

namespace gml_impl
{
	int get_fexp_base2(float d);

	constexpr float determinant(
		float a00, float a01,
		float a10, float a11);

	constexpr float determinant(
		float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22);

	constexpr float determinant(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);

	template<typename T>
	constexpr float Component(const T& v, int index);

	constexpr int EPSILON_Base2 = -22;
}

namespace gml
{

	inline bool fequal(float lhs, float rhs)
	{
		if (lhs != rhs)
		{
			auto exp_diff = gml_impl::get_fexp_base2(lhs - rhs);
			if (exp_diff > gml_impl::EPSILON_Base2)//epsilon
			{
				exp_diff -= gml_impl::EPSILON_Base2;
				auto exp_lhs = gml_impl::get_fexp_base2(lhs);
				auto exp_rhs = gml_impl::get_fexp_base2(rhs);
				return exp_lhs > exp_diff && exp_rhs > exp_diff;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	template<typename T>
	constexpr T lerp(T left, T right, float fac)
	{
		return left * (1.0f - fac) + right * fac;
	}

	template<typename T>
	inline void swap(T& lhs, T& rhs)
	{
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	template<typename T>
	inline T clamp(T value, T minValue, T maxValue)
	{
		if (value < minValue)
			return minValue;
		else if (value > maxValue)
			return maxValue;
		else return value;
	}

	template<typename T>
	inline T clamp01(T value)
	{
		return clamp(value, T(0), T(1));
	}

	template<class T>
	constexpr T exp(T n, unsigned int iexp)
	{
		return iexp == 0 ? T(1) : (iexp == 1 ? n : exp(n, iexp - 1) * n);
	}

	constexpr float frac(float f)
	{
		return f - (int)f;
	}
}

namespace gml_impl
{
	inline int get_fexp_base2(float d)
	{
		auto binary = reinterpret_cast<short*>(&d);
		short exp_bits = (binary[1] & 0x7FC0);
		return (exp_bits >> 7) - 127;
	}

	template<typename T>
	constexpr float Component(const T& v, int index)
	{
		return (index == gml::_0::SwizzleIndex) ? 0 :
			(index == gml::_1::SwizzleIndex ? 1 : v[index]);
	}

	constexpr float determinant(
		float a00, float a01,
		float a10, float a11)
	{
		return a00 * a11 - a01 * a10;
	}

	constexpr float determinant(
		float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22)
	{
		return a00 * determinant(a11, a12, a21, a22)
			- a01 * determinant(a10, a12, a20, a22)
			+ a02 * determinant(a10, a11, a20, a21);
	}

	constexpr float determinant(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33)
	{
		return a00 * determinant(a11, a12, a13, a21, a22, a23, a31, a32, a33)
			- a01 * determinant(a10, a12, a13, a20, a22, a23, a30, a32, a33)
			+ a02 * determinant(a10, a11, a13, a20, a21, a23, a30, a31, a33)
			- a03 * determinant(a10, a11, a12, a20, a21, a22, a30, a31, a32);
	}
}