#pragma once

import ES.concepts;


namespace ES::meta{

    template<typename copyType>
    using const_pass_t = std::conditional_t<concepts::CheapToCopy<copyType>, const copyType, const copyType&>;

}
