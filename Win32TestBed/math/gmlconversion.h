#pragma once
#include "gmlaabb.h"
#include "gmlmatrix.h"
#include "gmlrotation.h"
#include "gmldualquat.h"

namespace gml
{
	mat33 to_mat33_from(const quat& q);

	mat44 to_mat44_from(const quat& q);

	mat44 to_mat44_from(const dquat& q);

	quat to_quat_from(const mat33& mat);

	quat to_quat_from(const mat44& mat);

	dquat to_dquat_from(const mat33& m33);

	dquat to_dquat_from(const mat44& m44);

	aabb transform(const mat44& mat, const aabb& inaabb);

	aabb2d transform(const mat32& mat, const aabb2d& inaabb2d);
}

namespace gml
{
	inline mat33 to_mat33_from(const quat& q)
	{
		mat33 rst;
		vec3 vsqr2 = q.v * q.v * 2;
		float xy2 = q.v.x * q.v.y * 2;
		float yz2 = q.v.y * q.v.z * 2;
		float zx2 = q.v.z * q.v.x * 2;
		float wx2 = q.w * q.v.x * 2;
		float wy2 = q.w * q.v.y * 2;
		float wz2 = q.w * q.v.z * 2;

		rst._00 = 1 - vsqr2.y - vsqr2.z;
		rst._11 = 1 - vsqr2.z - vsqr2.x;
		rst._22 = 1 - vsqr2.x - vsqr2.y;

		rst._01 = xy2 - wz2;
		rst._02 = zx2 + wy2;

		rst._10 = xy2 + wz2;
		rst._12 = yz2 - wx2;

		rst._20 = zx2 - wy2;
		rst._21 = yz2 + wx2;

		return rst;
	}

	inline mat44 to_mat44_from(const quat& q)
	{
		return (mat44)to_mat33_from(q);
	}

	inline mat44 to_mat44_from(const dquat& q)
	{
		mat44 rst;
		auto normalDQ = q.normalized();

#define M(r,c) rst.m[r][c]

		// Extract rotational information
		float w = normalDQ.real.w;
		float x = normalDQ.real.v.x;
		float y = normalDQ.real.v.y;
		float z = normalDQ.real.v.z;

		float w2 = w * w;
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		float xy2 = x * y * 2;
		float yz2 = y * z * 2;
		float xz2 = x * z * 2;
		float wx2 = w * x * 2;
		float wy2 = w * y * 2;
		float wz2 = w * z * 2;

		M(0, 0) = w2 + x2 - y2 - z2;
		M(1, 0) = xy2 + wz2;
		M(2, 0) = xz2 - wy2;

		M(0, 1) = xy2 - wz2;
		M(1, 1) = w2 + y2 - x2 - z2;
		M(2, 1) = yz2 + wx2;

		M(0, 2) = xz2 + wy2;
		M(1, 2) = yz2 - wx2;
		M(2, 2) = w2 + z2 - x2 - y2;

		// Extract translation information
		vec3 translation = normalDQ.get_translation();
		M(0, 3) = translation.x;
		M(1, 3) = translation.y;
		M(2, 3) = translation.z;
#undef M

		rst.row[3].set(0, 0, 0, 1);

		return rst;
	}

	inline quat to_quat_impl(
		float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22)
	{
		float wsqr = _00 + _11 + _22;
		float xsqr = _00 - _11 - _22;
		float ysqr = _11 - _00 - _22;
		float zsqr = _22 - _00 - _11;

		int maxIndex = 0;
		float maxSqr = wsqr;
		if (xsqr > maxSqr)
		{
			maxSqr = xsqr;
			maxIndex = 1;
		}
		if (ysqr > maxSqr)
		{
			maxSqr = ysqr;
			maxIndex = 2;
		}
		if (zsqr > maxSqr)
		{
			maxSqr = zsqr;
			maxIndex = 3;
		}

		maxSqr = sqrtf(maxSqr + 1) * 0.5f;
		float base = 0.25f / maxSqr;
		quat rst;
		switch (maxIndex)
		{
		case 0:
			rst.w = maxSqr;
			rst.v.x = (_21 - _12) * base;
			rst.v.y = (_02 - _20) * base;
			rst.v.z = (_10 - _01) * base;
			break;
		case 1:
			rst.v.x = maxSqr;
			rst.w = (_21 - _12) * base;
			rst.v.y = (_01 + _10) * base;
			rst.v.z = (_02 + _20) * base;
			break;
		case 2:
			rst.v.y = maxSqr;
			rst.w = (_02 - _20) * base;
			rst.v.x = (_01 + _10) * base;
			rst.v.z = (_12 + _21) * base;
			break;
		case 3:
			rst.v.z = maxSqr;
			rst.w = (_10 - _01) * base;
			rst.v.x = (_02 + _20) * base;
			rst.v.y = (_12 + _21) * base;
			break;
		}
		rst.normalize();
		return rst;
	}

	inline quat to_quat_from(const mat33& mat)
	{
		return to_quat_impl(
			mat._00, mat._01, mat._02,
			mat._10, mat._11, mat._12,
			mat._20, mat._21, mat._22);
	}

	inline quat to_quat_from(const mat44& mat)
	{
		return to_quat_impl(
			mat._00, mat._01, mat._02,
			mat._10, mat._11, mat._12,
			mat._20, mat._21, mat._22);
	}

	inline dquat to_dquat_from(const mat33& m33)
	{
		return gml::dquat(
			to_quat_from(m33),
			gml::vec3::zero());
	}

	inline dquat to_dquat_from(const mat44& m44)
	{
		return gml::dquat(
			to_quat_from(m44),
			(gml::vec3)m44.column(3));
	}

	inline aabb transform(const mat44& mat, const aabb& inaabb)
	{
		return aabb(
			gml::transform_point(mat, inaabb.min_bound()),
			gml::transform_point(mat, inaabb.max_bound())
			);
	}

	inline aabb2d transform(const mat32& mat, const aabb2d& inaabb2d)
	{
		return aabb2d(
			gml::transform_point(mat, inaabb2d.min_bound()),
			gml::transform_point(mat, inaabb2d.max_bound())
			);
	}
}
