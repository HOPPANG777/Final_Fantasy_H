#pragma once

class Values
{
public:
        static UINT GetWinWidth() { return static_cast<UINT>(gWinWidth); }
        static UINT GetWinHeight() { return static_cast<UINT>(gWinHeight); };
        static float GetCenterX() { return gWinWidth * 0.5f; };
        static float GetCenterY() { return gWinHeight * 0.5f; };
        static DirectX::SimpleMath::Vector3 GetCenter() { return DirectX::SimpleMath::Vector3(GetCenterX(), GetCenterY(), 0); };
        static DirectX::SimpleMath::Vector3 GetScreenSize() { return DirectX::SimpleMath::Vector3(gWinWidth, gWinHeight, 1); };

        static void SetReferenceResolution(float width, float height);
        static float GetReferenceWidth();
        static float GetReferenceHeight();
        static DirectX::SimpleMath::Vector2 GetReferenceResolution();
        static DirectX::SimpleMath::Vector2 GetUIScale();
        static float GetUIScaleX();
        static float GetUIScaleY();
        static float ApplyUIScaleX(float value);
        static float ApplyUIScaleY(float value);
        static DirectX::SimpleMath::Vector2 ApplyUIScale(const DirectX::SimpleMath::Vector2& value);

        const static float PI;
        const static float Deg2Rad;

	const static DirectX::SimpleMath::Vector3 ZeroVector3;
	const static DirectX::SimpleMath::Vector3 OneVector3;
	const static DirectX::SimpleMath::Vector3 LeftVector3;
	const static DirectX::SimpleMath::Vector3 RightVector3;
	const static DirectX::SimpleMath::Vector3 UpVector3;
	const static DirectX::SimpleMath::Vector3 DownVector3;
	const static DirectX::SimpleMath::Vector3 ForwardVector3;

	const static DirectX::SimpleMath::Vector2 ZeroVector2;
	const static DirectX::SimpleMath::Vector2 OneVector2;
	const static DirectX::SimpleMath::Vector2 uvLT;
	const static DirectX::SimpleMath::Vector2 uvLB;
	const static DirectX::SimpleMath::Vector2 uvRT;
	const static DirectX::SimpleMath::Vector2 uvRB;

	const static DirectX::SimpleMath::Color Red;
	const static DirectX::SimpleMath::Color Green;
	const static DirectX::SimpleMath::Color Blue;
	const static DirectX::SimpleMath::Color Yellow;
	const static DirectX::SimpleMath::Color Magenta;
	const static DirectX::SimpleMath::Color Cyan;
	const static DirectX::SimpleMath::Color Black;
	const static DirectX::SimpleMath::Color White;
	const static DirectX::SimpleMath::Color Gray;
        const static DirectX::SimpleMath::Color ClearColor;
        const static DirectX::SimpleMath::Color Transparent;

        const static DirectX::SimpleMath::Matrix Identity;

private:
        static float referenceWidth;
        static float referenceHeight;

};

