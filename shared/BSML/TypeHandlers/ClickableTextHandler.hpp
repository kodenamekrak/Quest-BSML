#pragma once

#include "BSML/TypeHandlers/TypeHandler.hpp"
#include "BSML/Components/ClickableText.hpp"

namespace BSML {
    class ClickableTextHandler : public TypeHandler<BSML::ClickableText*> {
        public:
            using Base = TypeHandler<BSML::ClickableText*>;
            ClickableTextHandler() : Base() {}

            virtual Base::PropMap get_props() const override;
            virtual Base::SetterMap get_setters() const override;
            
            virtual void HandleType(const ComponentTypeWithData& componentType, BSMLParserParams& parserParams) override;
    };
}