#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "HeaderFiles/window.h"
#include "HeaderFiles/Core.h"
#include "HeaderFiles/GamesEngineeringBase.h"
#include "HeaderFiles/GEMLoader.h"

#define SQ(x) ((x)*(x))

static const int canvasWidth = 1024;
static const int canvasHeight = 1024;

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
    Vec3 cross(const Vec3& v1) const {
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
    static Matrix lookAtMatrix(const Vec3& from, const Vec3& to, const Vec3& up) {
        Matrix lookat;
        memset(lookat.m, 0, 16 * sizeof(float));
        Vec3 dir = (to - from) / (to - from).length();
        Vec3 right = up.cross(dir);//only cross x,y,z. 
        Vec3 realUp = dir.cross(right);//real up vector

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
Matrix proM;//The ProjectionMatrix of this camera
float _near = 0.1;
float _far = 1000;
struct PRIM_VERTEX
{
    Vec3 position;
    Colour colour;
};
struct STATIC_VERTEX
{
    Vec3 pos;
    Vec3 normal;
    Vec3 tangent;
    float tu;
    float tv;
};
//------------------------------
class VertexLayoutCache
{
public:
    static const D3D12_INPUT_LAYOUT_DESC& getStaticLayout() {
        static const D3D12_INPUT_ELEMENT_DESC inputLayoutStatic[] = {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };
        static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutStatic, 4 };
        return desc;
    }


};
class GeneralMesh {
public:
    ID3D12Resource* vertexBuffer;
    ID3D12Resource* indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_INDEX_BUFFER_VIEW ibView;
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
    unsigned int numMeshIndices;

    void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices) {
        init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
        inputLayoutDesc = VertexLayoutCache::getStaticLayout();
    }
    void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices,
        unsigned int* indices, int numIndices) {
        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vbDesc = {};
        vbDesc.Width = numVertices * vertexSizeInBytes;
        vbDesc.Height = 1;
        vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vbDesc.DepthOrArraySize = 1;
        vbDesc.MipLevels = 1;
        vbDesc.SampleDesc.Count = 1;
        vbDesc.SampleDesc.Quality = 0;
        vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
        core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vbView.StrideInBytes = vertexSizeInBytes;
        vbView.SizeInBytes = numVertices * vertexSizeInBytes;

        D3D12_RESOURCE_DESC ibDesc;
        memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
        ibDesc.Width = numIndices * sizeof(unsigned int);
        ibDesc.Height = 1;
        ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        ibDesc.DepthOrArraySize = 1;
        ibDesc.MipLevels = 1;
        ibDesc.SampleDesc.Count = 1;
        ibDesc.SampleDesc.Quality = 0;
        ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        HRESULT hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
        core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
            D3D12_RESOURCE_STATE_INDEX_BUFFER);

        ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R32_UINT;
        ibView.SizeInBytes = numIndices * sizeof(unsigned int);
        numMeshIndices = numIndices;

    }
    void draw(Core* core) {
        core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
        core->getCommandList()->IASetIndexBuffer(&ibView);
        core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
    }

};
class Mesh {//simple mesh class
public:
    ID3D12Resource* vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vbView;

    void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices) {
        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC vbDesc = {};
        vbDesc.Width = numVertices * vertexSizeInBytes;
        vbDesc.Height = 1;
        vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        vbDesc.DepthOrArraySize = 1;
        vbDesc.MipLevels = 1;
        vbDesc.SampleDesc.Count = 1;
        vbDesc.SampleDesc.Quality = 0;
        vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
            D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
        core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
        vbView.StrideInBytes = vertexSizeInBytes;
        vbView.SizeInBytes = numVertices * vertexSizeInBytes;
    }

    void draw(Core* core) {
        core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
        core->getCommandList()->DrawInstanced(3, 1, 0, 0);
    }

};
class WorldCube {//create a simple cube by using world axis
public:
    GeneralMesh mesh;
    void init(Core* core, Vec3 offset = Vec3(0, 0, 0)) {
        std::vector<STATIC_VERTEX> vertices;
        Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f) + offset;
        Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f) + offset;
        Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f) + offset;
        Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f) + offset;
        Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f) + offset;
        Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f) + offset;
        Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f) + offset;
        Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f) + offset;

        vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
        vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));
        vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

        vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
        vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
        vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
        vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
        vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
        vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

        std::vector<unsigned int> indices;
        indices.push_back(0); indices.push_back(1); indices.push_back(2);
        indices.push_back(0); indices.push_back(2); indices.push_back(3);
        indices.push_back(4); indices.push_back(5); indices.push_back(6);
        indices.push_back(4); indices.push_back(6); indices.push_back(7);
        indices.push_back(8); indices.push_back(9); indices.push_back(10);
        indices.push_back(8); indices.push_back(10); indices.push_back(11);
        indices.push_back(12); indices.push_back(13); indices.push_back(14);
        indices.push_back(12); indices.push_back(14); indices.push_back(15);
        indices.push_back(16); indices.push_back(17); indices.push_back(18);
        indices.push_back(16); indices.push_back(18); indices.push_back(19);
        indices.push_back(20); indices.push_back(21); indices.push_back(22);
        indices.push_back(20); indices.push_back(22); indices.push_back(23);

        mesh.init(core, vertices, indices);

    }
    STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv) {
        STATIC_VERTEX v;
        v.pos = p;
        v.normal = n;
        v.tangent = Vec3(0, 0, 0); // For now
        v.tu = tu;
        v.tv = tv;
        return v;
    }
};
class PSOManager {
public:
    std::unordered_map<std::string, ID3D12PipelineState*> psos;

    void createPSO(Core* core, std::string name, ID3DBlob* vs, ID3DBlob* ps, D3D12_INPUT_LAYOUT_DESC layout) {
        //Avoid creating extra state
        if (psos.find(name) != psos.end()) return;
        //Configure GPU pipeline with shaders, layout and Root Signature
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.InputLayout = layout;
        desc.pRootSignature = core->rootSignature;
        desc.VS = { vs->GetBufferPointer(), vs->GetBufferSize() };
        desc.PS = { ps->GetBufferPointer(), ps->GetBufferSize() };
        //Responsible for configuring the rasterizer
        D3D12_RASTERIZER_DESC rasterDesc = {};
        rasterDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterDesc.CullMode = D3D12_CULL_MODE_NONE;
        rasterDesc.FrontCounterClockwise = FALSE;
        rasterDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        rasterDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterDesc.DepthClipEnable = TRUE;
        rasterDesc.MultisampleEnable = FALSE;
        rasterDesc.AntialiasedLineEnable = FALSE;
        rasterDesc.ForcedSampleCount = 0;
        rasterDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        desc.RasterizerState = rasterDesc;
        //Responsible for configuring the depth buffer
        D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        depthStencilDesc.StencilEnable = FALSE;
        desc.DepthStencilState = depthStencilDesc;
        //Blend State
        D3D12_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = FALSE;
        blendDesc.IndependentBlendEnable = FALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlend = {
        FALSE, FALSE,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL
        };
        for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
            blendDesc.RenderTarget[i] = defaultRenderTargetBlend;
        }
        desc.BlendState = blendDesc;
        //Render Target State + Topology
        desc.SampleMask = UINT_MAX;
        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count = 1;
        //Create Pipeline State Object
        ID3D12PipelineState* pso;
        HRESULT hr = core->device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pso));
        //Insert into map
        psos.insert({ name, pso });
    }
    void bind(Core* core, std::string name) {
        core->getCommandList()->SetPipelineState(psos[name]);
    }

};
struct ConstantBufferVariable
{
    unsigned int offset;
    unsigned int size;
};
class ConstantBuffer {
public:
    ID3D12Resource* constantBuffer;
    unsigned char* buffer;
    unsigned int cbSizeInBytes;

    std::string name;
    std::map<std::string, ConstantBufferVariable> constantBufferData;

    unsigned int maxDrawCalls;
    unsigned int offsetIndex;

    void init(Core* core, unsigned int sizeInBytes, unsigned int _maxDrawCalls = 1024) {
        cbSizeInBytes = (sizeInBytes + 255) & ~255;
        maxDrawCalls = _maxDrawCalls;
        unsigned int cbSizeInBytesAligned = cbSizeInBytes * maxDrawCalls;
        offsetIndex = 0;
        HRESULT hr;
        D3D12_HEAP_PROPERTIES heapprops = {};
        heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
        heapprops.CreationNodeMask = 1;
        heapprops.VisibleNodeMask = 1;
        D3D12_RESOURCE_DESC cbDesc = {};
        cbDesc.Width = cbSizeInBytesAligned;
        cbDesc.Height = 1;
        cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        cbDesc.DepthOrArraySize = 1;
        cbDesc.MipLevels = 1;
        cbDesc.SampleDesc.Count = 1;
        cbDesc.SampleDesc.Quality = 0;
        cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL,
            IID_PPV_ARGS(&constantBuffer));
        constantBuffer->Map(0, NULL, (void**)&buffer);
    }
    void update(std::string name, void* data) {
        ConstantBufferVariable cbVariable = constantBufferData[name];
        unsigned int offset = offsetIndex * cbSizeInBytes;
        memcpy(&buffer[offset + cbVariable.offset], data, cbVariable.size);
    }
    D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress() const {
        return (constantBuffer->GetGPUVirtualAddress() + (offsetIndex * cbSizeInBytes));
    }
    void next() {
        offsetIndex++;
        if (offsetIndex >= maxDrawCalls) {
            offsetIndex = 0;
        }
    }
};
enum shaderTypes { VS, PS };
class Shader {
public:
    std::string name;
    ID3DBlob* shader;

    std::map<std::string, ConstantBuffer> constantBuffers;//map is better

    std::string readShaderFile(std::string fileName) {
        std::ifstream file(fileName);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    void compileShader(std::string fileName, shaderTypes shaderType, std::string shaderName) {
        ID3DBlob* status;
        std::string shaderStr = readShaderFile(fileName);
        name = shaderName;
        HRESULT hr;
        switch (shaderType) {
        case VS:
            hr = D3DCompile(shaderStr.c_str(), strlen(shaderStr.c_str()), NULL, NULL, NULL, "VS", "vs_5_0", 0, 0, &shader, &status);
            if (FAILED(hr)) {
                (char*)status->GetBufferPointer();
            }
            break;
        case PS:
            hr = D3DCompile(shaderStr.c_str(), strlen(shaderStr.c_str()), NULL, NULL, NULL, "PS", "ps_5_0", 0, 0, &shader, &status);
            if (FAILED(hr)) {
                (char*)status->GetBufferPointer();
            }
            break;
        }
    }

    void getConstantBuffer() {
        ID3D12ShaderReflection* reflection;
        D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
        D3D12_SHADER_DESC desc;
        reflection->GetDesc(&desc);
        //read constant buffers in shader (may have more than 1 constant buffers)
        for (int i = 0; i < desc.ConstantBuffers; i++) {
            ConstantBuffer buffer;
            ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
            D3D12_SHADER_BUFFER_DESC cbDesc;
            constantBuffer->GetDesc(&cbDesc);
            unsigned int totalSize = 0;
            for (int j = 0; j < cbDesc.Variables; j++) {
                ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
                D3D12_SHADER_VARIABLE_DESC vDesc;
                var->GetDesc(&vDesc);
                ConstantBufferVariable bufferVariable;
                bufferVariable.offset = vDesc.StartOffset;
                bufferVariable.size = vDesc.Size;
                buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
                totalSize += bufferVariable.size;
            }
            //add
            buffer.name = cbDesc.Name;
            buffer.cbSizeInBytes = totalSize;
            constantBuffers.insert(std::pair<std::string, ConstantBuffer>(buffer.name, buffer));
        }
    }
};

class ShaderManager {
public:
    std::map<std::string, Shader> shaders;
    void init() {
        loadShader("VertexShader", VS);
        loadShader("PixelShader", PS);
    }

    void loadShader(std::string shaderName, shaderTypes shaderType) {
        Shader shader;
        shader.name = shaderName;
        //find and complile
        std::string foldPath = "Shaders/";
        std::string fileName = foldPath + shaderName + ".cso";
        std::wstring wideName = std::wstring(fileName.begin(), fileName.end());
        std::ifstream compiledFile(fileName);
        if (compiledFile.is_open()) {//check if there is compiled shader code
            D3DReadFileToBlob(wideName.c_str(), &shader.shader);
        }
        else {//if not, generate a .cso file
            shader.compileShader(std::string(foldPath + shaderName + ".hlsl"), shaderType, shaderName);
            D3DWriteBlobToFile(shader.shader, wideName.c_str(), false);
        }
        shader.getConstantBuffer();
        shaders.insert(std::pair<std::string, Shader>(shaderName, shader));
    }
    void updateConstant(std::string shaderName, std::string cbName, std::string varName, void* data) {
        shaders[shaderName].constantBuffers[cbName].update(varName, data);
    }
};
class Cube {
public:
    WorldCube cube;
    PSOManager psos;
    ShaderManager* sm;

    void init(Core* core, ShaderManager* _sm, Vec3 offset = Vec3(0, 0, 0)) {
        sm = _sm;
        cube.init(core, offset);
        cbinit(core);
        psos.createPSO(core, "Cube", sm->shaders["VertexShader"].shader, sm->shaders["PixelShader"].shader, cube.mesh.inputLayoutDesc);
    }
    void cbinit(Core* core) {
        for (auto it = sm->shaders.begin(); it != sm->shaders.end(); ++it)
            for (auto it1 = it->second.constantBuffers.begin(); it1 != it->second.constantBuffers.end(); ++it1)
                it1->second.init(core, it1->second.cbSizeInBytes, 1024);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix planeWorld, Matrix vp) {
        sm->updateConstant("VertexShader", "staticMeshBuffer", "W", &planeWorld);
        sm->updateConstant("VertexShader", "staticMeshBuffer", "VP", &vp);
    }
    void apply(Core* core) {//actually we dont need loop in this example
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["VertexShader"].constantBuffers.begin(); it != sm->shaders["VertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelShader"].constantBuffers.begin(); it != sm->shaders["PixelShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(Core* core) {
        core->beginRenderPass();
        apply(core);
        psos.bind(core, "Cube");
        cube.mesh.draw(core);
    }
};

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    //window init
    Window win;
    win.initWnd(canvasWidth, canvasHeight, "My Window");
    //core init
    Core core;
    core.init(win.hwnd, 1024, 1024);
    //shader manager init
    ShaderManager sm;
    sm.init();
    //proM init
    proM = Matrix::projectionMatrix(M_PI / 4, static_cast<float>(canvasWidth) / canvasHeight, _near, _far);
    //draw object init
    Cube cube;
    cube.init(&core, &sm);
    Cube cube1;
    cube1.init(&core, &sm, Vec3(5, 0, 0));
    //timer init
    float t = 0;
    GamesEngineeringBase::Timer timer;
    while (1) {
        core.beginFrame();
        win.processMessages();
        if (win.keys[VK_ESCAPE] == 1) break;
        //lookat matrix update
        float dt = timer.dt();
        t += dt;
        Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
        Matrix v = Matrix::lookAtMatrix(from, Vec3(0, 1, 0), Vec3(0, 1, 0));
        //update constant buffer and draw
        cube.update(v, proM);
        cube.draw(&core);
        cube1.update(v, proM);
        cube1.draw(&core);
        //how to add offset after initializing objects
        core.finishFrame();
    }
    core.flushGraphicsQueue();
    return 0;
}

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

