//
// Created by haochuanchen on 18-4-18.
//

#ifndef HELLO_BSPLINEFUNCTION_H
#define HELLO_BSPLINEFUNCTION_H

#include <stdexcept>
#include <vector>

/**
 * B spline function defined on a given knot vector. Its contains methods such as
 * to get function values, to get derivative values.
 * It is NOT THREAD SAFETY!
 * See: *The NURBS Book* Section 2.5.
 * @tparam _DataType data type the function use, default `double`
 */
template <typename _DataType = double>
class BSplineFunction
{
    typedef _DataType _Dt;

private:
    /// `_n` + 1 the number of control points
    int _n;
    /// degree(order - 1) of the B spline
    int _degree;
    /// knot vector contains (`n_ctrpt` + degree + 2) knots
    const std::vector<_Dt>& knots;

    // ---------- aux variable. See: *The NURBS Book* A2.1 A2.2 A2.3. ----------

    /// left array, _DT[p + 1]
    mutable _Dt* _left = nullptr;
    /// right array, _DT[p + 1]
    mutable _Dt* _right = nullptr;

    /// to store the basis functions and knot differences, _DT[p + 1][p + 1]
    mutable _Dt** _ndu = nullptr;

    // to store (in an alternating fashion) the two most recently computed
    // rows a_{k,j} and a_{k-1,j}
    mutable _Dt** _a = nullptr;


public:

    /**
     * Create `degree`-th B spline function defined on knots.
     * @param n n + 1 the number of the control points
     * @param degree degree(order - 1) of the B spline
     * @param knots knot vector, contains (`n_ctrpt` + degree + 2) knots
     */
    BSplineFunction(int n, int degree, const std::vector<_Dt>& knots)
        : _n(n), _degree(degree), knots(knots)
    {
        _left = new _Dt[degree + 1];
        _right = new _Dt[degree + 1];

        _ndu = new _Dt*[degree + 1];
        for (int i = 0; i < degree + 1; i++)
        {
            _ndu[i] = new _Dt[degree + 1];
        }

        _a = new _Dt*[2];
        for (int i = 0; i < 2; i++)
        {
            _a[i] = new _Dt[degree + 1];
        }
    }

    ~BSplineFunction()
    {
        if (_left != nullptr)
        {
            delete[] _left;
            _left = nullptr;
        }

        if (_right != nullptr)
        {
            delete[] _right;
            _right = nullptr;
        }

        if (_ndu != nullptr)
        {
            for (int i = 0; i < _degree + 1; i++)
            {
                delete[] _ndu[i];
            }
            delete[] _ndu;
            _ndu = nullptr;
        }

        if (_a != nullptr)
        {
            for (int i = 0; i < 2; i++)
            {
                delete[] _a[i];
            }
            delete[] _a;
            _a = nullptr;
        }
    }

    /**
     * Determine the knot span index of parameter u.
     * See: *The NURBS Book* Algorithm A2.1
     * @param u the parameter
     * @return the knot span index
     */
    int find_span(_Dt u) const
    {
        // wrong case
        if (u < knots[0] || u > knots[_n + 1])
        {
            throw std::out_of_range("parameter u is out of knot vector.");
        }

        // special case, last one
        if (u == knots[_n + 1])
        {
            return _n;
        }

        // binary search
        int low = _degree, high = _n + 1;
        int mid = (low + high) / 2;
        while (u < knots[mid] || u >= knots[mid + 1])
        {
            if (u < knots[mid])
            {
                high = mid;
            }
            else
            {
                low = mid;
            }
            mid = (low + high) / 2;
        }
        return mid;
    }

    /**
     * Compute the nonvanishing basis functions values of parameter u, and store all the function
     * values into `func_values`. `func_values` store B basis function values of
     * N(`i_knot_span` - `degree`, `degree`) -> N(`i_knot_span`, `degree`).
     * See: *The NURBS Book* Algorithm A2.2
     * @param span the index of the knot span containing u
     * @param u the parameter
     * @param func_values pre alloced array to store basic functions values. _Dt[degree + 1]
     */
    void basis_funcs(int span, _Dt u, _Dt* func_values) const
    {
        _Dt saved, temp;

        func_values[0] = _Dt(1.0);

        for (int j = 1; j <= _degree; j++)
        {
            _left[j] = u - knots[span + 1 - j];
            _right[j] = knots[span + j] - u;
            saved = 0.0;
            for (int r = 0; r < j; r++)
            {
                temp = func_values[r] / (_right[r + 1] + _left[j - r]);
                func_values[r] = saved + _right[r + 1] * temp;
                saved = _left[j - r] * temp;
            }
            func_values[j] = saved;
        }
    }

    /**
     * Compute nonzero basis functions and their derivatives. First section is A2.2 modified
     * to store functions and knot differences.
     * See: *The NURBS Book* Algorithm A2.3
     * @param span the index of the knot span containing u
     * @param u the parameter
     * @param der_order the max derivative order
     * @param ders pre-alloced two dimensional array to store the basis functions and their derivatives.
     * _Dt[der_order + 1][degree + 1]
     */
    void ders_basis_funcs(int span, _Dt u, int der_order, _Dt** ders) const
    {
        _Dt saved, temp;

        _ndu[0][0] = _Dt(1.0);
        for (int j = 1; j <= _degree; j++)
        {
            _left[j] = u - knots[span + 1 - j];
            _right[j] = knots[span + j] - u;
            saved = 0.0;
            for (int r = 0;  r < j; r++)
            {
                // Lower triangle
                _ndu[j][r] = _right[r + 1] + _left[j - r];
                temp = _ndu[r][j - 1] / _ndu[j][r];

                // Upper triangle
                _ndu[r][j] = saved + _right[r + 1] * temp;
                saved = _left[j - r] * temp;
            }
            _ndu[j][j] = saved;
        }

        // load the basis functions
        for (int j = 0; j <= _degree; j++)
        {
            ders[0][j] = _ndu[j][_degree];
        }

        // Alternate rows in array a
        int s1, s2;

        int rk, pk;
        int j1, j2;
        _Dt d;
        int temp_swap;

        // This section computes the derivatives (Eq. [2.9])
        for (int r = 0; r <= _degree; r++)
        {
            s1 = 0;
            s2 = 1;
            _a[0][0] = _Dt(1.0);

            // Loop to compute kth derivative, k = 1, 2, ..., der_order
            for (int k = 1; k <= der_order; k++)
            {
                d = _Dt(0.0);
                rk = r - k;
                pk = _degree - k;

                if (r >= k)
                {
                    _a[s2][0] = _a[s1][0] / _ndu[pk + 1][rk];
                    d = _a[s2][0] * _ndu[rk][pk];
                }

                if (rk >= -1)
                {
                    j1 = 1;
                }
                else
                {
                    j1 = -rk;
                }

                if (r - 1 <= pk)
                {
                    j2 = k - 1;
                }
                else
                {
                    j2 = _degree - r;
                }

                for (int j = j1; j <= j2; j++)
                {
                    _a[s2][j] = (_a[s1][j] - _a[s1][j - 1]) / _ndu[pk + 1][rk + j];
                    d += _a[s2][j] * _ndu[rk + j][pk];
                }
                if (r <= pk)
                {
                    _a[s2][k] = -_a[s1][k - 1] / _ndu[pk + 1][r];
                    d += _a[s2][k] * _ndu[r][pk];
                }
                ders[k][r] = d;

                // swap row
                temp_swap = s1;
                s1 = s2;
                s2 = temp_swap;
            }
        }

        // Multiply through by the correct factors (Eq. [2.9])
        int r = _degree;
        for (int k = 1; k <= der_order; k++)
        {
            for (int j = 0; j <= _degree; j++)
            {
                ders[k][j] *= r;
            }
            r *= (_degree - k);
        }

    }
};


#endif //HELLO_BSPLINEFUNCTION_H
