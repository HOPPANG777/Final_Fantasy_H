#pragma once

// Collider  浹 Ʈ ϴ  Ŭ.
// 浹 Ÿ԰ ݹ ϸ ü 浹  Ļ Ŭ Ѵ.

class Collider : public Component
{
public:
    enum class Type
    {
        Box,
        Circle,
    };

    Collider(const std::string& name, Type type) : Component(name), type(type) {}
    virtual ~Collider() = default;

    Type GetType() const { return type; }

    // 浹   ݹ Ѵ.
    void SetCollision(std::function<void(Collider*)> setCollision)
    {
        collisionCallback = setCollision;
    }

    virtual bool IsColliding(const Collider* other) const = 0;

    virtual void OnCollision(Collider* other)
    {
        if (collisionCallback) collisionCallback(other);
    }

protected:
    Type type;                                 // ڽ/  浹 
    std::function<void(Collider*)> collisionCallback; //   浹 ó
};
