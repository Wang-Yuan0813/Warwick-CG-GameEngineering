Warwick CG Notes
=============

a simple&basic learning notes
------------

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/CPUExample/Example/1123_1.gif)

<details>
<summary>2025/11/22</summary>

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
I'm still not figuring out whether should I do the clip to the screen (Maybe GPU can automatically handle this kind of situation when I literally use it? ). But I found that if I implement this to the screen plane, the bounding box will be smaller in some cases (e.g. The triangle only left a little part in the screen after projecting). 

using **quaternion** to implement rotation functionality

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/CPUExample/Example/1122.gif)

</details>



<details>
<summary>2025/11/23</summary>

use **Lookup Matrix** to simlify the rotating operation.

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/CPUExample/Example/1123.gif)

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

Function below can simulate diffusion light to a model. 

Because light simulation needs triangle's normal vertex to calculate color intensity, so I added a Vec3 object in class Triangle named normal and also the relative function.

```cpp
static Colour diffusionLignt(Triangle& tri) {
    Vec3 omega_i = Vec3(1, 1, 0).normalize();
    Vec3 N = tri.findNormal();
    Colour rho(0, 1.0f, 0);
    Colour L(1.0f, 1.0f, 1.0f);
    Colour ambient(0.5f, 0.5f, 0.5f);
    return (rho / M_PI) * (L * max(omega_i.dot(N), 0.0f) + ambient);
}
```
In the course's pdf, we use Lookat matrix to rotate the camera. That means other objects in the world space should be still at their location and the light direction as well.

If we don't implement the Lookat matrix to light vector :
```cpp
Vec3 omega_i = Vec3(1, 1, 0).normalize();
```
the light rendering will be wrong

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Example/1123_2.gif)

So it's supposed to be:
```cpp
Vec3 omega_i = lookupM.mulVec(Vec3(1, 1, 0)).normalize();//rotate the camera, light ray should not change its vector in world space
```
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/CPUExample/Example/1123_3.gif)



</details>




