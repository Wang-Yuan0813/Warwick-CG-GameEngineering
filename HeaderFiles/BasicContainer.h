#pragma once
#include "HeaderFiles/GamesEngineeringBase.h"
#define SQ(x) ((x)*(x))
template<typename T>
static T clamp(const T value, const T minValue, const T maxValue)
{
    return max(min(value, maxValue), minValue);
}
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
    Vec3 normalize() const{
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
//class Matrix {
//public:
//    union {
//        float a[4][4];
//        float m[16];
//    };
//    Matrix() {
//        memset(m, 0, 16 * sizeof(float));
//        a[0][0] = 1; a[1][1] = 1; a[2][2] = 1; a[3][3] = 1;
//    }
//    Matrix(float* _m) {
//        m[0] = _m[0];   m[1] = _m[1];   m[2] = _m[2];   m[3] = _m[3];
//        m[4] = _m[4];   m[5] = _m[5];   m[6] = _m[6];   m[7] = _m[7];
//        m[8] = _m[8];   m[9] = _m[9];   m[10] = _m[10]; m[11] = _m[11];
//        m[12] = _m[12]; m[13] = _m[13]; m[14] = _m[14]; m[15] = _m[15];
//    }
//    void print() const {
//        for (int i = 0; i < 4; i++) {
//            std::cout << '|' << '\t';
//            for (int j = 0; j < 4; j++) {
//                std::cout << a[i][j] << '\t';
//            }
//            std::cout << '|' << std::endl;
//        }
//    }
//    Vec4 mul(const Vec4& v) {
//        return Vec4(
//            v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3],
//            v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7],
//            v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11],
//            v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]);
//    }
//    Vec3 mulPoint(const Vec3& v) {
//        Vec3 v1 = Vec3(
//            (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
//            (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
//            (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
//    }
//    Vec3 mulVec(const Vec3& v) {
//        return Vec3(
//            (v.x * m[0] + v.y * m[1] + v.z * m[2]),
//            (v.x * m[4] + v.y * m[5] + v.z * m[6]),
//            (v.x * m[8] + v.y * m[9] + v.z * m[10]));
//    }
//    Matrix mul(const Matrix& matrix) const {
//        Matrix ret;
//        ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
//        ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
//        ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
//        ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
//        ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
//        ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
//        ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
//        ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
//        ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
//        ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
//        ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
//        ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
//        ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
//        ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
//        ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
//        ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
//        return ret;
//    }
//    float determinant() {
//        Matrix inv;
//        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
//        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
//        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
//        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
//        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
//        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
//        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
//        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
//        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
//        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
//        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
//        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
//        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
//        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
//        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
//        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
//        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
//        return det;
//    }
//    Matrix invert() const {
//        Matrix inv;
//        inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
//        inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
//        inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
//        inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
//        inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
//        inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
//        inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
//        inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
//        inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
//        inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
//        inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
//        inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
//        inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
//        inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
//        inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
//        inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
//        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
//        if (det == 0) {
//            // Handle this case 
//            std::cout << "det = 0!!" << std::endl;
//        }
//        det = 1.0f / det;
//        for (int i = 0; i < 16; i++) {
//            inv[i] = inv[i] * det;
//        }
//        return inv;
//    }
//    Matrix transpose() const {
//        Matrix inv;
//        inv[0] = m[0];  inv[1] = m[4];  inv[2] = m[8];  inv[3] = m[12];
//        inv[4] = m[1];  inv[5] = m[5];  inv[6] = m[9];  inv[7] = m[13];
//        inv[8] = m[2];  inv[9] = m[6];  inv[10] = m[10]; inv[11] = m[14];
//        inv[12] = m[3]; inv[13] = m[7]; inv[14] = m[11]; inv[15] = m[15];
//        return inv;
//    }
//    static Matrix projectionMatrix(float fov, float aspect, float _near, float _far) {
//        Matrix proM;
//        memset(proM.m, 0, 16 * sizeof(float));
//        proM.a[0][0] = 1 / (aspect * (tan(fov / 2)));//no pi
//        proM.a[1][1] = 1 / (tan(fov / 2));
//        proM.a[2][2] = _far / (_far - _near);
//        proM.a[2][3] = -(_far * _near) / (_far - _near);
//        proM.a[3][2] = 1;
//        return proM;
//    }
//    static Matrix lookAtMatrix(const Vec3& from, const Vec3& to, const Vec3& up) {
//        Matrix lookat;
//        memset(lookat.m, 0, 16 * sizeof(float));
//        Vec3 dir = (to - from) / (to - from).length();
//        Vec3 right = up.cross(dir);//only cross x,y,z. 
//        Vec3 realUp = dir.cross(right);//real up vector
//
//        lookat.a[0][0] = right.x;  lookat.a[0][1] = right.y;  lookat.a[0][2] = right.z;  lookat.a[0][3] = -(from.dot(right));
//        lookat.a[1][0] = realUp.x; lookat.a[1][1] = realUp.y; lookat.a[1][2] = realUp.z; lookat.a[1][3] = -(from.dot(realUp));
//        lookat.a[2][0] = dir.x;    lookat.a[2][1] = dir.y;    lookat.a[2][2] = dir.z;    lookat.a[2][3] = -(from.dot(dir));
//        lookat.a[3][3] = 1;
//        return lookat;
//
//    }
//    static Matrix translation(const Vec3& offset) {
//        Matrix tranM;
//        tranM.a[0][3] = offset.x;
//        tranM.a[1][3] = offset.y;
//        tranM.a[2][3] = offset.z;
//        return tranM;
//    }
//    static Matrix scaling(const Vec3& offset) {
//        Matrix tranM;
//        tranM.a[0][0] = offset.x;
//        tranM.a[1][1] = offset.y;
//        tranM.a[2][2] = offset.z;
//        return tranM;
//    }
//    float& operator[](const int index) {
//        return m[index];
//    }
//    Matrix operator*(const Matrix& matrix) {
//        return mul(matrix);
//    }
//};
static float Dot(const Vec3& v1, const Vec3& v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }
static Vec3 Cross(const Vec3& v1, const Vec3& v2) { return Vec3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x)); }
static Vec3 Max(const Vec3& v1, const Vec3& v2) { return Vec3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z)); }
static Vec3 Min(const Vec3& v1, const Vec3& v2) { return Vec3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z)); }

class alignas(64) Matrix
{
public:
    union
    {
        float a[4][4];
        float m[16];
    };
    Matrix() { identity(); }
    Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
    {
        a[0][0] = m00;
        a[0][1] = m01;
        a[0][2] = m02;
        a[0][3] = m03;
        a[1][0] = m10;
        a[1][1] = m11;
        a[1][2] = m12;
        a[1][3] = m13;
        a[2][0] = m20;
        a[2][1] = m21;
        a[2][2] = m22;
        a[2][3] = m23;
        a[3][0] = m30;
        a[3][1] = m31;
        a[3][2] = m32;
        a[3][3] = m33;
    }
    void identity()
    {
        memset(m, 0, 16 * sizeof(float));
        m[0] = 1.0f;
        m[5] = 1.0f;
        m[10] = 1.0f;
        m[15] = 1.0f;
    }
    Matrix transpose()
    {
        return Matrix(a[0][0], a[1][0], a[2][0], a[3][0],
            a[0][1], a[1][1], a[2][1], a[3][1],
            a[0][2], a[1][2], a[2][2], a[3][2],
            a[0][3], a[1][3], a[2][3], a[3][3]);
    }
    float& operator[](int index)
    {
        return m[index];
    }
    static Matrix translation(const Vec3& v)
    {
        Matrix mat;
        mat.a[0][3] = v.x;
        mat.a[1][3] = v.y;
        mat.a[2][3] = v.z;
        return mat;
    }
    static Matrix scaling(const Vec3& v)
    {
        Matrix mat;
        mat.m[0] = v.x;
        mat.m[5] = v.y;
        mat.m[10] = v.z;
        return mat;
    }
    static Matrix rotateX(float theta)
    {
        Matrix mat;
        float ct = cosf(theta);
        float st = sinf(theta);
        mat.m[5] = ct;
        mat.m[6] = st;
        mat.m[9] = -st;
        mat.m[10] = ct;
        return mat;
    }
    static Matrix rotateY(float theta)
    {
        Matrix mat;
        float ct = cosf(theta);
        float st = sinf(theta);
        mat.m[0] = ct;
        mat.m[2] = -st;
        mat.m[8] = st;
        mat.m[10] = ct;
        return mat;
    }
    static Matrix rotateZ(float theta)
    {
        Matrix mat;
        float ct = cosf(theta);
        float st = sinf(theta);
        mat.m[0] = ct;
        mat.m[1] = st;
        mat.m[4] = -st;
        mat.m[5] = ct;
        return mat;
    }
    Matrix mul(const Matrix& matrix) const
    {
        Matrix ret;

        ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
        ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
        ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
        ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];

        ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
        ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
        ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
        ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];

        ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
        ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
        ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
        ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];

        ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
        ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
        ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
        ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];

        return ret;
    }
    Matrix operator*(const Matrix& matrix)
    {
        return mul(matrix);
    }
    Vec3 mulVec(const Vec3& v)
    {
        return Vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]),
            (v.x * m[4] + v.y * m[5] + v.z * m[6]),
            (v.x * m[8] + v.y * m[9] + v.z * m[10]));
    }
    Vec3 mulPoint(const Vec3& v)
    {
        Vec3 v1 = Vec3(
            (v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
            (v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
            (v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
        float w;
        w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
        w = 1.0f / w;
        return (v1 * w);
    }
    Matrix operator=(const Matrix& matrix)
    {
        memcpy(m, matrix.m, sizeof(float) * 16);
        return (*this);
    }
    Matrix invert() // Unrolled inverse from MESA library
    {
        Matrix inv;
        inv[0] = m[5] * m[10] * m[15] -
            m[5] * m[11] * m[14] -
            m[9] * m[6] * m[15] +
            m[9] * m[7] * m[14] +
            m[13] * m[6] * m[11] -
            m[13] * m[7] * m[10];
        inv[4] = -m[4] * m[10] * m[15] +
            m[4] * m[11] * m[14] +
            m[8] * m[6] * m[15] -
            m[8] * m[7] * m[14] -
            m[12] * m[6] * m[11] +
            m[12] * m[7] * m[10];
        inv[8] = m[4] * m[9] * m[15] -
            m[4] * m[11] * m[13] -
            m[8] * m[5] * m[15] +
            m[8] * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];
        inv[12] = -m[4] * m[9] * m[14] +
            m[4] * m[10] * m[13] +
            m[8] * m[5] * m[14] -
            m[8] * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];
        inv[1] = -m[1] * m[10] * m[15] +
            m[1] * m[11] * m[14] +
            m[9] * m[2] * m[15] -
            m[9] * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];
        inv[5] = m[0] * m[10] * m[15] -
            m[0] * m[11] * m[14] -
            m[8] * m[2] * m[15] +
            m[8] * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];
        inv[9] = -m[0] * m[9] * m[15] +
            m[0] * m[11] * m[13] +
            m[8] * m[1] * m[15] -
            m[8] * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];
        inv[13] = m[0] * m[9] * m[14] -
            m[0] * m[10] * m[13] -
            m[8] * m[1] * m[14] +
            m[8] * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];
        inv[2] = m[1] * m[6] * m[15] -
            m[1] * m[7] * m[14] -
            m[5] * m[2] * m[15] +
            m[5] * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];
        inv[6] = -m[0] * m[6] * m[15] +
            m[0] * m[7] * m[14] +
            m[4] * m[2] * m[15] -
            m[4] * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];
        inv[10] = m[0] * m[5] * m[15] -
            m[0] * m[7] * m[13] -
            m[4] * m[1] * m[15] +
            m[4] * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];
        inv[14] = -m[0] * m[5] * m[14] +
            m[0] * m[6] * m[13] +
            m[4] * m[1] * m[14] -
            m[4] * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];
        inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];
        inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];
        inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];
        inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];
        float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
        if (det == 0)
        {
            exit(0); // Perhaps add logging here. This should not happen in a games context
        }
        det = 1.0 / det;
        for (int i = 0; i < 16; i++)
        {
            inv[i] = inv[i] * det;
        }
        return inv;
    }
    static Matrix lookAt(const Vec3& from, const Vec3& to, const Vec3& up)
    {
        Matrix mat;
        Vec3 dir = (to - from).normalize();
        Vec3 left = Cross(up, dir).normalize();
        Vec3 newUp = Cross(dir, left);
        mat.a[0][0] = left.x;
        mat.a[0][1] = left.y;
        mat.a[0][2] = left.z;
        mat.a[1][0] = newUp.x;
        mat.a[1][1] = newUp.y;
        mat.a[1][2] = newUp.z;
        mat.a[2][0] = dir.x;
        mat.a[2][1] = dir.y;
        mat.a[2][2] = dir.z;
        mat.a[0][3] = -Dot(from, left);
        mat.a[1][3] = -Dot(from, newUp);
        mat.a[2][3] = -Dot(from, dir);
        mat.a[3][3] = 1;
        return mat;
    }
    static Matrix perspective(const float n, const float f, float aspect, const float fov) // FOV in degrees, outputs transposed Matrix for DX
    {
        Matrix pers;
        memset(pers.m, 0, sizeof(float) * 16);
        float t = 1.0f / (tanf(fov * 0.5f * 3.141592654f / 180.0f));
        pers.a[0][0] = t / aspect;
        pers.a[1][1] = t;
        pers.a[2][2] = f / (f - n);
        pers.a[2][3] = -(f * n) / (f - n);
        pers.a[3][2] = 1.0f;
        return pers;
    }
    static Matrix rotateAxis(const Vec3& axis, float angle)
    {
        Vec3 u = axis.normalize();
        float c = cosf(angle);
        float s = sinf(angle);
        float t = 1.0f - c;

        return Matrix(
            t * u.x * u.x + c, t * u.x * u.y + s * u.z, t * u.x * u.z - s * u.y, 0,
            t * u.x * u.y - s * u.z, t * u.y * u.y + c, t * u.y * u.z + s * u.x, 0,
            t * u.x * u.z + s * u.y, t * u.y * u.z - s * u.x, t * u.z * u.z + c, 0,
            0, 0, 0, 1
        );
    }
};
class Quaternion {
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
    Quaternion() : a(0), b(0), c(0), d(0) {}
    Quaternion(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}
    Quaternion(float theta, Vec3 dir) {
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
        float n = 1.0f / sqrtf(SQ(a) + SQ(b) + SQ(c) + SQ(d));
        a *= n;
        b *= n;
        c *= n;
        d *= n;
        /*float m = magnitude();
        *this = *this / m;*/
    }
    Quaternion conjugate() const {
        return Quaternion(-a, -b, -c, d);
    }
    Quaternion inverse() const {
        float m2 = magnitudeSquare();
        return Quaternion(-a / m2, -b / m2, -c / m2, d / m2);
    }
    //Quaternion slerp(const Quaternion& q, float t) const {
    //    Quaternion res;
    //    if (t < 0 || t > 1) {
    //        std::cout << "slerp error, invalid t!!" << std::endl;
    //        return res;
    //    }
    //    float theta = findAngle(q);
    //    //std::cout << cosf(theta) << std::endl;
    //    float q1_co = sinf(theta * (1 - t)) / sinf(theta);
    //    float q2_co = sinf(theta * t) / sinf(theta);
    //    res.a = q1_co * a + q2_co * q.a;
    //    res.b = q1_co * b + q2_co * q.b;
    //    res.c = q1_co * c + q2_co * q.c;
    //    res.d = q1_co * d + q2_co * q.d;
    //    return res;
    //}
    static Quaternion slerp(Quaternion q1, Quaternion q2, float t)
    {
        Quaternion qr;
        float dp = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
        Quaternion q11 = dp < 0 ? -q1 : q1;
        dp = dp > 0 ? dp : -dp;
        float theta = acosf(clamp(dp, -1.0f, 1.0f));
        if (theta == 0)
        {
            return q1;
        }
        float d = sinf(theta);
        float a = sinf((1 - t) * theta);
        float b = sinf(t * theta);
        float coeff1 = a / d;
        float coeff2 = b / d;
        qr.a = coeff1 * q11.a + coeff2 * q2.a;
        qr.b = coeff1 * q11.b + coeff2 * q2.b;
        qr.c = coeff1 * q11.c + coeff2 * q2.c;
        qr.d = coeff1 * q11.d + coeff2 * q2.d;
        qr.normalize();
        return qr;
    }
    static float dot(const Quaternion& q1, const Quaternion& q2){
        return q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
    }
    static float findAngle(const Quaternion& q1, const Quaternion& q2){
        return acosf(dot(q1, q2));
    }
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(a + q.a, b + q.b, c + q.c, d + q.d);
    }
    Quaternion operator+(const float val) const {
        return Quaternion(a + val, b + val, c + val, d + val);
    }
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(a - q.a, b - q.b, c - q.c, d - q.d);
    }
    Quaternion operator-(const float val) const {
        return Quaternion(a - val, b - val, c - val, d - val);
    }
    Quaternion operator-() const {
        return Quaternion(-a, -b, -c, -d);
    }
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(d * q.a + a * q.d + b * q.c - c * q.b,
            d * q.b - a * q.c + b * q.d + c * q.a,
            d * q.c + a * q.b - b * q.a + c * q.d,
            d * q.d - a * q.a - b * q.b - c * q.c);
    }
    Quaternion operator/(const float val) const {
        return Quaternion(a / val, b / val, c / val, d / val);
    }
    void print() const {
        std::cout << '(' << a << ',' << b << ',' << c << ',' << d << ')' << std::endl;
    }
};

class BezierCurve {
private:
    int fac(int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    }
    float biCoefficient(int n, int k) {
        return (float)fac(n) / (fac(k) * fac(n - k));
    }
public:
    std::vector<Vec3> cPoints;
    void loadPoints(std::vector<Vec3>& _cPoints) {
        cPoints = _cPoints;
    }
    Vec3 curPoint(float t) {
        Vec3 res;
        int n = cPoints.size() - 1;
        for (int i = 0; i <= n; ++i) {
            float coeff = biCoefficient(n, i);
            float basis = coeff * pow(1 - t, n - i) * pow(t, i);
            res = res + cPoints[i] * basis;
        }
        return res;
    }

};