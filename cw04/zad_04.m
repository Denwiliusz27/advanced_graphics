pkg load image;
pkg list; 

LENA = imread('test_img/lena_color.png');

##[rstat, info, bands] = gdalread(LENA)

info = imfinfo('test_img/lena_color.png');
printf("%s\n", info.ColorType);

if ( size(LENA, 3) == 3) 
  
  result = rgb2gray(LENA);
  imwrite(result, 'zad04_result.png');
  
  info =  imfinfo('zad04_result.png');
  printf("%s\n", info.ColorType )
endif

