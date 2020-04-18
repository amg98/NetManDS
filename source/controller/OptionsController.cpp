/**
 * @file OptionsController.cpp
 */

// Own includes
#include "controller/OptionsController.h"
#include "Application.h"
#include "gui/ListView.h"
#include "gui/TextView.h"
#include "gui/ImageView.h"
#include "gui/EditTextView.h"
#include "gui/CheckboxView.h"
#include "gui/BinaryButtonView.h"
#include "snmp/Snmpv3UserStore.h"
#include "Config.h"

// Defines
#define ICON_SIZE           81.0f
#define TEXT_OFFX           10.0f
#define TEXT_SCALE          0.8f
#define CROSS_OFF           12.0f
#define SCREEN_SNMPV3_USERS 2

namespace NetMan {

static void gotoMenu(void *args) {
    Application::getInstance().requestLayoutChange("menu");
    Config::getInstance().save();
}

static void goAddUser(void *args) {
    Application::getInstance().requestLayoutChange("adduser");
}

static void fillUsers(void *args) {

    ListViewFillParams *params = (ListViewFillParams*)args;
    params->layouts.clear();
    auto controller = std::static_pointer_cast<OptionsController>(params->controller);
    controller->clearCrosses();
    controller->clearTexts();

    auto& users = Snmpv3UserStore::getInstance().getUserTable();

    if(params->endElement >= users.size()) {
        params->remaining = false;
    }

    auto userit = users.begin();
    for(u32 i = 0; i < params->startElement; i++, userit++);

    for(u32 i = params->startElement; i < params->endElement; i++) {
        if(i < users.size()) {
            float y = params->startY + (i % params->maxElements) * params->elementHeight;
            std::shared_ptr<ImageView> bg = std::make_shared<ImageView>("menuButton", params->startX + params->elementWidth / 2, y + params->elementHeight / 2, params->elementWidth / ICON_SIZE, params->elementHeight / ICON_SIZE);
            std::shared_ptr<TextView> tv = std::make_shared<TextView>(userit->first, params->startX + TEXT_OFFX, y, TEXT_SCALE, C2D_Color32(0, 0, 0, 0xFF));
            std::shared_ptr<ImageView> cross = std::make_shared<ImageView>("cross", params->startX + params->elementWidth - CROSS_OFF, y + params->elementHeight - CROSS_OFF, 0.5f, 0.5f);
            std::shared_ptr<GuiLayout> layout = std::make_shared<GuiLayout>();
            controller->addCross(cross);
            controller->addText(tv);
            layout->addView(bg);
            layout->addView(tv);
            layout->addView(cross);
            params->layouts.push_back(layout);
            userit ++;
        } else {
            i = params->endElement;
            params->remaining = false;
        }
    }
}

static void clickUser(void *args) {
    ListViewClickParams *params = (ListViewClickParams*)args;
    auto controller = std::static_pointer_cast<OptionsController>(params->controller);
    u32 down = Application::getInstance().getDown();
    u32 index = params->element - params->startElement;
    touchPosition &touch = Application::getInstance().getTouch();

    if(controller->getCrosses().at(index)->touched(down, touch)) {
        auto& store = Snmpv3UserStore::getInstance();
        store.removeUser(controller->getTexts().at(index)->getText());
        store.save();
        std::shared_ptr<u32> contextData = std::make_shared<u32>(SCREEN_SNMPV3_USERS);
        Application::getInstance().requestLayoutChange("options", contextData);
    } else {
        std::shared_ptr<std::string> context = std::make_shared<std::string>(controller->getTexts().at(index)->getText());
        Application::getInstance().requestLayoutChange("viewuser", context);
    }
}

static void setPort(EditTextParams *params, u16 *dest) {
    if(!params->init) {
        sprintf(params->text, "%d", *dest);
        params->init = true;
    } else {
        int port = strtol(params->text, NULL, 10);
        if(port == 0 || port > 0xFFFF) {
            params->init = false;
            Application::getInstance().messageBox("Invalid port");
        } else {
            *dest = (u16)port;
        }
    }
}

static void setInteger(EditTextParams *params, u32 *dest, u32 limit) {
    if(!params->init) {
        sprintf(params->text, "%ld", *dest);
        params->init = true;
    } else {
        int n = strtol(params->text, NULL, 10);
        if(n == 0 || (u32)n > limit) {
            params->init = false;
            Application::getInstance().messageBox("Invalid number");
        } else {
            *dest = (u32)n;
        }
    }
}

static void editSnmpPort(void *args) {
    setPort((EditTextParams*)args, &Config::getInstance().getData().snmpPort);
}

static void editTrapv1(void *args) {
    setPort((EditTextParams*)args, &Config::getInstance().getData().trapv1Port);
}

static void editTrapv1Bool(void *args) {
    CheckboxParams *params = (CheckboxParams*)args;
    if(!params->init) {
        params->state = Config::getInstance().getData().trapv1Enabled;
        params->init = true;
    } else {
        Config::getInstance().getData().trapv1Enabled = params->state;
    }
}

static void editTrapv2(void *args) {
    setPort((EditTextParams*)args, &Config::getInstance().getData().trapv2Port);
}

static void editTrapv2Bool(void *args) {
    CheckboxParams *params = (CheckboxParams*)args;
    if(!params->init) {
        params->state = Config::getInstance().getData().trapv2Enabled;
        params->init = true;
    } else {
        Config::getInstance().getData().trapv2Enabled = params->state;
    }
}

static void editTrapLimit(void *args) {
    setInteger((EditTextParams*)args, &Config::getInstance().getData().trapLimit, 999);
}

static void setSMI(void *args) {
    Application::getInstance().requestLayoutChange("setsmi");
}

static void editEngineID(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) {
        sprintf(params->text, Config::getInstance().getEngineID().c_str());
        params->init = true;
    } else {
        Config::getInstance().getEngineID().assign(params->text);
    }
}

static void editContextName(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) {
        sprintf(params->text, Config::getInstance().getContextName().c_str());
        params->init = true;
    } else {
        Config::getInstance().getContextName().assign(params->text);
    }
}

static void editTrapv3(void *args) {
    setPort((EditTextParams*)args, &Config::getInstance().getData().trapv3Port);
}

static void editTrapv3Bool(void *args) {
    CheckboxParams *params = (CheckboxParams*)args;
    if(!params->init) {
        params->state = Config::getInstance().getData().trapv3Enabled;
        params->init = true;
    } else {
        Config::getInstance().getData().trapv3Enabled = params->state;
    }
}

static void editSyslogPort(void *args) {
    setPort((EditTextParams*)args, &Config::getInstance().getData().syslogPort);
}

static void editSyslogTransport(void *args) {
    BinaryButtonParams *params = (BinaryButtonParams*)args;
    if(!params->init) {
        params->selected = Config::getInstance().getData().syslogTransport;
        params->init = true;
    } else {
        Config::getInstance().getData().syslogTransport = params->selected;
    }
}

static void editLogLimit(void *args) {
    setInteger((EditTextParams*)args, &Config::getInstance().getData().syslogLimit, 999);
}

static void rcTimeout(void *args) {
    setInteger((EditTextParams*)args, &Config::getInstance().getData().rcTimeout, 999);
}

static void rcURL(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) {
        sprintf(params->text, Config::getInstance().getRestConfURL().c_str());
        params->init = true;
    } else {
        Config::getInstance().getRestConfURL().assign(params->text);
    }
}

static void rcUsername(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) {
        sprintf(params->text, Config::getInstance().getRestConfUser().c_str());
        params->init = true;
    } else {
        Config::getInstance().getRestConfUser().assign(params->text);
    }
}

static void rcPassword(void *args) {
    EditTextParams *params = (EditTextParams*)args;
    if(!params->init) {
        sprintf(params->text, Config::getInstance().getRestConfPassword().c_str());
        params->init = true;
    } else {
        Config::getInstance().getRestConfPassword().assign(params->text);
    }
}

static void tcpTimeout(void *args) {
    setInteger((EditTextParams*)args, &Config::getInstance().getData().tcpTimeout, 999);
}

static void udpTimeout(void *args) {
    setInteger((EditTextParams*)args, &Config::getInstance().getData().udpTimeout, 999);
}

static void setScreen(void *args) {
    u32 *context = (u32*)Application::getInstance().getContextData().get();
    if(context != NULL) {
        *(u32*)args = *context;
    } else {
        *(u32*)args = 0;
    }
}

OptionsController::OptionsController() {
    this->crosses = std::vector<std::shared_ptr<ImageView>>();
    this->texts = std::vector<std::shared_ptr<TextView>>();

    this->cbMap = std::unordered_map<std::string, void(*)(void*)> {
        {"gotoMenu", gotoMenu},
        {"goAddUser", goAddUser},
        {"fillUsers", fillUsers},
        {"clickUser", clickUser},
        {"editSnmpPort", editSnmpPort},
        {"editTrapv1", editTrapv1},
        {"editTrapv1Bool", editTrapv1Bool},
        {"editTrapv2", editTrapv2},
        {"editTrapv2Bool", editTrapv2Bool},
        {"editTrapv3", editTrapv3},
        {"editTrapv3Bool", editTrapv3Bool},
        {"editTrapLimit", editTrapLimit},
        {"setSMI", setSMI},
        {"editEngineID", editEngineID},
        {"editContextName", editContextName},
        {"editSyslogPort", editSyslogPort},
        {"editSyslogTransport", editSyslogTransport},
        {"editLogLimit", editLogLimit},
        {"rcTimeout", rcTimeout},
        {"rcURL", rcURL},
        {"rcUsername", rcUsername},
        {"rcPassword", rcPassword},
        {"tcpTimeout", tcpTimeout},
        {"udpTimeout", udpTimeout},
        {"setScreen", setScreen},
    };
}

OptionsController::~OptionsController() { }

}
