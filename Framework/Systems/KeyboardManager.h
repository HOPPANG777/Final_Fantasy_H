#pragma once

constexpr size_t MAX_INPUT_KEY = 256;

class KeyboardManager
{
        DECLARE_SINGLETON(KeyboardManager);

        enum
        {
                KEY_INPUT_STATUS_NONE = 0,
                KEY_INPUT_STATUS_DOWN,
                KEY_INPUT_STATUS_UP,
                KEY_INPUT_STATUS_PRESS
        };

public:
        //   Ű ¸ Ѵ.
        void Update();

        // Ű  ȴ θ ȯѴ.
        bool Down(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
        // Ű   θ ȯѴ.
        bool Up(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_UP; }
        // Ű  · ǰ ִ θ ȯѴ.
        bool Press(const DWORD key) const { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

private:
        // GetKeyboardState  ϴ   Ű .
        std::array<unsigned char, MAX_INPUT_KEY> keyState = { 0 };
        //   Ű .
        std::array<unsigned char, MAX_INPUT_KEY> keyOldState = { 0 };
        // Է ¸ Down/Up/Press   .
        std::array<unsigned char, MAX_INPUT_KEY> keyMap = { 0 };

};

