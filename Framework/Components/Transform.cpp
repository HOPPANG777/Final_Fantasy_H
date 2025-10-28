#include "Framework.h"
#include "Transform.h"

void Transform::Update()
{
    // S, R, T 행렬 계산
    DirectX::SimpleMath::Matrix S = DirectX::XMMatrixScalingFromVector(scale);
    DirectX::SimpleMath::Matrix R = DirectX::XMMatrixRotationZ(rotation);
    DirectX::SimpleMath::Matrix T = DirectX::XMMatrixTranslationFromVector(position);

    world = S * R * T;
}

void Transform::Render()
{
    // Transform은 시각적으로 그릴 요소가 없으므로 보통 비웁니다.
    // (필요 시, 디버그용 기즈모 등으로 확장 가능)
}

void Transform::GUI()
{
    // ImGui를 이용해 UI를 만들어내는 기능과 코드들

    // ImGui 코드들을 이용해 UI를 만들기위해 먼저 실행해야하는 함수
    ImGui::Begin("Transform", &isOpen);
    {
        ImGui::Text("Transform");

        ImGui::SliderFloat3(
            "Position",
            (float*)&position,
            0,
            Values::GetWinWidth()
        );

        ImGui::SliderFloat3(
            "Scale",
            (float*)&scale,
            0,
            Values::GetWinWidth()
        );

        ImGui::SliderAngle(
            "Rotation",
            &rotation,
            0,
            360.0f
        );
    }
    // ImGui의 UI만드는게 끝났다는것을 알려줄 함수
    ImGui::End();
}

DirectX::SimpleMath::Vector3 Transform::GetPosition() const
{
    return position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale() const
{
    return scale;
}

float Transform::GetRotation() const
{
    // 내부는 라디안이지만, 외부엔 각도(도)로 반환
    return DirectX::XMConvertToDegrees(rotation);
}

const DirectX::SimpleMath::Matrix& Transform::GetWorld() const
{
    return world;
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& pos)
{
    position = pos;
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& sca)
{
    scale = sca;
}

void Transform::SetRotation(float degrees)
{
    rotation = DirectX::XMConvertToRadians(degrees);
}

void Transform::Move(const DirectX::SimpleMath::Vector3& speed)
{
    position += speed * TimeManager::Get()->GetDeltaTime();
}

void Transform::Rotate(float degrees)
{
    rotation += DirectX::XMConvertToRadians(degrees) * TimeManager::Get()->GetDeltaTime();
}

void Transform::Scale(const DirectX::SimpleMath::Vector3& scale)
{
    this->scale += scale * TimeManager::Get()->GetDeltaTime();
}
