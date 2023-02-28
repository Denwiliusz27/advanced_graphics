/* ***************************************************************************

Przykład wyświetlający czerwony sześcian z białymi krawędziami oraz załadowany
z zewnętrznego pliku model małpiej głowy. Korzysta z transformacji układu
współrzędnych. Graf sceny nie jest drzewem: na ekranie widać dwie małpki,
ale w pamięci jest tylko jeden obiekt reprezentujący Zuzannę.

W poprzednim przykładzie używano węzłów ShapeDrawable (matematyczny opis
bryły), tu są węzły Geometry (bryła opisana siatką prymitywów OpenGL).
Proszę zwrócić uwagę, jak wiele linii kodu trzeba napisać aby stworzyć
bardzo prosty model sześcianu. Poproszenie kolegi-grafika, aby nam taki
sześcian wymodelował w 3D Studio zaczyna wtedy wyglądać na dobry pomysł.

Kolega-grafik pewnie nawet przygotowałby nam model zawierający informacje
o kierunku wektorów normalnych dla wierzchołków i/lub ścian. Nasz sześcian
normalnych nie ma, w związku z czym algorytm oświetlenia Phonga nie byłby
w stanie zwrócić dla niego realistycznych wyników.

*************************************************************************** */

#include <cstdio>
#include <cstdlib>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>

osg::Node * czerwona_kostka()
{
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
    c->push_back(Vec4(1.0, 0.0, 0.0, 1.0));

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

    auto g = new osg::Geometry();
    g->setVertexArray(v);
    g->setColorArray(c);
    g->setColorBinding(osg::Geometry::BIND_OVERALL);
    g->addPrimitiveSet(e);
    // wyłącz oświetlenie Phonga -- kostka będzie jednolicie czerwona
    // niezależnie od tego, z której strony się na nią patrzy
    g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    auto n = new osg::Geode();
    n->addDrawable(g);

    // białe krawędzie, rozpięte na punktach troszeczkę na zewnątrz sześcianu
    g = new osg::Geometry();
    v = new osg::Vec3Array();
    v->push_back(Vec3(0.0, 1.001, 1.001));
    v->push_back(Vec3(0.0, 0.0, 1.001));
    v->push_back(Vec3(1.001, 0.0, 1.001));
    v->push_back(Vec3(1.001, 1.001, 1.001));
    v->push_back(Vec3(0.0, 1.001, 0.0));
    v->push_back(Vec3(0.0, 0.0, 0.0));
    v->push_back(Vec3(1.001, 0.0, 0.0));
    v->push_back(Vec3(1.001, 1.001, 0.0));
    g->setVertexArray(v);
    c = new osg::Vec4Array();
    c->push_back(Vec4(1.0, 1.0, 1.0, 1.0));
    g->setColorArray(c);
    g->setColorBinding(osg::Geometry::BIND_OVERALL);
    e = new osg::DrawElementsUByte(osg::PrimitiveSet::LINE_STRIP);
    e->push_back(0);
    e->push_back(1);
    e->push_back(2);
    e->push_back(3);
    e->push_back(0);
    e->push_back(4);
    e->push_back(5);
    e->push_back(6);
    e->push_back(7);
    e->push_back(4);
    g->addPrimitiveSet(e);
    e = new osg::DrawElementsUByte(osg::PrimitiveSet::LINES);
    e->push_back(1);
    e->push_back(5);
    e->push_back(2);
    e->push_back(6);
    e->push_back(3);
    e->push_back(7);
    g->addPrimitiveSet(e);
    g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    n->addDrawable(g);

    return n;
}

osg::Node * stworz_scene()
{
    osg::MatrixTransform * t;

    auto scn = new osg::Group();

    auto kostka = czerwona_kostka();
    t = new osg::MatrixTransform();
    t->setMatrix(osg::Matrix::translate(1.5, 0.0, 0.0));
    t->addChild(kostka);
    scn->addChild(t);

    osg::Node * zuzia = osgDB::readNodeFile("suzanne.obj");
    if (zuzia == nullptr) {
        std::fprintf(stderr, "Błąd: nie udało się wczytać modelu 3D.\n");
        std::exit(EXIT_FAILURE);
    }

    // dla celów dydaktycznych zapiszmy grafową reprezentację właśnie
    // wczytanego modelu do pliku tekstowego
    osgDB::writeNodeFile(*zuzia, "wynik01.osgt");

    scn->addChild(zuzia);

    t = new osg::MatrixTransform();
    t->setMatrix(osg::Matrix::scale(0.5, 0.5, 0.5) *
                    osg::Matrix::translate(0.0, 0.0, 2.0));
    // model małpiej główki ma poprawnie znormalizowane wektory normalne, ale
    // po przeskalowaniu już takie nie będą i algorytm Phonga zwracałby złe
    // wyniki, nakazujemy więc karcie OpenGL dokonywać renormalizacji
    t->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    t->addChild(zuzia);
    scn->addChild(t);

    // zapiszmy jeszcze do pliku całą scenę (też tylko w celach dydaktycznych)
    osgDB::writeNodeFile(*scn, "wynik02.osgt");

    return scn;
}

int main(int argc, char * argv[])
{
    auto scn = stworz_scene();
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100, 400, 300);
    viewer.setSceneData(scn);
    return viewer.run();
}
