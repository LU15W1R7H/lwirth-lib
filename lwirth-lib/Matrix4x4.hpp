#pragma once

#include "Standard.hpp"

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace lw
{
	template<>
	class Matrix<4, 4>
	{
	private:
		F32 m_elements[4][4]; //first index = row, second index = column

	public:
		static const Matrix<4, 4> zero;
		static const Matrix<4, 4> identity;

		static Matrix<4, 4> createTranslation(const Vector3& translation)
		{
			Matrix<4, 4> r;
			r[1][3] = translation.x;
			r[2][3] = translation.y;
			r[3][3] = translation.z;
			return r;
		}

		static Matrix<4, 4> createRotation(F32 radians, const Vector3& rotationAxis)
		{
			if (radians == 0)
			{
				return identity;
			}

			Vector3 nra = norm(rotationAxis);
			F32 x = nra.x;
			F32 y = nra.y;
			F32 z = nra.z;
			F32 cos = std::cos(radians);
			F32 sin = std::sin(radians);

			Matrix<4, 4> r;
			r[0][0] = cos + x * x * (1 - cos);
			r[0][1] = x * y * (1 - cos) - z * sin;
			r[0][2] = x * z * (1 - cos) + y * sin;

			r[1][0] = y * x * (1 - cos) + z * sin;
			r[1][1] = cos + y * y * (1 - cos);
			r[1][2] = y * z * (1 - cos) - x * sin;

			r[2][0] = z * x * (1 - cos) - y * sin;
			r[2][1] = z * y * (1 - cos) + x * sin;
			r[2][2] = cos + z * z * (1 - cos);

			return r;
		}

		static Matrix<4, 4> createScale(const Vector3& scale)
		{
			Matrix<4, 4> r;
			r[0][0] = scale.x;
			r[1][1] = scale.y;
			r[2][2] = scale.z;
			return r;
		}

		static Matrix<4, 4> createPerspective(F32 fieldOfView, F32 aspectRatio, F32 nearClipPlane, F32 farClipPlane)
		{
			F32 tanFoV = std::tan(fieldOfView / 2.f);

			Matrix<4, 4> r;
			r[0][0] = 1.0f / tanFoV / aspectRatio;
			r[1][1] = -1.0f / tanFoV;
			r[2][2] = -(farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
			r[3][3] = 0;

			r[4][2] = -1;
			r[3][3] = -(2 * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);

			return r;
		}

		static Matrix<4, 4> createView(const Vector3& cameraPos, const Vector3& lookingAt, const Vector3& upDirection)
		{
			Vector3 direction = norm(lookingAt - cameraPos);
			Vector3 right = norm(cross(direction, upDirection));
			Vector3 down = cross(right, direction);

			Matrix<4, 4> r;
			r[0][0] = right.x;
			r[0][1] = right.y;
			r[0][2] = right.z;
			 
			r[1][0] = down.x;
			r[1][1] = down.y;
			r[1][2] = down.z;
			 
			r[2][0] = -direction.x;
			r[2][1] = -direction.y;
			r[2][2] = -direction.z;
			 
			r[3][0] = -dot(right, cameraPos);
			r[3][1] = -dot(down, cameraPos);
			r[3][2] = dot(direction, cameraPos);

			return r;
		}

		static Matrix<4, 4> createTransform(const Vector3& pos, const Vector3& scale, const Vector3& rotationVector, F32 radians)
		{
			Matrix<4, 4> matTranslation = createTranslation(pos);
			Matrix<4, 4> matScale = createScale(scale);
			Matrix<4, 4> matRotation = createRotation(radians, rotationVector);

			return matTranslation * matRotation * matScale;
		}

		//creates identity matrix
		Matrix()
		{
			for (auto& row : m_elements)
			{
				for (float& element : row)
				{
					element = 0;
				}
			}
		}

		Matrix(F32 values[4][4])
		{
			for (Size i = 0; i < 4; i++)
			{
				for (Size j = 0; j < 4; j++)
				{
					m_elements[i][j] = values[i][j];
				}
			}
		}

		template <typename ...Args, typename std::enable_if<sizeof...(Args) == 4 * 4, int>::type = 0>
		constexpr Matrix(const Args&... args) : m_elements{ args... }
		{
		}

		Matrix(const Matrix<4, 4>& m)
		{
			for (Size i = 0; i < 4; i++)
			{
				for (Size j = 0; j < 4; j++)
				{
					m_elements[i][j] = m.m_elements[i][j];
				}
			}
		}

		inline F32* operator[](Size rowIndex)
		{
			return m_elements[rowIndex];
		}

		inline const F32* operator[](Size rowIndex) const
		{
			return m_elements[rowIndex];
		}

		Matrix<4, 4>& operator*=(F32 scalar)
		{
			for (auto& row : m_elements)
			{
				for (float& element : row)
				{
					element *= scalar;
				}
			}
			return *this;
		}

		Matrix<4, 4>& operator+=(const Matrix<4, 4>& other)
		{
			for (Size i = 0; i < 4; i++)
			{
				for (Size j = 0; j < 4; j++)
				{
					m_elements[i][j] += other.m_elements[i][j];
				}
			}
			return *this;
		}

		Matrix<4, 4>& operator-=(const Matrix<4, 4>& other)
		{
			for (Size i = 0; i < 4; i++)
			{
				for (Size j = 0; j < 4; j++)
				{
					m_elements[i][j] -= other.m_elements[i][j];
				}
			}
			return *this;
		}

	};

	inline bool operator==(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2)
	{
		for (Size i = 0; i < 4; i++)
		{
			for (Size j = 0; j < 4; j++)
			{
				if (m1[i][j] != m2[i][j])return false;
			}
		}
		return true;
	}

	inline bool operator!=(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2)
	{
		return !(m1 == m2);
	}

	
	inline Matrix<4, 4> operator*(const Matrix<4, 4>& m, F32 scalar)
	{
		Matrix<4, 4> r = m;
		r *= scalar;
		return r;
	}

	
	inline Matrix<4, 4> operator*(F32 scalar, const Matrix<4, 4>& m)
	{
		Matrix<4, 4> r = m;
		r *= scalar;
		return r;
	}

	
	inline Matrix<4, 4> operator+(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2)
	{
		Matrix<4, 4> r = m1;
		r += m2;
		return r;
	}

	
	inline Matrix<4, 4> operator-(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2)
	{
		Matrix<4, 4> r = m1;
		r -= m2;
		return r;
	}

	inline Matrix<4, 4> operator*(const Matrix<4, 4>& m1, const Matrix<4, 4>& m2)
	{
		Matrix<4, 4> r;
		for (Size i = 0; i < 4; i++)
		{
			for (Size j = 0; j < 4; j++)
			{
				F32 v = 0;
				for (Size k = 0; k < 4; k++)
				{
					v += m1[i][k] * m2[k][j];
				}
				r[i][j] = v;
			}
		}
		return r;
	}

	
	inline Matrix<4, 4> pow(const Matrix<4, 4>& m, Size exponent)
	{
		auto r = Matrix<4, 4>::identity;
		for (Size i = 0; i < exponent; i++)
		{
			r = m * m;
		}
		return r;
	}

	
	inline Vector<4> operator*(const Matrix<4, 4>& m, const Vector<4>& v)
	{
		Vector<4> r;
		for (Size i = 0; i < 4; i++)
		{
			for (Size j = 0; j < 4; j++)
			{
				r[i] = m[i][j] * v[j];
			}
		}
		return r;
	}

	
	inline Matrix<4, 4> transpose(const Matrix<4, 4>& m)
	{
		Matrix<4, 4> r;
		for (Size i = 0; i < 4; i++)
		{
			for (Size j = 0; j < 4; j++)
			{
				r[j][i] = m[i][j];
			}
		}
		return r;
	}

	using Matrix4 = Matrix<4, 4>;
}



