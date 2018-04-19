//
// Created by haochuanchen on 18-4-19.
//

#include "KTPFitting.h"

std::vector<KTPFitting::_Dt> KTPFitting::select_knot_vector()
{
    const auto& vertices = _src_curve.get_vertices();

    int m = vertices.size() - 1;
    int n = _n;
    int p = _degree;

    _Dt d = _Dt(m + 1) / _Dt(n - p + 1);

    std::vector<_Dt> knots(n + p + 2);

    // first p + 1 knots: 0.0
    for (int j = 0; j <= p; j++)
    {
        knots[j] = 0.0;
    }

    _Dt alpha;

    int i_span;

    // middle knots
    for (int j = 1; j <= n - p; j++)
    {

        i_span = static_cast<int>(std::floor(j * d));

        alpha = j * d - i_span;
        knots[p + j] = (1 - alpha) * vertices[i_span - 1].trait.u + alpha * vertices[i_span].trait.u;
    }

    // last p + 1 knots: 1.0
    for (int j = n + 1; j <= n + p + 1; j++)
    {
        knots[j] = 1.0;
    }
    return knots;
}