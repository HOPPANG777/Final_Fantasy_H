#pragma once


class ColorLine : public Drawable, public IColorable
{
public:
    ColorLine(
        const DirectX::SimpleMath::Vector3& position,
        float length,
        float rotation,
        const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
        bool useRectTransform = false
    );

    void Update() override;
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();
    void UpdateVertexColors();

private:
    std::vector<VertexColor> _vertices;
    DirectX::SimpleMath::Color _color;
};
