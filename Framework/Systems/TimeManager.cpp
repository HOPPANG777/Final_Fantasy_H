#include "Framework.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
        _currentTime = std::chrono::steady_clock::now();
        _lastTime = _currentTime;
}

TimeManager::~TimeManager() {}

void TimeManager::Update()
{
        _lastTime = _currentTime;

        _currentTime = std::chrono::steady_clock::now();

        std::chrono::duration<double> delta = _currentTime - _lastTime;
        _deltaTime = delta.count();

        _worldTime += _deltaTime;

        _fpsTimeElapsed += _deltaTime;
        ++_frameCount;

        if (_fpsTimeElapsed >= 1.0)
        {
                _fps = _frameCount;
                _frameCount = 0;
                _fpsTimeElapsed = 0.0;

                CheckTime();
        }

        _scaledDeltaTime = float(_deltaTime * _timeScale);
}
