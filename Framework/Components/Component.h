#pragma once

// Base class for every component that can be attached to a GameObject.
// Derived classes override Update and Render to implement their behaviour.

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

        // Assign the owning GameObject instance.
	void SetOwner(GameObject* obj) { owner = obj; }

public:
        // Called once per frame to update the component state.
	virtual void Update() = 0;
        // Called every frame to render the component if required.
	virtual void Render() = 0;

};

