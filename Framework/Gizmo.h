#pragma once

class Gizmo
{
    DECLARE_SINGLETON(Gizmo)

public:
    // 도형 그리기
    static void DrawBox(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& size, const DirectX::SimpleMath::Color& color, float thickness = 1.0f);
    static void DrawCircle(const DirectX::SimpleMath::Vector3& center, float radius, const DirectX::SimpleMath::Color& color, float thickness = 1.0f);
    static void DrawLine(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, const DirectX::SimpleMath::Color& color, float thickness = 1.0f);
};

