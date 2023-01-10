//
// Created by Paul Walker on 1/10/23.
//

#ifndef JI_MTS_SOURCE_JIMATH_H
#define JI_MTS_SOURCE_JIMATH_H

#include <utility>

struct JIMath
{
    uint64_t horizNum{3}, horizDen{2}, diagNum{5}, diagDen{4};

    JIMath(uint64_t horizN = 3, uint64_t horizD = 2, uint64_t diagN = 5, uint64_t diagD = 4)
        : horizNum(horizN), horizDen(horizD), diagNum(diagN), diagDen(horizD)
    {
    }

    // Maybe move these to the tuning library on Tones one day?
    // 3/2 up by 3/2 is 9/4
    std::pair<uint64_t, uint64_t> fracMul(uint64_t N1, uint64_t D1, uint64_t N2, uint64_t D2) {
        assert(N1 >= D1);
        assert(N2 >= D2);
        auto nR = N1 * N2;
        auto dR = D1 * D2;

        // Take out octaves until we are within [1,2]
        while (nR > 2 * dR)
        {
            dR *= 2;
        }
        return {nR, dR};
    }

    std::pair<uint64_t, uint64_t> fracDiv(uint64_t N1, uint64_t D1, uint64_t N2, uint64_t D2) {
        assert(N1 >= D1);
        assert(N2 >= D2);

        auto nR = N1 * D2;
        auto dR = D1 * N2;

        // Add in octaves until we are within [1,2]
        while (nR < dR)
        {
            nR *= 2;
        }
        return {nR, dR};
    }
};

#endif // JI_MTS_SOURCE_JIMATH_H
