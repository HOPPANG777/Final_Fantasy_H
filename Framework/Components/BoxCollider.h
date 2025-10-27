#pragma once

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
    // Component 상속
    void Update() override;
    void Render() override;
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
    bool IsBoxColliding(const BoxCollider* other) const
    {
        //return (abs(center.x - other->center.x) <= (size.x * 0.5f + other->size.x * 0.5f)) &&
        //    (abs(center.y - other->center.y) <= (size.y * 0.5f + other->size.y * 0.5f));

        // X축 분리 검사
        if (edge.left > other->edge.right || edge.right < other->edge.left) return false;

        // Y축 분리 검사
        if (edge.top < other->edge.bottom || edge.bottom > other->edge.top) return false;
        
        // 두 축이 모두 겹치면 충돌
        return true;
    }

    DirectX::SimpleMath::Vector3 center;
    DirectX::SimpleMath::Vector3 size;
    DirectX::SimpleMath::Vector3 extents;
    DirectX::SimpleMath::Color color;
    BoxEdge edge;

};


