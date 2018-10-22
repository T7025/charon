//
// Created by thomas on 6/16/18.
//

#ifndef CHARON_BARNESHUTOFFLOADUNIVERSE_H
#define CHARON_BARNESHUTOFFLOADUNIVERSE_H


#include "BarnesHutUniverseBase.h"

class BarnesHutOffloadUniverse : public BarnesHutUniverseBase {
public:
    BarnesHutOffloadUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    ~BarnesHutOffloadUniverse() override;

    void initBodies(std::shared_ptr<UniverseShape> shape) override;

    void calculateFirstStep() override;

    void calculateNextStep() override;

    // Used for testing
    void calcNextPosition() override;

private:
    Vector3 calculateAcceleration(const Node &targetNode) const;

    void flattenTree();

    void buildTree();

    void sortTree();

    void scalePositions();

    void calcSFCIndices();

    void generateInternalNodes();

    void removeDuplicateInternalNodes();

    void establishParentChildRel();

    void calculateNodeData();
};


#endif //CHARON_BARNESHUTOFFLOADUNIVERSE_H
