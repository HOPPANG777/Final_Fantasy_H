#pragma once

// ColorCircle  ̷  ׸ Drawable Ʈ.
//  ׸Ʈ      Ѵ.

class ColorCircle : public Drawable, public IColorable
{
public:
    ColorCircle(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        size_t segments,
        const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f),
        bool useRectTransform = false
    );

    // Drawable   θ Ŭ  Ѵ.
    void Update() override;
    //  Ʈ ܰ ׸  Render  DrawCall Ѵ.
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();    //  ؽ ε ʱȭ
    void UpdateVertexColors();    // ؽ ۿ   

private:
    std::vector<VertexColor> _vertices; //    ؽ Ʈ
    size_t _segments = 0;                //  ϴ ׸Ʈ 
    DirectX::SimpleMath::Color _color;   //    
};
