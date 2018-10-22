//
// Created by thomas on 6/14/18.
//

#ifndef CHARON_BARNESHUTUNIVERSE_H
#define CHARON_BARNESHUTUNIVERSE_H


#include "BarnesHutUniverseBase.h"

class BarnesHutUniverse : public BarnesHutUniverseBase {
public:
    BarnesHutUniverse(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

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


#endif //CHARON_BARNESHUTUNIVERSE_H
