#pragma once

class TimeManager
{
        DECLARE_SINGLETON(TimeManager);

public:
        //  Ӹ ð   Ѵ.
        void Update();

        // ð Ӱ   Ÿ Ÿ() ȯѴ.
        float GetRealDeltaTime() const { return float(_deltaTime); }
        // ð  ݿ Ÿ Ÿ() ȯѴ.
        float GetDeltaTime() const { return _scaledDeltaTime; }

public:
        // ֱ  FPS  ȯѴ.
        UINT GetFPS() const { return _fps; }
        //  ۵    ð ȯѴ.
        double GetWorldTime() const { return _worldTime; }

public:
        // Ÿӽ  Ѵ. 1.0̸ ǽð Ѵ.
        void SetTimeScale(float scale) { _timeScale = scale; }
        //  Ÿӽ  ȯѴ.
        float GetTimeScale() const { return _timeScale; }

        //   ð (FPS, Ÿ Ÿ,  Ÿ) ֿܼ Ѵ.
        void CheckTime() const
        {
                std::cout << "FPS : " << _fps << ", DeltaTime : "
                        << _scaledDeltaTime << ", WorldTime : " << _worldTime << '\n';
        }

public:
        // UTC( ǥؽ) SYSTEMTIME ü ȯѴ.
        SYSTEMTIME GetUTCTime() const
        {
                SYSTEMTIME st;
                ::GetSystemTime(&st);  //  UTC ð st Ѵ.
                return st;
        }

        // PC  ð SYSTEMTIME ü ȯѴ.
        SYSTEMTIME GetLocalPCTime() const
        {
                SYSTEMTIME st;
                ::GetLocalTime(&st);  //   ð st Ѵ.
                return st;
        }

private:
        //  ſ Ǵ  ð   ð.
        std::chrono::steady_clock::time_point _currentTime;
        std::chrono::steady_clock::time_point _lastTime;
        // ǽð Ÿ Ÿ() Ÿӽ  Ÿ Ÿ.
        double _deltaTime = 0.0;
        double _scaledDeltaTime = 0.0;

private:
        // ʴ    ü  ð().
        UINT _fps = 0;
        double _worldTime = 0.0;

private:
        // FPS         ð.
        UINT _frameCount = 0;
        double _fpsTimeElapsed = 0.0;

private:
        //  Ÿ ŸӰ Ÿӽ .
        float _scaleDeltaTime = 0.0f;
        float _timeScale = 1.0f;                // Ÿӽ ⺻ 1.0(ǽð).
};
