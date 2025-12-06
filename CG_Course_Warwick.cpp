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
#include "HeaderFiles/GEMLoader.h"
#include "HeaderFiles/Shader.h"
#include "HeaderFiles/BasicContainer.h"
#include "HeaderFiles/Model.h"

//-------------------

Matrix proM;
float _near = 0.1f;
float _far = 10000.f;
static const float canvasWidth = 1920;
static const float canvasHeight = 1080;
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    //window init
    Window win;
    win.initWnd(canvasWidth, canvasHeight, "My Window");
    //core init
    Core core;
    core.init(win.hwnd, canvasWidth, canvasHeight);
    //shader manager init
    ShaderManager sm;
    sm.init(&core);
    //psosmanager init
    PSOManager pm;
    //proM init
    proM = Matrix::perspective(_near, _far, canvasWidth / canvasHeight, 60.0f);
    //draw object init
    Cube cube;
    cube.init(&core, &sm, &pm);
    //draw static mesh
    StaticMesh staticMesh;
    staticMesh.init(&core, &sm, &pm, "Resources/acacia_003.gem");
    BezierCurve bc;
    std::vector<Vec3> cPoints = { Vec3(0,0,0),Vec3(5,0,0),Vec3(2,0,2) };
    bc.loadPoints(cPoints);
    int frameCount = 0;
    //lab
    AnimatedModel trex;
    trex.init(&core, &sm, &pm, "Resources/Trex.gem");
    AnimationInstance instance;
    instance.init(&trex.animation);
    // lab end
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
        Matrix v = Matrix::lookAt(from, Vec3(0, 0, 0), Vec3(0, 1, 0));
        Matrix vp = v * proM;
        Matrix scale = Matrix::scaling(Vec3(0.01f, 0.01f, 0.01f));;
        core.beginRenderPass();
        //cube test
        Matrix cubePos = Matrix::translation(cPoints[0]);
        cube.update(cubePos, vp);
        cube.draw(&core);
        cubePos = Matrix::translation(cPoints[1]);
        cube.update(cubePos, vp);
        cube.draw(&core);
        cubePos = Matrix::translation(cPoints[2]);
        cube.update(cubePos, vp);
        cube.draw(&core);
        //tree test (using bezier curve)
        Matrix treePos;
        frameCount++;
        treePos = Matrix::translation(bc.curPoint((float)(abs(frameCount % 200 - 100)) / 100));
        staticMesh.update(scale * treePos, vp);
        staticMesh.draw(&core);
        //Trex test
        Matrix trexPos = Matrix::translation(Vec3(-5, 0, 0));
        instance.update("run", dt);
        if (instance.animationFinished() == true) {
            instance.resetAnimationTime();
        }
        Matrix temp = scale * trexPos;
        trex.update(temp, vp, &instance);
        trex.draw(&core);

        //draw frame
        core.finishFrame();
    }
    core.flushGraphicsQueue();
    return 0;
}

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}