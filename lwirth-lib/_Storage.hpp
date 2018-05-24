template<class Derived>
class matrix_base_t
{
	f32* operator[](size_t rowIndex);

	const f32* operator[](size_t rowIndex) const;

	static matrix_t<R, C, T> identity();

	static size_t getRowCount();

	static size_t getColumnCount();

	static bool isSquare();

	bool isSymmetric();
	bool hasInverse();

	f32 det() const;

	Derived& operator*=(f32 scalar);
	Derived& operator+=(const Derived& rhs);
	Derived& operator-=(const Derived& rhs);
};

//square matrix
template<size_t S, class T>
class matrix_t<S, S, T>
{
private:
	T m_elements[S][S];

public:

};



template<class T>
class matrix_t<2, 2, T>
{
private:
	f32 m_elements[2][2]; //first index = row, second index = column

public:
	static const matrix_t<2, 2> zero;
	static const matrix_t<2, 2> identity;

	//creates identity matrix
	matrix_t()
	{
		for (auto& row : m_elements)
		{
			for (float& element : row)
			{
				element = 0;
			}
		}
	}

	matrix_t(f32 values[2][2])
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m_elements[i][j] = values[i][j];
			}
		}
	}

	matrix_t(f32 r0c0, f32 r0c1, f32 r1c0, f32 r1c1)
	{
		m_elements[0][0] = r0c0;
		m_elements[0][1] = r0c1;
		m_elements[1][0] = r1c0;
		m_elements[1][1] = r1c1;
	}

	static matrix_t<2, 2> createRotation(f32 radians)
	{
		matrix_t<2, 2> r;
		f32 sin = std::sin(radians);
		f32 cos = std::cos(radians);
		r[0][0] = cos;
		r[0][1] = -sin;
		r[1][0] = sin;
		r[1][1] = cos;

		return r;
	}

	matrix_t(const matrix_t<2, 2>& m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m_elements[i][j] = m.m_elements[i][j];
			}
		}
	}

	f32* operator[](size_t rowIndex)
	{
		return m_elements[rowIndex];
	}

	const f32* operator[](size_t rowIndex) const
	{
		return m_elements[rowIndex];
	}

	matrix_t<2, 2>& operator*=(f32 scalar)
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

	matrix_t<2, 2>& operator+=(matrix_t<2, 2>&& other)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m_elements[i][j] += other.m_elements[i][j];
			}
		}
		return *this;
	}

	matrix_t<2, 2>& operator-=(matrix_t<2, 2>&& other)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m_elements[i][j] -= other.m_elements[i][j];
			}
		}
		return *this;
	}

};

template<>
class matrix_t<4, 4>
{
private:
	f32 m_elements[4][4]; //first index = row, second index = column

public:
	static const matrix_t<4, 4> zero;
	static const matrix_t<4, 4> identity;

	static matrix_t<4, 4> createTranslation(const Vec3& translation)
	{
		matrix_t<4, 4> r;
		r[1][3] = translation.x;
		r[2][3] = translation.y;
		r[3][3] = translation.z;
		return r;
	}

	static matrix_t<4, 4> createRotation(f32 radians, const Vec3& rotationAxis)
	{
		if (radians == 0)
		{
			return identity;
		}

		Vec3 nra = norm(rotationAxis);
		f32 x = nra.x;
		f32 y = nra.y;
		f32 z = nra.z;
		f32 cos = std::cos(radians);
		f32 sin = std::sin(radians);

		matrix_t<4, 4> r;
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

	static matrix_t<4, 4> createScale(const Vec3& scale)
	{
		matrix_t<4, 4> r;
		r[0][0] = scale.x;
		r[1][1] = scale.y;
		r[2][2] = scale.z;
		return r;
	}

	static matrix_t<4, 4> createPerspective(f32 fieldOfView, f32 aspectRatio, f32 nearClipPlane, f32 farClipPlane)
	{
		f32 tanFoV = std::tan(fieldOfView / 2.f);

		matrix_t<4, 4> r;
		r[0][0] = 1.0f / tanFoV / aspectRatio;
		r[1][1] = -1.0f / tanFoV;
		r[2][2] = -(farClipPlane + nearClipPlane) / (farClipPlane - nearClipPlane);
		r[3][3] = 0;

		r[4][2] = -1;
		r[3][3] = -(2 * farClipPlane * nearClipPlane) / (farClipPlane - nearClipPlane);

		return r;
	}

	static matrix_t<4, 4> createView(const Vec3& cameraPos, const Vec3& lookingAt, const Vec3& upDirection)
	{
		Vec3 direction = norm(lookingAt - cameraPos);
		Vec3 right = norm(cross(direction, upDirection));
		Vec3 down = cross(right, direction);

		matrix_t<4, 4> r;
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

	static matrix_t<4, 4> createTransform(const Vec3& pos, const Vec3& scale, const Vec3& rotationVector, f32 radians)
	{
		matrix_t<4, 4> matTranslation = createTranslation(pos);
		matrix_t<4, 4> matScale = createScale(scale);
		matrix_t<4, 4> matRotation = createRotation(radians, rotationVector);

		return matTranslation * matRotation * matScale;
	}

	//creates identity matrix
	matrix_t()
	{
		for (auto& row : m_elements)
		{
			for (float& element : row)
			{
				element = 0;
			}
		}
	}

	matrix_t(f32 values[4][4])
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m_elements[i][j] = values[i][j];
			}
		}
	}

	template <typename ...Args, typename std::enable_if<sizeof...(Args) == 4 * 4, int>::type = 0>
	constexpr matrix_t(const Args&... args) : m_elements{ args... }
	{
	}

	matrix_t(const matrix_t<4, 4>& m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m_elements[i][j] = m.m_elements[i][j];
			}
		}
	}

	inline f32* operator[](size_t rowIndex)
	{
		return m_elements[rowIndex];
	}

	inline const f32* operator[](size_t rowIndex) const
	{
		return m_elements[rowIndex];
	}

	matrix_t<4, 4>& operator*=(f32 scalar)
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

	matrix_t<4, 4>& operator+=(const matrix_t<4, 4>& other)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m_elements[i][j] += other.m_elements[i][j];
			}
		}
		return *this;
	}

	matrix_t<4, 4>& operator-=(const matrix_t<4, 4>& other)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m_elements[i][j] -= other.m_elements[i][j];
			}
		}
		return *this;
	}

};

inline bool operator==(const matrix_t<4, 4>& m1, const matrix_t<4, 4>& m2)
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (m1[i][j] != m2[i][j])return false;
		}
	}
	return true;
}

inline bool operator!=(const matrix_t<4, 4>& m1, const matrix_t<4, 4>& m2)
{
	return !(m1 == m2);
}


inline matrix_t<4, 4> operator*(const matrix_t<4, 4>& m, f32 scalar)
{
	matrix_t<4, 4> r = m;
	r *= scalar;
	return r;
}


inline matrix_t<4, 4> operator*(f32 scalar, const matrix_t<4, 4>& m)
{
	matrix_t<4, 4> r = m;
	r *= scalar;
	return r;
}


inline matrix_t<4, 4> operator+(const matrix_t<4, 4>& m1, const matrix_t<4, 4>& m2)
{
	matrix_t<4, 4> r = m1;
	r += m2;
	return r;
}


inline matrix_t<4, 4> operator-(const matrix_t<4, 4>& m1, const matrix_t<4, 4>& m2)
{
	matrix_t<4, 4> r = m1;
	r -= m2;
	return r;
}

inline matrix_t<4, 4> operator*(const matrix_t<4, 4>& m1, const matrix_t<4, 4>& m2)
{
	matrix_t<4, 4> r;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			f32 v = 0;
			for (size_t k = 0; k < 4; k++)
			{
				v += m1[i][k] * m2[k][j];
			}
			r[i][j] = v;
		}
	}
	return r;
}


inline matrix_t<4, 4> pow(const matrix_t<4, 4>& m, size_t exponent)
{
	auto r = matrix_t<4, 4>::identity;
	for (size_t i = 0; i < exponent; i++)
	{
		r = m * m;
	}
	return r;
}


inline vector_t<4> operator*(const matrix_t<4, 4>& m, const vector_t<4>& v)
{
	vector_t<4> r;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			r[i] = m[i][j] * v[j];
		}
	}
	return r;
}


inline matrix_t<4, 4> transpose(const matrix_t<4, 4>& m)
{
	matrix_t<4, 4> r;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			r[j][i] = m[i][j];
		}
	}
	return r;
}

template<size_t R1, size_t C1, class T1, size_t R2, size_t C2, class T2>
bool operator==(matrix_t<R1, C1, T1>&& m1, matrix_t<R2, C2, T2>&& m2)
{
	if (T1 != T2 || R1 != R2 || C1 != C2)
	{
		return false;
	}
	for (size_t i = 0; i < R1; i++)
	{
		for (size_t j = 0; j < C1; j++)
		{
			if (m1[i][j] != m2[i][j])return false;
		}
	}
	return true;
}

template<size_t R1, size_t C1, class T1, size_t R2, size_t C2, class T2>
bool operator!=(matrix_t<R1, C1, T1>&& m1, matrix_t<R2, C2, T2>&& m2)
{
	return !(LW_FORWARD(m1) == LW_FORWARD(m2));
}

template<size_t R, size_t C, class T>
decltype(auto) operator*(matrix_t<R, C, T>&& m, f32 scalar)
{
	auto r = LW_FORWARD(m);
	r *= scalar;
	return LW_FORWARD(r);
}

template<size_t R, size_t C, class T>
decltype(auto) operator*(f32 scalar, matrix_t<R, C, T>&& m)
{
	auto r = LW_FORWARD(m);
	r *= scalar;
	return LW_FORWARD(r);
}

template<size_t R, size_t C, class T>
decltype(auto) operator+(matrix_t<R, C, T>&& m1, matrix_t<R, C, T>&& m2)
{
	auto r = LW_FORWARD(m1);
	r += LW_FORWARD(m2);
	return LW_FORWARD(r);
}

template<size_t R, size_t C, class T>
decltype(auto) operator-(matrix_t<R, C, T>&& m1, matrix_t<R, C, T>&& m2)
{
	auto r = LW_FORWARD(m1);
	r -= LW_FORWARD(m2);
	return LW_FORWARD(r);
}

template<size_t R1, class T1, size_t C1R2, size_t C2, class T2>
decltype(auto) operator*(matrix_t<R1, C1R2, T1>&& m1, matrix_t<C1R2, C2, T2>&& m2)
{
	matrix_t<R1, C2, decltype(T1*T2)> r;
	for (size_t i = 0; i < R1; i++)
	{
		for (size_t j = 0; j < C2; j++)
		{
			f32 v = 0;
			for (size_t k = 0; k < C1R2; k++)
			{
				v += m1[i][k] * m2[k][j];
			}
			r[i][j] = v;
		}
	}
	return std::move(r);
}