#pragma once

#include "BSML/Tags/BSMLTag.hpp"
#include "UnityEngine/UI/Button.hpp"

namespace BSML {
    class ButtonTag : public BSMLTag {
        public:
            ButtonTag() : BSMLTag() {}
        
            virtual void Construct(UnityEngine::Transform* parent, Il2CppObject* host = nullptr) const override;
        protected:
            virtual void parse(const tinyxml2::XMLElement& elem) override;

            virtual UnityEngine::GameObject* CreateObject(UnityEngine::Transform* parent) const override;
            virtual UnityEngine::UI::Button* get_buttonPrefab() const;
    };
}