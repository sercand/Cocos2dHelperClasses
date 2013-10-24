#ifndef _BAYAZIT_H
#define _BAYAZIT_H
/// 
/// Convex decomposition algorithm created by Mark Bayazit (http://mnbayazit.com/)
/// For more information about this algorithm, see http://mnbayazit.com/406/bayazit
/// 

#include <vector>
#include "cocos2d.h"
#include "HelperFunctions.h"

namespace cocos2d { namespace extension {
 std::vector<PolygonShape> DecomposePolygon(PolygonShape poly);
}}
#endif
