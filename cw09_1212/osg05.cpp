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
#include <osg/Quat>
#include <osg/NodeCallback>
#include <map>
#include <istream>
#include <float.h>
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/AnimationPath>

#define OSG_ANIMATIONPATH 1


osg::AnimationPath* createAnimationPath(){
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode( osg::AnimationPath::LOOP );

    path->insert(0, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 0.0)));
    path->insert(1, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 2.0)));
    path->insert(2, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 4.0)));
    path->insert(3, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 5.0)));
    path->insert(4, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 4.0)));
    path->insert(5, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 2.0)));
    path->insert(0, osg::AnimationPath::ControlPoint(Vec3(0.0, 0.0, 0.0)));

      return path.release();      
}


osg::Node * create_floor()
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

    // tablica z definicjami rysowanych prymitywów
    auto e = new osg::DrawElementsUByte(osg::PrimitiveSet::QUADS);
    // pierwszy czworokąt:
    e->push_back(0);
    e->push_back(1);
    e->push_back(2);
    e->push_back(3);
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

    // tworze teksture
    osg::ref_ptr<osg::Vec2Array> texture_c = new osg::Vec2Array();
    texture_c->push_back(Vec2(0.0, 1.0));
    texture_c->push_back(Vec2(0.0, 0.0));
    texture_c->push_back(Vec2(1.0, 0.0));
    texture_c->push_back(Vec2(1.0, 1.0));

    texture_c->push_back(Vec2(0.0, 1.0));
    texture_c->push_back(Vec2(0.0, 0.0));
    texture_c->push_back(Vec2(1.0, 0.0));
    texture_c->push_back(Vec2(1.0, 1.0));

    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("wood.jpg" );
    osg::ref_ptr<osg::Texture2D> myTex = new osg::Texture2D(image.get());


    auto g = new osg::Geometry();
    g->setVertexArray(v);
    g->setColorBinding(osg::Geometry::BIND_OVERALL);
    g->addPrimitiveSet(e);
    g->setTexCoordArray(0, texture_c.get());
    g->getOrCreateStateSet()->setTextureAttributeAndModes(0, myTex.get());


    // wyłącz oświetlenie Phonga -- kostka będzie jednolicie czerwona
    // niezależnie od tego, z której strony się na nią patrzy
    g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    auto n = new osg::Geode();
    n->addDrawable(g);

    return n;
}


osg::Node * stworz_scene()
{
    osg::MatrixTransform * t;
    osg::AnimationPath * ap;

    auto scn = new osg::Group();

    auto floor = create_floor();

    t = new osg::MatrixTransform();
    t->setMatrix(osg::Matrix::scale(10.0, 3.0, 0.5) *
                    osg::Matrix::translate(-2.0, -1.5, -1.5));
    t->addChild(floor);
    scn->addChild(t);


    osg::Node * zuzia = osgDB::readNodeFile("suzanne.obj");
    if (zuzia == nullptr) {
        std::fprintf(stderr, "Błąd: nie udało się wczytać modelu 3D.\n");
        std::exit(EXIT_FAILURE);
    }
    scn->addChild(zuzia);

    ap = new osg::AnimationPath();
    ap->insert(0.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,0.0)));
    ap->insert(2.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,0.2)));
    ap->insert(3.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,0.4)));
    ap->insert(4.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,0.6)));
    ap->insert(5.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,0.8)));
    ap->insert(6.0, new ANIMATIONPATHControlPoint(new Vec3dReference(0.0,0.0,1.0)));
    ap->setLoopMode(ANIMATIONPATHLoopMode.LOOP);

    PositionAttitudeTransform xform = new PositionAttitudeTransform();
    xform.setUpdateCallback(new AnimationPathCallback(ap));
    xform.addChild(zuzia);


    t = new osg::MatrixTransform();
    t->setMatrix(osg::Matrix::translate(3.0, 0.0, 0.0));
    t->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    t->addChild(zuzia);
    scn->addChild(t);

        t = new osg::MatrixTransform();
    t->setMatrix(osg::Matrix::translate(6.0, 0.0, 0.0));
    t->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    t->addChild(zuzia);
    scn->addChild(t);


    return scn;
}

int main(int argc, char * argv[])
{
    auto scn = stworz_scene();
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(200, 200, 400, 300);
    viewer.setSceneData(scn);
    return viewer.run();
}