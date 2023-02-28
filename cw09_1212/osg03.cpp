/* ***************************************************************************

Program demonstrujący jak zaaranżować oświetlenie sceny. Proszę pamiętać,
że OSG domyślnie sama dodaje do sceny źródło GL_LIGHT0. Jeśli chcemy w pełni
panować nad oświetleniem sceny, to tę automatykę trzeba wyłączyć.

Ustawienia źródeł światła są w OpenGL częścią stanu karty graficznej. OSG nie
obsługuje ich jednak tak jak innych trybów i atrybutów będących częścią stanu,
czyli używając obiektów StateSet stowarzyszonych z węzłami grafu sceny. Robi
to trochę inaczej, używając dodatkowych obiektów typu Light i LightSource.

Klasa Light jest pojemnikiem na atrybuty określające parametry danego źródła
światła: numer, kolor, rodzaj (kierunkowe, punktowe, spotlight), położenie
itd. Standard OpenGL wymaga, aby karty obsługiwały co najmniej osiem źródeł,
numerowanych od 0 do 7.

LightSource dziedziczy po klasie Node. Z każdym obiektem LightSource związany
jest obiekt typu Light. Zawarte w nim położenie źródła jest modyfikowane
przez węzły transformacji leżące na ścieżce od węzła LightSource do korzenia
sceny. Pozwala to łatwo modelować źródła światła związane z poruszającymi się
bryłami, np. reflektory aut. Niniejszy program nie korzysta z tej możliwości,
węzły LightSource są podpięte bezpośrednio do korzenia sceny.

Samo wstawienie węzłów LightSource do grafu sceny nie wystarcza. Trzeba też
włączyć bryłom tryby "używaj modelu oświetlenia Phonga" i "uwzględniaj źródło
światła nr X" dla wszystkich używanych przez nas źródeł. Najprościej ustawić
to w StateSet związanym z korzeniem sceny, bo te ustawienia obowiązują wtedy
dla wszystkich węzłów w grafie.

Ten przykład używa dwóch źródeł. Pierwsze jest białym źródłem kierunkowym,
drugie zielonym źródłem punktowym. Oba generują też słabe białe światło
otoczenia, bo konstruktor klasy Light przyjmuje (0.05, 0.05, 0.05) jako
domyślną wartość atrybutu "ambient", a poniższy kod jej nie nadpisuje.

W poprzednich programach początkowe położenie kamery było automatycznie
wyliczane przez domyślny manipulator. W tym programie niestety kończy się
to ustawieniem jej w punkcie po zacienionej stronie bryły. Ustawiamy więc
to położenie ręcznie, tak aby kamera była w bardziej odpowiednim miejscu.

Uwaga: od tego momentu będziemy używać osg::ref_ptr<>. Jest to zalecane, bo
broni nas przed wyciekaniem pamięci, warto więc się przyzwyczaić do takiego
stylu pisania programów.

*************************************************************************** */

#include <cstdio>
#include <cstdlib>

using std::exit;
using std::fprintf;

#include <osg/Geode>
#include <osg/Group>
#include <osg/LightSource>
#include <osg/ShapeDrawable>
#include <osg/ref_ptr>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

using namespace osg;
using osgDB::writeNodeFile;
using osgGA::TrackballManipulator;
using osgViewer::Viewer;

Node * stworz_scene()
{
    ref_ptr<Group> root_group = new Group();
    StateSet * root_ss = root_group->getOrCreateStateSet();
    root_ss->setMode(GL_LIGHTING, StateAttribute::ON);
    root_ss->setMode(GL_LIGHT0, StateAttribute::ON);
    root_ss->setMode(GL_LIGHT1, StateAttribute::ON);

    // światło nr 0 (w=0.0, jest więc w nieskończoności)

    ref_ptr<LightSource> lt_src = new LightSource();
    Light * lt = lt_src->getLight();
    lt->setLightNum(0);
    lt->setPosition(Vec4(-100.0, 100.0, 50.0, 0.0));

    root_group->addChild(lt_src.get());

    // światło nr 1

    lt_src = new LightSource();
    lt = lt_src->getLight();
    lt->setLightNum(1);
    lt->setPosition(Vec4(10.0, 5.0, 4.0, 1.0));
    lt->setDiffuse(Vec4(0.25, 1.0, 0.25, 1.0));
    lt->setSpecular(Vec4(0.25, 1.0, 0.25, 1.0));
    lt->setQuadraticAttenuation(0.2);

    root_group->addChild(lt_src.get());

    // bryła złożona z prostopadłościennego fundamentu i trzech kolumn
    // nie określono koloru/materiału, OSG użyje więc domyślnych parametrów

    ref_ptr<Geode> geom_node = new Geode();

    ref_ptr<ShapeDrawable> drw = new ShapeDrawable();
    drw->setShape(new Box(Vec3(5.0, 2.5, 0.0), 10.0, 5.0, 0.2));
    geom_node->addDrawable(drw.get());

    for (int i = 1; i <= 3; ++i) {
        drw = new ShapeDrawable();
        drw->setShape(new Cylinder(Vec3(2.5 * i, 2.5, 2.0), 0.5, 4.0));
        geom_node->addDrawable(drw.get());
    }

    root_group->addChild(geom_node.get());

    return root_group.release();
}

int main(int argc, char * argv[])
{
    ref_ptr<Node> scn = stworz_scene();

    // graf sceny zapisujemy do pliku, użyjemy go w następnym programie
    bool success = writeNodeFile(*scn, "wynik03.osgb");
    if (!success) {
        fprintf(stderr, "Błąd: nie udało się zapisać grafu sceny.\n");
        exit(EXIT_FAILURE);
    }

    ref_ptr<Viewer> viewer = new Viewer();
    viewer->setUpViewInWindow(100, 100, 400, 300);

    // nie dodawaj automatycznie oświetlenia sceny
    viewer->setLightingMode(Viewer::NO_LIGHT);

    // czarne tło sceny zamiast domyślnego ciemnoniebieskiego
    viewer->getCamera()->setClearColor(Vec4(0.0, 0.0, 0.0, 1.0));

    // zamiast pozwalać na użycie domyślnego manipulatora można dostarczyć
    // swój własny; niech to też będzie TrackballManipulator, ale z ręcznie
    // ustawioną pozycję startową
    ref_ptr<TrackballManipulator> m = new TrackballManipulator();
    m->setHomePosition(
            Vec3(5.0, 20.0, 4.0), // eye
            Vec3(5.0, 2.5, 2.0),  // center
            Vec3(0, 0, 1)         // up
    );
    viewer->setCameraManipulator(m.get());

    viewer->setSceneData(scn.get());
    return viewer->run();
}
