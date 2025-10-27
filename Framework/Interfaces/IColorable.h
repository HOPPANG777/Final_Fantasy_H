#pragma once

class IColorable
{
public:
    virtual ~IColorable() = default;

    virtual void SetColor(const DirectX::SimpleMath::Color& color) = 0;
    virtual DirectX::SimpleMath::Color GetColor() const = 0;
};
