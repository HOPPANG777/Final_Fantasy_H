#pragma once

// ColorRect ܻ ä 簢 ׸ Drawable Ļ Ŭ.
// ũ   ̿ 簢 ؽ Ѵ.

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

    // 簢 Transform ȭ ݿϸ ǹǷ ⺻ Update ȣѴ.
    void Update() override;
    // 簢 ä  ﰢ Ʈ ο츦 Ѵ.
    void Render() override;

    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

private:
    void InitializeGeometry();    // ܻ 簢  ⺻ ؽ 
    void UpdateVertexColors();    //    ؽ ۸ 

private:
    std::vector<VertexColor> _vertices; // 簢 ϴ ؽ 
    std::vector<UINT> _indices;         // 簢 ׸  ε
    DirectX::SimpleMath::Color _color;   // 簢 ä 
};
