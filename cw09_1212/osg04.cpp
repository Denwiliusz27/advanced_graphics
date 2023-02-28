/* ***************************************************************************

Program pozwalający obserwować bryłę z różnych punktów widzenia. Przełączanie
pomiędzy nimi robione jest za pomocą klawiatury (klawisze 1-4 lub A-D).

W kodzie źródłowym połączono ze sobą:
- wczytywanie z pliku wygenerowanej przez inny program sceny;
- alternatywny sposób pozbycia się domyślnego manipulatora;
- obsługę naciskanych przez użytkownika klawiszy;
- zmianę współrzędnych i kierunku patrzenia kamery;
- oszczędzające czas procesora przerysowywanie sceny na żądanie.

*************************************************************************** */

#include <cstdio>
#include <cstdlib>

using std::exit;
using std::fprintf;

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/observer_ptr>
#include <osg/ref_ptr>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

using namespace osg;
using osgDB::readNodeFile;
using osgGA::GUIActionAdapter;
using osgGA::GUIEventAdapter;
using osgGA::GUIEventHandler;
using osgViewer::Viewer;

// Tablica z macierzami opisującymi predefiniowane punkty widzenia. Użytkownik
// będzie mógł się między nimi przełączać przy pomocy klawiatury.
//
const Matrix camera_positions[4] = {
        // Matrix::lookAt(eye, center, up)
        Matrix::lookAt(Vec3(5, 20, 4), Vec3(5, 2.5, 2), Vec3(0, 0, 1)),
        Matrix::lookAt(Vec3(20, 20, 10), Vec3(0, 0, 0), Vec3(0, 0, 1)),
        Matrix::lookAt(Vec3(5, -25, 0), Vec3(5, 2.5, 0), Vec3(0, 0, 1)),
        Matrix::lookAt(Vec3(-20, 2.5, 0), Vec3(5, 2.5, 0), Vec3(0, -1, 1)),
};

class PointOfViewHandler : public GUIEventHandler
{
    observer_ptr<Viewer> _viewer;

public:
    virtual bool handle(const GUIEventAdapter &, GUIActionAdapter &);

    PointOfViewHandler(const ref_ptr<Viewer> & viewer)
    : _viewer(viewer)
    { }

protected:
    virtual ~PointOfViewHandler()
    { }
};

// Metoda obsługująca zdarzenia dostawać będzie wszystkie typy zdarzeń,
// sama musi rozpoznawać te, które ją interesują. Powinna zwrócić "true"
// gdy uda się jej rozpoznać i obsłużyć zdarzenie.
//
bool PointOfViewHandler::handle(
        const GUIEventAdapter & ea, GUIActionAdapter & aa)
{
    if (ea.getEventType() != GUIEventAdapter::KEYDOWN) {
        return false;
    }

    Matrix cam_pos;
    // uwaga: używamy nie getKey(), lecz getUnmodifiedKey()
    switch (ea.getUnmodifiedKey()) {
        case GUIEventAdapter::KEY_1:
        case GUIEventAdapter::KEY_A:
            cam_pos = camera_positions[0];
            break;
        case GUIEventAdapter::KEY_2:
        case GUIEventAdapter::KEY_B:
            cam_pos = camera_positions[1];
            break;
        case GUIEventAdapter::KEY_3:
        case GUIEventAdapter::KEY_C:
            cam_pos = camera_positions[2];
            break;
        case GUIEventAdapter::KEY_4:
        case GUIEventAdapter::KEY_D:
            cam_pos = camera_positions[3];
            break;
        default:
            return false;
    }
    ref_ptr<Viewer> p;
    _viewer.lock(p);
    if (p) {
        p->getCamera()->setViewMatrix(cam_pos);
    }

    // Skoro Viewer jest w trybie ON_DEMAND, to trzeba jawnie żądać
    // przerysowania sceny z nowego punktu widzenia.
    //
    aa.requestRedraw();

    return true;
}

int main(int argc, char * argv[])
{
    ref_ptr<Node> scn = readNodeFile("wynik03.osgb");
    if (!scn) {
        fprintf(stderr, "Błąd: nie udało się wczytać grafu sceny.\n");
        exit(EXIT_FAILURE);
    }

    ref_ptr<Viewer> viewer = new Viewer();
    viewer->setUpViewInWindow(100, 100, 400, 300);
    viewer->setLightingMode(Viewer::NO_LIGHT);
    viewer->getCamera()->setClearColor(Vec4(0, 0, 0, 1));

    // Zaznacz, że nie chcesz aby kamera sama z siebie reagowała na akcje
    // użytkownika. Co za tym idzie, viewer->run() nie stworzy domyślnego
    // manipulatora kamery.
    //
    viewer->getCamera()->setAllowEventFocus(false);

    // Zamiast manipulatora można zainstalować procedurę obsługi zdarzeń
    // opakowaną w obiekt-handler.
    //
    ref_ptr<PointOfViewHandler> povh = new PointOfViewHandler(viewer);
    viewer->addEventHandler(povh.get());

    // Ustaw początkowe położenie kamery (normalnie wylicza je manipulator,
    // ale skoro go nie będzie...).
    //
    viewer->getCamera()->setViewMatrix(camera_positions[0]);

    viewer->setSceneData(scn.get());

    // W tym przykładzie nie ma żadnych animacji, nie ma więc sensu marnować
    // czasu procesora wyświetlając 60 klatek na sekundę. W trybie ON_DEMAND
    // przerysowywanie ekranu robione jest tylko wtedy, gdy zażąda tego
    // system operacyjny lub któraś z naszych procedur obsługi zdarzeń.
    //
    viewer->setRunFrameScheme(Viewer::ON_DEMAND);

    // Wyświetlane przez OSG okno domyślnie nie ma żadnego tytułu. Można jakiś
    // ustawić, ale najpierw to okno musi istnieć. Tworzy je metoda realize(),
    // która jest automatycznie wywoływana przez run(). Metodę tę można
    // ręcznie wywołać, a gdy potem wewnątrz run() zostanie wywołana po raz
    // drugi, zauważy że okno już istnieje i zachowa się jak operacja pusta.
    //
    viewer->realize();

    // Viewer może mieć więcej niż jedno okno, dlatego zamiast getWindow()
    // jest getWindows(), zwracająca wektor wypełniony wskaźnikami do okien.
    //
    Viewer::Windows w;
    viewer->getWindows(w);
    for (unsigned int i = 0; i < w.size(); ++i) {
        w[i]->setWindowName("Okno OSG");
    }

    return viewer->run();
}
