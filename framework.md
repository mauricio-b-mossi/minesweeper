Framework `Game`.

Game calls `stateManager.ProcessStateChanges` before each iteration. Then it 
polymorphycally retrives and uses `stateManger.GetCurrentState`. With it it calls
the `State` methods `ProcessEvent`, `Update`, `Draw`.

Each `State` object has access to a `shared_ptr` to `Game`.
