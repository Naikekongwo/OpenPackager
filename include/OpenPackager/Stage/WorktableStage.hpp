#pragma once

#include "World/Stage/BaseStage.hpp"

class WorktableStage : public BaseStage
{
  public:
    WorktableStage();
    ~WorktableStage() override = default;

    bool parseEvents(Event *event) override;

    void onEnter() override;
    void initializeComponents() override;

    void onUpdate() override;

    void onRender() override;

    void onExit() override;

    void onDestroy() override;
};