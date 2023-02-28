import cv2
import sys
import numpy as np

org_image = cv2.imread(sys.argv[1])
diff_image = cv2.imread(sys.argv[2])


diff = org_image
diff = cv2.absdiff(org_image, diff_image)

# result = np.zeros_like(org_image, np.uint8)
# result[diff] = org_image[diff]

cv2.imshow("3",diff)
cv2.waitKey(0)


# cv2.imwrite("result.jpg", result)

