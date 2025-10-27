#pragma once

constexpr size_t MAX_INPUT_MOUSE = 8;

class MouseManager
{
        DECLARE_SINGLETON(MouseManager);

        enum
        {
                MOUSE_ROTATION_RIGHT = 0,
                MOUSE_ROTATION_LEFT,
                MOUSE_ROTATION_WHEEL
        };

        enum
        {
                BUTTON_INPUT_STATUS_NONE = 0,
                BUTTON_INPUT_STATUS_DOWN,
                BUTTON_INPUT_STATUS_UP,
                BUTTON_INPUT_STATUS_PRESS,
                BUTTON_INPUT_STATUS_DBLCLK
        };

public:
        //  Ŭ    ʱ ͸ غѴ.
        void Init();

        // ư  ¸ Ѵ.
        void Update();
        //  ޽ ޾ 콺 ġ   Ѵ.
        LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
        //  콺 ǥ ȯѴ.
        DirectX::SimpleMath::Vector3 GetMousePos() const { return mousePos; }
        //   ̵( ) ȯѴ.
        DirectX::SimpleMath::Vector3 GetMoveValue() const { return wheelMoveValue; }

public:
        // ư  ȴ ȮѴ.
        bool Down(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
        // ư   ȮѴ.
        bool Up(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
        // ư   ִ ȮѴ.
        bool Press(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }


private:
        // Է   ڵ  콺 ǥ.
        HWND handle;
        DirectX::SimpleMath::Vector3 mousePos;

private:
        //   ư ,   , ȯ Է .
        std::array<unsigned char, MAX_INPUT_MOUSE> buttonStatus = { 0 };
        std::array<unsigned char, MAX_INPUT_MOUSE> buttonOldStatus = { 0 };
        std::array<unsigned char, MAX_INPUT_MOUSE> buttonMap = { 0 };

private:
        //  /  ̵.
        DirectX::SimpleMath::Vector3 wheelStatus = Values::ZeroVector3;
        DirectX::SimpleMath::Vector3 wheelOldStatus = Values::ZeroVector3;
        DirectX::SimpleMath::Vector3 wheelMoveValue = Values::ZeroVector3;

private:
        //  Ŭ Ǵ  ð  ī.
        DWORD timeDblClk;
        DWORD startDblClk[MAX_INPUT_MOUSE];
        int buttonCount[MAX_INPUT_MOUSE];

};

