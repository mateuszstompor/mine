//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include <cmath>

namespace mine {
    inline float degreesToRadians(float angleDegrees) {
        return angleDegrees * M_PI / 180.0f;
    }

    inline float radiansToDegrees(float angleRadians) {
        return angleRadians * 180.0f / M_PI;
    }
}
