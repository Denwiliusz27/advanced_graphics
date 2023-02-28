/* ***************************************************************************

Przykład demonstrujący wczytywanie i zapisywanie plików grafiki rastrowej,
oraz możliwość tworzenia podmacierzy współdzielących zaalokowany blok pamięci
z oryginalną macierzą. Proszę przejrzeć ppm03.c aby przypomnieć sobie czym
był stride/step i dlaczego nie zmienia się w podmacierzach.

Testowane z OpenCV 3.2.0 na Debianie 10 i Ubuntu 18.04.

*************************************************************************** */

#include <cstdio>
#include <iostream>

// Aby nie musieć zastanawiać się, które z wielu nagłówków biblioteki OpenCV
// są nam potrzebne, można po prostu włączyć je wszystkie. Kompilacja będzie
// wolniejsza, ale nie jest to dla nas problemem. Biblioteka ułatwia takie
// podejście oferując specjalny plik nagłówkowy włączający wszystkie inne.

#include <opencv2/opencv.hpp>

const char * depth2str(int id)
{
    switch (id) {
        case CV_8U: return "CV_8U";
        case CV_8S: return "CV_8S";
        case CV_16U: return "CV_16U";
        case CV_16S: return "CV_16S";
        case CV_32S: return "CV_32S";
        case CV_32F: return "CV_32F";
        case CV_64F: return "CV_64F";
        default: return "???";
    }
}

void print_mat_info(const cv::Mat & m)
{
    std::cout << "macierz spod adresu " << static_cast<const void *>(&m)
            << " z danymi pod " << static_cast<const void *>(m.data) << '\n'
            << "type: " << m.type()
            << ", czyli depth: " << m.depth()
            << " (" << depth2str(m.depth())
            << ") oraz channels: " << m.channels() << '\n'
            << "dims: " << m.dims << '\n'
            << "rows x cols: " << m.rows << " x " << m.cols << '\n';
    for (int d = 0; d < m.dims; ++d) {
        std::cout << "size[" << d << "]: " << m.size[d]
                << ", step[" << d << "]: " << m.step[d] << '\n';
    }
}

int main(int argc, char * argv[])
{
    const char * const default_names[] = {
        "test_img/lena_gray.png",
        "test_img/lena_color.png",
        nullptr
    };

    const char * const * src_names = default_names;

    if (argc != 1) {
        src_names = argv + 1;
    }

    cv::Mat image;
    char dst_name[16];

    for (int i = 0; src_names[i] != nullptr; ++i) {
        std::cout << "Wczytuję plik " << src_names[i] << "...\n";
        image = cv::imread(src_names[i], cv::IMREAD_ANYCOLOR);
        if (image.empty()) {
            std::cerr << "Błąd przy wczytywaniu pliku.\n";
            continue;
        }

        // Gdyby podać IMREAD_GRAYSCALE, to wczytywane obrazy byłyby
        // konwertowane do odcieni szarości; IMREAD_COLOR to konwersja do
        // trójek BGR (tak! BGR, nie RGB!). IMREAD_ANYCOLOR może zwrócić
        // jedno albo drugie, w zależności od formatu wczytywanego pliku.
        // W każdym przypadku zwrócona macierz będzie miała po 8 bitów
        // na kanał, nawet jeśli w pliku było mniej lub więcej.

        print_mat_info(image);

        // Niby inna macierz, ale odwołująca się do tego samego bloku
        // pamięci co oryginał.
        //
        cv::Mat left_half = image.colRange(0, image.cols / 2);

        print_mat_info(left_half);

        // Odpowiednik matlabowego "LH = bitnot(LH)".
        //
        cv::bitwise_not(left_half, left_half);

        std::sprintf(dst_name, "wynik%02i.png", i);
        if (! cv::imwrite(dst_name, image)) {
            std::cerr << "Błąd przy zapisie do pliku.\n";
        }
    }

    return 0;
}
