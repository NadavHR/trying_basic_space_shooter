#ifndef INPUT_HPP
#define INPUT_HPP
#include <vector>
#include <functional>
#include <GLFW/glfw3.h>

class InputAction
{
private:
    static std::vector<InputAction *> boundActions;
    std::function<void()> maction;
    unsigned mkey;
    unsigned mkeyActionType;
    size_t mindex;
    void checkKeyAndPerformAction() const;
    
    
public:
    InputAction(unsigned key, unsigned keyActionType, std::function<void()> action);
    ~InputAction();
    std::function<void()> getAction() const;
    void setAction(const std::function<void()> &action);
    unsigned getKey() const;
    void setKey(unsigned key);
    unsigned getKeyActionType() const;
    void setKeyActionType(unsigned keyActionType);

    void bind();

    static void runChecksAndActions(GLFWwindow * window);
    static void clearBoundActions();
};

#endif /* INPUT_HPP */
