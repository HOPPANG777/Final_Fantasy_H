#include "Framework.h"
#include "LogManager.h"

LogManager::LogManager()
{

}

LogManager::~LogManager()
{
	
}

void LogManager::Pre_Log()
{
        std::cout << "--------------------------------------------------------------------------" << std::endl;

        time_t now1 = time(nullptr);
        tm tm_1;

        localtime_s(&tm_1, &now1);

        Log_Count++;

        // 12ð ǥ ڸ ߱  , , ʸ ڿ Ѵ.
        int Hour_Temp = (tm_1.tm_hour > 12) ? (tm_1.tm_hour - 12) : tm_1.tm_hour;

        std::string HHour = (Hour_Temp > 9) ? std::to_string(Hour_Temp) : "0" + std::to_string(Hour_Temp);
        std::string MMin = (tm_1.tm_min > 9) ? std::to_string(tm_1.tm_min) : "0" + std::to_string(tm_1.tm_min);
        std::string SSec = (tm_1.tm_sec > 9) ? std::to_string(tm_1.tm_sec) : "0" + std::to_string(tm_1.tm_sec);

        //  α  : [::] - (ȣ Ƚ)
        std::cout << "[" << HHour << ":" << MMin << ":" << SSec << "] - " << "(" << Log_Count << ")" << std::endl;
}

void LogManager::Log()
{
        // ⺻ ڿ α.  ޽ ϱ  ׻ Pre_Log()  Ѵ.
        Pre_Log();

        std::cout << "Hello~" << std::endl;
}

void LogManager::Log(int p_Value)
{
        //   Ѵ.
        Pre_Log();

        std::cout << p_Value << std::endl;
}

void LogManager::Log(float p_Value)
{
        // float  ⺻ Ʈ  .
        Pre_Log();

        std::cout << p_Value << std::endl;
}

void LogManager::Log(double p_Value)
{
        // double  ϰ óѴ.
        Pre_Log();

        std::cout << p_Value << std::endl;
}

void LogManager::Log(bool p_Value)
{
        // bool True/False ڿ ȯؼ ش.
        Pre_Log();

        std::cout << (p_Value ? "True" : "False") << std::endl;
}

void LogManager::Log(std::wstring p_Value)
{
        // wide ڿ ӽ÷ UTF-8 ȯ ǥ ¿ Ѵ.
        Pre_Log();

        std::cout << std::string(p_Value.begin(), p_Value.end()) << std::endl;
}

void LogManager::Log(std::string p_Value)
{
        // Ϲ ڿ .
        Pre_Log();

        std::cout << p_Value << std::endl;
}

void LogManager::Log(char p_Value)
{
        //  ڵ Ʈ ״ .
        Pre_Log();

        std::cout << p_Value << std::endl;
}

void LogManager::Log(wchar_t p_Value)
{
        // wchar_t ϳ ڿ ȯ  .
        Pre_Log();

        std::wstring Temp(&p_Value);

        std::cout << std::string(Temp.begin(), Temp.end()) << std::endl;
}

void LogManager::Log(wchar_t& p_Value)
{
        //     óѴ.
        Pre_Log();

        std::wstring Temp(&p_Value);

        std::cout << std::string(Temp.begin(), Temp.end()) << std::endl;
}

void LogManager::Log(wchar_t p_Value[])
{
        // wchar_t 迭 ü ڿ ȯ.
        Pre_Log();

        std::wstring Temp(p_Value);

        std::cout << std::string(Temp.begin(), Temp.end()) << std::endl;
}

void LogManager::Log(char& p_Value)
{
        // char  ּҰ ϵ Ǿ ִ.
        Pre_Log();

        std::cout << &p_Value << std::endl;
}

void LogManager::Log(char p_Value[])
{
        // char 迭 ڿ .
        Pre_Log();

        std::cout << p_Value << std::endl;
}

//void LogManager::Log(uint p_Value)
//{
//	Pre_Log();
//
//	std::cout << p_Value << std::endl;
//}
//
//void LogManager::Log(Color p_Value)
//{
//	Pre_Log();
//
//	std::cout << "(";
//	std::cout << p_Value.r << ",";
//	std::cout << p_Value.g << ",";
//	std::cout << p_Value.b << ",";
//	std::cout << p_Value.a << ")" << std::endl;
//}
//
//void LogManager::Log(Vector2 p_Value)
//{
//	Pre_Log();
//
//	std::cout << "(";
//	std::cout << p_Value.x << ",";
//	std::cout << p_Value.y << ")" << std::endl;
//}
//
//void LogManager::Log(Vector3 p_Value)
//{
//	Pre_Log();
//
//	std::cout << "(";
//	std::cout << p_Value.x << ",";
//	std::cout << p_Value.y << ",";
//	std::cout << p_Value.z << ")" << std::endl;
//}
//
//void LogManager::Log(Matrix p_Value)
//{
//	Pre_Log();
//
//	std::cout << "l " << p_Value._11 << " , " << p_Value._12 << " , " << p_Value._13 << " , " << p_Value._14 << " l" << std::endl;
//	std::cout << "l " << p_Value._21 << " , " << p_Value._22 << " , " << p_Value._23 << " , " << p_Value._24 << " l" << std::endl;
//	std::cout << "l " << p_Value._31 << " , " << p_Value._32 << " , " << p_Value._33 << " , " << p_Value._34 << " l" << std::endl;
//	std::cout << "l " << p_Value._41 << " , " << p_Value._42 << " , " << p_Value._43 << " , " << p_Value._44 << " l" << std::endl;
//
//
//}
//
//void LogManager::Log(IObject* p_Value)
//{
//	Pre_Log();
//
//	std::cout << (p_Value != nullptr ? "True" : "False") << std::endl;
//}
//
//float LogManager::Distance(Vector2 p_Start, Vector2 p_End)
//{
//	float X = (p_End.x - p_Start.x);
//	float Y = (p_End.y - p_Start.y);
//
//
//	return (sqrt(X * X) + (Y * Y));
//}
//
//float LogManager::Distance(Vector3 p_Start, Vector3 p_End)
//{
//	float X = (p_End.x - p_Start.x);
//	float Y = (p_End.y - p_Start.y);
//	float Z = (p_End.z - p_Start.z);
//
//	return (sqrt(X * X) + (Y * Y) + (Z * Z));
//}
