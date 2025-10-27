// CameraManager.cpp
#include "Framework.h"
#include "CameraManager.h"

namespace
{
    constexpr float kMaxAccelTime = 3.0f;
    constexpr float kAccelDivisor = 5.0f;
    constexpr float kDefaultNear = 0.0f;
    constexpr float kDefaultFar = 1.0f;
}

CameraManager::CameraManager() {}
CameraManager::~CameraManager() {}

void CameraManager::Init()
{
    _vpBuffer = std::make_unique<ViewProjectionBuffer>();

    _view = DirectX::XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 0, 1), DirectX::SimpleMath::Vector3(0, 1, 0));
    _proj = DirectX::XMMatrixOrthographicOffCenterLH(0, Values::GetWinWidth(), 0, Values::GetWinHeight(), kDefaultNear, kDefaultFar);

    _vpBuffer->SetView(_view);
    _vpBuffer->SetProjection(_proj);
}

void CameraManager::Update()
{
    Move();
    UpdateView();
}

void CameraManager::Render()
{
    _vpBuffer->SetVSBuffer(1);
}

void CameraManager::SetProjection(float width, float height)
{
    _proj = DirectX::XMMatrixOrthographicOffCenterLH(0, width, 0, height, kDefaultNear, kDefaultFar);
    if (_vpBuffer) _vpBuffer->SetProjection(_proj);
}

void CameraManager::UpdateView()
{
    _view = DirectX::XMMatrixLookAtLH
    (
        _position,
        _position + DirectX::SimpleMath::Vector3(0, 0, 1),
        DirectX::SimpleMath::Vector3(0, 1, 0)
    );

    _vpBuffer->SetView(_view);
}

void CameraManager::Move()
{
    GameObject* targetPtr = _rawTarget ? _rawTarget : _target.lock().get();

    if (targetPtr != nullptr)
    {
        _position.x = targetPtr->GetComponent<Transform>("Transform")->GetPosition().x - Values::GetWinWidth() * 0.5f;
        _position.y = targetPtr->GetComponent<Transform>("Transform")->GetPosition().y - Values::GetWinHeight() * 0.5f;
        return;
    }

    if (KeyboardManager::Get()->Down(VK_F4))
    {
        _isMove = !_isMove;
        _accelerationTime = 0.0f;
        _currentSpeed = _moveSpeed;
    }

    if (_isMove)
    {
        bool moving = false;

        if (KeyboardManager::Get()->Press('W')) { _position.y += _currentSpeed * TimeManager::Get()->GetDeltaTime(); moving = true; }
        else if (KeyboardManager::Get()->Press('S')) { _position.y -= _currentSpeed * TimeManager::Get()->GetDeltaTime(); moving = true; }

        if (KeyboardManager::Get()->Press('A')) { _position.x -= _currentSpeed * TimeManager::Get()->GetDeltaTime(); moving = true; }
        else if (KeyboardManager::Get()->Press('D')) { _position.x += _currentSpeed * TimeManager::Get()->GetDeltaTime(); moving = true; }

        if (moving)
        {
            _accelerationTime += TimeManager::Get()->GetDeltaTime();
            if (_accelerationTime > kMaxAccelTime) _accelerationTime = kMaxAccelTime;
        }
        else
        {
            _accelerationTime = 0.0f;
        }

        _currentSpeed = _moveSpeed * (1.0f + _accelerationTime / kAccelDivisor);
    }
}

DirectX::SimpleMath::Vector3 CameraManager::Drag() const
{
    DirectX::SimpleMath::Vector3 mousePos = MouseManager::Get()->GetMousePos();
    mousePos += _position;
    return mousePos;
}

void CameraManager::Reset()
{
    _position = { 0, 0, 0 };
    _target.reset();
    _rawTarget = nullptr;
    UpdateView();
}

void CameraManager::UpdateFollow()
{
    GameObject* targetPtr = _rawTarget ? _rawTarget : _target.lock().get();
    if (targetPtr == nullptr) return;

    DirectX::SimpleMath::Vector3 camPos = _position;
    const DirectX::SimpleMath::Vector3 targetPos = targetPtr->GetComponent<Transform>("Transform")->GetPosition();

    const float marginX = 600.0f;
    const float marginY = 200.0f;

    const float viewLeft = camPos.x;
    const float viewRight = camPos.x + Values::GetWinWidth();
    const float viewTop = camPos.y;
    const float viewBottom = camPos.y + Values::GetWinHeight();

    const float deadLeft = viewLeft + marginX;
    const float deadRight = viewRight - marginX;
    const float deadTop = viewTop + marginY;
    const float deadBottom = viewBottom - marginY;

    if (targetPos.x < deadLeft) camPos.x = targetPos.x - marginX;
    else if (targetPos.x > deadRight) camPos.x = targetPos.x + marginX - Values::GetWinWidth();

    if (targetPos.y < deadTop) camPos.y = targetPos.y - marginY;
    else if (targetPos.y > deadBottom) camPos.y = targetPos.y + marginY - Values::GetWinHeight();

    _position = camPos;
    UpdateView();
}

void CameraManager::UpdateHorizontalScroll(GameObject* target, float minRatio, float maxRatio)
{
    if (target == nullptr) return;

    const DirectX::SimpleMath::Vector3 targetPos = target->GetComponent<Transform>("Transform")->GetPosition();
    const float w = Values::GetWinWidth();

    if (targetPos.x > w * minRatio && targetPos.x < w * maxRatio)
    {
        const float newX = targetPos.x - w * 0.5f;
        SetPosition(DirectX::SimpleMath::Vector3(newX, _position.y, _position.z));
    }
}

void CameraManager::UpdateVerticalScroll(GameObject* target, float minRatio, float maxRatio)
{
    if (target == nullptr) return;

    const DirectX::SimpleMath::Vector3 targetPos = target->GetComponent<Transform>("Transform")->GetPosition();
    const float h = Values::GetWinHeight();

    if (targetPos.y > h * minRatio && targetPos.y < h * maxRatio)
    {
        const float newY = targetPos.y - h * 0.5f;
        SetPosition(DirectX::SimpleMath::Vector3(_position.x, newY, _position.z));
    }
}
