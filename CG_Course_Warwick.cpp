#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "HeaderFiles/window.h"
#include "HeaderFiles/Core.h"
#include "HeaderFiles/GEMLoader.h"
#include "HeaderFiles/Shader.h"
#include "HeaderFiles/BasicContainer.h"
#include "HeaderFiles/Model.h"

//-------------------

class GeometryPlane {
public:
    DynamicMesh mesh;
    std::vector<STATIC_VERTEX> vertices;
    std::vector<unsigned int> indices;
    int rowCount;
    int colCount;
    void CreateGrid(Core* core,int m, int n) {
        rowCount = m;
        colCount = n;
        float size = 0.8;//for now
        float width = size * (n - 1);
        float depth = size * (m - 1);
        //generate vertices
        int vertexCount = m * n;
        int faceCount = (m - 1) * (n - 1) * 2;
        float halfWidth = 0.5f * width;
        float halfDepth = 0.5f * depth;
        float dx = width / (n - 1);
        float dz = depth / (m - 1);
        float du = 1.0f / (n - 1);//u and v should be [0, 1]
        float dv = 1.0f / (m - 1);
        vertices.resize(vertexCount);
        for (int i = 0; i < m; ++i){
            float z = halfDepth - i * dz;
            for (int j = 0; j < n; ++j){
                float x = -halfWidth + j * dx;//how to calculate normal vector?
                vertices[i * n + j] = addVertex(Vec3(x, 0.0f, z), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),
                    j * du, i * dv);
            }
        }
        //generate indices
        indices.resize(faceCount * 3);
        int k = 0;
        for (int i = 0; i < m - 1; ++i){
            for (int j = 0; j < n - 1; ++j){
                indices[k] = i * n + j;
                indices[k + 1] = i * n + j + 1;
                indices[k + 2] = (i + 1) * n + j;
                indices[k + 3] = (i + 1) * n + j;
                indices[k + 4] = i * n + j + 1;
                indices[k + 5] = (i + 1) * n + j + 1;
                k += 6; // next quad
            }
        }
        //mesh.init(core, vertices, indices);
    }
    
};
class Land {
public:
    GeometryPlane grid;
    PSOManager* psos;
    ShaderManager* sm;
    Core* core;
    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos) {
        sm = _sm;
        psos = _psos;
        core = _core;
        buildLandGeometry();
        psos->createPSO(core, "LandAndWaves", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout());
        psos->createPSO(core, "LandAndWavesWire", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout(), true);

    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix vp) {
        sm->updateConstant("StaticColourVertexShader", "staticMeshBuffer", "W", &world);
        sm->updateConstant("StaticColourVertexShader", "staticMeshBuffer", "VP", &vp);
    }
    void apply() {//actually we dont need loop in this example
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["StaticColourVertexShader"].constantBuffers.begin(); it != sm->shaders["StaticColourVertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelColourShader"].constantBuffers.begin(); it != sm->shaders["PixelColourShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, "LandAndWavesWire");
        else                psos->bind(core, "LandAndWaves");
        grid.mesh.draw(core);
    }
    void buildLandGeometry() {
        grid.CreateGrid(core, 50, 50);//for now
        //std::vector<Vertex> vertices(grid.vertices.size());//may delete later
        for (int i = 0; i < grid.vertices.size(); ++i) {
            STATIC_VERTEX& p = grid.vertices[i];
            p.pos.y = getHillsHeight(p.pos.x, p.pos.z);
            //Color based on height
            if (p.pos.y < -10.0f) {
                // Sandy beach color.
                p.color = Colour(1.0f, 0.96f, 0.62f, 1.0f);
            }
            else if (p.pos.y < 5.0f) {
                // Light yellow-green.
                p.color = Colour(0.48f, 0.77f, 0.46f, 1.0f);
            }
            else if (p.pos.y < 12.0f) {
                // Dark yellow-green.
                p.color = Colour(0.1f, 0.48f, 0.19f, 1.0f);
            }
            else if (p.pos.y < 20.0f) {
                // Dark brown.
                p.color = Colour(0.45f, 0.39f, 0.34f, 1.0f);
            }
            else {
                // White snow.
                p.color = Colour(1.0f, 1.0f, 1.0f, 1.0f);
            }
        }
        grid.mesh.init(core, grid.vertices, grid.indices);
    }
    float getHillsHeight(float x, float z)const {
        return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
    }
};
class Waves {
public:
    float c;
    float beta;
    int rows;
    int cols;
    float h;
    float dt;
    float mK1, mK2, mK3;
    std::vector<float> prev, curr, next;
    std::vector<Vec3> normals;
    void init(int _rows, int _cols, float _h, float _dt, float _speed, float _beta){

        rows = _rows;
        cols = _cols;
        h = _h;
        dt = _dt;
        c = _speed;
        beta = _beta;


        prev.resize(rows * cols);
        curr.resize(rows * cols);
        next.resize(rows * cols);
        normals.resize(rows * cols);

        float s = beta * dt;
        float e = (c * c) * (dt * dt) / (h * h);
        float d = s + 2;
        mK1 = (s - 2) / d;
        mK2 = (4 - 2 * e) / d;
        mK3 = (2 * e) / d;


        std::fill(prev.begin(), prev.end(), 0.0f);
        std::fill(curr.begin(), curr.end(), 0.0f);
        std::fill(next.begin(), next.end(), 0.0f);
        std::fill(normals.begin(), normals.end(), Vec3(0, 1, 0));
    }
    void disturb(int i, int j, float r) {
        curr[i * cols + j] += r;
        curr[i * cols + j + 1] += r / 2;
        curr[i * cols + j - 1] += r / 2;
        curr[(i + 1) * cols + j] += r / 2;
        curr[(i - 1) * cols + j] += r / 2;
        prev = curr;
    }
    void update() {
        for (int i = 1; i < rows - 1; ++i){
            for (int j = 1; j < cols - 1; ++j){
                int idx = i * cols + j;
                float uij = curr[idx];
                float u00 = prev[idx];

                float uip1 = curr[(i + 1) * cols + j];
                float uim1 = curr[(i - 1) * cols + j];
                float ujp1 = curr[i * cols + j + 1];
                float ujm1 = curr[i * cols + j - 1];

                float lap = (uip1 + uim1 + ujp1 + ujm1 - 4.0f * uij);

                next[idx] = mK1 * u00 + mK2 * uij + mK3 * lap;

                //normal simple calculation

                float du_dx = (ujp1 - ujm1) / (2.0f * 0.8);
                float du_dy = (uip1 - uim1) / (2.0f * 0.8);

                Vec3 nCurr = Vec3(-du_dx, 1.0f, -du_dy);
                normals[idx] = nCurr.normalize();
            }
        }
        prev.swap(curr);
        curr.swap(next);
    }
};
class Water {
public:
    GeometryPlane grid;
    PSOManager* psos;
    ShaderManager* sm;
    Core* core;
    Waves waves;

    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos) {
        sm = _sm;
        psos = _psos;
        core = _core;
        buildWaterGeometry();
        psos->createPSO(core, "LandAndWaves", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout());
        psos->createPSO(core, "LandAndWavesWire", sm->shaders["StaticColourVertexShader"].shader, sm->shaders["PixelColourShader"].shader, VertexLayoutCache::getStaticColourLayout(), true);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix vp) {
        sm->updateConstant("StaticColourVertexShader", "staticMeshBuffer", "W", &world);
        sm->updateConstant("StaticColourVertexShader", "staticMeshBuffer", "VP", &vp);
    }
    void apply() {
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["StaticColourVertexShader"].constantBuffers.begin(); it != sm->shaders["StaticColourVertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelColourShader"].constantBuffers.begin(); it != sm->shaders["PixelColourShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, "LandAndWavesWire");
        else                psos->bind(core, "LandAndWaves");
        grid.mesh.draw(core);
    }
    void buildWaterGeometry() {
        grid.CreateGrid(core, 128, 128);//for now
        waves.init(128,128, 0.8, 0.02, 3.25, 0.4);//the third parameter should be change according to CreateGrid()
        //std::vector<Vertex> vertices(grid.vertices.size());//may delete later
        for (int i = 0; i < grid.vertices.size(); ++i) {
            STATIC_VERTEX& p = grid.vertices[i];
            //Color based on height
            p.color = Colour(0.0f, 0.0f, 0.62f, 1.0f);//for now
        }
        grid.mesh.init(core, grid.vertices, grid.indices);
    }
    float getWaterHeight(float x, float z)const {
        return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
    }

    void waterUpdate(float dt) {
        static float totalTime = 0.f;
        totalTime += dt;
        if (totalTime >= 0.5) {
            //create a wave source at a random point
            totalTime = 0.f;
            int i = mrand(4, grid.rowCount - 5);
            int j = mrand(4, grid.colCount - 5);
            float r = mrandf(2, 5);
            waves.disturb(i, j, r);
            std::cout << "Disturb position = " << "(" << i << "," << j << ")" << std::endl;
        }
        //update wave simulation
        waves.update();
        for (int i = 0; i < grid.vertices.size(); i++) {
            grid.vertices[i].pos.y = waves.curr[i];
            grid.vertices[i].normal = waves.normals[i];
        }
        grid.mesh.copyData(0, grid.vertices.size() * sizeof(STATIC_VERTEX), &grid.vertices[0]);
    }
    int rand(int a, int b) {
        return std::rand() % a + b;
    }

};

//-------------------
void InputCheck(Window& win, bool& wireFrameMode, bool& running, Vec3& from, Vec3& look, int pre_x, int pre_y) {
    if (win.keys[VK_ESCAPE] == 1) running = false;
    if (win.keys['1'] == 1) wireFrameMode = true;
#if DEBUG_MODE
    //lookat matrix update
    float speed = 0.5f;
    float sensitive = 0.5 * M_PI / 180.f;
    Vec3 up(0, 1, 0);
    int threshold = 1;
    Vec3 cameraDir = (look - from).normalize();
    Vec3 right = up.cross(cameraDir).normalize();
    if (win.mouseButtons[0]) {

        int dx = win.mousex - pre_x;
        int dy = win.mousey - pre_y;

        if (std::abs(dx) < threshold) dx = 0;
        if (std::abs(dy) < threshold) dy = 0;

        float deltaYaw = sensitive * static_cast<float>(dx);
        float deltaPitch = -sensitive * static_cast<float>(dy);

        cameraDir.normalize();

        if (right.length() < 1e-6f) {
            right = Vec3(0, 0, 1).cross(cameraDir);
            if (right.length() < 1e-6f) {
                right = Vec3(1, 0, 0).cross(cameraDir);
            }
        }
        right = right.normalize();

        Quaternion qYaw(deltaYaw, up);
        Quaternion qPitch(deltaPitch, right);

        Quaternion q = (qYaw * qPitch);
        q.normalize();
        Quaternion::rotatePoint(q, cameraDir);
        cameraDir = cameraDir.normalize();

        const float maxY = 0.9f;
        //cameraDir.y = max(cameraDir.y, -maxY);
        //cameraDir.y = min(cameraDir.y, maxY);
        cameraDir.y = clamp(cameraDir.y, -maxY, maxY);
        cameraDir = cameraDir.normalize();

        right = up.cross(cameraDir).normalize();
    }
    look = from + cameraDir; 
    Vec3 offset1, offset2;
    if (win.keys[VK_UP])  offset1 = cameraDir * speed;
    if (win.keys[VK_DOWN])  offset1 = - cameraDir * speed;
    if (win.keys[VK_LEFT])  offset2 = right * speed;
    if (win.keys[VK_RIGHT])  offset2 = - right * speed;
    from += offset1 + offset2;
    look += offset1 + offset2;
#endif
}

Matrix persM;
float _near = 0.1f;
float _far = 10000.f;
static const float canvasWidth = 800;
static const float canvasHeight = 800;
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    //window init
    Window win;
    win.initWnd(canvasWidth, canvasHeight, "My Window");
#if DEBUG_MODE
    win.CreateDebugConsole();
#endif
    //core init
    Core core;
    core.init(win.hwnd, canvasWidth, canvasHeight);
    //shader manager init
    ShaderManager sm;
    sm.init(&core);
    //psosmanager init
    PSOManager pm;
    //texturemanager init
    TextureManager tm;
    //Camera init
    persM = Matrix::perspective(_near, _far, canvasWidth / canvasHeight, 60.0f);
    Vec3 from = Vec3(0, 10, -100);
    Vec3 look = Vec3(from.x, from.y, from.z + 1);
    //draw object init
    Cube cube;
    cube.init(&core, &sm, &pm);
    //draw static mesh
    StaticMesh staticMesh;
    staticMesh.init(&core, &sm, &pm, "Models/acacia_003.gem");
    BezierCurve bc;
    std::vector<Vec3> cPoints = { Vec3(0,0,0),Vec3(5,0,0),Vec3(2,0,2) };
    bc.loadPoints(cPoints);
    int frameCount = 0;
    //draw trex animation
    AnimatedModel trex;
    trex.init(&core, &sm, &pm, &tm, "Models/Trex.gem");
    AnimationInstance instance;
    instance.init(&trex.animation);
    // lab
    Land land;
    land.init(&core, &sm, &pm);
    Water water;
    water.init(&core, &sm, &pm);
    // lab end
    //timer init
    float t = 0;
    GamesEngineeringBase::Timer timer;
    bool running = true;
    while (running) {
        bool wireFrameMode = false;
        int pre_x = win.mousex;
        int pre_y = win.mousey;

        core.beginFrame();
        win.processMessages();
        core.beginRenderPass();
        
        float dt = timer.dt();
        t += dt;
        
        Matrix scale = Matrix::scaling(Vec3(0.01f, 0.01f, 0.01f));;
        InputCheck(win, wireFrameMode, running, from, look, pre_x, pre_y);
        Matrix v = Matrix::lookAt(from, look, Vec3(0, 1, 0));
        Matrix vp = v * persM;
#if 0
        //cube test
        Matrix cubePos = Matrix::translation(cPoints[0]);
        cube.update(cubePos, vp);
        cube.draw( wireFrameMode);
        cubePos = Matrix::translation(cPoints[1]);
        cube.update(cubePos, vp);
        cube.draw(wireFrameMode);
        cubePos = Matrix::translation(cPoints[2]);
        cube.update(cubePos, vp);
        cube.draw(wireFrameMode);
#endif   
#if 0
        //tree test (using bezier curve)
        Matrix treePos;
        frameCount++;
        treePos = Matrix::translation(bc.curPoint((float)(abs(frameCount % 200 - 100)) / 100));
        staticMesh.update(scale * treePos, vp);
        staticMesh.draw(wireFrameMode);
#endif     
#if 1
        //Trex test
        Matrix trexPos = Matrix::translation(Vec3(0, 0, 0));
        instance.update("run", dt);
        if (instance.animationFinished() == true) {
            instance.resetAnimationTime();
        }
        trex.update(scale * trexPos, vp, &instance);
        trex.draw(wireFrameMode);
#endif
        //lab
#if 0
        Matrix lWPos;
        land.update(lWPos, vp);
        land.draw(wireFrameMode);
#endif
        Matrix waterPos;
        water.waterUpdate(dt);
        water.update(waterPos, vp);
        water.draw(wireFrameMode);

        //lab end

        //draw frame
        core.finishFrame();
    }
    core.flushGraphicsQueue();
    return 0;
}
//int WinMain1(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
//    //window init
//    Window win;
//    win.initWnd(canvasWidth, canvasHeight, "My Window");
//    //core init
//    Core core;
//    core.init(win.hwnd, canvasWidth, canvasHeight);
//    //shader manager init
//    ShaderManager sm;
//    sm.init(&core);
//    //psosmanager init
//    PSOManager pm;
//    //proM init
//    persM = Matrix::perspective(_near, _far, canvasWidth / canvasHeight, 60.0f);
//    //draw object init
//    Cube cube;
//    cube.init(&core, &sm, &pm);
//    //draw static mesh
//    StaticMesh staticMesh;
//    staticMesh.init(&core, &sm, &pm, "Resources/acacia_003.gem");
//    BezierCurve bc;
//    std::vector<Vec3> cPoints = { Vec3(0,0,0),Vec3(5,0,0),Vec3(2,0,2) };
//    bc.loadPoints(cPoints);
//    int frameCount = 0;
//    //lab
//    AnimatedModel trex;
//    trex.init(&core, &sm, &pm, "Resources/Trex.gem");
//    AnimationInstance instance;
//    instance.init(&trex.animation);
//    // lab end
//    //timer init
//    float t = 0;
//    GamesEngineeringBase::Timer timer;
//    while (1) {
//        core.beginFrame();
//        win.processMessages();
//        if (win.keys[VK_ESCAPE] == 1) break;
//        //lookat matrix update
//        float dt = timer.dt();
//        t += dt;
//        Vec3 from = Vec3(11 * cos(t), 5, 11 * sinf(t));
//        Matrix v = Matrix::lookAt(from, Vec3(0, 0, 0), Vec3(0, 1, 0));
//        Matrix vp = v * persM;
//        Matrix scale = Matrix::scaling(Vec3(0.01f, 0.01f, 0.01f));;
//        core.beginRenderPass();
//        //cube test
//        Matrix cubePos = Matrix::translation(cPoints[0]);
//        cube.update(cubePos, vp);
//        cube.draw(&core);
//        cubePos = Matrix::translation(cPoints[1]);
//        cube.update(cubePos, vp);
//        cube.draw(&core);
//        cubePos = Matrix::translation(cPoints[2]);
//        cube.update(cubePos, vp);
//        cube.draw(&core);
//        //tree test (using bezier curve)
//        Matrix treePos;
//        frameCount++;
//        treePos = Matrix::translation(bc.curPoint((float)(abs(frameCount % 200 - 100)) / 100));
//        staticMesh.update(scale * treePos, vp);
//        staticMesh.draw(&core);
//        //Trex test
//        Matrix trexPos = Matrix::translation(Vec3(-5, 0, 0));
//        instance.update("run", dt);
//        if (instance.animationFinished() == true) {
//            instance.resetAnimationTime();
//        }
//        trex.update(scale * trexPos, vp, &instance);
//        trex.draw(&core);
//
//        //draw frame
//        core.finishFrame();
//    }
//    core.flushGraphicsQueue();
//    return 0;
//}

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}