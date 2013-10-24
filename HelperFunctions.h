#ifndef __HELPER_H__
#define __HELPER_H__

#include "cocos2d.h"
#include <string.h>
#include <sstream>
using namespace std;
namespace cocos2d { namespace extension {

	const float E =  2.71828f;
    const float Log10E = 0.4342945f;
    const float Log2E = 1.442695f;
    const float Pi = 3.141592654f;
    const float PiOver2 = (Pi/2.0f);
    const float PiOver4 = (Pi/4.0f);
    const float TwoPi =  (Pi*2.0f);
    const float Epsilon = 1.19209289550781250000e-7F;
	typedef std::vector<cocos2d::CCPoint> PolygonShape;

	template<typename ValueType>
	static string NumberToString(ValueType const & number)
	{	
		ostringstream convert;
		convert<<number;
		return convert.str();
	}

	template<typename ReturnType>
	static ReturnType StringToNumber(string const & text)
	{
		istringstream ss(text);
		ReturnType result;
		return ss >> result ? result : 0;
	}

	static int HexToNumber(string& text)
	{
		unsigned int result;
		std::stringstream ss;
		ss << std::hex << text;
		ss >> result;
		return result;
	}
	static float Hermite(float value1, float tangent1, float value2, float tangent2, float amount)
	{
		float num3 = amount;
		float num = num3 * num3;
		float num2 = num3 * num;
		float num7 = ((2.0f * num2) - (3.0f * num)) + 1.0f;
		float num6 = (-2.0f * num2) + (3.0f * num);
		float num5 = (num2 - (2.0f * num)) + num3;
		float num4 = num2 - num;
		return ((((value1 * num7) + (value2 * num6)) + (tangent1 * num5)) + (tangent2 * num4));
	}

	static float Lerp(float value1, float value2, float amount)
    {
		return value1 + (value2 - value1)*amount;
    }

    static float Max(float value1, float value2)
	{
		return max<float>(value1,value2);
	}

	static float Min(float value1, float value2)
	{
		return min<float>(value1,value2);
	}

	static float Barycentric(float value1, float value2, float value3, float amount1, float amount2)
	{
            return value1 + (value2 - value1)*amount1 + (value3 - value1)*amount2;
	}

	static float CatmullRom(float value1, float value2, float value3, float value4, float amount)
	{
            // Using formula from http://www.mvps.org/directx/articles/catmull/
            // Internally using doubles not to lose precission
		double amountSquared = amount*amount;
		double amountCubed = amountSquared*amount;
		return (float) (0.5*(2.0*value2 +
							(value3 - value1)*amount +
							(2.0*value1 - 5.0*value2 + 4.0*value3 - value4)*amountSquared +
							(3.0*value2 - value1 - 3.0*value3 + value4)*amountCubed));
	}

	static float Clamp(float value, float min, float max)
	{
		// First we check to see if we're greater than the max
		value = (value > max) ? max : value;

		// Then we check to see if we're less than the min.
		value = (value < min) ? min : value;

		// There's no check to see if min > max.
		return value;
	}

	static float SmoothStep(float value1, float value2, float amount)
	{
		// It is expected that 0 < amount < 1
		// If amount < 0, return value1
		// If amount > 1, return value2
		float result = Clamp(amount, 0.0f, 1.0f);
		result = Hermite(value1, 0.0f, value2, 0.0f, result);
		return result;
	}

	static std::string ToLower(std::string text)
	{
		int j= text.length();
		for(int i=0;i<j;i++)
		{
			text[i]=tolower(text[i]);
		}
		return text;
	}

	static bool ParseBoolean(std::string text)
	{
		text=ToLower(text);
		if(text == "false")
			return false;
		else if(text == "true")
			return true;
		return false;
	}
	template<typename ReturnType>
	static ReturnType ParseNumber(string const & text)
	{
		istringstream ss(text);
		ReturnType result;
		return ss >> result ? result : 0;
	}
}}

#endif//__HELPER_H__