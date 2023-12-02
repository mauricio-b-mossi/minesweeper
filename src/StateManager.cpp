#include "StateManager.hpp"

void StateManager::ProcessStateChanges() {
  if (mIsReplacing && mIsAdding) {
    mStack.pop();
    mStack.push(std::move(mBuffState));
    mStack.top()->Init();
    mIsReplacing = false;
    mIsAdding = false;
  } else if (mIsAdding) {
    if (!mStack.empty()) {
      mStack.top()->Pause();
    }
    mStack.push(std::move(mBuffState));
    mStack.top()->Init();
    mIsAdding = false;
  } else if (mIsRemoving) {
    mStack.pop();
    mStack.top()->Resume();
    mIsRemoving = false;
  }
}

StateRef &StateManager::GetCurrentState() { return mStack.top(); }

void StateManager::PushState(StateRef stateRef, bool isReplacing) {
  mBuffState = std::move(stateRef);
  mIsAdding = true;
  mIsReplacing = isReplacing;
}

void StateManager::PopState() { mIsRemoving = true; }
