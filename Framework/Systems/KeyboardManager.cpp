#include "Framework.h"
#include "KeyboardManager.h"

KeyboardManager::KeyboardManager() {}

KeyboardManager::~KeyboardManager() {}

void KeyboardManager::Update()
{
    keyOldState = std::move(keyState);

    const ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard || io.WantTextInput)
    {
        keyState.fill(0);

        for (DWORD i = 0; i < MAX_INPUT_KEY; ++i)
        {
            const auto& oldState = keyOldState[i];
            if (oldState == 1)
            {
                keyMap[i] = KEY_INPUT_STATUS_UP;
            }
            else
            {
                keyMap[i] = KEY_INPUT_STATUS_NONE;
            }
        }

        return;
    }

    if (GetKeyboardState(keyState.data()))
    {
        for (DWORD i = 0; i < MAX_INPUT_KEY; ++i)
        {
            keyState[i] = keyState[i] & 0x80 ? 1 : 0;

            auto& oldState = keyOldState[i];
            auto& state = keyState[i];

            if (oldState == 0 && state == 1)
            {
                keyMap[i] = KEY_INPUT_STATUS_DOWN;
            }
            else if (oldState == 1 && state == 0)
            {
                keyMap[i] = KEY_INPUT_STATUS_UP;
            }
            else if (oldState == 1 && state == 1)
            {
                keyMap[i] = KEY_INPUT_STATUS_PRESS;
            }
            else
            {
                keyMap[i] = KEY_INPUT_STATUS_NONE;
            }
        }
    }

}