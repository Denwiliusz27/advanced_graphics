pkg load image;
pkg list; 

LENA = imread('test_img/lena_color.png');
LENA_B = imread('test_img/lena_gray.png');

A = [ 1   1   1   1   1   1   1   1   1   1  1;
         1   0   0   0   0   0   0   0.7 0.2 0  1;
         1   0   0   0   1   1   0.8 0   0   0  1;
         1   0   0   0   0   0   0   1   0.8 0  1;
         1   0   0.2 0.4 0.6 0   0   0   0   0  1;
         1   0   0.1 0   0   0   0   0   0   0  1;
         1   0   0   0   0.9 0   0   0   0   0  1;
         1   0   0   0.4 0.4 0.4 0   0   1   0  1;
         1   0   0.9 0.4 0.1 0.4 0.9 0   1   0  1;
         1   0   0   0.4 0.4 0.4 0   0   1   0  1;
         1   0   0   0   0.9 0   0   0   1   0  1;
         1   1   1   1   1   1   1   1   1   1  1];

         
% SKALOWANIE         
lena_r1 = imresize(LENA, 0.5);
imwrite(lena_r1, 'zad01-lenar1.png');

lena_r2 = imresize(LENA, [70, 200]);
imwrite(lena_r2, 'zad01-lenar2.png');

imwrite(A, 'macierz.png');

arr = mat2gray(A);  % zamienia macierz na obraz
a_r = imresize(arr, [300, 400]);
imwrite(a_r, 'macierz_1.png');

a_r2 = imresize(A, 0.5);
imwrite(a_r2, 'macierz_2.png');

%imshow(a_r2);
%drawnow;
%input('Naciśnij Enter: ', 's');
%close all;


% FILTROWANIE
avg = imfilter(LENA_B, fspecial('average'), 'replicate');
%imwrite(avg, 'lena_avg.png');

avg_2 = imfilter(LENA_B,  fspecial('disk',10), 'replicate');
%imwrite(avg_2, 'lena_avg2.png');

mot = imfilter(LENA_B, fspecial('motion', 30, 0), 'replicate');
%imwrite(mot, 'lena_mot.png');

%dan = imread('dan.png');
%dan = imfilter(dan,  fspecial('motion', 120, 0), 'replicate');
%imwrite(dan, 'dan2.png');
%dan_s = imfilter(dan, fspecial('unsharp'), 'replicate');
%imwrite(dan_s, 'dan_sharp.png');

prew = imfilter(LENA_B, fspecial('prewitt'), 'replicate');
%imwrite(prew, 'lena_prew.png');

sob = imfilter(LENA_B, fspecial('sobel'), 'replicate');
%imwrite(sob, 'lena_sob.png');

sharp = imfilter(LENA_B, fspecial('unsharp'), 'replicate');
%imwrite(sharp, 'lena_sharp.png');


%subplot(1,4,1), imshow(LENA_B);
%subplot(1,4,2), imshow(prew);
%subplot(1,4,3), imshow(sob);
%subplot(1,4,4), imshow(sharp);
%drawnow;
%input('Naciśnij Enter: ', 's');
%close all;


%array
a_avg = imfilter(arr, fspecial('average'), 'replicate');
imwrite(a_avg, 'matr_avg.png');

a_avg_2 = imfilter(arr,  fspecial('disk',10), 'replicate');
imwrite(a_avg_2, 'matr_avg2.png');

a_mot = imfilter(arr, fspecial('motion', 30, 0), 'replicate');
imwrite(a_mot, 'matr_mot.png');

a_prew = imfilter(arr, fspecial('prewitt'), 'replicate');
imwrite(a_prew, 'matr_prew.png');

a_sob = imfilter(arr, fspecial('sobel'), 'replicate');
imwrite(a_sob, 'matr_sob.png');

a_sharp = imfilter(arr, fspecial('unsharp'), 'replicate');
imwrite(a_sharp, 'matr_sharp.png');

%FILTR
lena = double(LENA_B) / 255;

M = [ 0.1,   0.1,  0.1;
         0.1,  0.2,  0.1;
        0.1, 0.1, 0.1];

%img = double(imread("test_img/lena_gray.png"))/255;

res = conv2(lena, M);
%res = res/10

imshow(res);
drawnow;
input('Naciśnij Enter: ', 's');
close all;


random = rand(10);
randomConv = conv2(random, mask);


