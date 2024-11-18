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
#include "headers/renderer.hpp"
#include "headers/shader.hpp"
#include "headers/camera.hpp"
#include "headers/model.hpp"
#include "headers/model_render_object.hpp"
#include "headers/screen_renderer.hpp"
#include "headers/input.hpp"
#include "headers/crosshair.hpp"
#include "headers/spaceship.hpp"
#include "headers/static_utils.hpp"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void bindInputs(GLFWwindow *window);

Camera cam(glm::vec3(0.0f, 0.0f, 0.0f));
Crosshair * crosshair;
bool firstMouse = true;

Spaceship * spaceship;
float timing::deltaTime = 0;
float timing::lastFrame = 0;
Renderer* rendering::renderer = NULL;
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
    GLFWwindow* window = glfwCreateWindow(screen::SCR_WIDTH, screen::SCR_HEIGHT, "Space Shooter", NULL, NULL);
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
    glm::mat4 projection = glm::perspective(glm::radians(cam.FovY), (float)screen::SCR_WIDTH / (float)screen::SCR_HEIGHT, 0.1f, 100.0f);
    // render loop
    // -----------

    Shader screenShader("shaders/screen.vs", "shaders/screen.fs");
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
    Renderer renderer = Renderer(screen::SCR_WIDTH, screen::SCR_HEIGHT);
    rendering::renderer = &renderer;
    Crosshair cross = Crosshair(screen::SCR_WIDTH, screen::SCR_HEIGHT, cam);
    crosshair = &cross;
    spaceship = new Spaceship(*crosshair);
    ScreenRenderer screenRenderer(rendering::renderer, &screenShader);
    
    bindInputs(window);

    while (!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);  // enable depth test
        renderer.clear(); 
        float currentFrame = static_cast<float>(glfwGetTime());
        timing::deltaTime = currentFrame - timing::lastFrame;
        timing::lastFrame = currentFrame;
        
        InputAction::runChecksAndActions(window);
        spaceship->periodic(timing::deltaTime);
        renderer.render();
        crosshair->periodic();

        // render
        // ------
        screenRenderer.renderToScreen();
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

    auto shoot = new InputAction(GLFW_KEY_E, GLFW_PRESS, [&]() {spaceship->shoot(true);}, [&]() {spaceship->shoot(false);});
    shoot->bind();

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
    unsigned int ypos = static_cast<unsigned int>(screen::SCR_HEIGHT - 1 - yposIn);
    crosshair->setScreenPos(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.processMouseScroll(static_cast<float>(yoffset));
}