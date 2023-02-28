% Ilustracja podstawowych możliwości MATLAB-a / Octave'a.
% Testowane pod Octave 4.2.2 na Ubuntu 18.04 i 4.4.1 na Debianie 10.
%
% Mogą Państwo kopiować ręcznie z tego pliku kolejne instrukcje i je wklejać
% w linii poleceń Octave'a. Można też uruchomić cały plik jako skrypt albo
% pisząc w uniksowej linii komend "octave octave01.m", albo w linii poleceń
% Octave'a "run('octave01.m')".
%
% Jeśli będziecie korzystali z Octave'a na spk-ssh przez PuTTY, to nie możecie
% liczyć na poprawne działanie znajdujących się w drugiej połowie pliku
% poleceń otwierających okienka z podglądem przetwarzanych obrazów (imshow,
% figure, subplot). Funkcje imread oraz imwrite będą działać poprawnie, bo
% one nie potrzebują graficznego terminala.

disp('Wyrażenia i zmienne skalarne:');

2 + 2
x = 2 + 2
y = 2 + 2;
z = 2 * pi^2;

b = x < z;      % wyrażenie logiczne, czyli boolowskie

whos    % wyświetla aktualnie istniejące zmienne i ich typy
x, y
z       % wyświetlane wartości zmiennoprzecinkowe są zaokrąglane

printf('%.40f\n', z);
sin(z)
printf('%.40f\n', sin(z));

b

clear all;      % usuń wszystkie zmienne

input('Naciśnij Enter: ', 's');

disp('Definiowanie macierzy przez wyliczenie elementów:');

A = [ 1, 2, 3 ];        % albo po prostu [ 1 2 3 ]
B = [ 7; 8; 9 ];
C = [ 11, 12; 21, 22 ];

A       % ku pamięci: brak średnika powoduje wypisanie wyniku na ekran
A(1)
B
B(2)
C
C(2,1)

input('Naciśnij Enter: ', 's');

disp('Składanie i rozcinanie macierzy:');

D = [ C; 31 32 ]
E = [ D B ]

whos;   % czy wymiary macierzy D i E są zgodne z Twoimi oczekiwaniami?

input('Naciśnij Enter: ', 's');

E
E(1,:)
E(:,2:3)
E(:)    % zamienia macierz na ciąg (wszystkie elementy w jednej kolumnie)

input('Naciśnij Enter: ', 's');

disp('Operacje arytmetyczno-logiczne na macierzach:');

A = 2 * E + 1
B = A > 25      % wynikiem jest macierz wartości logicznych
C = A + B       % przy operacjach arytmetycznych są traktowane jako 0 lub 1

whos;

input('Naciśnij Enter: ', 's');

disp('Standardowe macierze o zadanej liczbie wierszy i kolumn:');

A = zeros(4)
B = ones(3, 5)
C = eye(3, 5)
D = rand(2)

clear all;

input('Naciśnij Enter: ', 's');

disp('Wczytywanie i wyświetlanie obrazów:');

LENA_COL = imread('test_img/lena_color.png');
LENA_GR = imread('test_img/lena_gray.png');

whos;

figure('Name', 'Obraz kolorowy');
imshow(LENA_COL);

figure('Name', 'Obraz w odcieniach szarości');
imshow(LENA_GR);

drawnow;    % wyświetl teraz utworzone wcześniej okna (ważne w skryptach)
input('Naciśnij Enter: ', 's');
close all;  % zamknij okna z grafiką i/lub wykresami

disp('Konwersja z reprezentacji uint8 [0, 255] na double [0.0, 1.0]:');

LENA_COL = double(LENA_COL) / 255;
LENA_GR = double(LENA_GR) / 255;

whos;

figure('Name', 'Dwa obrazy w jednym oknie');
subplot(1, 2, 1);
imshow(LENA_COL);
subplot(1, 2, 2);
imshow(LENA_GR);

drawnow;
input('Naciśnij Enter: ', 's');
close all;

disp('Banalne operacje na pikselach (czasochłonne petle for):');

[liczba_wierszy, liczba_kolumn, liczba_warstw] = size(LENA_COL);

for y = 1:liczba_wierszy
  for x = 1:liczba_kolumn
    W(y,x,1) = 1.0 - LENA_COL(y,x,1);
    W(y,x,2) = 1.0 - LENA_COL(y,x,2);
    W(y,x,3) = 1.0 - LENA_COL(y,x,3);
  end
end

imwrite(W, 'wynik01.png');

figure('Name', 'Dwa algorytmy, ten sam wynik');
subplot(1, 2, 1);
imshow(W);

disp('Ta sama banalna operacja, ale zapisana macierzowo:');

W2 = 1.0 - LENA_COL;

imwrite(W2, 'wynik02.png');

subplot(1, 2, 2);
imshow(W2);

drawnow;
input('Naciśnij Enter: ', 's');
close all;

disp('Rozłożenie obrazu na kanały i ponowne złączenie w innej kolejności:');

red = LENA_COL(:,:,1);
green = LENA_COL(:,:,2);
blue = LENA_COL(:,:,3);
W = cat(3, blue, red, green);
clear red green blue;

imwrite(W, 'wynik03.png');

figure('Name', 'Zmiana kolejności kanałów');
imshow(W);

drawnow;
input('Naciśnij Enter: ', 's');
close all;

disp('Modyfikacja wybranego fragmentu obrazu:');

W = LENA_COL;
W(:,1:200) = 1 - LENA_COL(:,1:200);

imwrite(W, 'wynik04.png');

figure('Name', 'Modyfikacja fragmentu obrazu');
imshow(W);

drawnow;
input('Naciśnij Enter: ', 's');
close all;

disp('Koniec.');
