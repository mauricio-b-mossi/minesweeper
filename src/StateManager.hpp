#pragma once
#include "State.hpp"
#include <memory> // Cannot use std::make_unique<>() due to C++11 restiction;
#include <stack>
#include <vector>

typedef std::unique_ptr<State> StateRef;

// Invariant must start with a State.
class StateManager {

public:
  void ProcessStateChanges();
  StateRef &GetCurrentState();
  void PushState(StateRef stateRef, bool isReplacing);
  void PushState(StateRef stateRef, bool isReplacing, ExtrasRef extrasRef);
  void PopState();

private:
  std::stack<StateRef> mStack;

  // Buffer all states as I want to draw the buffered state in the next
  // iteration.
  StateRef mBuffState = nullptr;
  bool mIsReplacing = false;
  bool mIsAdding = false;
  bool mIsRemoving = false;
  ExtrasRef mExtrasRef = nullptr;
};
