module;

#include <utility>

export module ES.Size;

import ES.ContainerN;

export namespace ES{

    struct Size: public ContainerN<Size,int,2> {    
        using ContainerN<Size,int,2>::ContainerN;

        [[nodiscard]] constexpr auto&& width(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[0]);
        }

        [[nodiscard]] constexpr auto&& height(this auto&& self) noexcept {
            return std::forward_like<decltype(self)>(self[1]);
        }

        [[nodiscard]] constexpr double aspect_ratio() const noexcept {
            if (height() == 0) {
                return 0.0;
            }
            return static_cast<double>(width()) / static_cast<double>(height());
        }

    };

}