#ifndef __RANDOM_HELPER_H__
#define __RANDOM_HELPER_H__
// Created by Sercan Değirmenci
namespace cocos2d { namespace extension {
	//Returns a random float number between min and max
	float RandomFloat(float min,float max);
	
	//Returns a random float number between 0.0 and max
	float RandomFloat(float max);	
	
	//Returns a random number between 0.0 and 1.0
	float RandomFloat();//0-1
	
	//Returns a random integer number between min and max
	int RandomInt(int min,int max);
	
	//Returns a random integer number between 0 and max
	int RandomInt(int max);

	//Returns a random integer number between 0 and INT_MAX
	int RandomInt();

	//Returns a random boolean value
	bool RandomBool();

	//Returns a random float number between "value - variation" and "value + variation"
	float RandomVariation(float value, float variation);
}}
#endif //__RANDOM_HELPER_H__
