This project has several supporting functions, classes for cocos2d developers. I have collected them from different projects which I don't remember. I used c++11 ,therefore, you need c++11 compiler such as Visual Studio 2012 and gcc 4.7 .
###Features
- "RandomHelper.h" requires c++11, calculates random int, float and bool variables
- "InputManager.h" gamepad, keyboard and mouse support for cocos2d windows. You must call InputManager::Initialize() before use
- "UtfConverter.h" a converter to convert between std::wstring and std::string
- "tinyxml2.h" https://github.com/leethomason/tinyxml2 , you should update tinyxml2 from main page
- "BayazitDecomposer.h" Convex decomposition algorithm created by Mark Bayazit, see http://mnbayazit.com/406/bayazit
- "PolygonTools.hpp" has CreateRectangle, CreateRoundedRectangle, CreateEllipse functions
