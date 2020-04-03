/**
 * @file GuiLayout.h
 * @brief GUI layout
 */

#ifndef _GUILAYOUT_H_
#define _GUILAYOUT_H_

// Includes C/C++
#include <string>
#include <vector>
#include <memory>

// Includes 3DS
#include <3ds/types.h>

// Own includes
#include "GuiView.h"

namespace NetMan {

class GuiLayout {
    private:
        std::vector<std::shared_ptr<GuiView>> views;
	public:
        GuiLayout(const std::string &path);
        virtual ~GuiLayout();
		void draw();
};

}

#endif
