import ES.Window;

int main() {
    ES::Window window(800, 600, "ES sandbox: window_open");

    while (!window.should_close()) {
        window.poll_events();
    }
}