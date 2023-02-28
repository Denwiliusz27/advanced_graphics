#!/usr/bin/python3

import cv2 as cv

movie = cv.VideoCapture('test_mov/youtube-2kMq-VF0M7g.mp4')

n = 0
rv, frame = movie.read()
while rv:
    n += 1
    # przetwarzamy właśnie pobraną klatkę:
    sw = int(0.1 * frame.shape[1])
    sh = int(0.1 * frame.shape[0])
    sf = cv.resize(frame, (sw, sh))
    cv.imwrite('wynik%03d.png' % n, sf)
    # próbujemy pobrać następną:
    rv, frame = movie.read()

print('Liczba przetworzonych klatek: %d' % n)
