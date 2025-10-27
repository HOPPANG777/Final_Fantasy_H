#pragma once

// Component   Ʈ    ̴.
// ̸  ͸ ϰ Update/Render ̽ Ѵ.

class GameObject;

class Component
{
protected:
	std::string name;
	GameObject* owner = nullptr;

public:
	Component(const std::string& compName) : name(compName) {}
	virtual ~Component() = default;

public:
	GameObject* GetOwner() const { return owner; }
	std::string GetName() const { return name; }

        // GameObject   ڸ Ѵ.
	void SetOwner(GameObject* obj) { owner = obj; }

public:
        // Ӻ   Ļ Ŭ Ѵ.
	virtual void Update() = 0;
        // ȭ  ʿ  Ļ Ŭ Ѵ.
	virtual void Render() = 0;

};

