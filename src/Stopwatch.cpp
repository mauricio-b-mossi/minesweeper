#include "Stopwatch.hpp"

void Stopwatch::Start() {
  start_time = std::chrono::high_resolution_clock::now();
  mIsPaused = false;
  mIsZeroed = false;
  mIsFreezed = false;
  pauseBuffer = std::chrono::seconds{0};
  timeAtPause = std::chrono::seconds{0};
};

void Stopwatch::Toggle() {
  if (!mIsFreezed) {
    if (mIsPaused) {
      pauseBuffer += std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::high_resolution_clock::now() - pause_time);
      timeAtPause = std::chrono::seconds{0};
      mIsPaused = false;
    } else {
      pause_time = std::chrono::high_resolution_clock::now();
      timeAtPause = std::chrono::duration_cast<std::chrono::seconds>(
          (std::chrono::high_resolution_clock::now() - start_time) -
          pauseBuffer);
      mIsPaused = true;
    }
  }
};

int Stopwatch::GetSeconds() {
  if (mIsFreezed) {
    return timeStamp % 60;
  } else if (mIsZeroed) {
    return 0;
  } else if (mIsPaused) {
    return timeAtPause.count() % 60;
  }
  return std::chrono::duration_cast<std::chrono::seconds>(
             (std::chrono::high_resolution_clock::now() - start_time) -
             pauseBuffer)
             .count() %
         60;
};

int Stopwatch::GetMinutes() {
  if (mIsFreezed) {
    return timeStamp / 60;
  } else if (mIsZeroed) {
    return 0;
  } else if (mIsPaused) {
    return timeAtPause.count() / 60;
  }
  return std::chrono::duration_cast<std::chrono::seconds>(
             (std::chrono::high_resolution_clock::now() - start_time) -
             pauseBuffer)
             .count() /
         60;
};

void Stopwatch::Reset() { mIsZeroed = true; }

int Stopwatch::GetTimeStamp() {
  return std::chrono::duration_cast<std::chrono::seconds>(
             (std::chrono::high_resolution_clock::now() - start_time) -
             pauseBuffer)
      .count();
};

void Stopwatch::Freeze() {
  mIsFreezed = true;
  timeStamp = GetTimeStamp();
}

int Stopwatch::GetTimeSinceStart() {
  return std::chrono::duration_cast<std::chrono::seconds>(
             std::chrono::high_resolution_clock::now() - start_time)
      .count();
}
