#include "stdafx.hpp"
/**

	const Matrix4x4 Matrix4x4::identity({ {1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 1.f} });

	Matrix4x4::Matrix4x4()
	{
		static_assert(sizeof(Matrix4x4) == sizeof(float) * 16, "Matrix4x4 != 16 * float");

		m_cols[0].x = 1;
		m_cols[0].y = 0;
		m_cols[0].z = 0;
		m_cols[0].w = 0;

		m_cols[1].x = 0;
		m_cols[1].y = 1;
		m_cols[1].z = 0;
		m_cols[1].w = 0;

		m_cols[2].x = 0;
		m_cols[2].y = 0;
		m_cols[2].z = 1;
		m_cols[2].w = 0;

		m_cols[3].x = 0;
		m_cols[3].y = 0;
		m_cols[3].z = 0;
		m_cols[3].w = 1;
	}

	Matrix4x4::Matrix4x4(Vector4 const& col0, Vector4 const& col1, Vector4 const& col2, Vector4 const& col3)
	{
		m_cols[0] = col0;
		m_cols[1] = col1;
		m_cols[2] = col2;
		m_cols[3] = col3;
	}

	Matrix4x4::Matrix4x4(Vector4 vecs[4])
	{
		m_cols[0] = vecs[0];
		m_cols[1] = vecs[1];
		m_cols[2] = vecs[2];
		m_cols[3] = vecs[3];
	}

	Matrix4x4::Matrix4x4(F32 data[4][4])
	{
		m_cols[0] = Vector4(data[0]);
		m_cols[1] = Vector4(data[1]);
		m_cols[2] = Vector4(data[2]);
		m_cols[3] = Vector4(data[3]);
	}

	void Matrix4x4::getData(F32* outPtr) const
	{
		for (Size i = 0; i < 4; i++)
		{
			m_cols[i].getData(outPtr + (i * 4));
		}
	}

	Vector4& Matrix4x4::operator[](U32 index)
	{
		if (index >= 4)
		{
			throw std::exception("Out of bounds");
		}
		return m_cols[index];
	}

	Vector4 Matrix4x4::getColumn(U32 colIndex) const
	{
		if (colIndex >= 4)
		{
			throw std::exception("Out of bounds");
		}
		return m_cols[colIndex];
	}

	Vector4 Matrix4x4::getRow(U32 rowIndex) const
	{
		if (rowIndex >= 4)
		{
			throw std::exception("Out of bounds");
		}
		//return Vector4(m_cols[0][rowIndex], m_cols[1][rowIndex], m_cols[2][rowIndex], m_cols[3][rowIndex]);
		return Vector4();
	}

	Vector3 Matrix4x4::getTranslation() const
	{
		return m_cols[3].xyz();
	}

	Vector3 Matrix4x4::getScale() const
	{
		return Vector3(
			m_cols[0].xyz().mag(),
			m_cols[1].xyz().mag(),
			m_cols[2].xyz().mag()
		);
	}

	Matrix4x4 Matrix4x4::getRotation() const
	{
		Vector3 scale = getScale();
		return Matrix4x4(
			Vector4((m_cols[0] / scale.x).xyz(), 0),
			Vector4((m_cols[1] / scale.y).xyz(), 0),
			Vector4((m_cols[2] / scale.z).xyz(), 0),
			Vector4(0, 0, 0, 1)
		);
	}

	Matrix4x4 & Matrix4x4::operator*=(F32 scalar)
	{
		for (U32 i = 0; i < 4; i++)
		{
			for (U32 k = 0; k < 4; k++)
			{
				this->m_cols[i][k] *= scalar;
			}
		}
		return *this;
	}

	Matrix4x4& Matrix4x4::operator+=(const Matrix4x4 & other)
	{
		for (U32 i = 0; i < 4; i++)
		{
			for (U32 k = 0; k < 4; k++)
			{
				this->m_cols[i][k] += other.m_cols[i][k];
			}
		}
		return *this;
	}

	Matrix4x4 & Matrix4x4::operator-=(const Matrix4x4 & other)
	{
		for (U32 i = 0; i < 4; i++)
		{
			for (U32 k = 0; k < 4; k++)
			{
				this->m_cols[i][k] -= other.m_cols[i][k];
			}
		}
		return *this;
	}


	Matrix4x4 operator*(const Matrix4x4 & m, F32 s)
	{
		Matrix4x4 r = m;
		r *= s;
		return r;
	}

	Matrix4x4 operator*(F32 s, const Matrix4x4 & m)
	{
		Matrix4x4 r = m;
		r *= s;
		return r;
	}

	Matrix4x4 operator+(const Matrix4x4 & m1, const Matrix4x4 & m2)
	{
		Matrix4x4 r = m1;
		r += m2;
		return r;
	}

	Matrix4x4 operator-(const Matrix4x4 & m1, const Matrix4x4 & m2)
	{
		Matrix4x4 r = m1;
		r -= m2;
		return r;
	}

}
**/