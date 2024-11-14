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
    std::function<void()> mactionOnFalse = []() {};
    unsigned mkey;
    unsigned mkeyActionType;
    size_t mindex;
    void checkKeyAndPerformAction() const;
    
    
public:
    InputAction(unsigned key, unsigned keyActionType, std::function<void()> action);
    InputAction(unsigned key, unsigned keyActionType, std::function<void()> action, std::function<void()> onFalse);
    ~InputAction();
    std::function<void()> getAction() const;
    void setAction(const std::function<void()> &action);
    std::function<void()> getOnFalseAction() const;
    void setOnFalseAction(const std::function<void()> &action);
    unsigned getKey() const;
    void setKey(unsigned key);
    unsigned getKeyActionType() const;
    void setKeyActionType(unsigned keyActionType);

    void bind();
    void unbind();

    static void runChecksAndActions(GLFWwindow * window);
    static void deleteAllBoundActions();
    static InputAction* getBoundAction(size_t i);
    static size_t boundActionCount();

};

#endif /* INPUT_HPP */
