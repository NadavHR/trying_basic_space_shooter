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

#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "renderer.hpp"
#include "model_render_object.hpp"
#include "screen_renderer.hpp"

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float Y_MAX = 1.5;
const float X_MAX = 2.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float speed = 2.5f;
glm::vec3 position(0.0f, 0.0f, 0.0f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader modelLoadingShader("shaders/modelLoading.vs", "shaders/modelLoading.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // transformation
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // render loop
    // -----------

    Shader screenShader("shaders/screen.vs", "shaders/screen.fs");
    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    ModelRenderObject object((std::filesystem::absolute("models/backpack/backpack.obj")).generic_string(), &modelLoadingShader);
    renderer.addRenderObject(&object);
    object.setRotationRad(glm::vec3(glm::radians(0.0), 0.0f, 0.0f));
    object.setScale(glm::vec3(1.0, 1.0, 1.0));
    ScreenRenderer ScreenRenderer(&renderer, &screenShader);

    glm::vec3 lightPos(1.0, 1.0, 0.0);
    modelLoadingShader.use();
    modelLoadingShader.setProjection(projection);
    modelLoadingShader.setVec3("light.position", lightPos);
    modelLoadingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    modelLoadingShader.setVec3("light.diffuse", glm::vec3(0.9f, 0.9f, 0.9f)); // darken diffuse light a bit
    modelLoadingShader.setVec3("light.specular",glm::vec3(1.0f, 1.0f, 1.0f)); 
    modelLoadingShader.setFloat("light.constant",  1.0f);
    modelLoadingShader.setFloat("light.linear",    0.045f);
    modelLoadingShader.setFloat("light.quadratic", 0.0032f);
    

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        object.setPosition(position);
        modelLoadingShader.use();
        modelLoadingShader.setTransform("view", cam.getViewMatrix());
        modelLoadingShader.setVec3("viewPos", cam.position());
        ScreenRenderer.render();
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
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += glm::vec3(0, deltaTime*speed, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += glm::vec3(0, -deltaTime*speed, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += glm::vec3(-deltaTime*speed, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += glm::vec3(deltaTime*speed, 0, 0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.processKeyboard(DOWN, deltaTime);
    if  (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.processKeyboard(UP, deltaTime);
    
    position.x = min(max(position.x, -X_MAX), X_MAX);
    position.y = min(max(position.y, -Y_MAX), Y_MAX);

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

    cam.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.processMouseScroll(static_cast<float>(yoffset));
}