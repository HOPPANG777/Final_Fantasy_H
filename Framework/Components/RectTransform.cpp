#include "Framework.h"
#include "RectTransform.h"

namespace
{
    constexpr float kMinAnchor = 0.0f;
    constexpr float kMaxAnchor = 1.0f;
}

void RectTransform::Update()
{
    using namespace DirectX::SimpleMath;

    const Vector2 screenSize(static_cast<float>(Values::GetWinWidth()), static_cast<float>(Values::GetWinHeight()));
    Vector3 translation = position;

    const Vector3 anchorOffset(anchor.x * screenSize.x, anchor.y * screenSize.y, 0.0f);
    translation += anchorOffset;

    const Vector2 pivotOffset((pivot.x - 0.5f) * size.x, (pivot.y - 0.5f) * size.y);
    translation.x -= pivotOffset.x;
    translation.y -= pivotOffset.y;

    translation += CameraManager::Get()->GetPosition();

    const Matrix S = DirectX::XMMatrixScalingFromVector(scale);
    const Matrix R = DirectX::XMMatrixRotationZ(rotation);
    const Matrix T = DirectX::XMMatrixTranslationFromVector(translation);

    world = S * R * T;
}

void RectTransform::Render()
{
    // RectTransform does not render anything by itself.
}

const DirectX::SimpleMath::Matrix& RectTransform::GetWorld() const
{
    return world;
}

RECT RectTransform::GetRect() const
{
    using namespace DirectX::SimpleMath;

    const Vector2 screenSize(static_cast<float>(Values::GetWinWidth()), static_cast<float>(Values::GetWinHeight()));
    const Vector2 scaledSize(size.x * scale.x, size.y * scale.y);

    Vector2 position2D(position.x, position.y);
    position2D += Vector2(anchor.x * screenSize.x, anchor.y * screenSize.y);

    const Vector2 pivotOffset(pivot.x * scaledSize.x, pivot.y * scaledSize.y);
    Vector2 topLeft = position2D - pivotOffset;

    const DirectX::SimpleMath::Vector3 cameraPos = CameraManager::Get()->GetPosition();
    topLeft += Vector2(cameraPos.x, cameraPos.y);

    const Vector2 bottomRight = topLeft + scaledSize;

    RECT rect{};
    rect.left = static_cast<LONG>(topLeft.x);
    rect.top = static_cast<LONG>(topLeft.y);
    rect.right = static_cast<LONG>(bottomRight.x);
    rect.bottom = static_cast<LONG>(bottomRight.y);

    return rect;
}

DirectX::SimpleMath::Vector3 RectTransform::GetPosition() const
{
    return position;
}

DirectX::SimpleMath::Vector2 RectTransform::GetAnchoredPosition() const
{
    return DirectX::SimpleMath::Vector2(position.x, position.y);
}

DirectX::SimpleMath::Vector3 RectTransform::GetScale() const
{
    return scale;
}

float RectTransform::GetRotation() const
{
    return DirectX::XMConvertToDegrees(rotation);
}

DirectX::SimpleMath::Vector2 RectTransform::GetAnchor() const
{
    return anchor;
}

DirectX::SimpleMath::Vector2 RectTransform::GetPivot() const
{
    return pivot;
}

DirectX::SimpleMath::Vector2 RectTransform::GetSize() const
{
    return size;
}

void RectTransform::SetPosition(const DirectX::SimpleMath::Vector3& pos)
{
    position = pos;
}

void RectTransform::SetAnchoredPosition(const DirectX::SimpleMath::Vector2& pos)
{
    position.x = pos.x;
    position.y = pos.y;
}

void RectTransform::SetScale(const DirectX::SimpleMath::Vector3& sca)
{
    scale = sca;
}

void RectTransform::SetRotation(float degrees)
{
    rotation = DirectX::XMConvertToRadians(degrees);
}

void RectTransform::SetAnchor(const DirectX::SimpleMath::Vector2& value)
{
    anchor.x = CLAMP(value.x, kMinAnchor, kMaxAnchor);
    anchor.y = CLAMP(value.y, kMinAnchor, kMaxAnchor);
}

void RectTransform::SetPivot(const DirectX::SimpleMath::Vector2& value)
{
    pivot.x = CLAMP(value.x, kMinAnchor, kMaxAnchor);
    pivot.y = CLAMP(value.y, kMinAnchor, kMaxAnchor);
}

void RectTransform::SetSize(const DirectX::SimpleMath::Vector2& value)
{
    size.x = max(0.0f, value.x);
    size.y = max(0.0f, value.y);
}

void RectTransform::Move(const DirectX::SimpleMath::Vector3& speed)
{
    position += speed * TimeManager::Get()->GetDeltaTime();
}

void RectTransform::Scale(const DirectX::SimpleMath::Vector3& value)
{
    scale += value * TimeManager::Get()->GetDeltaTime();
}

void RectTransform::Rotate(float degrees)
{
    rotation += DirectX::XMConvertToRadians(degrees) * TimeManager::Get()->GetDeltaTime();
}