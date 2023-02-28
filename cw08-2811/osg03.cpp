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



osg::Node * stworz_scene()
{
    osg::MatrixTransform * t;

    auto scn = new osg::Group();

    osg::Node * zuzia = osgDB::readNodeFile("bed.obj");
    if (zuzia == nullptr) {
        std::fprintf(stderr, "Błąd: nie udało się wczytać modelu 3D.\n");
        std::exit(EXIT_FAILURE);
    }

    scn->addChild(zuzia);


    osg::Node * car = osgDB::readNodeFile("car.obj");
    if (car == nullptr) {
        std::fprintf(stderr, "Błąd: nie udało się wczytać modelu 3D.\n");
        std::exit(EXIT_FAILURE);
    }
    scn->addChild(car);

    t = new osg::MatrixTransform();
    t->setMatrix(
        osg::Matrix::scale(0.5,0.5,1.0) *
        osg::Matrix::rotate(-0.9, osg::Vec3(3.0, 0.0, 0.0)) *
        // osg::Matrix::rotate(osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(3.0, 0.0, 0.0)) *
        osg::Matrix::translate(300.0, 300.0, 300.0)); 

    t->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);
    t->addChild(car);
    scn->addChild(t);

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
