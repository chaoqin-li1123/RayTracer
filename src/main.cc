#include <iostream>
#include <thread>

#include "camera.h"

int main() {
  World::init();
  Image image;
  Camera camera(Point(15, 2, 3), Point(0, 0, 0), Direction(0, 1, 0), 30,
                ASPECT_RATIO, 0.04);
  // Divide the workload and spawn multiple threads to handle it.
  // TODO(chaoqin-li1123): Use GPU for parallelism.
  int thread_cnt = std::thread::hardware_concurrency();
  int h_interval = IMAGE_H / thread_cnt;
  auto func = [&](int h0, int h1) {
    for (int h = h0; h < IMAGE_H && h <= h1; h++) {
      for (int w = 0; w < IMAGE_W; w++) {
        Color accumulated = Color(0, 0, 0);
        int samples_cnt = 0;
        // Take SAMPLE_RATE ^ 2 samples for each pixel for anti-aliasing.
        for (int i = -SAMPLE_RATE / 2; i < SAMPLE_RATE / 2; i++) {
          for (int j = -SAMPLE_RATE / 2; j < SAMPLE_RATE / 2; j++) {
            constexpr double SAMPLE_INTERVAL = 1.0 / SAMPLE_RATE;
            double dx = (w + i * SAMPLE_INTERVAL) / (IMAGE_W - 1);
            double dy = (h + j * SAMPLE_INTERVAL) / (IMAGE_H - 1);
            if (dx < 0.0 || dx > 1.0 || dy < 0.0 || dy > 1.0) continue;
            Ray r = camera.emitRay(dx, dy);
            accumulated += World::findColor(r, 0);
            samples_cnt++;
          }
        }
        image[h][w] = accumulated / (float)samples_cnt;
      }
      std::cerr << h << ", " << std::endl;
    }
  };
  std::vector<std::thread> threads;
  for (int i = 0; i * h_interval < IMAGE_H; i++) {
    threads.emplace_back(func, i * h_interval, (i + 1) * h_interval);
  }
  for (std::thread& thread : threads) thread.join();
  ImagePrinter::printPpm(image, "world.ppm");
}