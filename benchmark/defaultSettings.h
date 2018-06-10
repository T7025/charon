//
// Created by thomas on 5/8/18.
//

#ifndef CHARON_DEFAULTSETTINGS_H
#define CHARON_DEFAULTSETTINGS_H

#include <settings/Settings.h>

std::shared_ptr<Settings> getDefaultSettings() {
    auto settings = std::make_shared<Settings>();
    settings->fileSettings->enableFileOutput = false;
    settings->universeSettings->timeStep = 0.001;
    settings->universeSettings->gravitationalConstant = 1.0;
    settings->universeSettings->useCollisions = false;

    return settings;
}

#endif //CHARON_DEFAULTSETTINGS_H
