#pragma once


class ColorRect : public Drawable, public IColorable
{
public:
    ColorRect(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
        bool useRectTransform = false
    );
    ~ColorRect() override = default;

    void Update() override;
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();
    void UpdateVertexColors();

private:
    std::vector<VertexColor> _vertices;
    std::vector<UINT> _indices;
    DirectX::SimpleMath::Color _color;
};
