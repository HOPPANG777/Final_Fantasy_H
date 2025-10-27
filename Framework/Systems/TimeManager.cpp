#include "Framework.h"
#include "TimeManager.h"

// :  ð ʱȭϿ ð   Ѵ.
TimeManager::TimeManager()
{
        _currentTime = std::chrono::steady_clock::now();
        _lastTime = _currentTime;
}

// Ҹ: ̶̱   ۾ ʿ .
TimeManager::~TimeManager() {}

//   ȣǾ Ÿ ŸӰ FPS Ѵ.
void TimeManager::Update()
{
        //   ð Ѵ.
        _lastTime = _currentTime;

        //  ð  Ѵ.
        _currentTime = std::chrono::steady_clock::now();

        //  Ӱ     ð Ѵ.
        std::chrono::duration<double> delta = _currentTime - _lastTime;
        _deltaTime = delta.count();

        //   ð Ѵ.
        _worldTime += _deltaTime;

        // FPS    ð   Ѵ.
        _fpsTimeElapsed += _deltaTime;
        ++_frameCount;

        // 1ʰ ٸ FPS Ȯϰ   ʱȭѴ.
        if (_fpsTimeElapsed >= 1.0)
        {
                _fps = _frameCount;
                _frameCount = 0;
                _fpsTimeElapsed = 0.0;

                CheckTime();
        }

        // Ÿӽ  Ÿ Ÿ Ѵ.
        _scaledDeltaTime = float(_deltaTime * _timeScale);
}
