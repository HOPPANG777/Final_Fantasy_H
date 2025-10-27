#pragma once

// ColorLine Ʈ   մ ÷  ׸  Ѵ.
//    ؽ   Ѵ.

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

    //  ġ    Ƿ ⺻ Update Ѵ.
    void Update() override;
    //  Ƽ ϱ  DrawCall ȣѴ.
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();    //   ϴ ⺻ ؽ 
    void UpdateVertexColors();    // ؽ ۿ    

private:
    std::vector<VertexColor> _vertices; //   Ǵ ؽ 
    DirectX::SimpleMath::Color _color;    //  ü  
};
