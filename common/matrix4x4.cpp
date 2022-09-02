#include <elpch.h>
#include "matrix4x4.h"

namespace el
{
    constexpr matrix4x4::matrix4x4() : data{} {}
    constexpr matrix4x4::matrix4x4(float f) 
        : data{ f, 0.0f, 0.0f, 0.0f,  0.0f, f, 0.0f, 0.0f,  0.0f, 0.0f, f, 0.0f,  0.0f, 0.0f, 0.0f, f } {}
    constexpr matrix4x4::matrix4x4(float c11, float c12, float c13, float c14, float c21, float c22, float c23, float c24, float c31, float c32, float c33, float c34, float c41, float c42, float c43, float c44) :
        data{ c11, c12, c13, c14, c21, c22, c23, c24, c31, c32, c33, c34, c41, c42, c43, c44 } {
    }

    void matrix4x4::makeIndentity() {
        memcpy(data, s_identity, sizeof(float) * 16);
    }

    constexpr matrix4x4 matrix4x4::operator*(const matrix4x4& m) const {
        matrix4x4 res;
        auto& r = res.data;
        sizet it = 0;
        for (sizet i = 0; i < 4; i++) {
            for (sizet j = 0; j < 4; j++) {
                r[it] =
                    data[j] * m.data[i * 4] +
                    data[j + 4] * m.data[i * 4 + 1] +
                    data[j + 8] * m.data[i * 4 + 2] +
                    data[j + 12] * m.data[i * 4 + 3];
                it++;
            }
        }
        return res;
    }
}