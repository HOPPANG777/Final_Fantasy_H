// MoveComponent δ Ÿ Ÿ  ġ ӵ Ѵ.

#include "Framework.h"
#include "MoveComponent.h"

void MoveComponent::Update()
{
	ResetKeyCount();
        // ο Ӹ  Ű  ʱȭѴ.

        // ̵ Ű Է Ȯϸ   ÿ ȴ .
	if (KeyboardManager::Get()->Press('W')) AddKeyCount();
	if (KeyboardManager::Get()->Press('S')) AddKeyCount();
	if (KeyboardManager::Get()->Press('A')) AddKeyCount();
	if (KeyboardManager::Get()->Press('D')) AddKeyCount();

        // 밢 ̵ ÿ Ʈ2   ü ӵ Ѵ.
	if (keyCount == 2) speed = maxSpeed / sqrt(2.0f);
	else speed = maxSpeed;

        //   Ű  Transform Move Լ ȣѴ.
	if (KeyboardManager::Get()->Press('W')) owner->GetComponent<Transform>("Transform")->Move(DirectX::SimpleMath::Vector3(0, speed, 0));
	if (KeyboardManager::Get()->Press('S')) owner->GetComponent<Transform>("Transform")->Move(DirectX::SimpleMath::Vector3(0, -speed, 0));
	if (KeyboardManager::Get()->Press('A')) owner->GetComponent<Transform>("Transform")->Move(DirectX::SimpleMath::Vector3(-speed, 0, 0));
	if (KeyboardManager::Get()->Press('D')) owner->GetComponent<Transform>("Transform")->Move(DirectX::SimpleMath::Vector3(speed, 0, 0));

        // Ű ũ ȸ ϴ  Ѵ.
	if (KeyboardManager::Get()->Press(VK_DOWN)) owner->GetComponent<Transform>("Transform")->Scale(DirectX::SimpleMath::Vector3(0.5f) * -speed);
	if (KeyboardManager::Get()->Press(VK_UP)) owner->GetComponent<Transform>("Transform")->Scale(DirectX::SimpleMath::Vector3(0.5f) * speed);

	if (KeyboardManager::Get()->Press(VK_LEFT)) owner->GetComponent<Transform>("Transform")->Rotate(-speed);
	if (KeyboardManager::Get()->Press(VK_RIGHT)) owner->GetComponent<Transform>("Transform")->Rotate(speed);
}

void MoveComponent::Render()
{

}
