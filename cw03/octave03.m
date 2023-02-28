% Operacje na obrazach rastrowych w MATLAB-ie / Octave'ie.
% Testowane pod Octave 4.2.2 na Ubuntu 18.04 i 4.4.1 na Debianie 10.

IMG_RGB = imread('test_img/lena_color.png');
IMG_GRAY = imread('test_img/cameraman.png');

disp('Ładowanie biblioteki funkcji dla obrazów rastrowych');

pkg load image;
pkg list;       % przy nazwie uaktywnionej biblioteki pojawia się gwiazdka

disp('Macierze klasy "logical" jako maski');

% Zadanie: znaleźć żółtawe (a nie tylko czysto żółte) fragmenty obrazu.
%
% Trzeba zdefiniować, co oznacza "żółtawy piksel". Można to w miarę wygodnie
% zrobić w przestrzeni HSV, bo tam tylko kanał H decyduje z jakim odcieniem
% mamy do czynienia.

IMG_HSV = rgb2hsv(IMG_RGB);     % konwersja między przestrzeniami barw
HUE = IMG_HSV(:,:,1);           % rozdzielenie kanałów H, S, V
SAT = IMG_HSV(:,:,2);
VAL = IMG_HSV(:,:,3);

% Funkcja rgb2hsv zawsze zwraca macierz liczb double z przedziału [0.0, 1.0],
% nawet jeśli argument był macierzą bajtów. Czysty żółty ma hue równe 0.16(6).
% Przyjmijmy, że "żółtawy" oznacza "hue z przedziału od 0.07 do 0.26".

MASK = (HUE > 0.07) & (HUE < 0.26);

whos IMG_RGB IMG_HSV HUE SAT VAL MASK;

% W macierzy MASK mamy wyniki przeprowadzonej analizy -- jedynki tam, gdzie
% są żółtawe piksele, zera wszędzie indziej.
%
% Można taką maskę interpretować jako czarno-biały obraz. Pozwala to jednym
% rzutem oka ocenić, czy uzyskana maska sensownie wygląda.

imwrite(MASK, 'wynik01.png');

% Masek zazwyczaj używamy do określenia, które piksele oryginalnego obrazu
% należy w jakiś sposób przetworzyć. Powiedzmy, że chcemy wszystkie żółtawe
% piksele zamienić na jednolicie turkusowe. Przyjmijmy, że kolor turkusowy
% to trójka RGB (50, 200, 200).

W = IMG_RGB;
for y = 1:rows(W)
  for x = 1:columns(W)
    if MASK(y,x)
      W(y,x,1) = 50;
      W(y,x,2) = 200;
      W(y,x,3) = 200;
    else
      % nic nie rób i zostaw W(y,x) niezmienione
    endif
  end
end

imwrite(W, 'wynik02.png');

% Zamiast pisać pętle lepiej użyć operacji macierzowych i wykorzystać to, że
% przemnożenie jakiejś wartości przez element z maski albo jej nie zmienia
% (gdy w masce była jedynka), albo daje nam zero.

W = IMG_RGB;
W(:,:,1) = MASK .* 50 + not(MASK) .* W(:,:,1);
W(:,:,2) = MASK .* 200 + not(MASK) .* W(:,:,2);
W(:,:,3) = MASK .* 200 + not(MASK) .* W(:,:,3);

imwrite(W, 'wynik03.png');

% To samo co powyżej, ale bez początkowego kopiowania całej IMG_RGB do W
% i z trochę inaczej zapisanymi podwyrażeniami. Ciężko powiedzieć czy będzie
% się szybciej wykonywało, nie wiadomo jak Octave będzie to obliczać.

W = cat(3, ~MASK .* IMG_RGB(:,:,1) + 50 * MASK,
              ~MASK .* IMG_RGB(:,:,2) + 200 * MASK,
                ~MASK .* IMG_RGB(:,:,3) + 200 * MASK);

imwrite(W, 'wynik04.png');

% Maski można użyć do tzw. logicznego indeksowania macierzy. Jeśli mam
% dwuwymiarowe macierze A i B oraz maskę M, to mogę napisać "A(M) = skalar"
% albo "A(M) = B(M)" i nowe wartości zostaną podstawione tylko do tych
% elementów macierzy A, którym odpowiadają jedynki w masce.

R = IMG_RGB(:,:,1);
G = IMG_RGB(:,:,2);
B = IMG_RGB(:,:,3);
R(MASK) = 50;
G(MASK) = 200;
B(MASK) = 200;
W = cat(3, R, G, B);

imwrite(W, 'wynik05.png');

% Jeszcze innym podejściem jest użycie tzw. liniowego adresowania macierzy.
% Nawet jeśli macierz ma dwa lub trzy wymiary, to podaje się tylko jeden
% indeks. W miarę wzrastania tego indeksu przesuwamy się w dół przez wartości
% z kolejnych wierszy, gdy już nie da się pójść niżej przechodzimy na górę
% kolumny po prawej stronie, a gdy i kolumny nam się skończą przechodzimy
% w trzecim wymiarze do następnego płata macierzy (czyli do kolejnego kanału
% obrazu rastrowego).

channel_stride = rows(IMG_RGB) * columns(IMG_RGB);
W = IMG_RGB;
W(find(MASK)) = 50;
W(find(MASK) + channel_stride) = 200;
W(find(MASK) + 2*channel_stride) = 200;

imwrite(W, 'wynik06.png');

% Na koniec coś z trochę innej beczki. Gdyby zamiast zamalowywania żółtawych
% obszarów turkusowym kolorem należało te miejsca wyszarzyć, czyli zamienić
% piksele na szare ale z zachowaniem oryginalnej jasności, to jak najłatwiej
% to zrobić?
%
% Skoro mamy już obraz przetworzony do HSV i rozdzielony na dwuwymiarowe
% macierze odpowiadające kanałom, to może wystarczy wyzerować saturację,
% aby osiągnąć takie "zrobienie na szaro" o jakie chodzi?

SAT .*= not(MASK);   % czyli SAT = SAT .* not(MASK)
W_HSV = cat(3, HUE, SAT, VAL);
W = hsv2rgb(W_HSV);

imwrite(W, 'wynik07.png');

clear -exclusive IMG_RGB IMG_GRAY;
whos;

input('Naciśnij Enter: ', 's');

disp('Skalowanie obrazu');

% Funkcji imresize jako drugi argument można podać współczynnik skalowania
% (oba wymiary zostaną przez niego przemnożone)...

W = imresize(IMG_RGB, 0.5);
imwrite(W, 'wynik11.png');

% ... albo dwuelementową macierz mówiącą, do jakiej liczby wierszy i kolumn
% raster przeskalować (uwaga: wierszy i kolumn, czyli wysokość x szerokość,
% a nie jak jesteśmy do tego przyzwyczajeni szerokość x wysokość).

W = imresize(IMG_RGB, [100 200]);
imwrite(W, 'wynik12.png');

% Zamiast jednej z tych liczb można podać NaN. Octave wyliczy brakującą
% liczbę tak, aby zachować proporcje obrazu.
%
% Opcjonalnym trzecim argumentem jest metoda interpolacji: 'nearest',
% 'bilinear' lub 'bicubic' (domyślna).

W = imresize(IMG_RGB, [NaN 2048]);
imwrite(W, 'wynik13.png');

W = imresize(IMG_RGB, [2048 NaN], 'nearest');   % zły wynik - Octave ma buga!
imwrite(W, 'wynik14.png');

% Wszystkie powyższe informacje o tym, jak korzystać z imresize() są dostępne
% w systemie pomocy Octave'a, wystarczy użyć "help imresize" i ewentualnie
% jeszcze "help interp2".
%
% Niestety, nie ma tam informacji o bugu ujawniającym się, gdy próbujemy
% skalować obraz RGB używając metody "nearest" a współczynniki skalowania
% są liczbami naturalnymi lub ich odwrotnościami.  :-(

disp('Rotacja obrazu');

% Obrót o 30 stopni w lewo, czyli counter-clockwise.

W = imrotate(IMG_RGB, 30);
whos IMG_RGB W;                 % oryginalny obraz 512x512, wynikowy 699x699
imwrite(W, 'wynik15.png');

% Albo o 30 stopni w prawo i z przycięciem wyniku do rozmiarów oryginalnego
% obrazu. Aby móc podać 'crop' jako czwarty argument trzeba też podać trzeci,
% czyli metodę interpolacji używaną przy obrocie:

W = imrotate(IMG_RGB, -30, 'bilinear', 'crop');
imwrite(W, 'wynik16.png');

% Czy przy poprzednim obrocie w lewo też była używana biliniowa? Jak to
% byście Państwo sprawdzili?

disp('Ogólne konwolucje macierzy - argumenty');

% Przykładowe macierz danych 5x5 oraz macierz współczynników 3x3:

A = [ 11 12 13 14 15 ; 21 22 23 24 25 ; 31 32 33 34 35 ; 41 42 43 44 45 ]
F = [ 0.2 0.2 0.0 ; 0.1 0.3 0.1 ; 0.0 0.0 0.1 ]

input('Naciśnij Enter: ', 's');

% Octave ma conv() obliczającą konwolucję wektorów, conv2() dla macierzy
% dwuwymiarowych, i convn() dla macierzy o dowolnej liczbie wymiarów.

disp('Ogólne konwolucje macierzy - wyniki');

W1 = conv2(A, F)
W2 = conv2(F, A)
W3 = convn(A, F);
W4 = convn(F, A);

whos A F W?;

input('Naciśnij Enter: ', 's');

% Na oko wszystkie powyższe wyniki są identyczne (i takie powinny być), ale
% przy obliczeniach na komputerze wkradają się błędy zaokrągleń.

isequal(W1, W2)
abs(W1 - W2)
isequal(W1, W3)
isequal(W2, W4)

input('Naciśnij Enter: ', 's');

disp('Ogólne konwolucje macierzy gdy jedna jest klasy "uint8"');

B = uint8(A);

W5 = conv2(B, F);
W6 = conv2(F, B);
W7 = convn(B, F);
W8 = convn(F, B);

isequal(W1, W5)
isequal(W2, W6)
isequal(W3, W7)
isequal(W4, W8)

clear W?;

input('Naciśnij Enter: ', 's');

disp('Konwolucje obrazów rastrowych');

% Funkcji conv2 można podać dodatkowy argument nakazujący przycięcie wyniku.
% Nam przyda się 'same', przycinające wynik do rozmiarów pierwszego argumentu.

W1 = conv2(A, F, 'same')

% Podobnie obcina wynik funkcja signal2, implementująca dwuwymiarowy filtr
% o skończonej odpowiedzi impulsowej (inżynierowie wiedzą, co to znaczy).
% Z historycznych powodów macierz filtru jest obracana o 180 stopni, czyli
% formalnie mamy tu do czynienia z liczeniem korelacji, a nie konwolucji:
%   signal2(F, A) <=> conv2(A, rot90(rot90(F)), 'same')
% Nie jesteśmy inżynierami, więc nie będziemy z tej funkcji korzystać.

% Do przetwarzania obrazów przeznaczona jest funkcja imfilter. Jako pierwszy
% arg. bierze obraz (dwu- lub trójwymiarową macierz), jako drugi dwuwymiarową
% macierz-filtr. Wynikowa macierz jest przycinana tak jak dla 'same'.

W2 = imfilter(A, F)

abs(W2 - W1)    % max = 10.4, zupełnie różne wyniki

input('Naciśnij Enter: ', 's');

% imfilter() domyślnie używa przy filtrowaniu wzoru na korelację, a nie
% konwolucję. Ma to znaczenie jeśli macierz filtru nie jest symetryczna
% względem obrotu o 180°.

W3 = conv2(A, rot90(rot90(F)), 'same');

abs(W2 - W3)    % max = 7.1054e-15

W4 = imfilter(A, F, 'corr');
W5 = imfilter(A, F, 'conv');

abs(W2 - W4)    % max = 0
abs(W1 - W5)    % max = 7.1054e-15

clear W?;

input('Naciśnij Enter: ', 's');

% Miłą cechą imfilter() jest to, że wynik ma ten sam typ jak obraz na wejściu.

W = imfilter(B, F);

imwrite(W, 'wynik21.png');

whos B F W;

% Można też kazać uzupełniać obraz czymś innym niż zerami:

W = imfilter(B, F, uint8(255));

imwrite(W, 'wynik22.png');

W = imfilter(B, F, 'replicate');

imwrite(W, 'wynik23.png');

W = imfilter(IMG_RGB, F, 'replicate');

imwrite(W, 'wynik24.png');

input('Naciśnij Enter: ', 's');

disp('Predefiniowane filtry');

% Rozmywanie obrazu:

fspecial('average')

fspecial('average', 7)

fspecial('disk', 3)

input('Naciśnij Enter: ', 's');

fspecial('gaussian')

fspecial('gaussian', 5)

fspecial('gaussian', 5, 0.8)

input('Naciśnij Enter: ', 's');

% Wykrywanie krawędzi:

F = fspecial('prewitt')

F'      % F' czyli ctranspose(F), sprzężenie hermitowskie macierzy
        % transpose(F) dałoby ten sam wynik, bo w F nie ma liczb zespolonych

fspecial('sobel')

fspecial('sobel')'

input('Naciśnij Enter: ', 's');

disp('Praktyczne zastosowania standardowych filtrów');

W = imfilter(IMG_RGB, fspecial('average'), 'replicate');

imwrite(W, 'wynik31.png');

W = imfilter(IMG_RGB, fspecial('disk', 5), 'replicate');

imwrite(W, 'wynik32.png');

% Filtr wyostrzający zawiera wartości ujemne, może zwrócić wyniki spoza
% dopuszczalnego zakresu. Lepiej przejść na [0, 1] i potem samemu przyciąć.

I = double(IMG_RGB) / 255;
W = imfilter(I, fspecial('unsharp'), 'replicate');
W = min(max(W, 0), 1);

imwrite(W, 'wynik33.png');

% Aby unsharp masking nie zniekształciło kolorów można przejść do HSV
% i przefiltrować tylko kanał V.

W = rgb2hsv(I);
W(:,:,3) = imfilter(W(:,:,3), fspecial('unsharp'), 'replicate');
W = min(max(W, 0), 1);
W = hsv2rgb(W);

imwrite(W, 'wynik34.png');

% Filtry wykrywające krawędzie też zwracają wyniki spoza dop. przedziału.

I = double(IMG_GRAY) / 255;

W1 = imfilter(I, fspecial('sobel'), 'replicate');
W1 = min(abs(W1), 1);

imwrite(W1, 'wynik35.png');

W2 = imfilter(I, transpose(fspecial('sobel')), 'replicate');
W2 = min(abs(W2), 1);

imwrite(W2, 'wynik36.png');

W = sqrt(W1 .^ 2 + W2 .^ 2);    % ręcznie obliczony gradient Sobela

imwrite(W, 'wynik37.png');

% O uzyskanych liczbach rzeczywistych można myśleć jako o prawdopodobieństwie,
% że ten piksel jest częścią krawędzi. Jeśli potrzebny jest ostry podział
% tak-nie, to trzeba przyjąć jakiś graniczny próg.

E = W > 0.8;

imwrite(E, 'wynik38.png');

% Octave ma specjalizowaną funkcję do wykrywania krawędzi na obrazach
% w odcieniach szarości, która potrafi m.in. użyć gradientu Sobela i na
% podstawie jego wyników inteligentnie wyznaczyć przebieg krawędzi.

W = edge(IMG_GRAY, 'Sobel');

imwrite(W, 'wynik39.png');

clear -exclusive IMG_RGB IMG_GRAY;

disp('Filtr medianowy');

% Obraz z dodanym szumem impulsowym:

I = imnoise(IMG_GRAY, 'salt & pepper');
imwrite(I, 'wynik41.png');

% Banalny filtr dolnoprzepustowy:

W = imfilter(I, fspecial('average'), 'replicate');
imwrite(W, 'wynik42.png');

% Filtr medianowy:

W = medfilt2(I);    % domyślny rozmiar przetwarzanego otoczenia to [ 3 3 ]
imwrite(W, 'wynik43.png');

disp('Filtry adaptacyjne');

% Octave nie ma specjalizowanych funkcji je realizujących, ale można je sobie
% złożyć z opisanych powyżej rzeczy. Zastanów się: jak zrobić rozmycie filtrem
% konwolucyjnym dolnoprzepustowym pomijające te piksele, dla których gradient
% Sobela sygnalizuje, że leżą na krawędzi?
