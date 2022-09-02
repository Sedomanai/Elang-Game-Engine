#include <elpch.h>
#include "math.h"

namespace el
{
    namespace el_math
    {
        float normalizeAngle(float degrees) {
            // reduce the angle  
            degrees = fmod(degrees, 360.f);

            // force it to be the positive remainder, so that 0 <= angle < 360  
            degrees = fmod(degrees + 360.0f, 360.0f);

            // force into the minimum absolute value residue class, so that -180 < angle <= 180  
            if (degrees > 180)
                degrees -= 360;

            return degrees;
        }

        /**
         * @brief For more loose float comparions.
         */
        bool nearly_equal(float a, float b, float epsilon, float relth) {
            assert(std::numeric_limits<float>::epsilon() <= epsilon);
            assert(epsilon < 1.f);

            if (a == b) return true;

            auto diff = std::abs(a - b);
            auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<float>::max());
            // or even faster: std::min(std::abs(a + b), std::numeric_limits<float>::max());
            // keeping this commented out until I update figures below
            return diff < std::max(relth, epsilon* norm);
        }
    }
}