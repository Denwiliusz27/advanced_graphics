pkg load image;

function new_img = img_to_bin (img)
  new_img = img(:,:,1) > 200 & img(:,:,1) < 255 | ...
  img(:,:,2) > 200 & img(:,:,2) < 255 | ...
  img(:,:,3) > 200 & img(:,:,3) < 255; 
  
endfunction


img = imread("kartka.jpg");

bin_img = im2bw(img,0.8);
bin_img_2 = img_to_bin (img);

subplot(3,1,1);
imshow(img);
subplot(3,1,2);
imshow(bin_img);
subplot(3,1,3);
imshow(bin_img_2);
input('Naciśnij Enter: ', 's');