#include "Framework.h"
#include "String.h"

void String::SplitString(std::vector<std::string>* result, std::string origin, std::string tok)
void String::SplitString(std::vector<std::wstring>* result, std::wstring origin, std::wstring tok)

bool String::StartWith(std::string str, std::string comp)
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && (int)index == 0) return true;
bool String::StartWith(std::wstring str, std::wstring comp)
	std::wstring::size_type index = str.find(comp);
	if (index != std::wstring::npos && (int)index == 0) return true;
bool String::Contain(std::string str, std::string comp)
	return found != std::wstring::npos;
bool String::Contain(std::wstring str, std::wstring comp)
	return found != std::wstring::npos;

void String::Replace(std::string* str, std::string comp, std::string rep)
	std::string temp = *str;

	while ((startPos = temp.find(comp, startPos)) != std::wstring::npos)
void String::Replace(std::wstring* str, std::wstring comp, std::wstring rep)
	std::wstring temp = *str;

	while ((startPos = temp.find(comp, startPos)) != std::wstring::npos)
// std::setprecision : ε Ҽ  е ϴ 
std::string String::ToString(Vector3 vec3, int precision)
	std::string temp;
	std::stringstream stream1, stream2, stream3;
	stream1 << std::fixed << std::setprecision(precision) << vec3.x;
	stream2 << std::fixed << std::setprecision(precision) << vec3.y;
	stream3 << std::fixed << std::setprecision(precision) << vec3.z;
std::wstring String::ToWstring(Vector3 vec3, int precision)
	std::wstring temp;
	std::wstringstream stream1, stream2, stream3;
	stream1 << std::fixed << std::setprecision(precision) << vec3.x;
	stream2 << std::fixed << std::setprecision(precision) << vec3.y;
	stream3 << std::fixed << std::setprecision(precision) << vec3.z;
// std::string -> std::wstring
std::wstring String::ToWstring(std::string str)
	std::wstring temp = L"";

// std::wstring -> std::string
std::string String::ToString(std::wstring str)
	std::string temp = "";
{
	//npos no position 즉 없다는 소리
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && (int)index == 0) return true;

	return false;
}

bool String::StartWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && (int)index == 0) return true;
	return false;
}
#pragma endregion

// 특정한 문자를 포함하는지 체크
#pragma region Contain
bool String::Contain(string str, string comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}

bool String::Contain(wstring str, wstring comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}
#pragma endregion

// 특정한 문자들을 다른 문자들로 대체
#pragma region Replace
void String::Replace(string* str, string comp, string rep)
{
	string temp = *str;

	size_t startPos = 0;
	while ((startPos = temp.find(comp, startPos)) != wstring::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	*str = temp;
}

void String::Replace(wstring* str, wstring comp, wstring rep)
{
	wstring temp = *str;

	size_t startPos = 0;
	while ((startPos = temp.find(comp, startPos)) != wstring::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	*str = temp;
}
#pragma endregion

// 벡터를 문자열로 바꿔서 출력
#pragma region ToString
// setprecision : 부동 소수점 숫자의 정밀도를 설정하는 조작자

string String::ToString(Vector3 vec3, int precision)
{
	string temp;
	stringstream stream1, stream2, stream3;
	stream1 << fixed << setprecision(precision) << vec3.x;
	stream2 << fixed << setprecision(precision) << vec3.y;
	stream3 << fixed << setprecision(precision) << vec3.z;
	temp = "X : " + stream1.str() + " Y : " + stream2.str() + " Z : " + stream3.str();
	return temp;
}

wstring String::ToWstring(Vector3 vec3, int precision)
{
	wstring temp;
	wstringstream stream1, stream2, stream3;
	stream1 << fixed << setprecision(precision) << vec3.x;
	stream2 << fixed << setprecision(precision) << vec3.y;
	stream3 << fixed << setprecision(precision) << vec3.z;
	temp = L"X : " + stream1.str() + L" Y : " + stream2.str() + L" Z : " + stream3.str();
	return temp;
}
#pragma endregion

// string -> wstring
wstring String::ToWstring(string str)
{
	wstring temp = L"";
	temp.assign(str.begin(), str.end());
	return temp;
}

// wstring -> string
string String::ToString(wstring str)
{
	string temp = "";
	temp.assign(str.begin(), str.end());
	return temp;
}