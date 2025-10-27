#pragma once


class MoveComponent : public Component
{
public:
        float maxSpeed = 300.0f;
	~MoveComponent() override = default;

public:
	void Update() override;
	void Render() override;

	void AddKeyCount() { ++keyCount; }
	void ResetKeyCount() { keyCount = 0; }

private:
        float maxSpeed = 300.0f;
        float speed = maxSpeed;
        UINT keyCount = 0;

};

