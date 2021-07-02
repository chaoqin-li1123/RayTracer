#include "image.h"
#include "iostream"
#include "world.h"

int main() {
  Point origin(0, 0, 0);
  Direction horizontal(VIEWPORT_W, 0, 0);
  Direction vertical(0, VIEWPORT_H, 0);
  Point lower_left_corner = origin - (horizontal / 2.0) - (vertical / 2.0) -
                            Direction(0.0, 0.0, FOCAL_LEN);
  Color image[IMAGE_H][IMAGE_W]{};
  World::init();
  for (int h = 0; h < IMAGE_H; h++) {
    for (int w = 0; w < IMAGE_W; w++) {
      Color accumulated = Color(0, 0, 0);
      int samples_cnt = 0;
      // Take SAMPLE_RATE ^ 2 samples for each pixel for anti-aliasing.
      for (int i = -SAMPLE_RATE / 2; i < SAMPLE_RATE / 2; i++) {
        for (int j = -SAMPLE_RATE / 2; j < SAMPLE_RATE / 2; j++) {
          constexpr double INTERVAL = 1.0 / SAMPLE_RATE;
          double dx = (w + i * INTERVAL) / (IMAGE_W - 1);
          double dy = (h + j * INTERVAL) / (IMAGE_H - 1);
          if (dx < 0.0 || dx > 1.0 || dy < 0.0 || dy > 1.0) continue;
          Ray r(lower_left_corner + dx * horizontal + dy * vertical);
          accumulated += World::findColor(r, 0);
          samples_cnt++;
        }
      }
      image[h][w] = accumulated / (float)samples_cnt;
    }
  }
  ImagePrinter::printPpm(image, "background.ppm");
}