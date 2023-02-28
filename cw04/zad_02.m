pkg load image;
pkg list; 

LENA = imread('test_img/lena_color.png');
lena = imresize(LENA, [300, 480]);

imshow(lena);
drawnow;
input('Naciśnij Enter: ', 's');
close all;

[height, width , cos] = size(lena);

a = (height/width);

if (height > width)
  new_w = 128/a;
  img = imresize(lena, [128, new_w]);
elseif (height < width)
  new_h = a*128;
  img = imresize(lena, [new_h, 128]);
else
  img = imresize(lena, [128, 128]);
endif

imwrite(img, 'zad02_resize.png');
