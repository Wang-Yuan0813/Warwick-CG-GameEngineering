Warwick CG Notes
=============

a basic learning notes
------------

_2025/11/22_

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

_2025/11/23_

use **Lookup Matrix** to simlify the rotating operation.

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Example/1123.gif)

This function will draw triangles by each 3 verteces.
```cpp
static void drawByVerteces(GamesEngineeringBase::Window& canvas, const std::vector<Vec4>& verteces, const Vec4& offset, Matrix& lookupM) {
    if (verteces.size() % 3 != 0) {//triangle
        std::cout << "verteces num wrong!" << std::endl;
        return;
    }
    for (int i = 0; i < verteces.size() / 3; i++) {
        Vec4 v[3] = { verteces[i * 3], verteces[i * 3 + 1],verteces[i * 3 + 2] };
        Triangle tri(lookupM.mul(v[0]) + offset, lookupM.mul(v[1]) + offset, lookupM.mul(v[2]) + offset);
        //Triangle tri(v[0] + offset, v[1] + offset, v[2] + offset);
        tri.draw(canvas);
    }

}
```
Will update diffuse lighting later.


