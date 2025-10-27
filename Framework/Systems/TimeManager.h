#pragma once

class TimeManager
{
        DECLARE_SINGLETON(TimeManager);

public:
        void Update();

        float GetRealDeltaTime() const { return float(_deltaTime); }
        float GetDeltaTime() const { return _scaledDeltaTime; }

public:
        UINT GetFPS() const { return _fps; }
        double GetWorldTime() const { return _worldTime; }

public:
        void SetTimeScale(float scale) { _timeScale = scale; }
        float GetTimeScale() const { return _timeScale; }

        void CheckTime() const
        {
                std::cout << "FPS : " << _fps << ", DeltaTime : "
                        << _scaledDeltaTime << ", WorldTime : " << _worldTime << '\n';
        }

public:
        SYSTEMTIME GetUTCTime() const
        {
                SYSTEMTIME st;
                ::GetSystemTime(&st);
                return st;
        }

        SYSTEMTIME GetLocalPCTime() const
        {
                SYSTEMTIME st;
                ::GetLocalTime(&st);
                return st;
        }

private:
        std::chrono::steady_clock::time_point _currentTime;
        std::chrono::steady_clock::time_point _lastTime;
        double _deltaTime = 0.0;
        double _scaledDeltaTime = 0.0;

private:
        UINT _fps = 0;
        double _worldTime = 0.0;

private:
        UINT _frameCount = 0;
        double _fpsTimeElapsed = 0.0;

private:
        float _scaleDeltaTime = 0.0f;
        float _timeScale = 1.0f;
};
