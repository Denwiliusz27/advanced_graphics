MANDRIL = imread('test_img/mandril_color.png');
#MANDRIL = imread('test_img/lena_color.png');



W1 = (MANDRIL(:, :, 1)/3 + MANDRIL(:, :, 2)/3 + MANDRIL(:, :, 3)/3);
imwrite(W1, 'wynik_zad02_1.png'); 

W2 = 0.299 * MANDRIL(:, :, 1) + 0.587 * MANDRIL(:, :, 2) + 0.114 * MANDRIL(:, :, 3);
imwrite(W2, 'wynik_zad02_2.png');