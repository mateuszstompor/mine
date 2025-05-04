//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "triangle.h"
#include "../assertion/finite.h"
#include "../assertion/perpendicular.h"
#include "../assertion/normalized.h"

mine::Triangle::Triangle(mine::Vertex const & inV0,
                         mine::Vertex const & inV1,
                         mine::Vertex const & inV2) {
    v0 = inV0.position;
    v1 = inV1.position;
    v2 = inV2.position;
    
    uv0 = inV0.uv;
    uv1 = inV1.uv;
    uv2 = inV2.uv;

    normal = simd::normalize(inV0.normal + inV1.normal + inV2.normal);
    tangent = simd::normalize(inV0.tangent + inV1.tangent + inV2.tangent);
    bitangent = simd::normalize(inV0.bitangent + inV1.bitangent + inV2.bitangent);

    assertFinite(v0);
    assertFinite(v1);
    assertFinite(v2);

    assertFinite(uv0);
    assertFinite(uv1);
    assertFinite(uv2);

    assertFinite(tangent);
    assertFinite(bitangent);
    assertFinite(normal);
    
    assertNormalized(tangent);
    assertNormalized(bitangent);
    assertNormalized(normal);
    
    assertPerpendicular(normal, tangent);
    assertPerpendicular(normal, bitangent);
    assertPerpendicular(tangent, bitangent);
}

mine::Triangle::Triangle(const std::array<simd::float3, 3>& vertices,
                         const std::array<simd::float2, 3>& uvsInput) {
    v0 = vertices[0];
    v1 = vertices[1];
    v2 = vertices[2];
    
    uv0 = uvsInput[0];
    uv1 = uvsInput[1];
    uv2 = uvsInput[2];
    
    const auto edge1 = v1 - v0;
    const auto edge2 = v2 - v0;
    const auto deltaUV1 = uv1 - uv0;
    const auto deltaUV2 = uv2 - uv0;

    const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    simd::float3 rawTangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
    simd::float3 rawBitangent = f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);
    
    normal = simd::normalize(simd::cross(edge1, edge2));

    // Orthonormalize tangent against normal
    tangent = simd::normalize(rawTangent - normal * simd::dot(normal, rawTangent));
    
    // Recompute bitangent to guarantee orthogonality and correct handedness
    bitangent = simd::cross(normal, tangent);

    // Fix handedness if needed (preserve original bitangent direction)
    if (simd::dot(bitangent, rawBitangent) < 0.0f) {
        bitangent = -bitangent;
    }

    // Sanity checks
    assertFinite(v0);
    assertFinite(v1);
    assertFinite(v2);

    assertFinite(uv0);
    assertFinite(uv1);
    assertFinite(uv2);

    assertFinite(tangent);
    assertFinite(bitangent);
    assertFinite(normal);

    assertNormalized(tangent);
    assertNormalized(bitangent);
    assertNormalized(normal);

    assertPerpendicular(normal, tangent);
    assertPerpendicular(normal, bitangent);
    assertPerpendicular(tangent, bitangent);
}
