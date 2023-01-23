
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

USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(obj)
USE_GRAPHICSWINDOW()

using namespace std;

class ModelController : public osgGA::GUIEventHandler
{
public:
    ModelController(osg::MatrixTransform* node)
        : _model(node) {}

    virtual bool handle(const osgGA::GUIEventAdapter& ea,
        osgGA::GUIActionAdapter& aa) override;
protected:
    osg::ref_ptr<osg::MatrixTransform> _model;
    osg::ref_ptr<osg::AnimationPathCallback> ap_leaf1;
    osg::ref_ptr<osg::AnimationPathCallback> ap_tree1;
    osg::ref_ptr<osg::AnimationPathCallback> ap_leaf2;
    osg::ref_ptr<osg::AnimationPathCallback> ap_tree2;
};


bool ModelController::handle(const osgGA::GUIEventAdapter& ea,
    osgGA::GUIActionAdapter& aa)
{
    if (!_model) return false;
    osg::Matrix matrix = _model->getMatrix();
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        switch (ea.getKey()) {
        case 'a': case 'A':
            matrix *= osg::Matrix::translate(-2.0, 0.0, 0.0);
            break;
        case 'd': case 'D':
            matrix *= osg::Matrix::translate(2.0, 0.0, 0.0);
            break;
        case 'w': case 'W':
            matrix *= osg::Matrix::translate(0.0, 2.0, 0.0);
            break;
        default:
            break;
        }
        _model->setMatrix(matrix);
        break;
    default:
        break;
    }

    osg::Vec3d position = matrix.getTrans();
    cout << "pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")" << endl;

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
    //osgGA::GUIEventHandler::handle();
    //viewer->getCamera->getV

    viewer->getCamera()->getOrCreateStateSet()->setMode(GL_LIGHTING,
        osg::StateAttribute::ON);

    {
        auto river = createTable();
        osg::MatrixTransform* t = new osg::MatrixTransform();
        t->setMatrix(osg::Matrix::scale(24, 9.0, 0.2) *
            osg::Matrix::translate(-12.0, 0.0, 0.0));
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
        //scn->addChild(l);

        // PIEŃ
        osg::Node* tree = osgDB::readNodeFile("assets/Tronco_low.obj");
        if (tree == nullptr) {
            std::fprintf(stderr, "Blad: nie udalo sie wczytac modelu 3D.\n");
            std::exit(EXIT_FAILURE);
        }
        tree->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

        auto* t = new osg::MatrixTransform();
        t->setMatrix(osg::Matrix::scale(0.5, 0.5, 2.0) *
            osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
            osg::Matrix::translate(5.8, 5.8, 1.7));
        t->addChild(tree);
        //scn->addChild(t);


        // liść 1
        auto* l1 = new osg::MatrixTransform();
        osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
        path->setLoopMode(osg::AnimationPath::LOOP);
        path->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3(-10.0, 0.0, 0.0)));
        path->insert(3, osg::AnimationPath::ControlPoint(osg::Vec3(10.0, 0.0, 0.0)));
        path->insert(6, osg::AnimationPath::ControlPoint(osg::Vec3(-10.0, 0.0, 0.0)));

        osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
        apcb->setAnimationPath(path);
        l1->setUpdateCallback(apcb);

        //t3->setMatrix(osg::Matrix::scale(0.5, 0.5, 2.0) *
        //    osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0)) *
        //    osg::Matrix::translate(0.0, 0.0, 0.0));
        l1->addChild(l);
        scn->addChild(l1);

        // drzewo 1
        auto* t1 = new osg::MatrixTransform();
        osg::ref_ptr<osg::AnimationPath> path2 = new osg::AnimationPath;
        path2->setLoopMode(osg::AnimationPath::LOOP);
        path2->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3(8.0, 0.0, 0.0)));
        path2->insert(3, osg::AnimationPath::ControlPoint(osg::Vec3(-8.0, 0.0, 0.0)));
        path2->insert(6, osg::AnimationPath::ControlPoint(osg::Vec3(8.0, 0.0, 0.0)));

        osg::ref_ptr<osg::AnimationPathCallback> apcb2 = new osg::AnimationPathCallback;
        apcb2->setAnimationPath(path2);
        t1->setUpdateCallback(apcb2);
        t1->addChild(t);
        scn->addChild(t1);


        // liść 2
        auto* l2 = new osg::MatrixTransform();
        osg::ref_ptr<osg::AnimationPath> path3 = new osg::AnimationPath;
        path3->setLoopMode(osg::AnimationPath::LOOP);
        path3->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3(-4.0, 4.0, 0.0)));
        path3->insert(3, osg::AnimationPath::ControlPoint(osg::Vec3(10.0, 4.0, 0.0)));
        path3->insert(6.75, osg::AnimationPath::ControlPoint(osg::Vec3(-10.0, 4.0, 0.0)));
        path3->insert(7.875, osg::AnimationPath::ControlPoint(osg::Vec3(-4.0, 4.0, 0.0)));

        osg::ref_ptr<osg::AnimationPathCallback> apcb3 = new osg::AnimationPathCallback;
        apcb3->setAnimationPath(path3);
        l2->setUpdateCallback(apcb3);

        //l2->setMatrix(osg::Matrix::translate(0.0, 8.0, 1.0));
        l2->addChild(l);
        scn->addChild(l2);


        // drzewo 2
        auto* t2 = new osg::MatrixTransform();
        osg::ref_ptr<osg::AnimationPath> path4 = new osg::AnimationPath;
        path4->setLoopMode(osg::AnimationPath::LOOP);
        path4->insert(0, osg::AnimationPath::ControlPoint(osg::Vec3(10.0, 4.0, 0.0)));
        path4->insert(2.5, osg::AnimationPath::ControlPoint(osg::Vec3(-8.0, 4.0, 0.0)));
        path4->insert(5, osg::AnimationPath::ControlPoint(osg::Vec3(10.0, 4.0, 0.0)));

        osg::ref_ptr<osg::AnimationPathCallback> apcb4 = new osg::AnimationPathCallback;
        apcb4->setAnimationPath(path4);
        t2->setUpdateCallback(apcb4);
        t2->addChild(t);
        scn->addChild(t2);


        double time = 2.0;
        osg::Matrix temp_matrix;
        path4->getMatrix(time, temp_matrix);     //getMatrix(time);
        osg::Vec3 temp_position = temp_matrix.getTrans();
        std::cout << "Position at time " << time << ": (" << temp_position.x() << ", " << temp_position.y() << ", " << temp_position.z() << ")" << std::endl;



        osg::Matrix matrix = frog->getMatrix();
        osg::Vec3d position = matrix.getTrans();
        cout << "pos: (" << position.x() << ", " << position.y() << ", " << position.z() << ")" << endl;


        // kontroler
        osg::ref_ptr<ModelController> controller = new ModelController(frog);
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
    auto scn = stworz_scene(&viewer);
    viewer.setUpViewInWindow(100, 100, 800, 600);
    viewer.realize();
    viewer.setSceneData(scn);

    return viewer.run();
}
