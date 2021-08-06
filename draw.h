#pragma once

#include "box2d/box2d.h"
#include "graphics.h"
#include <vector>

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class DebugDraw : public b2Draw
{
public:
	int scale = 20;

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		std::vector<POINT> points;
		for (int32 i = 0; i < vertexCount; ++i) {
			POINT point;
			point.x = vertices[i].x*scale;
			point.y = vertices[i].y*scale;
			points.push_back(point);
		}
		polygon(points.data(), points.size());
	}

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		std::vector<POINT> points;
		for (int32 i = 0; i < vertexCount; ++i) {
			POINT point;
			point.x = vertices[i].x*scale;
			point.y = vertices[i].y*scale;
			points.push_back(point);
		}
		fillpolygon(points.data(), points.size());
	}

	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
		circle(center.x*scale, center.y*scale, radius*scale);
	}

	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
		fillcircle(center.x*scale, center.y*scale, radius*scale);
	}

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
		line(p1.x*scale, p1.y*scale, p2.x*scale, p2.y*scale);
	}

	void DrawTransform(const b2Transform& xf) {
	}

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
		putpixel(p.x*scale, p.y*scale, RGB(255, 255, 255));
	}
};
