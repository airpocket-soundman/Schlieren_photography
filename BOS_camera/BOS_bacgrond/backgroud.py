import numpy as np
import cv2

# Camera resolution
W = 960
H = 720

# Dot parameters
dot_coverage = 0.15   # 15% coverage
dot_radius_px = 2     # radius => diameter ~4px (0.4mm)

# White background
img = np.ones((H, W), dtype=np.uint8) * 255

# Number of dots (approx.)
pixel_area_per_dot = np.pi * dot_radius_px * dot_radius_px
N = int((W * H * dot_coverage) / pixel_area_per_dot)

ys = np.random.randint(dot_radius_px, H - dot_radius_px, N)
xs = np.random.randint(dot_radius_px, W - dot_radius_px, N)

for x, y in zip(xs, ys):
    cv2.circle(img, (x, y), dot_radius_px, 0, -1)

cv2.imwrite("bos_random_dots_960x720.png", img)
print("Saved: bos_random_dots_960x720.png")