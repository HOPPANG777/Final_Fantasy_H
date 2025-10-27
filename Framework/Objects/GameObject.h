#pragma once

class GameObject
{
public:
        // Ʈ ⺻  ϴ Դϴ.
        // name:  Ϳ ĺ ϴ ̸Դϴ.
        // RectTransformType: true UI RectTransform, false 3D Transform ϴ.
        GameObject(
                const std::string& name,
                bool RectTransformType = false                  // false = Transform, true = RectTransform
        );

        // ʱ ġ, ũ, ȸ   ϴ Դϴ.
        // RectTransformType   Transform Ǵ RectTransform Ʈ غմϴ.
        GameObject(
                const std::string& name,
                const DirectX::SimpleMath::Vector3& initPos,
                const DirectX::SimpleMath::Vector3& initScale,
                float initRotation,
                bool RectTransformType = false
        );

        //  縦  ʵ    Դϴ.
        GameObject(const GameObject& other);
        GameObject& operator=(const GameObject& other)
        {
                if (this != &other)
                {
                        name = other.name;
                        // TODO: ʿ  Ʈ  ڵ带 ߰մϴ.
                }
                return *this;
        }

        virtual ~GameObject() = default;

public:
        //  Ʈ Ʈ  ȣϴ ⺻ Դϴ.
        virtual void Update();
        //  Ʈ  ûϴ ⺻ Դϴ.
        virtual void Render();

public:
        // ο Ʈ ϰ Ʈ ڷ մϴ.
        void AddComponent(const std::shared_ptr<Component>& component);
        // ̸  ϵ Ʈ մϴ.
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
        // Transform 迭 Ʈ ϰ ȯմϴ.
        std::shared_ptr<Transform> GetTransform() const;
        std::shared_ptr<RectTransform> GetRectTransform() const;

public:
        // ̽  Ư Ÿ Ʈ ãϴ.
        template<typename T>
        T* FindComponentByInterface() const
        {
                try
                {
                        // ϵ Ʈ  nullptr ȯմϴ.
                        if (components.empty())
                        {
                                return nullptr;
                        }

                        // components ڷ: std::map<std::string, std::shared_ptr<Component>>
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
                        // ʿϴٸ ⿡ α ó   ߰մϴ.
                }
                return nullptr;
        }

        // Ư ̽   Ʈ  ȯմϴ.
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

                        // components ڷ: std::map<std::string, std::shared_ptr<Component>>
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
        // Ʈ ĺ ̸Դϴ.
        std::string name;
        // ϵ Ʈ ̸ ϴ ̳Դϴ.
        std::map<std::string, std::shared_ptr<Component>> components;

        // ̴    ͸ մϴ.
        std::unique_ptr<WorldBuffer> worldBuffer;

        // true RectTransform ϴ UI Ʈ ǹմϴ.
        bool RectTransformType = false;
};
