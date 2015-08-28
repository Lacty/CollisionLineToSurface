
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/TriMesh.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CollisionLineToPlaneApp : public AppNative {
private:
  struct Plane {
    Vec3f v1, v2, v3;
    Vec3f center;
    Vec3f normal;
    ColorA color;
  };
  Plane plane;
  
  struct Line {
    Vec3f start;
    Vec3f end;
    ColorA color;
  };
  Line line;
  
public:
  void setup();
  void mouseDown(MouseEvent event);
  void update();
  void draw();
};

void CollisionLineToPlaneApp::setup() {
  plane.v1 = Vec3f(250, 100, -50);
  plane.v2 = Vec3f(350, 250, 50);
  plane.v3 = Vec3f(250, 400, -50);
  
  // ポリゴンの中点を求める
  // Center = 1/3 * (A + B + C); より
  plane.center = [&] {
    return (0.33333333f * plane.v1) +
           (0.33333333f * plane.v2) +
           (0.33333333f * plane.v3);
  } ();
  
  console() << plane.center << endl;
  
  // 面の方線を求める
  plane.normal = [&] {
    Vec3f a = plane.v2 - plane.v1;
    Vec3f b = plane.v3 - plane.v1;
    return b.cross(a).normalized();
  } ();
  
  plane.color = Color(1, 1, 1);
  
  line.start = Vec3f(0, 250, 0);
  line.end   = Vec3f::zero();
  line.color = Color(0, 1, 0);
}

void CollisionLineToPlaneApp::mouseDown(MouseEvent event) {}

void CollisionLineToPlaneApp::update() {
  line.end = Vec3f(getMousePos(), 0);
  
  // http://marupeke296.com/COL_3D_No3_LineToPlane.html より
  // 線と面の当たり判定をとる
  bool isCollision = [&] {
    Vec3f v1 = line.start - plane.center;
    Vec3f v2 = line.end   - plane.center;
    return (v1.dot(plane.normal)) * (v2.dot(plane.normal)) <= 0;
  } ();
  
  plane.color = isCollision ? ColorA(1, 1, 1) : ColorA(1, 0, 0);
}

void CollisionLineToPlaneApp::draw() {
	// clear out the window with black
	gl::clear(Color(0, 0, 0));
  
  gl::color(plane.color);
  gl::drawLine(plane.v1, plane.v2);
  gl::drawLine(plane.v2, plane.v3);
  gl::drawLine(plane.v3, plane.v1);
  
  gl::drawCube(plane.center, Vec3f::one());
  
  gl::color(line.color);
  gl::drawLine(line.start, line.end);
}

CINDER_APP_NATIVE(CollisionLineToPlaneApp, RendererGl)
