CAMERAMAN = imread('test_img/cameraman.png');
MANDRIL = imread('test_img/mandril_gray.png');

[liczba_wierszy, liczba_kolumn, liczba_warstw] = size(CAMERAMAN);

W(:,1:liczba_kolumn/2) = CAMERAMAN(:,1:liczba_kolumn/2);
W(:,(liczba_kolumn/2)+1:liczba_kolumn) = MANDRIL(:,(liczba_kolumn/2)+1:liczba_kolumn);

imwrite(W, 'wynik_zad01.png');

whos