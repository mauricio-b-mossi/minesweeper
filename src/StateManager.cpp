#include "StateManager.hpp"
#include "State.hpp"
#include <iostream>

void StateManager::ProcessStateChanges() {
  if (mIsReplacing && mIsAdding) {

        mStack.pop();
        mStack.push(std::move(mBuffState));

        if (mExtrasRef) {
          std::cout << "Extras init" << std::endl;
          mStack.top()->Init(std::move(mExtrasRef));
        } else {
          std::cout << "NO Extras init" << std::endl;
          mStack.top()->Init();
        }

        mIsReplacing = false;
        mIsAdding = false;

  } else if (mIsAdding) {

        if (!mStack.empty()) {
          mStack.top()->Pause();
        }

        mStack.push(std::move(mBuffState));

        if (mExtrasRef) {
          mStack.top()->Init(std::move(mExtrasRef));
        } else {
          mStack.top()->Init();
        }

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

void StateManager::PushState(StateRef stateRef, bool isReplacing,
                             ExtrasRef extrasRef) {
  mBuffState = std::move(stateRef);
  mExtrasRef = std::move(extrasRef);
  mIsAdding = true;
  mIsReplacing = isReplacing;
}

void StateManager::PopState() { mIsRemoving = true; }
