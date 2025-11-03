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

    vertexBuffer = std::make_shared<VertexBuffer>();

    vertexShader = std::make_shared<VertexShader>();
    vertexShader->Create(shaderPath, "VS");

    indexBuffer = std::make_shared<IndexBuffer>();

    inputLayout = std::make_shared<InputLayout>();

    pixelShader = std::make_shared<PixelShader>();
    pixelShader->Create(shaderPath, "PS");
}

void Drawable::Update()
{
    ApplyPendingTransform();
}

void Drawable::Render()
{
    vertexBuffer->SetIA();
    indexBuffer->SetIA();
    inputLayout->SetIA();

    vertexShader->SetShader();

    pixelShader->SetShader();
}

void Drawable::GUI()
{

}

void Drawable::DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, bool indexed)
{
    GraphicsManager::Get()->GetDC()->IASetPrimitiveTopology(topology);

    if (indexed) GraphicsManager::Get()->GetDC()->DrawIndexed(indexBuffer->GetCount(), 0, 0);
    else GraphicsManager::Get()->GetDC()->Draw(vertexBuffer->GetCount(), 0);
}

void Drawable::ApplyPendingTransform()
{
    if (!pendingTransformApply) return;

    GameObject* ownerObject = GetOwner();
    if (!ownerObject) return;

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