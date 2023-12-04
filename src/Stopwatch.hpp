#pragma once
#include <chrono>

struct Stopwatch {

  void Start();

  void Toggle();

  int GetSeconds();

  int GetMinutes();

  int GetTimeSinceStart();

  int GetTimeStamp();

  void Reset();

  void Freeze();

  bool mIsFreezed = false;

private:
  bool mIsPaused = false;
  bool mIsZeroed = true;
  std::chrono::seconds pauseBuffer{0};
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  std::chrono::time_point<std::chrono::high_resolution_clock> pause_time;
  std::chrono::seconds timeAtPause{0};
  int timeStamp{0};
};
