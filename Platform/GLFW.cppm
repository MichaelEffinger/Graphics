module;

#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>

export module ES.GLFW;

import ES.VectorN;
import ES.PointN;
import ES.Size;
import ES.WindowMode;

export namespace ES{
    using GLFWwindow = ::GLFWwindow;
    using FramebufferSizeCallback = void(*)(GLFWwindow*, int width, int height);
    using WindowSizeCallback      = void(*)(GLFWwindow*, int width, int height);
    using WindowCloseCallback     = void(*)(GLFWwindow*);
    using WindowFocusCallback     = void(*)(GLFWwindow*, int focused);
    using KeyCallback             = void(*)(GLFWwindow*, int key, int scancode, int action, int mods);
    using CharCallback             = void(*)(GLFWwindow*, unsigned int codepoint);
    using CursorPosCallback       = void(*)(GLFWwindow*, double x, double y);
    using MouseButtonCallback     = void(*)(GLFWwindow*, int button, int action, int mods);
    using ScrollCallback          = void(*)(GLFWwindow*, double x_offset, double y_offset);
    using JoystickCallback        = void(*)(int joystick, int event);

    class PlatformError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class PlatformInitError : public PlatformError {
    public:
        using PlatformError::PlatformError;
    };

    class WindowCreationError : public PlatformError {
    public:
        using PlatformError::PlatformError;
    };


}

namespace ES{

    inline std::string to_message(const char* description) {
        return description ? description : "unknown GLFW error";
    }

    [[noreturn]] inline void throw_translated(int code, const std::string& msg) {
        switch (code) {
            case GLFW_NOT_INITIALIZED:
            case GLFW_INVALID_ENUM:
            case GLFW_INVALID_VALUE:
            case GLFW_NO_CURRENT_CONTEXT:
            case GLFW_NO_WINDOW_CONTEXT:
                throw std::logic_error(msg);
            default:
                throw PlatformError(msg);
        }
    }

    inline void check() {
        const char* description = nullptr;
        int code = glfwGetError(&description);
        if (code == GLFW_NO_ERROR ||
            code == GLFW_FEATURE_UNAVAILABLE ||
            code == GLFW_FEATURE_UNIMPLEMENTED) return;
        throw_translated(code, to_message(description));
    }

    inline bool unsupported() {
        const char* description = nullptr;
        int code = glfwGetError(&description);
        if (code == GLFW_NO_ERROR) return false;
        if (code == GLFW_FEATURE_UNAVAILABLE || code == GLFW_FEATURE_UNIMPLEMENTED) return true;
        throw_translated(code, to_message(description));
    }

    struct GLFWState {
        GLFWState() {
            if (!glfwInit()) {
                const char* description = nullptr;
                glfwGetError(&description);
                throw PlatformInitError(to_message(description));
            }
        }

        ~GLFWState() {
            glfwTerminate();
        }
    };
}

export namespace ES {
    class GLFW {
        inline static std::weak_ptr<GLFWState> runtime_;
        std::shared_ptr<GLFWState> state_;

    public:

        GLFW(): state_(runtime_.lock()) {
            if (!state_) {
                state_ = std::make_shared<GLFWState>();
                runtime_ = state_;
            }
        }

        GLFWwindow* make_window(int width, int height, const char* title, WindowMode mode) const {
            GLFWmonitor* monitor = nullptr;

            if (mode == WindowMode::fullscreen) {
                monitor = glfwGetPrimaryMonitor();
            }

            glfwGetError(nullptr);
            GLFWwindow* w = glfwCreateWindow(width, height, title, monitor, nullptr);

            if (!w) {
                const char* description = nullptr;
                glfwGetError(&description);
                throw WindowCreationError(to_message(description));
            }
            return w;
        }

        void destroy_window(GLFWwindow* window) const{
            glfwDestroyWindow(window);
        }

        void set_window_title(GLFWwindow* window, const char* title)const{
            glfwSetWindowTitle(window, title);
        }

        std::string get_window_title(GLFWwindow* window) const {
            return glfwGetWindowTitle(window);
        }

        void set_window_size(GLFWwindow* window, int width, int height) const{
            glfwSetWindowSize(window, width, height);
        }

        Size get_window_size(GLFWwindow* window) const{
            Size result;
            glfwGetWindowSize(window, &result.width(), &result.height());
            return result;
        }

        void set_windowed(GLFWwindow* window, int xpos, int ypos, int width, int height) const{
            glfwSetWindowMonitor(window,nullptr, xpos, ypos, width, height, GLFW_DONT_CARE);
        }

        void set_window_fullscreen( GLFWwindow* window, int width, int height, int refresh_rate = GLFW_DONT_CARE) const{
            glfwSetWindowMonitor(window,glfwGetPrimaryMonitor(), 0, 0, width, height, refresh_rate);
        }

        bool should_close(GLFWwindow* window) const{
            return glfwWindowShouldClose(window);
        }

        void set_should_close(GLFWwindow* window, bool value) const {
            glfwSetWindowShouldClose(window, value);
        }

        std::optional<PointN<int, 2>> get_window_position(GLFWwindow* window) const {
            glfwGetError(nullptr);
            PointN<int, 2> result;
            glfwGetWindowPos(window, &result.x(), &result.y());
            if (unsupported()) return std::nullopt;
            return result;
        }

        void set_window_position(GLFWwindow* window, PointN<int, 2> position) const {
            glfwSetWindowPos(window, position.x(), position.y());
        }

        void set_window_position(GLFWwindow* window, int x, int y) const {
            glfwSetWindowPos(window, x, y);
        }

        void set_window_x(GLFWwindow* window, int x) const {
            if (auto p = get_window_position(window)) glfwSetWindowPos(window, x, p->y());
        }

        void set_window_y(GLFWwindow* window, int y) const {
            if (auto p = get_window_position(window)) glfwSetWindowPos(window, p->x(), y);
        }

        void set_window_size_limits(GLFWwindow* window, int min_width,int min_height, int max_width, int max_height) const {
            glfwSetWindowSizeLimits(window, min_width, min_height, max_width, max_height);
        }

        void iconify_window(GLFWwindow* window)const{
            glfwIconifyWindow(window);
        }

        void maximize_window(GLFWwindow* window)const{
            glfwMaximizeWindow(window);
        }

        void restore_window(GLFWwindow* window) const{
            glfwRestoreWindow(window);
        }

        void show(GLFWwindow* window) const {
            glfwShowWindow(window);
        }

        void hide(GLFWwindow* window) const {
            glfwHideWindow(window);
        }

        void focus(GLFWwindow* window) const {
            glfwFocusWindow(window);
        }

        bool is_focused(GLFWwindow* window) const {
            return glfwGetWindowAttrib(window, GLFW_FOCUSED);
        }

        bool is_minimized(GLFWwindow* window) const {
            return glfwGetWindowAttrib(window, GLFW_ICONIFIED);
        }

        void reset_window_hints() const {
            glfwDefaultWindowHints();
        }

        Size get_framebuffer_size(GLFWwindow* window)const{
            Size result;
            glfwGetFramebufferSize(window, &result.width(), &result.height());
            return result;
        }

        Vector2<float> get_content_scale(GLFWwindow* window) const{
            Vector2<float> result;
            glfwGetWindowContentScale(window, &result.x(), &result.y());
            return result;
        }

        void poll_events() const {
            glfwPollEvents();
        }

        void wait_events() const {
            glfwWaitEvents();
        }

        void wait_events(double timeout_seconds) const {
            glfwWaitEventsTimeout(timeout_seconds);
        }

        void post_empty_event() const {
            glfwPostEmptyEvent();
        }

        double get_time() const {
            return glfwGetTime();
        }

        void set_time(double seconds) const {
            glfwSetTime(seconds);
        }

        void set_window_opacity(GLFWwindow* window, float opacity) const {
            glfwSetWindowOpacity(window, opacity);
        }

        float get_window_opacity(GLFWwindow* window) const {
            return glfwGetWindowOpacity(window);
        }

        PointN<double, 2> get_cursor_pos(GLFWwindow* window) const {
            PointN<double, 2> result;
            glfwGetCursorPos(window, &result.x(), &result.y());
            return result;
        }

        void set_cursor_pos(GLFWwindow* window, double x, double y) const {
            glfwSetCursorPos(window, x, y);
        }

        void set_cursor_pos(GLFWwindow* window, PointN<double, 2> position) const {
            glfwSetCursorPos(window, position.x(), position.y());
        }

        void set_raw_mouse_motion(GLFWwindow* window, bool enabled) const {
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, enabled);
        }

        bool raw_mouse_motion_supported() const {
            return glfwRawMouseMotionSupported();
        }

        void set_framebuffer_size_callback(GLFWwindow* window, FramebufferSizeCallback cb) const {
            glfwSetFramebufferSizeCallback(window, cb);
        }

        void set_window_size_callback(GLFWwindow* window, WindowSizeCallback cb) const {
            glfwSetWindowSizeCallback(window, cb);
        }

        void set_window_close_callback(GLFWwindow* window, WindowCloseCallback cb) const {
            glfwSetWindowCloseCallback(window, cb);
        }

        void set_window_focus_callback(GLFWwindow* window, WindowFocusCallback cb) const {
            glfwSetWindowFocusCallback(window, cb);
        }

        void set_key_callback(GLFWwindow* window, KeyCallback cb) const {
            glfwSetKeyCallback(window, cb);
        }

        void set_char_callback(GLFWwindow* window, CharCallback cb) const {
            glfwSetCharCallback(window, cb);
        }

        void set_cursor_pos_callback(GLFWwindow* window, CursorPosCallback cb) const {
            glfwSetCursorPosCallback(window, cb);
        }

        void set_mouse_button_callback(GLFWwindow* window, MouseButtonCallback cb) const {
            glfwSetMouseButtonCallback(window, cb);
        }

        void set_scroll_callback(GLFWwindow* window, ScrollCallback cb) const {
            glfwSetScrollCallback(window, cb);
        }

        void set_joystick_callback(JoystickCallback callback) const {
            glfwSetJoystickCallback(callback);
        }

        int get_key(GLFWwindow* window, int key) const {
            return glfwGetKey(window, key);
        }

        int get_mouse_button(GLFWwindow* window, int button) const {
            return glfwGetMouseButton(window, button);
        }

        void set_cursor_mode(GLFWwindow* window, int mode) const {
            glfwSetInputMode(window, GLFW_CURSOR, mode);
        }

        void set_window_user_pointer(GLFWwindow* window, void* ptr) const {
            glfwSetWindowUserPointer(window, ptr);
        }

        static void* get_window_user_pointer(GLFWwindow* window) {
            return glfwGetWindowUserPointer(window);
        }

        void set_clipboard(GLFWwindow* window, const std::string& text) const {
            glfwSetClipboardString(window, text.data());
        }

        std::string get_clipboard(GLFWwindow* window) const {
            const char* text = glfwGetClipboardString(window);

            if(!text) {
                return {};
            }

            return text;
        }


        bool is_gamepad(int joystick) const {
            return glfwJoystickIsGamepad(joystick);
        }

        std::string get_gamepad_name(int joystick) const {
            const char* name = glfwGetGamepadName(joystick);

            if(!name) {
                return {};
            }

            return name;
        }

        bool get_gamepad_button(int joystick, int button) const {
            GLFWgamepadstate state;

            if(!glfwGetGamepadState(joystick, &state)) {
                return false;
            }

            return state.buttons[button] == GLFW_PRESS;
        }

        float get_gamepad_axis(int joystick, int axis) const {
            GLFWgamepadstate state;

            if(!glfwGetGamepadState(joystick, &state)) {
                return 0.0f;
            }

            return state.axes[axis];
        }

        bool is_joystick_present(int joystick) const {
            return glfwJoystickPresent(joystick);
        }

        std::string get_joystick_name(int joystick) const {
            const char* name = glfwGetJoystickName(joystick);

            if(!name) {
                return {};
            }

            return name;
        }

        std::string get_joystick_guid(int joystick) const {
            const char* guid = glfwGetJoystickGUID(joystick);

            if(!guid) {
                return {};
            }

            return guid;
        }

        std::vector<float> get_joystick_axes(int joystick) const {
            int count = 0;
            const float* axes = glfwGetJoystickAxes(joystick, &count);

            if(!axes) {
                return {};
            }

            return {axes, axes + count};
        }

        std::vector<unsigned char> get_joystick_buttons(int joystick) const {
            int count = 0;
            const unsigned char* buttons = glfwGetJoystickButtons(joystick, &count);

            if(!buttons) {
                return {};
            }

            return {buttons, buttons + count};
        }

        std::vector<unsigned char> get_joystick_hats(int joystick) const {
            int count = 0;
            const unsigned char* hats = glfwGetJoystickHats(joystick, &count);

            if(!hats) {
                return {};
            }

            return {hats, hats + count};
        }

// Vulkan
/*
bool vulkan_supported() const;
std::vector<const char*> get_required_instance_extensions() const;
VkResult create_window_surface(VkInstance instance, GLFWwindow* window, VkSurfaceKHR& surface, const VkAllocationCallbacks* allocator = nullptr) const;
*/
    };

}