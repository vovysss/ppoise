#ifndef USER_ACCOUNT_H
#define USER_ACCOUNT_H

#include "../exceptions/AuthenticationException.h"
#include <string>
#include <iostream>

namespace software {

class UserAccount {
private:
    std::string username;
    std::string passwordHash;
    std::string role;
    bool isLocked;
    int failedAttempts;

public:
    UserAccount(std::string name, std::string password, std::string userRole)
        : username(name), passwordHash(password), role(userRole),
          isLocked(false), failedAttempts(0) {}

    void authenticate(const std::string& password) {
        if (isLocked) {
            throw exceptions::AuthenticationException(
                "учетная запись " + username + " заблокирована");
        }
        if (password == passwordHash) {
            failedAttempts = 0;
            std::cout << "Пользователь " << username << " успешно вошел в систему.\n";
            return;
        }
        ++failedAttempts;
        if (failedAttempts >= 3) {
            isLocked = true;
            throw exceptions::AuthenticationException(
                "превышено число попыток для " + username);
        }
        std::cout << "Неверный пароль для " << username << ". Осталось попыток: "
                  << (3 - failedAttempts) << ".\n";
    }

    void changePassword(const std::string& oldPassword, const std::string& newPassword) {
        if (oldPassword != passwordHash) {
            throw exceptions::AuthenticationException(
                "неверный текущий пароль для " + username);
        }
        if (newPassword.size() < 6) {
            throw exceptions::AuthenticationException(
                "новый пароль слишком короткий для " + username);
        }
        passwordHash = newPassword;
        std::cout << "Пароль пользователя " << username << " изменен.\n";
    }

    void lockAccount() {
        isLocked = true;
        std::cout << "Учетная запись " << username << " заблокирована.\n";
    }

    void unlockAccount() {
        isLocked = false;
        failedAttempts = 0;
        std::cout << "Учетная запись " << username << " разблокирована.\n";
    }

    void resetFailedAttempts() {
        failedAttempts = 0;
        std::cout << "Счетчик неудачных попыток для " << username << " сброшен.\n";
    }
};

} // namespace software

#endif
