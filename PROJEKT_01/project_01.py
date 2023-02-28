# Daniel Wielgosz, projekt_01

import cv2
import os

# zwraca ilość zdjęć w folderze
def get_nr_of_images(folder_name):
    img_counter = 0

    for path in os.listdir(folder_name):
        if os.path.isfile(os.path.join(folder_name, path)):
            img_counter += 1

    return img_counter


# oblicza iloczyny dające podaną wartość
def find_factors(n):
    factors = []

    for i in range(1, n+1):
        if n % i == 0:
            factors.append(i)

    return factors


# z podanego obrazka oblicza ilość zdjęć na szerokość i wysokość
def divide_size_by_images_nr(image, nr_images):
    org_photo = cv2.imread(image)

    # odczytuje rozmiary oryginalnego obrazka
    # dimensions = org_photo.shape
    p_height = org_photo.shape[0]
    p_width = org_photo.shape[1]

    # obliczam ratio obrazka głównego
    photo_ratio = p_width / p_height
    # print("ratio: ", photo_ratio)

    # znajduje iloczyny dla podanej ilosci obrazkow
    nr_images_factors = find_factors(nr_images)

    # znajduje najlepsze wartości z listy iloczynów, których ratio zblizone jest do ratio obrazka gł
    min_ratio_differ = nr_images_factors[len(nr_images_factors)-1] / nr_images_factors[0]
    factor1 = 0
    factor2 = 0
    for i in range((len(nr_images_factors)//2)):
        if nr_images_factors[i] > nr_images_factors[len(nr_images_factors)-1-i]:
            test_ratio = nr_images_factors[i] / nr_images_factors[len(nr_images_factors)-1-i]
        else:
            test_ratio = nr_images_factors[len(nr_images_factors)-1-i] / nr_images_factors[i]

        differ = abs(photo_ratio - test_ratio)
        if differ < min_ratio_differ:
            min_ratio_differ = differ
            factor1 = nr_images_factors[i]
            factor2 = nr_images_factors[len(nr_images_factors)-1-i]

    if p_width > p_height:
        return factor2, factor1
    else:
        return factor1, factor2


# zwraca wysokość i szerokość małego obrazka w moizaice na podstawie ilości zdj per wiersz i kolumna
def get_single_image_size(image, factor_w, factor_h):
    org_photo = cv2.imread(image)

    # odczytuje rozmiary oryginalnego obrazka
    p_height = org_photo.shape[0]
    p_width = org_photo.shape[1]

    img_width = p_width // factor_w
    img_height = p_height // factor_h

    return img_width, img_height


# dla każdego odczytanego zdjęcia z folderu zmienia jego rozmiar na podstawie podanych argumentów
def resize_images(folder, img_w, img_h):
    resized_images = []

    for filename in os.listdir(folder):
        img = cv2.imread(os.path.join(folder, filename))
        if img is not None:
            h = img.shape[0]
            w = img.shape[1]

            if h > w:
                img = img[0:w, 0:w]
            else:
                img = img[0:h, 0:h]

            img = cv2.resize(img, (img_w, img_h))
            resized_images.append(img)

    return resized_images


# tworzy mozaike z listy obrazków na podstawie liczby wierszy i kolumn
def create_mosaic(resized_images, factor_w, factor_h):
    nr_per_row = 0

    mosaic = cv2.hconcat(resized_images[0:factor_w])
    nr_per_row = nr_per_row + factor_w

    for i in range(factor_h-1):
        mosaic2 = cv2.hconcat(resized_images[nr_per_row:nr_per_row+factor_w])
        mosaic = cv2.vconcat([mosaic, mosaic2])
        nr_per_row = nr_per_row + factor_w

    return mosaic


# nakłada dwa zdjęcia na siebie
def blend_images(org_image, mosaic):
    org_photo = cv2.imread(org_image)

    # przycięcie gł zdjęcia do rozmiarów mozaiki
    org_photo = org_photo[0:mosaic.shape[0], 0:mosaic.shape[1]]

    blend = cv2.addWeighted(org_photo, 0.6, mosaic, 0.4, 0)
    return blend



if __name__ == '__main__':
    folder = "images"
    main_img = 'volleyball.png'
    resized_images = []

    img_counter = get_nr_of_images(folder)

    factor_w, factor_h = divide_size_by_images_nr(main_img, img_counter)

    img_w, img_h = get_single_image_size(main_img, factor_w, factor_h)

    resized_images = resize_images(folder, img_w, img_h)

    mosaic = create_mosaic(resized_images, factor_w, factor_h)

    result = blend_images(main_img, mosaic)

    cv2.imwrite("wynik.png", result)