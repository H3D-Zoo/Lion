#pragma once
#include "gmlutility.h"
#include "gmlvector.h"

namespace gml
{
	template<typename T>
	class aabb_t
	{
	public:
		constexpr aabb_t();

		aabb_t(const T& min_bound, const T& max_bound);

		friend bool operator==(const aabb_t<T>& lhs, const aabb_t<T>& rhs);

		friend inline bool operator!=(const aabb_t<T>& lhs, const aabb_t<T>& rhs) { return !(lhs == rhs); }

		void set(const T& min_bound, const T& max_bound);

		constexpr const T& center() const { return m_center; }

		constexpr const T& min_bound() const { return m_min_bound; }

		constexpr const T& max_bound() const { return m_max_bound; }

		constexpr const T& extend() const { return m_extend; }

		constexpr bool is_empty() const { return m_is_empty; }

		constexpr bool is_point() const { return m_is_point; }

		it_mode is_intersect(const aabb_t<T>& other) const;

		bool contains(const aabb_t<T>& other) const;

		bool contains(const T& point) const;

		void expand(const T& point);

		void expand(const aabb_t<T>& other);

		void scaling(float scaler);

		void scaling(const T& scaler);

		void move(const T& offset);

		void move_to(const T& point);

		void empty();

	private:
		void test_is_point();
		bool m_is_empty;
		bool m_is_point;
		T m_min_bound;
		T m_max_bound;
		T m_center;
		T m_extend;

	public:
		inline friend bool operator==(const aabb_t<T>& lhs, const aabb_t<T>& rhs)
		{
			if (&lhs == &rhs ) return true;
			if (lhs.is_empty() && rhs.is_empty()) return true;
			if (lhs.is_empty() || rhs.is_empty()) return false;

			return lhs.m_min_bound == rhs.m_min_bound
				&& lhs.m_max_bound == rhs.m_max_bound;
		}
	};

	typedef aabb_t<vec2> aabb2d;
	typedef aabb_t<vec3> aabb;
}

namespace gml
{
	template<typename T>
	constexpr aabb_t<T>::aabb_t()
		: m_is_empty(true)
		, m_is_point(true)
		, m_min_bound()
		, m_max_bound()
		, m_center()
		, m_extend() { }

	template<typename T>
	inline aabb_t<T>::aabb_t(const T& min_bound, const T& max_bound)
	{
		set(min_bound, max_bound);
	}

	template<typename T>
	inline void aabb_t<T>::set(const T& min_bound, const T& max_bound)
	{
		m_min_bound = min_combine(min_bound, max_bound);
		m_max_bound = max_combine(min_bound, max_bound);
		m_center = (m_min_bound + m_max_bound) * 0.5f;
		m_extend = m_center - m_min_bound;
		m_is_empty = false;
		test_is_point();
	}

	template<typename T>
	inline it_mode aabb_t<T>::is_intersect(const aabb_t& other) const
	{
		if (*this == other)
		{
			return it_mode::same;
		}
		else if (is_point() && other.is_point())
		{
			return (center() == other.center())? it_mode::same : it_mode::none;
		}
		else if (!is_empty() && !other.is_empty())
		{
			int minless = 0;
			int maxless = 0;
			for (int i = 0; i < T::DIMENSION; i++)
			{
				if (m_min_bound[i] < other.m_min_bound[i])
					minless++;
				if (m_max_bound[i] < other.m_max_bound[i])
					maxless++;

				if (m_min_bound[i] > other.m_max_bound[i] || m_max_bound[i] < other.m_min_bound[i])
				{
					return it_mode::none;
				}

			}

			if (minless == T::DIMENSION && maxless == 0)
			{
				return it_mode::contain;
			}
			else if (minless == 0 && maxless == T::DIMENSION)
			{
				return it_mode::inside;
			}
			else
			{
				return it_mode::hit;
			}
		}
		else
		{
			return it_mode::none;
		}
	}

	template<typename T>
	inline bool aabb_t<T>::contains(const aabb_t<T>& other) const
	{
		if (is_empty() || other.is_empty())
		{
			return false;
		}
		else if (is_point())
		{
			if (other.is_point())
			{
				return center() == other.center();
			}
			else
			{

				return false;
			}
		}
		else
		{
			for (int i = 0; i < T::DIMENSION; i++)
			{
				if (m_min_bound[i] > other.m_min_bound[i]
					|| m_max_bound[i] < other.m_max_bound[i])
					return false;
			}
			return true;
		}
	}

	template<typename T>
	inline bool aabb_t<T>::contains(const T& point) const
	{
		if (is_empty())
		{
			return false;
		}
		else if (is_point())
		{
			return center() == point;
		}
		else
		{
			for (int i = 0; i < T::DIMENSION; i++)
			{
				if (m_min_bound[i] > point[i] || m_max_bound[i] < point[i])
					return false;
			}
			return true;
		}
	}

	template<typename T>
	inline void aabb_t<T>::expand(const T& point)
	{
		if (is_empty())
		{
			m_min_bound = m_max_bound = point;
			m_is_empty = false;
		}
		else
		{
			m_min_bound = min_combine(m_min_bound, point);
			m_max_bound = max_combine(m_max_bound, point);
		}

		m_center = (m_min_bound + m_max_bound) * 0.5f;
		m_extend = m_center - m_min_bound;
		test_is_point();
	}

	template<typename T>
	inline void aabb_t<T>::expand(const aabb_t<T>& other)
	{
		if (other.is_empty())
		{
			return;
		}

		if (is_point())
		{
			*this = other;
		}
		else
		{
			m_min_bound = min_combine(m_min_bound, other.m_min_bound);
			m_max_bound = max_combine(m_max_bound, other.m_max_bound);
			m_center = (m_min_bound + m_max_bound) * 0.5f;
			m_extend = m_center - m_min_bound;
			test_is_point();
		}
	}

	template<typename T>
	inline void aabb_t<T>::empty()
	{
		m_min_bound = T::zero();
		m_max_bound = T::zero();
		m_center = T::zero();
		m_extend = T::zero();
		m_is_empty = true;
		m_is_point = true;
	}

	template<typename T>
	inline void aabb_t<T>::scaling(float scaler)
	{
		if (!is_empty())
		{
			m_extend *= scaler;
			m_min_bound = m_center - m_extend;
			m_max_bound = m_center + m_extend;
		}
	}

	template<typename T>
	inline void aabb_t<T>::scaling(const T& scaler)
	{
		if (!is_empty())
		{
			m_extend *= scaler;
			m_min_bound = m_center - m_extend;
			m_max_bound = m_center + m_extend;
		}
	}

	template<typename T>
	inline void aabb_t<T>::move(const T& offset)
	{
		if (!is_empty())
		{
			m_min_bound += offset;
			m_max_bound += offset;
			m_center += offset;
		}
	}

	template<typename T>
	inline void aabb_t<T>::move_to(const T& point)
	{
		if (!is_empty())
		{
			auto offset = point - m_center;
			m_min_bound += offset;
			m_max_bound += offset;
			m_center = point;
		}
	}

	template<class T>
	inline void aabb_t<T>::test_is_point()
	{
		for (int i = 0; i < T::DIMENSION; i++)
		{
			if (fequal(m_min_bound[i], m_max_bound[i]))
			{
				m_is_point = true;
				return;
			}
		}
		m_is_point = false;
	}
}