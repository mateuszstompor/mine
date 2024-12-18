//
//  bitmap.h
//
//  Created on 18/12/2024.
//  Copyright © 2024 Mateusz Stompór. All rights reserved.
//

#include <cstdint>
#include <vector>

struct Bitmap {
    const uint16_t width;
    const uint16_t height;
    const uint8_t bytesPerPixel;
    std::vector<uint8_t> data;
    
    Bitmap(uint16_t width, uint16_t height, uint8_t bytesPerPixel)
    : width{width}
    , height{height}
    , bytesPerPixel{bytesPerPixel}
    , data(static_cast<int>(width) * static_cast<int>(height) * static_cast<int>(bytesPerPixel))
    {
        // Empty
    }
};
