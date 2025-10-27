#pragma once

class GameObject
{
public:
        GameObject(
                const std::string& name,
                bool RectTransformType = false                  // false = Transform, true = RectTransform
        );

        GameObject(
                const std::string& name,
                const DirectX::SimpleMath::Vector3& initPos,
                const DirectX::SimpleMath::Vector3& initScale,
                float initRotation,
                bool RectTransformType = false
        );

        GameObject(const GameObject& other);
        GameObject& operator=(const GameObject& other)
        {
                if (this != &other)
                {
                        name = other.name;
                }
                return *this;
        }

        virtual ~GameObject() = default;

public:
        virtual void Update();
        virtual void Render();

public:
        void AddComponent(const std::shared_ptr<Component>& component);
        void RemoveComponent(const std::string& compName);

        template<typename T>
        std::shared_ptr<T> GetComponent(const std::string& compName) const
        {
                if (!components.count(compName)) return nullptr;

                return std::dynamic_pointer_cast<T>(components.at(compName));
        }

public:
        std::string GetName() const { return name; }
        void SetName(const std::string& newName) { name = newName; }

public:
        std::shared_ptr<Transform> GetTransform() const;
        std::shared_ptr<RectTransform> GetRectTransform() const;

public:
        template<typename T>
        T* FindComponentByInterface() const
        {
                try
                {
                        if (components.empty())
                        {
                                return nullptr;
                        }

                        for (const auto& pair : components)
                        {
                                T* comp = dynamic_cast<T*>(pair.second.get());
                                if (comp != nullptr)
                                {
                                        return comp;
                                }
                        }
                }
                catch (const std::exception& e)
                {
                        std::cerr << "[FindComponentByInterface]  ߻: " << e.what() << std::endl;
                }
                return nullptr;
        }

        template<typename T>
        std::vector<T*> GetAllComponentsByInterface() const
        {
                std::vector<T*> result;
                try
                {
                        if (components.empty())
                        {
                                return result;
                        }

                        for (const auto& pair : components)
                        {
                                T* comp = dynamic_cast<T*>(pair.second.get());
                                if (comp != nullptr)
                                {
                                        result.push_back(comp);
                                }
                        }
                }
                catch (const std::exception& e)
                {
                        std::cerr << "[GetAllComponentsByInterface]  ߻: " << e.what() << std::endl;
                }
                return result;
        }

protected:
        std::string name;
        std::map<std::string, std::shared_ptr<Component>> components;

        std::unique_ptr<WorldBuffer> worldBuffer;

        bool RectTransformType = false;
};
