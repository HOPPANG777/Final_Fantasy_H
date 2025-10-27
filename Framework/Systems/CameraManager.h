#pragma once

class CameraManager
{
    DECLARE_SINGLETON(CameraManager);

public:
    void Init();

    void Update();
    void Render();

public:
    void SetProjection(float width, float height);
    void UpdateView();
    void Move();
    DirectX::SimpleMath::Vector3 Drag() const;

    void Reset();

public:
    [[nodiscard]] DirectX::SimpleMath::Vector3 GetPosition() const noexcept { return _position; }
    void SetPosition(const DirectX::SimpleMath::Vector3& position) noexcept { _position = position; }

public:
    void SetTarget(const std::shared_ptr<GameObject>& target) noexcept { _target = target; }
    void RemoveTarget() noexcept { _target.reset(); }

public:
    void SetTarget(GameObject* target) noexcept { _rawTarget = target; }
    GameObject* GetTarget() const noexcept { return _rawTarget ? _rawTarget : _target.lock().get(); }

public:
    void UpdateFollow();
    void UpdateHorizontalScroll(GameObject* target, float minRatio = 0.5f, float maxRatio = 2.5f);
    void UpdateVerticalScroll(GameObject* target, float minRatio = 0.5f, float maxRatio = 2.5f);

private:
    DirectX::SimpleMath::Matrix _view;
    DirectX::SimpleMath::Matrix _proj;
    std::unique_ptr<ViewProjectionBuffer> _vpBuffer = nullptr;

    DirectX::SimpleMath::Vector3 _position = Values::ZeroVector3;

private:
    std::weak_ptr<GameObject> _target;
    GameObject* _rawTarget = nullptr;

    bool _isMove = false;
    float _moveSpeed = 150.0f;

    float _accelerationTime = 0.0f;
    float _currentSpeed = 0.0f;
};

