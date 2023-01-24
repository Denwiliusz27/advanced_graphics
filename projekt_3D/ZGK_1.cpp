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


class RiverObject {
public:
    RiverObject(osg::MatrixTransform* node, int direction, double size)
        : _node(node), _direction(new int(direction)), size(size) {}

    osg::MatrixTransform* getMatrix();
    int* getDirection();
    double getSize();

private:
    osg::ref_ptr<osg::MatrixTransform> _node;
    int* _direction;
    double size;
};

osg::MatrixTransform* RiverObject::getMatrix() {
    return _node;
}

int* RiverObject::getDirection() {
    return _direction;
}


double RiverObject::getSize() {
    return size;
}





class ModelController : public osgGA::GUIEventHandler
{
public:
    ModelController(osg::MatrixTransform* node, vector<osg::MatrixTransform*> matrixes)
        : _model(node), obj_matrixes(matrixes) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) override;
protected:
    osg::ref_ptr<osg::MatrixTransform> _model;
    vector<osg::MatrixTransform*> obj_matrixes;
};


bool ModelController::handle(const osgGA::GUIEventAdapter& ea,
    osgGA::GUIActionAdapter& aa)
{
    if (!_model) return false;
    osg::Matrix frog_matrix = _model->getMatrix();
    osg::Vec3 frog_pos = frog_matrix.getTrans();
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        switch (ea.getKey()) {
        case 'a': case 'A':
            if (frog_pos.x() <= 10.0 && frog_pos.x() >= -8.0) {
                frog_matrix *= osg::Matrix::translate(-2.0, 0.0, 0.0);
            }
            break;
        case 'd': case 'D':
            if (frog_pos.x() <= 8.0 && frog_pos.x() >= -10.0) {
                frog_matrix *= osg::Matrix::translate(2.0, 0.0, 0.0);
            }
            break;
        case 'w': case 'W':
            frog_matrix *= osg::Matrix::translate(0.0, 2.0, 0.0);
            break;
        default:
            break;
        }
        _model->setMatrix(frog_matrix);
        break;
    default:
        break;
    }

    cout << "FROGG: (" << frog_pos.x() << ", " << frog_pos.y() << ", " << frog_pos.z() << ")" << endl;

    for (int i = 0; i < obj_matrixes.size(); i++) {
        osg::Matrix obj_matrix = obj_matrixes[i]->getMatrix();
        osg::Vec3d obj_pos = obj_matrix.getTrans();


        if ((frog_pos.y() == obj_pos.y()) && (frog_pos.x() >= obj_pos.x()-0.5) && (frog_pos.x() <= (obj_pos.x()+0.5))) {
            frog_pos.x() += 0.1;
            _model->setMatrix(
                osg::Matrixd::scale(_model->getMatrix().getScale()) *
                osg::Matrixd::rotate(osg::DegreesToRadians(-90.0), osg::Vec3(1, 0, 0)) *
                osg::Matrixd::rotate(osg::DegreesToRadians(180.0), osg::Vec3(0, 0, 1)) *
                osg::Matrixd::translate(frog_pos)
            );
        }


        cout << "OBJ: (" << obj_pos.x() << ", " << obj_pos.y() << ", " << obj_pos.z() << ")" << endl;
    }


    //osg::Vec3d position = matrix.getTrans();
    //cout << "FROGG: (" << position.x() << ", " << position.y() << ", " << position.z() << ")" << endl;

    return false;
}




class ObjController : public osgGA::GUIEventHandler
{
public:
    ObjController(osg::MatrixTransform* node, int k, string type)
        : _model(node), k(k), type(type) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) override;
protected:
    osg::ref_ptr<osg::MatrixTransform> _model;
    int k;
    string type;
};


bool ObjController::handle(const osgGA::GUIEventAdapter& ea,
    osgGA::GUIActionAdapter& aa) {
    
    if (ea.getEventType() != osgGA::GUIEventAdapter::FRAME) return false;

    double deltaTime = Time::GetDeltaTime();
    osg::Matrix matrix = _model->getMatrix();
    osg::Vec3 position = matrix.getTrans();


    if (type == "leaf") {
        if (position.x() < -10.0) {
            position.x() = -10.0;
            k = 1;
        }
        else if (position.x() > 10.0) {
            position.x() = 10.0;
            k = -1;
        }
    }
    else {
        if (position.x() < -9.0) {
            position.x() = -9.0;
            k = 1;
        }
        else if (position.x() > 9.0) {
            position.x() = 9.0;
            k = -1;
        }
    }
     
    position.x() += k * 0.1; //Time::GetTime() * deltaTime * 0.3 * k;

    if (type == "leaf") {
        _model->setMatrix(
            osg::Matrixd::scale(_model->getMatrix().getScale()) *
            osg::Matrixd::rotate(_model->getMatrix().getRotate()) *
            osg::Matrixd::translate(position)
        );
    }
    else if (type == "planks") {
        _model->setMatrix(
            osg::Matrix::scale(0.015, 0.015, 0.04) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 0, 1)) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
            osg::Matrixd::translate(position)
        );
    }
    
    //cout << "pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")" << endl;
   
    return false;
}




osg::Node* createTable()
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

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("assets/river.jpg");
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
        auto river = createTable();
        osg::MatrixTransform* t = new osg::MatrixTransform();
        t->setMatrix(osg::Matrix::scale(40, 9.0, 0.2) *
            osg::Matrix::translate(-20.0, 0.5, 0.0));
        t->addChild(river);
        scn->addChild(t);
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


        osg::ref_ptr<ObjController> controller_l = new ObjController(l, 1, "leaf");
        viewer->addEventHandler(controller_l);

        osg::ref_ptr<ObjController> controller_l2 = new ObjController(l2, -1, "leaf");
        viewer->addEventHandler(controller_l2);

        osg::ref_ptr<ObjController> controller_p = new ObjController(p, -1, "planks");
        viewer->addEventHandler(controller_p);

        osg::ref_ptr<ObjController> controller_p2 = new ObjController(p2, 1, "planks");
        viewer->addEventHandler(controller_p2);


        RiverObject leaf_obj(l ,1, 1);
        RiverObject leaf2_obj(l2 ,1, 1);
        RiverObject plank_obj(p ,1, 1);
        RiverObject plank2_obj(p ,1, 1);

        vector<osg::MatrixTransform*> obj_controllers;
        obj_controllers.push_back(l);
        obj_controllers.push_back(p2);
        obj_controllers.push_back(l2);
        obj_controllers.push_back(p);
        


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
 
    osg::ref_ptr<TimeManager> tm = new TimeManager(&viewer);
    viewer.addEventHandler(tm);

    osg::ref_ptr<InputManager> im = new InputManager(&viewer);
    viewer.addEventHandler(im);

    auto scn = stworz_scene(&viewer);
    
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.setSceneData(scn);
    auto om = new osgGA::OrbitManipulator();
    viewer.setCameraManipulator(om);

    viewer.realize();
    while (!viewer.done()) {
        Input::ClearKeys();
        viewer.frame();
    }
}
