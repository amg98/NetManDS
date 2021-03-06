/**
 * @file GuiController.cpp
 * @brief GUI Controller interface
 */

// Includes C/C++
#include <unordered_map>

// Own includes
#include "controller/GuiController.h"
#include "controller/MainController.h"
#include "controller/MenuController.h"
#include "controller/MenuTopController.h"
#include "controller/SshController.h"
#include "controller/CreditsController.h"
#include "controller/OptionsController.h"
#include "controller/AddUserController.h"
#include "controller/SetSMIController.h"
#include "controller/SshConsoleController.h"
#include "controller/LogsController.h"
#include "controller/ViewLogController.h"
#include "controller/SnmpController.h"
#include "controller/AgentDiscoveryController.h"
#include "controller/AgentViewController.h"
#include "controller/MibBrowserController.h"
#include "controller/SnmpParamsController.h"
#include "controller/SendSnmpController.h"
#include "controller/LoadYinController.h"
#include "controller/SnmpTypeController.h"
#include "controller/GetBulkController.h"
#include "controller/SnmpResultController.h"
#include "controller/SnmpTableController.h"
#include "controller/YinBrowserController.h"
#include "controller/RestConfOpController.h"
#include "controller/RestConfListController.h"
#include "controller/RestConfKeyController.h"

namespace NetMan {

template<typename T> static GuiController *bakeController() { return new T; }

// Map to store controllers
const static std::unordered_map<std::string, GuiController*(*)()> controllerFactory = {
    {"MainController", &bakeController<MainController>},
    {"MenuController", &bakeController<MenuController>},
    {"MenuTopController", &bakeController<MenuTopController>},
    {"SshController", &bakeController<SshController>},
    {"CreditsController", &bakeController<CreditsController>},
    {"OptionsController", &bakeController<OptionsController>},
    {"AddUserController", &bakeController<AddUserController>},
    {"SetSMIController", &bakeController<SetSMIController>},
    {"SshConsoleController", &bakeController<SshConsoleController>},
    {"LogsController", &bakeController<LogsController>},
    {"ViewLogController", &bakeController<ViewLogController>},
    {"SnmpController", &bakeController<SnmpController>},
    {"AgentDiscoveryController", &bakeController<AgentDiscoveryController>},
    {"AgentViewController", &bakeController<AgentViewController>},
    {"MibBrowserController", &bakeController<MibBrowserController>},
    {"SnmpParamsController", &bakeController<SnmpParamsController>},
    {"SendSnmpController", &bakeController<SendSnmpController>},
    {"LoadYinController", &bakeController<LoadYinController>},
    {"SnmpTypeController", &bakeController<SnmpTypeController>},
    {"GetBulkController", &bakeController<GetBulkController>},
    {"SnmpResultController", &bakeController<SnmpResultController>},
    {"SnmpTableController", &bakeController<SnmpTableController>},
    {"YinBrowserController", &bakeController<YinBrowserController>},
    {"RestConfOpController", &bakeController<RestConfOpController>},
    {"RestConfListController", &bakeController<RestConfListController>},
    {"RestConfKeyController", &bakeController<RestConfKeyController>},
};

/**
 * @brief Create a controller instance
 * @param className     Controller class name
 * @return The newly created controller
 */
std::shared_ptr<GuiController> GuiController::createController(const std::string &className) {
    auto it = controllerFactory.find(className);
    if(it == controllerFactory.end()) {
        throw std::runtime_error("Controller " + className + " not found");
    }

    try {
        return std::shared_ptr<GuiController>(it->second());
    } catch (const std::bad_alloc &e) {
        throw;
    } catch (const std::runtime_error &e) {
        throw;
    }
}

/**
 * @brief Call a method from the controller
 * @param method    Method to call
 * @param args      Arguments to the method
 */
void GuiController::callMethod(const std::string &method, void *args) {
    auto it = cbMap.find(method);
    if(it != cbMap.end()) {
        it->second(args);
    }
}

}
