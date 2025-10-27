#pragma once

// Drawable   Ʈ  ϴ  Ŭ.
// /ε ۿ ̴ ʱȭϰ Ʈ  óѴ.

class Drawable : public Component
{
public:
    //   ⺻ Ʈ, /̴ ʱȭ Ʈ Ѵ.
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
    // Transform ȭ   û óѴ.
    void Update() override;
    // ̴ ۸ εϰ  DrawCall Ѵ.
    void Render() override;

    void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, bool indexed = true); //   ο

private:
    // Transform  ž   ó
    void ApplyPendingTransform();

protected:
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;

    std::unique_ptr<VertexShader> vertexShader;

    std::unique_ptr<InputLayout> inputLayout;

    std::unique_ptr<PixelShader> pixelShader;

    std::wstring shaderPath = L"";

    bool RectTransformType = false;

    DirectX::SimpleMath::Vector3 initialPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 initialScale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);
    float initialRotation = 0.0f;
    bool pendingTransformApply = false;

};

