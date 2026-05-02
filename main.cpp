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

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 45.0;
float rotateAngle_Y = 45.0;
float rotateAngle_Z = 45.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 0.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rafay 1013", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();

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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);


        /*
        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

        */

        
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

        // table base
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.5f, 2.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2798394136874722f, 0.0f, 0.0f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.2f, 0.2f, 2.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.22983941368747218f, 0.39999999999999997f, -0.050000000000000044f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.1f, 2.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2798394136874722f, 0.35f, 0.0f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.2f, 0.2f, 2.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.22983941368747218f, 0.25f, -0.050000000000000044f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.1f, 2.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2798394136874722f, 0.5f, 0.0f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.2f, 0.2f, 2.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.22983941368747218f, 0.55f, -0.050000000000000044f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.1f, 2.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2798394136874722f, 0.65f, 0.0f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // table base_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.2f, 0.2f, 2.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.22983941368747218f, 0.7000000000000001f, -0.050000000000000044f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair_base
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.702f, 0.3294450662103783f, 0.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair bottom x
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.1f, 0.7f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.652f, 0.0f, 0.325f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair bottom y
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.8f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.627f, 0.0f, 0.3f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair bottom xy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.1f, 0.7f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.627f, 0.013f, 0.325f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair bottom pillar
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.515f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.577f, 0.0f, 0.375f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair_base_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.1f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.702f, 0.329f, 0.22499999999999998f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair back rest
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 1.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.722f, 0.329f, 0.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair back rest_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.9f, 0.9f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.741f, 0.354f, 0.275f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair hand rest
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.3f, 0.15f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.702f, 0.429f, 0.225f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair hand rest
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.3f, 0.15f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.702f, 0.429f, 0.7f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair hand rest_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.2f, 0.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.652f, 0.494f, 0.688f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair hand rest_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.2f, 0.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.652f, 0.494f, 0.213f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room floor
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(15.0f, 0.2f, 15.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.4701605863125278f, -0.1f, -3.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair head rest
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.4f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.722f, 0.829f, 0.375f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair head rest_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.3f, 0.6f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.722f, 0.854f, 0.35f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // chair head rest_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 0.3f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.747f, 0.854f, 0.37499999999999994f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall left
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(15.0f, 5.5f, 0.2f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.47f, -0.1f, -3.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.5f, 4.418f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, -0.1f, -3.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 5.5f, 4.418f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, -0.1f, 2.041f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.5f, 7.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, -0.1f, -1.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 7.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 2.1500761746371024f, -1.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 1.9070512311216876f, -1.0463522154689289f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 1.7591894061370348f, -1.164902882416939f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 1.9000761746371024f, 1.7958778725414823f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 2.0144925723807567f, -0.9226597896603219f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 2.0123501288866814f, 1.667000511777092f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 1.7673804101979327f, 1.9244862957544835f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.6439858102134575f, -1.0463522154689289f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.7583202593181471f, -1.1832593905849045f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.5105074691629226f, -0.9336961201688809f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.6486342422907343f, 1.7958205941073389f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.765162494796233f, 1.9244862957544835f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.93f, 0.5277472979892717f, 1.667000511777092f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 1.1f, 15.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.629999999999999f, -0.1f, -3.25f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // room wall right_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.16159609344292794f, 0.728956168443726f, 15.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-56.448109209000535f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.62978740621484f, 0.448329602501177f, -3.25f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.8f, 0.8f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-11.435f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.557f, 0.407f, 0.3f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.7f, 0.7f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-11.435f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.539568570428004f, 0.43570543494587805f, 0.325f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.7f, 0.9f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-11.435f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.557f, 0.432f, 0.275f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6834587217934712f, 0.2f, 0.8f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.345082383130537f, 0.30699999999999994f, 0.3f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.0f, 0.6f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.461f, -0.091f, -0.038615081362191495f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.569f, 0.6f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-43.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.461f, 0.407f, 0.737f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.569f, 0.6f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.628348850124319f, 0.33351634715113454f, 0.737f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.0f, 0.6f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.461f, -0.091f, 0.737f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.569f, 0.6f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-43.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.461f, 0.407f, -0.038615081362191495f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.569f, 0.6f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.633612354415408f, 0.3289524191179264f, -0.038615081362191495f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.4f, 0.5f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-43.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.487174962512758f, 0.4552336691534423f, 0.762f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy_copy_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.4f, 0.5f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-43.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.4711639860997066f, 0.46613320981589285f, -0.0136150813621915f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // computer body_copy_copy
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.2f, 0.6f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.361405362939237f, 0.33132125692947795f, 0.36532943244181076f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // showcase back
scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 4.0f, 0.2f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.03f, 0.0f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase left
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 4.0f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.03f, 0.0f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase right
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 4.0f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.43f, 0.0f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase top
scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.03f, 2.0f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase top_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0300000000000002f, 0.95f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase top_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0300000000000002f, 0.0f, -3.13f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.53f, 1.05f, -3.005f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
rotateZMatrix = glm::rotate(identityMatrix, glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.7470455338053281f, 1.113445734790694f, -2.805f));
model = translateMatrix * rotateZMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-26.578132929609158f), glm::vec3(0.0f, 0.0f, 1.0f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.7102348787793042f, 1.2816583884828325f, -2.8049999999999997f));
model = translateMatrix * rotateZMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 1.0f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5300000000000002f, 0.1f, -3.005f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.73f, 0.2f, -2.805f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.9300000000000002f, 0.2f, -2.8049999999999997f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// showcase trophy_copy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5300000000000002f, 0.2f, -2.8049999999999997f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box
scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 1.5f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.878f, 0.0f, -3.25f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.8888773926324796f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.8566626543683533f, 0.03668719693759481f, -2.682095696907665f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.8888773926324796f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(3.707939591054692f, 0.03668719693759481f, -2.682095696907665f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.8888773926324796f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.8566626543683533f, 0.03668719693759481f, -3.2856119410810845f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.8888773926324796f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(3.707456779723513f, 0.03668719693759481f, -3.2902427755190873f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.1f, 0.4f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.857f, 0.815f, -2.682f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.1f, 0.4f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.857f, 0.815f, -3.2960871550694404f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.6f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.857f, 0.815f, -3.282f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.6f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(3.707f, 0.815f, -3.282f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.1f, 0.4f, 0.4f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.857f, 0.003103673043114874f, -2.682f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.6f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(3.707f, 0.003103673043114874f, -3.282f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

// storage box_copy_copy_copy_copy_copy_copy
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 0.4f, 1.6f));
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.857f, 0.003103673043114874f, -3.282f));
model = translateMatrix * scaleMatrix;
ourShader.setMat4("model", model);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);





        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 1;
        else if (rotateAxis_Y) rotateAngle_Y -= 1;
        else rotateAngle_Z -= 1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        lookAtX += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        lookAtX -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
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
