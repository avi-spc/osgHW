#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>

#include <osg/Camera>

#include <cmath>

osg::Group* root = new osg::Group;
osg::Matrix md;
osg::Matrix m;
osg::BoundingSphere bs;
//osg::Vec3 initialVector;
//osg::Vec3 changedVector;
osg::Vec3 forwardVector(0, 1000, 0);
osg::Vec3 rootNodeVec(0, 0, 0);

double fov;
struct coords {
	double fL;
	double fR;
	double fT;
	double fB;
};

coords c;
osg::Vec3 rightTop;
osg::Vec3 leftTop;
float angle;

osg::Matrixd proj;

float getMagnitude(osg::Vec3 v) {
	float mag = sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());

	return mag;
}

float getAngle(osg::Vec3 a, osg::Vec3 b) {
	float mag1 = getMagnitude(a);
	float mag2 = getMagnitude(b);

	float vecDotProd(a.x() * b.x() + a.y() * b.y() + a.z() * b.z());

	float resVec(vecDotProd/(mag1*mag2));
	
	angle = acosf(resVec) * (180 / 3.14) * 10;

	return angle;
}

void createFrustum(osg::Camera* cam) {


	double far;
	double near;
	double aspectRatio;

	cam->getProjectionMatrix().getPerspective(fov, aspectRatio, near, far);
	//proj = cam->getViewMatrix().getper

	getAngle(rootNodeVec, forwardVector);

}


float getResultantVector(osg::Vec3 a, osg::Vec3 b) {
	osg::Vec3 c = osg::Vec3(b.x() - a.x(), b.y() - a.y(), b.z() - a.z());

	float resVector = sqrt(c.x() * c.x() + c.y() * c.y() + c.z() * c.z());

	return resVector;
}

float distance(osg::Matrix m, osg::BoundingSphere& bs) {
	float x = m.getTrans().x() - bs.center().x();
	float y = m.getTrans().y() - bs.center().y();
	float z = m.getTrans().z() - bs.center().z();
	
	float dis = sqrt(x * x + y * y + z * z);
	
	return dis;
}

class CameraHandler : public osgGA::GUIEventHandler {
public:
	CameraHandler(osgViewer::Viewer* v) {
		viewer = v;
	}

	osg::Vec3f eye = osg::Vec3f(0.0, 0.0, 0.0);
	osg::Vec3f centre = osg::Vec3f(0.0, 0.0, 0.0);
	osg::Vec3f up = osg::Vec3f(0.0, 0.0, 1.0);


	osgViewer::Viewer* getViewer() {
		return viewer;
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::PUSH):
		{
			osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
			//if (view) pick(view,ea);
			return false;
		}
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)
			{
				md = getViewer()->getCamera()->getViewMatrix();
				
				osg::Vec3d trans = md.getTrans();

				md.getLookAt(eye, centre, up);

				trans.z() = trans.z() + 2;

				forwardVector.z() = forwardVector.z() + 2;
				m = getViewer()->getCamera()->getWorldMatrices()[0];

				rootNodeVec = osg::Vec3(bs.center().x() - m.getTrans().x(), bs.center().y() - m.getTrans().y(), bs.center().z() - m.getTrans().z());


				if (getAngle(rootNodeVec, forwardVector) < fov/2) {
					md.setTrans(trans);
				}
				else {
					trans.z() = trans.z() - 2;
					forwardVector.z() = forwardVector.z() - 2;
				}

				getViewer()->getCamera()->setViewMatrix(md);

			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)
			{
				md = getViewer()->getCamera()->getViewMatrix();
				osg::Vec3d trans = md.getTrans();

				md.getLookAt(eye, centre, up);
				trans.z() = trans.z() - 2;
				//initialVector = osg::Vec3(md.getTrans().x(), md.getTrans().y(), md.getTrans().z() - 2);
				forwardVector.z() = forwardVector.z() - 2;
				m = getViewer()->getCamera()->getWorldMatrices()[0];
				rootNodeVec = osg::Vec3(bs.center().x() - m.getTrans().x(), bs.center().y() - m.getTrans().y(), bs.center().z() - m.getTrans().z());


				//forwardVector.z() = forwardVector.z() - 2;

				if (getAngle(rootNodeVec, forwardVector) < fov / 2) {
					md.setTrans(trans);

					//getViewer()->getCamera()->setProjectionMatrixAsPerspective(90, 1.333, 0.1, 50);
					//angle = getAngle()
				}
				else {
					trans.z() = trans.z() + 2;
					forwardVector.z() = forwardVector.z() + 2;

				}
					getViewer()->getCamera()->setViewMatrix(md);

				//std::cout << distance(md, bs) << std::endl;

				//std::cout << a << std::endl;

			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)
			{
				md = getViewer()->getCamera()->getViewMatrix();
				osg::Vec3d trans = md.getTrans();

				md.getLookAt(eye, centre, up);

				trans.x() = trans.x() + 2;

				//changedVector = osg::Vec3(trans.x(), md.getTrans().y(), md.getTrans().z());


				forwardVector.x() = forwardVector.x() + 2;
				m = getViewer()->getCamera()->getWorldMatrices()[0];

				rootNodeVec = osg::Vec3(bs.center().x() - m.getTrans().x(), bs.center().y() - m.getTrans().y(), bs.center().z() - m.getTrans().z());


				if (getAngle(rootNodeVec, forwardVector) < fov / 2) {
					md.setTrans(trans);

					//getViewer()->getCamera()->setProjectionMatrixAsPerspective(90, 1.333, 0.1, 50);
					
				}
				else {
					trans.x() = trans.x() - 2;
					forwardVector.x() = forwardVector.x() - 2;

				}


				getViewer()->getCamera()->setViewMatrix(md);
				//std::cout << distance(md, bs) << std::endl;
				//std::cout << a << std::endl;
			}

			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)
			{
				md = getViewer()->getCamera()->getViewMatrix();
				osg::Vec3d trans = md.getTrans();

				trans.x() = trans.x() - 2;

				forwardVector.x() = forwardVector.x() - 2;
				m = getViewer()->getCamera()->getWorldMatrices()[0];

				rootNodeVec = osg::Vec3(bs.center().x() - m.getTrans().x(), bs.center().y() - m.getTrans().y(), bs.center().z() - m.getTrans().z());


				//changedVector = osg::Vec3(trans.x(), md.getTrans().y(), md.getTrans().z());

				if (getAngle(rootNodeVec, forwardVector) < fov / 2) {
					md.setTrans(trans);


				}
				else {
					trans.x() = trans.x() + 2;

					forwardVector.x() = forwardVector.x() + 2;
				}

				getViewer()->getCamera()->setViewMatrix(md);

			}
		}
		}
	}
public:
	osgViewer::Viewer* viewer;
};

class UpdateUV : public osg::NodeCallback
{
public:
	UpdateUV(osg::Geometry* geom, osg::ref_ptr<osg::Image> image, int direction) : _pos(-1.) {
		geo = geom;
		this->image = image;
		xDirection = -3.5 * direction;
	}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::Vec2 myTexCoords[] =
		{
			osg::Vec2(_pos , 1),
			osg::Vec2(_pos , 0),
			osg::Vec2(_pos + xDirection, 0),
			osg::Vec2(_pos + xDirection, 1)
		};

		int numTexCoords = sizeof(myTexCoords) / sizeof(osg::Vec2);

		geo->setTexCoordArray(0, new osg::Vec2Array(numTexCoords, myTexCoords));

		osg::StateSet* stateset = new osg::StateSet;

		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

		geo->setStateSet(stateset);

		_pos += 0.03;

		if (xDirection < 0) {
			if (_pos > 3)
				_pos = 0.03f;
		}
		if (xDirection > 0) {
			if (_pos > 1)
				_pos = -2.0;
		}
	}
protected:
	double _pos;
	osg::Geometry* geo;
	osg::ref_ptr<osg::Image> image;
	int xDirection;
};

class FindGeoNamedNode :
	public osg::NodeVisitor
{
public:
	FindGeoNamedNode();
	FindGeoNamedNode(const std::string name) :
		osg::NodeVisitor(TRAVERSE_ALL_CHILDREN)
	{
		resultNode = NULL;
		this->name = name;
	}

	virtual void apply(osg::Node& searchNode)
	{
		if (searchNode.getName() == name)
		{
			osg::Geometry* dynamicTry = dynamic_cast<osg::Geometry*>(&searchNode);

			if (dynamicTry)
			{
				resultNode = dynamicTry;
			}
		}
		traverse(searchNode);
	}

	osg::Geometry* getNode()
	{
		return resultNode;
	}
private:
	osg::Geometry* resultNode;
	std::string name;
};

class KeyboardModel : public osg::Referenced
{
public:
	KeyboardModel(osg::Geometry* geom) : geo(geom) {}

	void keyChange(int key, int virtualKey, int value);
	osg::Geometry* geo;

protected:

	~KeyboardModel() {}

	typedef std::map<int, osg::ref_ptr<osg::Switch>> KeyModelMap;
	KeyModelMap _keyModelMap;

};

void KeyboardModel::keyChange(int key, int virtualKey, int value)
{
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	//mt->setName("h");
	mt->setDataVariance(osg::Object::DYNAMIC);

	osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/left.png");

	//bool lr = true;
	
	//KeyModelMap::iterator itr = _keyModelMap.find(virtualKey);
	if (value)
	{
		if (key == osgGA::GUIEventAdapter::KEY_Left)
		{
			mt->setUpdateCallback(new UpdateUV(geo, image, -1));

			root->addChild(mt);
		}
		if (key == osgGA::GUIEventAdapter::KEY_Right) {

			mt->setUpdateCallback(new UpdateUV(geo, image, 1));

			root->addChild(mt);

		}

	}

}

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

	KeyboardEventHandler(KeyboardModel* keyboardModel) :
		_keyboardModel(keyboardModel) {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			_keyboardModel->keyChange(ea.getKey(), ea.getUnmodifiedKey(), 1);
			return true;
		}
		case(osgGA::GUIEventAdapter::KEYUP):
		{
			_keyboardModel->keyChange(ea.getKey(), ea.getUnmodifiedKey(), 0);
			return true;
		}

		default:
			return false;
		}
	}

	osg::ref_ptr<KeyboardModel> _keyboardModel;

};


class PickHandler : public osgGA::GUIEventHandler
{
public:
	PickHandler() : _mX(0.), _mY(0.) {}
	bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		osgViewer::Viewer* viewer =
			dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)
			return false;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
		case osgGA::GUIEventAdapter::MOVE:
		{
			_mX = ea.getX();
			_mY = ea.getY();
			return false;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			if (_mX == ea.getX() && _mY == ea.getY())
			{
				if (pick(ea.getXnormalized(),
					ea.getYnormalized(), viewer))
					return true;
			}
			return false;
		}
		default:
			return false;
		}
	}
protected:

	bool touched = false;
	float _mX, _mY;

	bool pick(const double x, const double y,
		osgViewer::Viewer* viewer)
	{
		if (!viewer->getSceneData())
			return false;

		double w(.05), h(.05);
		//osgUtil::PolytopeIntersector* picker = new osgUtil::PolytopeIntersector(osgUtil::Intersector::PROJECTION, x - w, y - h, x + w, y + h);

		osgUtil::LineSegmentIntersector* picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION, x, y);
		osgUtil::IntersectionVisitor iv(picker);
		viewer->getCamera()->accept(iv);
		if (picker->containsIntersections())
		{
			const osg::NodePath& nodePath =
				picker->getFirstIntersection().nodePath;

			const osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();

			/*std::cout << intersection.drawable->className() << std::endl;
			std::cout << intersection.drawable->getName() << std::endl;*/

			osg::Geometry* g = new osg::Geometry;

			g = intersection.drawable->asGeometry();


			if (g->getName() == "O1") {
				touched = !touched;

				if (touched) {
					osg::Vec4Array* colors = new osg::Vec4Array;
					colors->push_back(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f));
					g->setColorArray(colors, osg::Array::BIND_OVERALL);
				}
				else if (!touched) {
					osg::Vec4Array* colors = new osg::Vec4Array;
					colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
					g->setColorArray(colors, osg::Array::BIND_OVERALL);
				}
			}
			return true;
		}
	}
};

osg::Vec3Array* getAllPoints(const float radius, const int points, const osg::Vec3& center) {
	auto array = new osg::Vec3Array();
	const float ratio = (4 * float(2.0 * osg::PI) / float(points));
	for (int i = 0; i < points; i++) {
		const float angle = float(i) * ratio;
		array->push_back(osg::Vec3(
			center.x() + radius * cosf(angle),
			center.y(),
			center.z() + radius * sinf(angle))
		);
	}
	return array;
}

osg::Geometry* createGeometry(const float radius, const int points, const osg::Vec3& center) {
	osg::Geometry* geom = new osg::Geometry();

	geom->setVertexArray(getAllPoints(radius, points, center));
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP, 0, points));

	return geom;
}

osg::ref_ptr<osg::Node> createModel() {

	//First Quad with Hell
	{
		osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/hell.png");
		if (!image) return NULL;

		osg::Geometry* quad1 = new osg::Geometry();

		osg::Vec3 myCoords[] =
		{
			osg::Vec3(-10, 0, 4),
			osg::Vec3(-10, 0, 1),
			osg::Vec3(-5, 0, 1),
			osg::Vec3(-5, 0, 4),
		};

		int numCoords = sizeof(myCoords) / sizeof(osg::Vec3);

		quad1->setVertexArray(new osg::Vec3Array(numCoords, myCoords));

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		quad1->setColorArray(colors, osg::Array::BIND_OVERALL);


		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		quad1->setNormalArray(normals, osg::Array::BIND_OVERALL);

		osg::Vec2 myTexCoords[] =
		{
			osg::Vec2(0,1),
			osg::Vec2(0,0),
			osg::Vec2(1,0),
			osg::Vec2(1,1)
		};

		int numTexCoords = sizeof(myTexCoords) / sizeof(osg::Vec2);

		quad1->setTexCoordArray(0, new osg::Vec2Array(numTexCoords, myTexCoords));

		unsigned short myIndices[] =
		{
			0,
			1,
			3,
			2
		};

		int numIndices = sizeof(myIndices) / sizeof(unsigned short);

		quad1->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP, numIndices, myIndices));

		osg::StateSet* stateset = new osg::StateSet;

		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

		quad1->setStateSet(stateset);

		osg::Geode* geode = new osg::Geode();

		geode->addDrawable(quad1);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
			osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

		root->addChild(geode);
	}

	// O'O
	{
		osg::Geometry* o1 = createGeometry(1.0f, 50, osg::Vec3(-3.5, 0, 2.5));

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		o1->setColorArray(colors, osg::Array::BIND_OVERALL);

		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		o1->setNormalArray(normals, osg::Array::BIND_OVERALL);

		o1->setName("O1");

		osg::Matrix m;
		osg::Geode* geode = new osg::Geode();

		geode->addDrawable(o1);

		{
			osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
			m.makeTranslate(0.f, 0.f, 0.f);
			mt->setMatrix(m);
			root->addChild(mt.get());
			mt->addChild(geode);
		}

		{
			osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
			m.makeTranslate(7, 0, 0);
			mt->setMatrix(m);
			root->addChild(mt.get());
			mt->addChild(geode);
		}
	}

	//W
	{
		osg::Geode* geode = new osg::Geode;

		osg::ref_ptr<osgText::Font> font = osgText::readRefFontFile("fonts/arial.ttf");

		osg::Vec4 wColor(1.0f, 1.0f, 1.0f, 1.0f);
		float charSize = 3.2f;

		float theta(osg::PI * 0.5f);
		osg::Vec3 axis(1, 0, 0);
		osg::Quat q0(theta, axis);

		osgText::Text* text = new osgText::Text;
		text->setFont(font);
		text->setColor(wColor);
		text->setCharacterSize(charSize);
		text->setFontResolution(64, 64);
		text->setPosition(osg::Vec3(1, 0, 1.5));
		//text->setAutoRotateToScreen(true);

		text->setLayout(osgText::Text::LEFT_TO_RIGHT);
		text->setAlignment(osgText::Text::CENTER_BOTTOM_BASE_LINE);
		text->setRotation(q0);
		text->setText("W");
		geode->addDrawable(text);

		root->addChild(geode);
	}

	//Second O
	/*{
		osg::Geometry* o2 = createGeometry(1.0f, 50, osg::Vec3(3.5, 0, 2.5));

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		o2->setColorArray(colors, osg::Array::BIND_OVERALL);


		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		o2->setNormalArray(normals, osg::Array::BIND_OVERALL);

		o2->setName("O2");

		osg::Geode* geode = new osg::Geode();

		geode->addDrawable(o2);

		root->addChild(geode);
	}*/

	//Second quad with rld
	{
		osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/rld.png");
		if (!image) return NULL;

		osg::Geometry* quad2 = new osg::Geometry();

		osg::Vec3 myCoords[] =
		{
			osg::Vec3(5, 0, 4), //5
			osg::Vec3(5, 0, 1), //0
			osg::Vec3(10, 0, 1), //0
			osg::Vec3(10, 0, 4), //5
		};


		int numCoords = sizeof(myCoords) / sizeof(osg::Vec3);

		quad2->setVertexArray(new osg::Vec3Array(numCoords, myCoords));

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		quad2->setColorArray(colors, osg::Array::BIND_OVERALL);


		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		quad2->setNormalArray(normals, osg::Array::BIND_OVERALL);

		osg::Vec2 myTexCoords[] =
		{
			osg::Vec2(0,1),
			osg::Vec2(0,0),
			osg::Vec2(1,0),
			osg::Vec2(1,1)
		};

		int numTexCoords = sizeof(myTexCoords) / sizeof(osg::Vec2);

		quad2->setTexCoordArray(0, new osg::Vec2Array(numTexCoords, myTexCoords));

		unsigned short myIndices[] =
		{
			0,
			1,
			3,
			2
		};



		int numIndices = sizeof(myIndices) / sizeof(unsigned short);
		//quad2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4, 1));
		quad2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

		//quad2->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP, numIndices, myIndices));

		osg::StateSet* stateset = new osg::StateSet;

		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

		quad2->setStateSet(stateset);

		osg::Geode* geode = new osg::Geode();

		geode->addDrawable(quad2);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
			osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

		root->addChild(geode);
	}

	//Quad underline
	{

		osg::Geometry* underline = new osg::Geometry();

		osg::Vec3 myCoords[] =
		{
			osg::Vec3(-10, 0, 0.2),
			osg::Vec3(-10, 0, -1),
			osg::Vec3(10, 0, -1),
			osg::Vec3(10, 0, 0.2),
		};

		int numCoords = sizeof(myCoords) / sizeof(osg::Vec3);

		underline->setVertexArray(new osg::Vec3Array(numCoords, myCoords));

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		underline->setColorArray(colors, osg::Array::BIND_OVERALL);


		osg::Vec3Array* normals = new osg::Vec3Array;
		normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
		underline->setNormalArray(normals, osg::Array::BIND_OVERALL);

		osg::Vec2 myTexCoords[] =
		{
			osg::Vec2(0,1),
			osg::Vec2(0,0),
			osg::Vec2(1,0),
			osg::Vec2(1,1)
		};

		int numTexCoords = sizeof(myTexCoords) / sizeof(osg::Vec2);

		underline->setTexCoordArray(0, new osg::Vec2Array(numTexCoords, myTexCoords));

		unsigned short myIndices[] =
		{
			0,
			1,
			3,
			2
		};

		underline->setName("underline");

		int numIndices = sizeof(myIndices) / sizeof(unsigned short);

		underline->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP, numIndices, myIndices));

		osg::Geode* geode = new osg::Geode();

		geode->addDrawable(underline);

		geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
			osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

		root->addChild(geode);
	}

	return root;
}

    
int main(int argc, char** argv)
{
	// create the view of the scene.
	osgViewer::Viewer viewer;
	viewer.setSceneData(createModel().get());


	osg::Node* testNode = NULL;
	testNode = dynamic_cast<osg::Node*>(viewer.getSceneData());

	FindGeoNamedNode* visitor = new FindGeoNamedNode("underline");
	testNode->accept(*visitor);

	osg::Geometry* geom = visitor->getNode();

	osg::ref_ptr<KeyboardModel> keyboardModel = new KeyboardModel(geom);

	viewer.addEventHandler(new PickHandler);
	viewer.addEventHandler(new KeyboardEventHandler(keyboardModel.get()));
	viewer.addEventHandler(new CameraHandler(&viewer));

	bs = testNode->getBound();

	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	viewer.getCamera()->setViewMatrixAsLookAt(bs.center() + osg::Vec3(0.0f, -(3 * bs.radius()), 0.0f), bs.center(), osg::Z_AXIS);

	md = viewer.getCamera()->getViewMatrix();

	m = viewer.getCamera()->getWorldMatrices()[0];

	rootNodeVec = osg::Vec3(bs.center().x() - m.getTrans().x(), bs.center().y() - m.getTrans().y(), bs.center().z() - m.getTrans().z());
	forwardVector = osg::Vec3(m.getTrans().x(), m.getTrans().y(), m.getTrans().z() - 1000);

	float initialDis = distance(md, bs);

	viewer.realize();
	createFrustum(viewer.getCamera());

	while (!viewer.done())
	{

		std::cout << angle <<std::endl;


		viewer.frame();
	}
	//return viewer.run();
}	 
