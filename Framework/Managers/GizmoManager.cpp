#include "Framework.h"
#include "GizmoManager.h"

GizmoManager::GizmoManager() {}

GizmoManager::~GizmoManager() {}

void GizmoManager::DrawBox(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& size, const DirectX::SimpleMath::Color& color, float thickness)
{
	if (!GraphicsManager::Get()->GetD2DC()) return;

	D2D1_RECT_F rect = { center.x - size.x / 2,gWinHeight - (center.y + size.y / 2),center.x + size.x / 2,gWinHeight - (center.y - size.y / 2) };

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	GraphicsManager::Get()->GetD2DC()->CreateSolidColorBrush(D2D1::ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	GraphicsManager::Get()->GetD2DC()->DrawRectangle(&rect, brush.Get(), thickness);
}

void GizmoManager::DrawCircle(const DirectX::SimpleMath::Vector3& center, float radius, const DirectX::SimpleMath::Color& color, float thickness)
{
	if (!GraphicsManager::Get()->GetD2DC()) return;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	GraphicsManager::Get()->GetD2DC()->CreateSolidColorBrush(D2D1::ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	D2D1_ELLIPSE ellipse = { { center.x, gWinHeight - center.y }, radius, radius };
	GraphicsManager::Get()->GetD2DC()->DrawEllipse(&ellipse, brush.Get(), thickness);
}

void GizmoManager::DrawLine(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, const DirectX::SimpleMath::Color& color, float thickness)
{
	if (!GraphicsManager::Get()->GetD2DC()) return;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	GraphicsManager::Get()->GetD2DC()->CreateSolidColorBrush(D2D1::ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	GraphicsManager::Get()->GetD2DC()->DrawLine(D2D1::Point2F(start.x, gWinHeight - start.y), D2D1::Point2F(end.x, gWinHeight - end.y), brush.Get(), thickness);
}


