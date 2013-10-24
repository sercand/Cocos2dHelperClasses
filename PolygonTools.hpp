#ifndef _POLYGONTOOLS_H
#define _POLYGONTOOLS_H
#include <vector>
#include "HelperFunctions.h"
using namespace cocos2d;
namespace cocos2d { namespace extension {

    // Creates a ellipse with the specified width, height and number of edges.
    // xRadius: Width of the ellipse.
    // yRadius: Height of the ellipse.
    // numberOfEdges: The number of edges. The more edges, the more it resembles an ellipse
	static PolygonShape CreateEllipse(float xRadius, float yRadius, int numberOfEdges)
	{
		std::vector<CCPoint> vertices;
	
		float stepSize = cocos2d::extension::TwoPi/numberOfEdges;

		vertices.push_back(ccp(xRadius, 0));
		for (int i = numberOfEdges - 1; i > 0; --i)
			vertices.push_back(ccp(xRadius*(float) cos(stepSize*i),
													-yRadius*(float) sin(stepSize*i)));
		return vertices;
	}
    // Rounded rectangle contributed by Jonathan Smars - jsmars@gmail.com
    // Creates a rounded rectangle with the specified width and height.
    // width: The width.
    // height: The height.
    // xRadius: The rounding X radius.
    // yRadius: The rounding Y radius.
    // segments: The number of segments to subdivide the edges.</param>
	static PolygonShape CreateRoundedRectangle(float width, float height, 
		float xRadius, float yRadius, int segments)
	{
			std::vector<CCPoint> vertices;
			if (yRadius > height / 2 || xRadius > width / 2)
				return vertices;
			if (segments < 0)
				return vertices;
			//We need at least 8 vertices to create a rounded rectangle
			//Debug.Assert(MathHelper.MaxPolygonVertices >= 8);

			if (segments == 0)
			{
				vertices.push_back( ccp(width*.5f - xRadius, -height*.5f));
				vertices.push_back( ccp(width*.5f, -height*.5f + yRadius));

				vertices.push_back( ccp(width*.5f, height*.5f - yRadius));
				vertices.push_back( ccp(width*.5f - xRadius, height*.5f));

				vertices.push_back( ccp(-width*.5f + xRadius, height*.5f));
				vertices.push_back( ccp(-width*.5f, height*.5f - yRadius));

				vertices.push_back( ccp(-width*.5f, -height*.5f + yRadius));
				vertices.push_back( ccp(-width*.5f + xRadius, -height*.5f));
			}
			else
			{
				int numberOfEdges = (segments*4 + 8);

				float stepSize = cocos2d::extension::TwoPi/(numberOfEdges - 4);
				int perPhase = numberOfEdges/4;
				
				CCPoint posOffset = ccp(width/2 - xRadius, height/2 - yRadius);
				vertices.push_back(ccpAdd( posOffset, ccp(xRadius, -yRadius + yRadius)));
				short phase = 0;
				for (int i = 1; i < numberOfEdges; i++)
				{
					if (i - perPhase == 0 || i - perPhase*3 == 0)
					{
						posOffset.x *= -1;
						phase--;
					}
					else if (i - perPhase*2 == 0)
					{
						posOffset.y *= -1;
						phase--;
					}

					vertices.push_back(ccpAdd( posOffset,ccp(xRadius* cos(stepSize*-(i + phase)),
															-yRadius* sin(stepSize*-(i + phase)))));
				}
			}

			return vertices;
	}

    // Build vertices to represent an axis-aligned box.
    // hx: the half-width
    // hy: the half-height
	static PolygonShape CreateRectangle(float width, float height)
	{
	    std::vector<CCPoint> vertices;
		vertices.push_back(ccp(-width / 2 , -height / 2));
		vertices.push_back(ccp(width / 2, -height / 2));
		vertices.push_back(ccp(width / 2, height / 2));
		vertices.push_back(ccp(-width / 2, height / 2));
		return vertices;
	}

}}
#endif