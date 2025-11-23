// CG_Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include "GamesEngineeringBase.h"
#include "GEMLoader.h"
#define SQ(x) ((x)*(x))
template<typename T>
static T lerp(const T a, const T b, float t) {
    return a * (1.0f - t) + (b * t);
}

static const int canvasWidth = 800;
static const int canvasHeight = 600;

class Vec3 {
public:
    union {
        float v[3];
        struct { float x, y, z; };
    };
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vec3() : x(0), y(0), z(0) {}
    Vec3 operator+(const Vec3& pVec) const {
        return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
    }
    Vec3 operator+(const float val) const {
        return Vec3(v[0] + val, v[1] + val, v[2] + val);
    }
    Vec3 operator-(const Vec3& pVec) const {
        return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
    }
    Vec3 operator-(const float val) const {
        return Vec3(v[0] - val, v[1] - val, v[2] - val);
    }
    Vec3 operator-() const {
        return Vec3(-v[0], -v[1], -v[2]);
    }
    Vec3& operator+=(const Vec3& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        return *this;
    }
    Vec3& operator+=(const float val) {
        v[0] += val;
        v[1] += val;
        v[2] += val;
        return *this;
    }
    Vec3& operator-=(const Vec3& pVec) {
        v[0] -= pVec.v[0];
        v[1] -= pVec.v[1];
        v[2] -= pVec.v[2];
        return *this;
    }
    Vec3& operator-=(const float val) {
        v[0] -= val;
        v[1] -= val;
        v[2] -= val;
        return *this;
    }
    Vec3 operator*(const Vec3& pVec) const {
        return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
    }
    Vec3 operator*(const float val) const {
        return Vec3(v[0] * val, v[1] * val, v[2] * val);
    }
    Vec3& operator*=(const Vec3& pVec) {
        v[0] *= pVec.v[0];
        v[1] *= pVec.v[1];
        v[2] *= pVec.v[2];
        return *this;
    }
    Vec3& operator*=(const float val) {
        v[0] *= val;
        v[1] *= val;
        v[2] *= val;
        return *this;
    }
    Vec3 operator/(const Vec3& pVec) const {
        return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);
    }
    Vec3 operator/(const float val) const {
        return Vec3(v[0] / val, v[1] / val, v[2] / val);
    }
    Vec3& operator/=(const Vec3& pVec) {
        v[0] /= pVec.v[0];
        v[1] /= pVec.v[1];
        v[2] /= pVec.v[2];
        return *this;
    }
    Vec3& operator/=(const float val) {
        v[0] /= val;
        v[1] /= val;
        v[2] /= val;
        return *this;
    }
    float& operator[](const int index) {
        return v[index];
    }
    float length() const {
        return sqrtf(SQ(v[0]) + SQ(v[1]) + SQ(v[2]));
    }
    float lenthSquare() const {
        return SQ(v[0]) + SQ(v[1]) + SQ(v[2]);
    }
    Vec3 normalize() {
        float len = 1.0f / length();
        return Vec3(x * len, y * len, z * len);
    }
    float normalize_GetLength() {
        float _length = length();
        float len = 1.0f / _length;
        v[0] *= len; v[1] *= len; v[2] *= len;
        return _length;
    }
    float dot(const Vec3& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
    }
    Vec3 cross(const Vec3& v1) {
        return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
            v1.v[2] * v[0] - v1.v[0] * v[2],
            v1.v[0] * v[1] - v1.v[1] * v[0]);
    }
    void print() const {
        std::cout << '(' << v[0] << ',' << v[1] << ',' << v[2] << ')' << std::endl;
    }
    std::string print2str() const {
        return std::string('(' + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]) + ')');
    }
    Vec3 Max(const Vec3& v1, const Vec3& v2)
    {
        return Vec3(max(v1.v[0], v2.v[0]),
            max(v1.v[1], v2.v[1]),
            max(v1.v[2], v2.v[2]));
    }
    Vec3 Min(const Vec3& v1, const Vec3& v2)
    {
        return Vec3(min(v1.v[0], v2.v[0]),
            min(v1.v[1], v2.v[1]),
            min(v1.v[2], v2.v[2]));
    }
    float Max() const
    {
        return max(x, max(y, z));
    }
    float Min() const
    {
        return min(x, min(y, z));
    }
};
//vector functions
float dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
class Vec4 {
public:
    union {
        float v[4];
        struct { float x, y, z, w; };
    };

    Vec4() : x(0), y(0), z(0), w(0) {}//useless
    Vec4(float _x, float _y, float _z = 0, float _w = 0) : x(_x), y(_y), z(_z), w(_w) {}

    Vec4 operator+(const Vec4& pVec) const {
        return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
    }
    Vec4 operator+(const float val) const {
        return Vec4(v[0] + val, v[1] + val, v[2] + val, v[3] + val);
    }
    Vec4 operator-(const Vec4& pVec) const {
        return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], v[3] - pVec.v[3]);
    }
    Vec4 operator-(const float val) const {
        return Vec4(v[0] - val, v[1] - val, v[2] - val, v[3] - val);
    }
    Vec4 operator-() const {
        return Vec4(-v[0], -v[1], -v[2], -v[3]);
    }
    Vec4& operator+=(const Vec4& pVec) {
        v[0] += pVec.v[0];
        v[1] += pVec.v[1];
        v[2] += pVec.v[2];
        v[3] += pVec.v[3];
        return *this;
    }
    Vec4& operator+=(const float val) {
        v[0] += val;
        v[1] += val;
        v[2] += val;
        v[3] += val;
        return *this;
    }
    Vec4& operator-=(const Vec4& pVec) {
        v[0] -= pVec.v[0];
        v[1] -= pVec.v[1];
        v[2] -= pVec.v[2];
        v[3] -= pVec.v[3];
        return *this;
    }
    Vec4& operator-=(const float val) {
        v[0] -= val;
        v[1] -= val;
        v[2] -= val;
        v[3] -= val;
        return *this;
    }
    Vec4 operator*(const Vec4& pVec) const {
        return Vec4(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2], v[3] * pVec.v[3]);
    }
    Vec4 operator*(const float val) const {
        return Vec4(v[0] * val, v[1] * val, v[2] * val, v[3] * val);
    }
    Vec4& operator*=(const Vec4& pVec) {
        v[0] *= pVec.v[0];
        v[1] *= pVec.v[1];
        v[2] *= pVec.v[2];
        v[3] *= pVec.v[3];
        return *this;
    }
    Vec4& operator*=(const float val) {
        v[0] *= val;
        v[1] *= val;
        v[2] *= val;
        v[3] *= val;
        return *this;
    }
    Vec4 operator/(const Vec4& pVec) const {
        return Vec4(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2], v[3] / pVec.v[3]);
    }
    Vec4 operator/(const float val) const {
        return Vec4(v[0] / val, v[1] / val, v[2] / val, v[3] / val);
    }
    Vec4& operator/=(const Vec4& pVec) {
        v[0] /= pVec.v[0];
        v[1] /= pVec.v[1];
        v[2] /= pVec.v[2];
        v[3] /= pVec.v[3];
        return *this;
    }
    Vec4& operator/=(const float val) {
        v[0] /= val;
        v[1] /= val;
        v[2] /= val;
        v[3] /= val;
        return *this;
    }
    float& operator[](const int index) {
        return v[index];
    }
    float length() const {
        return sqrtf(SQ(v[0]) + SQ(v[1]) + SQ(v[2]) + SQ(v[3]));
    }
    float lenthSquare() const {
        return SQ(v[0]) + SQ(v[1]) + SQ(v[2]) + SQ(v[3]);
    }
    Vec4 normalize() const {
        float L = length();
        if (L == 0.0f) return Vec4(0, 0, 0, 0);
        float len = 1.0f / L;
        return Vec4(x * len, y * len, z * len, w * len);
    }
    float normalizeGetLength() {
        float _length = length();
        if (_length == 0.0f) return 0.0f;
        float len = 1.0f / _length;
        v[0] *= len; v[1] *= len; v[2] *= len; v[3] *= len;
        return _length;
    }
    float dot(const Vec4& pVec) const {
        return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2] + v[3] * pVec.v[3];
    }
    // Cross uses xyz, w set to 0
    Vec4 cross(const Vec4& v1) const {
        return Vec4(v1.v[1] * v[2] - v1.v[2] * v[1],
            v1.v[2] * v[0] - v1.v[0] * v[2],
            v1.v[0] * v[1] - v1.v[1] * v[0],
            0.0f);
    }
    void print() const {
        std::cout << '(' << v[0] << ',' << v[1] << ',' << v[2] << ',' << v[3] << ')' << std::endl;
    }
    std::string print2str() const {
        return std::string('(' + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' +
            std::to_string(v[2]) + ',' + std::to_string(v[3]) + ')');
    }
    Vec4 dividebyw() {
        if (w == 0) {
            std::cout << "w is 0!" << std::endl;
            return *this;
        }
        return Vec4(x / w, y / w, z / w, w);
    }
    Vec4 Max(const Vec4& v1, const Vec4& v2)
    {
        return Vec4(max(v1.v[0], v2.v[0]),
            max(v1.v[1], v2.v[1]),
            max(v1.v[2], v2.v[2]),
            max(v1.v[3], v2.v[3]));
    }
    Vec4 Min(const Vec4& v1, const Vec4& v2)
    {
        return Vec4(min(v1.v[0], v2.v[0]),
            min(v1.v[1], v2.v[1]),
            min(v1.v[2], v2.v[2]),
            min(v1.v[3], v2.v[3]));
    }
    float Max() const
    {
        return max(x, max(y, max(z, w)));
    }
    float Min() const
    {
        return min(x, min(y, min(z, w)));
    }
};

class Matrix {
public:
    union {
        float a[4][4];
        float m[16];
    };
    Matrix() {
        memset(m, 0, 16 * sizeof(float));
        a[0][0] = 1; a[1][1] = 1; a[2][2] = 1; a[3][3] = 1;
    }
    Matrix(float* _m) {
        m[0] = _m[0];   m[1] = _m[1];   m[2] = _m[2];   m[3] = _m[3];
        m[4] = _m[4];   m[5] = _m[5];   m[6] = _m[6];   m[7] = _m[7];
        m[8] = _m[8];   m[9] = _m[9];   m[10] = _m[10]; m[11] = _m[11];
        m[12] = _m[12]; m[13] = _m[13]; m[14] = _m[14]; m[15] = _m[15];
    }
    void print() const {
        for (int i = 0; i < 4; i++) {
            std::cout << '|' << '\t';
            for (int j = 0; j < 4; j++) {
                std::cout << a[i][j] << '\t';
            }
            std::cout << '|' << std::endl;
        }
    }
    Vec4 mul(const Vec4& v) {
        return Vec4(
            v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3],
            v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7],
            v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11],
            v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]);
    }
    Vec3 mulPoint(const Vec3& v) {
        Vec3 v1 = Vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
            (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
            (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
    }
    Vec3 mulVec(const Vec3& v) {
        return Vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]),
            (v.x * m[4] + v.y * m[5] + v.z * m[6]),
            (v.x * m[8] + v.y * m[9] + v.z * m[10]));
    }
    Matrix mul(const Matrix& matrix) const {
        Matrix ret;
        ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
        ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
        ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
        ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
        ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
        ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
        ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
        ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
        ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
        ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
        ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
        ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
        ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
        ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
        ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
        ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
        return ret;
    }
    float determinant() {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        return det;
    }
    Matrix invert() const {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0) {
            // Handle this case 
            std::cout << "det = 0!!" << std::endl;
        }
        det = 1.0f / det;
        for (int i = 0; i < 16; i++) {
            inv[i] = inv[i] * det;
        }
        return inv;
    }
    Matrix transpose() const {
        Matrix inv;
        inv[0] = m[0];  inv[1] = m[4];  inv[2] = m[8];  inv[3] = m[12];
        inv[4] = m[1];  inv[5] = m[5];  inv[6] = m[9];  inv[7] = m[13];
        inv[8] = m[2];  inv[9] = m[6];  inv[10] = m[10]; inv[11] = m[14];
        inv[12] = m[3]; inv[13] = m[7]; inv[14] = m[11]; inv[15] = m[15];
        return inv;
    }
    static Matrix projectionMatrix(float fov, float aspect, float _near, float _far) {
        Matrix proM;
        memset(proM.m, 0, 16 * sizeof(float));
        proM.a[0][0] = 1 / (aspect * (tan(fov / 2)));//no pi
        proM.a[1][1] = 1 / (tan(fov / 2));
        proM.a[2][2] = _far / (_far - _near);
        proM.a[2][3] = -(_far * _near) / (_far - _near);
        proM.a[3][2] = 1;
        return proM;
    }
    static Matrix lookAtMatrix(const Vec4& from, const Vec4& to, const Vec4& up ) {
        Matrix lookat;
        memset(lookat.m, 0, 16 * sizeof(float));
        Vec4 dir = (to - from) / (to - from).length();
        Vec4 right = up.cross(dir);//only cross x,y,z. 
        Vec4 realUp = dir.cross(right);//real up vector
         
        lookat.a[0][0] = right.x;  lookat.a[0][1] = right.y;  lookat.a[0][2] = right.z;  lookat.a[0][3] = -(from.dot(right));
        lookat.a[1][0] = realUp.x; lookat.a[1][1] = realUp.y; lookat.a[1][2] = realUp.z; lookat.a[1][3] = -(from.dot(realUp));
        lookat.a[2][0] = dir.x;    lookat.a[2][1] = dir.y;    lookat.a[2][2] = dir.z;    lookat.a[2][3] = -(from.dot(dir));
        lookat.a[3][3] = 1;
        return lookat;

    }
    float& operator[](const int index) {
        return m[index];
    }
    Matrix operator*(const Matrix& matrix) {
        return mul(matrix);
    }
};
//use pinhole camera
Matrix proM;//The ProjectionMatrix of this camera
float _near = 0.1;
float _far = 100;

//translation
Vec4 Translation(const Vec4& v, float x, float y, float z) {
    Matrix m;
    m.m[3] = x; m.m[7] = y; m.m[11] = z;
    return m.mul(v);
}
//rotation
Vec4 RotateX(const Vec4& v, float theta) {
    return Vec4(
        (v.x + v.y * 0 + v.z * 0),
        (v.y * cos(theta) - v.z * sin(theta)),
        (v.y * sin(theta) + v.z * cos(theta)),
        v.w);
}
Vec4 RotateY(const Vec4& v, float theta) {
    return Vec4(
        (v.x * cos(theta) + v.y * 0 + v.z * sin(theta)),
        (v.x * 0 + v.y * 1 + v.z * 0),
        (-v.x * sin(theta) + v.y * 0 + v.z * cos(theta)),
        v.w);
}
Vec4 RotateZ(const Vec4& v, float theta) {
    return Vec4(
        (v.x * cos(theta) - v.y * sin(theta) + v.z * 0),
        (v.x * sin(theta) + v.y * cos(theta) + v.z * 0),
        (v.x * 0 + v.y * 0 + v.z * 1),
        v.w);
}
//scaling
Vec4 Scaling(const Vec4& v, float x, float y, float z) {
    Matrix m;
    m.m[0] = x; m.m[5] = y; m.m[10] = z;
    return m.mul(v);
}
class SphericalCoordinates {
public:
    float theta;
    float phi;
    float r;
    SphericalCoordinates(Vec3 v, bool isZup = true) {//z up, for shading
        if (isZup) {
            theta = acosf(v.z);
            phi = atan2f(v.y, v.x);
            r = 1;
        }
        else {
            theta = acosf(v.y);
            phi = atan2f(v.z, v.x);
            r = 1;
        }
    }
    Vec3 convertZup() const {
        return Vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
    }
    Vec3 convertYup() const {
        return Vec3(r * sin(theta) * cos(phi), r * cos(theta), r * sin(theta) * sin(phi));
    }
};


class Quaternions {
public:
    union {//q = d + ai + bj + ck
        struct {
            float a;
            float b;
            float c;
            float d;
        };
        float q[4];
    };
    Quaternions() : a(0), b(0), c(0), d(0) {}
    Quaternions(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}
    Quaternions(float theta, Vec3 dir) {
        d = cosf(theta / 2);
        dir = dir.normalize();
        a = dir.x * sinf(theta / 2);
        b = dir.y * sinf(theta / 2);
        c = dir.z * sinf(theta / 2);
    }
    Matrix toMatrix() const {
        float aa = a * a, ab = a * b, ac = a * c;
        float bb = b * b, bc = b * c, cc = c * c;
        float da = d * a, db = d * b, dc = d * c;
        Matrix m;
        m[0] = 1 - 2 * (bb + cc); m[1] = 2 * (ab - dc); m[2] = 2 * (ac + db); m[3] = 0;
        m[4] = 2 * (ab + dc); m[5] = 1 - 2 * (aa + cc); m[6] = 2 * (bc - da); m[7] = 0;
        m[8] = 2 * (ac - db); m[9] = 2 * (bc + da); m[10] = 1 - 2 * (aa + bb); m[11] = 0;
        m[12] = m[13] = m[14] = 0; m[15] = 1;
        return m;
    }
    float magnitude() const {
        return sqrtf(SQ(a) + SQ(b) + SQ(c) + SQ(d));
    }
    float magnitudeSquare() const {
        return SQ(a) + SQ(b) + SQ(c) + SQ(d);
    }
    void normalize() {
        float m = magnitude();
        *this = *this / m;
    }
    Quaternions conjugate() const {
        return Quaternions(-a, -b, -c, d);
    }
    Quaternions inverse() const {
        float m2 = magnitudeSquare();
        return Quaternions(-a / m2, -b / m2, -c / m2, d / m2);
    }
    Quaternions slerp(const Quaternions& q, float t) const {
        Quaternions res;
        if (t < 0 || t > 1) {
            std::cout << "slerp error, invalid t!!" << std::endl;
            return res;
        }
        float theta = findAngle(q);
        //std::cout << cosf(theta) << std::endl;
        float q1_co = sinf(theta * (1 - t)) / sinf(theta);
        float q2_co = sinf(theta * t) / sinf(theta);
        res.a = q1_co * a + q2_co * q.a;
        res.b = q1_co * b + q2_co * q.b;
        res.c = q1_co * c + q2_co * q.c;
        res.d = q1_co * d + q2_co * q.d;
        return res;
    }
    Vec3 pointRotate(const Vec3& v) {
        Quaternions p(v.v[0], v.v[1], v.v[2], 0);
        p = *this * p * conjugate();
        return Vec3(p.a, p.b, p.c);
    }
    float dot(const Quaternions& q) const {
        return a * q.a + b * q.b + c * q.c + d * q.d;
    }
    float findAngle(const Quaternions& q) const {
        return acosf(dot(q));
    }
    Quaternions operator+(const Quaternions& q) const {
        return Quaternions(a + q.a, b + q.b, c + q.c, d + q.d);
    }
    Quaternions operator+(const float val) const {
        return Quaternions(a + val, b + val, c + val, d + val);
    }
    Quaternions operator-(const Quaternions& q) const {
        return Quaternions(a - q.a, b - q.b, c - q.c, d - q.d);
    }
    Quaternions operator-(const float val) const {
        return Quaternions(a - val, b - val, c - val, d - val);
    }
    Quaternions operator-() const {
        return Quaternions(-a, -b, -c, -d);
    }
    Quaternions operator*(const Quaternions& q) const {
        return Quaternions(d * q.a + a * q.d + b * q.c - c * q.b,
            d * q.b - a * q.c + b * q.d + c * q.a,
            d * q.c + a * q.b - b * q.a + c * q.d,
            d * q.d - a * q.a - b * q.b - c * q.c);
    }
    Quaternions operator/(const float val) const {
        return Quaternions(a / val, b / val, c / val, d / val);
    }
    void print() const {
        std::cout << '(' << a << ',' << b << ',' << c << ',' << d << ')' << std::endl;
    }
};
class Colour {
public:
    float r, g, b, a;
    Colour(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 0.f) : r(_r), g(_g), b(_b), a(_a) {}
    Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
        : r(_r / 255.0f), g(_g / 255.0f), b(_b / 255.0f), a(_a / 255.0f) {
    }
    Colour operator+(const Colour& c) const {
        return Colour(c.r + r, c.g + g, c.b + b, c.a + a);
    }
    Colour operator*(const Colour& c) const {
        return Colour(c.r * r, c.g * g, c.b * b, c.a * a);
    }
    Colour operator*(const float _a) const {
        return Colour(r * _a, g * _a, b * _a, a * _a);
    }
    Colour operator/(const float _a) const {
        return Colour(r / _a, g / _a, b / _a, a / _a);
    }
};

class ShadingFrame {
public:
    union {
        Vec3 v[3];
        struct { Vec3 i, j, k; };
        float m[3][3];
    };
    ShadingFrame(Vec3 _v) {
        _v = _v.normalize();
        //create vectors linearly independent
        Vec3 v1, v2;
        do {
            v1 = randomVector();
            v2 = randomVector();
        } while (fabs(determinant3x3(_v, v1, v2)) < 1e-9);
        //Gram-Schmidt
        v[0] = _v;
        v[1] = v1 - (v[0] * (v1.dot(v[0]) / v[0].dot(v[0])));
        v[2] = v2 - (v[0] * (v2.dot(v[0]) / v[0].dot(v[0])));
        v[2] = v[2] - (v[1] * (v[2].dot(v[1]) / v[1].dot(v[1])));
        //normlize        
        v[1] = v[1].normalize();
        v[2] = v[2].normalize();
    }
    float determinant3x3(Vec3& v0, Vec3& v1, Vec3& v2) {
        return v0[0] * (v1[1] * v2[2] - v1[2] * v2[1])
            - v0[1] * (v1[0] * v2[2] - v1[2] * v2[0])
            + v0[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
    }
    Vec3 randomVector() {
        return Vec3(static_cast<float>(rand() % 10 + 1), 
            static_cast<float>(rand() % 10 + 1),
            static_cast<float>(rand() % 10 + 1));
    }
    void print() const {
        std::cout << "orthonormal basis:" << std::endl;
        v[0].print();
        v[1].print();
        v[2].print();
    }
};
template<typename t>
t simpleInterpolateAttribute(t a0, t a1, t a2, float alpha, float beta, float gamma) {
    return (a0 * alpha) + (a1 * beta) + (a2 * gamma);
}
template<typename t>
t perspectiveCorrectInterpolateAttribute(t a0, t a1, t a2, float v0_w, float v1_w, float v2_w,
    float alpha, float beta, float gamma, float frag_w)
{
    t attrib[3];
    attrib[0] = a0 * alpha * v0_w;
    attrib[1] = a1 * beta * v1_w;
    attrib[2] = a2 * gamma * v2_w;
    return ((attrib[0] + attrib[1] + attrib[2]) / frag_w);
}


class Zbuffer {
private:
    float buffer[canvasWidth][canvasHeight];
public:
    Zbuffer() {
        init();
    }
    void init() {
        for (int i = 0; i < canvasWidth; i++)
            for(int j = 0; j < canvasHeight; j++)
                buffer[i][j] = 1.0f;
    }
    bool zbufferUpdate(const int x, const int y, const float z) {
        if (z > 0.0f && z < buffer[x][y]) {
            buffer[x][y] = z;
            return true;
        }
        else
            return false;
    }
};
Zbuffer zb;

class Triangle {
public:
    union {
        Vec4 v[3];
        struct {
            Vec4 a, b, c;
        };
    };
    Vec4 tr, bl;
    Vec3 normal;
    Triangle(Vec4 _a, Vec4 _b, Vec4 _c) : a(_a), b(_b), c(_c) {}
    float bary() {
        float projArea = 0;
        projArea = fabs((v[1].x - v[0].x) * (v[2].y - v[0].y) - (v[2].x - v[0].x) * (v[1].y - v[0].y));
        return projArea;
    }
    float areaComputing() {
        float projArea = bary();
        float area = 1.0f / projArea;
        return area;
    }
    //return this triangle's normal vector and normalize it
    Vec3 findNormal() {
        Vec3 v1(v[0].x, v[0].y, v[0].z);
        Vec3 v2(v[1].x, v[1].y, v[1].z);
        Vec3 v3(v[2].x, v[2].y, v[2].z);
        return ((v2 - v1).cross(v3 - v1).normalize());
    }
    float edgeFunction(const Vec4& v0, const Vec4& v1, const Vec4& p)//check if point in the area
    {
        return (((p.x - v0.x) * (v1.y - v0.y)) - ((v1.x - v0.x) * (p.y - v0.y)));
    }
    //no using clip, the bounding box may be larger if the triangle out of the screen
    void findBounds()
    {
        tr.x = min(max(max(v[0].x, v[1].x), v[2].x), canvasWidth - 1);
        tr.y = min(max(max(v[0].y, v[1].y), v[2].y), canvasHeight - 1);
        bl.x = max(min(min(v[0].x, v[1].x), v[2].x), 0);
        bl.y = max(min(min(v[0].y, v[1].y), v[2].y), 0);
    }
    //using sutherlanHodgmanClip, return bounding box
    void findBoundsClip(const std::vector<Vec4>& points) {
        tr.x = 0;
        tr.y = 0;
        bl.x = canvasWidth - 1;
        bl.y = canvasHeight - 1;
        for (auto p : points) {
            tr.x = max(tr.x, p.x);
            tr.y = max(tr.y, p.y);
            bl.x = min(bl.x, p.x);
            bl.y = min(bl.y, p.y);
        }
        tr.x = min(tr.x, canvasWidth - 1);
        tr.y = min(tr.y, canvasHeight - 1);
        bl.x = max(bl.x, 0);
        bl.y = max(bl.y, 0);
    }
    Vec4 intersection(const Vec4& v1, const Vec4& v2, const Vec4& p1, const Vec4& p2) {
        //return v1-v2, a-b intersection
        float A1 = v2.y - v1.y;
        float B1 = v1.x - v2.x;
        float C1 = A1 * v1.x + B1 * v1.y;

        float A2 = p2.y - p1.y;
        float B2 = p1.x - p2.x;
        float C2 = A2 * p1.x + B2 * p2.y;

        float det = A1 * B2 - A2 * B1;
        Vec4 inter;
        inter.x = (B2 * C1 - B1 * C2) / det;
        inter.y = (A1 * C2 - A2 * C1) / det;
        inter.w = 1;
        return inter;
    }
    Vec4 intersection_Plane(const Vec4& v1, const Vec4& v2, const float planeDis) {
        float t = (planeDis - v1.z) / (v2.z - v1.z);
        return Vec4(v1.x + t * (v2.x - v1.x), v1.y + t * (v2.y - v1.y), planeDis, 1);
    }
    std::vector<Vec4> sutherlanHodgmanClip(const std::vector<Vec4>& vertexes, float planeDis, bool isFar = true) {
        std::vector<Vec4> ret;
        for (int i = 0; i < vertexes.size(); i++) {
            Vec4 cur = vertexes[i];
            Vec4 next = vertexes[(i + 1) % vertexes.size()];
            bool curInside = isFar ? cur.z <= planeDis : cur.z >= planeDis;
            bool nextInside = isFar ? next.z <= planeDis : next.z >= planeDis;
            if (curInside && nextInside) {
                ret.push_back(next);
            }
            else if (curInside && !nextInside) {
                ret.push_back(intersection_Plane(cur, next, planeDis));
            }
            else if (!curInside && nextInside) {
                ret.push_back(intersection_Plane(cur, next, planeDis));
                ret.push_back(next);
            }
        }
        return ret;
    }
    std::vector<Vec4> sutherlanHodgmanClip_Screen(const std::vector<Vec4>& vertexes) {//can use screen size
        std::vector<Vec4> clipPolygon = { {0, 0}, {0, canvasHeight - 1}, {canvasWidth - 1, canvasHeight - 1}, {canvasWidth - 1, 0} };
        //std::vector<Vec4> ret = {v[0], v[1], v[2]};
        std::vector<Vec4> ret = vertexes;
        for (int i = 0; i < clipPolygon.size(); i++) {
            std::vector<Vec4> input = ret;
            ret.clear();

            Vec4 A = clipPolygon[i];
            Vec4 B = clipPolygon[(i + 1) % clipPolygon.size()];

            for (int j = 0; j < input.size(); j++) {
                Vec4 P = input[j];
                Vec4 Q = input[(j + 1) % input.size()];

                bool P_inside = edgeFunction(A, B, P) >= 0 ? true : false;
                bool Q_inside = edgeFunction(A, B, Q) >= 0 ? true : false;


                if (P_inside && Q_inside) {
                    ret.push_back(Q);
                }
                else if (P_inside && !Q_inside) {
                    ret.push_back(intersection(P, Q, A, B));
                }
                else if (!P_inside && Q_inside) {
                    ret.push_back(intersection(P, Q, A, B));
                    ret.push_back(Q);
                }
            }
        }
        return ret;
    }
    void draw(GamesEngineeringBase::Window& canvas, Colour colour) {
        // classic bounds
        //findBounds(); 
        
        //clip by near plane and far plane should be here(before transforming to screen)
        std::vector<Vec4> vertexes = { v[0], v[1], v[2] };
        vertexes = sutherlanHodgmanClip(vertexes, _near, false);
        vertexes = sutherlanHodgmanClip(vertexes, _far, true);
        for (auto& v : vertexes) {
            v = proM.mul(v).dividebyw();
            v.x = ((v.x + 1) / 2) * canvasWidth; v.y = canvasHeight - ((v.y + 1) / 2) * canvasHeight;//make Y axis is up-towards in the screen(because Y axis is down in the screen)
        }
        //bounds after clipping screen(it does work when triangle out of the screen)
        vertexes = sutherlanHodgmanClip_Screen(vertexes);
        
        findBoundsClip(vertexes);

        v[0] = proM.mul(v[0]).dividebyw();
        v[1] = proM.mul(v[1]).dividebyw();
        v[2] = proM.mul(v[2]).dividebyw();

        v[0].x = ((v[0].x + 1) / 2) * canvasWidth; v[0].y = canvasHeight - ((v[0].y + 1) / 2) * canvasHeight;//make Y axis is up-towards in the screen(because Y axis is down in the screen)
        v[1].x = ((v[1].x + 1) / 2) * canvasWidth; v[1].y = canvasHeight - ((v[1].y + 1) / 2) * canvasHeight;
        v[2].x = ((v[2].x + 1) / 2) * canvasWidth; v[2].y = canvasHeight - ((v[2].y + 1) / 2) * canvasHeight;

        float area = areaComputing();
        for (int y = (int)bl.y; y < (int)tr.y + 1; y++) {
            for (int x = (int)bl.x; x < (int)tr.x + 1; x++) {
                Vec4 p(x + 0.5f, y + 0.5f, 0);
                // Compute triangle here
                float alpha = edgeFunction(v[1], v[2], p);
                float beta = edgeFunction(v[2], v[0], p);
                float gamma = edgeFunction(v[0], v[1], p);

                alpha *= area;
                beta *= area;
                gamma *= area;

                if ((alpha > 0 && beta > 0 && gamma > 0)|| (alpha < 0 && beta < 0 && gamma < 0)) {
                    //zbuffer
                    p.z = fabs(alpha * v[0].z + beta * v[1].z + gamma * v[2].z);//fabs: sometimes the alpha beta and gamma are all nagetive
                    //std::cout << p.z << std::endl;
                    if (zb.zbufferUpdate(x, y, p.z)) {
                        //simple interpolate attribute, may cause streching
                        //Colour frag = simpleInterpolateAttribute(Colour(1.0f, 0, 0), Colour(0, 1.0f, 0), Colour(0, 0, 1.0f), fabs(alpha), fabs(beta), fabs(gamma));
                        
                        //using w for correcting barycentric coordinates
                        /*float frag_w = ((alpha * v[0].w) + (beta * v[1].w) + (gamma * v[2].w));
                        Colour frag = perspectiveCorrectInterpolateAttribute(Colour(1.0f, 0, 0), Colour(0, 1.0f, 0), Colour(0, 0, 1.0f), v[0].w, v[1].w, v[2].w,
                            alpha, beta, gamma, frag_w);
                        canvas.draw(x, y, frag.r * 255, frag.g * 255, frag.b * 255);*/

                        //using constant colour
                        canvas.draw(x, y, colour.r * 255, colour.g * 255, colour.b * 255);
                    }
                    
                }
            }
        }
    }
    void print() {
        std::cout << v[0].print2str() << v[1].print2str() << v[2].print2str() << std::endl;
    }
    Vec4& operator[](int index) {
        return v[index];
    }
};

class Draw3D {
public:
    Draw3D(){}
    static void drawByVerteces(GamesEngineeringBase::Window& canvas, const std::vector<Vec4>& verteces, const Vec4& offset, Matrix& lookupM) {
        if (verteces.size() % 3 != 0) {//triangle
            std::cout << "verteces num wrong!" << std::endl;
            return;
        }
        for (int i = 0; i < verteces.size() / 3; i++) {
            Vec4 v[3] = { verteces[i * 3], verteces[i * 3 + 1],verteces[i * 3 + 2] };
            Triangle tri(lookupM.mul(v[0]) + offset, lookupM.mul(v[1]) + offset, lookupM.mul(v[2]) + offset);
            tri.draw(canvas, diffusionLignt(tri));
        }
    
    }
    static Colour diffusionLignt(Triangle& tri) {
        Vec3 omega_i = Vec3(1, 1, 0).normalize();
        Vec3 N = tri.findNormal();
        Colour rho(0, 1.0f, 0);
        Colour L(1.0f, 1.0f, 1.0f);
        Colour ambient(0.5f, 0.5f, 0.5f);
        return (rho / M_PI) * (L * max(omega_i.dot(N), 0.0f) + ambient);
    }
};

//lab



//lab end

int main() {
    GamesEngineeringBase::Window canvas;
    canvas.create(canvasWidth, canvasHeight, "Example");
    bool running = true;
    //Set Matrix proM;
    float aspect = static_cast<float>(canvasWidth) / canvasHeight;
    proM = Matrix::projectionMatrix(M_PI / 4, aspect, _near, _far);

    /*Matrix lookatM;
    lookatM.toLookAtMatrix(Vec4(0,0,-10,1), Vec4(0, 0, 1, 1), Vec4(0, 1, 0, 1));
    lookatM.print();*/
    //lab
    Quaternions q1(0, Vec3(0, 1, 0));
    Quaternions q2(M_PI * 2 - 0.01f, Vec3(0, 1, 0));

    

    std::vector<GEMLoader::GEMMesh> gemmeshes;
    GEMLoader::GEMModelLoader loader;
    loader.load("Resources/bunny.gem", gemmeshes);
    std::vector<Vec4> vertexList;
    for (int i = 0; i < gemmeshes.size(); i++) {
        
        for (int j = 0; j < gemmeshes[i].indices.size(); j++) {
            GEMLoader::GEMVec3 vec;
            int index = gemmeshes[i].indices[j];
            vec = gemmeshes[i].verticesStatic[index].position;
            vertexList.push_back(Vec4(vec.x, vec.y, vec.z, 1));
        }
    }

    Matrix viewM;
    float radius = 0.5f;
    float t = 0.0f;
    //lab end
    while (running)
    {
        // Check for input (key presses or window events)
        canvas.checkInput();

        // Clear the window for the next frame rendering
        canvas.clear();
        zb.init();
        //lookat matrix
        t += 0.05;
        Vec4 from = Vec4(radius * cosf(t), 0, radius * sinf(t), 1);

        viewM = Matrix::lookAtMatrix(from, Vec4(0, 0, 0, 1), Vec4(0, 1, 0, 1));

        Vec4 offset(0, -0.1, 0, 0);
        Draw3D::drawByVerteces(canvas, vertexList, offset, viewM);
        
        // Display the frame on the screen. This must be called once the frame is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}
