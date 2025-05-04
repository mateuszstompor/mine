//
//  Copyright © 2025 Mateusz Stompór. All rights reserved.
//

#include "triangle.h"
#include "../assertion/finite.h"

Triangle::Triangle(mine::Vertex const & inV0, mine::Vertex const & inV1, mine::Vertex const & inV2) {
    v0 = inV0.position;
    v1 = inV1.position;
    v2 = inV2.position;
    
    uv0 = inV0.uv;
    uv1 = inV1.uv;
    uv2 = inV2.uv;

    normal = simd::normalize(inV0.normal + inV1.normal + inV2.normal);
    tangent = simd::normalize(inV0.tangent + inV1.tangent + inV2.tangent);
    bitangent = simd::normalize(inV0.bitangent + inV1.bitangent + inV2.bitangent);

    mine::assertFinite(v0);
    mine::assertFinite(v1);
    mine::assertFinite(v2);

    mine::assertFinite(uv0);
    mine::assertFinite(uv1);
    mine::assertFinite(uv2);

    mine::assertFinite(tangent);
    mine::assertFinite(bitangent);
    mine::assertFinite(normal);
}


Triangle::Triangle(const std::array<simd::float3, 3>& vertices,
                   const std::array<simd::float2, 3>& uvsInput) {
    v0 = vertices[0];
    v1 = vertices[1];
    v2 = vertices[2];
    
    uv0 = uvsInput[0];
    uv1 = uvsInput[1];
    uv2 = uvsInput[2];
    
    auto v0v1 = v1 - v0;
    auto v0v2 = v2 - v0;
    auto crossed = simd::cross(v0v1, v0v2);
    
    auto edge1 = vertices[1] - vertices[0];
    auto edge2 = vertices[2] - vertices[0];
    auto deltaUV1 = uv1 - uv0;
    auto deltaUV2 = uv2 - uv0;
    
    float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    
    simd::float3 tangent1;
    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    
    simd::float3 bitangent1;
    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    
    tangent = simd::normalize(tangent1);
    bitangent1 = simd::normalize(bitangent1);
    
    normal = simd::normalize(crossed);
    auto Bprim = simd::cross(normal, tangent);
    if (simd::dot(Bprim, bitangent1) < 0) {
        bitangent = -bitangent1;
    } else {
        bitangent = bitangent1;
    }
    
    mine::assertFinite(v0);
    mine::assertFinite(v1);
    mine::assertFinite(v2);
    
    mine::assertFinite(uv0);
    mine::assertFinite(uv1);
    mine::assertFinite(uv2);
    
    mine::assertFinite(tangent);
    mine::assertFinite(bitangent);
    mine::assertFinite(normal);
}
