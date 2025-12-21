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
class Collider {
public:
};
class Camera {
public:
    Vec3 pos;
    Vec3 look;
    Vec3 pos_max = Vec3(0, 10, -40);
	Vec3 pos_min = Vec3(0, 5, -20);
    float lerp = 1;
    void init() {
        pos = pos_max;
		look = Vec3(0, 1, 0);
    }
    void posUpdate(Vec3 offset) {
        pos += offset;
        pos_max += offset;
		pos_min += offset;
    }
    void update(Matrix& view) {
        view = Matrix::lookAt(pos, look, Vec3(0, 1, 0));
        //from = pos;
    }
};
class Animals {//a container for all animated models and certain animations
public:
    std::vector< AnimatedModel> models;
	std::vector<std::string> animationCele;
    //std::map<std::string, AnimatedModel> models;
    void init(Core* core, ShaderManager* sm, PSOManager* pm, TextureManager* tm) {
        AnimatedModel pig;
        pig.init(core, sm, pm, tm, "Models/Pig.gem");
		animationCele.push_back("attack01");
		models.push_back(pig);

        AnimatedModel bull;
        bull.init(core, sm, pm, tm, "Models/Bull-white.gem");
        animationCele.push_back("dance");
        models.push_back(bull);

        AnimatedModel dog;
        dog.init(core, sm, pm, tm, "Models/Doberman.gem");
        animationCele.push_back("bark");
        models.push_back(dog);

        AnimatedModel chicken;
        chicken.init(core, sm, pm, tm, "Models/Hen-brown.gem");
        animationCele.push_back("eating");
        models.push_back(chicken);

        AnimatedModel sheep;
        sheep.init(core, sm, pm, tm, "Models/Sheep-01.gem");
        animationCele.push_back("stand to sit");
        models.push_back(sheep);

        AnimatedModel goat;
        goat.init(core, sm, pm, tm, "Models/Goat-01.gem");
        animationCele.push_back("attack01");
        models.push_back(goat);
    }
};
class Player {
public:
    Vec3 pos;
	Vec3 forward;
    StaticMesh* model;
	Matrix worldMatrix;
    Water* water;
	float speed = 0.3f;
    Vec3 waterNormal;
	Camera* camera;


    static inline float dot2(float ax, float az, float bx, float bz) {
        return ax * bx + az * bz;
    }
    static inline float len(float ax, float az) {
        return std::sqrt(ax * ax + az * az);
    }
    static inline void normalize2(float& x, float& z) {
        float l = std::sqrt(x * x + z * z);
        if (l > 1e-8f) { x /= l; z /= l; }
    }


    void init(StaticMesh* _model, Vec3 startPos, Water* _water, Camera* _camera) {
        model = _model;
        pos = startPos;
        water = _water;
        camera = _camera;
		//make the player face to positive z direction
		forward = Vec3(0, 0, 1);
        worldMatrix = Matrix::translation(pos) * Matrix::rotateY(-M_PI/2);
    }
    void edgeCheck(Vec3& offset) {

        const float R = 70;
        const float eps = 1e-6f;

        float px = pos.x, pz = pos.z;
        float vx = offset.x, vz = offset.z;

        float plen = len(px, pz);
        if (plen > R + 1e-4f) {
            float nx = px, nz = pz;
            normalize2(nx, nz);
            px = nx * R;
            pz = nz * R;
        }

        if (std::abs(vx) + std::abs(vz) < eps) return;

        float nxp = px + vx;
        float nzp = pz + vz;
        if (nxp * nxp + nzp * nzp <= R * R + 1e-6f) {
            return;
        }

        float A = dot2(vx, vz, vx, vz);
        float B = 2.0f * dot2(px, pz, vx, vz);
        float C = dot2(px, pz, px, pz) - R * R;

        if (A < eps) {
            offset.x = 0.0f; offset.z = 0.0f;
            return;
        }

        float disc = B * B - 4.0f * A * C;
        if (disc < 0.0f) {
            float cx = px, cz = pz; normalize2(cx, cz);
            float v_out = vx * cx + vz * cz;
            if (v_out > 0.0f) { vx -= v_out * cx; vz -= v_out * cz; }
            offset.x = vx; offset.z = vz;
            return;
        }

        float sqrt_disc = sqrt(disc);
        float t1 = (-B - sqrt_disc) / (2.0f * A);
        float t2 = (-B + sqrt_disc) / (2.0f * A);

        float t_hit = 1.0f;
        if (t1 >= 0.0f && t1 <= 1.0f) t_hit = min(t_hit, t1);
        if (t2 >= 0.0f && t2 <= 1.0f) t_hit = min(t_hit, t2);

        if (t_hit >= 1.0f) {
            float cx = px, cz = pz; normalize2(cx, cz);
            float v_out = vx * cx + vz * cz;
            if (v_out > 0.0f) { vx -= v_out * cx; vz -= v_out * cz; }
            offset.x = vx; offset.z = vz;
            return;
        }

        float hitx = px + t_hit * vx;
        float hitz = pz + t_hit * vz;

        float remx = (1.0f - t_hit) * vx;
        float remz = (1.0f - t_hit) * vz;

        float nx_hit = hitx, nz_hit = hitz;
        normalize2(nx_hit, nz_hit); 

        float n_dot_rem = remx * nx_hit + remz * nz_hit;
        float slide_x = remx - n_dot_rem * nx_hit;
        float slide_z = remz - n_dot_rem * nz_hit;

        offset.x = t_hit * vx + slide_x;
        offset.z = t_hit * vz;
    }
    void move(int flag) {//flag == 1, move forward; 0, stop; -1, backward
        if (flag == 1) {
			Vec3 offset = forward * speed * flag;
            edgeCheck(offset);
            worldMatrix = worldMatrix * Matrix::translation(offset);
            pos += offset;
            camera->posUpdate(offset);
            camera->look = pos;
        }

        else if (flag == -1) {
            Vec3 offset = forward * speed * flag * 0.5;
            edgeCheck(offset);
            worldMatrix = worldMatrix * Matrix::translation(offset);
            pos += offset;
			camera->posUpdate(offset);
            //camera->pos += offset;
            camera->look = pos;
        }
    }

    void rotate(int flag) {
        forward = Matrix::rotateY(flag * 0.05f).mulVec(forward);
        worldMatrix = Matrix::rotateY(flag * 0.05f) * worldMatrix;
    }
    void waveActivate(float dt) {
        static float totalTime = 0.0f;
        totalTime += dt;
        if (totalTime > 2) {
			totalTime = 0.0f;
			water->waves.disturb(
                static_cast<int>(( - pos.z + 0.5f * 0.8f * (water->waves.rows - 1)) / 0.8f),
                static_cast<int>((pos.x + 0.5f * 0.8f * (water->waves.cols - 1)) / 0.8f),
				1.f);
        }
    }
    void update(Matrix& viewMatrix, Matrix& projMatrix, Vec3& lightPos, float dt) {
        //getWaterNormal();
        //worldMatrixUpdate();
		static float totalTime = 0.0f;
        totalTime += dt;
        Matrix currWorldMatrix = worldMatrix;
        Vec3 floating = Vec3(0, sinf(totalTime) * 0.2, 0);
        currWorldMatrix = worldMatrix * Matrix::translation(floating);// *Matrix::rotateAxis(forward, sinf(t));

		lightPos = pos + Vec3(RenderTexture::getLightOffset(totalTime) / 2, 10, RenderTexture::getLightOffset(totalTime) / 2);
        lightPos += forward * 2;

        model->update(currWorldMatrix, viewMatrix, projMatrix);
		waveActivate(dt);
    }
    void draw(bool wireframe) {
        model->draw(wireframe);
	}
};
class AnimatedObject {
public:
	AnimatedModel* model;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    float animationSpeed;
    AnimationInstance instance;
    void init(AnimatedModel* _model, Vec3 _position, Vec3 _rotation, Vec3 _scale, float _animationSpeed) {
        model = _model;
        position = _position;
        rotation = _rotation;
        scale = _scale;
        animationSpeed = _animationSpeed;
        instance.init(&model->animation);

    }
    void update(float dt) {
        // Update animation state
    }
};
class GameManager {
public:
    bool animalExist = false;
    float appearCD = 2;
    float appearCDMax = 2;
	float range = 50.0f;
	Vec3 animalPos;
	bool animalPicking = false;

    Matrix animalPosM = Matrix::translation(Vec3(0, 0, 0));
	Matrix animalRotM = Matrix::rotateX(-M_PI / 2);//make the animal face to sky
    Matrix animalScaleM = Matrix::scaling(Vec3(0.03f, 0.03f, 0.03f));

    AnimationInstance instance;
	AnimatedModel* animal;
	Animals* animals;
	int animalIndex;

	Player* player;
	Water* water;
	float playerRange = 5.0f;
    void init(Animals* _animals, Player* _player, Water* _water) {
        animals = _animals;
        player = _player;
		water = _water;
    }
    void update(float dt) {
        
        if (!animalExist) {
            appearCD -= dt;
            if(appearCD <= 0) {
                animalAppear();
				std::cout << "An animal has appeared!" << std::endl;
			}
        }
        else {
			static float totalTime = 0.0f;
			totalTime += dt;
            if (totalTime > 2.0f) {
				totalTime = 0.0f;
                water->waves.disturb(
                    static_cast<int>((- animalPos.z + 0.5f * 0.8f * (water->waves.rows - 1)) / 0.8f),
                    static_cast<int>((animalPos.x + 0.5f * 0.8f * (water->waves.cols - 1)) / 0.8f),
					4.f);
            }

			appearCD = appearCDMax;
			float distance = (animalPos - player->pos).length();
            if (distance < playerRange) {
                animalPick();
				std::cout << "The animal has been picked!" << std::endl;
            }
        }

	}
    void animalAppear() {
        animalExist = true;
        animalPos = Vec3(((float)rand() / RAND_MAX) * range - range / 2, 0, ((float)rand() / RAND_MAX) * range - range / 2);
		//spawn animal at animalPos
        animalIndex = rand() % animals->models.size();
		animal = &animals->models[animalIndex]; // Randomly select an animal model
        instance.init(&animal->animation);
		animalRotM = animalRotM * Matrix::rotateY(((float)rand() / RAND_MAX) * 2 * M_PI);
		animalPosM = Matrix::translation(animalPos);
    }
    void drawAnimal(float dt, bool wireFrameMode, Matrix& view, Matrix& proj) {
        if (animalExist) {
            //draw animal
            if (animalPicking) {
				static float totalTime = 0.0f;
                totalTime += dt;

				Matrix animalPosP = Matrix::translation(Vec3(animalPos.x, 1.0f + totalTime, animalPos.z));

                instance.update(animals->animationCele[animalIndex], dt);
                if (instance.animationFinished() == true) {
                    instance.resetAnimationTime();
                }
                animal->update(animalScaleM * animalPosP, view, proj, &instance);

                if (totalTime > 2.0f) {
                    animalExist = false;
					animalPicking = false;
					totalTime = 0.0f;
                }
            }
            else {
                instance.update("run forward", dt);
                if (instance.animationFinished() == true) {
                    instance.resetAnimationTime();
                }
                animal->update(animalScaleM * animalRotM * animalPosM, view, proj, &instance);
            }
            
            animal->draw(wireFrameMode);
		}
    }
    void animalPick() {
        //animalExist = false;
		animalPicking = true;

    }
};
//-------------------
void InputCheck(Window& win, bool& wireFrameMode, bool& running, Player& player, int pre_x, int pre_y, Camera& camera) {
    if (win.keys[VK_ESCAPE] == 1) running = false;
    if (win.keys['1'] == 1) wireFrameMode = true;

    if (win.keys['W']) player.move(1);
    else if (win.keys['S']) player.move(-1);
    else player.move(0);
    if (win.keys['A']) player.rotate(1);
	else if (win.keys['D']) player.rotate(-1);
    else player.rotate(0);
    float sensitive = 0.5;
    int threshold = 1;
    if (win.mouseButtons[0]) {
        int dx = win.mousex - pre_x;
        if (std::abs(dx) < threshold) dx = 0;

        Vec3 from1 = camera.pos - camera.look;
		from1 = Matrix::rotateY(-sensitive * static_cast<float>(dx) * M_PI / 180.f).mulVec(from1);
        camera.pos = from1 + camera.look;
    }
	//from = camera.pos;
#if DEBUG_MODE
    //lookat matrix update
    /*
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
    look += offset1 + offset2;*/
#endif
}
//-------------------
Matrix persM;
float _near = 0.1f;
float _far = 1000.f;
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
    pm.createPSO(&core, "MRTCompose", sm.shaders["MRTVS"].shader, sm.shaders["MRTPS"].shader, { NULL, 0 });
    //texturemanager init
    TextureManager tm;
    //Camera init
    persM = Matrix::perspective(_near, _far, canvasWidth / canvasHeight, 60.0f);
    Vec3 from = Vec3(0, 10, -100);
    Vec3 look = Vec3(from.x, from.y, from.z + 1);

    // lab
    StaticMesh boat;
    boat.init(&core, &sm, &pm, &tm, "Models/Warehouse_Boat_Full_01a.gem");
    Land land;
    land.init(&core, &sm, &pm, &tm);
    Water water;
    water.init(&core, &sm, &pm, &tm);
    InstanceTreeModel trees;
    trees.init(&core, &sm, &pm, &tm, "Models/Maple.gem");
    InstanceGrassModel grass;
    grass.init(&core, &sm, &pm, &tm, "Models/flower1.gem");
	SkyDome skyDome;
	skyDome.init(&core, &sm, &pm, &tm);
    Camera camera;
    camera.init();
	Player player;
	player.init(&boat, Vec3(0, 0.5, 0), &water, &camera);
    
    Animals animals;
    animals.init(&core, &sm, &pm, &tm);

	GameManager gm;
    gm.init(&animals, &player, &water);
	
    // lab end
    // light init
    Vec3 gLightPosWS;
    float gLightRadius = 150.0f;
    Vec3 gLightColor = Vec3(1.0, 0.95, 0.8);
    float gLightIntensity = 150.0f;
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
        InputCheck(win, wireFrameMode, running, player, pre_x, pre_y, camera);
		//from = camera.pos;
        Matrix v;
        //Matrix v = Matrix::lookAt(from, look, Vec3(0, 1, 0));
        camera.update(v);
#if 0
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
#if 1
        gm.update(dt);
        gm.drawAnimal(dt, wireFrameMode, v, persM);
        /*Matrix animalPos = Matrix::translation(Vec3(0, 0, 0));
		Matrix animalRot = Matrix::rotateX( - M_PI /2);
		Matrix animalScale = Matrix::scaling(Vec3(0.03f, 0.03f, 0.03f));
        instance.update("run forward", dt);
        if (instance.animationFinished() == true) {
            instance.resetAnimationTime();
        }
        animal->update(animalScale * animalPos * animalRot, v, persM, &instance);
        animal->draw(wireFrameMode);*/
#endif
#if 1
        //camera.update(player, v, from);
        player.update(v, persM, gLightPosWS, dt);
		player.draw(wireFrameMode);
#endif
#if 1
        Matrix lWPos = Matrix::translation(Vec3(0, -10, 0));
        land.update(lWPos, v, persM);
        land.draw(wireFrameMode);
#endif
#if 1
        Matrix waterPos;
        water.waterUpdate(dt);
        water.update(waterPos, v, persM, t);
        water.draw(wireFrameMode);
#endif
        //lab end
#if 1
		trees.update(v, persM, t);
		trees.draw(wireFrameMode);
        grass.update(v, persM, t);
		grass.draw(wireFrameMode);
#endif
#if 1
        Matrix skyPos;
        skyDome.update(skyPos, v, persM, t);
		skyDome.draw(wireFrameMode);
#endif
        //MRT render
        core.beginMRTRenderPass();
        pm.bind(&core, "MRTCompose");
		//constant update
		RenderTexture::apply(sm, core);
		Matrix invV = v.invert();
		Matrix invP = persM.invert();
        sm.updateConstant("MRTPS", "cameraBuffer", "gInvView", &invV);
        sm.updateConstant("MRTPS", "cameraBuffer", "gInvProj", &invP);
		sm.updateConstant("MRTPS", "cameraBuffer", "gCameraPosWS", &camera.pos);

        //Vec3 gLightPosWS = Vec3(0, 5, -10);
       
        sm.updateConstant("MRTPS", "lightBuffer", "time", &t);
        sm.updateConstant("MRTPS", "lightBuffer", "gLightPosWS", &gLightPosWS);
        sm.updateConstant("MRTPS", "lightBuffer", "gLightRadius", &gLightRadius);
        sm.updateConstant("MRTPS", "lightBuffer", "gLightColor", &gLightColor);
        sm.updateConstant("MRTPS", "lightBuffer", "gLightIntensity", &gLightIntensity);
        //test lights
		int numLights = 10;
        Vec4 gLightPosWSArray[10];
		RenderTexture::lightsArrayUpdate(t, gLightPosWSArray);
		sm.updateConstant("MRTPS", "lightBuffer", "gNumLights", &numLights);
		sm.updateConstant("MRTPS", "lightBuffer", "gLightPosWSArray", &gLightPosWSArray);

		RenderTexture::draw(core);

        static int frameCount = 0;
		static float timeElapsed = 0.0f;
		frameCount++;
		timeElapsed += dt;
        if (timeElapsed >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            timeElapsed = 0.0f;
            frameCount = 0;
        }
		
        //draw frame
        core.finishFrame();
    }
    core.flushGraphicsQueue();
    return 0;
}
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}