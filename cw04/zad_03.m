#!/usr/bin/octave

pkg load image;
pkg list; 

for i = 1:nargin
  fname_src = imread(argv(){i});
  fname_dst = sprintf('wynik%02i.jpg', i);
  
  [height, width , cos] = size(fname_src);

  a = (height/width);

  if (height > width)
    new_w = 128/a;
    img = imresize(fname_src, [128, new_w]);
  elseif (height < width)
    new_h = a*128;
    img = imresize(fname_src, [new_h, 128]);
  else
    img = imresize(fname_src, [128, 128]);
  endif

  imwrite(img, fname_dst);
end
