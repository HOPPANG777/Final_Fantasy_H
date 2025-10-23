#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ImageLoader
{
    struct LoadedImage
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        std::vector<uint8_t> pixels;
    };

    bool Load(const std::wstring& filePath, LoadedImage& outImage);
    bool Load(const std::string& filePath, LoadedImage& outImage);
}

