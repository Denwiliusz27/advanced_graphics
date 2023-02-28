/* ***************************************************************************

Program tworzy banalny graf sceny z jednym węzłem geometrii zawierającym dwie
proste bryły, przekazuje go obiektowi klasy Viewer, i uruchamia Viewer. Viewer
automatycznie tworzy kamerę obejmującą wzrokiem całą scenę.

Viewery mają domyślne mechanizmy obsługi myszki -- proszę spróbować ciągnięcia
z wciśniętym którymś z przycisków i zobaczyć co się stanie. Mają też domyślną
obsługę klawiatury -- spacja przywraca początkowe położenie kamery, a Esc
kończy działanie programu.

Viewery domyślnie używają modelu oświetlenia Phonga (włączają GL_LIGHTING)
i konfigurują pierwsze ze świateł (GL_LIGHT0) tak, aby było przymocowane do
kamery. W ten sposób, niezależnie od tego jak myszką obrócimy widok, bryły
na które patrzymy zawsze są oświetlone z przodu.

Uwaga: ten kod używa zwykłych wskaźników zamiast osg::ref_ptr<>. Nie jest to
zalecane, bo w bardziej skomplikowanych programach może być przyczyną wycieków
pamięci.

Zadanie: sprawdź w jaki sposób zorientowane są osie współrzędnych. Jeśli
trzeba dodaj do sceny dodatkowe bryły.

*************************************************************************** */

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

osg::Node * stworz_scene()
{
    osg::Geode * geom_node = new osg::Geode();

    osg::ShapeDrawable * drw = new osg::ShapeDrawable();
    // prostopadłościan: środek i długości boków X, Y, Z
    drw->setShape(new osg::Box(osg::Vec3(-5.0, 0.0, 0.0), 10.0, 5.0, 2.5));
    // kolor czerwony, w pełni nieprzezroczysty
    drw->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    geom_node->addDrawable(drw);

    drw = new osg::ShapeDrawable();
    // sfera: środek i promień
    drw->setShape(new osg::Sphere(osg::Vec3(3.0, 2.0, 0.0), 3.0));
    // zielony, też z współczynnikiem alpha równym 100%
    drw->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));
    geom_node->addDrawable(drw);

    return geom_node;
}

int main(int argc, char * argv[])
{
    // osg::setNotifyLevel(osg::INFO);
    // jeśli chcesz, aby OSG oprócz komunikatów o błędach wyświetlała
    // również zgrubne komunikaty diagnostyczne odkomentuj powyższą linię

    osg::Node * scn = stworz_scene();
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(100, 100, 400, 300);
    viewer.setSceneData(scn);
    return viewer.run();
}