
#include "Framework.h"
#include "CircleCollider.h"

void CircleCollider::Update()
{
    if (GetOwner())
    {
        DirectX::SimpleMath::Vector3 ownerPosition = GetOwner()->GetComponent<Transform>("Transform")->GetPosition();
        center = ownerPosition;
    }
}

void CircleCollider::Render()
{
    if (!CollisionManager::Get()->GetColliderDebug()) return;

    // 화면 좌표로 변환 (카메라가 이동한 만큼 빼기)
    DirectX::SimpleMath::Vector3 screenCenter = center - cameraPos;

    // 기즈모 그리기
    // size(박스 크기)는 그대로, center만 screenCenter로
    GizmoManager::Get()->DrawCircle(screenCenter, radius, color, 1.0f);
}
