#pragma once

// CircleCollider Ʈ  浹  ϰ   ˻縦 Ѵ.
// ߽ ġ  ϸ ٸ  浹 θ Ѵ.

class CircleCollider : public Collider
{
public:
    CircleCollider(
        const DirectX::SimpleMath::Vector3& center, 
        float radius, 
        const DirectX::SimpleMath::Color& color = Values::Green
    ) 
        : Collider("CircleCollider", Type::Circle), center(center), radius(radius), color(color) 
    {
        
    }
    ~CircleCollider() override = default;

public:
    // Component ֱ⿡  ߽ ǥ   Ѵ.
    void Update() override;
    void Render() override;

public:
    // Transform ޹ ߽ ǥ Ʈ  Ѵ.
    void SetCenter(const DirectX::SimpleMath::Vector3& newCenter) { center = newCenter; }
    void SetRadius(float newRadius) { radius = newRadius; }

public:
    DirectX::SimpleMath::Vector3 GetCenter() const { return center; }
    float GetRadius() const { return radius; }

public:
    bool IsColliding(const Collider* other) const override
    {
        if (other->GetType() == Type::Circle)
        {
            const CircleCollider* otherCircle = static_cast<const CircleCollider*>(other);
            return IsCircleColliding(otherCircle);
        }
        return false;
    }

private:
    //   ߽ Ÿ   浹 θ ǴѴ.
    bool IsCircleColliding(const CircleCollider* other) const
    {
        float dx = center.x - other->center.x;
        float dy = center.y - other->center.y;
        float distance = sqrt(dx * dx + dy * dy);

        return distance < (radius + other->radius);
    }

    DirectX::SimpleMath::Vector3 center; //  ߽ ǥ
    float radius;                         // 浹 
    DirectX::SimpleMath::Color color;     //   

};
