#pragma once

//   Ŭ  ؾ ϴ ̽Դϴ.
class Scene
{
public:
    virtual ~Scene() = default; //    ȣǴ  ҸԴϴ.

    virtual void Awake() = 0;        // Ʈ     Ǵ ʱ غ ܰԴϴ.
    virtual void Start() = 0;        // Awake , ù   Ǵ ʱȭ  ۼմϴ.
    virtual void Destroy() = 0;      //    ؾ  ҽ մϴ.

    virtual void FixedUpdate() = 0;  //      óؾ ϴ  ۼմϴ.
    virtual void Update() = 0;       //  Ӹ ȣǴ Ϲ Ʈ Դϴ.
    virtual void LateUpdate() = 0;   // Update  ó ʿ  ۼմϴ.

    virtual void Render() = 0;       //  ȭ  ׸  Դϴ.
    virtual void PostRender() = 0;   //   ó ȿ ׿  óմϴ.

    virtual void ImGUI() = 0;        // ׿ ImGui UI ϰų մϴ.

    virtual bool IsLoaded() const = 0; //  Ͱ ε ϷǾ θ ȯմϴ.
};
