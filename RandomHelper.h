#ifndef __RANDOM_HELPER_H__
#define __RANDOM_HELPER_H__

namespace cocos2d { namespace extension {
	float RandomFloat(float min,float max);
	float RandomFloat(float max);	
	float RandomFloat();//0-1
	int RandomInt(int min,int max);	
	int RandomInt(int max);	
	int RandomInt();
	bool RandomBool();
	float RandomVariation(float value, float variation);
}}
#endif //__RANDOM_HELPER_H__