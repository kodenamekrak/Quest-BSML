#include "BSML.hpp"
#include "logging.hpp"
#include "BSML/MenuButtons/MenuButtons.hpp"
#include "BSML/Settings/BSMLSettings.hpp"
#include "BSML/GameplaySetup/GameplaySetup.hpp"
#include "BSML/FlowCoordinators/MainMenuHolderFlowCoordinator.hpp"

extern "C" void load();

namespace BSML {
    void Init() {
        load();
    }

    std::shared_ptr<BSMLParser> parse(std::string_view str) {
        Init();
        return BSMLParser::parse(str);
    }

    std::shared_ptr<BSMLParser> parse_and_construct(std::string_view str, UnityEngine::Transform* parent, Il2CppObject* host) {
        Init();
        return BSMLParser::parse_and_construct(str, parent, host);
    }

    namespace Register {
        MenuButton* RegisterMenuButton(std::string_view text, std::string_view hoverHint, std::function<void(void)> onClick) {
            Init();
            auto btn = MenuButton::Make_new(text, hoverHint, onClick);
            if (RegisterMenuButton(btn)) return btn;
            btn->Finalize();
            return nullptr;
        }

        bool RegisterMenuButton(MenuButton* button) {
            Init();
            return MenuButtons::get_instance()->Registerbutton(button);
        }

        bool UnRegisterMenuButton(MenuButton* button) {
            Init();
            return MenuButtons::get_instance()->UnRegisterbutton(button);
        }

        bool RegisterSettingsMenu(std::string_view name, std::string_view content_key, Il2CppObject* host, bool enableExtraButtons) {
            Init();
            return BSMLSettings::get_instance()->AddSettingsMenu(name, content_key, host, enableExtraButtons);
        }

        bool UnRegisterSettingsMenu(Il2CppObject* host) {
            Init();
            return BSMLSettings::get_instance()->RemoveSettingsMenu(host);
        }

        bool RegisterGameplaySetupTab(std::string_view name, std::string_view content_key, Il2CppObject* host, MenuType menuType) {
            Init();
            return BSML::GameplaySetup::get_instance()->AddTab(name, content_key, host, menuType);
        }

        bool UnRegisterGameplaySetupTab(std::string_view name) {
            Init();
            return BSML::GameplaySetup::get_instance()->RemoveTab(name);
        }

        void RegisterMainMenuFlowCoordinator(const std::string_view& buttonText, const std::string_view& hoverhint, System::Type* flowCoordinatorType) {
            AddMainMenuRegistration(new MainMenuRegistration("", buttonText, hoverhint, flowCoordinatorType, MainMenuRegistration::RegistrationType::FlowCoordinator));
        }

        void RegisterMainMenuViewController(const std::string_view& title, const std::string_view& buttonText, const std::string_view& hoverhint, System::Type* viewControllerType) {
            AddMainMenuRegistration(new MainMenuRegistration(title, buttonText, hoverhint, viewControllerType, MainMenuRegistration::RegistrationType::ViewController));
        }

        void RegisterMainMenuViewControllerMethod(const std::string_view& title, const std::string_view& buttonText, const std::string_view& hoverhint, std::function<void(HMUI::ViewController*, bool, bool, bool)> viewControllerDidActivate) {
            AddMainMenuRegistration(new MainMenuRegistration(title, buttonText, hoverhint, viewControllerDidActivate));
        }

        void AddMainMenuRegistration(MainMenuRegistration* reg) {
            DEBUG("Registering {}", reg->buttonText);

            MainMenuRegistration::registrations.emplace_back(reg);
            RegisterMenuButton(reg->buttonText, reg->hoverHint, std::bind(&MainMenuRegistration::Present, reg));
        }
    }

    namespace Events {
        UnorderedEventCallback<> onGameDidRestart;
    }
}