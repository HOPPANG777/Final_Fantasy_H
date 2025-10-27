#pragma once

class Collider;
class GameObject;

class CollisionManager
{
	DECLARE_SINGLETON(CollisionManager);

public:
    void AddCollider(std::shared_ptr<Collider> collider);
    void RemoveCollider(std::shared_ptr<Collider> collider);
    void RemoveAllCollider();
    void Update();
    void RenderDebug();

public:
    bool GetColliderDebug() const { return isColliderDebug; }
    std::vector<GameObject*> GetCollidingObjects(GameObject* obj) const;

private:
    std::vector<std::shared_ptr<Collider>> colliders;
    bool isColliderDebug = false;

};