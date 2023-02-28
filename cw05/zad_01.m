pkg load image;
pkg list; 

plane= double(imread('test_img/jetplane.png'))/255;

kx= [1, 0 , -1; 
         2, 0 , -2; 
         1, 0 , -1];
        
ky= [1, 2, 1;
         0, 0, 0;
        -1,-2, -1];

[y, x, z] = size(plane);       

## robie sobla        
H = conv2(im2double(plane),kx,'same');
V = conv2(im2double(plane),ky,'same');
E = sqrt(H.*H + V.*V); 

# z otrzymanego wyniku robie czerwony kolor
red_sobel= zeros(y, x, 3);
red_sobel(:, :, 1) = E;

# zapisuje jako maska 
MASK = red_sobel > 0.5;

# biore czerwony kolor
red = plane; red(MASK) = 1;
blue = plane; blue(MASK) = 0;
green = plane; green(MASK) = 0;

result = zeros(y, x, 3);
result(:,:,1) = red;
result(:,:,2) = blue;
result(:,:,3) = green;

imwrite(result, 'wynik_zad01_1.png'); 
