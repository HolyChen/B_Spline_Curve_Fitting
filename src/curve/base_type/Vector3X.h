//
// Created by haochuanchen on 18-4-6.
//

#ifndef B_SPLINE_VERTEX_H
#define B_SPLINE_VERTEX_H

/// Three-dimensional vertex.
/// \tparam DataType data type of the coordinate
template <typename DataType = double>
struct Vector3X
{
    using _Dt = DataType;

public:
    /// x coordinate
    _Dt x;
    /// y coordinate
    _Dt y;
    /// z coordinate
    _Dt z;

public:
    /// Create a 3D vertex.
    /// \param x x-coordinate
    /// \param y y-coordinate
    /// \param z z-coordinate
    explicit Vector3X(_Dt x = _Dt(0.0), _Dt y = _Dt(0.0), _Dt z = _Dt(0.0))
            : x(x), y(y), z(z)
    {
    }

    /// Create a 3D vertex.
    /// \param l initialize list
    explicit Vector3X(std::initializer_list<_Dt> l)
    {
        auto iter = l.begin();
        x = *iter;
        iter++;
        y = *iter;
        iter++;
        z = *iter;
    }

public:
    /// Add a 3D vertex.
    /// \param rhs right hand operand
    /// \return sum
    Vector3X operator+(const Vector3X& rhs) const
    {
        return Vector3X(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    /// Add a 3D vertex.
    /// \param rhs right hand operand
    /// \return *this
    Vector3X& operator+=(const Vector3X& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    /// Sub a 3D vertex.
    /// \param rhs right hand operand
    /// \return difference
    Vector3X operator-(const Vector3X& rhs) const
    {
        return Vector3X(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    /// Sub a 3D vertex.
    /// \param rhs right hand operand
    /// \return *this
    Vector3X& operator-=(const Vector3X& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    /// Scalar multiply with `rhs`.
    /// \param rhs right hand operand
    /// \return scalar multiplication of the vertex
    Vector3X operator*(_Dt rhs) const
    {
        return Vector3X(x * rhs, y * rhs, z * rhs);
    }

    /// Scalar multiply with `lhs`
    /// \param lhs scalar
    /// \param rhs vertex
    /// \return scalar multiplication of the vertex
    friend Vector3X operator*(_Dt lhs, const Vector3X& rhs)
    {
        return Vector3X(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
    }

    /// Scalar multiply with `rhs`.
    /// \param rhs right hand operand
    /// \return *this
    Vector3X& operator*=(_Dt rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    /// Scalar multiply with (1.0 / `rhs`).
    /// \param rhs right hand operand
    /// \return scalar multiplication of the vertex
    Vector3X operator/(_Dt rhs) const
    {
        return Vector3X(x / rhs, y / rhs, z / rhs);
    }

    /// Scalar multiply with (1.0 / `rhs`).`.
    /// \param rhs right hand operand
    /// \return *this
    Vector3X& operator/=(_Dt rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    /// Dot product with another vertex.
    /// \param rhs right hand operand
    /// \return dot production
    _Dt dot(const Vector3X& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    /// Squared Euclidean length, means (Euclidean length)^2
    /// \return Squared Euclidean distance.
    _Dt squared_length() const
    {
        return dot(*this);
    }

    /// Euclidean length.
    /// \return Euclidean length
    _Dt length() const
    {
        return std::sqrt(dot(*this));
    }

    /// Cross product with another vertex.
    /// \param rhs right hand operand
    /// \return cross production
    Vector3X cross(Vector3X rhs) const
    {
        return Vector3X(
                y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x
        );
    }

    bool operator==(const Vector3X& ano) const
    {
        return x == ano.x && y == ano.y && z == ano.z;
    }

    bool operator!=(const Vector3X& ano) const
    {
        return x != ano.x || y != ano.y || z != ano.z;
    }

};

template <typename _DataType>
using Vertex = Vector3X<_DataType>;

#endif //B_SPLINE_VERTEX_H
