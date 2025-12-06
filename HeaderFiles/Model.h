#pragma once

#include "HeaderFiles/Mesh.h"
#include "HeaderFiles/Core.h"
#include "HeaderFiles/GEMLoader.h"
#include "HeaderFiles/Shader.h"
#include "HeaderFiles/BasicContainer.h"

struct Bone
{
    std::string name;
    Matrix offset;
    int parentIndex;
};
struct Skeleton
{
    std::vector<Bone> bones;
    Matrix globalInverse;
};
struct AnimationFrame
{
    std::vector<Vec3> positions;
    std::vector<Quaternion> rotations;
    std::vector<Vec3> scales;
};
class Cube {
public:
    PSOManager* psos;
    ShaderManager* sm;
    Mesh mesh;

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
    void init(Core* core, ShaderManager* _sm, PSOManager* _psos, Vec3 offset = Vec3(0, 0, 0)) {
        sm = _sm;
        psos = _psos;
        init(core, offset);
        //cbinit(core);
        psos->createPSO(core, "Cube", sm->shaders["StaticVertexShader"].shader, sm->shaders["PixelShader"].shader, mesh.inputLayoutDesc);
    }
    void cbinit(Core* core) {
        for (auto it = sm->shaders.begin(); it != sm->shaders.end(); ++it)
            for (auto it1 = it->second.constantBuffers.begin(); it1 != it->second.constantBuffers.end(); ++it1)
                it1->second.init(core, it1->second.cbSizeInBytes, 1024);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix vp) {
        sm->updateConstant("StaticVertexShader", "staticMeshBuffer", "W", &world);
        sm->updateConstant("StaticVertexShader", "staticMeshBuffer", "VP", &vp);
    }
    void apply(Core* core) {//actually we dont need loop in this example
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["StaticVertexShader"].constantBuffers.begin(); it != sm->shaders["StaticVertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelShader"].constantBuffers.begin(); it != sm->shaders["PixelShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(Core* core) {
        apply(core);
        psos->bind(core, "Cube");
        mesh.draw(core);
    }
};
class StaticMesh {
public:

    ShaderManager* sm;
    std::vector<Mesh> meshes;
    PSOManager* psos;

    void loadMeshes(Core* core, std::string filename) {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh mesh;
            std::vector<STATIC_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
                vertices.push_back(v);
            }
            mesh.init(core, vertices, gemmeshes[i].indices);
            meshes.push_back(mesh);
        }
    }

    void init(Core* core, ShaderManager* _sm, PSOManager* _psos, std::string filename) {
        sm = _sm;
        psos = _psos;
        loadMeshes(core, filename);
        //cbinit(core);
        psos->createPSO(core, "StaticModel", sm->shaders["StaticVertexShader"].shader, sm->shaders["PixelShader"].shader, VertexLayoutCache::getStaticLayout());
    }
    void cbinit(Core* core) {
        for (auto it = sm->shaders.begin(); it != sm->shaders.end(); ++it)
            for (auto it1 = it->second.constantBuffers.begin(); it1 != it->second.constantBuffers.end(); ++it1)
                it1->second.init(core, it1->second.cbSizeInBytes, 1024);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix vp) {
        sm->updateConstant("StaticVertexShader", "staticMeshBuffer", "W", &world);
        sm->updateConstant("StaticVertexShader", "staticMeshBuffer", "VP", &vp);
    }
    void apply(Core* core) {//actually we dont need loop in this example
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["StaticVertexShader"].constantBuffers.begin(); it != sm->shaders["StaticVertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelShader"].constantBuffers.begin(); it != sm->shaders["PixelShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(Core* core) {
        apply(core);
        psos->bind(core, "StaticModel");
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].draw(core);
        }
    }

};
class AnimationSequence {
public:

    std::vector<AnimationFrame> frames;
    float ticksPerSecond;

    Vec3 interpolate(Vec3 p1, Vec3 p2, float t) {
        return ((p1 * (1.0f - t)) + (p2 * t));
    }
    Quaternion interpolate(Quaternion q1, Quaternion q2, float t) {
        return Quaternion::slerp(q1, q2, t);
    }
    float duration() {
        return ((float)frames.size() / ticksPerSecond);
    }
    void calcFrame(float t, int& frame, float& interpolationFact) {
        interpolationFact = t * ticksPerSecond;
        frame = (int)floorf(interpolationFact);
        interpolationFact = interpolationFact - (float)frame;
        frame = min(frame, frames.size() - 1);
    }
    int nextFrame(int frame) {
        return min(frame + 1, frames.size() - 1);
    }
    Matrix interpolateBoneToGlobal(Matrix* matrices, int baseFrame, float interpolationFact, Skeleton* skeleton, int boneIndex) {
        Matrix scale = Matrix::scaling(interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact));
        Matrix rotation = interpolate(frames[baseFrame].rotations[boneIndex], frames[nextFrame(baseFrame)].rotations[boneIndex], interpolationFact).toMatrix();
        Matrix translation = Matrix::translation(interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact));
        Matrix local = scale * rotation * translation;
        if (skeleton->bones[boneIndex].parentIndex > -1) {
            Matrix global = local * matrices[skeleton->bones[boneIndex].parentIndex];
            return global;
        }
        return local;
    }

};
class Animation {
public:
    std::map<std::string, AnimationSequence> animations;
    Skeleton skeleton;

    void calcFrame(std::string name, float t, int& frame, float& interpolationFact) {
        animations[name].calcFrame(t, frame, interpolationFact);
    }
    Matrix interpolateBoneToGlobal(std::string name, Matrix* matrices, int baseFrame, float interpolationFact, int boneIndex) {
        return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
    }
    void calcFinalTransforms(Matrix* matrices, Matrix coordTransform) {
        for (int i = 0; i < bonesSize(); i++) {
            matrices[i] = skeleton.bones[i].offset * matrices[i] * skeleton.globalInverse * coordTransform;
        }
    }
    int bonesSize() const {
        return skeleton.bones.size();
    }
};
class AnimationInstance {
public:
    Animation* animation;
    std::string currentAnimation;
    float t;
    Matrix matrices[256];
    Matrix matricesPose[256]; // This is to store transforms needed for finding bone positions
    Matrix coordTransform;

    void init(Animation* _animation) {
        animation = _animation;
    }
    void resetAnimationTime() {
        t = 0;
    }
    bool animationFinished() {
        if (t > animation->animations[currentAnimation].duration()) {
            return true;
        }
        return false;
    }
    void update(std::string name, float dt) {
        if (name == currentAnimation) {
            t += dt;
        }
        else {
            currentAnimation = name;
            t = 0;
        }
        if (animationFinished() == true) { resetAnimationTime(); }
        int frame = 0;
        float interpolationFact = 0;
        animation->calcFrame(name, t, frame, interpolationFact);
        for (int i = 0; i < animation->bonesSize(); i++) {
            matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, i);
        }
        animation->calcFinalTransforms(matrices, coordTransform);
    }

};
class AnimatedModel {
public:
    ShaderManager* sm;
    std::vector<Mesh> meshes;
    Animation animation;
    PSOManager* psos;

    void loadMeshes(Core* core, std::string filename) {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        GEMLoader::GEMAnimation gemanimation;
        loader.load(filename, gemmeshes, gemanimation);
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh mesh;
            std::vector<ANIMATED_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
                ANIMATED_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
                vertices.push_back(v);
            }
            mesh.init(core, vertices, gemmeshes[i].indices);
            meshes.push_back(mesh);
        }
        memcpy(&animation.skeleton.globalInverse, &gemanimation.globalInverse, 16 * sizeof(float));
        for (int i = 0; i < gemanimation.bones.size(); i++) {
            Bone bone;
            bone.name = gemanimation.bones[i].name;
            memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
            bone.parentIndex = gemanimation.bones[i].parentIndex;
            animation.skeleton.bones.push_back(bone);
        }
        for (int i = 0; i < gemanimation.animations.size(); i++) {
            std::string name = gemanimation.animations[i].name;
            AnimationSequence aseq;
            aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
            for (int n = 0; n < gemanimation.animations[i].frames.size(); n++) {
                AnimationFrame frame;
                for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++) {
                    Vec3 p;
                    Quaternion q;
                    Vec3 s;
                    memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
                    frame.positions.push_back(p);
                    memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
                    frame.rotations.push_back(q);
                    memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
                    frame.scales.push_back(s);
                }
                aseq.frames.push_back(frame);
            }
            animation.animations.insert({ name, aseq });
        }

    }
    void init(Core* core, ShaderManager* _sm, PSOManager* _psos, std::string filename) {
        sm = _sm;
        psos = _psos;
        loadMeshes(core, filename);
        //cbinit(core);
        psos->createPSO(core, "AnimatedModel", sm->shaders["AnimatedVertexShader"].shader, sm->shaders["PixelShader"].shader, VertexLayoutCache::getAnimatedLayout());
    }
    void cbinit(Core* core) {
        for (auto it = sm->shaders.begin(); it != sm->shaders.end(); ++it)
            for (auto it1 = it->second.constantBuffers.begin(); it1 != it->second.constantBuffers.end(); ++it1)
                it1->second.init(core, it1->second.cbSizeInBytes, 1024);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix vp, AnimationInstance* instance) {
        sm->updateConstant("AnimatedVertexShader", "animatedMeshBuffer", "W", &world);
        sm->updateConstant("AnimatedVertexShader", "animatedMeshBuffer", "VP", &vp);
        sm->updateConstant("AnimatedVertexShader", "animatedMeshBuffer", "bones", instance->matrices);
    }
    void apply(Core* core) {//actually we dont need loop in this example
        //In our case:
        // Index 0: Vertex Shader constant buffer(if it exists) 
        // Index 1 : Pixel Shader constant buffer
        for (auto it = sm->shaders["AnimatedVertexShader"].constantBuffers.begin(); it != sm->shaders["AnimatedVertexShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(0, it->second.getGPUAddress());
            it->second.next();
        }
        for (auto it = sm->shaders["PixelShader"].constantBuffers.begin(); it != sm->shaders["PixelShader"].constantBuffers.end(); ++it) {
            core->getCommandList()->SetGraphicsRootConstantBufferView(1, it->second.getGPUAddress());
            it->second.next();
        }
    }
    void draw(Core* core) {
        apply(core);
        psos->bind(core, "AnimatedModel");
        for (int i = 0; i < meshes.size(); i++) {
            meshes[i].draw(core);
        }
    }
};