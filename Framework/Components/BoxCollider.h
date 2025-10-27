#pragma once

// BoxCollider Ʈ AABB 浹  ϰ 浹 ˻縦 Ѵ.
// ߽ ǥ, ũ,   Ͽ ٸ 簢 浹 θ ǴѴ.


class BoxCollider : public Collider
{
    struct BoxEdge
    {
        float left;
        float right;
        float top;
        float bottom;
    };

public:
    BoxCollider(
        const DirectX::SimpleMath::Vector3& center, 
        const DirectX::SimpleMath::Vector3& size, 
        const DirectX::SimpleMath::Color& color = Values::Green
    ) 
        : Collider("BoxCollider", Type::Box), center(center), size(size), color(color)
    {
        SetExtents(size);
    }

    ~BoxCollider() override = default;

public:
    // Component ֱ⿡  浹  Ѵ.
    void Update() override;
    void Render() override;
    // ߽ɰ ũ⸦  簢 谪 ٽ Ѵ.
    void UpdateEdge() 
    { 
        edge.left = center.x - extents.x; 
        edge.right = center.x + extents.x; 
        edge.top = center.y + extents.y; 
        edge.bottom = center.y - extents.y; 
    }

public:
    void SetCenter(const DirectX::SimpleMath::Vector3& newCenter) { center = newCenter; }
    void SetSize(const DirectX::SimpleMath::Vector3& newSize) { size = newSize; }
    // SetSize     extents Ѵ.
    void SetExtents(const DirectX::SimpleMath::Vector3& size) { extents = size * 0.5f; }


public:
    DirectX::SimpleMath::Vector3 GetCenter() const { return center; }
    DirectX::SimpleMath::Vector3 GetSize() const { return size; }

public:
    bool IsColliding(const Collider* other) const override
    {
        if (other->GetType() == Type::Box)
        {
            const BoxCollider* otherBox = static_cast<const BoxCollider*>(other);
            return IsBoxColliding(otherBox);
        }
        return false;
    }


private:
    //  и   簢 ġ ˻Ѵ.
    bool IsBoxColliding(const BoxCollider* other) const
    {
        //return (abs(center.x - other->center.x) <= (size.x * 0.5f + other->size.x * 0.5f)) &&
        //    (abs(center.y - other->center.y) <= (size.y * 0.5f + other->size.y * 0.5f));

        // X и ˻
        if (edge.left > other->edge.right || edge.right < other->edge.left) return false;

        // Y и ˻
        if (edge.top < other->edge.bottom || edge.bottom > other->edge.top) return false;
        
        //    ġ 浹
        return true;
    }

    // Transform ޾ƿ ߽ ǥ ũ 
    DirectX::SimpleMath::Vector3 center;
    DirectX::SimpleMath::Vector3 size;    // ü ڽ ũ
    DirectX::SimpleMath::Vector3 extents; // half-size, 浹  ȭ 뵵
    DirectX::SimpleMath::Color color;     //   
    BoxEdge edge;                         // 簢 /// 谪

};


