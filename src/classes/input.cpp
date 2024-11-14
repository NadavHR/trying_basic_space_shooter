#include "../headers/input.hpp"
#define UNBOUND_INDEX -1 

std::vector<InputAction *> InputAction::boundActions = std::vector<InputAction *>();

InputAction::InputAction(unsigned key, unsigned keyActionType, std::function<void()> action)
{
    mkey = key;
    mkeyActionType = keyActionType;
    maction = action;
    mindex = UNBOUND_INDEX; 
}

InputAction::InputAction(unsigned key, unsigned keyActionType, std::function<void()> action, std::function<void()> onFalse)
{
    mkey = key;
    mkeyActionType = keyActionType;
    maction = action;
    mactionOnFalse = onFalse;
    mindex = UNBOUND_INDEX; 
}


void InputAction::bind() {
    mindex = boundActions.size();
    boundActions.push_back(this);
}

void InputAction::unbind() {
    if (this->mindex != UNBOUND_INDEX) {
        for (int i = mindex + 1; i < boundActions.size(); i++){
            boundActions[i]->mindex -= 1;
        }
        boundActions.erase(boundActions.begin() + this->mindex);
        this->mindex = UNBOUND_INDEX;
    }
}

InputAction::~InputAction()
{
    unbind();
}

std::function<void()> InputAction::getAction() const {
    return maction;
}

void InputAction::setAction(const std::function<void()> &action) {
    maction = action;
}

std::function<void()> InputAction::getOnFalseAction() const {
    return mactionOnFalse;
}

void InputAction::setOnFalseAction(const std::function<void()> &action) {
    mactionOnFalse = action;
}

unsigned InputAction::getKey() const {
    return mkey;
}

void InputAction::setKey(unsigned key) {
    mkey = key;
}

unsigned InputAction::getKeyActionType() const {
    return mkeyActionType;
}

void InputAction::setKeyActionType(unsigned keyActionType) {
    mkeyActionType = keyActionType;
}

void InputAction::runChecksAndActions(GLFWwindow * window) {
    for (InputAction * inputAction : boundActions) {
        if (glfwGetKey(window, inputAction->mkey) == inputAction->mkeyActionType) {
            inputAction->maction();
        }
        else {
            inputAction->mactionOnFalse();
        }
    }
}

void InputAction::deleteAllBoundActions() {
    while (boundActions.size() != 0) {
        delete boundActions[0];
    }
}

InputAction* InputAction::getBoundAction(size_t i) {
    return boundActions[i];
}

size_t InputAction::boundActionCount() {
    return boundActions.size();
}