Warwick CG Notes
=============

a basic learning notes
------------

2025/11/22
add **Zbuffer**
```cpp
bool zbufferUpdate(const int x, const int y, const float z) {
    if (z > 0.0f && z < buffer[x][y]) {
        buffer[x][y] = z;
        return true;
    }
    else
        return false;
}
```
**Sutherland-Hodgman Algorithm**(for plane near and far)
```cpp
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
```
using **quaternion** to implement rotation functionality
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Example/1122.gif)


