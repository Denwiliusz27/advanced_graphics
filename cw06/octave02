#!/usr/bin/octave

# Octave oprócz % rozpoznaje również # jako znak komentarza. Dzięki temu można
# dodać na początek skryptu magiczną linię zaczynającą się od "#!", ustawić
# plikowi bity wykonywalności (chmod +x), i móc uruchamiać octave'owe skrypty
# jak każdy inny uniksowy program.

for i = 1:nargin
    fname_src = argv(){i};
    fname_dst = sprintf('wynik%02i.jpg', i);
    IMG = imread(fname_src);
    IMG = 255 - IMG;
    imwrite(IMG, fname_dst);
end
