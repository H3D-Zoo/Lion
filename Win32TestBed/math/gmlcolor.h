#pragma once
#include "gmlutility.h"
#include <cstdlib>
#include <cassert>

namespace gml
{
	class color3;
	class color4;

	class color3
	{
	public:
		static const color3& black();
		static const color3& gray();
		static const color3& white();
		static const color3& red();
		static const color3& green();
		static const color3& blue();
		static const color3& yellow();
		static const color3& cyan();
		static const color3& purple();
		static color3 random();
		constexpr static color3 from_rgba(unsigned int rgba);
		constexpr static color3 from_bgra(unsigned int bgra);

	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

	public:
		constexpr color3() = default;

		constexpr color3(float _r, float _g, float _b) : r(_r), g(_g), b(_b) { }

		constexpr explicit color3(const color4& c4);

		friend bool operator==(const color3& lhs, const color3& rhs);

		friend inline bool operator!=(const color3& lhs, const color3& rhs) { return !(lhs == rhs); }

		friend constexpr color3 operator+(const color3& lhs, const color3& rhs);

		friend constexpr color3 operator+(const color3& lhs, float rhs);

		friend constexpr color3 operator+(float lhs, const color3& rhs);

		friend constexpr color3 operator*(const color3& lhs, const color3& rhs);

		friend constexpr color3 operator*(const color3& lhs, float rhs);

		friend constexpr color3 operator*(float lhs, const color3& rhs);

		friend inline color3& operator+=(color3& lhs, const color3& rhs);

		friend inline color3& operator+=(color3& lhs, float rhs);

		friend inline color3& operator*=(color3& lhs, const color3& rhs);

		friend inline color3& operator*=(color3& lhs, float rhs);

		float& operator[](int index); //hack

		const float& operator[](int index) const;

		explicit operator float*(); //hack

		constexpr explicit operator const float*() const { return &(r); }

		void set(float _r, float _g, float _b);

		void set(const color4& c4);

		void clamp();

		color3 clamped() const;

		unsigned int rgba() const;

		unsigned int bgra() const;
	};

	class color4
	{
	public:
		static const color4& black();
		static const color4& gray();
		static const color4& white();
		static const color4& red();
		static const color4& green();
		static const color4& blue();
		static const color4& yellow();
		static const color4& cyan();
		static const color4& purple();
		static color4 random();
		constexpr static color4 from_rgba(unsigned int rgba);
		constexpr static color4 from_bgra(unsigned int bgra);

	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

	public:
		constexpr color4() = default;

		constexpr color4(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) { }

		constexpr color4(const color3& c3, float _a) : r(c3.r), g(c3.g), b(c3.b), a(_a) { }

		friend bool operator==(const color4& lhs, const color4& rhs);

		friend inline bool operator!=(const color4& lhs, const color4& rhs) { return !(lhs == rhs); }

		friend constexpr color4 operator+(const color4& lhs, const color4& rhs);

		friend constexpr color4 operator+(const color4& lhs, float rhs);

		friend constexpr color4 operator+(float lhs, const color4& rhs);

		friend constexpr color4 operator*(const color4& lhs, const color4& rhs);

		friend constexpr color4 operator*(const color4& lhs, float rhs);

		friend constexpr color4 operator*(float lhs, const color4& rhs);

		friend color4& operator+=(color4& lhs, const color4& rhs);

		friend color4& operator+=(color4& lhs, float rhs);

		friend color4& operator*=(color4& lhs, const color4& rhs);

		friend color4& operator*=(color4& lhs, float rhs);

		float& operator[](int index); //hack

		const float& operator[](int index) const;

		inline explicit operator float*();

		constexpr explicit operator const float*() const { return &(r); }

		void set(float _r, float _g, float _b, float _a);

		void replace(const color3& c3);

		void clamp();

		color4 clamped() const;

		unsigned int rgba() const;

		unsigned int bgra() const;
	};

	constexpr float dot(const color3& lhs, const color3& rhs);

	constexpr float dot(const color4& lhs, const color4& rhs);

	struct _0; struct _1;
	struct _R { constexpr static int SwizzleIndex = 0; };
	struct _G { constexpr static int SwizzleIndex = 1; };
	struct _B { constexpr static int SwizzleIndex = 2; };
	struct _A { constexpr static int SwizzleIndex = 3; };

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB>
	constexpr color3 swizzle(const color3& c3);

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB>
	constexpr color3 swizzle(const color4& c4);

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB, typename SwizzleA>
	constexpr color4 swizzle(const color3& c3);

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB, typename SwizzleA>
	constexpr color4 swizzle(const color4& c4);
}

namespace gml_impl
{
	constexpr unsigned int bgra(
		unsigned char b,
		unsigned char g,
		unsigned char r,
		unsigned char a)
	{
		return (b | (g << 8) | (r << 16) | (a << 24));
	}

	constexpr unsigned int rgba(
		unsigned char r,
		unsigned char g,
		unsigned char b,
		unsigned char a)
	{
		return (r | (g << 8) | (b << 16) | (a << 24));
	}

	constexpr float u2f_factor = 1.0f / 255.0f;
	constexpr float r2f_factor = 1.0f / RAND_MAX;
}

namespace gml
{
	inline const color3& color3::black()
	{
		static color3 c(0, 0, 0);
		return c;
	}
	inline const color3& color3::gray()
	{
		static color3 c(0.5f, 0.5f, 0.5f);
		return c;
	}
	inline const color3& color3::white()
	{
		static color3 c(1, 1, 1);
		return c;
	}
	inline const color3& color3::red()
	{
		static color3 c(1, 0, 0);
		return c;
	}
	inline const color3& color3::green()
	{
		static color3 c(0, 1, 0);
		return c;
	}
	inline const color3& color3::blue()
	{
		static color3 c(0, 0, 1);
		return c;
	}
	inline const color3& color3::yellow()
	{
		static color3 c(1, 1, 0);
		return c;
	}
	inline const color3& color3::cyan()
	{
		static color3 c(0, 1, 1);
		return c;
	}
	inline const color3& color3::purple()
	{
		static color3 c(1, 0, 1);
		return c;
	}
	inline color3 color3::random()
	{
		return color3(
			rand() * gml_impl::r2f_factor,
			rand() * gml_impl::r2f_factor,
			rand() * gml_impl::r2f_factor);
	}

	constexpr color3 color3::from_rgba(unsigned int rgba)
	{
		return color3(
			(rgba & 0xFF) * gml_impl::u2f_factor,
			(rgba >> 8 & 0xFF) * gml_impl::u2f_factor,
			(rgba >> 16 & 0xFF) * gml_impl::u2f_factor);
	}

	constexpr color3 color3::from_bgra(unsigned int bgra)
	{
		return color3(
			(bgra >> 16 & 0xFF) * gml_impl::u2f_factor,
			(bgra >> 8 & 0xFF) * gml_impl::u2f_factor,
			(bgra & 0xFF) * gml_impl::u2f_factor);
	}

	constexpr color3::color3(const color4& c4) : r(c4.r), g(c4.g), b(c4.b) { }

	inline bool operator==(const color3& lhs, const color3& rhs)
	{
		if (&rhs != &lhs)
		{
			return fequal(lhs.r, rhs.r) &&
				fequal(lhs.g, rhs.g) &&
				fequal(lhs.b, rhs.b);
		}
		else
		{
			return true;
		}
	}

	constexpr color3 operator+(const color3& lhs, const color3& rhs)
	{
		return color3(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
	}

	constexpr color3 operator+(const color3& lhs, float rhs)
	{
		return color3(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs);
	}

	constexpr color3 operator+(float lhs, const color3& rhs)
	{
		return rhs + lhs;
	}

	constexpr color3 operator*(const color3& lhs, const color3& rhs)
	{
		return color3(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);
	}

	constexpr color3 operator*(const color3& lhs, float rhs)
	{
		return color3(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
	}

	constexpr color3 operator*(float lhs, const color3& rhs)
	{
		return rhs * lhs;
	}

	inline color3& operator+=(color3& lhs, const color3& rhs)
	{
		lhs.r += rhs.r;
		lhs.g += rhs.g;
		lhs.b += rhs.b;
		return lhs;
	}

	inline color3& operator+=(color3& lhs, float rhs)
	{
		lhs.r += rhs;
		lhs.g += rhs;
		lhs.b += rhs;
		return lhs;
	}

	inline color3& operator*=(color3& lhs, const color3& rhs)
	{
		lhs.r *= rhs.r;
		lhs.g *= rhs.g;
		lhs.b *= rhs.b;
		return lhs;
	}

	inline color3& operator*=(color3& lhs, float rhs)
	{
		lhs.r *= rhs;
		lhs.g *= rhs;
		lhs.b *= rhs;
		return lhs;
	}

	inline const float& color3::operator[](int index) const
	{
		assert(index >= 0 && index < 3);
		return *(&r + index);
	}

	inline float& color3::operator[](int index)
	{
		return const_cast<float&>(const_cast<const color3*>(this)->operator[](index));
	}

	inline color3::operator float*()
	{
		return const_cast<float*>(const_cast<const color3*>(this)->operator const float*());
	}

	inline void color3::set(float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}

	inline void color3::set(const color4& c4)
	{
		set(c4.r, c4.g, c4.b);
	}

	inline void color3::clamp()
	{
		r = clamp01(r);
		g = clamp01(g);
		b = clamp01(b);
	}

	inline color3 color3::clamped() const
	{
		color3 copy(*this);
		copy.clamp();
		return copy;
	}

	inline unsigned int color3::rgba() const
	{
		unsigned char r255 = gml::clamp<unsigned char>(static_cast<unsigned char>(r * 255), 0, 255);
		unsigned char g255 = gml::clamp<unsigned char>(static_cast<unsigned char>(g * 255), 0, 255);
		unsigned char b255 = gml::clamp<unsigned char>(static_cast<unsigned char>(b * 255), 0, 255);
		return gml_impl::rgba(r255, g255, b255, 255);
	}

	inline unsigned int color3::bgra() const
	{
		unsigned char r255 = gml::clamp<unsigned char>(static_cast<unsigned char>(r * 255), 0, 255);
		unsigned char g255 = gml::clamp<unsigned char>(static_cast<unsigned char>(g * 255), 0, 255);
		unsigned char b255 = gml::clamp<unsigned char>(static_cast<unsigned char>(b * 255), 0, 255);
		return gml_impl::bgra(b255, g255, r255, 255);
	}

	inline const color4& color4::black()
	{
		static color4 c(0, 0, 0, 1);
		return c;
	}
	inline const color4& color4::gray()
	{
		static color4 c(0.5f, 0.5f, 0.5f, 1);
		return c;
	}
	inline const color4& color4::white()
	{
		static color4 c(1, 1, 1, 1);
		return c;
	}
	inline const color4& color4::red()
	{
		static color4 c(1, 0, 0, 1);
		return c;
	}
	inline const color4& color4::green()
	{
		static color4 c(0, 1, 0, 1);
		return c;
	}
	inline const color4& color4::blue()
	{
		static color4 c(0, 0, 1, 1);
		return c;
	}
	inline const color4& color4::yellow()
	{
		static color4 c(1, 1, 0, 1);
		return c;
	}
	inline const color4& color4::cyan()
	{
		static color4 c(0, 1, 1, 1);
		return c;
	}
	inline const color4& color4::purple()
	{
		static color4 c(1, 0, 1, 1);
		return c;
	}
	inline color4 color4::random()
	{
		return color4(
			rand() * gml_impl::r2f_factor,
			rand() * gml_impl::r2f_factor,
			rand() * gml_impl::r2f_factor,
			1.0f);
	}
	constexpr color4 color4::from_rgba(unsigned int rgba)
	{
		return color4(
			(rgba & 0xFF) * gml_impl::u2f_factor,
			(rgba >> 8 & 0xFF) * gml_impl::u2f_factor,
			(rgba >> 16 & 0xFF) * gml_impl::u2f_factor,
			(rgba >> 24 & 0xFF) * gml_impl::u2f_factor);
	}
	constexpr color4 color4::from_bgra(unsigned int bgra)
	{
		return color4(
			(bgra >> 16 & 0xFF) * gml_impl::u2f_factor,
			(bgra >> 8 & 0xFF) * gml_impl::u2f_factor,
			(bgra & 0xFF) * gml_impl::u2f_factor,
			(bgra >> 24 & 0xFF) * gml_impl::u2f_factor);
	}

	inline bool operator==(const color4& lhs, const color4& rhs)
	{
		if (&rhs != &lhs)
		{
			return fequal(lhs.r, rhs.r) &&
				fequal(lhs.g, rhs.g) &&
				fequal(lhs.b, rhs.b) &&
				fequal(lhs.a, rhs.a);
		}
		else
		{
			return true;
		}
	}

	constexpr color4 operator+(const color4& lhs, const color4& rhs)
	{
		return color4(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
	}

	constexpr color4 operator+(const color4& lhs, float rhs)
	{
		return color4(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a + rhs);
	}

	constexpr color4 operator+(float lhs, const color4& rhs)
	{
		return rhs + lhs;
	}

	constexpr color4 operator*(const color4& lhs, const color4& rhs)
	{
		return color4(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b, lhs.a * rhs.a);
	}

	constexpr color4 operator*(const color4& lhs, float rhs)
	{
		return color4(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
	}

	constexpr color4 operator*(float lhs, const color4& rhs)
	{
		return rhs * lhs;
	}

	inline color4& operator+=(color4& lhs, const color4& rhs)
	{
		lhs.r += rhs.r;
		lhs.g += rhs.g;
		lhs.b += rhs.b;
		lhs.a += rhs.a;
		return lhs;
	}

	inline color4& operator+=(color4& lhs, float rhs)
	{
		lhs.r += rhs;
		lhs.g += rhs;
		lhs.b += rhs;
		lhs.a += rhs;
		return lhs;
	}

	inline color4& operator*=(color4& lhs, const color4& rhs)
	{
		lhs.r *= rhs.r;
		lhs.g *= rhs.g;
		lhs.b *= rhs.b;
		lhs.a *= rhs.a;
		return lhs;
	}

	inline color4& operator*=(color4& lhs, float rhs)
	{
		lhs.r *= rhs;
		lhs.g *= rhs;
		lhs.b *= rhs;
		lhs.a *= rhs;
		return lhs;
	}

	inline float& color4::operator[](int index)
	{
		return const_cast<float&>(const_cast<const color4*>(this)->operator[](index));
	}

	inline const float& color4::operator[](int index) const
	{
		assert(index >= 0 && index < 4);
		return *(&r + index);
	}

	inline color4::operator float*()
	{
		return const_cast<float*>(const_cast<const color4*>(this)->operator const float*());
	}

	inline void color4::set(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline void color4::replace(const color3& c3)
	{
		r = c3.r;
		g = c3.g;
		b = c3.b;
	}

	inline void color4::clamp()
	{
		r = clamp01(r);
		g = clamp01(g);
		b = clamp01(b);
		a = clamp01(a);
	}

	inline color4 color4::clamped() const
	{
		color4 copy(*this);
		copy.clamp();
		return copy;
	}

	inline unsigned int color4::rgba() const
	{
		unsigned char r255 = gml::clamp<unsigned char>(static_cast<unsigned char>(r * 255), 0, 255);
		unsigned char g255 = gml::clamp<unsigned char>(static_cast<unsigned char>(g * 255), 0, 255);
		unsigned char b255 = gml::clamp<unsigned char>(static_cast<unsigned char>(b * 255), 0, 255);
		unsigned char a255 = gml::clamp<unsigned char>(static_cast<unsigned char>(a * 255), 0, 255);
		return gml_impl::rgba(r255, g255, b255, a255);
	}

	inline unsigned int color4::bgra() const
	{
		unsigned char r255 = gml::clamp<unsigned char>(static_cast<unsigned char>(r * 255), 0, 255);
		unsigned char g255 = gml::clamp<unsigned char>(static_cast<unsigned char>(g * 255), 0, 255);
		unsigned char b255 = gml::clamp<unsigned char>(static_cast<unsigned char>(b * 255), 0, 255);
		unsigned char a255 = gml::clamp<unsigned char>(static_cast<unsigned char>(a * 255), 0, 255);
		return gml_impl::bgra(b255, g255, r255, a255);
	}

	constexpr float dot(const color3& lhs, const color3& rhs)
	{
		return lhs.r * rhs.r + lhs.g * rhs.g + lhs.b * rhs.b;
	}

	constexpr float dot(const color4& lhs, const color4& rhs)
	{
		return lhs.r * rhs.r + lhs.g * rhs.g + lhs.b * rhs.b + lhs.a * rhs.a;
	}

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB>
	constexpr color3 swizzle(const color3& c3)
	{
		return color3(
			gml_impl::Component(c3, SwizzleR::SwizzleIndex),
			gml_impl::Component(c3, SwizzleG::SwizzleIndex),
			gml_impl::Component(c3, SwizzleB::SwizzleIndex));
	}

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB>
	constexpr color3 swizzle(const color4& c4)
	{
		return color3(
			gml_impl::Component(c4, SwizzleR::SwizzleIndex),
			gml_impl::Component(c4, SwizzleG::SwizzleIndex),
			gml_impl::Component(c4, SwizzleB::SwizzleIndex));
	}

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB, typename SwizzleA>
	constexpr color4 swizzle(const color3& c3)
	{
		return color4(
			gml_impl::Component(c3, SwizzleR::SwizzleIndex),
			gml_impl::Component(c3, SwizzleG::SwizzleIndex),
			gml_impl::Component(c3, SwizzleB::SwizzleIndex),
			gml_impl::Component(c3, SwizzleA::SwizzleIndex));
	}

	template<typename SwizzleR, typename SwizzleG, typename SwizzleB, typename SwizzleA>
	constexpr color4 swizzle(const color4& c4)
	{
		return color4(
			gml_impl::Component(c4, SwizzleR::SwizzleIndex),
			gml_impl::Component(c4, SwizzleG::SwizzleIndex),
			gml_impl::Component(c4, SwizzleB::SwizzleIndex),
			gml_impl::Component(c4, SwizzleA::SwizzleIndex));
	}
}