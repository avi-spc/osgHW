#include <osg/Geode>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgDB\ReadFile>

osg::Group* root = new osg::Group;

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

	typedef std::map<int, osg::ref_ptr<osg::Switch> > KeyModelMap;
	KeyModelMap _keyModelMap;

};

void KeyboardModel::keyChange(int key, int virtualKey, int value)
{
	//KeyModelMap::iterator itr = _keyModelMap.find(virtualKey);
	if (value)
	{
		if (key == osgGA::GUIEventAdapter::KEY_Left)
		{
			osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/left.png");

			std::cout << "left" << std::endl;

			osg::StateSet* stateset = new osg::StateSet;

			osg::Texture2D* texture = new osg::Texture2D;
			texture->setImage(image);

			stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

			geo->setStateSet(stateset);

		}
		else if (key == osgGA::GUIEventAdapter::KEY_Right)
		{
			osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/right.png");

			std::cout << "right" << std::endl;

			osg::StateSet* stateset = new osg::StateSet;

			osg::Texture2D* texture = new osg::Texture2D;
			texture->setImage(image);

			stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

			geo->setStateSet(stateset);

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
			// Record mouse location for the button press
			// and move events.
			_mX = ea.getX();
			_mY = ea.getY();
			return false;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			// If the mouse hasn't moved since the last
			// button press or move event, perform a
			// pick. (Otherwise, the trackball
			// manipulator will handle it.)
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
	// Store mouse xy location for button press & move events.
	bool touched = false;
	float _mX, _mY;
	// Perform a pick operation.
	bool pick(const double x, const double y,
		osgViewer::Viewer* viewer)
	{
		if (!viewer->getSceneData())
			// Nothing to pick.
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


			if (intersection.drawable->getName() == "O1" || intersection.drawable->getName() == "O2") {
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
			//	unsigned int idx = nodePath.size();
			//	while (idx--)
			//	{
			//		// Find the LAST MatrixTransform in the node
			//		// path; this will be the MatrixTransform
			//		// to attach our callback to.
			//		osg::MatrixTransform* mt =
			//			dynamic_cast<osg::MatrixTransform*>(
			//				nodePath[idx]);
			//		if (mt == NULL)
			//			continue;
			//		// If we get here, we just found a
			//		// MatrixTransform in the nodePath.
			//		if (nodePath[nodePath.size()-1].valid())
			//			// Clear the previous selected node's
			//			// callback to make it stop spinning.
			//			_selectedNode->setUpdateCallback(NULL);
			//		_selectedNode = mt;
			//			_selectedNode->setUpdateCallback();
			//		break;
			//	}
			//	if (!_selectedNode.valid())
			//		osg::notify() << "Pick failed." << std::endl;
			//}
			//else if (_selectedNode.valid())
			//{
			//	_selectedNode->setUpdateCallback(NULL);
			//	_selectedNode = NULL;
			//}
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

		//First O
		{
			osg::Geometry* o1 = createGeometry(1.0f, 50, osg::Vec3(-3.5, 0, 2.5));

			osg::Vec4Array* colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			o1->setColorArray(colors, osg::Array::BIND_OVERALL);

			osg::Vec3Array* normals = new osg::Vec3Array;
			normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
			o1->setNormalArray(normals, osg::Array::BIND_OVERALL);

			o1->setName("O1");

			osg::Geode* geode = new osg::Geode();

			geode->addDrawable(o1);

			root->addChild(geode);
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
		{
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
		}

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

			quad2->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP, numIndices, myIndices));

			osg::StateSet* stateset = new osg::StateSet;

			osg::Texture2D* texture = new osg::Texture2D;
			texture->setImage(image);

			stateset->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

			quad2->setStateSet(stateset);

			osg::Geode* geode = new osg::Geode();

			geode->addDrawable(quad2);

			geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
				osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

			// create a transform to move the background back and forward with.
			/*osg::MatrixTransform* transform = new osg::MatrixTransform();
			osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
			osg::Matrix m;
			m.makeScale(0, 0, -300);
			mt->setMatrix(m);
			transform->addChild(geode);*/
			root->addChild(geode);
		}

		//Quad underline
		{
			osg::Geometry* underline = new osg::Geometry();

			osg::Vec3 myCoords[] =
			{
				osg::Vec3(-10, 0, 0.2),
				osg::Vec3(-10, 0, -0.2),
				osg::Vec3(10, 0, -0.2),
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

	return viewer.run();
}

