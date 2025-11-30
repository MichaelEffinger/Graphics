#include <GLFW/glfw3.h>

#include <iostream>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;


namespace ES {
    void glfw_error_callback(int error, const char* description) noexcept{
        std::cerr
        << "Owie!!!! GLFW had a boo boo! Here's what went wrong!\n"
        << "Error code: " << error << '\n'
        << "Desc: " << description << '\n';
    }
}



int main() {
    if (!glfwInit()) return -1;
    glfwSetErrorCallback(ES::glfw_error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

    while (window && not glfwWindowShouldClose(window)) {

    }
    glfwDestroyWindow(window);

    glfwTerminate();
}