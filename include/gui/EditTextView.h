/**
 * @file EditTextView.h
 * @brief View for editable text
 */

#ifndef _EDITTEXT_VIEW_H_
#define _EDITTEXT_VIEW_H_

// Includes C/C++
#include <memory>

// Includes 3DS
#include <citro2d.h>

// Includes tinyxml2
#include <tinyxml2.h>

// Own includes
#include "GuiView.h"
#include "controller/GuiController.h"

// Defines
#define DEFAULT_FONT_SIZE   25.0f

namespace NetMan {

/**
 * @class EditTextView
 */
class EditTextView : public GuiView {
    protected:
        C2D_TextBuf textBuffer;
        C2D_Text text;
        std::shared_ptr<GuiController> controller;
        float x, y, width, height;
        float textScale;
        u32 textColor;
        u16 textLength;
        bool numeric;
        bool password;
        std::string onEdit;
    public:
        EditTextView(tinyxml2::XMLElement *node, std::shared_ptr<GuiController> controller);
        virtual void input(u32 held, u32 down, u32 up, touchPosition &touch) override;
        virtual void draw() override;
        virtual ~EditTextView();
};

}

#endif
