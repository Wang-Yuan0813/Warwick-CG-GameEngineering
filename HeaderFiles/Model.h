#pragma once

#include "Mesh.h"
#include "Texture.h"

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
static class InstanceTool {
public:
    static void generateW(std::vector<INSTANCE>& instances, int num, float scale, Vec3 posOffset, float range = 50.0f) {
        instances.resize(num);
        Matrix scaleM = Matrix::scaling(Vec3(scale, scale, scale));
        Matrix offset = Matrix::translation(posOffset);
        for (int i = 0; i < num; i++) {
            instances[i].w = scaleM * 
                Matrix::translation(Vec3(((float)rand() / RAND_MAX) * range - range / 2, 0, ((float)rand() / RAND_MAX) * range - range / 2)) * 
                offset;
        }
    }

    static float getHillsHeight(float x, float z) {
        const float kScale = 0.006f; 
        const float kAmp = 280.0f;  
        const float kRippleA = 0.25f; 
        const float kRippleF = 0.06f;  

        const float k = 0.2f;

        float r = sqrtf(x * x + z * z);

        float t = r * kScale;
        float hBowl = (t * t) / (1.0f + t * t);

        float ripples = kRippleA * sinf(kRippleF * x) * cosf(kRippleF * z);

        return k * (kAmp * hBowl + ripples);
    }

    static Vec3 getHillsNormal(float x, float z)
    {
        const float kScale = 0.006f;
        const float kAmp = 280.0f;
        const float kRippleA = 0.25f;
        const float kRippleF = 0.06f;
        const float k = 0.2f;
		float nx, ny, nz;

        float r = std::sqrt(x * x + z * z);
        float rSafe = max(r, 1e-6f);

        float t = r * kScale;

        float denom = 1.0f + t * t;
        float dhdt = (2.0f * t) / (denom * denom);
        float dtdx = kScale * (x / rSafe);
        float dtdz = kScale * (z / rSafe);
        float dBdx = dhdt * dtdx; 
        float dBdz = dhdt * dtdz; 

        float sinFx = std::sinf(kRippleF * x);
        float cosFx = std::cosf(kRippleF * x);
        float sinFz = std::sinf(kRippleF * z);
        float cosFz = std::cosf(kRippleF * z);

        float dRdx = kRippleA * kRippleF * (cosFx * cosFz);     
        float dRdz = -kRippleA * kRippleF * (sinFx * sinFz);    

        float hx = k * (kAmp * dBdx + dRdx);
        float hz = k * (kAmp * dBdz + dRdz);

        nx = -hx;
        ny = 1.0f;
        nz = -hz;

        float len = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (len > 0.0f) {
            nx /= len;
            ny /= len;
            nz /= len;
        }
        else {
            nx = 0.0f; ny = 1.0f; nz = 0.0f;
        }
        return Vec3(nx, ny, nz);
    }

    static float getHillsHeight1(float x, float z) {
        return 0.2f * (z * sinf(0.05f * x) + x * cosf(0.05f * z));
    }
    static Vec3 getHillsNormal1(float x, float z) {
        Vec3 n(-0.01f * x * sinf(0.05f * z) - 0.2f * cosf(0.05f * x), 1.0f, -0.2f * sinf(0.05f * z) + 0.01f * z * cosf(0.05f * x));
        n.normalize();
        return n;
    }
    static Vec3 getPlantPos(float width, float length) {//generate correct position according to land height
        float xPos;
        float zPos;
        float yPos;
        do {
            xPos = ((float)rand() / RAND_MAX) * width - width / 2;
            zPos = ((float)rand() / RAND_MAX) * length - length / 2;
            yPos = getHillsHeight(xPos, zPos);
        } while (yPos < 10);
        return Vec3(xPos, yPos - 11, zPos);
    }
    static void generateWbyLand(std::vector<INSTANCE>& instances, int num, float scale, Vec3 posOffset, float width, float length) {
        instances.resize(num);
        Matrix scaleM = Matrix::scaling(Vec3(scale, scale, scale));
        Matrix offset = Matrix::translation(posOffset);
        for (int i = 0; i < num; i++) {
            instances[i].w = scaleM *
                Matrix::translation(getPlantPos(width, length)) *
                offset;
        }
    }
};
//class Cube {
//public:
//    PSOManager* psos;
//    ShaderManager* sm;
//    //Mesh mesh;
//    InstanceMesh mesh;
//    Core* core;
//    Shader* vs;
//    Shader* ps;
//    void init(Core* _core, Vec3 offset = Vec3(0, 0, 0)) {
//        core = _core;
//        std::vector<STATIC_VERTEX> vertices;
//        Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f) + offset;
//        Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f) + offset;
//        Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f) + offset;
//        Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f) + offset;
//        Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f) + offset;
//        Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f) + offset;
//        Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f) + offset;
//        Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f) + offset;
//
//        Vec3 t = Vec3(0.0f, 0.0f, 0.0f);//for now
//
//        vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), t, 0.0f, 0.0f));
//        vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), t, 0.0f, 0.0f));
//        vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), t, 0.0f, 0.0f));
//
//        vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), t, 0.0f, 0.0f));
//        vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), t, 0.0f, 0.0f));
//        vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), t, 0.0f, 1.0f));
//        vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), t, 1.0f, 1.0f));
//        vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), t, 1.0f, 0.0f));
//        vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), t, 0.0f, 0.0f));
//
//        std::vector<unsigned int> indices;
//        indices.push_back(0); indices.push_back(1); indices.push_back(2);
//        indices.push_back(0); indices.push_back(2); indices.push_back(3);
//        indices.push_back(4); indices.push_back(5); indices.push_back(6);
//        indices.push_back(4); indices.push_back(6); indices.push_back(7);
//        indices.push_back(8); indices.push_back(9); indices.push_back(10);
//        indices.push_back(8); indices.push_back(10); indices.push_back(11);
//        indices.push_back(12); indices.push_back(13); indices.push_back(14);
//        indices.push_back(12); indices.push_back(14); indices.push_back(15);
//        indices.push_back(16); indices.push_back(17); indices.push_back(18);
//        indices.push_back(16); indices.push_back(18); indices.push_back(19);
//        indices.push_back(20); indices.push_back(21); indices.push_back(22);
//        indices.push_back(20); indices.push_back(22); indices.push_back(23);
//
//        int instanceNum = 50;
//		float scale = 0.2f;
//		Vec3 posOffset = Vec3(0, 0, 0);
//		std::vector<INSTANCE> instances;
//		InstanceTool::generateW(instances, instanceNum, scale, posOffset, 50.0f);
//
//        mesh.init(core, vertices, indices, instances);
//
//    }
//    
//    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, Vec3 offset = Vec3(0, 0, 0)) {
//        sm = _sm;
//        psos = _psos;
//        core = _core;
//        init(core, offset);
//        //vs = &sm->shaders["StaticVertexShader"];
//        vs = &sm->shaders["InstanceStaticVertexShader"];
//        ps = &sm->shaders["PixelColourShader"];
//        psos->createPSO(core, "Cube", vs->shader, ps->shader, VertexLayoutCache::getInstanceLayout());
//        psos->createPSO(core, "CubeWire", vs->shader, ps->shader, VertexLayoutCache::getInstanceLayout(), true);
//    }
//
//    //change variable by searching constantbuffer's name and variable's name
//    void update(Matrix world, Matrix vp) {
//        sm->updateConstant(vs->name, "staticMeshBuffer", "W", &world);
//        sm->updateConstant(vs->name, "staticMeshBuffer", "VP", &vp);
//    }
//    void apply() {
//        for (int i = 0; i < vs->constantBuffers.size(); i++) {
//            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
//            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
//            vs->constantBuffers[i].next();
//        }
//        for (int i = 0; i < ps->constantBuffers.size(); i++) {
//            int parameterIndex = core->getRootParameterIndex("cbps") + i;
//            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
//            ps->constantBuffers[i].next();
//        }
//    }
//    void draw(bool wireFrameMode = false) {
//        apply();
//        if(wireFrameMode)   psos->bind(core, "CubeWire");
//        else    psos->bind(core, "Cube");
//        mesh.draw(core);
//    }
//};
class StaticMesh {
public:

    ShaderManager* sm;
    std::vector<Mesh> meshes;
    PSOManager* psos;
    Core* core;
	Shader* vs;
	Shader* ps;
    std::vector<std::vector<std::string>> textureProperties;//store multiple texture properties for each mesh
    TextureManager* tm;

    void loadMeshes(std::string filename) {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
        textureProperties.resize(gemmeshes.size());
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh mesh;
            std::vector<STATIC_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
                vertices.push_back(v);
            }
            //get all texture properties for this mesh
            for (int n = 0; n < gemmeshes[i].material.properties.size(); n++) {
                textureProperties[i].push_back(gemmeshes[i].material.properties[n].getValue());
                tm->add(core, textureProperties[i][n]);
            }
            mesh.init(core, vertices, gemmeshes[i].indices);
            meshes.push_back(mesh);
        }
    }

    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm, std::string filename) {
        sm = _sm;
        psos = _psos;
        core = _core;
        tm = _tm;

        loadMeshes(filename);

		vs = &sm->shaders["StaticVertexShader"];
		ps = &sm->shaders["TextureBoatPS"];

        psos->createPSO(core, "StaticModel", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout());
        psos->createPSO(core, "StaticModelWire", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout(), true);

    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix view, Matrix proj) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "W", &world);
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
		sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
    }
    void apply() {//actually we dont need loop in this example
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
            ps->constantBuffers[i].next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if(wireFrameMode)   psos->bind(core, "StaticModelWire");
        else                psos->bind(core, "StaticModel");
        for (int i = 0; i < meshes.size(); i++)
        {
            for (int n = 0; n < textureProperties[i].size(); n++) {
                ps->updateTexturePS(core, "tex" + std::to_string(n), tm->find(textureProperties[i][n]));
            }
            meshes[i].draw(core);
        }
    }

};
//----this class generate instance static model
class InstanceStaticModel {
public:
    PSOManager* psos;
    ShaderManager* sm;
    std::vector<InstanceMesh> meshes;
    Core* core;
    Shader* vs;
    Shader* ps;
    //texture info
	std::vector<std::vector<std::string>> textureProperties;//store multiple texture properties for each mesh
    TextureManager* tm;
    //instance info
    int instanceNum;
    float scale;
    Vec3 posOffset;//the center position of a instances cluster
    std::vector<INSTANCE> instances;//info of instances
	std::string psoName;

    virtual void bindShaders() {
        vs = &sm->shaders["InstanceStaticVertexShader"];
        ps = &sm->shaders["TexturePixelShader"];
        psoName = "Instance";
    }
    virtual void loadMeshes(std::string filename) {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
        for (int i = 0; i < gemmeshes.size(); i++) {
            InstanceMesh mesh;
            std::vector<STATIC_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
                vertices.push_back(v);
            }
            //get all texture properties for this mesh
            for (int n = 0; n < gemmeshes[i].material.properties.size(); n++) {
                textureProperties[i].push_back(gemmeshes[i].material.properties[n].getValue());
                tm->add(core, textureProperties[i][n]);
            }

            mesh.init(core, vertices, gemmeshes[i].indices, instances);
            meshes.push_back(mesh);
        }
    }
    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm, std::string filename,
        int _instanceNum = 500, float _scale = 0.02f, Vec3 _posOffset = Vec3(0, 0, 0)) {
        sm = _sm;
        psos = _psos;
        core = _core;
		tm = _tm;

        instanceNum = _instanceNum;
        scale = _scale;
        posOffset = _posOffset;
        //InstanceTool::generateW(instances, instanceNum, scale, posOffset, range);
        InstanceTool::generateWbyLand(instances, instanceNum, scale, posOffset, 310, 310);
        loadMeshes(filename);

		bindShaders();

        psos->createPSO(core, psoName, vs->shader, ps->shader, VertexLayoutCache::getInstanceLayout());
        psos->createPSO(core, psoName + "Wire", vs->shader, ps->shader, VertexLayoutCache::getInstanceLayout(), true);
    }
    
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix vp) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "VP", &vp);
    }
    void apply() {
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
			int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
			ps->constantBuffers[i].next();
        }
    }
    virtual void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, psoName + "Wire");
        else    psos->bind(core, psoName);

        for (int i = 0; i < meshes.size(); i++)
        {
            for (int n = 0; n < textureProperties[i].size(); n++) {
                ps->updateTexturePS(core, "tex" + std::to_string(n), tm->find(textureProperties[i][n]));
            }
            meshes[i].draw(core);
        }
    }
};
//----this class generate instance static model for trees, use green channel to indicate tree model
class InstanceTreeModel : public InstanceStaticModel {
public:
    void bindShaders() override {
        vs = &sm->shaders["InstanceTreeVS"];
        ps = &sm->shaders["TextureTreePS"];
		psoName = "InstanceTree";
	}
    void loadMeshes(std::string filename) override {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
		textureProperties.resize(gemmeshes.size());//different mesh may have different number of texture properties
        for (int i = 0; i < gemmeshes.size(); i++) {
            InstanceMesh mesh;
            std::vector<STATIC_VERTEX> vertices;
			//get all texture properties for this mesh
            for(int n = 0; n < gemmeshes[i].material.properties.size(); n++) {
				textureProperties[i].push_back(gemmeshes[i].material.properties[n].getValue());
                tm->add(core, textureProperties[i][n]);
			}
            //check if is leaf mesh
            float g = 0.0f;
            if (textureProperties[i][0].find("leaf") != std::string::npos) {
                g = 1.0f;
            }
			//load vertices
            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                //memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX)); //GEMStaticVertex
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(GEMLoader::GEMStaticVertex));
				v.color.g = g;//set green channel to 0 to indicate this is tree model
                vertices.push_back(v);
            }

            mesh.init(core, vertices, gemmeshes[i].indices, instances);
            meshes.push_back(mesh);
        }
    }
    void update(Matrix view, Matrix proj, float t)  {
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
		sm->updateConstant(vs->name, "timeBuffer", "time", &t);
    }
};
class InstanceGrassModel : public InstanceStaticModel {
public:
    void bindShaders() override {
        vs = &sm->shaders["InstanceGrassVS"];
        ps = &sm->shaders["TextureTreePS"];//test
		psoName = "InstanceGrass";
    }
    void loadMeshes(std::string filename) override {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        loader.load(filename, gemmeshes);
        textureProperties.resize(gemmeshes.size());//different mesh may have different number of texture properties
        for (int i = 0; i < gemmeshes.size(); i++) {
            InstanceMesh mesh;
            std::vector<STATIC_VERTEX> vertices;
            //get all texture properties for this mesh
            for (int n = 0; n < gemmeshes[i].material.properties.size(); n++) {
                textureProperties[i].push_back(gemmeshes[i].material.properties[n].getValue());
                tm->add(core, textureProperties[i][n]);
            }
            float g = 1.0f;

            for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
                STATIC_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
                v.color.g = g;//set green channel to 0 to indicate this is tree model
                vertices.push_back(v);
            }

            mesh.init(core, vertices, gemmeshes[i].indices, instances);
            meshes.push_back(mesh);
        }
    }
    void update(Matrix view, Matrix proj, float t) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
        sm->updateConstant(vs->name, "timeBuffer", "time", &t);
    }
};
class GeometryPlane {
public:
    DynamicMesh mesh;
    std::vector<STATIC_VERTEX> vertices;
    std::vector<unsigned int> indices;
    int rowCount;
    int colCount;
    void CreateGrid(Core* core, int m, int n) {
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
        for (int i = 0; i < m; ++i) {
            float z = halfDepth - i * dz;
            for (int j = 0; j < n; ++j) {
                float x = -halfWidth + j * dx;//how to calculate normal vector?
                vertices[i * n + j] = addVertex(Vec3(x, 0.0f, z), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),
                    j * du, i * dv);
            }
        }
        //generate indices
        indices.resize(faceCount * 3);
        int k = 0;
        for (int i = 0; i < m - 1; ++i) {
            for (int j = 0; j < n - 1; ++j) {
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
class GeometrySphere {
public:
    Mesh mesh;
    std::vector<STATIC_VERTEX> vertices;
    std::vector<unsigned int> indices;
    int stackCount;
    int sliceCount;
    void CreateSphere(Core* core, int stacks, int slices, float radius) {

        stackCount = stacks;
        sliceCount = slices;

        if (stacks < 2) stacks = 2;
        if (slices < 3) slices = 3;
        if (radius <= 0.0f) radius = 1.0f;

        const int stride = slices + 1;
        const int vertexCount = stacks * stride;
        vertices.resize(vertexCount);

        const float dTheta = 2.0f * float(M_PI) / float(slices);
        const float dPhi = float(M_PI) / float(stacks - 1);

        const float du = 1.0f / float(slices);
        const float dv = 1.0f / float(stacks - 1);

        for (int i = 0; i < stacks; ++i) {
            float phi = i * dPhi;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            for (int j = 0; j <= slices; ++j) {
                float theta = j * dTheta;
                float sinTheta = sinf(theta);
                float cosTheta = cosf(theta);

                float x = radius * sinPhi * cosTheta;
                float y = radius * cosPhi;
                float z = radius * sinPhi * sinTheta;
                Vec3 pos(x, y, z);

                Vec3 n = pos.normalize();

                Vec3 t(-sinPhi * sinTheta, 0.0f, sinPhi * cosTheta);
                if (t.length() < 1e-6f) {
                    Vec3 up(0.0f, 1.0f, 0.0f);
                    t = up.cross(n);
                    if (t.length() < 1e-6f) {
                        Vec3 right(1.0f, 0.0f, 0.0f);
                        t = right.cross(n);
                    }
                }
                t = t.normalize();

                float u = j * du;
                float v = i * dv;

                vertices[i * stride + j] = addVertex(pos, n, t, u, v);
            }
        }

        const int quadCount = (stacks - 1) * slices;
        indices.resize(quadCount * 6);

        int k = 0;
        for (int i = 0; i < stacks - 1; ++i) {
            for (int j = 0; j < slices; ++j) {
                int i0 = i * stride + j;
                int i1 = i * stride + (j + 1);
                int i2 = (i + 1) * stride + j;
                int i3 = (i + 1) * stride + (j + 1);

                indices[k + 0] = i0;
                indices[k + 1] = i1;
                indices[k + 2] = i2;

                indices[k + 3] = i2;
                indices[k + 4] = i1;
                indices[k + 5] = i3;

                k += 6;
            }
        }
    }
};
class GeometryCylinder {
    public:
    Mesh mesh;
    std::vector<STATIC_VERTEX> vertices;
    std::vector<unsigned int> indices;
    int stackCount;
    int sliceCount;
    void CreateCylinder(Core* core, float radius, float height) {
        for (unsigned int j = 0; j < 65; ++j)
        {
            float tmp = float(j) * (2.f * M_PI / 64.f);
            float cs = cosf(tmp);
            float sn = sinf(tmp);
            float x = radius * cs;
            float z = radius * sn;
            float u = float(j) / 64.0f;

            vertices.push_back(addVertex(Vec3(x, -(height / 3), z), Vec3(cs, 0.f, sn), Vec3(0.f, 0.f, 0.f), u, 1.f));
            vertices.push_back(addVertex(Vec3(x, height * 2 / 3, z), Vec3(cs, 0.f, sn), Vec3(0.f, 0.f, 0.f), u, 0.f));
        }
        for (unsigned int j = 0; j < 64; ++j)
        {
            unsigned int i0 = j * 2;
            unsigned int i1 = j * 2 + 1;
            unsigned int i2 = (j + 1) * 2;
            unsigned int i3 = (j + 1) * 2 + 1;

            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
	}
};
class SkyDome {
public:
    //GeometryCylinder cylinder;
	GeometrySphere sphere;
    PSOManager* psos;
    ShaderManager* sm;
    Core* core;
    Shader* vs;
    Shader* ps;
	TextureManager* tm;
    std::vector<std::string> textureFilenames;
    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm) {
        sm = _sm;
        psos = _psos;
        core = _core;
        tm = _tm;
        vs = &sm->shaders["StaticColourVertexShader"];
        ps = &sm->shaders["SkyPS"];

        textureFilenames.push_back("Models/Textures/sky.jpg");
        tm->add(core, "Models/Textures/sky.jpg");

        buildSkyDomeGeometry();
        psos->createPSO(core, "SkyDome", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout());
        psos->createPSO(core, "SkyDomeWire", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout(), true);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix view, Matrix proj, float time) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "W", &world);
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
        sm->updateConstant(ps->name, "spinBuffer", "time", &time);
    }
    void apply() {//actually we dont need loop in this example
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
            ps->constantBuffers[i].next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, "SkyDomeWire");
        else                psos->bind(core, "SkyDome");
        ps->updateTexturePS(core, "tex0", tm->find(textureFilenames[0]));
        sphere.mesh.draw(core);
    }
    void buildSkyDomeGeometry() {
        sphere.CreateSphere(core, 400 ,400, 500);
        for (int i = 0; i < sphere.vertices.size(); ++i) {
            STATIC_VERTEX& p = sphere.vertices[i];
            p.color = Colour(0.1f, 0.48f, 0.19f, 1.0f);
        }
        sphere.mesh.init(core, sphere.vertices, sphere.indices);
    }
};
class Land {
public:
    GeometryPlane grid;
    PSOManager* psos;
    ShaderManager* sm;
    Core* core;
    Shader* vs;
    Shader* ps;
	TextureManager* tm;
    std::vector<std::string> textureFilenames;

    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm) {
        sm = _sm;
        psos = _psos;
        core = _core;
		tm = _tm;
        vs = &sm->shaders["StaticColourVertexShader"];
        ps = &sm->shaders["GroundPS"];

        textureFilenames.push_back("Models/Textures/ground.png");
        tm->add(core, "Models/Textures/ground.png");
        textureFilenames.push_back("Models/Textures/ground-normal.png");
        tm->add(core, "Models/Textures/ground-normal.png");

        buildLandGeometry();
        psos->createPSO(core, "Land", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout());
        psos->createPSO(core, "LandWire", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout(), true);

    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix view, Matrix proj) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "W", &world);
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
    }
    void apply() {//actually we dont need loop in this example
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
            ps->constantBuffers[i].next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, "LandWire");
        else                psos->bind(core, "Land");
        ps->updateTexturePS(core, "tex0", tm->find(textureFilenames[0]));
        ps->updateTexturePS(core, "tex1", tm->find(textureFilenames[1]));
        grid.mesh.draw(core);
    }
    void buildLandGeometry() {
        grid.CreateGrid(core, 400, 400);//for now
        for (int i = 0; i < grid.vertices.size(); ++i) {
            STATIC_VERTEX& p = grid.vertices[i];
            p.pos.y = InstanceTool::getHillsHeight(p.pos.x, p.pos.z);
			p.normal = InstanceTool::getHillsNormal(p.pos.x, p.pos.z);
            //Color based on height
            if (p.pos.y < 0.0f) {
                // Sandy beach color.
                p.color = Colour(1.0f, 0.96f, 0.62f, 1.0f);
            }
            else if (p.pos.y < 10.0f) {
                // Light yellow-green.
                p.color = Colour(0.48f, 0.77f, 0.46f, 1.0f);
            }
            else {
                // White snow.
                p.color = Colour(0.1f, 0.48f, 0.19f, 1.0f);
            }
        }
        grid.mesh.init(core, grid.vertices, grid.indices);
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
    void init(int _rows, int _cols, float _h, float _dt, float _speed, float _beta) {

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
        for (int i = 1; i < rows - 1; ++i) {
            for (int j = 1; j < cols - 1; ++j) {
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
    std::vector<std::string> textureFilenames;
    TextureManager* tm;
    Shader* vs;
    Shader* ps;

    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm) {
        sm = _sm;
        psos = _psos;
        core = _core;
        tm = _tm;
        buildWaterGeometry();

        /*textureFilenames.push_back("Models/Textures/sea-waves.jpg");
        tm->add(core, "Models/Textures/sea-waves.jpg");*/
        textureFilenames.push_back("Models/Textures/sky.jpg");
        tm->add(core, "Models/Textures/sky.jpg");
        textureFilenames.push_back("Models/Textures/sea-waves-normal.png");
        tm->add(core, "Models/Textures/sea-waves-normal.png");

        vs = &sm->shaders["StaticColourVertexShader"];
        ps = &sm->shaders["WaterPS"];

        psos->createPSO(core, "Water", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout());
        psos->createPSO(core, "WaterWire", vs->shader, ps->shader, VertexLayoutCache::getStaticLayout(), true);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix view, Matrix proj, float time) {
        sm->updateConstant(vs->name, "staticMeshBuffer", "W", &world);
        sm->updateConstant(vs->name, "staticMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "staticMeshBuffer", "Proj", &proj);
        sm->updateConstant(ps->name, "waveBuffer", "time", &time);
    }
    void apply() {
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
            ps->constantBuffers[i].next();
        }
    }

    void buildWaterGeometry() {
        grid.CreateGrid(core, 200, 200);//for now
        waves.init(200, 200, 0.8, 0.02, 5, 0.2);//the third parameter should be change according to CreateGrid()
        for (int i = 0; i < grid.vertices.size(); ++i) {
            grid.vertices[i].color = Colour(0.07f, 0.14f, 0.29f, 1.f);//for now
        }
        grid.mesh.init(core, grid.vertices, grid.indices);
    }
    float getWaterHeight(float x, float z)const {
        return 0.3f * (z * sinf(0.1f * x) + x * cosf(0.1f * z));
    }

    void waterUpdate(float dt) {
        static float totalTime = 0.f;
        totalTime += dt;
        if (totalTime >= 0.2) {
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
    void draw(bool wireFrameMode = false) {
        apply();
        if (wireFrameMode)   psos->bind(core, "WaterWire");
        else                psos->bind(core, "Water");

        ps->updateTexturePS(core, "tex0", tm->find(textureFilenames[0]));
		ps->updateTexturePS(core, "tex1", tm->find(textureFilenames[1]));

        grid.mesh.draw(core);
    }
    Vec3 getNormalAtPosition(float x, float z) const {
		int col = (int)((x + 0.8f * (waves.cols - 1) / 2) / 0.8f);
		int row = (int)((-z + 0.8f * (waves.rows - 1) / 2) / 0.8f);
        return waves.normals[row * waves.cols + col];
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
    Core* core;
    ShaderManager* sm;
    std::vector<Mesh> meshes;
    Animation animation;
    PSOManager* psos;
    //std::vector<std::string> textureFilenames;
    std::vector<std::vector<std::string>> textureProperties;//store multiple texture properties for each mesh
    TextureManager* tm;
    Shader* vs;
    Shader* ps;

    void loadMeshes(Core* core, std::string filename) {
        GEMLoader::GEMModelLoader loader;
        std::vector<GEMLoader::GEMMesh> gemmeshes;
        GEMLoader::GEMAnimation gemanimation;
        loader.load(filename, gemmeshes, gemanimation);
		textureProperties.resize(gemmeshes.size());//different mesh may have different number of texture properties
        for (int i = 0; i < gemmeshes.size(); i++) {
            Mesh mesh;
            std::vector<ANIMATED_VERTEX> vertices;
            for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
                ANIMATED_VERTEX v;
                memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
                v.color = Colour(0.0f, 1.0f, 0.0f, 1.0f);//green
                vertices.push_back(v);
            }

            for (int n = 0; n < gemmeshes[i].material.properties.size(); n++) {
                textureProperties[i].push_back(gemmeshes[i].material.properties[n].getValue());
                tm->add(core, textureProperties[i][n]);
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
    void init(Core* _core, ShaderManager* _sm, PSOManager* _psos, TextureManager* _tm, std::string filename) {
        sm = _sm;
        psos = _psos;
        core = _core;
        tm = _tm;
        loadMeshes(core, filename);
        vs = &sm->shaders["AnimatedVertexShader"];
        ps = &sm->shaders["AnimationPS"];

        psos->createPSO(core, "AnimatedModel", vs->shader, ps->shader, VertexLayoutCache::getAnimatedLayout());
        psos->createPSO(core, "AnimatedModelWire", vs->shader, ps->shader, VertexLayoutCache::getAnimatedLayout(), true);
    }
    //change variable by searching constantbuffer's name and variable's name
    void update(Matrix world, Matrix view, Matrix proj, AnimationInstance* instance) {
        sm->updateConstant(vs->name, "animatedMeshBuffer", "W", &world);
        sm->updateConstant(vs->name, "animatedMeshBuffer", "View", &view);
        sm->updateConstant(vs->name, "animatedMeshBuffer", "Proj", &proj);
        sm->updateConstant(vs->name, "animatedMeshBuffer", "bones", instance->matrices);
    }
    void apply() {
        for (int i = 0; i < vs->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbvs") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, vs->constantBuffers[i].getGPUAddress());
            vs->constantBuffers[i].next();
        }
        for (int i = 0; i < ps->constantBuffers.size(); i++) {
            int parameterIndex = core->getRootParameterIndex("cbps") + i;
            core->getCommandList()->SetGraphicsRootConstantBufferView(parameterIndex, ps->constantBuffers[i].getGPUAddress());
            ps->constantBuffers[i].next();
        }
    }
    void draw(bool wireFrameMode = false) {
        apply();
        if(wireFrameMode)   psos->bind(core, "AnimatedModelWire");
        else                psos->bind(core, "AnimatedModel");
        for (int i = 0; i < meshes.size(); i++) {
            for (int n = 0; n < textureProperties[i].size(); n++) {
                ps->updateTexturePS(core, "tex" + std::to_string(n), tm->find(textureProperties[i][n]));
            }
            meshes[i].draw(core);
        }
    }
};