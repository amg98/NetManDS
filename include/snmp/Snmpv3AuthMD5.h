/**
 * @file Snmpv3AuthMD5.h
 * @brief SNMPv3 Authentication using MD5
 */

#ifndef _SNMPV3_AUTH_MD5_H_
#define _SNMPV3_AUTH_MD5_H_

// Includes C/C++
#include <string>
#include <memory>

// Includes 3DS
#include <3ds.h>

// Own includes
#include "Snmpv3Pdu.h"

namespace NetMan {

/**
 * @class Snmpv3AuthMD5
 */
class Snmpv3AuthMD5 {
    public:
        Snmpv3AuthMD5();
        virtual ~Snmpv3AuthMD5();
        void createHash(std::unique_ptr<u8> data, const std::string &password, Snmpv3SecurityParams &params);
        bool authenticate(std::unique_ptr<u8> data, const std::string &password, Snmpv3SecurityParams &params);
};

}

#endif