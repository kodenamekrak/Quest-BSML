#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"

namespace BSML {
    class MainMenuRegistration;
    namespace Register {
        void AddMainMenuRegistration(MainMenuRegistration* reg);
    }

    class MainMenuRegistration {
        public:
            enum class RegistrationType {
                FlowCoordinator,
                ViewController,
                Method
            };

            MainMenuRegistration(const std::string_view& title, const std::string_view& buttonText, const std::string_view& hoverHint, const System::Type* csType, const RegistrationType registrationType);
            MainMenuRegistration(const std::string_view& title, const std::string_view& buttonText, const std::string_view& hoverHint, const std::function<void(HMUI::ViewController*, bool, bool, bool)> setupFunc);
            ~MainMenuRegistration();

            void Present();

            const std::string title;
            const std::string buttonText;
            const std::string hoverHint;
            union {
                HMUI::ViewController* viewController;
                HMUI::FlowCoordinator* flowCoordinator;
            };
            const System::Type* csType;
            const std::function<void(HMUI::ViewController*, bool, bool, bool)> setupFunc;
            const RegistrationType registrationType;

            static MainMenuRegistration* get_registration(const std::string_view& buttonText);
        private:
            friend class MainMenuHolderFlowCoordinator;
            friend void ::BSML::Register::AddMainMenuRegistration(MainMenuRegistration* reg);

            void OnGameDidRestart();
            void PresentWithFlowCoordinator(HMUI::FlowCoordinator* presentOn);
            void PresentWithViewController(HMUI::FlowCoordinator* presentOn);
            void PresentWithMethod(HMUI::FlowCoordinator* presentOn);

            static std::vector<MainMenuRegistration*> registrations;
    };
}

DECLARE_CLASS_CODEGEN(BSML, MainMenuHolderFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, placeholder);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::BackButtonWasPressed>::get(), HMUI::ViewController* topViewController);
    private:
        custom_types::Helpers::Coroutine EndOfFramePresentVC();
        static SafePtrUnity<MainMenuHolderFlowCoordinator> instance;
        static MainMenuHolderFlowCoordinator* get_instance();

        friend class MainMenuRegistration;
        MainMenuRegistration* currentRegistration;
)