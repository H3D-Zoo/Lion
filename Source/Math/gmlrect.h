#pragma once
#include "gmlutility.h"

namespace gml
{
	class coord
	{
	public:
		int x = 0;
		int y = 0;

	public:
		constexpr coord() = default;

		constexpr coord(int _x, int _y) :x(_x), y(_y) { }

		friend bool operator==(const coord& lhs, const coord& rhs);

		friend inline bool operator!=(const coord& lhs, const coord& rhs) { return !(lhs == rhs); }

		void set(int _x, int _y);

		void move(int offsetx, int offsety);
	};

	class rect
	{
	public:
		constexpr rect() = default;

		friend bool operator==(const rect& lhs, const rect& rhs);

		friend inline bool operator!=(const rect& lhs, const rect& rhs) { return !(lhs == rhs); }

		constexpr int left() const { return m_pos.x; }

		constexpr int right() const { return left() + width(); }

		constexpr int top() const { return m_pos.y; }

		constexpr int bottom() const { return  top() + height(); }

		constexpr int width() const { return m_size.x; }

		constexpr int height() const { return m_size.y; }

		constexpr coord center() const;

		constexpr coord position() const { return m_pos; }

		constexpr coord size() const { return m_size; }

		inline void set_pos(int x, int y) { m_pos.set(x, y); }

		inline void set_pos(const coord& pos) { m_pos = pos; }

		inline void set_width(int w) { m_size.x = w > 0 ? w : 0; }

		inline void set_height(int h) { m_size.y = h > 0 ? h : 0; }

		inline void set_size(int w, int h) { m_size.set(w, h); }

		inline void set_size(const coord& size) { m_size = size; }

		constexpr bool contains(int x, int y) const;

		constexpr bool contains(const coord& point) const;

		it_mode hit_test(const rect& other) const;

		void move(int offsetx, int offsety);

		void enlarge(int offsetw, int offseth);

	private:
		coord m_pos;
		coord m_size;
	};
}

namespace gml_impl
{
	inline int get_contain_count(const gml::rect& container, const gml::rect& to_check)
	{
		int inside_count = 0;
		if (container.contains(to_check.left(), to_check.top()))		inside_count++;
		if (container.contains(to_check.left(), to_check.bottom()))		inside_count++;
		if (container.contains(to_check.right(), to_check.top()))		inside_count++;
		if (container.contains(to_check.right(), to_check.bottom()))	inside_count++;
		return inside_count;
	}
}

namespace gml
{
	inline bool operator==(const coord& lhs, const coord& rhs)
	{
		if (&lhs != &rhs)
		{
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
		else
		{
			return true;
		}
	}

	inline void coord::set(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	inline void coord::move(int offsetx, int offsety)
	{
		set(x + offsetx, y + offsety);
	}

	inline bool operator==(const rect& lhs, const rect& rhs)
	{
		if (&lhs != &rhs)
		{
			return lhs.m_pos == rhs.m_pos && lhs.m_size == rhs.m_size;
		}
		else
		{
			return true;
		}
	}

	constexpr coord rect::center() const
	{
		return coord(
			(left() + right()) / 2,
			(top() + bottom()) / 2
			);
	}

	constexpr bool rect::contains(int x, int y) const
	{
		return !(x < left() || x > right() || y < top() || y > bottom());
	}

	constexpr bool rect::contains(const coord& point) const
	{
		return contains(point.x, point.y);
	}

	inline it_mode rect::hit_test(const rect& other) const
	{
		if (*this == other)
		{
			return it_mode::same;
		}

		int contain_count = gml_impl::get_contain_count(other, *this);
		if (contain_count == 4)
		{
			return it_mode::inside;
		}
		else if (contain_count > 0)
		{
			return it_mode::hit;
		}
		else
		{
			contain_count = gml_impl::get_contain_count(*this, other);
			if (contain_count == 4)
			{
				return it_mode::contain;
			}
			else if (contain_count > 0)
			{
				return it_mode::hit;
			}
			else
			{
				return it_mode::none;
			}
		}
	}

	inline void rect::move(int offsetx, int offsety)
	{
		m_pos.move(offsetx, offsety);
	}

	inline void rect::enlarge(int offsetw, int offseth)
	{
		m_size.move(offsetw, offseth);
		if (m_size.x < 0) m_size.x = 0;
		if (m_size.y < 0) m_size.y = 0;
	}
}