module;

#include <string>
#include <stdexcept>

export module ES.Window;

import ES.PointN;
import ES.Size;
import ES.WindowMode;
import ES.GLFW;


export namespace ES{

    class Window{

        GLFW glfw;
        GLFWwindow* handle = nullptr;

        std::string title;

        Size window_size;
        PointN<int,2> position;
        Size frame_buffer_size;
        bool focused = true;

        static Window* self(GLFWwindow* w) {
            return static_cast<Window*>(GLFW::get_window_user_pointer(w));
        }

        void install_callbacks() {
            glfw.set_window_user_pointer(handle, this);

            glfw.set_window_size_callback(handle, [](GLFWwindow* w, int width, int height) {
                if (auto* win = self(w)){
                    win->on_resize(width, height);
                }
            });

            glfw.set_framebuffer_size_callback(handle, [](GLFWwindow* w, int width, int height) {
                if (auto* win = self(w)){
                    win->on_framebuffer_resize(width, height);
                }
            });

            glfw.set_window_focus_callback(handle, [](GLFWwindow* w, int f) {
                if (auto* win = self(w)){
                    win->focused = f;
                }
            });
        }

        void on_resize(int w, int h) {
            window_size.width() = w;
            window_size.height() = h;
        }

        void on_framebuffer_resize(int w, int h) {
            frame_buffer_size.width() = w;
            frame_buffer_size.height() = h;
        }


        public:


        Window(int w, int h, const char* t, WindowMode mode = WindowMode::windowed): glfw(), title(t), window_size(w,h) {
            handle = glfw.make_window(w, h, t, mode);
            if (!handle) throw std::runtime_error("Failed to create window");

            position = glfw.get_window_position(handle);

            frame_buffer_size = glfw.get_framebuffer_size(handle);

            install_callbacks();
        }

        ~Window() {
            if (handle){
                glfw.destroy_window(handle);
            }
        }

        bool should_close() const { return glfw.should_close(handle); }
        void poll_events() const { glfw.poll_events(); }



        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;




    };
}