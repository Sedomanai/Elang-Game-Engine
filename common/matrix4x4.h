/*****************************************************************//**
 * @file   matrix4x4.h
 * @brief  matrix4 (not much used since transform.h)
 * 
 * @author Sedomanai
 * @date   September 2022
 *********************************************************************/
#pragma once
#include "../elang_builder.h"

namespace el
{
    using sizet = size_t;
    struct ELANG_DLL matrix4x4
    {
        float data[16];

        constexpr matrix4x4();
        constexpr matrix4x4(float f);
        constexpr matrix4x4(
            float c11, float c12, float c13, float c14, 
            float c21, float c22, float c23, float c24, 
            float c31, float c32, float c33, float c34, 
            float c41, float c42, float c43, float c44
        );
        void makeIndentity();
        constexpr matrix4x4 operator*(const matrix4x4& m) const;

        /**
         * For Cereal.
         * 
         * @param Cereal InArchive
         */
        template<typename T>
        void serialize(T& archive) {
            for (sizet i = 0; i < 16; i++) // optimize?..
                archive(data[i]);
        }
    private:
        static const constexpr float s_identity[16] = { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
    };
}