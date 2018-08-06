#pragma once

#include "Standard.hpp"

#include "qmath.hpp"
#include "Meta.hpp"
#include <iostream>
#include <array> //#TODO use own custom array

#define ARITHMETIC_CHECK(T) static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

namespace lw
{

    //declerations & usings
    template<class T, size_t N>
    class components_t;

    template<class T, size_t N>
    class vector_t;

    template<size_t N>
    using Vec = vector_t<f32, N>;

    using Vec2 = Vec<2>;
    using Vec3 = Vec<3>;
    using Vec4 = Vec<4>;


    //definitons
    template<class T, size_t N>
    class components_t
    {
        ARITHMETIC_CHECK(T);
    public:
        using type = T;

        constexpr static size_t getDim() noexcept
        {
            return N;
        }

        components_t()
            : arr {}
        {
        }

        components_t(const components_t<T, N>& c)
        {
            arr = c.arr;
        }

        components_t<T, N>& operator=(const components_t<T, N>& c)
        {
            arr = c.arr;
            return *this;
        }

        components_t(components_t<T, N>&& c)
        {
            arr = std::move(c.arr);
        }

        components_t<T, N>& operator=(components_t<T, N>&& c)
        {
            arr = std::move(arr);
            return *this;
        }

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == N>>
        components_t(Args&& ... args)
            : arr{ std::forward<T>(static_cast<T>(args))... }
        {
        }

        union
        {
            std::array<T, N> arr;
        };
    };

    template<class T>
    class components_t<T, 2>
    {
        ARITHMETIC_CHECK(T);
    public:
        using type = T;

        constexpr static size_t getDim() noexcept
        {
            return 2;
        }

        components_t()
            : arr{}
        {
        }

        components_t(const components_t<T, 2>& c)
        {
            arr = c.arr;
        }

        components_t<T, 2>& operator=(const components_t<T, 2>& c)
        {
            arr = c.arr;
            return *this;
        }

        components_t(components_t<T, 2>&& c)
        {
            arr = std::move(c.arr);
        }

        components_t<T, 2>& operator=(components_t<T, 2>&& c)
        {
            arr = std::move(arr);
            return *this;
        }

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == 2>>
        components_t(Args&& ... args)
            : arr{ std::forward<T>(static_cast<T>(args))... }
        {
        }

        union
        {
            std::array<T, 2> arr;
#pragma pack(sizeof(T))
            struct { T x, y; };
        };
    };

    template<class T>
    class components_t<T, 3>
    {
        ARITHMETIC_CHECK(T);
    public:
        using type = T;

        constexpr static size_t getDim() noexcept
        {
            return 3;
        }

        components_t()
            : arr{}
        {
        }

        components_t(const components_t<T, 3>& c)
        {
            arr = c.arr;
        }

        components_t<T, 3>& operator=(const components_t<T, 3>& c)
        {
            arr = c.arr;
            return *this;
        }

        components_t(components_t<T, 3>&& c)
        {
            arr = std::move(c.arr);
        }

        components_t<T, 3>& operator=(components_t<T, 3>&& c)
        {
            arr = std::move(arr);
            return *this;
        }

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == 3>>
        components_t(Args&& ... args)
            : arr{ std::forward<T>(static_cast<T>(args))... }
        {
        }

        union
        {
            std::array<T, 3> arr;
#pragma pack(sizeof(T))
            struct { T x, y, z; };
        };
    };

    template<class T>
    class components_t<T, 4>
    {
        ARITHMETIC_CHECK(T);
    public:
        using type = T;

        constexpr static size_t getDim() noexcept
        {
            return 4;
        }

        components_t()
            : arr{}
        {
        }

        components_t(const components_t<T, 4>& c)
        {
            arr = c.arr;
        }

        components_t<T, 4>& operator=(const components_t<T, 4>& c)
        {
            arr = c.arr;
            return *this;
        }

        components_t(components_t<T, 4>&& c)
        {
            arr = std::move(c.arr);
        }

        components_t<T, 4>& operator=(components_t<T, 4>&& c)
        {
            arr = std::move(arr);
            return *this;
        }

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == 4>>
        components_t(Args&& ... args)
            : arr { std::forward<T>(static_cast<T>(args))... }
        {
        }

        union
        {
            std::array<T, 4> arr;
#pragma pack(sizeof(T))
            struct { T x, y, z, w; };
        };
    };


    /*!
        @brief The vector_t(general) class is a mathematical vector of any dimensions and type.

        vector_t is a templated class of a mathematical vector
        of any dimensions and type. The data is allocated on the stack.

        @templateparam COMPONENTS specifies the class for storing the components data as union.
    */
    template <class T, size_t N>
    class vector_t : public components_t<T, N>
    {
    private:
        //no additional members
        //components are inherited by "COMPONENTS"
    public:
        using SELF = vector_t<T, N>;
		using COMPONENTS = components_t<T, N>;

         /*!
         	@brief Default constructs.
         
         	Default Constructor which creates a zero vector.
         */
        vector_t()
            : COMPONENTS()
        {
        }

        /*!
         	@brief Copy constructs.
         
         	Copy Constructor which copies the data of the reference of v.
         
            @param v is the vector_t to copy
         */
        vector_t(const SELF& v)
            : COMPONENTS(v)
        {
        }

        SELF& operator=(const SELF& v)
        {
            COMPONENTS::operator=(v);
            return *this;
        }


        /*!
            @brief Move constructs.

            Move Constructor which moves the data of the reference of v.

            @param v is the vector_t to move
        */
        vector_t(SELF&& v)
            : COMPONENTS(v)
        {
        }

        SELF& operator=(SELF&& v)
        {
            COMPONENTS::operator=(v);
            return *this;
        }

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == COMPONENTS::getDim()>>
        vector_t(Args&& ... args)
            : COMPONENTS(std::forward<Args>(args)...)
        {
        }

        /*! 
            @brief Constructs a vector defined by two points.

            Constructs a vector defined by a vector as tail and another as head

            @param tail is the tail of the vector
            @param head is the head of the vector

            @return the constructed vector
        */
        friend SELF VectorByPoints(SELF&& tail, SELF&& head) noexcept
        {
            return head - tail;
        }

        /*!
            @brief Constructs a vector defined by two points.

            Constructs a vector defined by a vector as tail and another as head

            @param tail is the tail of the vector
            @param head is the head of the vector

            @return the constructed vector
        */
        friend SELF VectorByPoints(const SELF& tail, const SELF& head) noexcept
        {
            return head - tail;
        }

        /*!
            @brief Returns the magnitude of the vector.

            Calculates lw::sqrt(magSqd()) which is the
            magnitude/length of the vector.

            @performance calls lw::sqrt()

            @return the magnitude
        */
        typename T mag() const noexcept
        {
            return lw::sqrt(magSqd());
        }

        /*!
            @brief Returns the squared magnitude of the vector.

            Calculates the square of the magnitude of the vector,
            by adding all squares of the compontents up.

            @return the squared magnitude
        */
        typename T magSqd() const noexcept
        {
            f32 r = 0;
            for (size_t i = 0; i < N; i++)
            {
                r += COMPONENTS::arr[i] * COMPONENTS::arr[i];
            }
            return r;
        }

        /*!
             @brief normalizes the vector.

             Normalizes the vector by dividing
             every component by the magnitude.

             @performance calls mag() -> lw::sqrt()
        */
        void norm() noexcept
        {
            if (magSqd() == 1.f)return;
            f32 f = 1.f / mag();
            for (size_t i = 0; i < N ; i++)
            {
                COMPONENTS::arr[i] *= f;
            }
        }

        /*!
            @brief sets the magnitude of the vector to newMag

            Scales the vector to the new magnitude newMag,
            by multipling every component by newMag and
            dividing it by the actual magnitude.

            @param newMag is the new magnitude

            @performance calls mag() -> lw::sqrt()
        */
        void setMag(f32 newMag) noexcept
        {
            if (magSqd() == newMag * newMag)return;
            f32 f = newMag / mag();
            for (size_t i = 0; i < N; i++)
            {
                COMPONENTS::arr[i] *= f;
            }
        }

        /*!
            @brief clamps the magnitude to maxMag

            If the vector's magnitude is greater than
            maxMag then it's set to maxMag.

            @param maxMag is the maximal magnitude
        */
        void clamp(f32 maxMag) noexcept
        {
            if (magSqd() > (maxMag * maxMag))
                setMag(maxMag);
        }

        /*!
            @brief returns a reference to the indexed component.

            The operator[] returns a reference to the index'th component.

            @param index is the index of the component

            @return f32 lreference to component
        */
        f32& operator[](size_t index)
        {
            return arr[index];
        }

        /*!
            @brief returns a const reference to the indexed component.

            The operator[] returns a const reference to the index'th component.

            @param index is the index of the component

            @return f32 const reference to component
        */
        const f32& operator[](size_t index) const
        {
            return COMPONENTS::arr[index];
        }

        /*!
            @brief scalar-vector multiplication

            Performes a vector-scalar multiplcation (component-wise)
            and assigns the new values.

            @param scalar to multiply by
            
            @return lreference to this
        */
        SELF& operator*=(f32 scalar) noexcept
        {
            for (size_t i = 0; i < N; i++)
            {
                COMPONENTS::arr[i] *= scalar;
            }
            return *this;
        }

        /*!
            @brief vector-scalar division

            Performs a vector-scalar division (component-wise)
            and assigns the new values.

            @param scalar to divide by

            @return lreference to this
        */
        SELF& operator/=(f32 scalar) noexcept
        {
            f32 f = 1.f / scalar;
            for (size_t i = 0; i < N; i++)
            {
                COMPONENTS::arr[i] *= f;
            }
            return *this;
        }

        /*!
            @brief vector-vector addition

            Performs a vector-vector addition (component-wise)
            and assigns the new values.

            @param rhs is a rreference to the right hand side.

            @return lreference 
        */
        SELF& operator+=(SELF&& rhs)
        {
            for (size_t i = 0; i < N; i++)
            {
                COMPONENTS::arr[i] += rhs.COMPONENTS::arr[i];
            }
            return *this;
        }

        /*!
            @brief vector-vector addition

            Performs a vector-vector addition (component-wise)
            and assigns the new values.

            @param rhs is a lreference to the right hand side.

            @return lreference to this
        */
        SELF& operator+=(const SELF& v)
        {
            for (size_t i = 0; i < N; i++)
            {
                COMPONENTS::arr[i] += v.COMPONENTS::arr[i];
            }
            return *this;
        }

        /*!
            @brief vector-vector subtraction

            Performs a vector-vector subtraction (component-wise)
            and assigns the new values.

            @param rhs is a rreference to the right hand side.

            @return lreference to this
        */
        SELF& operator-=(SELF&& v)
        {
            for (size_t i = 0; i < N; i++)
            {
                arr[i] -= v.arr[i];
            }
            return *this;
        }

        /*!
            @brief vector-vector subtraction

            Performs a vector-vector subtraction (component-wise)
            and assigns the new values.

            @param rhs is a lreference to the right hand side.

            @return lreference to this
        */
        SELF& operator-=(const SELF& v)
        {
            for (size_t i = 0; i < N; i++)
            {
				COMPONENTS::arr[i] -= v.arr[i];
            }
            return *this;
        }

        /*!
            @brief Returns a string formatting the components of the vector

            Stringifies the components of the vector and
            formats it accordingly.

            @return the string
        */
        std::string stringify() const noexcept
        {
            std::string s;

            s.append("[");
            for (size_t i = 0; i < N; i++)
            {
                s.append(std::to_string(COMPONENTS::arr[i]));
                if (i != COMPONENTS::getDim() - 1)
                {
                    s.append(", ");
                }
                else
                {
                    s.append("]");
                }
            }
            return s;
        }
    };

    /*!
        @brief returns whetever two vectors are the same.

        Performs an equals comparison on v1 and v2.

        @param v1 is the rreference to the first vector
        @param v2 is the rreference to the second vector

        @return bool indicating equalness
    */
    template<class T, size_t N>
    bool operator==(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        for (size_t i = 0; i < N; i++)
        {
            if (v1.arr[i] != v2.arr[i])
            {
                return false;
            }
        }
        return true;
    }

    /*!
        @brief returns whetever two vectors are the same.

        Performs an equals comparison on v1 and v2.

        @param v1 is the lreference to the first vector
        @param v2 is the lreference to the second vector

        @return bool indicating equalness
    */
	template<class T, size_t N>
    bool operator==(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        for (size_t i = 0; i < N; i++)
        {
            if (v1.arr[i] != v2.arr[i])
            {
                return false;
            }
        }
        return true;
    }

    /*!
        @brief returns whetever two vectors are not the same.

        Performs a non-equals comparison on v1 and v2.

        @param v1 is the rreference to the first vector
        @param v2 is the rreference to the second vector

        @return bool indicating non-equalness
    */
	template<class T, size_t N>
    bool operator!=(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        return !(v1 == v2);
    }

    /*!
        @brief returns whetever two vectors are not the same.

        Performs a non-equals comparison on v1 and v2.

        @param v1 is the lreference to the first vector
        @param v2 is the lreference to the second vector

        @return bool indicating non-equalness
    */
	template<class T, size_t N>
    bool operator!=(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        return !(v1 == v2);
    }

    /*!
        @brief negates the vector

        Performs a negation of v, which is 
        the same as a multiplication with -1.

        @param v is the vector rreference

        @return the negated vector
    */
	template<class T, size_t N>
    vector_t<T, N> operator-(vector_t<T, N>&& v)
    {
        return (v *= -1.f);
    }

    /*!
    @brief negates the vector

    Performs a negation of v, which is
    the same as a multiplication with -1.

    @param v is the vector lreference

    @return the negated vector
    */
	template<class T, size_t N>
    vector_t<T, N> operator-(const vector_t<T, N>& v)
    {
        auto r = v;
        return (r *= -1.f);
    }

    /*!
        @brief returns the sum of two vectors.

        Performs an addition of two vectors.

        @param v1 is the rreference of the first vector
        @param v2 is the rreference of the second vector

        @return the sum
    */
	template<class T, size_t N>
    vector_t<T, N> operator+(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        return(v1 += v2);
    }

    /*!
        @brief returns the sum of two vectors.

        Performs an addition of two vectors.

        @param v1 is the lreference of the first vector
        @param v2 is the lreference of the second vector

        @return the sum
    */
	template<class T, size_t N>
    vector_t<T, N> operator+(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        auto r = v1;
        return (r += v2);
    }
    
    /*!
        @brief Returns the difference of two vectors.

        Performs a subtraction of two vectors.

        @param v1 is the rreference of the first vector
        @param v2 is the rreference of the second vector

        @return the difference
    */
	template<class T, size_t N>
    vector_t<T, N> operator-(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        return (v1 += v2);
    }

    /*!
        @brief Returns the difference of two vectors.

        Performs a subtraction of two vectors.

        @param v1 is the lreference of the first vector
        @param v2 is the lreference of the second vector

        @return the difference
    */
	template<class T, size_t N>
    vector_t<T, N> operator-(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        auto r = v1;
        return (r -= v2);
    }

    /*!
        @brief Returns the vector-scalar product.

        Performs a vector-scalar multiplication,
        which scales the vector by the given amount.

        @param v is the rreference of the vector
        @param scalar is the scalar

        @return the product
    */
	template<class T, size_t N>
    vector_t<T, N> operator*(vector_t<T, N>&& v, f32 scalar)
    {
        return (v *= scalar);
    }

    /*!
        @brief Returns the vector-scalar product.

        Performs a vector-scalar multiplication,
        which scales the vector by the given amount.

        @param v is the lreference of the vector
        @param scalar is the scalar

        @return the product
    */
	template<class T, size_t N>
    vector_t<T, N> operator*(const vector_t<T, N>& v, f32 scalar)
    {
        auto r = v;
        return (r *= scalar);
    }

    /*!
        @brief Returns the vector-scalar product.

        Performs a scalar-vector multiplication,
        which scales the vector by the given amount.

        @param scalar is the scalar
        @param v is the rreference of the vector

        @return the product
    */
	template<class T, size_t N>
    vector_t<T, N> operator*(f32 scalar, vector_t<T, N>&& v)
    {
        return (v *= scalar);
    }

    /*!
        @brief Returns the vector-scalar product.

        Performs a scalar-vector multiplication,
        which scales the vector by the given amount.

        @param scalar is the scalar
        @param v is the lreference of the vector

        @return the product
    */
	template<class T, size_t N>
    vector_t<T, N> operator*(f32 scalar, const vector_t<T, N>& v)
    {
        auto r = v;
        return (r *= scalar);
    }

    /*!
        @brief Returns the vector-scalar quotient.

        Performs a vector-scalar division,
        which invertly scales the vector by the given amount.

        @param v is the rreference of the vector
        @param scalar is the scalar

        @return the quotient
    */
	template<class T, size_t N>
    vector_t<T, N> operator/(vector_t<T, N>&& v, f32 scalar)
    {
        return (v /= scalar);
    }

    /*!
        @brief Returns the vector-scalar quotient.

        Performs a vector-scalar division,
        which invertly scales the vector by the given amount.

        @param v is the lreference of the vector
        @param scalar is the scalar

        @return the quotient
    */
	template<class T, size_t N>
    vector_t<T, N> operator/(const vector_t<T, N>& v, f32 scalar)
    {
        auto r = v;
        return (r /= scalar);
    }

    /*!
        @brief Returns a vector with minimal components.

        Creates a vector with the smaller components of
        both vectors.

        @param v1 is the lreference of the first vector
        @param v2 is the lreference of the second vector

        @return the minimal vector
    */
	template<class T, size_t N>
    vector_t<T, N> min(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        vector_t<T, N> r;
        for (size_t i = 0; i < N; i++)
        {
            r.arr[i] = lw::min(v1.arr[i], v2.arr[i]);
        }
        return r;
    }

    /*!
        @brief Returns a vector with maximal components.

        Creates a vector with the bigger components of
        both vectors.

        @param v1 is the lreference of the first vector
        @param v2 is the lreference of the second vector

        @return the maximal vector
    */
	template<class T, size_t N>
    vector_t<T, N> max(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        vector_t<T, N> r;
        for (size_t i = 0; i < N; i++)
        {
            r.arr[i] = lw::max(v1.arr[i], v2.arr[i]);
        }
        return r;
    }

    /*!
        @brief Returns whetever two vectors are collinear.

        Performs a collinear/parallel check of v1 and v2.

        @param v1 is the rreference to the first vector
        @param v2 is the rreference to the second vector

        @return a bool indicating the collinearity
    */
	template<class T, size_t N>
    bool collinear(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        f32 f = v1[0] / v2[0];
        for (size_t i = 1; i < N; i++)
        {
            if (v1[i] != f * v2[i])return false;
        }
        return true;
    }

    /*!
        @brief Returns whetever two vectors are collinear.

        Performs a collinear/parallel check of v1 and v2.

        @param v1 is the lreference to the first vector
        @param v2 is the lreference to the second vector

        @return a bool indicating the collinearity
    */
	template<class T, size_t N>
    bool collinear(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        f32 f = v1[0] / v2[0];
        for (size_t i = 1; i < N; i++)
        {
            if (v1[i] != f * v2[i])return false;
        }
        return true;
    }

    /*!
        @brief Returns normalized vector.

        Performs a normalization on v.

        @param v is the rreference to the vector

        @performance calls mag() -> lw::sqrt()

        @return the normalized vector
    */
	template<class T, size_t N>
    vector_t<T, N> norm(vector_t<T, N>&& v)
    {
        v.norm();
        return v;
    }

    /*!
        @brief Returns normalized vector.

        Performs a normalization on v.

        @param v is the lreference to the vector

        @performance calls mag() -> lw::sqrt()

        @return the normalized vector
    */
	template<class T, size_t N>
    vector_t<T, N> norm(const vector_t<T, N>& v)
    {
        auto r = v;
        r.norm();
        return r;
    }

    /*!
        @brief Returns the scaled vector with indicated magnitude.

        Performs a scaling of v to set the magnitude to mag.

        @param v is the rreference to the vector
        @param mag is the new magnitude

        @performance calls mag() -> lw::sqrt()

        @return scaled vector
    */
	template<class T, size_t N>
    vector_t<T, N> setMag(vector_t<T, N>&& v, f32 mag)
    {
        v.setMag(mag);
        return v;
    }

    /*!
        @brief Returns the scaled vector with indicated magnitude.

        Performs a scaling of v to set the magnitude to mag.

        @param v is the lreference to the vector
        @param mag is the new magnitude

        @performance calls mag() -> lw::sqrt()

        @return scaled vector
    */
	template<class T, size_t N>
    vector_t<T, N> setMag(const vector_t<T, N>& v, f32 mag)
    {
        auto r = v;
        r.setMag(mag);
        return r;
    }

    /*!
        @brief Returns the dot product of two vectors.

        Calculates the dot/scalar product of v1 and v2.

        @param v1 is the rreference of the first vector
        @param v2 is the rreference of the second vector

        @return the product(scalar)
    */
	template<class T, size_t N>
    f32 dot(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        f32 r = 0;
        for (size_t i = 0; i < N; i++)
        {
            r += v1.arr[i] * v2.arr[i];
        }
        return r;
    }

    /*!
        @brief Returns the dot product of two vectors.

        Calculates the dot/scalar product of v1 and v2.

        @param v1 is the lreference of the first vector
        @param v2 is the lreference of the second vector

        @return the product(scalar)
    */
	template<class T, size_t N>
    f32 dot(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        f32 r = 0;
        for (size_t i = 0; i < N; i++)
        {
            r += v1.arr[i] * v2.arr[i];
        }
        return r;
    }

    /*!
        @brief Returns the angle between two vectors

        Calculates the angle between v1 and v2.

        @param v1 is the rreference to the first vector
        @param v2 is the rreference to the second vector

        @performance calls mag() -> lw::sqrt two times

        @return the angle
    */
	template<class T, size_t N>
    f32 angle(vector_t<T, N>&& v1, vector_t<T, N>&& v2)
    {
        return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
    }

    /*!
        @brief Returns the angle between two vectors

        Calculates the angle between v1 and v2.

        @param v1 is the rreference to the first vector
        @param v2 is the rreference to the second vector

        @performance calls mag() -> lw::sqrt two times

        @return the angle
    */
	template<class T, size_t N>
    f32 angle(const vector_t<T, N>& v1, const vector_t<T, N>& v2)
    {
        return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
    }

    /*!
        @brief inserts a stringified vector into ostream

        Stringifies a vector and inserts it into
        the given ostream and returns it.

        @param os is the lreference to the ostream
        @param v is the vector

        @return the modified ostream
    */
	template<class T, size_t N>
    std::ostream& operator<<(std::ostream& os, const vector_t<T, N>& v)
    {
        return os << v.stringify();
    }

	template<class T>
    vector_t<T, 3> cross(vector_t<T, 3>&& v1, vector_t<T, 3>&& v2)
    {
        return vector_t<T, 3>
        (
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    template<class T>
    vector_t<T, 3> cross(const vector_t<T, 3>& v1, const vector_t<T, 3>& v2)
    {
        return vector_t<T, 3>
            (
                v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x
            );
    }

    
    

}