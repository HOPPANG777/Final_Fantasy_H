#pragma once

// MoveComponent  ӵ Ʈ ̵Ű  Ʈ.
// , ӵ,    ݺ ̵  Ѵ.

class MoveComponent : public Component
{
public:
        float maxSpeed = 300.0f; // ִ ̵ ӵ
	~MoveComponent() override = default;

public:
        // Component ֱ⿡  Ű Է а ̵ óѴ.
	void Update() override;
	void Render() override;

        // ÿ  ̵ Ű  īƮ 밢 ӵ Ѵ.
	void AddKeyCount() { ++keyCount; }
        //      Ű  ʱȭѴ.
	void ResetKeyCount() { keyCount = 0; }

private:
        float maxSpeed = 300.0f; // ִ ̵ ӵ
        float speed = maxSpeed;  //  ӿ  ӵ
        UINT keyCount = 0;       // ÿ  ̵ Ű 

};

