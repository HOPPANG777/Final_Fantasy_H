#pragma once

class RectTransform : public Component
{
public:
    RectTransform(const std::string& name = "RectTransform") : Component(name) {}
    virtual ~RectTransform() override = default;

public:
    void Update() override;
    void Render() override;

    void GUI() override;

    const DirectX::SimpleMath::Matrix& GetWorld() const;
    RECT GetRect() const;

public:
    DirectX::SimpleMath::Vector3 GetPosition() const;
    DirectX::SimpleMath::Vector2 GetAnchoredPosition() const;
    DirectX::SimpleMath::Vector3 GetScale() const;
    float GetRotation() const;

public:
    DirectX::SimpleMath::Vector2 GetAnchor() const;
    DirectX::SimpleMath::Vector2 GetPivot() const;
    DirectX::SimpleMath::Vector2 GetSize() const;
    DirectX::SimpleMath::Vector2 GetScaledSize() const;

public:
    void SetPosition(const DirectX::SimpleMath::Vector3& pos);
    void SetAnchoredPosition(const DirectX::SimpleMath::Vector2& pos);
    void SetScale(const DirectX::SimpleMath::Vector3& scale);
    void SetRotation(float degrees);

public:
    void SetAnchor(const DirectX::SimpleMath::Vector2& anchor);
    void SetPivot(const DirectX::SimpleMath::Vector2& pivot);
    void SetSize(const DirectX::SimpleMath::Vector2& size);

public:
    void Move(const DirectX::SimpleMath::Vector3& speed);
    void Scale(const DirectX::SimpleMath::Vector3& scale);
    void Rotate(float degrees);

private:
    DirectX::SimpleMath::Vector3 position = Values::ZeroVector3;
    DirectX::SimpleMath::Vector3 scale = Values::OneVector3;
    float rotation = 0.0f;

    // Normalized anchor relative to the screen (0.0f ~ 1.0f)
    DirectX::SimpleMath::Vector2 anchor = DirectX::SimpleMath::Vector2(0.5f, 0.5f);
    // Pivot inside the rectangle (0.0f ~ 1.0f)
    DirectX::SimpleMath::Vector2 pivot = DirectX::SimpleMath::Vector2(0.5f, 0.5f);
    // Pixel size of the rectangle at the reference resolution used for pivot offset
    DirectX::SimpleMath::Vector2 size = Values::ZeroVector2;

    DirectX::SimpleMath::Matrix world = DirectX::XMMatrixIdentity();

    bool isOpen = false;
};


