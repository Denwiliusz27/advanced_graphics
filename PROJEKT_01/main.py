import cv2
import os

def find_factors(n):
    factors = []

    for i in range(1, n+1):
        if n % i == 0:
            factors.append(i)

    return factors


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    folder = "images/"
    images = []

    # for filename in os.listdir(folder):
    #     img = cv2.imread(os.path.join(folder, filename))
    #     if img is not None:
    #         images.append(img)
    #         print(len(images))

    # ilosc zdjec
    # nr_images = len(images)
    counter = 0

    for path in os.listdir(folder):
        if os.path.isfile(os.path.join(folder, path)):
            counter += 1

    print("counter: ", counter)

    for filename in os.listdir(folder):
        img = cv2.imread(os.path.join(folder, filename))
        if img is not None:
            size = img.shape
            h = img.shape[0]
            w = img.shape[1]

            if h > w:
                img = img[0:w, 0:w]
            else:
                img = img[0:h, 0:h]

            # cv2.imshow("cropped", img)
            # cv2.waitKey(0)

            img = cv2.resize(img, (100, 100))
            # cv2.imshow("cropped", img)
            # cv2.waitKey(0)
            images.append(img)
            print(len(images))

    mosaic = cv2.hconcat(images[0:10])
    mosaic2 = cv2.hconcat(images[0:10])
    mosaic3 = cv2.vconcat([mosaic, mosaic2])



    # nr_images = 88
    #
    # org_photo = cv2.imread('volleyball.png')
    #
    # #rozmiary oryginalnego obrazka
    # dimensions = org_photo.shape
    # height = org_photo.shape[0]
    # width = org_photo.shape[1]
    #
    # photo_ratio = width / height
    # print("ratio: ", photo_ratio)
    #
    # # iloczyny dajace ilosc obrazkow
    # nr_images_factors = find_factors(nr_images)
    # print(nr_images_factors)
    #
    # min_ratio_differ = nr_images_factors[len(nr_images_factors)-1] / nr_images_factors[0]
    # factor1 = 0
    # factor2 = 0
    # for i in range((len(nr_images_factors)//2)):
    #     print("pair: ", nr_images_factors[i], ", ", nr_images_factors[len(nr_images_factors)-1-i])
    #     if nr_images_factors[i] > nr_images_factors[len(nr_images_factors)-1-i]:
    #         test_ratio = nr_images_factors[i] / nr_images_factors[len(nr_images_factors)-1-i]
    #     else:
    #         test_ratio = nr_images_factors[len(nr_images_factors)-1-i] / nr_images_factors[i]
    #
    #     differ = abs(photo_ratio - test_ratio)
    #     print("differ: ", differ)
    #     if differ < min_ratio_differ:
    #         min_ratio_differ = differ
    #         factor1 = nr_images_factors[i]
    #         factor2 = nr_images_factors[len(nr_images_factors)-1-i]
    #
    # print("f1: ", factor1)
    # print("f2: ", factor2)






    # ZNALEZC DOBRE ZDJECIA W MALEJ ROZDZIELCZOSCI


    # for image in images:
    #     image = cv2.resize(image, (100, 100))
    #
    # mosaic = cv2.hconcat(images)
    # mosaic2 = cv2.hconcat(images)
    # mosaic3 = cv2.vconcat([mosaic, mosaic2])
    #
    #
    cv2.imwrite("wynik.png", mosaic3)