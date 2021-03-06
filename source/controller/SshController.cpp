/**
 * @file SshController.cpp
 */

// Own includes
#include "controller/SshController.h"
#include "gui/EditTextView.h"
#include "gui/ButtonView.h"
#include "ssh/SshHelper.h"
#include "Application.h"
#include "Utils.h"

namespace NetMan {

/**
 * @brief Go to the main menu
 */
static void goMenu(void *args) {
    Application::getInstance().requestLayoutChange("menu");
}

/**
 * @brief Edit the SSH server's host name
 */
static void editHost(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) return;
    auto controller = std::static_pointer_cast<SshController>(params->controller);
    controller->getSession().host.assign(params->text);
}

/**
 * @brief Edit the SSH port
 */
static void editPort(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    u16 port = Utils::handleFormPort(params, SSH_PORT);
    if(port > 0) {
        auto controller = std::static_pointer_cast<SshController>(params->controller);
        controller->getSession().port = port;
    }
}

/**
 * @brief Edit the user name to be used for authentication
 */
static void editUsername(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) return;
    auto controller = std::static_pointer_cast<SshController>(params->controller);
    controller->getSession().username.assign(params->text);
}

/**
 * @brief Edit the password to be used for authentication
 */
static void editPassword(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) return;
    auto controller = std::static_pointer_cast<SshController>(params->controller);
    controller->getSession().password.assign(params->text);
}

/**
 * @brief Connect to the SSH server
 */
static void connect(void *args) {
    ButtonParams *params = (ButtonParams*)args;
    auto controller = std::static_pointer_cast<SshController>(params->controller);

    try {
        std::shared_ptr<SshHelper> sshHelper = std::make_shared<SshHelper>();
        SshSession &session = controller->getSession();
        sshHelper->connect(session.host, session.port, session.username, session.password);
        Application::getInstance().requestLayoutChange("sshconsole", sshHelper);
    } catch (const std::runtime_error &e) {
        Application::getInstance().messageBox(e.what());
    }
}

/**
 * @brief Constructor for a SshController
 */
SshController::SshController() {
    this->cbMap = std::unordered_map<std::string, void(*)(void*)> {
        {"goMenu", goMenu},
        {"editHost", editHost},
        {"editPort", editPort},
        {"editUsername", editUsername},
        {"editPassword", editPassword},
        {"connect", connect},
    };
}

/**
 * @brief Destructor for a SshController
 */
SshController::~SshController() { }

}
