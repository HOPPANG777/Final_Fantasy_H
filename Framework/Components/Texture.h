#pragma once

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

    void Update() override;
    void Render() override;

public:
    DirectX::SimpleMath::Color GetColor() const override;
    void SetColor(const DirectX::SimpleMath::Color& color) override;

    void SetTexture(const std::wstring& filePath);
    void SetSRV(ID3D11ShaderResourceView* shaderResourceView);

    ID3D11ShaderResourceView* GetShaderResourceView() const { return _shaderResourceView.Get(); }

    void SetGeometry(const std::vector<VertexTexture>& vertices, const std::vector<UINT>& indices);
    void SetShapeFromPositions(
        const std::vector<DirectX::SimpleMath::Vector3>& positions,
        const std::vector<UINT>& indices
    );
    void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY& topology);

protected:
    std::vector<VertexTexture>& AccessVertices();
    void CommitVertices();

private:
    void InitializeGeometry();
    void InitializeStates();
    void UpdateVertexBuffer();
    void UpdateIndexBuffer();
    bool CreateTextureFromImage(const std::wstring& filePath);

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

