#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <istream>
#include <string>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <filesystem>

#include "headers/shader.hpp"
#include "headers/camera.hpp"
#include "headers/model.hpp"
#include "headers/renderer.hpp"
#include "headers/model_render_object.hpp"
#include "headers/screen_renderer.hpp"
#include "headers/input.hpp"
#include "headers/crosshair.hpp"
#include "headers/spaceship.hpp"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float Y_MAX = 1.5;
const float X_MAX = 2.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void bindInputs(GLFWwindow *window);

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
Crosshair crosshair(SCR_WIDTH, SCR_HEIGHT, cam);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Spaceship * spaceship;

int main()
{
    // glfw: initialize and configure
    // ------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Space Shooter", NULL, NULL);
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


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_STENCIL_TEST);    
    glEnable(GL_CULL_FACE);  

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // stbi_set_flip_vertically_on_load(true);

    // transformation
    glm::mat4 projection = glm::perspective(glm::radians(cam.FovY), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // render loop
    // -----------
    spaceship = new Spaceship(crosshair);

    Shader screenShader("shaders/screen.vs", "shaders/screen.fs");
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    ModelRenderObject& object = spaceship->getRenderObject();
    renderer.addRenderObject(&object);
    object.setRotationRad(glm::vec3(glm::radians(0.0), 0.0f, 0.0f));
    object.setScale(glm::vec3(1.0, 1.0, 1.0));
    ScreenRenderer screenRenderer(&renderer, &screenShader);
    
    bindInputs(window);

    Shader crosshairShader("shaders/crosshair.vs", "shaders/crosshair.fs");
    crosshairShader.use();
    crosshairShader.setInt("screenTexture", 0);
    ScreenRenderer crosshairScreenRenderer(&renderer, &crosshairShader);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        InputAction::runChecksAndActions(window);
        spaceship->periodic(deltaTime);

        // render
        // ------
        screenRenderer.renderAll();
        crosshairShader.use();
        crosshairShader.setVec2("position", crosshair.getNormalizedScreenCoords());
        crosshairScreenRenderer.renderToScreen();
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    InputAction::deleteAllBoundActions();
    delete spaceship;
    return 0;
}

// bind all inputs
// ---------------------------------------------------------------------------------------------------------
void bindInputs(GLFWwindow *window) {
    auto closeWindow = new InputAction(GLFW_KEY_ESCAPE, GLFW_PRESS, [&]() { glfwSetWindowShouldClose(window, true);});
    closeWindow->bind();

    auto up = new InputAction(GLFW_KEY_W, GLFW_PRESS, [&](){ spaceship->inputY(1.0);});
    up->bind();

    auto down = new InputAction(GLFW_KEY_S, GLFW_PRESS, [&](){ spaceship->inputY(-1.0);});
    down->bind();

    auto left = new InputAction(GLFW_KEY_A, GLFW_PRESS, [&]() { spaceship->inputX(-1.0);});
    left->bind();

    auto right = new InputAction(GLFW_KEY_D, GLFW_PRESS, [&]() { spaceship->inputX(1.0);});
    right->bind();


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
    unsigned int xpos = static_cast<unsigned int>(xposIn);
    unsigned int ypos = static_cast<unsigned int>(SCR_HEIGHT - yposIn);
    crosshair.setScreenPos(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.processMouseScroll(static_cast<float>(yoffset));
}