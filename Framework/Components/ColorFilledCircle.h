#pragma once

// ColorFilledCircle ä  ¸ ׸ Drawable Ʈ.
// ߽ɰ   ﰢ  ؽ Ѵ.

class ColorFilledCircle : public Drawable, public IColorable
{
public:
    ColorFilledCircle(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        size_t segments,
        const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
        bool useRectTransform = false
    );

    // Drawable ⺻  ܿ ߰  .
    void Update() override;
    // ä  ϱ  DrawCall Ѵ.
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();    // ﰢ  · ä  ؽ 
    void UpdateVertexColors();    //  ٲ  ؽ ÷ 

private:
    std::vector<VertexColor> _vertices; // ä  ϴ ؽ 
    std::vector<UINT> _indices;
    size_t _segments = 0;
    DirectX::SimpleMath::Color _color;   //  ä 
};
