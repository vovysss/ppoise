#ifndef USER_SESSION_H
#define USER_SESSION_H

#include "UserAccount.h"
#include <string>
#include <iostream>

namespace software {

class UserSession {
private:
    std::string sessionId;
    std::string startTime;
    bool isActive;
    UserAccount* account;

public:
    UserSession(std::string id, std::string startedAt, UserAccount* user)
        : sessionId(id), startTime(startedAt), isActive(false), account(user) {}

    void startSession() {
        if (account == nullptr) {
            std::cout << "Нельзя начать сессию без учетной записи.\n";
            return;
        }
        isActive = true;
        std::cout << "Сессия " << sessionId << " начата в " << startTime << ".\n";
    }

    void endSession() {
        isActive = false;
        std::cout << "Сессия " << sessionId << " завершена.\n";
    }

    bool validate() const {
        return isActive && account != nullptr;
    }

    void logActivity(const std::string& action) {
        std::cout << "Сессия " << sessionId << ": " << action << ".\n";
    }
};

} // namespace software

#endif
