// Drawable δ ȯ  ο ȣ Ѵ.

#include "Framework.h"
#include "Drawable.h"

Drawable::Drawable(
    const std::string& name,
    const DirectX::SimpleMath::Vector3& position,
    const DirectX::SimpleMath::Vector3& scale,
    float rotation,
    bool RectTransformType,
    const std::wstring& shaderPath
)
    : Component(name), shaderPath(shaderPath), RectTransformType(RectTransformType)
{
    initialPosition = position;
    initialScale = scale;
    initialRotation = rotation;
    pendingTransformApply = true;

    vertexBuffer = std::make_unique<VertexBuffer>();

    vertexShader = std::make_unique<VertexShader>();
    vertexShader->Create(shaderPath, "VS");

    indexBuffer = std::make_unique<IndexBuffer>();

    inputLayout = std::make_unique<InputLayout>();

    pixelShader = std::make_unique<PixelShader>();
    pixelShader->Create(shaderPath, "PS");
}

void Drawable::Update()
{
    // ʱ ġ, ȸ, ũ⸦   Ѵ.
    ApplyPendingTransform();
}

void Drawable::Render()
{
    //  ʿ ۿ ̴ εѴ.
    vertexBuffer->SetIA();
    indexBuffer->SetIA();
    inputLayout->SetIA();

    vertexShader->SetShader();

    pixelShader->SetShader();
}

void Drawable::DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, bool indexed)
{
    // ϴ    ε ο  Draw Լ ȣѴ.
    GraphicsManager::Get()->GetDC()->IASetPrimitiveTopology(topology);

    if (indexed) GraphicsManager::Get()->GetDC()->DrawIndexed(indexBuffer->GetCount(), 0, 0);
    else GraphicsManager::Get()->GetDC()->Draw(vertexBuffer->GetCount(), 0);
}

void Drawable::ApplyPendingTransform()
{
    // ʱȭ Ŀ ٽ  ʴ´.
    if (!pendingTransformApply) return;

    GameObject* ownerObject = GetOwner();
    //  Ʈ   Transform .
    if (!ownerObject) return;

    // RectTransform  ο  бѴ.
    if (RectTransformType == true)
    {
        const std::shared_ptr<RectTransform>& rectTransform = ownerObject->GetRectTransform();
        if (rectTransform)
        {
            rectTransform->SetPosition(initialPosition);
            rectTransform->SetScale(initialScale);
            rectTransform->SetRotation(initialRotation);
            pendingTransformApply = false;
        }
    }
    else
    {
        const std::shared_ptr<Transform>& transform = ownerObject->GetTransform();
        if (transform)
        {
            transform->SetPosition(initialPosition);
            transform->SetScale(initialScale);
            transform->SetRotation(initialRotation);
            pendingTransformApply = false;
        }
    }
}
