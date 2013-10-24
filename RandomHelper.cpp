#include "RandomHelper.h"
#include <random>
static std::random_device randDevice;
static std::mt19937 randEngine(randDevice());

float cocos2d::extension::RandomFloat()
{
	std::uniform_real_distribution<float>dist(0 ,1);
	return dist(randEngine);
}
float cocos2d::extension::RandomFloat(float max)
{
std::uniform_real_distribution<float>dist(0,max);
	return dist(randEngine);
}
float cocos2d::extension::RandomFloat(float min,float max)
{
	std::uniform_real_distribution<float>dist(min,max);
	return dist(randEngine);
}
int cocos2d::extension::RandomInt()
{
	std::uniform_int_distribution<int> dist(0,INT_MAX);
	return dist(randEngine);
}
int cocos2d::extension::RandomInt(int max)
{
	std::uniform_int_distribution<int> dist(0,max);
	return dist(randEngine);
}
int cocos2d::extension::RandomInt(int min,int max)
{
	std::uniform_int_distribution<int> dist(min,max);
	return dist(randEngine);
}
bool cocos2d::extension::RandomBool()
{
	std::uniform_int_distribution<int> dist(0,1);
	return (bool)dist(randEngine);
}
float cocos2d::extension::RandomVariation(float value, float variation)
{
	float min=value-variation;
	float max=value+variation;
	return RandomFloat(min,max);
}
