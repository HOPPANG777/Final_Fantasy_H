#pragma once

class Drawable : public Component
{
public:
    Drawable(
        const std::string& name,
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& scale,
        float rotation,
        bool RectTransformType,
        const std::wstring& shaderPath = L""
    );

    Drawable(const Drawable& other)
        : Drawable(
            other.name,
            other.initialPosition,
            other.initialScale,
            other.initialRotation,
            other.RectTransformType,
            other.shaderPath
        )
    {

    }

    Drawable& operator=(const Drawable& other)
    {
        if (this != &other)
        {
            name = other.name;
            RectTransformType = other.RectTransformType;
            shaderPath = other.shaderPath;

            initialPosition = other.initialPosition;
            initialScale = other.initialScale;
            initialRotation = other.initialRotation;
            pendingTransformApply = other.pendingTransformApply;
        }
        return *this;
    }

    virtual ~Drawable() override = default;

public:
    void Update() override;
    void Render() override;

    void GUI() override;

    void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, bool indexed = true);

private:
    void ApplyPendingTransform();

protected:
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> indexBuffer;

    std::shared_ptr<VertexShader> vertexShader;

    std::shared_ptr<InputLayout> inputLayout;

    std::shared_ptr<PixelShader> pixelShader;

    std::wstring shaderPath = L"";

    bool RectTransformType = false;

    DirectX::SimpleMath::Vector3 initialPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 initialScale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
    float initialRotation = 0.0f;
    bool pendingTransformApply = false;

};

