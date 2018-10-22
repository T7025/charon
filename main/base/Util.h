//
// Created by thomas on 3/14/18.
//


#ifndef CHARON_UTIL_H
#define CHARON_UTIL_H

using fp = double;

enum class UniversePerfType {
    singleThread,
    multiThread,
    offload,
};

constexpr fp barnesHutCutoff = 0.7;

template <typename T, typename S>
T &lshift(T &val, const S &shift) {
    val <<= shift - (shift > 0);
    val <<= shift > 0;
    return val;
};

template <typename T, typename S>
T lshift(T &&val, const S &shift) {
    val <<= shift - (shift > 0);
    val <<= shift > 0;
    return val;
};

template <typename T, typename S>
T &lshift(T &val, S &&shift) {
    val <<= shift - (shift > 0);
    val <<= shift > 0;
    return val;
};

template <typename T, typename S>
T lshift(T &&val, S &&shift) {
    val <<= shift - (shift > 0);
    val <<= shift > 0;
    return val;
};

template <typename T, typename S>
T &rshift(T &val, const S &shift) {
    val >>= shift - (shift > 0);
    val >>= shift > 0;
    return val;
};

template <typename T, typename S>
T rshift(T &&val, const S &shift) {
    val >>= shift - (shift > 0);
    val >>= shift > 0;
    return val;
};

template <typename T, typename S>
T &rshift(T &val, S &&shift) {
    val >>= shift - (shift > 0);
    val >>= shift > 0;
    return val;
};

template <typename T, typename S>
T rshift(T &&val, S &&shift) {
    val >>= shift - (shift > 0);
    val >>= shift > 0;
    return val;
};

#endif //CHARON_UTIL_H

