//
// Created by thomas on 5/28/18.
//

#ifndef CHARON_BARNESHUTUNIVERSE2_H
#define CHARON_BARNESHUTUNIVERSE2_H


#include <base/AbstractUniverse.h>
#include <base/Vector3Offload.h>
#include <iostream>
#include <bitset>

struct Node {
    Vector3Offload position;
    fp mass;
    unsigned sfcIndex;
};

class BarnesHutUniverse2 : public AbstractUniverse {
public:
    BarnesHutUniverse2(std::shared_ptr<Settings> settings, std::shared_ptr<spdlog::logger> console);

    void addBody(fp mass, Vector3 position, Vector3 velocity, Vector3 acceleration) override;
    size_t bodyCount() const override { return position.size(); }
    void calculateFirstStep() override {}
    void calculateNextStep() override {}
    void logInternalState() const override {}
    std::vector<Vector3> getPositions() const override;
    fp calcEnergy() const override { return {}; }
    Vector3 calcMomentum() const override { return {}; }
    void calcNextPosition() override {};

    /// Calculates the index of a position in the Z space filling curve.
    /// \param depth k
    /// \param cubeSize D
    unsigned getSFC(const Vector3Offload position, const unsigned depth, const fp cubeSize) {
        fp mult = (1u << depth) / cubeSize;
        auto x = static_cast<unsigned int>(position.x * mult);
        auto y = static_cast<unsigned int>(position.y * mult);
        auto z = static_cast<unsigned int>(position.z * mult);
        unsigned result = 0;
        for (unsigned i = 0; i < depth; ++i) {
            result |= (x & (1u << i)) << (2 + i * 2);
            result |= (y & (1u << i)) << (1 + i * 2);
            result |= (z & (1u << i)) << (0 + i * 2);
        }
        return result;
    }

    #pragma omp declare target
    #pragma omp declare simd
    fp min(const fp first, const fp second) {
        return first < second ? first : second;
    }

    #pragma omp declare simd
    fp max(const fp first, const fp second) {
        return first < second ? second : first;
    }
    #pragma omp end declare target

    void buildTree() {
        auto nBodies = bodyCount();
        auto posX = positionX.data();
        auto posY = positionY.data();
        auto posZ = positionZ.data();

        fp minX = std::numeric_limits<fp>::infinity();
        fp maxX = -std::numeric_limits<fp>::infinity();
        fp minY = std::numeric_limits<fp>::infinity();
        fp maxY = -std::numeric_limits<fp>::infinity();
        fp minZ = std::numeric_limits<fp>::infinity();
        fp maxZ = -std::numeric_limits<fp>::infinity();
        #pragma omp target map(from: minX, maxX, minY, maxY, minZ, maxZ) map(to: posX[0:nBodies], posY[0:nBodies], posZ[0:nBodies])
        {
            #pragma omp teams distribute parallel for simd reduction(min: minX) reduction(max: maxX)
            for (unsigned i = 0; i < nBodies; ++i) {
                minX = min(minX, posX[i]);
                maxX = max(maxX, posX[i]);
            }
            #pragma omp teams distribute parallel for simd reduction(min: minY) reduction(max: maxY)
            for (unsigned i = 0; i < nBodies; ++i) {
                minY = min(minY, posY[i]);
                maxY = max(maxY, posY[i]);
            }
            #pragma omp teams distribute parallel for simd reduction(min: minZ) reduction(max: maxZ)
            for (unsigned i = 0; i < nBodies; ++i) {
                minZ = min(minZ, posZ[i]);
                maxZ = max(maxZ, posZ[i]);
            }
        }

        std::cout << minX <<", " << maxX <<"\n";
        std::cout << minY <<", " << maxY <<"\n";
        std::cout << minZ <<", " << maxZ <<"\n";

    }

private:
    unsigned numBodies;
    std::vector<fp> positionX;
    std::vector<fp> positionY;
    std::vector<fp> positionZ;

    std::vector<unsigned> sfcIndex;

    std::vector<Vector3Offload> position;

    std::vector<Node> octTree;
};


#endif //CHARON_BARNESHUTUNIVERSE2_H
