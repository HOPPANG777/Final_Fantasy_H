#include "Framework.h"
#include "KeyboardManager.h"

// : Ư ʱȭ ʿ .
KeyboardManager::KeyboardManager() {}

// Ҹ:  ڿ   ִ.
KeyboardManager::~KeyboardManager() {}

// Ű Է ¸ ƮѴ.
void KeyboardManager::Update()
{
        //   ¸ Ѵ.
        keyOldState = std::move(keyState);

        //  Ű  ¸ ´.
        if (GetKeyboardState(keyState.data()))
        {
                for (DWORD i = 0; i < MAX_INPUT_KEY; ++i)
                {
                        // ֻ Ʈ(0x80)   θ ǺѴ.
                        keyState[i] = keyState[i] & 0x80 ? 1 : 0;

                        auto& oldState = keyOldState[i];
                        auto& state = keyState[i];

                        if (oldState == 0 && state == 1)
                        {
                                //     ȴٸ Down .
                                keyMap[i] = KEY_INPUT_STATUS_DOWN;
                        }
                        else if (oldState == 1 && state == 0)
                        {
                                //  Ȱ  ٸ Up .
                                keyMap[i] = KEY_INPUT_STATUS_UP;
                        }
                        else if (oldState == 1 && state == 1)
                        {
                                //  ִ  Press .
                                keyMap[i] = KEY_INPUT_STATUS_PRESS;
                        }
                        else
                        {
                                //  ȭ ٸ None .
                                keyMap[i] = KEY_INPUT_STATUS_NONE;
                        }
                }
        }

}
