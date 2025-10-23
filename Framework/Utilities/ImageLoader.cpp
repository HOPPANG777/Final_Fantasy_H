#include "Framework.h"
#include "ImageLoader.h"

#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
    bool PopulateImageData(stbi_uc* data, int width, int height, int channels, ImageLoader::LoadedImage& outImage)
    {
        if (data == nullptr || width <= 0 || height <= 0 || channels <= 0)
        {
            return false;
        }

        outImage.width = width;
        outImage.height = height;
        outImage.channels = channels;

        const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
        outImage.pixels.assign(data, data + pixelCount);

        return true;
    }
}

bool ImageLoader::Load(const std::string& filePath, LoadedImage& outImage)
{
    outImage = {};

    if (filePath.empty())
    {
        return false;
    }

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (data == nullptr)
    {
        return false;
    }

    const bool success = PopulateImageData(data, width, height, 4, outImage);
    stbi_image_free(data);

    return success;
}

bool ImageLoader::Load(const std::wstring& filePath, LoadedImage& outImage)
{
    outImage = {};

    if (filePath.empty())
    {
        return false;
    }

#if defined(_WIN32)
    FILE* file = nullptr;
#if defined(_MSC_VER)
    _wfopen_s(&file, filePath.c_str(), L"rb");
#else
    file = _wfopen(filePath.c_str(), L"rb");
#endif
    if (file == nullptr)
    {
        return false;
    }

    int width = 0;
    int height = 0;
    int channels = 0;

    stbi_uc* data = stbi_load_from_file(file, &width, &height, &channels, STBI_rgb_alpha);
    fclose(file);

    if (data == nullptr)
    {
        return false;
    }

    const bool success = PopulateImageData(data, width, height, 4, outImage);
    stbi_image_free(data);

    return success;
#else
    return Load(String::ToString(filePath), outImage);
#endif
}

