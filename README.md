Warwick CG Notes
=============

a simple&basic learning notes
------------

<details>
<summary>2025/11/22</summary>

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/CPUExample/Example/1123_1.gif)

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

<details>
<summary>2025/12/2</summary>
Using GPU programming to generate 3D objects in screen.
        
**Shader Manager** can manage shaders load, .cso file generate and constant buffer update.
    
use std::map to store shaders 
    
```cpp
class ShaderManager {
public:
    std::map<std::string, Shader> shaders;
    void init(){...}
    void loadShader(std::string shaderName, shaderTypes shaderType){...}
    void updateConstant(std::string shaderName, std::string cbName, std::string varName, void* data){...}
};
```
**Code Reflection** use map to store varibles of constant buffers in a certain shader.

```cpp
std::map<std::string, ConstantBuffer> constantBuffers;
```
instead of creating and updating a structure when we need to change constant buffer, this functionality can simplify update operations through directly changing data in memory by using address.
    
```cpp
void update(std::string name, void* data) {
    ConstantBufferVariable cbVariable = constantBufferData[name];
    unsigned int offset = offsetIndex * cbSizeInBytes;
    memcpy(&buffer[offset + cbVariable.offset], data, cbVariable.size);
}
```
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1202.gif)

</details>
<details>
<summary>2025/12/3</summary>
Seems like a little mistake in my cb.update method.
The correct output should be this:
        
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1203.gif)
    
That wrong GIF is pretty cool actually...
    
</details>
<details>
<summary>2025/12/5</summary>
    
Made some adjustments to make sure the animated models can act normally..

Use **Bezier Curve** to move the tree among 3 cubes.

In Tom's Math.h headfile, he defined a matrix multiplication which can swap the applying order of two matrix. 

e.g. 

```cpp
Matrix vp = v * persM;//v for Lookat Matrix, persM for Perspective Matrix
```

```cpp
 sm->updateConstant("AnimatedVertexShader", "animatedMeshBuffer", "VP", &vp); 
```

```hlsl
output.Pos = mul(output.Pos, VP);// output.Pos * P * V
```
V will be applied first because of the right multiplication of a matrix.

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1206.gif)

</details>
<details>
<summary>2025/12/7</summary>
    
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1208.gif)

add camera move test by using quaternion (but still have some problem with rotation).

Created a DynamicMesh, which can update vertex info by using memcpy.

I referred to the book written by Frank D. Luna.

```cpp
class DynamicMesh {
public:
    ID3D12Resource* vertexBuffer;
    ID3D12Resource* indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW vbView;
    D3D12_INDEX_BUFFER_VIEW ibView;
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
    unsigned int numMeshIndices;
    uint8_t* vMapped;
    //void* iMapped;
//blahblahblah
}
```

Then I can use this class to create a upload buffer to simulate waves.(I hope so)

</details>

<details>
<summary>2025/12/8</summary>
    
![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1208-1.gif)

add texture to the Trex.

press '1' to see the wireframe version(by changing PSO when drawing)

for example:
```cpp
psos->createPSO(core, "LandAndWaves", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout());
psos->createPSO(core, "LandAndWavesWire", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout(), true);
```

</details>

<details>
<summary>2025/12/9</summary>

added wave simulation(CPU) to project (referenced to Frank Luna)

![](https://github.com/Wang-Yuan0813/Warwick-CG/raw/master/Examples/1209.gif)

That is a very simple demo without calculating normal of each vertex. That will cause some problems with lighting.

And I assumed that dt is fixed (0.02f), this can be optimazed later.

Actually, all these work should be implemented in the GPU, which is much more efficient (and complicated I think).

For now, let's use CPU to simulate this effect.

</details>
