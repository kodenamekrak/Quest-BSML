#include "BSML/Tags/Settings/DropdownListSettingTag.hpp"
#include "BSML/Components/ExternalComponents.hpp"
#include "logging.hpp"
#include "Helpers/getters.hpp"

#include "BSML/Components/Settings/DropdownListSetting.hpp"

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "HMUI/SimpleTextDropdown.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "VRUIControls/VRGraphicRaycaster.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace BSML {
    static BSMLTagParser<DropdownListSettingTag> dropdownListSettingTagParser({"dropdown-list-setting"});
    GameObject* dropdownTemplate = nullptr;
    GameObject* safePrefab = nullptr;

    void DropdownListSettingTag::Construct(UnityEngine::Transform* parent, Il2CppObject* host) const {
        auto go = CreateObject(parent);
        auto externalComponents = go->GetComponent<ExternalComponents*>();
        auto dropdownListSetting = externalComponents->Get<BSML::DropdownListSetting*>();
        SetHostField(host, dropdownListSetting);

        CreateChildren(go->get_transform(), host);
    }

    UnityEngine::GameObject* DropdownListSettingTag::CreateObject(UnityEngine::Transform* parent) const {
        DEBUG("Creating DropdownListSetting");

        if (!dropdownTemplate || !Object::IsNativeObjectAlive(dropdownTemplate) || !safePrefab || !Object::IsNativeObjectAlive(safePrefab)) {
            dropdownTemplate = Resources::FindObjectsOfTypeAll<HMUI::SimpleTextDropdown*>().FirstOrDefault([](auto x){ 
                auto parent = x->get_transform()->get_parent();
                if (!parent) return false;
                return parent->get_name() == "NormalLevels";
            })->get_transform()->get_parent()->get_gameObject();

            safePrefab = Object::Instantiate(dropdownTemplate);
            Object::DontDestroyOnLoad(safePrefab);
            safePrefab->SetActive(false);
            safePrefab->set_name("BSMLDropdownListPrefab");
        }

        auto gameObject = Object::Instantiate(safePrefab, parent, false);
        auto transform = reinterpret_cast<RectTransform*>(gameObject->get_transform());
        auto externalComponents = gameObject->AddComponent<ExternalComponents*>();
        externalComponents->Add(transform);

        gameObject->set_name("BSMLDropdownList");
        auto dropdown = gameObject->GetComponentInChildren<HMUI::SimpleTextDropdown*>(true);
        dropdown->get_gameObject()->SetActive(false);
        gameObject->SetActive(false);
        
        dropdown->set_name("Dropdown");
        dropdown->get_gameObject()->GetComponentInChildren<VRUIControls::VRGraphicRaycaster*>(true)->physicsRaycaster = Helpers::GetPhysicsRaycasterWithCache();
        dropdown->get_gameObject()->GetComponentInChildren<HMUI::ModalView*>(true)->container = Helpers::GetDiContainer();
        dropdown->get_gameObject()->GetComponentInChildren<HMUI::ScrollView*>(true)->platformHelper = Helpers::GetIVRPlatformHelper();

        auto labelObject = transform->Find("Label")->get_gameObject();
        Object::Destroy(labelObject->GetComponent<Polyglot::LocalizedTextMeshProUGUI*>());

        auto textMesh = labelObject->GetComponent<HMUI::CurvedTextMeshPro*>();
        textMesh->set_text("BSMLDropdownSetting");
        externalComponents->Add(textMesh);

        auto layoutElement = gameObject->AddComponent<LayoutElement*>();
        layoutElement->set_preferredHeight(8.0f);
        layoutElement->set_preferredWidth(90.0f);
        externalComponents->Add(layoutElement);

        auto dropdownListSetting = dropdown->get_gameObject()->AddComponent<BSML::DropdownListSetting*>();
        dropdownListSetting->dropdown = dropdown;
        externalComponents->Add(dropdownListSetting);
        
        dropdown->get_gameObject()->SetActive(true);
        gameObject->SetActive(true);
        return gameObject;
    }

    void DropdownListSettingTag::parse(const tinyxml2::XMLElement& elem) {
        DEBUG("Parsing dropdown list setting tag");
        this->::BSML::BSMLTag::parse(elem);
    }
}