#pragma once
#include "gmlutility.h"
#include "gmlvector.h"
#include "gmlangle.h"
#include <cassert>

namespace gml
{
	class mat22;
	class mat32;
	class mat33;
	class mat44;

	class mat22
	{
		constexpr static const int ROW = 2;
		constexpr static const int COL = 2;
		constexpr static const int CELL = ROW * COL;

	public:
		static const mat22& identity();
		static mat22 rotate(const radian& r);
		constexpr static mat22 scale(float scaler);
		constexpr static mat22 scale(float sx, float sy);
		constexpr static mat22 flip_x();
		constexpr static mat22 flip_y();

	public:
		union
		{
			float m[ROW][COL];
			struct { float _00, _01, _10, _11; };
			struct { vec2 row[ROW]; };
		};

		constexpr mat22();

		constexpr mat22(float _m00, float _m01, float _m10, float _m11);

		constexpr mat22(const vec2& row1, const vec2& row2);

		constexpr mat22(const mat22&);

		mat22& operator=(const mat22& rhs);

		friend bool operator== (const mat22& lhs, const mat22& rhs);

		friend inline bool operator!= (const mat22& lhs, const mat22& rhs) { return !(lhs == rhs); }

		friend mat22 operator* (const mat22& lhs, const mat22& rhs);

		friend mat22 operator* (const mat22& lhs, float rhs);

		friend inline mat22 operator* (float lhs, const mat22& rhs) { return rhs * lhs; }

		friend vec2 operator* (const mat22& lhs, const vec2& rhs);

		friend vec2 operator* (const vec2& lhs, const mat22& rhs);

		friend mat22& operator*= (mat22& lhs, const mat22& rhs);

		friend mat22& operator*= (mat22& lhs, float rhs);

		float& operator[] (int index);//hack

		const float& operator[] (int index) const;

		vec2 column(int index) const;

		void set_column(int index, vec2 v);

		inline void set_identity() { *this = identity(); }

		void transpose();

		constexpr mat22 transposed() const;

		bool can_invert() const;

		void inverse();

		mat22 inversed() const;

		bool is_orthogonal() const;

		constexpr float determinant() const;
	};

	class mat32
	{
		constexpr static const int ROW = 2;
		constexpr static const int COL = 3;
		constexpr static const int CELL = ROW*COL;
	public:
		static const mat32& identity();
		static mat32 rotate(const radian& r);
		constexpr static mat32 scale(float scaler);
		constexpr static mat32 scale(float sx, float sy);
		constexpr static mat32 translate(float x, float y);
		constexpr static mat32 flip_x();
		constexpr static mat32 flip_y();
		static mat32 inv_trs(const vec2& translate, const radian& rotation, const vec2& scale);
		static mat32 trs(const vec2& translate, const radian& rotation, const vec2& scale);
		static mat32 trsp(const vec2& translate, const radian& rotation, const vec2& scale, const vec2& pivot);
		static mat32 trps(const vec2& translate, const radian& rotation, const vec2& pivot, const vec2& scale);

	public:
		union
		{
			float m[ROW][COL];
			struct { float _00, _01, _02, _10, _11, _12; };
			vec3 row[ROW];
		};

		constexpr mat32();

		constexpr mat32(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12);

		constexpr mat32(const vec3& row1, const vec3& row2);

		constexpr mat32(const mat32& m);

		constexpr explicit mat32(const mat22& m);

		mat32& operator=(const mat32& rhs);

		friend bool operator== (const mat32& lhs, const mat32& rhs);

		friend inline bool operator!= (const mat32& lhs, const mat32& rhs) { return !(lhs == rhs); }

		friend mat32 operator* (const mat32& lhs, const mat32& rhs);

		friend vec3 operator* (const mat32& lhs, const vec3& rhs);

		friend vec3 operator* (const vec3& lhs, const mat32& rhs);

		friend mat32& operator*= (mat32& lhs, const mat32& rhs);

		friend mat32& operator*= (mat32& lhs, float rhs);

		float& operator[] (int index);//hack

		const float& operator[] (int index) const;

		vec2 column(int index) const;

		vec3 column3(int index) const;

		void set_column(int index, vec2 v);

		inline void set_identity() { *this = identity(); }

		constexpr mat22 get_rotate_matrix22() const;

		constexpr mat32 get_rotate_matrix() const;

		inline vec2 get_local_translation() const;
	};

	class mat33
	{
		constexpr static const int ROW = 3;
		constexpr static const int COL = 3;
		constexpr static const int CELL = ROW*COL;
	public:
		static const mat33& identity();
		static mat33 rotate_x(const radian& r);
		static mat33 rotate_y(const radian& r);
		static mat33 rotate_z(const radian& r);
		constexpr static mat33 scale(float scaler);
		constexpr static mat33 scale(float sx, float sy, float sz);
		constexpr static mat33 flip_x();
		constexpr static mat33 flip_y();
		constexpr static mat33 flip_z();

	public:
		union
		{
			float m[ROW][COL];
			struct { float _00, _01, _02, _10, _11, _12, _20, _21, _22; };
			struct { vec3 row[ROW]; };
		};

		constexpr mat33();

		constexpr mat33(
			float _m00, float _m01, float _m02,
			float _m10, float _m11, float _m12,
			float _m20, float _m21, float _m22);

		constexpr mat33(const vec3& row1, const vec3& row2, const vec3& row3);

		constexpr mat33(const mat33& m);

		constexpr explicit mat33(const mat22& m);

		constexpr explicit mat33(const mat32& m);

		mat33& operator=(const mat33& rhs);

		friend bool operator== (const mat33& lhs, const mat33& rhs);

		friend inline bool operator!= (const mat33& lhs, const mat33& rhs) { return !(lhs == rhs); }

		friend mat33 operator* (const mat33& lhs, const mat33& rhs);

		friend mat33 operator* (const mat33& lhs, float rhs);

		friend inline mat33 operator* (float lhs, const mat33& rhs) { return rhs * lhs; }

		friend vec3 operator* (const mat33& lhs, const vec3& rhs);

		friend vec3 operator* (const vec3& lhs, const mat33& rhs);

		friend mat33& operator*= (mat33& lhs, const mat33& rhs);

		friend mat33& operator*= (mat33& lhs, float rhs);

		float& operator[] (int index);//hack

		const float& operator[] (int index) const;

		vec2 column2(int index) const;

		vec3 column(int index) const;

		void set_column(int index, vec3 v);

		inline void set_identity() { *this = identity(); }

		void transpose();

		constexpr mat33 transposed() const;

		bool can_invert() const;

		void inverse();

		mat33 inversed() const;

		bool is_orthogonal() const;

		constexpr float determinant() const;

		constexpr mat22 get_rotate_matrix22() const;

		constexpr mat33 get_rotate_matrix() const;

		inline vec2 get_local_translation() const;
	};

	class mat44
	{
		constexpr static const int ROW = 4;
		constexpr static const int COL = 4;
		constexpr static const int CELL = ROW*COL;
	public:
		static const mat44& identity();
		static mat44 rotate_x(const radian& r);
		static mat44 rotate_y(const radian& r);
		static mat44 rotate_z(const radian& r);
		constexpr static mat44 scale(float scale);
		constexpr static mat44 scale(float sx, float sy, float sz);
		constexpr static mat44 translate(float x, float y, float z);
		constexpr static mat44 flip_x();
		constexpr static mat44 flip_y();
		constexpr static mat44 flip_z();
		static mat44 look_at(const vec3& eye, const vec3& look, const vec3& up);
		static mat44 perspective_lh(const radian& fov, float aspect, float znear, float zfar);
		static mat44 ortho2d_lh(float width, float height, float znear, float zfar);
		static mat44 ortho2d_ui(float width, float height, float znear, float zfar);

	public:
		union
		{
			float m[ROW][COL];
			struct { float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
			struct { vec4 row[ROW]; };
		};

		constexpr mat44();

		constexpr mat44(
			float _m00, float _m01, float _m02, float _m03,
			float _m10, float _m11, float _m12, float _m13,
			float _m20, float _m21, float _m22, float _m23,
			float _m30, float _m31, float _m32, float _m33);

		constexpr mat44(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4);

		constexpr mat44(const mat44& m);

		constexpr explicit mat44(const mat22& m22);

		constexpr explicit mat44(const mat32& m32);

		constexpr explicit mat44(const mat33& m33);

		mat44& operator=(const mat44& rhs);

		friend bool operator== (const mat44& lhs, const mat44& rhs);

		friend inline bool operator!= (const mat44& lhs, const mat44& rhs) { return !(lhs == rhs); }

		friend mat44 operator* (const mat44& lhs, const mat44& rhs);

		friend mat44 operator* (const mat44& lhs, float rhs);

		friend inline mat44 operator* (float lhs, const mat44& rhs) { return rhs * lhs; }

		friend vec4 operator* (const mat44& lhs, const vec4& rhs);

		friend vec4 operator* (const vec4& lhs, const mat44& rhs);

		friend mat44& operator*= (mat44& lhs, const mat44& rhs);

		friend mat44& operator*= (mat44& lhs, float rhs);

		float& operator[] (int index);//hack

		const float& operator[] (int index) const;

		vec3 column3(int index) const;

		vec4 column(int index) const;

		void set_column(int index, vec4 v);

		inline void set_identity() { *this = identity(); }

		void transpose();

		constexpr mat44 transposed() const;

		bool can_invert() const;

		void inverse();

		mat44 inversed() const;

		bool is_orthogonal() const;

		constexpr float determinant() const;

		constexpr mat33 get_rotate_matrix33() const;

		constexpr mat44 get_rotate_matrix() const;

		vec3 get_local_translation() const;
	};

	vec2 transform_vector(const mat32& lhs, const vec2& rhs);

	vec2 transform_vector(const mat33& lhs, const vec2& rhs);

	vec3 transform_vector(const mat44& lhs, const vec3& rhs);

	vec2 transform_point(const mat32& lhs, const vec2& rhs);

	vec2 transform_point(const mat33& lhs, const vec2& rhs);

	vec3 transform_point(const mat44& lhs, const vec3& rhs);
}

namespace gml
{
	constexpr mat22::mat22(float _m00, float _m01, float _m10, float _m11)
		: _00(_m00), _01(_m01)
		, _10(_m10), _11(_m11) { }

	constexpr mat22::mat22() : mat22(
		1.0f, 0.0f,
		0.0f, 1.0f) { }

	constexpr mat22::mat22(const vec2& row1, const vec2& row2)
		: _00(row1.x), _01(row1.y)
		, _10(row2.x), _11(row2.y) {}

	constexpr mat22::mat22(const mat22& m)
		: _00(m._00), _01(m._01)
		, _10(m._10), _11(m._11) { }

	inline mat22& mat22::operator=(const mat22& rhs)
	{
		if (&rhs != this)
		{
			for (int i = 0; i < ROW; i++)
			{
				row[i] = rhs.row[i];
			}
		}
		return *this;
	}

	inline bool operator== (const mat22& lhs, const mat22& rhs)
	{
		if (&rhs != &lhs)
		{
			for (int i = 0; i < mat22::ROW; i++)
			{
				if (lhs.row[i] != rhs.row[i])
					return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}

	inline mat22 operator* (const mat22& lhs, const mat22& rhs)
	{
		mat22 result;
		for (int i = 0; i < mat22::ROW; i++)
		{
			for (int j = 0; j < mat22::COL; j++)
			{
				result.m[i][j] = dot(lhs.row[i], rhs.column(j));
			}
		}
		return result;
	}

	inline mat22 operator* (const mat22& lhs, float rhs)
	{
		mat22 result = lhs;
		for (int i = 0; i < mat22::ROW; i++)
		{
			result.row[i] *= rhs;
		}
		return result;
	}

	inline vec2 operator* (const mat22& lhs, const vec2& rhs)
	{
		vec2 result;
		for (int i = 0; i < mat22::ROW; i++)
		{
			result[i] = dot(lhs.row[i], rhs);
		}
		return result;
	}

	inline vec2 operator* (const vec2& lhs, const mat22& rhs)
	{
		vec2 result;
		for (int i = 0; i < mat22::ROW; i++)
		{
			result[i] = dot(lhs, rhs.column(i));
		}
		return result;
	}


	inline mat22& operator*= (mat22& lhs, float rhs)
	{
		for (int i = 0; i < mat22::ROW; i++)
		{
			lhs.row[i] *= rhs;
		}
		return lhs;
	}

	inline mat22& operator*= (mat22& lhs, const mat22& rhs)
	{
		return (lhs = lhs * rhs);
	}

	inline float& mat22::operator[] (int index)
	{
		return const_cast<float&>(const_cast<const mat22*>(this)->operator[](index));
	}

	inline const float& mat22::operator[] (int index) const
	{
		assert(index >= 0 && index < CELL);
		return *(&(m[0][0]) + index);
	}

	inline vec2 mat22::column(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec2(m[0][index], m[1][index]);
	}

	inline void mat22::set_column(int index, vec2 v)
	{
		assert(index >= 0 && index < COL);

		for (int i = 0; i < ROW; i++)
		{
			m[i][index] = v[i];
		}
	}

	inline void mat22::transpose()
	{
		swap(this->_10, this->_01);
	}

	constexpr mat22 mat22::transposed() const
	{
		return mat22(
			_00, _10,
			_01, _11);
	}

	inline bool mat22::can_invert() const
	{
		if (is_orthogonal())
		{
			return true;
		}
		else
		{
			return !fequal(determinant(), 0.0f);
		}
	}

	inline void mat22::inverse()
	{
		if (is_orthogonal())
		{
			transpose();
		}
		else
		{
			float det = determinant();
			if (!fequal(det, 0.0f))
			{
				//calc adjoint matrix 
				swap(this->m[0][0], this->m[1][1]);
				this->m[0][1] = -this->m[0][1];
				this->m[1][0] = -this->m[1][0];

				*this *= 1.0f / det;
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
			}
		}
	}

	inline mat22 mat22::inversed() const
	{
		mat22 result(*this);
		result.inverse();
		return result;
	}

	inline bool mat22::is_orthogonal() const
	{
		for (int i = 0; i < ROW; i++)
		{
			if (!fequal(row[i].length_sqr(), 1.0f))
			{
				return false;
			}

			for (int j = i + 1; j < ROW; j++)
			{
				float dotRow = dot(row[i], row[j]);
				if (!fequal(dotRow, 0.0f))
				{
					return false;
				}
			}
		}
		return true;
	}

	constexpr float mat22::determinant() const
	{
		return gml_impl::determinant(
			_00, _01,
			_10, _11);
	}

	constexpr mat32::mat32(float _m00, float _m01, float _m02, float _m10, float _m11, float _m12)
		: _00(_m00), _01(_m01), _02(_m02)
		, _10(_m10), _11(_m11), _12(_m12) { }

	constexpr mat32::mat32() : mat32(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f) { }

	constexpr mat32::mat32(const vec3& row1, const vec3& row2)
		: _00(row1.x), _01(row1.y), _02(row1.z)
		, _10(row2.x), _11(row2.y), _12(row2.z) { }

	constexpr mat32::mat32(const mat32& m)
		: _00(m._00), _01(m._01), _02(m._02)
		, _10(m._10), _11(m._11), _12(m._12) { }

	constexpr mat32::mat32(const mat22& m)
		: _00(m._00), _01(m._01), _02(0.0f)
		, _10(m._10), _11(m._11), _12(0.0f) { }

	inline mat32& mat32::operator=(const mat32& rhs)
	{
		if (&rhs != this)
		{
			for (int i = 0; i < ROW; i++)
			{
				row[i] = rhs.row[i];
			}
		}
		return *this;
	}

	inline bool operator== (const mat32& lhs, const mat32& rhs)
	{
		if (&rhs != &lhs)
		{
			for (int i = 0; i < mat32::ROW; i++)
			{
				if (lhs.row[i] != rhs.row[i])
					return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}

	inline mat32 operator* (const mat32& lhs, const mat32& rhs)
	{
		mat32 result;
		for (int i = 0; i < mat32::ROW; i++)
		{
			for (int j = 0; j < mat32::COL; j++)
			{
				result.m[i][j] = dot(lhs.row[i], rhs.column3(j));
			}
		}
		return result;
	}

	inline vec3 operator* (const mat32& lhs, const vec3& rhs)
	{
		vec3 result;
		for (int i = 0; i < mat32::ROW; i++)
		{
			result[i] = dot(lhs.row[i], rhs);
		}
		return result;
	}

	inline vec3 operator* (const vec3& lhs, const mat32& rhs)
	{
		vec3 result;
		for (int i = 0; i < mat32::ROW; i++)
		{
			result[i] = dot(lhs, rhs.column3(i));
		}
		return result;
	}

	inline mat32& operator*= (mat32& lhs, float rhs)
	{
		for (int i = 0; i < mat32::ROW; i++)
		{
			lhs.row[i] *= rhs;
		}
		return lhs;
	}

	inline mat32& operator*= (mat32& lhs, const mat32& rhs)
	{
		return (lhs = lhs * rhs);
	}

	inline float& mat32::operator[] (int index)
	{
		return const_cast<float&>(const_cast<const mat32*>(this)->operator[](index));
	}

	inline const float& mat32::operator[] (int index) const
	{
		assert(index >= 0 && index < CELL);
		return *(&(m[0][0]) + index);
	}

	inline vec2 mat32::column(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec2(m[0][index], m[1][index]);
	}

	inline vec3 mat32::column3(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec3(m[0][index], m[1][index], (index == COL - 1) ? 1.0f : 0.0f);
	}

	inline void mat32::set_column(int index, vec2 v)
	{
		assert(index >= 0 && index < COL);

		for (int i = 0; i < ROW; i++)
		{
			m[i][index] = v[i];
		}
	}

	constexpr mat22 mat32::get_rotate_matrix22() const
	{
		return mat22(
			_00, _01,
			_10, _11);
	}

	constexpr mat32 mat32::get_rotate_matrix() const
	{
		return mat32(
			_00, _01, 0.0f,
			_10, _11, 0.0f);
	}

	inline vec2 mat32::get_local_translation() const
	{
		vec2 T = column(2);
		return vec2(
			dot(T, column(0)),
			dot(T, column(1)));
	}

	constexpr mat33::mat33(
		float _m00, float _m01, float _m02,
		float _m10, float _m11, float _m12,
		float _m20, float _m21, float _m22)
		: _00(_m00), _01(_m01), _02(_m02)
		, _10(_m10), _11(_m11), _12(_m12)
		, _20(_m20), _21(_m21), _22(_m22) { }

	constexpr mat33::mat33() : mat33(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f) { }

	constexpr mat33::mat33(const vec3& row1, const vec3& row2, const vec3& row3)
		: _00(row1.x), _01(row1.y), _02(row1.z)
		, _10(row2.x), _11(row2.y), _12(row2.z)
		, _20(row3.x), _21(row3.y), _22(row3.z) { }

	constexpr mat33::mat33(const mat33& m)
		: _00(m._00), _01(m._01), _02(m._02)
		, _10(m._10), _11(m._11), _12(m._12)
		, _20(m._20), _21(m._21), _22(m._22) { }

	constexpr mat33::mat33(const mat22& m)
		: _00(m._00), _01(m._01), _02(0.0f)
		, _10(m._10), _11(m._11), _12(0.0f)
		, _20(0.0f), _21(0.0f), _22(1.0f) { }

	constexpr mat33::mat33(const mat32& m)
		: _00(m._00), _01(m._01), _02(m._02)
		, _10(m._10), _11(m._11), _12(m._12)
		, _20(0.0f), _21(0.0f), _22(1.0f) { }

	inline mat33& mat33::operator=(const mat33& rhs)
	{
		if (&rhs != this)
		{
			for (int i = 0; i < ROW; i++)
			{
				row[i] = rhs.row[i];
			}
		}
		return *this;
	}

	inline bool operator== (const mat33& lhs, const mat33& rhs)
	{
		if (&rhs != &lhs)
		{
			for (int i = 0; i < mat33::ROW; i++)
			{
				if (lhs.row[i] != rhs.row[i])
					return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}

	inline mat33 operator* (const mat33& lhs, const mat33& rhs)
	{
		mat33 result;
		for (int i = 0; i < mat33::ROW; i++)
		{
			for (int j = 0; j < mat33::COL; j++)
			{
				result.m[i][j] = dot(lhs.row[i], rhs.column(j));
			}
		}
		return result;
	}

	inline mat33 operator* (const mat33& lhs, float rhs)
	{
		mat33 result = lhs;
		for (int i = 0; i < mat33::ROW; i++)
		{
			result.row[i] *= rhs;
		}
		return result;
	}

	inline vec3 operator* (const mat33& lhs, const vec3& rhs)
	{
		vec3 result;
		for (int i = 0; i < mat33::ROW; i++)
		{
			result[i] = dot(lhs.row[i], rhs);
		}
		return result;
	}

	inline vec3 operator* (const vec3& lhs, const mat33& rhs)
	{
		vec3 result;
		for (int i = 0; i < mat33::ROW; i++)
		{
			result[i] = dot(lhs, rhs.column(i));
		}
		return result;
	}

	inline mat33& operator*= (mat33& lhs, float rhs)
	{
		for (int i = 0; i < mat33::ROW; i++)
		{
			lhs.row[i] *= rhs;
		}
		return lhs;
	}

	inline mat33& operator*= (mat33& lhs, const mat33& rhs)
	{
		return (lhs = lhs * rhs);
	}

	inline float& mat33::operator[] (int index)
	{
		return const_cast<float&>(const_cast<const mat33*>(this)->operator[](index));
	}

	inline const float& mat33::operator[] (int index) const
	{
		assert(index >= 0 && index < CELL);
		return *(&(m[0][0]) + index);
	}

	inline vec2 mat33::column2(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec2(m[0][index], m[1][index]);
	}

	inline vec3 mat33::column(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec3(m[0][index], m[1][index], m[2][index]);
	}

	inline void mat33::set_column(int index, vec3 v)
	{
		assert(index >= 0 && index < COL);

		for (int i = 0; i < ROW; i++)
		{
			m[i][index] = v[i];
		}
	}

	inline void mat33::transpose()
	{
		swap(this->_10, this->_01);
		swap(this->_20, this->_02);
		swap(this->_21, this->_12);
	}

	constexpr mat33 mat33::transposed() const
	{
		return mat33(
			_00, _10, _20,
			_01, _11, _21,
			_02, _12, _22
		);
	}

	inline bool mat33::can_invert() const
	{
		if (is_orthogonal())
		{
			return !fequal(determinant(), 0.0f);
		}
		else
		{
			return true;
		}
	}

	inline void mat33::inverse()
	{
		if (is_orthogonal())
		{
			transpose();
		}
		else
		{
			float det = determinant();
			if (!fequal(det, 0.0f))
			{
				det = 1.0f / det;
				mat33 copy(*this);

				//to-do det_impl is calculated above in determinant().
				//try to gcd
				m[0][0] = +gml_impl::determinant(copy._11, copy._12, copy._21, copy._22) * det;
				m[1][0] = -gml_impl::determinant(copy._10, copy._12, copy._20, copy._22) * det;
				m[2][0] = +gml_impl::determinant(copy._10, copy._11, copy._20, copy._21) * det;

				m[0][1] = -gml_impl::determinant(copy._01, copy._02, copy._21, copy._22) * det;
				m[1][1] = +gml_impl::determinant(copy._00, copy._02, copy._20, copy._22) * det;
				m[2][1] = -gml_impl::determinant(copy._00, copy._01, copy._20, copy._21) * det;

				m[0][2] = +gml_impl::determinant(copy._01, copy._02, copy._11, copy._12) * det;
				m[1][2] = -gml_impl::determinant(copy._00, copy._02, copy._10, copy._12) * det;
				m[2][2] = +gml_impl::determinant(copy._00, copy._01, copy._10, copy._11) * det;
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
			}
		}
	}

	inline mat33 mat33::inversed() const
	{
		mat33 result(*this);
		result.inverse();
		return result;
	}

	inline bool mat33::is_orthogonal() const
	{
		for (int i = 0; i < ROW; i++)
		{
			if (!fequal(row[i].length_sqr(), 1.0f))
			{
				return false;
			}

			for (int j = i + 1; j < ROW; j++)
			{
				float dotRow = dot(row[i], row[j]);
				if (!fequal(dotRow, 0.0f))
				{
					return false;
				}
			}
		}
		return true;
	}

	constexpr float mat33::determinant() const
	{
		return gml_impl::determinant(
			_00, _01, _02,
			_10, _11, _12,
			_20, _21, _22);
	}

	constexpr mat22 mat33::get_rotate_matrix22() const
	{
		return mat22(
			_00, _01,
			_10, _11);
	}

	constexpr mat33 mat33::get_rotate_matrix() const
	{
		return mat33(
			_00, _01, 0.0f,
			_10, _11, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	inline vec2 mat33::get_local_translation() const
	{
		vec2 T = column2(2);
		return vec2(
			dot(T, column2(0)),
			dot(T, column2(1)));
	}

	constexpr mat44::mat44(
		float _m00, float _m01, float _m02, float _m03,
		float _m10, float _m11, float _m12, float _m13,
		float _m20, float _m21, float _m22, float _m23,
		float _m30, float _m31, float _m32, float _m33)
		: _00(_m00), _01(_m01), _02(_m02), _03(_m03)
		, _10(_m10), _11(_m11), _12(_m12), _13(_m13)
		, _20(_m20), _21(_m21), _22(_m22), _23(_m23)
		, _30(_m30), _31(_m31), _32(_m32), _33(_m33) { }

	constexpr mat44::mat44() : mat44(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f) { }

	constexpr mat44::mat44(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4)
		: _00(row1.x), _01(row1.y), _02(row1.z), _03(row1.w)
		, _10(row2.x), _11(row2.y), _12(row2.z), _13(row2.w)
		, _20(row3.x), _21(row3.y), _22(row3.z), _23(row3.w)
		, _30(row4.x), _31(row4.y), _32(row4.z), _33(row4.w) { }

	constexpr mat44::mat44(const mat44& m)
		: _00(m._00), _01(m._01), _02(m._02), _03(m._03)
		, _10(m._10), _11(m._11), _12(m._12), _13(m._13)
		, _20(m._20), _21(m._21), _22(m._22), _23(m._23)
		, _30(m._30), _31(m._31), _32(m._32), _33(m._33) { }

	constexpr mat44::mat44(const mat22& m22)
		: _00(m22._00), _01(m22._01), _02(0.0f), _03(0.0f)
		, _10(m22._10), _11(m22._11), _12(0.0f), _13(0.0f)
		, _20(0.0f), _21(0.0f), _22(1.0f), _23(0.0f)
		, _30(0.0f), _31(0.0f), _32(0.0f), _33(1.0f) { }

	constexpr mat44::mat44(const mat32& m32)
		: _00(m32._00), _01(m32._01), _02(0.0f), _03(m32._02)
		, _10(m32._10), _11(m32._11), _12(0.0f), _13(m32._12)
		, _20(0.0f), _21(0.0f), _22(1.0f), _23(0.0f)
		, _30(0.0f), _31(0.0f), _32(0.0f), _33(1.0f) { }

	constexpr mat44::mat44(const mat33& mat3)
		: _00(mat3._00), _01(mat3._01), _02(mat3._02), _03(0.0f)
		, _10(mat3._10), _11(mat3._11), _12(mat3._12), _13(0.0f)
		, _20(mat3._20), _21(mat3._21), _22(mat3._22), _23(0.0f)
		, _30(0.0f), _31(0.0f), _32(0.0f), _33(1.0f) { }

	inline mat44& mat44::operator=(const mat44& rhs)
	{
		if (&rhs != this)
		{
			for (int i = 0; i < ROW; i++)
			{
				row[i] = rhs.row[i];
			}
		}
		return *this;
	}

	inline bool operator== (const mat44& lhs, const mat44& rhs)
	{
		if (&rhs != &lhs)
		{
			for (int i = 0; i < mat44::ROW; i++)
			{
				if (lhs.row[i] != rhs.row[i])
					return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}

	inline mat44 operator* (const mat44& lhs, const mat44& rhs)
	{
		mat44 result;
		for (int i = 0; i < mat44::ROW; i++)
		{
			for (int j = 0; j < mat44::COL; j++)
			{
				result.m[i][j] = dot(lhs.row[i], rhs.column(j));
			}
		}
		return result;
	}

	inline mat44 operator* (const mat44& lhs, float rhs)
	{
		mat44 result = lhs;
		for (int i = 0; i < mat44::ROW; i++)
		{
			result.row[i] *= rhs;
		}
		return result;
	}

	inline vec4 operator* (const mat44& lhs, const vec4& rhs)
	{
		vec4 result;
		for (int i = 0; i < mat44::ROW; i++)
		{
			result[i] = dot(lhs.row[i], rhs);
		}
		return result;
	}

	inline vec4 operator* (const vec4& lhs, const mat44& rhs)
	{
		vec4 result;
		for (int i = 0; i < mat44::ROW; i++)
		{
			result[i] = dot(lhs, rhs.column(i));
		}
		return result;
	}

	inline mat44& operator*= (mat44& lhs, float rhs)
	{
		for (int i = 0; i < mat44::ROW; i++)
		{
			lhs.row[i] *= rhs;
		}
		return lhs;
	}

	inline mat44& operator*= (mat44& lhs, const mat44& rhs)
	{
		return (lhs = lhs * rhs);
	}

	inline float& mat44::operator[] (int index)
	{
		return const_cast<float&>(const_cast<const mat44*>(this)->operator[](index));
	}

	inline const float& mat44::operator[] (int index) const
	{
		assert(index >= 0 && index < CELL);
		return *(&(m[0][0]) + index);
	}

	inline vec3 mat44::column3(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec3(m[0][index], m[1][index], m[2][index]);
	}

	inline vec4 mat44::column(int index) const
	{
		assert(index >= 0 && index < COL);
		return vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
	}

	inline void mat44::set_column(int index, vec4 v)
	{
		assert(index >= 0 && index < COL);

		for (int i = 0; i < ROW; i++)
		{
			m[i][index] = v[i];
		}
	}

	inline void mat44::transpose()
	{
		swap(this->_10, this->_01);
		swap(this->_20, this->_02);
		swap(this->_30, this->_03);

		swap(this->_21, this->_12);
		swap(this->_31, this->_13);

		swap(this->_32, this->_23);
	}

	constexpr mat44 mat44::transposed() const
	{
		return mat44(
			_00, _10, _20, _30,
			_01, _11, _21, _31,
			_02, _12, _22, _32,
			_03, _13, _23, _33
		);
	}

	inline bool mat44::can_invert() const
	{
		if (is_orthogonal())
		{
			return true;
		}
		else
		{
			return !fequal(determinant(), 0.0f);
		}
	}

	inline void mat44::inverse()
	{
		if (is_orthogonal())
		{
			transpose();
		}
		else
		{
			float det = determinant();
			if (!fequal(det, 0.0f))
			{
				det = 1.0f / det;
				mat44 copy(*this);

				//to-do det_impl is calculated above in determinant().
				//try to gcd
				m[0][0] = +gml_impl::determinant(copy._11, copy._12, copy._13, copy._21, copy._22, copy._23, copy._31, copy._32, copy._33) * det;
				m[1][0] = -gml_impl::determinant(copy._10, copy._12, copy._13, copy._20, copy._22, copy._23, copy._30, copy._32, copy._33) * det;
				m[2][0] = +gml_impl::determinant(copy._10, copy._11, copy._13, copy._20, copy._21, copy._23, copy._30, copy._31, copy._33) * det;
				m[3][0] = -gml_impl::determinant(copy._10, copy._11, copy._12, copy._20, copy._21, copy._22, copy._30, copy._31, copy._32) * det;

				m[0][1] = -gml_impl::determinant(copy._01, copy._02, copy._03, copy._21, copy._22, copy._23, copy._31, copy._32, copy._33) * det;
				m[1][1] = +gml_impl::determinant(copy._00, copy._02, copy._03, copy._20, copy._22, copy._23, copy._30, copy._32, copy._33) * det;
				m[2][1] = -gml_impl::determinant(copy._00, copy._01, copy._03, copy._20, copy._21, copy._23, copy._30, copy._31, copy._33) * det;
				m[3][1] = +gml_impl::determinant(copy._00, copy._01, copy._02, copy._20, copy._21, copy._22, copy._30, copy._31, copy._32) * det;

				m[0][2] = +gml_impl::determinant(copy._01, copy._02, copy._03, copy._11, copy._12, copy._13, copy._31, copy._32, copy._33) * det;
				m[1][2] = -gml_impl::determinant(copy._00, copy._02, copy._03, copy._10, copy._12, copy._13, copy._30, copy._32, copy._33) * det;
				m[2][2] = +gml_impl::determinant(copy._00, copy._01, copy._03, copy._10, copy._11, copy._13, copy._30, copy._31, copy._33) * det;
				m[3][2] = -gml_impl::determinant(copy._00, copy._01, copy._02, copy._10, copy._11, copy._12, copy._30, copy._31, copy._32) * det;

				m[0][3] = -gml_impl::determinant(copy._01, copy._02, copy._03, copy._11, copy._12, copy._13, copy._21, copy._22, copy._23) * det;
				m[1][3] = +gml_impl::determinant(copy._00, copy._02, copy._03, copy._10, copy._12, copy._13, copy._20, copy._22, copy._23) * det;
				m[2][3] = -gml_impl::determinant(copy._00, copy._01, copy._03, copy._10, copy._11, copy._13, copy._20, copy._21, copy._23) * det;
				m[3][3] = +gml_impl::determinant(copy._00, copy._01, copy._02, copy._10, copy._11, copy._12, copy._20, copy._21, copy._22) * det;
			}
			else
			{
				//TODO:
				// can not invert here,
				// what should i do when this occur ?
			}
		}
	}

	inline mat44 mat44::inversed() const
	{
		mat44 result(*this);
		result.inverse();
		return result;
	}

	inline bool mat44::is_orthogonal() const
	{
		for (int i = 0; i < ROW; i++)
		{
			if (!fequal(row[i].length_sqr(), 1.0f))
			{
				return false;
			}

			for (int j = i + 1; j < ROW; j++)
			{
				float dotRow = dot(row[i], row[j]);
				if (!fequal(dotRow, 0.0f))
				{
					return false;
				}
			}
		}
		return true;
	}

	constexpr float mat44::determinant() const
	{
		return gml_impl::determinant(
			_00, _01, _02, _03,
			_10, _11, _12, _13,
			_20, _21, _22, _23,
			_30, _31, _32, _33);
	}

	constexpr mat33 mat44::get_rotate_matrix33() const
	{
		return mat33(
			_00, _01, _02,
			_10, _11, _12,
			_20, _21, _22);
	}

	constexpr mat44 mat44::get_rotate_matrix() const
	{
		return mat44(
			_00, _01, _02, 0.0f,
			_10, _11, _12, 0.0f,
			_20, _21, _22, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline vec3 mat44::get_local_translation() const
	{
		vec3 T = column3(3);
		return vec3(
			dot(T, column3(0)),
			dot(T, column3(1)),
			dot(T, column3(2)));
	}

	inline vec2 transform_vector(const mat32& lhs, const vec2& rhs)
	{
		vec3 rhs3(rhs, 0.0f);
		return vec2(
			dot(lhs.row[0], rhs3),
			dot(lhs.row[1], rhs3));
	}

	inline vec2 transform_vector(const mat33& lhs, const vec2& rhs)
	{
		vec3 rhs3(rhs, 0.0f);
		return vec2(
			dot(lhs.row[0], rhs3),
			dot(lhs.row[1], rhs3));
	}

	inline vec3 transform_vector(const mat44& lhs, const vec3& rhs)
	{
		vec4 rhs4(rhs, 0.0f);
		return vec3(
			dot(lhs.row[0], rhs4),
			dot(lhs.row[1], rhs4),
			dot(lhs.row[2], rhs4));
	}

	inline vec2 transform_point(const mat32& lhs, const vec2& rhs)
	{
		vec3 rhs3(rhs, 1.0f);
		return vec2(
			dot(lhs.row[0], rhs3),
			dot(lhs.row[1], rhs3));
	}

	inline vec2 transform_point(const mat33& lhs, const vec2& rhs)
	{
		vec3 rhs3(rhs, 1.0f);
		return vec2(
			dot(lhs.row[0], rhs3),
			dot(lhs.row[1], rhs3));
	}

	inline vec3 transform_point(const mat44& lhs, const vec3& rhs)
	{
		vec4 rhs4(rhs, 1.0f);
		return vec3(
			dot(lhs.row[0], rhs4),
			dot(lhs.row[1], rhs4),
			dot(lhs.row[2], rhs4));
	}

	inline const mat22& mat22::identity()
	{
		static mat22 i(
			1.0f, 0.0f,
			0.0f, 1.0f
		);
		return i;
	}

	inline mat22 mat22::rotate(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);
		return mat22(
			cosr, -sinr,
			sinr, cosr);
	}

	constexpr mat22 mat22::scale(float scaler)
	{
		return mat22(
			scaler, 0.0f,
			0.0f, scaler);
	}

	constexpr mat22 mat22::scale(float sx, float sy)
	{
		return mat22(
			sx, 0.0f,
			0.0f, sy);
	}

	constexpr mat22 mat22::flip_x()
	{
		return mat22(
			-1.0f, 0.0f,
			0.0f, 1.0f);
	}

	constexpr mat22 mat22::flip_y()
	{
		return mat22(
			1.0f, 0.0f,
			0.0f, -1.0f);
	}

	inline const mat32& mat32::identity()
	{
		static mat32 i(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		);
		return i;
	}

	inline mat32 mat32::rotate(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);
		return mat32(
			cosr, -sinr, 0.0f,
			sinr, cosr, 0.0f);
	}

	constexpr mat32 mat32::scale(float scaler)
	{
		return mat32(
			scaler, 0.0f, 0.0f,
			0.0f, scaler, 0.0f);
	}

	constexpr mat32 mat32::scale(float sx, float sy)
	{
		return mat32(
			sx, 0.0f, 0.0f,
			0.0f, sy, 0.0f);
	}

	constexpr mat32 mat32::translate(float x, float y)
	{
		return mat32(
			1.0f, 0.0f, x,
			0.0f, 1.0f, y);
	}

	constexpr mat32 mat32::flip_x()
	{
		return mat32(
			-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
	}

	constexpr mat32 mat32::flip_y()
	{
		return mat32(
			1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f);
	}

	inline mat32 mat32::inv_trs(const vec2& translate, const radian& rotation, const vec2& scale)
	{
		/*
		1/sx,0,0 * +cos,sin,0 * 1,0,-tx
		0,1/sy,0   -sin,cos,0   0,1,-ty

	=   1/sx,0,0 * +cos,sin,-cos*tx-sin*ty
		0,1/sy,0	 -sin,cos,+sin*tx-cos*ty

	=   +1/sx*cos, 1/sx*sin, -1/sx*(cos*tx+sin*ty)
		-1/sy*sin, 1/sy*cos, +1/sy*(sin*tx-cos*ty)
		*/
		float cosr = cos(rotation);
		float sinr = sin(rotation);

		float inv_sx = 1.0f / scale.x;
		float inv_sy = 1.0f / scale.y;


		return mat32(
			+inv_sx*cosr, inv_sx*sinr, -inv_sx*(cosr*translate.x - sinr*translate.y),
			-inv_sy*sinr, inv_sy*cosr, +inv_sy*(sinr*translate.x - cosr*translate.y)
		);
	}

	inline mat32 mat32::trs(const vec2& translate, const radian& rotation, const vec2& scale)
	{
		/*
		1,0,tx * cos,-sin,0 * sx,0,0
		0,1,ty   sin, cos,0   0,sy,0

	=   cos,-sin,tx * sx,0,0
		sin,+cos,ty   0,sy,0

	=   cos*sx,-sin*sy, tx
		sin*sx, cos*sy, ty
		*/
		float cosr = cos(rotation);
		float sinr = sin(rotation);

		return mat32(
			cosr*scale.x, -sinr*scale.y, translate.x,
			sinr*scale.x, +cosr*scale.y, translate.y
		);
	}

	inline mat32 mat32::trsp(const vec2& translate, const radian& rotation, const vec2& scale, const vec2& pivot)
	{
		/*
		1,0,tx * cos,-sin,0 * sx,0,0 * 1,0,-px
		0,1,ty   sin, cos,0   0,sy,0   0,1,-py

	=	cos,-sin,tx * sx,0,-sx*px
		sin,+cos,ty   0,sy,-sy*py

	=	cos*sx,-sin*sy, -cos*sx*px+sin*sy*px + tx
		sin*sx, cos*sy, -sin*sx*px-cos*sy*py + ty
		*/
		float cosr = cos(rotation);
		float sinr = sin(rotation);
		float spx = -scale.x*pivot.x;
		float spy = -scale.y*pivot.y;

		return mat32(
			cosr*scale.x, -sinr*scale.y, cosr*spx - sinr*spy + translate.x,
			sinr*scale.x, +cosr*scale.y, sinr*spx + cosr*spy + translate.y
		);
	}

	inline mat32 mat32::trps(const vec2& translate, const radian& rotation, const vec2& pivot, const vec2& scale)
	{
		/*
		1,0,tx * cos,-sin,0 * 1,0,-px * sx,0,0
		0,1,ty   sin, cos,0   0,1,-px   0,sy,0

	=	cos,-sin,ty * sx,0,-px
		sin, cos,ty   0,sy,-px

	=	cos*sx,-sin*sy, -cos*px+sin*px + tx
		sin*sx,+cos*sy, -sin*px-cos*py + ty
		*/
		float cosr = cos(rotation);
		float sinr = sin(rotation);

		return mat32(
			cosr*scale.x, -sinr*scale.y, -cosr*pivot.x + sinr*pivot.y + translate.x,
			sinr*scale.x, +cosr*scale.y, -sinr*pivot.x - cosr*pivot.y + translate.y
		);
	}

	inline const mat33& mat33::identity()
	{
		static mat33 i(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
		return i;
	}

	inline mat33 mat33::rotate_x(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat33(
			1.0f, 0.0f, 0.0f,
			0.0f, cosr, -sinr,
			0.0f, sinr, cosr
		);
	}

	inline mat33 mat33::rotate_y(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat33(
			cosr, 0.0f, -sinr,
			0.0f, 1.0f, 0.0f,
			sinr, 0.0f, cosr
		);
	}

	inline mat33 mat33::rotate_z(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat33(
			cosr, -sinr, 0.0f,
			sinr, cosr, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat33 mat33::scale(float scaler)
	{
		return mat33(
			scaler, 0.0f, 0.0f,
			0.0f, scaler, 0.0f,
			0.0f, 0.0f, scaler
		);
	}

	constexpr mat33 mat33::scale(float sx, float sy, float sz)
	{
		return mat33(
			sx, 0.0f, 0.0f,
			0.0f, sy, 0.0f,
			0.0f, 0.0f, sz
		);
	}

	constexpr mat33 mat33::flip_x()
	{
		return mat33(
			-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat33 mat33::flip_y()
	{
		return mat33(
			1.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat33 mat33::flip_z()
	{
		return mat33(
			1, 0, 0,
			0, 1, 0,
			0, 0, -1
		);
	}

	inline const mat44& mat44::identity()
	{
		static mat44 i(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		return i;
	}

	inline mat44 mat44::rotate_x(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat44(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosr, -sinr, 0.0f,
			0.0f, sinr, cosr, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	inline mat44 mat44::rotate_y(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat44(
			cosr, 0.0f, -sinr, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinr, 0.0f, cosr, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	inline mat44 mat44::rotate_z(const radian& r)
	{
		float cosr = gml::cos(r);
		float sinr = gml::sin(r);

		return mat44(
			cosr, -sinr, 0.0f, 0.0f,
			sinr, cosr, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::scale(float scale)
	{
		return mat44(
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::scale(float sx, float sy, float sz)
	{
		return mat44(
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::translate(float x, float y, float z)
	{
		return mat44(
			1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::flip_x()
	{
		return mat44(
			-1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::flip_y()
	{
		return mat44(
			1, 0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	constexpr mat44 mat44::flip_z()
	{
		return mat44(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	inline mat44 mat44::look_at(const vec3& eye, const vec3& look, const vec3& up)
	{
		vec3 forward = (look - eye).normalized();
		vec3 real_up = (up - forward * dot(forward, up.normalized())).normalized();
		vec3 right = cross(real_up, forward).normalized();
		return mat44(
			vec4(right, -dot(eye, right)),
			vec4(real_up, -dot(eye, real_up)),
			vec4(forward, -dot(eye, forward)),
			vec4(0, 0, 0, 1)
		);
	}

	inline mat44 mat44::perspective_lh(const radian& fov, float aspect, float znear, float zfar)
	{
		float nearTop = tanf(fov.value * 0.5f);
		float nearRight = nearTop * aspect;
		float zRangeInv = 1.0f / (zfar - znear);
		zRangeInv *= zfar;
		return mat44(
			1.0f / nearRight, 0, 0, 0,
			0, 1.0f / nearTop, 0, 0,
			0, 0, zRangeInv, -znear * zRangeInv,
			0, 0, 1, 0);
	}

	inline mat44 mat44::ortho2d_lh(float width, float height, float znear, float zfar)
	{
		float zRangeInv = 1.0f / (zfar - znear);
		return mat44(
			2.0f / width, 0, 0, 0,
			0, 2.0f / height, 0, 0,
			0, 0, zRangeInv, -znear * zRangeInv,
			0, 0, 0, 1);
	}

	inline mat44 mat44::ortho2d_ui(float width, float height, float znear, float zfar)
	{
		float zRangeInv = 1.0f / (zfar - znear);
		return mat44(
			2.0f / width, 0, 0, -1,
			0, -2.0f / height, 0, 1,
			0, 0, zRangeInv, -znear *zRangeInv,
			0, 0, 0, 1);
	}
}