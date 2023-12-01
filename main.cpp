//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "sphere.h"
#include "curve.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
//void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);
//void Table(Cube &cube,Shader& lightingShader, glm::mat4 alTogether);
void Floor(Cube& cube,Shader& lightingShader, glm::mat4 alTogether);
void Platform(Cube& cube,Cube& cube13, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void SerialDivider(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Door(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
//void Wheel_hollow(Cube& cube,Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Wheel_hollow(Cube& cube_cyl, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);

void Bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Waiting_room(Cube& cube55, Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Chair(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void tvorpc(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether, bool ispc = false);
void Rood(Cube& cube1, Cube& cube2, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Waiting_room_base(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Platform_base(Cube& cube1,Cube& cube2,Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Lamp_post(Cube& cube, Sphere& sphere, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Divider(Cube& cube, Sphere& sphere, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Tree(Cube& cube, Curve& treeshape, Curve& treebelow, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);
void Readside(Cube& cube, Curve& treeshape, Curve& treebelow, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether);



//all VAO VBO EBO

unsigned int CVBO, CVAO,CEBO;
unsigned int cVBO, cVAO,cEBO;

//draw cylinder 

void drawCylinder(Shader& shader,glm::mat4 alTogether,float r=1.0f, float g=1.0f,float b=1.0f )
{
    int cylinder_size=146;
    shader.setMat4("model", alTogether);
    shader.setVec3("color", glm::vec3(r, g, b));
    glBindVertexArray(CVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, cylinder_size);

}

vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    const float EPSILON = 0.000001f;

    vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
/*
void drawCylinder(Shader& lightingShader, glm::mat4 model, float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", 60.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(CVAO);
    glDrawElements(GL_TRIANGLES, 75, GL_UNSIGNED_INT, 0);

}
*/
void drawCylinderwithlighting(Shader& lightingShader, glm::mat4 model, float r, float g, float b)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("material.shininess", 60.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cVAO);
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}

//new add
glm::mat4 transforamtion(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

unsigned int ch_wood_tex;
unsigned int almari_tex;
unsigned int cushion_tex;
unsigned int black_tex;
unsigned int floor_tiles;
unsigned int glassdoor_tex;
unsigned int bus_window_tex;
unsigned int sofa_tex;
unsigned int chrismastree_tex;
unsigned int treewood_tex;
unsigned int  grass_tex;
unsigned int  footpath_tex;
unsigned int  divider_tex;
unsigned int  divider2_tex;
unsigned int roof1left_tex;
unsigned int roof1right_tex;
unsigned int roof2_tex;
unsigned int wall1_tex;
unsigned int roof_green_tex;
unsigned int tv_tex;
unsigned int seat_blue_tex;
unsigned int seat_sky_tex;
unsigned int bus_tex;
unsigned int busfront_tex;
unsigned int busback_tex;
unsigned int road1_tex;
unsigned int road2_tex;
unsigned int tire_tex;


Camera camera(glm::vec3(-3.0f, 2.0f, -2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);



//position for hollow_cyllinder_vertices

vector<float>wheel_vertices = {
    -0.7300, 2.3200, 5.1000,
    -0.7400, 2.1250, 5.1000,
    -0.7350, 1.9250, 5.1000,
    -0.7250, 1.6700, 5.1000,
    -0.7250, 1.4700, 5.1000,
    -0.7250, 1.2600, 5.1000,
    -0.7200, 1.0350, 5.1000
};

vector<float>hollow_cyllinder_vertices =
{
    -0.4900, 1.9100, 5.1000,
    -1.5750, 1.7800, 5.1000,
};
vector<float>tree_shape =
{
    -0.0650, 2.5050, 5.1000,
        -0.0650, 2.5050, 5.1000,
        -0.1400, 2.2600, 5.1000,
        -0.2650, 2.0650, 5.1000,
        -0.2650, 2.0650, 5.1000,
        -0.4400, 1.8450, 5.1000,
        -0.6200, 1.6450, 5.1000,
        -0.6200, 1.6450, 5.1000,
        -0.9800, 1.4450, 5.1000,
};
vector<float>tree_below = {
    -0.5550, -0.3800, 5.1000,
        -0.4400, -0.3300, 5.1000,
        -0.2950, -0.2750, 5.1000,
        -0.2200, -0.2150, 5.1000,
        -0.1550, -0.0300, 5.1000,
        -0.1250, 0.1850, 5.1000,
        -0.1200, 0.3750, 5.1000,
        -0.1200, 0.5900, 5.1000,
        -0.1200, 0.7500, 5.1000,
};

// lights
vector< glm::vec3 > spotlightposition;


// positions of the point lights
glm::vec3 LightPositions[] = {
    glm::vec3(2.0 ,20.0 ,2.0),      //dir
    glm::vec3(-13.5+4.75,0.5 + 4.3,11.75 + 1.1),     //spot
    glm::vec3(14.0+2.0,3.5-.5,-10.0+5.0),      //point 1
    glm::vec3(14.0 + 5.0,3.5 - .5,-10.0 + 8.0),        //point 2
    glm::vec3(-13.5+4.75,0.5 + 4.3,11.75 - 1.1),         //spot 2
};
DirLight dirlight(
    LightPositions[0].x, LightPositions[0].y, LightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 0.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight1(

    LightPositions[1].x, LightPositions[1].y, LightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight1(

    LightPositions[2].x, LightPositions[2].y, LightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    LightPositions[3].x, LightPositions[3].y, LightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
// add 

SpotLight spotlight2(

    LightPositions[4].x, LightPositions[4].y, LightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    //0.0f, 1.0f, 0.0f,        // emission
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);



// light settings
bool pointLightOn1 = true;
bool pointLightOn2 = true;
bool dirLightOn = true;
bool spotLightOn1 = true;
bool spotLightOn2 = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool emissionToggle = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

//varaiable for dynamic obj

float counterdoorx=0.0f;
float busdoorx = 0.0f;
float bus_x = 0.0f;
float bus_z = 0.0f;
float  busrun_x1 = 20.0f;
float busrun_x2 = -20.0f;

//flag for dynamic obj

bool counterdoor = true;
bool busdoor = false;
bool busmove = false;
bool busrun = false;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";
    string f_tiles = "street.jpg";
    string glassdoor = "glasstex.jpg";
    string bus_window = "bus window.png";
    //string sofa = "sofa.jpg";
    string chrismastree = "chrismastree.jpg";
    string tree_wood = "tree.jpg";
    string grass = "grass.jpg";
    string footpath = "footpath.jpg";
    string divider = "divider.jpg";
    string divider2 = "divider2.jpg";
    string roof1left = "roof1 left.jpg";
    string roof1right = "roof1 right.jpg";
    string roof2= "roof2.jpg";
    string wall1 = "wall1.jpeg";
    string roof_green = "roof_green.jpg";
    string tv = "tv.jpg";
    string seat_sky = "seat sky.jpeg";
    string seat_blue = "seat blue2.jpg";
    string bus = "bus.jpg";
    string bus_front = "bus front.jpg";
    string bus_back = "bus back.jpg";
    string road1 = "road.jpg";
    string road2 = "road2.jpg";
    string tire = "tire.jpg";



    /**
    string wood = "wood.png";
    string woods = "woods.jpg";
    string sofa = "sofa.jpg";
    string marbel = "marbel.jpg";
    string cushion = "cushion.jpg";
    string plywood = "plywood.jpg";
    string ch_wood = "ch_wood.jpg";
    string chair = "chair.jpg";
    string wall = "wall.jpg";
    string walls = "walls.jpg";
    string black = "black.jpg";
    string news = "news.jpg";
    string city = "city.jpg";
    string almari = "almari.jpg";
    string door = "door.jpg";
    */

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

     floor_tiles = loadTexture(f_tiles.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     glassdoor_tex = loadTexture(glassdoor.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     bus_window_tex = loadTexture(bus_window.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     chrismastree_tex = loadTexture(chrismastree.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     treewood_tex= loadTexture(tree_wood.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     grass_tex = loadTexture(grass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     footpath_tex = loadTexture(footpath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     divider_tex = loadTexture(divider.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     divider2_tex = loadTexture(divider2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     roof1left_tex = loadTexture(roof1left.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     roof1right_tex = loadTexture(roof1right.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     roof2_tex = loadTexture(roof2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     wall1_tex = loadTexture(wall1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     roof_green_tex= loadTexture(roof_green.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     tv_tex = loadTexture(tv.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     seat_blue_tex = loadTexture(seat_blue.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     seat_sky_tex = loadTexture(seat_sky.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     bus_tex = loadTexture(bus.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     busfront_tex = loadTexture(bus_front.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     busback_tex = loadTexture(bus_back.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     road1_tex = loadTexture(road1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     road2_tex = loadTexture(road2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
     tire_tex = loadTexture(tire.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);




    // sofa_tex = loadTexture(sofa.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);




    
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube cube5 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);

    Cube cube51 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 5.0f, 1.0f);
    Cube cube15 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 5.0f);
    Cube cube101 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 10.0f, 1.0f);
    Cube cube110 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 10.0f);

    Cube cube52 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 5.0f, 2.0f);
    Cube cube25 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 5.0f);
    Cube cube23 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 3.0f);
    Cube cube22 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 2.0f, 2.0f);
    Cube cube32 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 3.0f, 2.0f);

    Cube cube13 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 3.0f);
    Cube cube31 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    Cube cube1010 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    Cube cube55 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);





    Cube tiles_cube = Cube(floor_tiles, floor_tiles, 32.0, 0.0f, 0.0f, 10.0f, 10.0f);
    
    //Sphere sphere = Sphere();

   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);








     
    //********************************************** add cylinder ********************************************
    struct Point {
        float x;
        float y;
        float z;
    };
    std::vector<Point> cylinder;
    //cylinder.push_back({ 0, 0, -8 });
    float radius1 = 0.5642;
    float radius = 0.5642;
    float x, y, x1, y1;
    for (float theta = 0; theta <= 360; theta +=5 ) {
         x = radius * glm::cos(glm::radians(theta));
         y = radius * glm::sin(glm::radians(theta));

         x1 = radius1 * glm::cos(glm::radians(theta));
         y1 = radius1 * glm::sin(glm::radians(theta));

        cylinder.push_back({ x, 0.0f,y });
        cylinder.push_back({ x1, 1.0f,y1 }); // normal cylinder
        //cylinder.push_back({ x * .5f, y * .5f, -8.0f }); // cylinder with different circle radius
    }
    //cylinder.push_back({ x, 0.0f,y });
   // cout << cylinder.size() << endl;

  /*  unsigned int cylinder_indices[] = {
        0,1,2,
3,4,5,
6,7,8,
9,10,11,
12,13,14,
15,16,17,
18,19,20,
21,22,23,
24,25,26,
27,28,29,
30,31,32,
33,34,35,
36,37,38,
39,40,41,
42,43,44,
45,46,47,
48,49,50,
51,52,53,
54,55,56,
57,58,59,
60,61,62,
63,64,65,
66,67,68,
69,70,71,
72,73,74
    };*/

    
    //std::vector<float> cylinderwithnormal;

    //for (int i = 0; i < cylinder.size(); i=i+3)
    //{
    //   
    //    Point p1 = cylinder[i];
    //    Point p2 = cylinder[i+1];
    //    Point p3 = cylinder[i+2];
    //    
    //    
    //    glm::vec3 v1 = { p1.x, p1.y, p1.z };
    //    glm::vec3 v2 = { p2.x, p2.y, p2.z };
    //    glm::vec3 v3 = { p3.x, p3.y, p3.z };
    //    vector<float> N = computeFaceNormal(v1.x,v1.y,v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
    //    /*
    //    glm::vec3 x = (v1 - v2);
    //    glm::vec3 y = (v3 - v2);

    //    glm::vec3 N = cross(x, y);

    //    float length = sqrtf(N.x * N.x + N.y * N.y + N.z * N.z);
    //    if (length > 0.000001f)
    //    {
    //        // normalize
    //        float lengthInv = 1.0f / length;
    //        N[0] = N.x * lengthInv;
    //        N[1] = N.y * lengthInv;
    //        N[2] = N.z * lengthInv;
    //    }
    //    */
    //    cylinderwithnormal.push_back(v1.x);
    //    cylinderwithnormal.push_back(v1.y);
    //    cylinderwithnormal.push_back(v1.z);

    //    cylinderwithnormal.push_back(N[0]);
    //    cylinderwithnormal.push_back(N[1]);
    //    cylinderwithnormal.push_back(N[2]);


    //    cylinderwithnormal.push_back(v2.x);
    //    cylinderwithnormal.push_back(v2.y);
    //    cylinderwithnormal.push_back(v2.z);

    //    cylinderwithnormal.push_back(N[0]);
    //    cylinderwithnormal.push_back(N[1]);
    //    cylinderwithnormal.push_back(N[2]);


    //    cylinderwithnormal.push_back(v3.x);
    //    cylinderwithnormal.push_back(v3.y);
    //    cylinderwithnormal.push_back(v3.z);

    //    cylinderwithnormal.push_back(N[0]);
    //    cylinderwithnormal.push_back(N[1]);
    //    cylinderwithnormal.push_back(N[2]);

    //}
    
    //unsigned int CVBO, CVAO;
    glGenVertexArrays(1, &CVAO);
    glGenBuffers(1, &CVBO);

    glBindVertexArray(CVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, CVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * cylinder.size(), cylinder.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Point), (void*)0);
    glEnableVertexAttribArray(0);

   // //new add
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   //glEnableVertexAttribArray(0);

   // // vertex normal attribute
   //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
   // glEnableVertexAttribArray(1);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CEBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinder_indices), cylinder_indices, GL_STATIC_DRAW);


    //new cylinder 
    float ver_arr[] = {

    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
    0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
    -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
    -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
    -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
    -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
    -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
    0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
    0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,

    1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
    0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
    -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
    -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
    -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
    -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
    -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
    0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
    0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,


    1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
    0.809017f, -1.0f, 0.587785f, 0.809017f, -1.0f, 0.587785f,
    0.309017f, -1.0f, 0.951057f, 0.309017f, -1.0f, 0.951057f,
    -0.309017f, -1.0f, 0.951057f, -0.309017f, -1.0f, 0.951057f,
    -0.809017f, -1.0f, 0.587785f, -0.809017f, -1.0f, 0.587785f,
    -1.0f, -1.0f, 1.22465e-16f, -1.0f, -1.0f, 1.22465e-16f,
    -0.809017f, -1.0f, -0.587785f, -0.809017f, -1.0f, -0.587785f,
    -0.309017f, -1.0f, -0.951057f, -0.309017f, -1.0f, -0.951057f,
    0.309017f, -1.0f, -0.951057f, 0.309017f, -1.0f, -0.951057f,
    0.809017f, -1.0f, -0.587785f, 0.809017f, -1.0f, -0.587785f,

    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.809017f, 1.0f, 0.587785f, 0.809017f, 1.0f, 0.587785f,
    0.309017f, 1.0f, 0.951057f, 0.309017f, 1.0f, 0.951057f,
    -0.309017f, 1.0f, 0.951057f, -0.309017f, 1.0f, 0.951057f,
    -0.809017f, 1.0f, 0.587785f, -0.809017f, 1.0f, 0.587785f,
    -1.0f, 1.0f, 1.22465e-16f, -1.0f, 1.0f, 1.22465e-16f,
    -0.809017f, 1.0f, -0.587785f, -0.809017f, 1.0f, -0.587785f,
    -0.309017f, 1.0f, -0.951057f, -0.309017f, 1.0f, -0.951057f,
    0.309017f, 1.0f, -0.951057f, 0.309017f, 1.0f, -0.951057f,
    0.809017f, 1.0f, -0.587785f, 0.809017f, 1.0f, -0.587785f,


    0.0,-1.0,0.0, 0.0,-1.0,0.0,
    0.0,1.0,0.0, 0.0,1.0,0.0

    };

    unsigned int ind_arr[] = {
        0, 11, 1,
        11, 0, 10,
        1, 12, 2,
        12, 1, 11,
        2, 13, 3,
        13, 2, 12,
        3, 14, 4,
        14, 3, 13,
        4, 15, 5,
        15, 4, 14,
        5, 16, 6,
        16, 5, 15,
        6, 17, 7,
        17, 6, 16,
        7, 18, 8,
        18, 7, 17,
        8, 19, 9,
        19, 8, 18,
        9, 10, 0,
        10, 9, 19,

        40,20,21,
        40,21,22,
        40,22,23,
        40,23,24,
        40,24,25,
        40,25,26,
        40,26,27,
        40,27,28,
        40,28,29,
        40,29,20,

        41,30,31,
        41,31,32,
        41,32,33,
        41,33,34,
        41,34,35,
        41,35,36,
        41,36,37,
        41,37,38,
        41,38,39,
        41,39,30


        /*
        21,10,11,
        21,11,12,
        21,12,13,
        21,13,14,
        21,14,15,
        21,15,16,
        21,16,17,
        21,17,18,
        21,18,19,
        21,19,10*/

    };

    glGenVertexArrays(1, &cVAO);
    glGenBuffers(1, &cVBO);
    glGenBuffers(1, &cEBO);

    glBindVertexArray(cVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ver_arr), ver_arr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind_arr), ind_arr, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //end of cylingder

//************************************************* END Cylinder ******************************


    //*******************************************************light**************************************************

    
    for (int i = 0; i < 20; i++)
    {
        spotlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, 11.75f + 1.1f));
        spotlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, 11.75f - 1.1f));
        spotlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, -15.75f + 1.1f));
        spotlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, -15.75f - 1.1f));

    }
    vector<SpotLight>setspotlight(20);
    for (int i = 0; i < 20; i++)
        setspotlight[i]= SpotLight(spotlightposition[i].x, spotlightposition[i].y, spotlightposition[i].z,  // position
            0.05f, 0.05f, 0.05f,     // ambient
            0.8f, 0.8f, 0.8f,     // diffuse
            1.0f, 1.0f, 1.0f,        // specular
            //0.0f, 1.0f, 0.0f,        // emission
            1.0f,   //k_c
            0.09f,  //k_l
            0.032f, //k_q
            i+1       // light number
        );
        
    
       

    

    //*****************************************************light end ********************************************************
    //add new for shere
    // u can chnage radious colour property here 
    Sphere sphere = Sphere();


    //Curve wheel(wheel_vertices);
   //Curve wheel_hollow(hollow_cyllinder_vertices);

    
   // Curve wheel(wheel_vertices, bus_window_tex, bus_window_tex, 1.0f);
   // Curve wheel_hollow(hollow_cyllinder_vertices, bus_window_tex, bus_window_tex, 1.0f);

    Curve wheel(wheel_vertices, tire_tex, tire_tex, 1.0f);
   Curve wheel_hollow(hollow_cyllinder_vertices, tire_tex, tire_tex, 1.0f);
   
   Curve treeshape(tree_shape, chrismastree_tex, chrismastree_tex, 1.0f);

   Curve treebelow(tree_below, treewood_tex, treewood_tex, 1.0f);


     // Curve wheel(wheel_vertices, sofa_tex, sofa_tex, 1.0f);
    //Curve wheel_hollow(hollow_cyllinder_vertices, sofa_tex, sofa_tex, 1.0f);
    // render loop
    // -----------
   while (!glfwWindowShouldClose(window))
   {
       // per-frame time logic
       // --------------------
       float currentFrame = static_cast<float>(glfwGetTime());
       deltaTime = currentFrame - lastFrame;
       lastFrame = currentFrame;

       // input
       // -----
       processInput(window);

       // render
       // ------
       glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);







       ///control for door 

       int z = camera.Position.z;
       if (z > 2.0)
           counterdoor = false;
       else
           counterdoor = true;


       //************************************************ lightingShaderWithTexture ******************************************


       // be sure to activate shader when setting uniforms/drawing objects
       lightingShaderWithTexture.use();
       lightingShaderWithTexture.setVec3("viewPos", camera.Position);

       // pass projection matrix to shader (note that in this case it could change every frame)
       glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
       lightingShaderWithTexture.setMat4("projection", projection);

       // camera/view transformation
       glm::mat4 view = camera.GetViewMatrix();
       //glm::mat4 view = basic_camera.createViewMatrix();
       lightingShaderWithTexture.setMat4("view", view);

       pointlight1.setUpPointLight(lightingShaderWithTexture);
       pointlight2.setUpPointLight(lightingShaderWithTexture);
       dirlight.setUpPointLight(lightingShaderWithTexture);
       //spotlight1.setUpPointLight(lightingShaderWithTexture);
       //spotlight2.setUpPointLight(lightingShaderWithTexture);


       //add new point light
       for (int i = 0; i < 20; i++)
          setspotlight[i].setUpPointLight(lightingShaderWithTexture);

       // Modelling Transformation
       glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
       glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
       translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
       rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
       rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
       rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
       model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;

       cube.setTextureProperty(diffMap, specMap, 32.0f);
       // cube.drawCubeWithTexture(lightingShaderWithTexture, model);
        //cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32);

       // translateMatrix = glm::translate(identityMatrix, glm::vec3(-25.2967, -0.9099926, 0.6557));

       Floor(tiles_cube, lightingShaderWithTexture, identityMatrix);






       //************************************************* light Shader **********************************************

       lightingShader.use();
       lightingShader.setMat4("projection", projection);
       lightingShader.setMat4("view", view);

       pointlight1.setUpPointLight(lightingShader);
       pointlight2.setUpPointLight(lightingShader);
       dirlight.setUpPointLight(lightingShader);
      // spotlight1.setUpPointLight(lightingShader);
      // spotlight2.setUpPointLight(lightingShader);

       //add new light
       for (int i = 0; i < 20; i++)
          setspotlight[i].setUpPointLight(lightingShader);

       model = glm::mat4(1.0f);
       model = transforamtion(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0f, 1.0f, 1.0f);
       Platform_base(cube1010, cube55, lightingShader, lightingShaderWithTexture, model);


       model = transforamtion(0.0, 0.3, 0.0, 0.0, 0.0, 0.0, 1.0f, 1.0f, 1.0f);
       Platform(cube, cube13, lightingShader, lightingShaderWithTexture, model);



       //hollow wheel
       model = transforamtion(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

       Divider(cube101, sphere, lightingShader, lightingShaderWithTexture, model);
       //Lamp_post(cube, sphere, lightingShader, lightingShaderWithTexture, model);



       //Wheel_hollow(wheel, wheel_hollow, cube,lightingShader, lightingShaderWithTexture, model);

        //draw bus
       model = transforamtion(0.0, 0.6, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       model = transforamtion(0.0, 0.6, 3.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       model = transforamtion(0.0, 0.6, -1.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       model = transforamtion(0.0, 0.6, -4.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       model = transforamtion(0.0, 0.6, -7.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       //1st platfrom
       model = transforamtion(-15.0, 0.6, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
       model = transforamtion(-15.0, 0.6, 3.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);



       if (busmove == false)
       {
           model = transforamtion(0.0, 0.2, 8.0f + 0.1f + 1.8f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
           Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
       }
       else
       {
           bus_x = camera.Position.x - 5.0f;
           bus_z = camera.Position.z+1.0f;
           model = transforamtion(camera.Position.x, camera.Position.y, camera.Position.z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
           //cube.drawCubeMe(lightingShader, model, 1.0f, 1.0f, 0.0f);
           model = transforamtion(bus_x, 0.2, bus_z, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
           Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);

       }

       //bus run

       if (busrun == false)
       {
        model = transforamtion(-10.0, 0.3, -19.5f+1.8+0.1f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
       model = transforamtion(10.0, 0.3, -19.5+4.5f+1.8+0.1f-0.9f, 0.0, 180, 0.0, 1.0, 1.0, 1.0);
       Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
        
       }
       else
       {
           busrun_x1 = busrun_x1 - 0.3f;
               if (busrun_x1 <= -20.f)
                   busrun_x1 = 20.f;

               busrun_x2 = busrun_x2 + 0.3f;
               if (busrun_x2 >= 20.0f)
                   busrun_x2 = -20.0f;

           model = transforamtion(busrun_x1, 0.3, -19.5f + 1.8 + 0.1f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
           Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
           model = transforamtion(busrun_x2, 0.3, -19.5 + 4.5f + 1.8 + 0.1f - 0.9f, 0.0, 180, 0.0, 1.0, 1.0, 1.0);
           Bus(cube, wheel, wheel_hollow, lightingShader, lightingShaderWithTexture, model);
       }


        model = transforamtion(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        Waiting_room_base(cube1010, lightingShader, lightingShaderWithTexture, model);


        model = transforamtion(2.0, 0.3, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
       Waiting_room(cube55,cube, lightingShader, lightingShaderWithTexture, model);

        model = transforamtion(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        //tvorpc(cube, lightingShader, lightingShaderWithTexture, model,true);
       // Chair(cube,lightingShader, lightingShaderWithTexture, model);
        Rood(cube101,cube110, lightingShader, lightingShaderWithTexture, model);

        model = transforamtion(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
        //Tree(cube, treeshape, treebelow,lightingShader, lightingShaderWithTexture, model);
        Readside(cube101, treeshape, treebelow, lightingShader, lightingShaderWithTexture, model);






        //add a light
       // glm::mat4 model = glm::mat4(1.0f);
        //model = transforamtion(3, 1.0, 13,rotateAngle_X,rotateAngle_Y,rotateAngle_Z,1.0,1.0,1.0);
        //cube.drawCubeMe(lightingShader, model, 1.0f, 1.0f, 0.0f);


        

        

        //sphere draw here
        model = transforamtion(3, 1.0, 13,rotateAngle_X,rotateAngle_Y,rotateAngle_Z,1.0,1.0,1.0);
       // sphere.drawSphere(lightingShader, model);


        //add new cylinder 
        // 0.605, .55, 0.605 ---> 1.0 , 1.0 ,1.0
       // model = transforamtion(1, 1.0+1.0*.55, 10, rotateAngle_X, rotateAngle_Y, rotateAxis_Z, 0.605, .55, 0.605);
        //drawCylinderwithlighting (lightingShader, model, 1.0, 0.0, 0.0);
        
        //model = transforamtion(3, 1.0, 13, rotateAngle_X, rotateAngle_Y, rotateAxis_Z, 1.0, 1.0, 1.0);
       // drawCylinder(lightingShader, model, 1.0, 1.0, 0.0);
        //*********************************************** OlD Shader  ***************************************************************
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //does not use 
        glm::vec3 LightColor[] = {
        glm::vec3(0.8f, 0.8f, 0.8f),// White - Dir
        glm::vec3(1.0f,  0.0f,  0.0f),//Red - Spot Light
        glm::vec3(0.0f,  1.0f,  0.0f),//Green - Point Light 1
        glm::vec3(0.0f,  0.0f,  1.0f),//Blue - Point Light 2
        glm::vec3(1.0f,  0.0f,  1.0f)
        };
        //*************************************************** light draw here ********************************************************
        for (unsigned int i = 0; i < 5; i++)
        {
            model = glm::mat4(1.0f);
            //LightPositions[i].y = translate_Y;
            model = glm::translate(model, LightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
            //cube.drawCube(ourShader, model,LightColor[i].x, LightColor[i].y, LightColor[i ].z);
        }
        /*
        model = transforamtion(-13.5,0.5+4.3,11.75+1.1,0.0,0.0,0.0,0.5,0.5,0.5);
        cube.drawCube(ourShader, model);
        model = transforamtion(-13.5, 0.5 + 4.3, 11.75 - 1.1, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5);
        cube.drawCube(ourShader, model);

        model = transforamtion(-13.5+4.75, 0.5 + 4.3, 11.75 + 1.1, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5);
        cube.drawCube(ourShader, model);
        model = transforamtion(-13.5+4.75, 0.5 + 4.3, 11.75 - 1.1, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5);
        cube.drawCube(ourShader, model);
        */
        vector< glm::vec3 > Pointlightposition;
        for (int i = 0; i < 5; i++)
        {
            Pointlightposition.push_back(glm::vec3(-13.5+i*4.75f,4.8f,11.75f+1.1f));
            Pointlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, 11.75f - 1.1f));
            Pointlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, -15.75f + 1.1f));
            Pointlightposition.push_back(glm::vec3(-13.5 + i * 4.75f, 4.8f, -15.75f - 1.1f));

        }
        for (int i = 0; i < Pointlightposition.size(); i++)
        {
            model = transforamtion(Pointlightposition[i].x, Pointlightposition[i].y, Pointlightposition[i].z, 0.0, 0.0, 0.0, 0.5, 0.5, 0.5);
           // cube.drawCube(ourShader, model);
        }
        //cylinder
       // model = transforamtion(2, 1.0, 12, rotateAngle_X, rotateAngle_Y, rotateAxis_Z, 1.0, 1.0, 1.0);
        
      // drawCylinder(ourShader,model, 1.0, 1.0, 0.5);
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    std::cout << translate_X << ", " << translate_Y << ", " <<translate_Z << endl;
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    /*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }*/
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
   
    
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;


    


}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        if (counterdoor)
        {
            counterdoor = false;
            // counterdoorx = 2.0f;
            std::cout << counterdoorx << "when counterdoor is false" << endl;
        }
        else
        {
            counterdoor = true;
            //counterdoorx = 0.0f;
            std::cout << counterdoorx << "when counterdoor is true" << endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (busdoor)
        {
            busdoor = false;
            busdoorx =0.6f;
            std::cout << busdoorx << "when busdoor is false" << endl;
        }
        else
        {
            busdoor = true;
            counterdoorx = 0.0f;
            std::cout << busdoorx << "when busdoor is true" << endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (busmove)
        {
            busmove = false;
            // counterdoorx = 2.0f;
            std::cout <<"bus in stop state" << endl;
        }
        else
        {
            busmove = true;
            //counterdoorx = 0.0f;
            std::cout << "bus in moving state" << endl;
        }
    }


    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if (busrun)
        {
            busrun = false;
            // counterdoorx = 2.0f;
            std::cout << "bus run in stop state" << endl;
        }
        else
        {
            busrun = true;
            //counterdoorx = 0.0f;
            std::cout << "bus run in moving state" << endl;
        }
    }



    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (dirLightOn)
        {
            dirlight.turnOff();
            dirLightOn = !dirLightOn;
        }
        else
        {
            dirlight.turnOn();
            dirLightOn = !dirLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn1)
        {
            pointlight1.turnOff();
            pointLightOn1 = !pointLightOn1;
        }
        else
        {
            pointlight1.turnOn();
            pointLightOn1 = !pointLightOn1;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (pointLightOn2)
        {
            pointlight2.turnOff();
            pointLightOn2 = !pointLightOn2;
        }
        else
        {
            pointlight2.turnOn();
            pointLightOn2 = !pointLightOn2;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (spotLightOn1)
        {
            spotlight1.turnOff();
            spotLightOn1 = !spotLightOn1;
        }
        else
        {
            spotlight1.turnOn();
            spotLightOn1 = !spotLightOn1;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOff();
            if (pointLightOn2)
                pointlight2.turnAmbientOff();
            if (spotLightOn1)
                spotlight1.turnAmbientOff();
            if (spotLightOn2)
                spotlight2.turnAmbientOff();
            if (dirLightOn)
                dirlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnAmbientOn();
            if (pointLightOn2)
                pointlight2.turnAmbientOn();
            if (spotLightOn1)
                spotlight1.turnAmbientOn();
            if (spotLightOn2)
                spotlight2.turnAmbientOn();
            if (dirLightOn)
                dirlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOff();
            if (pointLightOn2)
                pointlight2.turnDiffuseOff();
            if (dirLightOn)
                dirlight.turnDiffuseOff();
            if (spotLightOn1)
                spotlight1.turnDiffuseOff();
            if (spotLightOn2)
                spotlight2.turnDiffuseOff();

            diffuseToggle = !diffuseToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnDiffuseOn();
            if (pointLightOn2)
                pointlight2.turnDiffuseOn();
            if (dirLightOn)
                dirlight.turnDiffuseOn();
            if (spotLightOn1)
                spotlight1.turnDiffuseOn();
            if (spotLightOn2)
                spotlight2.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOff();
            if (pointLightOn2)
                pointlight2.turnSpecularOff();
            if (dirLightOn)
                dirlight.turnSpecularOff();
            if (spotLightOn1)
                spotlight1.turnSpecularOff();
            if (spotLightOn2)
                spotlight2.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            if (pointLightOn1)
                pointlight1.turnSpecularOn();
            if (pointLightOn2)
                pointlight2.turnSpecularOn();
            if (dirLightOn)
                dirlight.turnSpecularOn();
            if (spotLightOn1)
                spotlight1.turnSpecularOn();
            if (spotLightOn2)
                spotlight2.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (spotLightOn2)
        {
            spotlight2.turnOff();
            spotLightOn2 = !spotLightOn2;
        }
        else
        {
            spotlight2.turnOn();
            spotLightOn2 = !spotLightOn2;
        }
    }




}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void Floor(Cube& cube,Shader& lightingShader,  glm::mat4 alTogether)
{
   
    
    glm::mat4 model = transforamtion(-26.5f, -0.1f,-24.0f, 0.0,0.0,0.0, 54.5f, 0.1f, 44.0f);
    cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    cube.drawCubeWithTexture(lightingShader, alTogether * model);
}
void Platform_base(Cube& cube1, Cube& cube2 ,Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    glm::mat4 model = transforamtion(-17.0f, 0.0f, -12.0f, 0.0f, 0.0f, 0.0f, 26.0f, 0.3f, 20.0f);
    cube1.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube1.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);



    //base 1
    model = transforamtion(-15.0 - 0.5f, 0.3, -6.0f, 0.0, 0.0, 0.0, 6.0, 0.01, 12.0);
    cube2.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
    cube2.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    //base 2
    model = transforamtion(-3.0, 0.3, -10.0f, 0.0, 0.0, 0.0, 10.0, 0.01, 16);
    cube2.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
    cube2.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.0);
}
void Platform(Cube& cube, Cube& cube13, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{

    //platfrom 1
    //p1
    glm::mat4 model = transforamtion(-15.0f, 4.0f, -6.0f, 0.0f, 0.0f, -10.0f, 2.5f, 0.1f, 12.0f);
    cube13.setTextureProperty(roof1left_tex, roof1left_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);


    //p2
    model = transforamtion(-15.0f+2.5f - 0.05f, 4.0f - (4.0 * tan((10.0f * 3.1416) / 180))+0.26, -6.0f, 0.0f, 0.0f, 10.0f+1.5f, 2.5f, 0.1f, 12.0f);
    cube13.setTextureProperty(roof1right_tex, roof1right_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    /*
    //base 1
    model = transforamtion(-15.0-0.5f, 0.0, -6.0f, 0.0, 0.0, 0.0, 6.0, 0.01, 12.0);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.0);
    */


     //piller

    int numofpiller = 5;

    for (int i = 1; i < numofpiller; i++)
    {
        model = transforamtion(-15.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + i * 2.5f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
        drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    }
    /*
    model = transforamtion(-15.0+2.5, 0.0+3.3*.55, -6.0f+ 1.0f*2.0f,    0.0, 0.0, 0.0, 0.605*.15,    3.3*0.55, 0.605*.15);
    drawCylinderwithlighting (lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-15.0 + 2.5, 0.0 + 3.3 * .55, -6.0f+ 2.0f * 2.0f,     0.0, 0.0, 0.0, 0.605 * .15,     3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-15.0 + 2.5, 0.0 + 3.3 * .55, -6.0f+ 3.0f * 2.0f,      0.0, 0.0, 0.0, 0.605 * .15,     3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-15.0 + 2.5, 0.0 + 3.3 * .55, -6.0f+4.0f * 2.0f,     0.0, 0.0, 0.0, 0.605 * .15,       3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-15.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 5.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);
    */

    //platfrom 2

    //p1
    model = transforamtion(-3.0f, 4.0f, -10.0f, 0.0f, 0.0f, -10.0f, 2.5f, 0.1f, 16.0f);
    cube13.setTextureProperty(roof1left_tex, roof1left_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);


    //p2
    model = transforamtion(-3.0f + 2.5f -.05f , 4.0f - (4.0 * tan((10.0f * 3.1416) / 180))+.26f, -10.0f, 0.0f, 0.0f, 10.0f+1.5f, 2.0f, 0.1f, 16.0f);
    cube13.setTextureProperty(roof1right_tex, roof1right_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    //p3


    //model = transforamtion(-3.0f+2.5f+2.0f -0.15f, 4.0f-0.05f, -10.0f, 0.0f, 0.0f, -10.0f, 2.0f, 0.1f, 16.0f);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0f + 2.5f + 2.0f - 0.15f, 4.0f - 0.05f, -10.0f, 0.0f, 0.0f, -10.0f, 2.0f, 0.1f, 2.0f);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    
        int numofsmallcube = 43;
        for (int i = 1; i <= numofsmallcube; i = i + 2)
        {
            model = transforamtion(-3.0f + 2.5f + 2.0f - 0.15f, 4.0f - 0.05f, -8.0f + i * 0.3, 0.0f, 0.0f, -10.0f, 2.0f, 0.1f, 0.3f);
            //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
            //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
            cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);
        }
        model = transforamtion(-3.0f + 2.5f + 2.0f - 0.15f, 4.0f - 0.05f, -8.0f + (numofsmallcube+2) * 0.3, 0.0f, 0.0f, -10.0f, 2.0f, 0.1f, 0.5f);
        //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
        //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);


    //p4
    model = transforamtion(-3.0f + 2.5f+2.0f+2.0f -.18f, 4.0f - (4.0 * tan((10.0f * 3.1416) / 180))+.30f , -10.0f, 0.0f, 0.0f, 10.0f + 1.5f, 3.6f, 0.1f, 16.0f);
    cube13.setTextureProperty(roof1right_tex, roof1right_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);


    /*
    //base 2
    model = transforamtion(-3.0, 0.0, -10.0f, 0.0, 0.0, 0.0, 10.0, 0.01,16);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, a jjh+lTogether * model);
   cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.0);
   */


    //piller 2

    numofpiller = 5;

   for (int i = -1; i < numofpiller; i++)
   {
       model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + i * 2.5f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
       drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

   }
   /*
    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + ( - 1.0f) * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 0.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 1.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 2.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 3.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 4.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 2.5, 0.0 + 3.3 * .55, -6.0f + 5.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.3 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);
    */

    //piller 3

   for (int i = -1; i < numofpiller; i++)
   {
       model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + i * 2.5f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
       drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

   }
   /*
    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + (-1.0f) * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 0.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 1.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 2.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 3.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 4.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-3.0 + 7.5f, 0.0 + 3.5 * .55, -6.0f + 5.0f * 2.0f, 0.0, 0.0, 0.0, 0.605 * .15, 3.5 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);
    */

    //buscounter

    //back
    model = transforamtion(-11.0, 0.0, -10.0f, 0.0, 0.0, 0.0, 9.0, 3.5, .1);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);

    //left
    model = transforamtion(-11.0, 0.0, -10.0f, 0.0, 0.0, 0.0, 0.1, 3.5, 7.0);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);

    //right
    model = transforamtion(-11.0+9.0-.1, 0.0, -10.0f, 0.0, 0.0, 0.0, 0.1, 3.5, 7.0);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;

    //up
    model = transforamtion(-11.0f, 3.5f, -10.0f, 0.0, 0.0, 0.0, 9.0f, 0.1f, 7.0f);
    cube13.setTextureProperty(roof1left_tex, roof1left_tex, 32.0f);
    cube13.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   // cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 1.0);


    //front
    model = transforamtion(- 11.0f, 3.5f-.6f, -10.0f+7.0-.1f, 0.0, 0.0, 0.0, 9.0f, 0.6, .1);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;
     

    //shade
    model = transforamtion(-11.0f, 3.5f - .6f, -10.0f + 7.0 - .1f, 0.0, 0.0, 0.0, 9.0f, .1,1.5);
    cube.setTextureProperty(roof_green_tex, roof_green_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.0,1.0,0.5);


    //piller

    model = transforamtion(-11.0f+2.0f , 0.0 + 2.7 * .55f,  -3.0f+.75 ,     0.0, 0.0, 0.0,      0.605 * .15, 2.7 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, alTogether* model, 1.0, 0.0, 0.0);

    model = transforamtion(-4.0f, 0.0 + 2.7 * .55f, -3.0f + .75, 0.0, 0.0, 0.0, 0.605 * .15, 2.7 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);


    //bus counter insize

    //down
    model = transforamtion(-11.0, 0.0, -10.0f+2.0f, 0.0, 0.0, 0.0, 9.0, 1.0, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 1.0, 1.0);

    //up
    model = transforamtion(-11.0, 3.0, -10.0f + 2.0f, 0.0, 0.0, 0.0, 9.0, 0.5, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 1.0, 1.0);


    //mid
    model = transforamtion(-11.0, 1.0-0.1, -10.0f + 2.0f-0.8, 0.0, 0.0, 0.0, 9.0, 0.1, .8);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 1.0, 1.0);

    //main divider
    model = transforamtion(-11.0+2.57-.2f, 0.0, -10.0f + 2.0f-0.01f, 0.0, 0.0, 0.0, 0.4, 3.5, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);

    //main divider
    model = transforamtion(-11.0 + 6.428 - .2f, 1.0, -10.0f + 2.0f-.01f, 0.0, 0.0, 0.0, 0.4, 2.0, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);


    //all divider
    model = transforamtion(-11.0 + 1.285 - 0.05, 1.0, -10.0f + 2.0f-0.01f, 0.0, 0.0, 0.0, 0.1, 2.0, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    model = transforamtion(-11.0 + 2.59 + 1.286*1-0.05, 1.0, -10.0f + 2.0f-0.01f, 0.0, 0.0, 0.0, 0.1, 2.0, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    model = transforamtion(-11.0 + 2.59 + 1.286*2 - 0.05, 1.0, -10.0f + 2.0f-0.01f, 0.0, 0.0, 0.0, 0.1, 2.0, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    model = transforamtion(-11.0 + 6.428 + 1.286 - 0.05, 0.0, -10.0f + 2.0f-0.01f, 0.0, 0.0, 0.0, 0.1, 3.5, .1);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);


    //Tv
    //tv1
    model = transforamtion(-11.0 + 1.285-0.75*0.8, 1.0+0.8, -10.0+0.15, 0.0, 0.0, 0.0, 0.8,0.8,0.8);
    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, false);

    //tv2
   // model = transforamtion(-11.0 + 2.59 + 1.286 * 1 - 0.75-0.15, 1.0 + 0.8, -10.0 + 0.15, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8);
    model = transforamtion(-11.0 + 2.59 + 1.286 * 1-0.3, 1.0 + 0.8, -10.0 + 0.15, 0.0, 0.0, 0.0, 1.2, 1.0, 1.2);

    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, false);

    //tv3
    model = transforamtion(-11.0 + 2.59 + 1.286 * 2 - 0.75+0.15, 1.0 + 0.8, -10.0 + 0.15, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8);
    //tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, false);


    //tv4
    model = transforamtion(-11.0 + 6.428 + 1.286 -0.75 * 0.8, 1.0 + 0.8, -10.0 + 0.15, 0.0, 0.0, 0.0, 0.8, 0.8, 0.8);
    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, false);


    //pc
   // model = transforamtion(-11.0 + 1.285 - 0.75 * 0.8, -10.0f + 2.0f - 0.8, 0.0, 0.0, 0.0, 9.0, 0.1, .8);

    model = transforamtion(-11.0 + 1.285 - 0.75 * 0.6, 1.0 , -10.0f + 2.0f - 0.4f, 0.0, 0.0, 0.0, 0.6, 0.6, 0.6);
    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, true);

    model = transforamtion(-11.0 + 2.59 + 1.286 * 1+.25, 1.0, -10.0f + 2.0f - 0.4f, 0.0, 0.0, 0.0, 0.6, 0.6, 0.6);
    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, true);


    model = transforamtion(-11.0 + 6.428 + 1.286 - 0.75 * 0.6, 1.0, -10.0f + 2.0f - 0.4f, 0.0, 0.0, 0.0, 0.6, 0.6, 0.6);
    tvorpc(cube, lightingShader, lightingShaderTexture, alTogether* model, true);





    // serial divider
    /**
    model = transforamtion(-11.0f + 1.285f+1.5f, 0.0 + 0.05 * .55f, -3.0f + 1.75, 0.0, 0.0, 0.0, 0.605 * .3, 0.05 * 0.55, 0.605 * .3);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-11.0f + 1.285f+1.5f, 0.0 + 0.1 * .55f, -3.0f + 1.75, 0.0, 0.0, 0.0, 0.605 * .2, 0.1 * 0.55, 0.605 * .2);
    drawCylinderwithlighting(lightingShader, model, 1.0, 1.0, 0.0);

    model = transforamtion(-11.0f + 1.285f+1.5f, 0.0 + 1.0f * .55f, -3.0f + 1.75, 0.0, 0.0, 0.0, 0.605 * .1, 1.0 * 0.55, 0.605 * .1);
    drawCylinderwithlighting(lightingShader, model, 1.0, 1.0, .5);

    model = transforamtion(-11.0f + 1.285f + 1.5f, 1.0, -3.0f + 1.75f, 0.0, 0.0, 0.0, 0.01, 0.1, 2.0);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, model, 1.0, 1.0, 1.0);

    model = transforamtion(-11.0f + 1.285f + 1.5f, 0.0 + 0.05 * .55f, -3.0f + 1.75 +2.0, 0.0, 0.0, 0.0, 0.605 * .3, 0.05 * 0.55, 0.605 * .3);
    drawCylinderwithlighting(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-11.0f + 1.285f + 1.5f, 0.0 + 0.1 * .55f, -3.0f + 1.75 +2.0, 0.0, 0.0, 0.0, 0.605 * .2, 0.1 * 0.55, 0.605 * .2);
    drawCylinderwithlighting(lightingShader, model, 1.0, 1.0, 0.0);

    model = transforamtion(-11.0f + 1.285f + 1.5f, 0.0 + 1.0f * .55f, -3.0f + 1.75 +2.0, 0.0, 0.0, 0.0, 0.605 * .1, 1.0 * 0.55, 0.605 * .1);
    drawCylinderwithlighting(lightingShader, model, 1.0, 1.0, .5);
*/
    model= model = transforamtion(-11.0f + 2.59f, 0.0f, -10.0f + 2.0f+0.5f, 0.0, 0.0, 0.0, 1.0,1.0,1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);

    model = model = transforamtion(-11.0f + 1.285f, 0.0f, -10.0f + 2.0f+0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);



    model = model = transforamtion(-11.0f + 6.428f, 0.0f, -10.0f + 2.0f + 0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);

    model = model = transforamtion(-11.0f + 2.59 + 1.286 * 1 , 0.0f, -10.0f + 2.0f + 0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);

    model = model = transforamtion(-11.0f + 2.59 + 1.286 * 2 , 0.0f, -10.0f + 2.0f + 0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);



    model = model = transforamtion(-11.0f + 6.428f, 0.0f, -10.0f + 2.0f + 0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);

    model = model = transforamtion(-11.0f + 6.428f+1.286, 0.0f, -10.0f + 2.0f + 0.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SerialDivider(cube, lightingShader, lightingShaderTexture, alTogether* model);


    //door
    //left
    model = transforamtion(-11.0f+0.01f, 0.0f, -10.0f + 7.0f -0.1f, 0.0, 0.0, 0.0, 2.5f, 2.9f, .1f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);


    //right
    model = transforamtion(-11.0f +6.5f - 0.01f, 0.0f, -10.0f + 7.0f - 0.1f, 0.0, 0.0, 0.0, 2.5f, 2.9f, .1f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);



    /*
    model = transforamtion(-11.0f + 2.5f - 0.01f, 0.0f, -10.0f + 7.0f - 0.1f, 0.0, 0.0, 0.0, 2.0f, 2.9f, .05f);
    cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 0.0);

    model = transforamtion(-11.0f + 4.5f - 0.01f, 0.0f, -10.0f + 7.0f - 0.1f, 0.0, 0.0, 0.0, 2.0f, 2.9f, .05f);
    cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   // cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 0.0);
     
     */
     if (counterdoor && counterdoorx < 2.f)
        counterdoorx = counterdoorx + 0.05f;


     if (counterdoor == false && counterdoorx >0.0f)
         counterdoorx = counterdoorx - 0.05f;
     //cout << counterdoorx <<"-->" << counterdoor << endl;
    model = transforamtion(-11.0f + 2.5f - 0.01f, 0.0f, -10.0f + 7.0f - 0.1f, 0.0, 0.0, 0.0, 1.0f, 1.0f, 1.0f);
    Door(cube, lightingShader, lightingShaderTexture, alTogether * model);
  
    


    //bus





  
}

void SerialDivider(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{

    glm::mat4 model = transforamtion(0.0f, 0.0 + 0.05 * .55f, 0.0f, 0.0, 0.0, 0.0,      0.605 * .2, 0.05 * 0.55, 0.605 * .2);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    model = transforamtion(0.0f, 0.0 + 0.1 * .55f, 0.0f, 0.0, 0.0, 0.0,                 0.605 * .15, 0.1 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    model = transforamtion(0.0f, 0.0 + 1.0f * .55f, 0.0f, 0.0, 0.0, 0.0,                0.605 * .07, 1.0 * 0.55, 0.605 * .07);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, .5);

    model = transforamtion(0.0f, 1.0, 0.0f, 0.0, 0.0, 0.0, 0.01, 0.1, 2.0);
    //cube.setTextureProperty(floor_tiles, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(Shader, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);

    model = transforamtion(0.0f, 0.0 + 0.05 * .55f, 0.0f + 2.0, 0.0, 0.0, 0.0,               0.605 * .2, 0.05 * 0.55, 0.605 * .2);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    model = transforamtion(0.0f, 0.0 + 0.1 * .55f, 0.0f + 2.0, 0.0, 0.0, 0.0,            0.605 * .15, 0.1 * 0.55, 0.605 * .15);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    model = transforamtion(0.0f, 0.0 + 1.0f * .55f, 0.0f + 2.0, 0.0, 0.0, 0.0,            0.605 * .07, 1.0 * 0.55, 0.605 * .07);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, .5);
}

void Door(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    
   
        glm::mat4 model = transforamtion(-counterdoorx, 0.0f, 0.0f, 0.0, 0.0, 0.0, 2.0f, 2.9f, .05f);
        cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
        cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
        //cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 0.0);

        model = transforamtion(counterdoorx + 2.0f, 0.0f, 0.0f, 0.0, 0.0, 0.0, 2.0f, 2.9f, .05f);
        cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
        cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
        // cube.drawCub eMe(lightingShader, model, 1.0, 0.0, 0.0);
    
    

}

//void Wheel_hollow(Cube& cube,Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
//{
//    glm::mat4 model;
//
//    //cyllinder
//    model = transforamtion(0, 0, 0, 0, 0, 0, 1, 1, 1);
//    curve_cyl.draw(lightingShader, alTogether * model, glm::vec3(0.52f, 0.39f, 0.31f));
//
//    //hollow cyllinder front
//    model = transforamtion(0, 2.23, 0, 0, 0, 0, .465, .05, .465);
//    curve_hollow_cyl.draw(lightingShader, alTogether * model, glm::vec3(0.471f, 0.471f, 0.42f));
//
//    //hollow cyllinder back
//    model = transforamtion(0, .945, 0, 0, 0, 0, .4575, .05, .4575);
//    curve_hollow_cyl.draw(lightingShader, alTogether * model, glm::vec3(0.471f, 0.471f, 0.42f));
//    /*
//    //wheel center cube1 for texture(front)
//    model = transforamtion(-0.4, 2.25, -0.4, 0, 0, 0, 0.8, .04, 0.8);
//    cube_cyl.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
//    cube_cyl.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    //wheel center cube2 for texture(back)
//    model = transforamtion(-0.4, 1.07, -0.4, 0, 0, 0, 0.8, .04, 0.8);
//    cube_cyl.setTextureProperty(cushion_tex, cushion_tex, 32.0f);
//    cube_cyl.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//    */
//
//}

//new add
void Wheel_hollow(Cube& cube_cyl,Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 model;

    //cyllinder
    model = transforamtion(0, 0, 0, 0, 0, 0, 1, 1, 1);
    curve_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    //hollow cyllinder front
    model = transforamtion(0, 2.23, 0, 0, 0, 0, .465, .05, .465);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f, 0.0f, 0.0f));

    //hollow cyllinder back
    model = transforamtion(0, .945, 0, 0, 0, 0, .4575, .05, .4575);
    curve_hollow_cyl.draw(lightingShaderWithTexture, alTogether * model, glm::vec3(1.0f, 0.0f, 0.0f));

    
    //wheel center cube1 for texture(front)
    model = transforamtion(-0.4, 2.25, -0.4, 0, 0, 0, 0.8, .04, 0.8);
    cube_cyl.setTextureProperty(tire_tex, tire_tex, 32.0f);
    cube_cyl.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);

    //wheel center cube2 for texture(back)
    model = transforamtion(-0.4, 1.07, -0.4, 0, 0, 0, 0.8, .04, 0.8);
    cube_cyl.setTextureProperty(tire_tex, tire_tex, 32.0f);
    cube_cyl.drawCubeWithTexture(lightingShaderWithTexture, alTogether * model);
    
}

void Bus(Cube& cube, Curve& curve_cyl, Curve& curve_hollow_cyl, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{


    //bus side view 


    glm::mat4 model = transforamtion(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.3, 2.0, 0.1);
   // cube.setTextureProperty(bus_tex, bus_tex, 32.0f);
   // cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether*model, 0.0, 0.0, 0.0);

    model = transforamtion(0.3, 1.6, 0.0, 0.0, 0.0, 0.0, 0.6, 0.4, 0.1);
    //cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);


     model = transforamtion(0.9, 0.0, 0.0, 0.0, 0.0, 0.0, 1.95, 2.0, 0.1);
    cube.setTextureProperty(bus_tex, bus_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    model = transforamtion(2.85, 1.6, 0.0, 0.0, 0.0, 0.0, 0.6, 0.4, 0.1);
    //cube.setTextureProperty(glassdoor_tex, floor_tiles, 32.0f);
    //cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);


    model = transforamtion(3.45, 0.0, 0.0, 0.0, 0.0, 0.0, 1.55, 2.0, 0.1);
    cube.setTextureProperty(bus_tex, bus_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);



    //door1

    if (busdoor && busdoorx < 0.6)
        busdoorx = busdoorx + .01f;

    if (busdoor==false && busdoorx > 0.0)
        busdoorx = busdoorx - .01f;
           
     model = transforamtion(0.3+busdoorx, 0.0,-0.01,         0.0, 0.0, 0.0,             0.6, 1.6,0.1);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

   //window1
    model = transforamtion(0.9, 0.85, -0.01,            0.0, 0.0, 0.0,          0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 0.0);

   //window2
    model = transforamtion(1.5, 0.85, -0.01,             0.0, 0.0, 0.0,         0.75, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   // cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 1.0);

    //window3
    model = transforamtion(2.25, 0.85, -0.01,        0.0, 0.0, 0.0,                 0.6, 0.75, 0.12);
     cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 1.0);


    //door2
    model = transforamtion(2.85+ busdoorx, 0.0, -0.01,    0.0, 0.0, 0.0,          0.6, 1.6, 0.1);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    //window4
    model = transforamtion(3.45, 0.85, -0.01, 0.0, 0.0, 0.0, 0.75, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 1.0);


    //window5
    model = transforamtion(4.2, 0.85, -0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 0.0, 1.0);



    //back

    model = transforamtion(5.0, 0.0, -1.8+.1, 0.0, 0.0, 0.0, 0.1, 2.0, 1.8);
    cube.setTextureProperty(busback_tex, busback_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 1.0);

    model = transforamtion(5.0-0.01, 1.0+0.2, -1.55, 0.0, 0.0, 0.0, 0.12, 0.6, 1.5);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);



    //bus side view 


    model = transforamtion(0.0, 0.0, -1.7, 0.0, 0.0, 0.0, 5.0, 2.0, 0.1);
    cube.setTextureProperty(bus_tex, bus_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

    //w
    model = transforamtion(0.3, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 0.0);


    //window1
    model = transforamtion(0.9, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 0.0);

    //window2
    model = transforamtion(1.5, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.75, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    // cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 1.0);

     //window3
    model = transforamtion(2.25, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 1.0);

     //w
    model = transforamtion(2.85, 0.85, -1.7-0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 1.0, 0.0);


    //window4
    model = transforamtion(3.45, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.75, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 0.0, 1.0);


    //window5
    model = transforamtion(4.2, 0.85, -1.7 - 0.01, 0.0, 0.0, 0.0, 0.6, 0.75, 0.12);
    cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, model, 0.0, 0.0, 1.0);

    //bus front 

    model=transforamtion(0.0-.1, 0.0,-1.8+.1, 0.0, 0.0, 0.0, 0.1, 2.0, 1.8);
    cube.setTextureProperty(busfront_tex, busfront_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   // cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.0, 1.0);

    model = transforamtion(0.0 -.1-.01, 0.6, -1.8 + .1-0.01,           0.0, 0.0, 0.0,            0.18, 1.0, 1.82);
   cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
   cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
  //  cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);


    //bus below
    model = transforamtion(0.0,0.0,-1.8+0.1+0.1,                   0.0, 0.0, 0.0,                 5.0, 0.1, 1.8-0.2);
  cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
  cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   // cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 1.0, 0.0);



    //up

    model = transforamtion(0.0-0.1, 2.0, -1.8 + 0.1 ,        0.0, 0.0, 0.0,        5.2, 0.1, 1.8);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
  // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 1.0, 0.0);


    //bus wheel front

    model = transforamtion(1.35f, 0.15f, 0.0f-0.15f, 90.0f, 0.0f, 0.0f, 0.5f, .12f, .5f);
    Wheel_hollow(cube, curve_cyl, curve_hollow_cyl,lightingShader, lightingShaderTexture, alTogether* model);

    model = transforamtion(3.90f, 0.15f, 0.0f- 0.15f, 90.0f, 0.0f, 0.0f, 0.5f, .12f, .5f);
    Wheel_hollow(cube, curve_cyl, curve_hollow_cyl, lightingShader, lightingShaderTexture, alTogether* model);


    model = transforamtion(1.35f, 0.15f, -1.8f-0.05f ,  90.0f, 0.0f, 0.0f, 0.5f, .12f, .5f);
    Wheel_hollow(cube, curve_cyl, curve_hollow_cyl, lightingShader, lightingShaderTexture, alTogether* model);

    model = transforamtion(3.90f, 0.15f, -1.8f-0.05f, 90.0f, 0.0f, 0.0f, 0.5f, .12f, .5f);
    Wheel_hollow(cube, curve_cyl, curve_hollow_cyl, lightingShader, lightingShaderTexture, alTogether* model);



    //looking glasss

    model = transforamtion(0.0-0.4, 2.0-0.15f, 0.0+.05             ,0.0, 0.0, 0.0,   0.3, 0.08, 0.05);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);

    model = transforamtion(0.0 - 0.54, 2.0f - 0.18f-0.06f, 0.0+0.05 ,      0.0, 0.0, 30.0,                 0.2, 0.08, 0.05);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);

    model = transforamtion(0.0 - 0.55, 2.0f - 0.5f , 0.0-0.05 ,          0.0, 0.0, 0.0,    0.05, 0.3, 0.2);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);


    model = transforamtion(0.0 - 0.4, 2.0 - 0.15f, -1.8 + 0.1, 0.0, 0.0, 0.0, 0.3, 0.08, 0.05);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);

    model = transforamtion(0.0 - 0.54, 2.0f - 0.18f - 0.06f, -1.8 + 0.1, 0.0, 0.0, 30.0, 0.2, 0.08, 0.05);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);

    model = transforamtion(0.0 - 0.55, 2.0f - 0.5f, -1.8, 0.0, 0.0, 0.0, 0.05, 0.3, 0.2);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.0, 0.0, 0.0);



}
void Waiting_room_base(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    glm::mat4 model = transforamtion(14.0f, 0.0f, -12.0f, 0.0f, 0.0f, 0.0f,      14.0f, 0.3f, 20.0f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);
}


void Waiting_room(Cube& cube55,Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
     
    //base
    glm::mat4 model = transforamtion(14.0f,0.0f,-10.0f,     0.0f,0.0f,0.0f,            10.0f,0.01f,16.0f);
    cube55.setTextureProperty(divider_tex, divider_tex, 32.0f);
     cube55.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   // cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 0.5);


    //up
    model = transforamtion(14.0f, 3.5f, -10.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.1f, 16.0f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 0.5);

    //left1
    model = transforamtion(14.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.1f, 3.5f, 4.0f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;

    //left2
    model = transforamtion(14.0f, 0.0f, -6.0f, 0.0f, 0.0f, 0.0f, 0.1f, 3.5f, 6.0f);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
     cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 0.5, 1.0);

    //left3 up
    model = transforamtion(14.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 1.5f, 3.0f);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
     cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, model, 1.0, 1.0, 0.5);

    //left3 down
    model = transforamtion(14.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 2.0f, 1.0f);
    cube.setTextureProperty(glassdoor_tex, glassdoor_tex, 32.0f);
     cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.5);


    //left4
    model = transforamtion(14.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.1f, 3.5f, 3.0f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;


    //piller 1

    model = transforamtion(14.0f-0.01f, 0.0f, -6.0f-0.25f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.5f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);

    //piller 2
    model = transforamtion(14.0f - 0.01f, 0.0f, -3.0f - 0.25f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.5f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);


    //piller 3
    model = transforamtion(14.0f - 0.01f, 0.0f, 0.0f - 0.25f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.5f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);


    //piller 3

    model = transforamtion(14.0f - 0.01f, 0.0f, 3.0f - 0.25f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.5f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);


    //pilller small1 1
    model = transforamtion(14.0f - 0.01f, 0.0f, -5.0f - 0.05f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.1f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);

    //pilller small1 2
    model = transforamtion(14.0f - 0.01f, 0.0f, -4.0f - 0.05f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.1f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);


    //pilller small1 3
    model = transforamtion(14.0f - 0.01f, 0.0f, -2.0f - 0.05f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.1f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);


    //pilller small1 4
    model = transforamtion(14.0f - 0.01f, 0.0f, -1.0f - 0.05f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.1f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 1.0);

    // pilller small1 5
    model = transforamtion(14.0f - 0.01f, 0.0f, 1.0f - 0.05f, 0.0f, 0.0f, 0.0f, 0.12f, 3.5f, 0.1f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);




    //p1
    model = transforamtion(14.0f - 0.01f, 3.5f-0.1f , -6.0f, 0.0f, 0.0f, 0.0f, 0.12f, 0.1f, 9.0f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    //p2
    model = transforamtion(14.0f - 0.01f, 3.0f - 0.1f, -6.0f, 0.0f, 0.0f, 0.0f, 0.12f, 0.1f, 9.0f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    //p3
    model = transforamtion(14.0f - 0.01f, 2.5f - 0.1f, -6.0f, 0.0f, 0.0f, 0.0f, 0.12f, 0.1f, 9.0f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);

    //p4
    model = transforamtion(14.0f - 0.01f, 2.f - 0.1f, -6.0f, 0.0f, 0.0f, 0.0f, 0.12f, 0.1f, 9.0f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 1.0, 1.0);





    

    //right
    model = transforamtion(24.0f-0.1f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.1f, 3.5f, 16.0f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;

    //back
    model = transforamtion(14.0f+0.1f ,0.0f, -10.0f-0.1f, 0.0f, 0.0f, 0.0f, 10.f-0.2f, 3.5f, 0.1f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;

    //front
    model = transforamtion(14.0f+0.1f, 0.0f, 6.0f-0.1f, 0.0f, 0.0f, 0.0f, 10.f-0.2f, 3.5f, 0.1f);
    cube.setTextureProperty(wall1_tex, wall1_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether* model, 1.0, 0.5, 0.0);;



    //chair
        int numberofchair = 6;

        for (int i = 0; i < numberofchair; i++)
        {
            model = transforamtion(21.5f,0.0f,-9.0f+i*1.5f,  0.0,0.0,0.0, 1.0,1.0,1.0);
            Chair(cube, lightingShader, lightingShaderTexture, alTogether * model);

            model = transforamtion(18.5f, 0.0f, -9.0f + i * 1.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
            Chair(cube, lightingShader, lightingShaderTexture, alTogether * model);

            model = transforamtion(15.5f, 0.0f, -9.0f + i * 1.5f, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
            Chair(cube, lightingShader, lightingShaderTexture, alTogether*model);

        }
        //wall1
        model = transforamtion(14.0f+.1f , 0.0f, 3.0f , 0.0f, 0.0f, 0.0f, 1.0,3.5,0.2);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 1.5, 0.5, 1.5);
        //wall2
        model = transforamtion(14.0f + .1f+1.0f, 0.0f, 3.0f+0.1, 0.0f, 0.0f, 0.0f, 0.5, 3.5, 0.1);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.5, 1.5);

        //food counter below
        model = transforamtion(14.0f + .1f + 1.0f+.5f, 0.0f, 3.0f + 0.1f, 0.0f, 0.0f, 0.0f,8.3, 1.1, 0.1);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 1.0, 1.0);

        //food counter up
        model = transforamtion(14.0f + .1f + 1.0f + .5f, 2.2f, 3.0f + 0.1f, 0.0f, 0.0f, 0.0f, 8.3, 1.3, 0.1);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        //cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 1.0, 1.0);
        
        ////food counter middle
        model = transforamtion(14.0f + .1f + 1.0f + .5f, 1.0f, 3.0f + 0.1f+0.01, 0.0f, 0.0f, 0.0f, 8.3, 0.1, 1.0);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 1.0);

        //tv
       
        model = transforamtion(19.0f, 2.20f+.15f, 3.0f + 0.1f - 0.01, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, false);


        model = transforamtion(22.0f, 2.20f + .15f, 3.0f + 0.1f - 0.01, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, false);

        model = transforamtion(16.0f, 2.20f + .15f, 3.0f + 0.1f - 0.01, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, false);

        //pc
        model = transforamtion(19.0f, 1.00f + .15f, 3.0f+0.5f , 0.0f, 0.0f, 0.0f, 0.6, 0.6,0.6);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, true);


        model = transforamtion(22.0f, 1.00f + .15f, 3.0f + 0.5f, 0.0f, 0.0f, 0.0f, 0.6, 0.6, 0.6);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, true);

        model = transforamtion(16.0f, 1.00f + .15f, 3.0f + 0.5f, 0.0f, 0.0f, 0.0f, 0.6,0.6,0.6);
        tvorpc(cube, lightingShader, lightingShaderTexture, alTogether * model, true);



    
}

void Chair(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    glm::mat4 model = transforamtion(0.0, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f, 0.5f);
    cube.setTextureProperty(seat_blue_tex, seat_blue_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(0.0, 0.3f, 0.0f, -10.0f, 0.0f, 0.0f, 0.5f, 0.7f, 0.05f);
    cube.setTextureProperty(seat_blue_tex, seat_blue_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

     model = transforamtion(0.55, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f, 0.5f);
     cube.setTextureProperty(seat_sky_tex, seat_sky_tex, 32.0f);
     cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
     //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(0.55, 0.3f, 0.0f, -10.0f, 0.0f, 0.0f, 0.5f, 0.7f, 0.05f);
    cube.setTextureProperty(seat_sky_tex, seat_sky_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);


    model = transforamtion(1.1, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f, 0.5f);
    cube.setTextureProperty(seat_blue_tex, seat_blue_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(1.1, 0.3f, 0.0f, -10.0f, 0.0f, 0.0f, 0.5f, 0.7f, 0.05f);
    cube.setTextureProperty(seat_blue_tex, seat_blue_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(1.65, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.05f, 0.5f);
    cube.setTextureProperty(seat_sky_tex, seat_sky_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(1.65, 0.3f, 0.0f, -10.0f, 0.0f, 0.0f, 0.5f, 0.7f, 0.05f);
    cube.setTextureProperty(seat_sky_tex, seat_sky_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.5, 1.5);

    model = transforamtion(0.0+ 1.95 * .55, 0.0 +0.25, 0.0+0.1,                     0.0, 0.0, 90.0,                    0.605 * .1, 1.95 * 0.55, 0.605 * .1);
    drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);
   


    //leg1f
    model = transforamtion(0.0, 0.0f, 0.0f+.22,                -30.0f, 0.0f, 0.0f,            0.02f, 0.25f, 0.02f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
   // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.0, 1.5);



   //leg1b
    model = transforamtion(0.0, 0.0f, 0.0f-0.05, 30.0f, 0.0f, 0.0f, 0.02f, 0.25f, 0.02f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
   // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.0, 1.5);



   // leg2f
    model = transforamtion(2.1, 0.0f, 0.0f + .22, -30.0f, 0.0f, 0.0f, 0.02f, 0.25f, 0.02f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
   // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.5, 0.0, 1.5);



    //leg2b
    model = transforamtion(2.1, 0.0f, 0.0f - 0.05, 30.0f, 0.0f, 0.0f, 0.02f, 0.25f, 0.02f);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
   // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether* model, 0.5, 0.0, 1.5);




    

}
void tvorpc(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether,bool ispc)
{
    glm::mat4 model;
    float x = 0.0;
    if (ispc == false)
        x = -0.3f;
    //texture is here
    if (ispc == true)
    {
        model = transforamtion(0.0, 0.3 + x, 0.0, 0.0, 0.0, 0.0, 1.5, 1.0, 0.05);
        //cube.setTextureProperty(tv_tex, tv_tex, 32.0f);
       // cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);
    }
    else
    {
        model = transforamtion(0.0, 0.3 + x, 0.0, 0.0, 0.0, 0.0, 1.5, 1.0, 0.05);
        cube.setTextureProperty(tv_tex, tv_tex, 32.0f);
        cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    }
   

    //down
    model = transforamtion(0.0, 0.3+x, 0.0-0.01, 0.0, 0.0, 0.0, 1.5,0.05, 0.07);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    //left
    model = transforamtion(0.0, 0.3+x, 0.0 - 0.01, 0.0, 0.0, 0.0, 0.05, 1.0, 0.07);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    //right
    model = transforamtion(0.0+1.5-0.05, 0.3+x, 0.0 - 0.01, 0.0, 0.0, 0.0, 0.05, 1.0, 0.07);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    //up
    model = transforamtion(0.0, 0.3+x+1.0-0.05, 0.0 - 0.01, 0.0, 0.0, 0.0, 1.5, 0.05, 0.07);
    //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
    // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

    if (ispc)
    {
        model = transforamtion(0.0+.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.05, 0.3, 0.05);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

        model = transforamtion(0.0 + .75-0.15, 0.0, 0.0-0.15, 0.0, 0.0, 0.0, 0.3,0.05,0.3);
        //cube.setTextureProperty(bus_window_tex, bus_window_tex, 32.0f);
        // cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
        cube.drawCubeMe(lightingShader, alTogether * model, 0.0, 0.0, 0.0);
    }


}
void Rood(Cube& cube1, Cube& cube2, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    //left
    glm::mat4 model = transforamtion(9.0f, 0.0f, -24.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.01f, 44.0f);
    cube2.setTextureProperty(road1_tex, road1_tex, 32.0f);
     cube2.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube2.drawCubeMe(lightingShader, model, 0.5,0.5,0.5);

    //back
    model = transforamtion(-26.5f, 0.01f, -17.0f-2.5f, 0.0f, 0.0f, 0.0f, 54.5f, 0.01f, 3.0f);
    cube1.setTextureProperty(road2_tex,road2_tex, 32.0f);
     cube1.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   // cube1.drawCubeMe(lightingShader, model, 0.5, 0.5, 0.5);

     model = transforamtion(-26.5f, 0.01f, -17.0f - 2.5f+4.5f, 0.0f, 0.0f, 0.0f, 54.5f, 0.01f, 3.0f);
     cube1.setTextureProperty(road2_tex, road2_tex, 32.0f);
     cube1.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
     // cube1.drawCubeMe(lightingShader, model, 0.5, 0.5, 0.5);





    //front
    model = transforamtion(-26.5f, 0.01f, 8.0f, 0.0f, 0.0f, 0.0f, 54.5f, 0.01f, 3.0f);
    cube1.setTextureProperty(road2_tex, road2_tex, 32.0f);
    cube1.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube1.drawCubeMe(lightingShader, model, 0.5, 0.5, 0.5);

    model = transforamtion(-26.5f, 0.01f, 8.0f+4.5f, 0.0f, 0.0f, 0.0f, 54.5f, 0.01f, 3.0f);
    cube1.setTextureProperty(road2_tex, road2_tex, 32.0f);
    cube1.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube1.drawCubeMe(lightingShader, model, 0.5, 0.5, 0.5);

    //right
    model = transforamtion(-22.0f, 0.0f, -24.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.01f, 44.0f);
    cube2.setTextureProperty(road1_tex, road1_tex, 32.0f);
     cube2.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
   // cube2.drawCubeMe(lightingShader, model, 0.5, 0.5, 0.5);



}
void Lamp_post(Cube& cube,Sphere& sphere, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    glm::mat4 model = transforamtion(0.0, 0.0 + 0.1 * 0.55, 0.0, 0.0, 0.0, 0.0, 0.605 * .4, 0.1 * 0.55, 0.605 * .4);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    model = transforamtion(0.0, 0.1 + 0.1 * 0.55, 0.0, 0.0, 0.0, 0.0, 0.605 * .3, 0.1 * 0.55, 0.605 * .3);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    model = transforamtion(0.0, 0.2 + 0.1 * 0.55, 0.0, 0.0, 0.0, 0.0, 0.605 * .2, 0.1 * 0.55, 0.605 * .2);
    drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 1.0, 0.0);

    model = transforamtion(0.0, 0.0+ 4.0 * 0.55,0.0 , 0.0, 0.0, 0.0, 0.605 * .10, 4.0 * 0.55, 0.605 * .10);
  drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);



   model = transforamtion(0.0 , 0.0 + 4.0, 0.0, -90.0, 0.0, 0.0, 0.605 * .05, 0.5 * 0.55, 0.605 * .05);
  // drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

   model = transforamtion(0.0, 0.0 + 4.0, 0.0-0.3, -60.0, 0.0, 0.0, 0.605 * .05, 0.5 * 0.55, 0.605 * .05);
   drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);
   
   model = transforamtion(0.0, 0.0 + 4.0, 0.0+0.3, 60.0, 0.0, 0.0, 0.605 * .05, 0.5 * 0.55, 0.605 * .05);
  drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);


  model = transforamtion(0.0, 0.0 + 4.15, 0.0+0.8, -90.0, 0.0, 0.0, 0.605 * .05, 0.5 * 0.55, 0.605 * .05);
  drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

  model = transforamtion(0.0, 0.0 + 4.15, 0.0 - 0.8, -90.0, 0.0, 0.0, 0.605 * .05, 0.5 * 0.55, 0.605 * .05);
  drawCylinderwithlighting(lightingShader, alTogether * model, 0.0, 0.0, 0.0);

   //model = transforamtion(0.0, 0.0 + 4.0, 9.0, 0.0, 0.0, 0.0, 0.605 * .05, 1.0 * 0.55, 0.605 * .05);
   //drawCylinderwithlighting(lightingShader, alTogether * model, 1.0, 0.0, 0.0);

     model = transforamtion(0.0, 4.4-0.1, 0.0+1.1,0.0,0.0,0.0,  0.2, 0.2, 0.2);
     sphere.drawSphere(lightingShader, alTogether*model);

     model = transforamtion(0.0, 4.4 - 0.1, 0.0 - 1.1, 0.0, 0.0, 0.0, 0.2, 0.2, 0.2);
     sphere.drawSphere(lightingShader, alTogether * model);



}
void Divider(Cube& cube, Sphere& sphere, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{


    //front mid
    glm::mat4 model = transforamtion(-17.0f+1.5f, 0.0f, 8.0f+3.0f, 0.0f, 0.0f, 0.0f, 26.0f-3.0f, 0.3f, 1.5f);
    cube.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether*model, 0.5, 0.5, 0.5);


    model = transforamtion(-17.0f +1.5f+0.5f, .3f, 8.0f + 3.0f+0.25f, 0.0f, 0.0f, 0.0f, 26.0f-3.0f-1.0f, 0.2f, 1.0f);
    cube.setTextureProperty(divider_tex, divider_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.5);

    //front front
     model = transforamtion(14.0f + 1.5f, 0.0f, 8.0f + 3.0f, 0.0f, 0.0f, 0.0f, 14.0f - 1.5f, 0.3f, 1.5f);
    cube.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.5);


    model = transforamtion(14.0f + 1.5f + 0.5f, .3f, 8.0f + 3.0f + 0.25f, 0.0f, 0.0f, 0.0f, 14.0f - 1.5f - 0.5f, 0.2f, 1.0f);
    cube.setTextureProperty(divider_tex, divider_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.5);

    int numoflamppost = 5;
    for (int i = 0; i < numoflamppost; i++)
    {
        model = transforamtion(-17.0f + 1.5f + 2.0f+i*(19.0f/ float(numoflamppost-1)), 0.5f, 8.0f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);
    }

    model = transforamtion(14.0f + 1.5f + 2.0f+0*4.75f, 0.5f, 8.0f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);

    model = transforamtion(14.0f + 1.5f + 2.0f + 1 * 4.75f, 0.5f, 8.0f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);

    model = transforamtion(14.0f + 1.5f + 2.0f + 2 * 4.75f, 0.5f, 8.0f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);





    //back
   //back mid
     model = transforamtion(-17.0f + 1.5f, 0.0f, -19.5f + 3.0f, 0.0f, 0.0f, 0.0f, 26.0f - 3.0f, 0.3f, 1.5f);
     cube.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
     cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
   // cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.5);


    model = transforamtion(-17.0f + 1.5f + 0.5f, .3f, -19.5f + 3.0f + 0.25f, 0.0f, 0.0f, 0.0f, 26.0f - 3.0f - 1.0f, 0.2f, 1.0f);
    cube.setTextureProperty(divider_tex, divider_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.5);

    //back front
    model = transforamtion(14.0f + 1.5f, 0.0f, -19.5f + 3.0f, 0.0f, 0.0f, 0.0f, 14.0f - 1.5f, 0.3f, 1.5f);
    cube.setTextureProperty(divider2_tex, divider2_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.5);


    model = transforamtion(14.0f + 1.5f + 0.5f, .3f, -19.5f + 3.0f + 0.25f, 0.0f, 0.0f, 0.0f, 14.0f - 1.5f - 0.5f, 0.2f, 1.0f);
    cube.setTextureProperty(divider_tex, divider_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 1.0, 0.5);


    for (int i = 0; i < numoflamppost; i++)
    {
        model = transforamtion(-17.0f + 1.5f + 2.0f + i * (19.0f / float(numoflamppost - 1)), 0.5f, -19.5f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);
    }

    model = transforamtion(14.0f + 1.5f + 2.0f + 0 * 4.75f, 0.5f, -19.5f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);

    model = transforamtion(14.0f + 1.5f + 2.0f + 1 * 4.75f, 0.5f, -19.5f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);

    model = transforamtion(14.0f + 1.5f + 2.0f + 2 * 4.75f, 0.5f, -19.5f + 3.0f + .75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    Lamp_post(cube, sphere, lightingShader, lightingShaderTexture, model);

}




void Tree(Cube& cube, Curve& treeshape, Curve& treebelow,Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{
    glm::mat4 model = transforamtion(0.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0);
    treeshape.draw(lightingShaderTexture, alTogether*model, glm::vec3(0.0f, 0.0f, 0.0f));


    model = transforamtion(0.0, -0.5, 0.0, 0.0, 0.0, 0.0, 1.5, 1.0, 1.5);
    treeshape.draw(lightingShaderTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));


    model = transforamtion(0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.8, 1.0, 0.8);
    treeshape.draw(lightingShaderTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    model = transforamtion(0.0, 1.3, 0.0, 0.0, 0.0, 0.0, 0.6, 0.8, 0.6);
    treeshape.draw(lightingShaderTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));

    model = transforamtion(0.0, 0.4, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    treebelow.draw(lightingShaderTexture, alTogether * model, glm::vec3(0.0f, 0.0f, 0.0f));





}
void Readside(Cube& cube, Curve& treeshape, Curve& treebelow, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
{

    glm::mat4 model = transforamtion(-17.0f, 0.0f, 15.5f, 0.0f, 0.0f, 0.0f, 26.0f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);


    model = transforamtion(14.0f, 0.0f, 15.5f, 0.0f, 0.0f, 0.0f, 14.0f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);


    //grass
    model = transforamtion(-17.0f+1.0, 0.3f, 15.5f+1.0, 0.0f, 0.0f, 0.0f, 26.0f-2.0, 0.01f, 4.5f-1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether* model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);


    model = transforamtion(14.0f + 1.0, 0.3f, 15.5f + 1.0, 0.0f, 0.0f, 0.0f, 14.0f - 1.0, 0.01f, 4.5f - 1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);

    //tree
    int numoftree = 5;
    for (int i = 0; i < numoftree; i++)
    {
        model = transforamtion(-17.0f + 1.0 + 2.0+i*float(20.0f/(numoftree-1)), 0.3f, 15.5f + 1.0 + 1.75, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        Tree(cube, treeshape, treebelow, lightingShader, lightingShaderTexture, model);
    }
    numoftree = 3;
    for (int i = 0; i < numoftree; i++)
    {
        model = transforamtion(14.0f + 1.0 + 2.0 + i * 5.0f, 0.3f, 15.5f + 1.0 + 1.75, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        Tree(cube, treeshape, treebelow, lightingShader, lightingShaderTexture, model);
    }











    //back

    model = transforamtion(-17.0f, 0.0f, -24.0f, 0.0f, 0.0f, 0.0f, 26.0f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);

    model = transforamtion(14.0f, 0.0f, -24.0f, 0.0f, 0.0f, 0.0f, 14.0f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);


    //grass back
    model = transforamtion(-17.0f + 1.0, 0.3f, -24.0f, 0.0f, 0.0f, 0.0f, 26.0f - 2.0, 0.01f, 4.5f - 1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);

    model = transforamtion(14.0f + 1.0, 0.3f, -24.0f, 0.0f, 0.0f, 0.0f, 14.0f - 1.0, 0.01f, 4.5f - 1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);

    numoftree = 5;
    for (int i = 0; i < numoftree; i++)
    {
        model = transforamtion(-17.0f + 1.0 + 2.0 + i * float(20.0f / (numoftree - 1)), 0.3f, -24.0f+ 1.75, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        Tree(cube, treeshape, treebelow, lightingShader, lightingShaderTexture, model);
    }
    numoftree = 3;
    for (int i = 0; i < numoftree; i++)
    {
        model = transforamtion(14.0f + 1.0 + 2.0 + i * 5.0f, 0.3f, -24.0f + 1.75, 0.0f, 0.0f, 0.0f, 1.0, 1.0, 1.0);
        Tree(cube, treeshape, treebelow, lightingShader, lightingShaderTexture, model);
    }


    //right

    model = transforamtion(-26.5f, 0.0f, -12.0f, 0.0f, 0.0f, 0.0f, 4.5f, 0.3f,20.f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);

    model = transforamtion(-26.5f, 0.0f, -24.0f, 0.0f, 0.0f, 0.0f, 4.5f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);


    model = transforamtion(-26.5f, 0.0f, 15.5f, 0.0f, 0.0f, 0.0f, 4.5f, 0.3f, 4.5f);
    cube.setTextureProperty(footpath_tex, footpath_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 1.0);


    // glass

    model = transforamtion(-26.5f, 0.3f, -12.0f+1.0f, 0.0f, 0.0f, 0.0f, 4.5f-1.0, 0.01f, 20.f-2.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);


    model = transforamtion(-26.5f, 0.3f, -24.0f, 0.0f, 0.0f, 0.0f, 4.5f-1.0, 0.01f, 4.5f-1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);

    model = transforamtion(-26.5f, 0.3f, 15.5f+1.0f, 0.0f, 0.0f, 0.0f, 4.5f-1.0, 0.01f, 4.5f-1.0);
    cube.setTextureProperty(grass_tex, grass_tex, 32.0f);
    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
    //cube.drawCubeMe(lightingShader, alTogether * model, 1.0, 0.5, 0.0);



    numoftree = 4;
    for (int i = 0; i < numoftree; i++)
    {
        model = transforamtion(-26.5f+1.75, 0.3f, -12.0f + 1.0f+2.0f+ i * float(14.0f / (numoftree - 1)), 0.0f, 0.0f, 0.0f, 1.0,1.0,1.0);
        Tree(cube, treeshape, treebelow, lightingShader, lightingShaderTexture, model);
    }






}

//void ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 model = transform(-2.0, -.8, -4, 0, 0, 0, .15, 5.3, .15);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    model = transform(-1.4, -.8, -4, 0, 0, 0, .15, 5.3, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, -.4, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0.4, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 0.8, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    model = transform(-2.0, 1.2, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    model = transform(-2.0, 1.6, -4, 0, 0, 0, 1.2, 0.15, .15);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void bed(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    float baseHeight = 0.3;
//    float width = 1;
//    float length = 2;
//    float pillowWidth = 0.3;
//    float pillowLength = 0.15;
//    float blanketWidth = 1.0;
//    float blanketLength = 1.8;
//    float headHeight = 0.6;
//
//    //base
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 translate = glm::mat4(1.0f);
//    glm::mat4 translate2 = glm::mat4(1.0f);
//    glm::mat4 scale = glm::mat4(1.0f);
//    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//    cube.setTextureProperty(ch_wood_tex, ch_wood_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //foam
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight / 2, 0));
//    scale = glm::scale(model, glm::vec3(width, 0.06, length));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);
//    cube.setTextureProperty(cushion_tex, cushion_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //pillow 1
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((width / 4) - (0.1 + pillowWidth / 2), baseHeight / 2 + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.46, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //pillow 2
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3((-width / 4) + (0.1 + pillowWidth / 2), baseHeight / 2 + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
//    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
//    translate = glm::translate(model, glm::vec3(-0.21, 0.0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);
//    cube.drawCubeWithTexture(lightingShader, model);
//
//    //blanket
//    /*model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, baseHeight/2 + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
//    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
//    translate = glm::translate(model, glm::vec3(-0.5, -0.05, -0.41));
//    model = alTogether * translate2 * translate * scale ;
//    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);*/
//
//    //head
//    model = glm::mat4(1.0f);
//    translate = glm::mat4(1.0f);
//    translate2 = glm::mat4(1.0f);
//    scale = glm::mat4(1.0f);
//    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
//    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
//    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
//    model = alTogether * translate2 * translate * scale;
//    //drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);
//    cube.setTextureProperty(almari_tex, almari_tex, 32.0);
//    cube.drawCubeWithTexture(lightingShader, model);
//}
//
//void Table(Cube &cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //cube.setMaterialisticProperty(glm::vec3(0.862f, 0.46f, 0.321f));
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.2f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.2f, 3.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -0.1f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -1.49f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -1.49f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//}
//
//void Sofa(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //Back
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.19f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.55f, 2.0f, 0.5f));
//    alTogether = rotateYMatrix * alTogether;
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.4f, 0.226f, 0.44f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Seat
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.5f, 0.0f));//translate_X, translate_Y, translate_Z
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 1.0f, 2.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    ////left hand
//    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(1.1f, -0.5f, -1.0f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 2.0f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.setMaterialisticProperty(glm::vec3(0.52f, 0.39f, 0.31f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    ////right hand
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.79f, -0.5f, -1.0f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    ////left Leg
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.79f, 0.35f, -1.0f));//translate_X, translate_Y, translate_Z
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    ////right Leg
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(1.08f, 0.35f, -1.0f));//translate_X, translate_Y, translate_Z
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, 2.0f));
//    model = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//}
//
//
//void Ladder(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.77006, -0.75, 14.1102));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//
//    ladder(cube, lightingShader, translateMatrix * rotateZMatrix);
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.77006, 0.62, 14.75));
//    ladder(cube, lightingShader, translateMatrix * rotateZMatrix);
//    
//}
//
//
//void FWall(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(35.0f, 12.0f, 0.1f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model = scaleMatrix * translateMatrix;
//    //moveMatrix = rotateZMatrix * moveMatrix;
//    lightingShader.setMat4("model", alTogether * model);
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.53f, 0.81f, 0.92f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//
//void DiningTable(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.3f, 3.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.478, 0.573, 0.62);
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 2.52, -0.025));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5f, 0.05f, 3.5f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //ourShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix1 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -0.1f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.8f, 0.0f, -1.49f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix3 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.6f, 0.0f, -1.49f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix4 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.2f, 1.7f, 0.2f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//}
//
//void Tool(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.2f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2f, 2.0f));
//    model = scaleMatrix * translateMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.0f, 0.4f, 0.18f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Leg
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;//translate_X, translate_Y, translate_Z
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.05f, -0.86f, 0.36f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.5f, 1.7f, 0.5f));
//    model1 = translateMatrix1 * scaleMatrix1;
//    cube.setMaterialisticProperty(glm::vec3(0.52f, 0.39f, 0.31f));
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model1);
//
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.16f, -4.27f, 0.21f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//}
//
//void Chair(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //Seat
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.22, -1.0, 0.05));//
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.1f, 1.0f));
//    alTogether = rotateYMatrix * alTogether;
//    model = scaleMatrix * translateMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model, 0.165, 0.435, 0.451);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //Back
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.22, 0.15f, 0.5f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.7f, 0.1f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
//    translateMatrix1 = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.1, -0.1f));
//    rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model1 = rotateXMatrix1 * translateMatrix1 * scaleMatrix1;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model1);
//
//    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//    translateMatrix2 = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.1, -0.5f));
//    rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model2 = rotateXMatrix2 * translateMatrix2 * scaleMatrix2;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model2);
//
//    // Modelling Transformation
//    glm::mat4 identityMatrix3 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix3, rotateXMatrix3, rotateYMatrix3, rotateZMatrix3, scaleMatrix3, model3;
//    translateMatrix3 = glm::translate(identityMatrix, glm::vec3(0.2f, 0.1, -0.1f));
//    rotateXMatrix3 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix3 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model3 = rotateXMatrix3 * translateMatrix3 * scaleMatrix3;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model3);
//    // Modelling Transformation
//
//    glm::mat4 translateMatrix4, rotateXMatrix4, rotateYMatrix4, rotateZMatrix4, scaleMatrix4, model4;
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(0.2f, 0.1, -0.5f));
//    rotateXMatrix4 = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
//    model4 = rotateXMatrix4 * translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.1f, 0.05f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
//    model4 = translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//    translateMatrix4 = glm::translate(identityMatrix, glm::vec3(0.2f, -0.1f, 0.05f));
//    scaleMatrix4 = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
//    model4 = translateMatrix4 * scaleMatrix4;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model4);
//
//}
//
//void Bookself(Cube& cube, Shader& lightingShader, Shader& lightingShaderTexture, glm::mat4 alTogether)
//{
//    // base
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    //drawCube(cubeVAO, lightingShader, alTogether * model,);
//    //cube.setMaterialisticProperty(glm::vec3(0.462f, 0.258f, 0.145f));
//    cube.setTextureProperty(black_tex, black_tex, 32.0f);
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 1.3, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 1
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, 1.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 2
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, .7, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .1, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // mid 3
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, .35, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .1, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    // left
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, .05, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 2.5, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    //right
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8, .05, 1));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 2.5, 0.5));
//    model = translateMatrix * scaleMatrix;
//    cube.drawCubeWithTexture(lightingShaderTexture, alTogether * model);
//
//    //Books
//    float colors[] = { 0.545455, 0.909091, 0.545455, 0.181818, 0.0909091, 0.363636, 0, 0.545455, 0.272727, 0.0909091, 0.727273, 0.636364, 0.454545, 0.272727, 0.636364, 0.363636, 0.818182, 0.909091, 0.181818, 0, 0.909091, 0.727273, 0.454545, 0, 0.363636, 0.545455, 0, 0.909091, 0.272727, 0.909091, 0.909091, 0.636364, 0.909091, 0.272727, 0.636364, 0.818182 };
//    int k = 0;
//    for (float j = 0; j <= 0.9; j = j + 0.3)
//    {
//        for (float i = 0; i < 0.2; i = i + 0.06)
//        {
//            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.75 - i, 1.05 - j, 1.0));
//            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 0.6));
//            model = translateMatrix * scaleMatrix;
//            k = k + 3;
//            //drawCube(cubeVAO, lightingShader, alTogether * model, colors[k], colors[k + 1], colors[k + 2]);
//            cube.setMaterialisticProperty(glm::vec3(colors[k], colors[k + 1], colors[k + 2]));
//            cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//        }
//    }
//
//
//}
//
//void TV(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 4.0f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithMaterialisticProperty(lightingShader, alTogether * model);
//
//}
//void Show(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.70f, 3.70f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//}
//
//void Door(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 9.0f, 0.2f));
//    model = scaleMatrix * translateMatrix;
//    //cube.setMaterialisticProperty(glm::vec3(0.0f, 0.0f, 0.0f));
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//}
//void TV_Trolly(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 2.0f, 1.0f));
//    model = scaleMatrix * translateMatrix;
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void Wardrobe(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//    //WARDROBE 
//        //back
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    model = transform(6.95, -0.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 7.0f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //left
//    model = transform(5.95, -0.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //right
//    model = transform(5.95, -0.75, -6.55, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //upper
//    model = transform(5.95, 2.75, -8.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.1f, .1f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //.5 down
//    model = transform(5.95, -0.5 - 0.025, -8.5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //WARDROBE front self
//    model = transform(5.95, -0.5 + 0.025, -8.5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 6.45f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //WARDROBE front self border
//    model = transform(5.95 - 0.01, -0.5 + 0.025, -7.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 6.45f, 0.02f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//}
//
//void Shokez(Cube& cube, Shader& lightingShader, glm::mat4 alTogether)
//{
//
//    glm::mat4 translateMatrix, scaleMatrix, model;
//    model = transform(6.95, -0.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 7.0f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //left
//    model = transform(5.95, -0.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //right
//    model = transform(5.95, -0.75, 2.45, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, 7.0f, 0.1f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //upper
//    model = transform(5.95, 2.75, .5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.1f, .1f, 4.0f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //3
//    model = transform(5.95, 0.75 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //.5
//    model = transform(5.95, -0.5 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //front self
//    model = transform(5.95, -0.5 + 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 2.4f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //front self border
//    model = transform(5.95 - 0.01, -0.5 + 0.025, 1.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 2.4f, 0.02f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //4
//    model = transform(5.95, 1.25 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//    //5
//    model = transform(5.95, 1.75 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.0f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//
//
//    //6 
//    model = transform(5.95, 2.25 - 0.025, .5 + .05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.8f, .1f, 3.8f);
//    cube.drawCubeWithTexture(lightingShader, alTogether * model);
//    //bokself end
//
//
//}

