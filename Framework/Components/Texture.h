#pragma once

// Texture Ʈ 2D ؽó ȭ鿡 ϱ  Drawable Ļ Ŭ.
// ؽó ڿ UV ǥ ϸ ܺ SRV ü Ѵ.

class Texture : public Drawable, public IColorable
{
public:
    Texture(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        const std::wstring& texturePath = L"",
        bool useRectTransform = false,
        std::string componentName = "Texture"
    );
    ~Texture() override = default;

    // Drawable ⺻ Ʈ ܿ ߰  .
    void Update() override;
    // ؽó ¸ ̴ ε  DrawCall Ѵ.
    void Render() override;

public:
    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

    //  γ ܺ ҽ ̿ ؽó üѴ.
    void SetTexture(const std::wstring& filePath);
    // ܺο  ShaderResourceView    ִ.
    void SetSRV(ID3D11ShaderResourceView* shaderResourceView);

    void SetGeometry(const std::vector<VertexTexture>& vertices, const std::vector<UINT>& indices); //     
    void SetShapeFromPositions(
        const std::vector<DirectX::SimpleMath::Vector3>& positions,
        const std::vector<UINT>& indices
    ); // ġ ͸ ̿ UV ڵ 
    void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology); //   

protected:
    std::vector<VertexTexture>& AccessVertices(); //  ؽ ͸   
    void CommitVertices();                  //  ؽ ͸ GPU ۿ ݿ

private:
    void InitializeGeometry();            // ⺻ 簢   
    void InitializeStates();             // Ͷ, , ÷  ʱȭ
    void UpdateVertexBuffer();           // ؽ ۸ ϰų 
    void UpdateIndexBuffer();            // ε ۸ ϰų 
    bool CreateTextureFromImage(const std::wstring& filePath); // ̹  ε ؽó 

private:
    std::vector<VertexTexture> _vertices;
    std::vector<UINT> _indices;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> _blendStateAlpha;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> _samplerLinear;

    std::unique_ptr<ColorBuffer> _colorBuffer;

    D3D11_PRIMITIVE_TOPOLOGY _topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    bool _useIndices = true;
};

