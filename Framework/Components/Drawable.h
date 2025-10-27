#pragma once

// Drawable is the base class for components that submit draw calls.
// It manages GPU resources and handles the per-frame render invocation.

class Drawable : public Component
{
public:
    // Construct with initial transform data and the shader to use when drawing.
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
    // Apply any pending transform changes before the frame begins.
    void Update() override;
    // Issue the GPU draw call for the primitive.
    void Render() override;

    void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, bool indexed = true);

private:
    // Push pending transform information into the GPU resources.
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

