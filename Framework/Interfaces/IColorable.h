#pragma once

//  ٷ ü ؾ ϴ ̽Դϴ.
class IColorable
{
public:
    virtual ~IColorable() = default; // ̽  ü ϰ Ҹǵ մϴ.

    virtual void SetColor(const DirectX::SimpleMath::Color& color) = 0; // ü ǥ  մϴ.
    virtual DirectX::SimpleMath::Color GetColor() const = 0;            //    ȯմϴ.
};
