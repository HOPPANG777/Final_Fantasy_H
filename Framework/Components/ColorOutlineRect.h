#pragma once

class ColorOutlineRect : public Drawable, public IColorable
{
public:
    ColorOutlineRect(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
        bool useRectTransform = false
    );

    void Update() override;
    void Render() override;

    void GUI() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();
    void UpdateVertexColors();

private:
    std::vector<VertexColor> _vertices;
    DirectX::SimpleMath::Color _color;

    bool isOpen = false;
};
