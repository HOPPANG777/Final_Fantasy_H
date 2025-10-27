#pragma once

// ColorOutlineRect ܰ ִ 簢 ׸ Drawable Ʈ.
//      Ʈ ͸ Ѵ.

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

    // 簢 ܰ Transform ϹǷ ⺻ Update ȣѴ.
    void Update() override;
    //  Ʈ  ܰ ׸  DrawCall Ѵ.
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();    // 簢    ʱȭ
    void UpdateVertexColors();    // ׵θ    ؽ 

private:
    std::vector<VertexColor> _vertices; // 簢 ܰ ̷ ؽ 
    DirectX::SimpleMath::Color _color;   // ܰ 
};
