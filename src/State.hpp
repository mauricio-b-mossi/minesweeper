#pragma once

struct State{
    virtual ~State(){};

    virtual void Init() = 0;

    virtual void ProcessEvent() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;
};
