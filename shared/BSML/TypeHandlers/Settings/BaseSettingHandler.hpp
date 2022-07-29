#pragma once

#pragma once

#include "BSML/TypeHandlers/TypeHandler.hpp"
#include "BSML/Components/Settings/BaseSetting.hpp"

// note: this is analogous to GenericSettingHandler, but due to this being quest,
// there are some issues with the way we have to do things.

namespace BSML {
    class BaseSettingHandler : public TypeHandler<BSML::BaseSetting*> {
        public:
            using Base = TypeHandler<BSML::BaseSetting*>;
            BaseSettingHandler() : Base() {}

            virtual Base::PropMap get_props() const override;
            virtual Base::SetterMap get_setters() const override;

            virtual void HandleType(const ComponentTypeWithData& componentType, BSMLParserParams& parserParams) override;
    };
}