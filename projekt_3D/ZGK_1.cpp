#include <cstdio>
#include <cstdlib>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osg/Texture2D>
#include <osg/AnimationPath>
#include <osgGA/GUIEventHandler>
#include <osg/LightSource>
#include <osgGA/OrbitManipulator>
#include "Time.hpp"
#include "Input.hpp"

USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(obj)
USE_GRAPHICSWINDOW()

using namespace std;

bool win = false;


class RiverObject {
public:
    RiverObject(osg::MatrixTransform* node, int direction, double size)
        : _node(node), direction(direction), size(size) {}

    osg::MatrixTransform* getMatrix();
    int getDirection();
    double getSize();
    void setDirection(int dir);

private:
    osg::ref_ptr<osg::MatrixTransform> _node;
    int direction;
    double size;
};

osg::MatrixTransform* RiverObject::getMatrix() {
    return _node;
}

int RiverObject::getDirection() {
    return direction;
}

double RiverObject::getSize() {
    return size;
}

void RiverObject::setDirection(int dir) {
    direction = dir;
}





class ModelController : public osgGA::GUIEventHandler
{
public:
    ModelController(osg::MatrixTransform* node, vector<RiverObject*> matrixes)
        : _model(node), obj_matrixes(matrixes) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) override;
protected:
    osg::ref_ptr<osg::MatrixTransform> _model;
    vector<RiverObject*> obj_matrixes;
};


bool ModelController::handle(const osgGA::GUIEventAdapter& ea,
    osgGA::GUIActionAdapter& aa)
{
    if (ea.getEventType() != osgGA::GUIEventAdapter::FRAME) return false;

    if (!_model) return false;
    osg::Matrix frog_matrix = _model->getMatrix();
    osg::Vec3 frog_pos = frog_matrix.getTrans();
    

    if (!win) {
        if (Input::GetKeyDown('a')) {
            if (frog_pos.x() <= 10.0 && frog_pos.x() >= -8.0) {
                frog_matrix *= osg::Matrix::translate(-2.0, 0.0, 0.0);
                cout << "a" << endl;
            }
        }
        else if (Input::GetKeyDown('d')) {
            if (frog_pos.x() <= 8.0 && frog_pos.x() >= -10.0) {
                frog_matrix *= osg::Matrix::translate(2.0, 0.0, 0.0);
                cout << "d" << endl;
            }
        }
        else if (Input::GetKeyDown('w')) {
            if (frog_pos.y() < 10.0) {
                cout << "FROGG: (" << frog_pos.y() << endl;
                frog_matrix *= osg::Matrix::translate(0.0, 2.0, 0.0);
                cout << "w" << endl;
            }
            else {
                win = true;
            }
        }
    }
    
    _model->setMatrix(frog_matrix);
    frog_pos = frog_matrix.getTrans();

  
    //cout << "FROGG: (" << frog_pos.x() << ", " << frog_pos.y() << ", " << frog_pos.z() << ")" << endl;
    double deltaTime = Time::GetDeltaTime();

    for (int i = 0; i < obj_matrixes.size(); i++) {
        osg::Matrix obj_matrix = obj_matrixes[i]->getMatrix()->getMatrix();
        osg::Vec3d obj_pos = obj_matrix.getTrans();

        if ((frog_pos.y() == obj_pos.y()) && (frog_pos.x() >= obj_pos.x()-obj_matrixes[i]->getSize()) && (frog_pos.x() <= (obj_pos.x()+ obj_matrixes[i]->getSize()))) {
            if (frog_pos.x() < -10.0) {
                frog_pos.x() = -10.0;
            }
            else if (frog_pos.x() > 10.0) {
                frog_pos.x() = 10.0;
            }
            else {
                frog_pos.x() += deltaTime * obj_matrixes[i]->getDirection() * 5; //obj_matrixes[i]->getDirection() * 0.1;
            }

            //cout << "zmieniam na obiekcie" << endl;
            
            _model->setMatrix(
                osg::Matrixd::scale(_model->getMatrix().getScale()) *
                osg::Matrixd::rotate(osg::DegreesToRadians(-90.0), osg::Vec3(1, 0, 0)) *
                osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0, 0, 1)) *
                osg::Matrixd::translate(frog_pos)
            );
        } 

        //cout << "OBJ: (" << obj_pos.x() << ", " << obj_pos.y() << ", " << obj_pos.z() << ")" << endl;
    }

    return false;
}




class ObjController : public osgGA::GUIEventHandler
{
public:
    ObjController(RiverObject* obj, string type)//osg::MatrixTransform* node, int k, string type)
        : _obj(obj), type(type) {}
        //: _model(node), k(k), type(type) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) override;
protected:
    RiverObject* _obj;
    //osg::ref_ptr<osg::MatrixTransform> _model;
    //int k;
    string type;
};


bool ObjController::handle(const osgGA::GUIEventAdapter& ea,
    osgGA::GUIActionAdapter& aa) {
    
    if (ea.getEventType() != osgGA::GUIEventAdapter::FRAME) return false;

    double deltaTime = Time::GetDeltaTime();
    osg::Matrix matrix = _obj->getMatrix()->getMatrix();
    osg::Vec3 position = matrix.getTrans();


    if (type == "leaf") {
        if (position.x() < -10.0) {
            position.x() = -10.0;
            _obj->setDirection(1);
        }
        else if (position.x() > 10.0) {
            position.x() = 10.0;
            _obj->setDirection(-1); //k = -1;
        }
    }
    else {
        if (position.x() < -9.0) {
            position.x() = -9.0;
            _obj->setDirection(1); //k = 1;
        }
        else if (position.x() > 9.0) {
            position.x() = 9.0;
            _obj->setDirection(-1); //k = -1;
        }
    }
     
    position.x() +=  deltaTime * _obj->getDirection() * 5; //_obj->getDirection() * 0.1; 

    if (type == "leaf") {
        _obj->getMatrix()->setMatrix(
            osg::Matrixd::scale(_obj->getMatrix()->getMatrix().getScale()) *
            osg::Matrixd::rotate(_obj->getMatrix()->getMatrix().getRotate()) *
            osg::Matrixd::translate(position)
        );
    }
    else if (type == "planks") {
        _obj->getMatrix()->setMatrix(
            osg::Matrix::scale(0.015, 0.015, 0.04) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 0, 1)) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
            osg::Matrixd::translate(position)
        );
    }
    
    //cout << "pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")" << endl;
   
    return false;
}




osg::Node* createTable(string file_name)
{
    using osg::Vec2;
    using osg::Vec3;
    using osg::Vec4;

    // tablica ze współrzędnymi wierzchołków bryły
    auto v = new osg::Vec3Array();
    v->push_back(Vec3(0.0, 1.0, 1.0));
    v->push_back(Vec3(0.0, 0.0, 1.0));
    v->push_back(Vec3(1.0, 0.0, 1.0));
    v->push_back(Vec3(1.0, 1.0, 1.0));
    v->push_back(Vec3(0.0, 1.0, 0.0));
    v->push_back(Vec3(0.0, 0.0, 0.0));
    v->push_back(Vec3(1.0, 0.0, 0.0));
    v->push_back(Vec3(1.0, 1.0, 0.0));

    // tablica z kolorami wierzchołków -- definiujemy tylko jeden, później
    // zaznaczymy przez BIND_OVERALL że wszystkie wierzchołki mają przypisany
    // jeden wspólny kolor
    auto c = new osg::Vec4Array();
    c->push_back(Vec4(1.0, 1.0, 1.0, 1.0));

    // tablica z definicjami rysowanych prymitywów
    auto e = new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS);
    // pierwszy czworokąt:
    e->push_back(3);
    e->push_back(2);
    e->push_back(1);
    e->push_back(0);
    // drugi:
    e->push_back(7);
    e->push_back(6);
    e->push_back(5);
    e->push_back(4);
    // i następne:
    e->push_back(3);
    e->push_back(2);
    e->push_back(6);
    e->push_back(7);
    //
    e->push_back(4);
    e->push_back(0);
    e->push_back(3);
    e->push_back(7);
    //
    e->push_back(4);
    e->push_back(5);
    e->push_back(1);
    e->push_back(0);
    //
    e->push_back(1);
    e->push_back(5);
    e->push_back(6);
    e->push_back(2);

    osg::ref_ptr<osg::Vec2Array> texcoord = new osg::Vec2Array;
    texcoord->push_back(Vec2{ 1.0, 0.0 });
    texcoord->push_back(Vec2{ 0.0, 0.0 });
    texcoord->push_back(Vec2{ 0.0, 1.0 });
    texcoord->push_back(Vec2{ 1.0, 1.0 });

    texcoord->push_back(Vec2{ 1.0, 0.0 });
    texcoord->push_back(Vec2{ 0.0, 0.0 });
    texcoord->push_back(Vec2{ 0.0, 1.0 });
    texcoord->push_back(Vec2{ 1.0, 1.0 });

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(file_name);
    if (image == nullptr) {
        throw std::runtime_error("Couldn't read texture file!");
    }
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(image.get());
    texture->setUnRefImageDataAfterApply(true);

    auto g = new osg::Geometry();
    g->setVertexArray(v);
    g->addPrimitiveSet(e);
    g->setTexCoordArray(0, texcoord.get());
    g->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());

    g->setColorArray(c);
    g->setColorBinding(osg::Geometry::BIND_OVERALL);
    g->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

    auto n = new osg::Geode();
    n->addDrawable(g);
    return n;
}


osg::Node* createLightSource(
    unsigned int num,
    const osg::Vec3& trans,
    const osg::Vec4& color)
{
    osg::ref_ptr<osg::Light> light = new osg::Light{};
    light->setLightNum(num);
    light->setDiffuse(color);
    light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
    light->setPosition(osg::Vec4(0.0f, 0.0f, 3.0f, 1.0f));
    light->setDirection(osg::Vec3(0.0, 0.0, -1.0));
    light->setSpotCutoff(45.0f);

    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource{};
    lightSource->setLight(light);

    osg::ref_ptr<osg::MatrixTransform> sourceTrans = new osg::MatrixTransform{};
    sourceTrans->setMatrix(osg::Matrix::translate(trans));
    sourceTrans->addChild(lightSource.get());
    return sourceTrans.release();
}



osg::Node* stworz_scene(osgViewer::Viewer* viewer)
{
    auto scn = new osg::Group();

    viewer->getCamera()->getOrCreateStateSet()->setMode(GL_LIGHTING,
        osg::StateAttribute::ON);

    {
        auto river = createTable("assets/river.jpg");
        osg::MatrixTransform* t = new osg::MatrixTransform();
        t->setMatrix(osg::Matrix::scale(40, 11.0, 0.2) *
            osg::Matrix::translate(-20.0, 0.5, 0.0));
        t->addChild(river);
        scn->addChild(t);

        auto grass = createTable("assets/grass.jpg");
        osg::MatrixTransform* g = new osg::MatrixTransform();
        g->setMatrix(osg::Matrix::scale(40, 9.0, 0.2) *
            osg::Matrix::translate(-20.0, -8.5, 0.1));
        g->addChild(grass);
        scn->addChild(g);

        auto* g2 = new osg::MatrixTransform();
        g2->setMatrix(osg::Matrix::scale(40, 20.0, 0.2) *
            osg::Matrix::translate(-20.0, 11.5, 0.1));
        g2->addChild(grass);
        scn->addChild(g2);
    }

    {
        // ZABA
        osg::Node* f = osgDB::readNodeFile("assets/frogge.obj");
        if (f == nullptr) {
            std::fprintf(stderr, "Blad: nie udalo sie wczytac modelu 3D.\n");
            std::exit(EXIT_FAILURE);
        }
        f->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

        auto* frog = new osg::MatrixTransform();
        frog->setMatrix(osg::Matrix::scale(0.2, 0.2, 0.2) *
            osg::Matrix::rotate(osg::DegreesToRadians(-90.0), osg::Vec3(1, 0, 0)) *
            osg::Matrix::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0, 0, 1)) *
            osg::Matrix::translate(0.0, 0.0, 0.4));
        frog->addChild(f);
        scn->addChild(frog);

        // LIŚĆ
        osg::Node* leaf = osgDB::readNodeFile("assets/lily.obj");
        if (leaf == nullptr) {
            std::fprintf(stderr, "Blad: nie udalo sie wczytac modelu 3D.\n");
            std::exit(EXIT_FAILURE);
        }
        leaf->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

        auto* l = new osg::MatrixTransform();
        l->setMatrix(osg::Matrix::scale(0.01, 0.015, 0.01) * osg::Matrix::translate(0.0, 2.0, 1.0));
        l->addChild(leaf);
        scn->addChild(l);

        //Liść 2
        auto* l2 = new osg::MatrixTransform();
        l2->setMatrix(osg::Matrix::scale(0.01, 0.015, 0.01) * osg::Matrix::translate(-4.0, 6.0, 1.0));
        l2->addChild(leaf);
        scn->addChild(l2);
        
        // LIŚĆ 3
        auto* l3 = new osg::MatrixTransform();
        l3->setMatrix(osg::Matrix::scale(0.01, 0.015, 0.01) * osg::Matrix::translate(9.0, 10.0, 1.0));
        l3->addChild(leaf);
        scn->addChild(l3);

        // PALETY
        osg::Node* planks = osgDB::readNodeFile("assets/Old_planks.obj");
        if (planks == nullptr) {
            std::fprintf(stderr, "Blad: nie udalo sie wczytac modelu 3D.\n");
            std::exit(EXIT_FAILURE);
        }
        planks->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

        auto* p = new osg::MatrixTransform();
        p->setMatrix(osg::Matrix::scale(0.015, 0.015, 0.04) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 0, 1)) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
            osg::Matrix::translate(5.0, 8.0, 0.3));
        p->addChild(planks);
        scn->addChild(p);

        // PALETY 2
        auto* p2 = new osg::MatrixTransform();
        p2->setMatrix(osg::Matrix::scale(0.015, 0.015, 0.04) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 0, 1)) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
            osg::Matrix::translate(-8.0, 4.0, 0.3));
        p2->addChild(planks);
        scn->addChild(p2);

        // Kamień
        osg::Node* rock = osgDB::readNodeFile("assets/Rock_1.OBJ");
        if (rock == nullptr) {
            std::fprintf(stderr, "Blad: nie udalo sie wczytac modelu 3D.\n");
            std::exit(EXIT_FAILURE);
        }
        rock->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

        auto* r = new osg::MatrixTransform();
        r->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(12.0, 2.0, 0.0));
        r->addChild(rock);
        scn->addChild(r);

        auto* r2 = new osg::MatrixTransform();
        r2->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(12.0, 4.0, 0.0));
        r2->addChild(rock);
        scn->addChild(r2);

        auto* r3 = new osg::MatrixTransform();
        r3->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(12.0, 6.0, 0.0));
        r3->addChild(rock);
        scn->addChild(r3);

        auto* r4 = new osg::MatrixTransform();
        r4->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(12.0, 8.0, 0.0));
        r4->addChild(rock);
        scn->addChild(r4);

        auto* r5 = new osg::MatrixTransform();
        r5->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(-12.0, 2.0, 0.0));
        r5->addChild(rock);
        scn->addChild(r5);

        auto* r6 = new osg::MatrixTransform();
        r6->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004)* osg::Matrix::translate(-12.0, 4.0, 0.0));
        r6->addChild(rock);
        scn->addChild(r6);

        auto* r7 = new osg::MatrixTransform();
        r7->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(-12.0, 6.0, 0.0));
        r7->addChild(rock);
        scn->addChild(r7);

        auto* r8 = new osg::MatrixTransform();
        r8->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004)* osg::Matrix::translate(-12.0, 8.0, 0.0));
        r8->addChild(rock);
        scn->addChild(r8);

        auto* r9 = new osg::MatrixTransform();
        r9->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(-12.0, 10.0, 0.0));
        r9->addChild(rock);
        scn->addChild(r9);

        auto* r10 = new osg::MatrixTransform();
        r10->setMatrix(osg::Matrix::scale(0.004, 0.004, 0.004) * osg::Matrix::translate(12.0, 10.0, 0.0));
        r10->addChild(rock);
        scn->addChild(r10);


        RiverObject* leaf_obj = new RiverObject(l, 1, 0.5);
        RiverObject* leaf2_obj = new RiverObject(l2, -1, 0.5);
        RiverObject* plank_obj = new RiverObject(p, 1, 2);
        RiverObject* plank2_obj = new RiverObject(p2, -1, 2);
        RiverObject* leaf3_obj = new RiverObject(l3, 1, 0.5);


        osg::ref_ptr<ObjController> controller_l = new ObjController(leaf_obj, "leaf"); //new ObjController(l, 1, "leaf");
        viewer->addEventHandler(controller_l);

        osg::ref_ptr<ObjController> controller_l2 = new ObjController(leaf2_obj, "leaf");
        viewer->addEventHandler(controller_l2);

        osg::ref_ptr<ObjController> controller_p = new ObjController(plank_obj, "planks");
        viewer->addEventHandler(controller_p);

        osg::ref_ptr<ObjController> controller_p2 = new ObjController(plank2_obj, "planks");
        viewer->addEventHandler(controller_p2);

        osg::ref_ptr<ObjController> controller_l3 = new ObjController(leaf3_obj, "leaf");
        viewer->addEventHandler(controller_l3);


        vector<RiverObject*> obj_controllers;
        obj_controllers.push_back(leaf_obj);
        obj_controllers.push_back(plank2_obj);
        obj_controllers.push_back(leaf2_obj);
        obj_controllers.push_back(plank_obj);
        obj_controllers.push_back(leaf3_obj);
        


        //// kontroler
        osg::ref_ptr<ModelController> controller = new ModelController(frog, obj_controllers);
        viewer->addEventHandler(controller);


 /*       osg::Node* light0 = createLightSource(
            0, osg::Vec3(0.0f, -4.0f, 3.0f), osg::Vec4(
                1.0f, 0.0f, 0.0f, 1.0f));
        scn->getOrCreateStateSet()->setMode(GL_LIGHT0,
            osg::StateAttribute::ON);
        scn->addChild(light0);*/
    }

    return scn;
}

int main()
{ 
    osgViewer::Viewer viewer;

    //camera->setViewMatrixAsLookAt(osg::Vec3d(10, 10, 10), osg::Vec3d(0, 10, 0), osg::Vec3d(0, 0, 1));
 
    osg::ref_ptr<TimeManager> tm = new TimeManager(&viewer);
    viewer.addEventHandler(tm);

    osg::ref_ptr<InputManager> im = new InputManager(&viewer);
    viewer.addEventHandler(im);

    auto scn = stworz_scene(&viewer);
    
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.setSceneData(scn);
    auto om = new osgGA::OrbitManipulator();
    viewer.setCameraManipulator(om);

    viewer.getCamera()->setViewMatrixAsLookAt(osg::Vec3d(10, 10, 10), osg::Vec3d(0, 10, 0), osg::Vec3d(0, 0, 1));


    viewer.realize();
    while (!viewer.done()) {
        Input::ClearKeys();
        viewer.frame();
    }
}
