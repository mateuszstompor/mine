//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

namespace mine {
    inline std::pair<bool, bool> refract(const simd::float3& I, const simd::float3& N, float ior, simd::float3& outRefracted) {
        float cosi = simd::clamp(simd::dot(I, N), -1.0f, 1.0f);
        float etai = 1.0f, etat = ior;
        simd::float3 n = N;
        
        bool swap = false;
        if (cosi < 0.0f) {
            cosi = -cosi;
        } else {
            std::swap(etai, etat);
            n = -N;
            swap = true;
        }
        
        float eta = etai / etat;
        float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
        
        if (k < 0) {
            return std::make_pair(false, false); // Total internal reflection
        }
        
        outRefracted = eta * I + (eta * cosi - sqrtf(k)) * n;
        return std::make_pair(true, swap);
    }

    inline float fresnel(const simd::float3& I, const simd::float3& N, float ior) {
        float cosi = simd::clamp(simd::dot(I, N), -1.0f, 1.0f);
        float etai = 1.0f;
        float etat = ior;
        
        if (cosi > 0.0f) {
            std::swap(etai, etat);
        }
        
        // Compute sine of transmission angle using Snell’s law
        float sint = etai / etat * sqrtf(fmaxf(0.0f, 1.0f - cosi * cosi));
        
        // Total internal reflection
        if (sint >= 1.0f) {
            return 1.0f;
        }
        
        float cost = sqrtf(fmaxf(0.0f, 1.0f - sint * sint));
        cosi = fabsf(cosi);
        
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        return (Rs * Rs + Rp * Rp) * 0.5f;
    }
}
